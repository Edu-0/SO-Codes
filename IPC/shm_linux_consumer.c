/**
 * Consumidor - Memória compartilhada - Linux
 * Para compilar
 *	gcc shm-posix-consumer.c -lrt
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(){
	const char *name = "OS";
	const int SIZE = 4096;

	int shm_fd;
	void *ptr;
	int i;

	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDONLY, 0666); // O_RDONLY: Open the shared memory object for reading only. This means that the consumer process will only be able to read data from the shared memory, and it will not have permission to write to it. The producer process, on the other hand, can open the shared memory object with O_RDWR to allow both reading and writing.
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0); // PROT_READ: This flag indicates that the memory region should be mapped with read-only permissions. This means that the consumer process will only be able to read data from the shared memory, and it will not have permission to write to it. The producer process can map the shared memory region with PROT_READ | PROT_WRITE to allow both reading and writing.
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	/* now read from the shared memory region */
	printf("%s",(char *)ptr); // The consumer process reads the data from the shared memory region by dereferencing the pointer ptr. Since ptr is a void pointer, we need to cast it to a char pointer (char *) to read it as a string. The printf function then prints the contents of the shared memory, which should be the messages written by the producer process.

	/* remove the shared memory segment */
	if (shm_unlink(name) == -1) { // shm_unlink() is used to remove the shared memory object from the system. It takes the name of the shared memory object as an argument. If the function returns -1, it indicates that there was an error in removing the shared memory object, and we print an error message and exit with a non-zero status.
		printf("Error removing %s\n",name);
		exit(-1);
	}

	/*
		shm_unlink irá deletar todo aquele espaço de memória compartilhado que foi criado quando mais nenhum processo estiver usando ele. Se um processo ainda estiver usando o shared memory, ele ainda poderá acessar os dados.
		Quando todos os processos fecham e shm_unlink é chamado, tudo é destruído.
	*/

	return 0;
}

/*
	Primeiramente eu rodo o producer, cria o seguimento de memória compartilhado, escreve as mensagens e depois rodo o consumer, que lê as mensagens do segmento de memória compartilhado e depois de ler, o consumer chama shm_unlink() para remover o segmento de memória.
*/