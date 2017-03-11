#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <glob.h>

#define DELIMITER	" \t\n"

struct cmd_st{
	glob_t globrst;
};


void prompt(void);
static void parse(char **line, struct cmd_st *cmd);

static void parse(char **line, struct cmd_st *cmd)
{
	int flag = 0;
	char *arg;

	while((arg = strsep(line, DELIMITER)) != NULL)
	{
		if(!arg[0])
			continue;
		glob(arg, GLOB_NOCHECK|GLOB_APPEND*flag, NULL, &(cmd->globrst));
		flag = 1;
	}

	return;
}

void prompt(void)
{
	printf("mysh-0.1$ ");
}


int main()
{
	pid_t pid;
	char *lineptr;
	size_t length;
	struct cmd_st cmd;
	ssize_t read_cnt;

	while(1)
	{
		prompt();

		lineptr = NULL;
		length = 0;


		if((read_cnt = getline(&lineptr, &length, stdin)) < 0)
		{
			perror("getline(): ");
			exit(1);
		}


		if(read_cnt == 1)
			continue;

		parse(&lineptr, &cmd);

		pid = fork();

		if(pid < 0)
		{
			perror("fork(): ");
			exit(1);
		}

		if(pid == 0)
		{
			execvp(cmd.globrst.gl_pathv[0], cmd.globrst.gl_pathv);
			perror("execvp(): ");
			exit(1);
		}

		wait(NULL);

		if(lineptr)
			free(lineptr);
	}

	exit(0);
}