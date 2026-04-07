/*	
*	Ilustra a criacao de threads e uso de mutex
*	Compilar com:	gcc -lpthread -o pthreads1 pthreads1.c
*	ou
*			gcc -o pthreads1 pthreads1.c -lpthread
*
*/

#include	<pthread.h>
#include	<stdio.h>
#include	<unistd.h>

/*************************************************************************
*	Monitor responsavel pelo acesso ao valor total
*
*	Uma unica funcao, usada para somar 'delta' no valor total
***/

int valor_total = 0;
pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER;

int alteraValor( int delta) {
	int novo_valor;

	pthread_mutex_lock(&em); // bloqueia o monitor
	valor_total += delta; // atualiza o valor total com o delta passado
	novo_valor = valor_total; // guarda o valor atualizado para mostrar na tela
	printf("Somando %d ficarÃ¡ %d\n", delta, novo_valor); // mostra o valor atualizado na tela
	pthread_mutex_unlock(&em); // libera o monitor para outras threads somarem ou subtrairem do valor total
	return novo_valor;
}
/*************************************************************************/

/***
*	Codigo de thread que incrementa
***/
void thread_incrementa(void) {
	while(1){
		sleep(1);
		alteraValor(+1);
	}
}

void thread_decrementa(void) {
	while(1){
		sleep(2);
		alteraValor(-1);
	}
}

int main( int argc, char *argv[]){

        pthread_t t1, t2, t3, t4;

        pthread_create(&t1, NULL, (void *) thread_incrementa, NULL);
        pthread_create(&t2, NULL, (void *) thread_incrementa, NULL);
        pthread_create(&t3, NULL, (void *) thread_decrementa, NULL);
        pthread_create(&t4, NULL, (void *) thread_decrementa, NULL);

	printf("Digite enter para terminar o programa:\n");
	getchar();
}