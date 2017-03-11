#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>

int main(int argc, char **argv)
{
	char *input_pass;
	char *test_pass;
	struct spwd *shadowline;


	if(argc < 2)
	{
		fprintf(stderr, "Usage:  %s <username>\n", argv[0]);
		exit(1);
	}

	if((shadowline = getspnam(argv[1])) == NULL)
	{
		fprintf(stderr, "User <%s> don't exist \n", argv[1]);
		exit(1);
	}

	if((input_pass = getpass("Password: ")) == NULL)
	{
		perror("getpass: ");
		exit(1);
	}

	test_pass = crypt(input_pass, shadowline->sp_pwdp);
	if(strcmp(test_pass, shadowline->sp_pwdp) == 0)
		puts("Pass");
	else
		puts("Error");

	exit(0);
}
