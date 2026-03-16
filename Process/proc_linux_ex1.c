#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int value = 5; // global variable

int main(){
	pid_t pid; // process ID

	pid = fork(); // create a child process
	
	if (pid == 0) { /* child process */
		value += 15; /* child process modifies the global variable */
		return 0;
	}
	else if (pid > 0) { /* parent process */
		wait(NULL); /* wait for the child process to finish */
		printf ("PARENT: value = %d\n",value); /* LINE A */
		return 0;
	}
}