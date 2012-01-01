/*
 * signaltest.c
 *
 *  Created on: Oct 14, 2011
 *      Author: john
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

void sigtstp_handler(int);
void sigcont_handler(int);

void sigtstp_handler(int signum) {
	printf("Caught SIGTSTP (signum = %d)\n", signum);
	signal (signum, SIG_DFL);
	raise (signum);
}

void sigcont_handler(int signum) {
	printf("Caught SIGCONT (signum = %d)\n", signum);
	signal (signum, SIG_DFL);
	raise (signum);
}

int main(int argc, char* argv[]) {
	int r;

	r = signal(SIGTSTP, sigtstp_handler);
	printf("registering SIGTSTP returns: %d\n", r);

	r = signal(SIGCONT, sigcont_handler);
	printf("registering SIGCONT returns: %d\n", r);

	/*if (r == SIG_IGN) {
		printf("dentro de if\n");
		signal(SIGHUP, SIG_IGN);
	}*/
	/**
	 * Sleep returns prematurely on a signal. I.e. we sleep for 60 seconds and, when user hists C-z the signal handler
	 * is called (correct) but the sleep returns immediatelly, terminating the program.
	 */
	//sleep (60);
	struct timeval t;
	t.tv_sec = 10;
	for (;;) {
		printf("sleeping for %d seconds ... \n", (int) t.tv_sec);
		fflush(stdin);
		r = select(0, NULL, NULL, NULL, &t);
		printf("woken up with %d (%d)\n", r, errno);
	}
	printf("bye\n");
	return 0;
}
