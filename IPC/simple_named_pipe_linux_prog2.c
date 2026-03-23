// C program to implement one side of FIFO
// This side reads first, then reads
//https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int fd1; // fd1 é o descritor de arquivo usado para acessar o FIFO. Ele é declarado como um inteiro e será usado para abrir, ler e escrever no FIFO.

	// FIFO file path
	char * myfifo = "/tmp/myfifo"; // O caminho do FIFO é definido como "/tmp/myfifo". Este é o local onde o FIFO será criado e acessado pelos processos de leitura e escrita.

	// Creating the named file(FIFO)
	// mkfifo(<pathname>,<permission>)
	mkfifo(myfifo, 0666);

	char str1[80], str2[80];
	while (1)
	{
		// First open in read only and read
		fd1 = open(myfifo,O_RDONLY);
		read(fd1, str1, 80);

		// Print the read string and close
		printf("User1: %s\n", str1);
		close(fd1);

		// Now open in write mode and write
		// string taken from user.
		fd1 = open(myfifo,O_WRONLY);
		fgets(str2, 80, stdin);
		write(fd1, str2, strlen(str2)+1);
		close(fd1);
	}
	return 0;
}
