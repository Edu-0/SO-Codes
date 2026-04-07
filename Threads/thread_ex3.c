#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define NUMTHREADS 2
#define VETSIZE 5000

typedef struct {
	int fromidx, length; // fromidx → índice inicial do vetor que a thread irá processar, length → quantidade de elementos que a thread irá processar a partir do índice inicial.
}thread_arg, *ptr_thread_arg;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// Se mais de uma thread tiver acesso ao recurso compartilhado (variável sum), é necessário usar um mutex para evitar condições de corrida.

pthread_t threads[NUMTHREADS]; // IDs das threads
thread_arg arguments[NUMTHREADS]; // dados que cada thread irá receber
int nums[VETSIZE]; // vetor global de números a serem somados pelas threads
int sum; // Soma total compartilhada entre as threads

void *thread_func(void *arg);

int main(int argc, char **argv){
	int i, length, remainder;
	sum = 0; length = VETSIZE / NUMTHREADS;
	remainder = VETSIZE % NUMTHREADS;

	for(int i = 0; i < VETSIZE; i++) {
    	nums[i] = 1; // ou qualquer valor
	}

	/*
		Cada thread recebe uma parte do vetor para processar.
		lenght = 5000 / 2 = 2500
		remainder = 5000 % 2 = 0

		Portanto, a thread 0 irá processar os elementos de índice 0 a 2499 e a thread 1 irá processar os elementos de índice 2500 a 4999.
		Se o tamanho do vetor não fosse divisível pelo número de threads, a última thread receberia os elementos restantes para processar, 
		garantindo que todos os elementos do vetor sejam somados corretamente.
	*/

	for(i=0; i<NUMTHREADS; i++) {
		arguments[i].fromidx = i * length; // Índice inicial do vetor para a thread i
		arguments[i].length = length; // Quantidade de elementos que a thread i irá processar
		if(i == (NUMTHREADS - 1)) arguments[i].length += remainder; // Se for a última thread, adiciona os elementos restantes
		pthread_create(&(threads[i]), NULL, thread_func, &(arguments[i])); // Cada thread recebe um ponteiro para sua parte
	}
	for(i=0; i<NUMTHREADS; i++) pthread_join(threads[i], NULL); // Espera todas as threads terminarem
	printf("A soma dos numeros do vetor eh %d\n", sum); // Imprime a soma total calculada pelas threads
}

void *thread_func(void *arg) {
	ptr_thread_arg argument = (ptr_thread_arg)arg; // Converte o argumento para o tipo correto que é um ponteiro para thread_arg
	int i, localsum = 0, endidx; // i para iteração, localsum para armazenar a soma local da thread, endidx para calcular o índice final do vetor que a thread irá processar
	
	endidx = argument->fromidx + argument->length; // Define intervalo
	
	for(i=argument->fromidx; i<endidx; i++){
		pthread_mutex_lock(&lock);
		localsum += nums[i]; // Soma os elementos do vetor que a thread é responsável por processar e armazena o resultado em localsum
		pthread_mutex_unlock(&lock);
	}
	sum += localsum;
}