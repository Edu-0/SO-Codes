#include <stdio.h>
#include <unistd.h>

int main()
{
	fork();

	fork();

	fork();
	
	while(1); /* infinite loop to keep the processes alive */

	return 0;
}