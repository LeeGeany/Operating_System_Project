/**********************************************************************
 * Copyright (c) 2020
 *  Sang-Hoon Kim <sanghoonkim@ajou.ac.kr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTIABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 **********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

char* name;

void signal_handler(int signo)
{
	fprintf(stderr, "%s is timed out\n", name);
	kill(getpid(), SIGKILL);
	alarm(0);
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler = signal_handler;
	sigfillset(&act.sa_mask);
	act.sa_flags = 0;

	name = argv[0];
	
	fprintf(stderr, "pid  = %d\n", getpid());
	fprintf(stderr, "argc = %d\n", argc);

	for (int i = 0; i < argc; i++) {
		fprintf(stderr, "argv[%d] = %s\n", i, argv[i]);
	}
	
	sigaction(SIGALRM, &act, NULL);
	
	if (argc >= 3 && strncmp(argv[1], "sleep", strlen("sleep")) == 0) {
		int sleep_sec = sleep_sec = atoi(argv[2]);
		sleep(sleep_sec);
	}
	fprintf(stderr, "done!\n");

	return 0;
}
