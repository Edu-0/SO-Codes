/**
 * Produtor - Memória compartilhada - Linux
 * Para compilar
 * 	gcc shm-posix-producer.c -lrt
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h> // Shared memory segment control operations
#include <sys/stat.h> // File status and permission constants
#include <sys/mman.h> // Memory management declarations, including mmap() and related constants
#include <sys/types.h> // Data types used in system calls, such as pid_t, off_t, etc.

int main(){
	const int SIZE = 4096; // Bytes
	const char *name = "OS"; // Label, quando vou criar uma região de memória compartilhada, vou trabalhar com ponteiro, achar uma posição na memória é difícil e vou rotular para achar mais fácil. É alocado de maneira dinâmica e por isso n posso colocar números
	const char *message0= "Studying ";
	const char *message1= "Operating Systems ";
	const char *message2= "Is Fun!";

	int shm_fd; // File directory, crio um arquivo.
	void *ptr; // N vai ser estruturada na memória. Vai ser estruturado conforme retorno de função.

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	/*
		shm_open() creates a new shared memory object or opens an existing shared memory object.
		The name argument specifies the name of the shared memory object. It must begin with a slash
		O_CREAT: If the shared memory object does not exist, it will be created. If it already exists, this flag has no effect.
		O_RDWR: Open the shared memory object for reading and writing.
		0b00001000 OR 0b01010000 = 0b01011000

		0666: This sets the permissions for the shared memory object, allowing read and write. Usuário, público e grupo.
	*/

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE); // ftruncate() is used to set the size of the shared memory object. It takes two arguments: the file descriptor of the shared memory object and the desired size in bytes. In this case, we set the size to 4096 bytes (4 KB).

	/*
		Um cria/abre o objeto de memória compartilhada usando shm_open()
		Configura após o seu tamanho com ftruncate()
		Depois mapeia o segmento de memória compartilhada no espaço de endereçamento do processo usando mmap(). 
		O produtor então escreve mensagens na região de memória compartilhada, incrementando o ponteiro após cada escrita para garantir que as mensagens sejam armazenadas corretamente.
	*/


	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0); // ptr vai ser a referência para o início da região de memória compartilhada mapeada no espaço de endereçamento do processo.

	/*
		0 - The starting address for the mapping.
		SIZE - The max size of the mapping (4096 bytes).
		PROT_READ | PROT_WRITE - The desired memory protection of the mapping, which allows both reading and writing to the shared memory.
		MAP_SHARED - This flag indicates that updates to the mapping are visible to other processes that map the same shared memory object, and that changes are shared between processes.
		shm_fd - The file descriptor of the shared memory object obtained from shm_open().
		0 - The offset in the shared memory object where the mapping starts. In this case, it is set to 0, meaning the mapping starts at the beginning of the shared memory object.

		File Descriptor = Isso aqui vai ser uma referência no qual o SO vai usar para trabalhar com meus pedidos de separar um espaço para shared memory
	*/

	if (ptr == MAP_FAILED) { // MAP_FAILED é uma constante que indica que a função mmap() falhou ao mapear a memória compartilhada. Se ptr for igual a MAP_FAILED, significa que ocorreu um erro durante o mapeamento.
		printf("Map failed\n");
		return -1;
	}

	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
	sprintf(ptr,"%s",message0); // sprintf() is a function that formats and stores a series of characters and values in the buffer pointed to by ptr. In this case, it writes the string message0 ("Studying ") into the shared memory region pointed to by ptr.
	ptr += strlen(message0); // After writing message0, we increment the pointer ptr by the length of message0 using strlen(message0). This moves the pointer to the position immediately after the end of the written message, allowing us to write the next message without overwriting the previous one.
	// Each char is 1 byte, so in the end with message0 we will have 10 bytes used in the shared memory, and ptr will point to the 11th byte, which is where we will write message1.
	// \0 is the null terminator that marks the end of a string in C. When we write message0 to the shared memory, it includes the null terminator at the end of the string. So, after writing message0, we have 10 bytes used (9 characters + 1 null terminator), and ptr will point to the 11th byte, which is where we will write message1.
	// It is done so that when the consumer process reads the shared memory, it can correctly identify the end of each message using the null terminator.
	// However, to add this \0 I need to increment ptr by strlen(message0)+1, this +1 will give the null terminator.
	sprintf(ptr,"%s",message1);
	ptr += strlen(message1);
	sprintf(ptr,"%s",message2);
	ptr += strlen(message2);

	return 0;
}