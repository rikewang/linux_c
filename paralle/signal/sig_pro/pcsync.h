#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t flag;
static 	sigset_t blockset, originset, zeroset;

void handler(int s);
void TELL_WAIT(void);
void TELL_PARENT(pid_t pid);
void TELL_CHILD(pid_t pid);
void WAIT_PARENT(void);
void WAIT_CHILD(void);

void handler(int s)
{
	flag = 1;
}

void TELL_WAIT(void)
{
	sigemptyset(&zeroset);
	sigemptyset(&blockset);

	sigaddset(&blockset, SIGUSR1);
	sigaddset(&blockset, SIGUSR2);

	signal(SIGUSR1, handler);
	signal(SIGUSR2, handler);

	sigprocmask(SIG_BLOCK, &blockset, &originset);
}

void TELL_PARENT(pid_t pid)
{
	kill(pid, SIGUSR1);
}

void WAIT_PARENT(void)
{
	while(flag == 0)
		sigsuspend(&zeroset);
	flag = 0;

	sigprocmask(SIG_SETMASK, &originset, NULL);
}

void TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR2);
}

void WAIT_CHILD(void)
{
	while(flag == 0)
		sigsuspend(&zeroset);
	flag = 0;

	sigprocmask(SIG_SETMASK, &originset, NULL);
}
