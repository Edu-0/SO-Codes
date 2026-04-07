/**
 * A pthread program illustrating how to
 * create a simple thread and some of the pthread API
 * This program implements the summation function where
 * the summation operation is run as a separate thread.
 *
 * Most Unix/Linux/OS X users
 * gcc thrd.c -lpthread
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum; /* this data is shared by the thread(s) */

void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; // Está passando atributos padrões do sistema. Posso apagar essa linha e passar NULL no pthread_create, mas deixei para mostrar como usar os atributos.

	if (argc != 2) { // Se o número de argumentos for diferente de 2, exibe a mensagem de uso e retorna -1
		fprintf(stderr,"usage: a.out <integer value>\n");
		/*exit(1);*/
		return -1;
	}

	if (atoi(argv[1]) < 0) { // Se o argumento for um número negativo, exibe a mensagem de erro e retorna -1
		fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));
		/*exit(1);*/
		return -1;
	}

	/* get the default attributes */
	pthread_attr_init(&attr); // Aqui coloca os atributos para a thread, mas como não estamos usando nenhum atributo específico, podemos usar os atributos padrão do sistema. Se quisermos usar os atributos padrão, podemos passar NULL no pthread_create e não precisamos dessa linha. Deixei para mostrar como usar os atributos, mas nesse caso específico, não é necessário.

	/* create the thread */
	pthread_create(&tid,&attr,runner,argv[1]); // Cria uma nova thread, passando o identificador da thread, os atributos, a função a ser executada e o argumento para a função.

	/* now wait for the thread to exit */
	pthread_join(tid,NULL);

	printf("sum = %d\n",sum);
}

/**
 * The thread will begin control in this function
 */
void *runner(void *param) 
{
	int i, upper = atoi(param); // Converte o argumento passado para um número inteiro e armazena na variável upper
	sum = 0; // Inicializa a variável sum com 0

	if (upper > 0) { // Se upper for maior que 0, executa o loop para calcular a soma dos números de 1 até upper
		for (i = 1; i <= upper; i++)
			sum += i; // Adiciona o valor de i à variável sum em cada iteração do loop
	}

	pthread_exit(0); // Encerra a thread e retorna o valor 0 para indicar que a execução foi bem-sucedida
}