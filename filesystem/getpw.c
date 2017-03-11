#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

int main()
{
	struct passwd *pwrst;

	pwrst = getpwnam("wxl");

	puts("--------- wxl -----------");
	printf("name = %s, passwd = %s, \nuid = %d, gid = %d, realname = %s,\nhome_dir = %s, shell = %s\n", pwrst->pw_name, pwrst->pw_passwd, pwrst->pw_uid, pwrst->pw_gid, pwrst->pw_gecos, pwrst->pw_dir, pwrst->pw_shell);

	puts("--------- root -----------");

	pwrst = getpwuid(0);
	printf("name = %s, passwd = %s, \nuid = %d, gid = %d, realname = %s,\nhome_dir = %s, shell = %s\n", pwrst->pw_name, pwrst->pw_passwd, pwrst->pw_uid, pwrst->pw_gid, pwrst->pw_gecos, pwrst->pw_dir, pwrst->pw_shell);

	exit(0);	

}