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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "types.h"
#include "parser.h"

static int pid2 = 0;

/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING FROM THIS LINE ******       */
/**
 * String used as the prompt (see @main()). You may change this to
 * change the prompt */
static char __prompt[MAX_TOKEN_LEN] = "$";

/**
 * Time out value. It's OK to read this value, but ** SHOULD NOT CHANGE
 * IT DIRECTLY **. Instead, use @set_timeout() function below.
 */
static unsigned int __timeout = 2;

static void set_timeout(unsigned int timeout)
{
	__timeout = timeout;

	if (__timeout == 0) {
		fprintf(stderr, "Timeout is disabled\n");
	} else {
		fprintf(stderr, "Timeout is set to %d second%s\n",
				__timeout,
				__timeout >= 2 ? "s" : "");
	}
}
/*          ****** DO NOT MODIFY ANYTHING UP TO THIS LINE ******      */
/*====================================================================*/


/***********************************************************************
 * run_command()
 *
 * DESCRIPTION
 *   Implement the specified shell features here using the parsed
 *   command tokens.
 *
 * RETURN VALUE
 *   Return 1 on successful command execution
 *   Return 0 when user inputs "exit"
 *   Return <0 on error
 */
static int run_command(int nr_tokens, char *tokens[])
{
	/* This function is all yours. Good luck! */

	// exit running
	if (strncmp(tokens[0], "exit", strlen("exit")) == 0) 
	{
		return 0;
	}

	// prompt running
	else if (strncmp(tokens[0], "prompt", strlen("prompt")) == 0) 
	{
		strcpy(__prompt, tokens[1]);
	}
	
	// cd
	else if(strncmp(tokens[0], "cd", strlen("cd")) == 0)
	{
		if(strncmp(tokens[1], "~", strlen("~")) == 0)
		{
			char* home = getenv("HOME");
			chdir(home);
		}
		else 
		{
			int err = chdir(tokens[1]);
			if(err == -1) {printf("bash: cd: %s: No such file or directory\n", tokens[1]);}
		}
	}
	
	// /bin/pwd/, /bin/ls, /bin/echo/
	else if (strncmp(tokens[0], "/bin/", strlen("/bin/")) == 0) 
	{
		int pid = fork();
		if(pid == -1)
		{
			fprintf(stderr, "FORK FAILED\n");
		}
		else if(pid == 0)
		{
			execv(tokens[0], tokens);
			exit(0);
		}
		else
		{
			wait(NULL);
		}
	}

	// pwd, ls, echo
	else if (strncmp(tokens[0], "echo", strlen("echo")) == 0 ||
		strncmp(tokens[0], "ls", strlen("ls")) == 0 ||strncmp(tokens[0], "pwd", strlen("pwd")) == 0) 
	{
		int pid = fork();
		if(pid == -1)
		{
			fprintf(stderr, "FORK FAILED\n");
		}
		else if(pid == 0)
		{
			execvp(tokens[0], tokens);
			exit(0);
		}
		else
		{
			wait(NULL);
		}
	}

	// wc, touch, tar, sleep
	else if (strncmp(tokens[0], "wc", strlen("wc")) == 0 || strncmp(tokens[0], "touch", strlen("touch")) == 0
		||strncmp(tokens[0], "tar", strlen("tar")) == 0 || strncmp(tokens[0], "sleep", strlen("sleep")) == 0)
	{
		int pid = fork();
		if(pid == -1)
		{
			fprintf(stderr, "FORK FAILED\n");
		}
		else if(pid == 0)
		{
			execvp(tokens[0], tokens);
			exit(0);
		}
		else
		{
			wait(NULL);
		}
	}

	// RUNNING
	else if(strncmp(tokens[0], "./", strlen("./")) == 0)
	{
		int pid = fork();
		if(pid == -1){
			fprintf(stderr,"FORKED FAILED\n");
		}
		else if(pid == 0){
			alarm(__timeout);
			execve(tokens[0], tokens, NULL);
			exit(0);
		}
		else{	
			wait(0);
		}
	}

	// timeout & timeset
	else if(strncmp(tokens[0], "timeout", strlen("timeout")) == 0){
		if(nr_tokens == 1){	
			fprintf(stderr, "Current timeout is %d second\n", __timeout);}
		if(nr_tokens == 2){
			__timeout = atoi(tokens[1]);	
			fprintf(stderr, "Timeout is set to %d seconds\n", __timeout);
		}
	}

	//for
	else if(strncmp(tokens[0], "for", strlen("for")) == 0){
		
		int i = 0;
		int tm = 1;
		for(i = 0; i < nr_tokens; ++i)
		{
			if(strncmp(tokens[i], "for", strlen("for")) == 0)
			{				
				tm = tm * atoi(tokens[++i]);
			}
			else if(strncmp(tokens[i], "for", strlen("for")) == 1)
			{
				printf("break here\n");
				break;
			}
		}
		
		printf("%s\n", tokens[--i]);
		for(int j = 0; j< tm; ++j)
		{
			
			int pid1 = fork();
			if(pid1 == -1){ fprintf(stderr, "FORK FAILED\n"); }
			else if(pid1 == 0){
				execl(tokens[i], tokens[i], NULL);
				int pid2 = fork();
				if(pid2 == -1){fprintf(stderr, "FORK FAILED\n");}
				else if(pid2 == 0){

				}
				exit(0);
			}
			else{
				wait(NULL);
			}
		}
	}

	else {
		fprintf(stderr, "No such file or directory\n");
	}
	return 1;
}


/***********************************************************************
 * initialize()
 *
 * DESCRIPTION
 *   Call-back function for your own initialization code. It is OK to
 *   leave blank if you don't need any initialization.
 *
 * RETURN VALUE
 *   Return 0 on successful initialization.
 *   Return other value on error, which leads the program to exit.
 */
static int initialize(int argc, char * const argv[])
{
	return 0;
}


/***********************************************************************
 * finalize()
 *
 * DESCRIPTION
 *   Callback function for finalizing your code. Like @initialize(),
 *   you may leave this function blank.
 */
static void finalize(int argc, char * const argv[])
{

}


/*====================================================================*/
/*          ****** DO NOT MODIFY ANYTHING BELOW THIS LINE ******      */

static bool __verbose = true;
static char *__color_start = "[0;31;40m";
static char *__color_end = "[0m";

/***********************************************************************
 * main() of this program.
 */
int main(int argc, char * const argv[])
{
	char command[MAX_COMMAND_LEN] = { '\0' };
	int ret = 0;
	int opt;

	while ((opt = getopt(argc, argv, "qm")) != -1) {
		switch (opt) {
		case 'q':
			__verbose = false;
			break;
		case 'm':
			__color_start = __color_end = "\0";
			break;
		}
	}

	if ((ret = initialize(argc, argv))) return EXIT_FAILURE;

	if (__verbose)
		fprintf(stderr, "%s%s%s ", __color_start, __prompt, __color_end);

	while (fgets(command, sizeof(command), stdin)) {	
		char *tokens[MAX_NR_TOKENS] = { NULL };
		int nr_tokens = 0;

		if (parse_command(command, &nr_tokens, tokens) == 0)
			goto more; /* You may use nested if-than-else, however .. */

		ret = run_command(nr_tokens, tokens);
		if (ret == 0) {
			break;
		} else if (ret < 0) {
			fprintf(stderr, "Error in run_command: %d\n", ret);
		}

more:
		if (__verbose)
			fprintf(stderr, "%s%s%s ", __color_start, __prompt, __color_end);
	}

	finalize(argc, argv);

	return EXIT_SUCCESS;
}
