#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <errno.h>

int mygetline(char **lineptr, size_t *n, FILE *stream);

int mygetline(char **lineptr, size_t *n, FILE *stream)
{
	static char line[256];
	char *ptr;
	unsigned int len;

   if (lineptr == NULL || n == NULL)
   {
      errno = EINVAL;
      return -1;
   }

   if (ferror (stream))
      return -1;

   if (feof(stream))
      return -1;
     
   fgets(line,256,stream);

   ptr = strchr(line,'\n');   
   if (ptr)
      *ptr = '\0';

   len = strlen(line);
   
   if ((len+1) < 256)
   {
      ptr = realloc(*lineptr, 256);
      if (ptr == NULL)
         return(-1);
      *lineptr = ptr;
      *n = 256;
   }

   strcpy(*lineptr,line); 
   return(len);
}

// ssize_t mygetline(char **lineptr, size_t *n, FILE *stream){
// 	int ch;
// 	ssize_t count = 0;
// 	size_t i = 0;
// 	char *ptr;

// 	*lineptr = (char *)malloc(120);

// 	while(((ch = fgetc(stream)) != EOF) && (ch != 10)){
// 		if(count > 120){
// 			realloc(*lineptr, 120*i);
// 			i++;

// 		}

// 		count++;		
// 	}

// 	if(ch == EOF)
// 		return -1;


// 	*n = 120 * (1 + i);
// 	return count;
// }

int main()
{
	FILE *fp;
	char *line = NULL;
	size_t size = 0;
	ssize_t read;

	fp = fopen("t.c", "r");
	if(fp == NULL)
	{
		perror("Error: ");
		exit(1);
	}

	while((read = getline(&line, &size, fp)) != -1){
		printf("getline %zu \n", read);
		printf("string = %s\n", line);
		printf("size = %d\n\n", size);
	}

	if(line)
		free(line);
	exit(0);
}