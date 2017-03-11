/*************************************************************************
    > File Name: httpd.c
    > Author: rikewang 
    > Mail: 1138122262@qq.com 
    > Created Time: Thu 01 Dec 2016 08:33:23 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/wait.h>

#define BUFSIZE     1024
#define SERVER_STRING "rikewang web server 1.0\r\n"
#define ISspace(x) isspace((int)x)

int startup(int port);
void accept_request(void *sd);
void headers(int client, const char *filename);
void cat(int client, FILE *fp);
int get_line(int sd, char *buf, int size);
void serve_file(int client, const char *filename);
void execute_cgi(int client_sd, const char *path, const char *method, const char *query_string);
void bad_request(int client);
void unimplemented(int client);
void not_found(int client);
void cannot_execute(int client);

int main(int argc, char **argv)
{
    int sd;
    int port;
    int client_sd;
    int rst;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(struct sockaddr_in);
    pthread_t tid;

    if(argc != 2){
        fprintf(stderr, "Usage %s <port>\n", argv[0]);
        exit(1);
    }
    
    port = atoi(argv[1]);

    sd = startup(port);
    if(sd < 0){
        fprintf(stderr, "startup error\n");
        exit(1);
    }

    while(1)
    {
        client_sd = accept(sd, 
                           (struct sockaddr *)&client_addr,
                           &client_addr_len);
        if(client_sd < 0){
            perror("accept()");
            exit(1);
        }


        rst = pthread_create(&tid, NULL, accept_request, (void *)&client_sd);
        
        if(rst < 0){
            perror("pthread_create()");
            exit(1);
        }
    }

    close(sd);
}

int startup(int port)
{
    int sd;
    struct sockaddr_in server_addr;

    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("bind(): ");
        return -1;
    }
    
    if(listen(sd, 5) < 0){
        perror("listen():");
        return -1;
    }

    return sd;
}

void accept_request(void *sd)
{
    //printf("DEBUG: %s: %d\n",__FUNCTION__, __LINE__ );
    int client_sd = *(int *)sd;
    char buf[BUFSIZE];
    char method[255];
    char url[255];
    size_t i, j;
    struct stat st;
    int cgi = 0;
    int numchars;
    char path[255];

    char *query_string = NULL;


    numchars = get_line(client_sd, buf, sizeof(buf));
    i = 0; j = 0;

    printf("%s\n", buf);

    while(!ISspace(buf[j]) && (i < sizeof(buf) -1))
    {
        method[i] = buf[i];
        i++;
        j++;
    }
    method[i] = '\0';
    if(strcasecmp(method, "GET") && strcasecmp(method, "POST")){
        unimplemented(client_sd);
        return;
    }
    
    if(strcasecmp(method, "POST") == 0)
        cgi = 1;
    i = 0;
    while(ISspace(buf[j]) && (j < sizeof(buf)))
        j++;
    while(!ISspace(buf[j]) && (i < sizeof(url)-1) && (j < sizeof(buf)))
    {
        url[i] =  buf[j];
        i++;
        j++;
    }
    url[i] = '\0';
    
    if(strcasecmp(method, "GET") == 0){
        query_string = url;
        while((*query_string != '?') && (*query_string != '\0'))
            query_string++;
        if(*query_string == '?')
        {
            cgi = 1;
            *query_string = '\0';
            query_string++;
        }
    }

    sprintf(path, "htdocs%s", url);
    if(path[strlen(path) - 1] == '/')
        strcat(path, "index.html");
    if(stat(path, &st) == -1){
        while((numchars > 0) && strcmp("\n", buf))
            numchars = get_line(client_sd, buf, sizeof(buf));
        not_found(client_sd);
    } else {
        if((st.st_mode & S_IFMT) == S_IFDIR)
            strcat(path, "/index.html");
        else if ((st.st_mode & S_IFMT) == S_IFREG){
            if((st.st_mode & S_IXUSR) ||
            (st.st_mode & S_IXGRP) || 
            (st.st_mode & S_IXOTH))
                cgi = 1;
        }

        if(!cgi){
            serve_file(client_sd, path);
        }
        else 
            execute_cgi(client_sd, path, method, query_string);
    }

    close(client_sd);
}

void execute_cgi(int client_sd, const char *path, 
                 const char *method, const char *query_string)
{
    char buf[BUFSIZE];
    int cgi_output[2];
    int cgi_input[2];

    pid_t pid;

    int status;
    int i;
    char c;
    int numchars;
    int content_length;

    buf[0] = 'A';
    buf[1] = '\0';
    if(strcasecmp(method, "GET") == 0)
        while((numchars > 0) && strcmp("\n", buf))
            numchars = get_line(client_sd, buf, sizeof(buf));
    else{
        numchars = get_line(client_sd, buf, sizeof(buf));
        while((numchars > 0) && strcmp("\n", buf))
        {
            buf[15] = '\0';
            if(strcasecmp(buf, "Content-Length:") == 0)
                content_length = atoi(&(buf[16]));
            numchars = get_line(client_sd, buf, sizeof(buf));        
        }
        if(content_length == -1)
            bad_request(client_sd);
        return;
    }

    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    send(client_sd, buf, strlen(buf), 0);
    
    if((pipe(cgi_output)< 0) || (pipe(cgi_input) < 0)){
        cannot_execute(client_sd);
        return;       
    }

    pid = fork();

    if(pid < 0){
        cannot_execute(client_sd);
        return;
    }

    if(pid == 0)
    {
        char meth_env[255];
        char query_env[255];
        char length_env[255];

        dup2(cgi_output[1], 1);
        dup2(cgi_input[0], 0);
        close(cgi_output[0]);
        close(cgi_input[1]);

        sprintf(meth_env, "REQUEST_METHOD=%s", method);
        putenv(meth_env);
        if(strcasecmp(method, "GET") == 0)
        {
            sprintf(query_env, "QUERT_STRING=%s", query_string);
            putenv(query_env);
        } else {
            sprintf(length_env, "CONTENT_LENGTH=%d", content_length);
            putenv(length_env);
        }
        execl(path, path, NULL);
        exit(0);
    }
    
    else{
        close(cgi_output[1]);
        close(cgi_input[0]);
        if(strcasecmp(method, "POST") == 0)
            for(i = 0; i < content_length; i++){
                recv(client_sd, &c,  1, 0);
                write(cgi_input[1], &c, 1);
            }
        while(read(cgi_output[0], &c, 1) > 0)
            send(client_sd, &c, 1, 0);
        close(cgi_output[0]);
        close(cgi_input[1]);
        waitpid(pid, &status, 0);
    }  
}

void serve_file(int client, const char *filename)
{
    FILE *resource = NULL;
    int numchars = 1;
    char buf[BUFSIZE];

    buf[0] = 'A'; 
    buf[1] = '\0';


    while((numchars > 0) && strcmp("\n", buf)){
        numchars = get_line(client, buf, sizeof(buf));
    }
    resource = fopen(filename, "r");
    if(resource == NULL)
        not_found(client);
    else{
        headers(client, filename);
        cat(client, resource);
    }
    fclose(resource);
}

int get_line(int sd, char *buf, int size)
{
    int i = 0;
    char c;
    int n;

    while((i < size - 1) && (c != '\n')){
        n = recv(sd, &c, 1, 0);
        if(n > 0){
            if(c == '\r'){
                n = recv(sd, &c, 1, MSG_PEEK );
                if((n > 0) && (c == '\n'))
                    recv(sd, &c, 1, 0);
                else
                    c = '\n';
            }
            buf[i] = c;
            i++;
        } else {
            c = '\n';
        }

        buf[i] = '\0';
    }   

    return i;
}

void cat(int client, FILE *fp)
{
    char buf[BUFSIZE];

    fgets(buf, sizeof(buf), fp);
    while(!feof(fp)){
        send(client, buf, strlen(buf), 0);
        fgets(buf, sizeof(buf), fp);
    }
}

void headers(int client, const char *filename)
{
    char buf[BUFSIZE];

    strcpy(buf, "HTTP/1.0 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    strcpy(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

void bad_request(int client)
{
    char buf[BUFSIZE];

    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "<P> bad request without Content-Length header\r\n");
    send(client, buf, sizeof(buf), 0);
}

void unimplemented(int client)
{
    char buf[BUFSIZE];

    sprintf(buf, "HTTP/1.0 501 Method Not Implement\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<h1>method not supported<h1>\r\n");
    send(client, buf, strlen(buf), 0);
}

void not_found(int client)
{
    char buf[BUFSIZE];

    sprintf(buf, "HTTP/1.0 404 Not Found\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<h1> 404 not found</h1>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
}

void cannot_execute(int client)
{
    char buf[BUFSIZE];
    
    sprintf(buf, "HTTP/1.0 500 NOT FOUND\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<h1> 500 internal server error\r\n");
    send(client, buf, strlen(buf), 0);
}

