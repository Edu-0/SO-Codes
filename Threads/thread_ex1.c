/*	
*	Ilustra a criacao de threads e uso de mutex
*	Compilar com:	gcc -lpthread -o pthreads-tela pthreads-tela.c
*	ou
*			gcc -o pthreads-tela pthreads-tela.c -lpthread
*
*/

#include	<pthread.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>

/*************************************************************************
*	Monitor responsavel pelo acesso ao valor total
*
*	Uma unica funcao, usada para somar 'delta' no valor total
***/

int valor_total = 0; // valor_total → valor compartilhado entre threads
pthread_mutex_t em = PTHREAD_MUTEX_INITIALIZER; // em → mutex que protege esse valor (evita condição de corrida)

int alteraValor( int delta) { // Delta → valor a ser adicionado ao valor_total. Pode ser positivo ou negativo, dependendo se queremos incrementar ou decrementar o valor_total.
	int novo_valor;

	pthread_mutex_lock(&em);
	valor_total += delta;
	novo_valor = valor_total;
	pthread_mutex_unlock(&em);
	// Garante acesso exclusivo ao valor_total, evitando que múltiplas threads modifiquem o valor_total ao mesmo tempo, o que poderia levar a resultados inconsistentes ou incorretos. O mutex em é usado para garantir que apenas uma thread possa acessar e modificar o valor_total por vez.
	// As outras threads que tentarem acessar o valor_total enquanto ele estiver bloqueado pelo mutex em, terão que esperar até que a thread atual libere o mutex, garantindo assim a integridade do valor_total.
	return novo_valor;
}

void defineValor( int novo) { // defineValor → função para definir o valor_total diretamente, em vez de incrementá-lo. O parâmetro novo é o novo valor que queremos atribuir ao valor_total.
	pthread_mutex_lock(&em); // Fica esperando o mutex em para garantir acesso exclusivo ao valor_total, evitando condições de corrida.
	valor_total = novo;
	pthread_mutex_unlock(&em);
}
/*************************************************************************/

/*************************************************************************
*	Monitor responsavel pelo acesso a tela
*
***/

pthread_mutex_t tela = PTHREAD_MUTEX_INITIALIZER; // tela → mutex que protege o acesso à tela (evita que as threads imprimam ao mesmo tempo, causando confusão na saída)

void aloca_tela( void) { // aloca_tela → função para bloquear a tela, garantindo que apenas uma thread possa acessar a tela por vez
	pthread_mutex_lock(&tela);
	}

void libera_tela( void) { // libera_tela → função para desbloquear a tela, permitindo que outras threads possam acessar a tela
	pthread_mutex_unlock(&tela);
	}

/*************************************************************************/

/***
*	Thread que mostra status na tela
***/
void thread_mostra_status(void) { // thread_mostra_status → função que será executada pela thread responsável por mostrar o status na tela. Status é o valor total atualizado a cada segundo.
	int ultimo; // ultimo → variável local para armazenar o último valor lido do valor_total, evitando acessar o valor_total diretamente a cada vez que for imprimir o status. Isso também ajuda a reduzir a quantidade de vezes que a thread precisa adquirir o mutex para acessar o valor_total.
	while(1){
		sleep(1); // sleep(1) → faz a thread dormir por 1 segundo, simulando um intervalo de tempo entre as atualizações do status
		aloca_tela(); // aloca_tela() → bloqueia a tela para garantir que a saída seja organizada e não misturada com a saída de outras threads
		ultimo = alteraValor(+1); // alteraValor(+1) → incrementa o valor_total em 1 e retorna o novo valor, que é armazenado na variável ultimo. Isso simula uma atualização do status a cada segundo.
		printf("Ultimo lido foi %d, digite <enter> para alterar\n", ultimo);
		libera_tela();
		// A cada segundo, a thread_mostra_status incrementa o valor_total em 1, bloqueia a tela para imprimir o status atualizado, e depois libera a tela para que o usuário possa interagir.
	}
}

/***
*	Thread que le teclado
***/
void thread_le_teclado(void) {
	int novo;
	char teclado[1000];
	while(1){
		fgets( teclado, 1000, stdin); // Aguarda o usuário digitar Enter
		aloca_tela(); // Trava o mostra_status
		printf("Digite novo valor:\n");
		fgets( teclado, 1000, stdin); // Aguarda o usuário digitar um novo valor e pressionar Enter.
		defineValor(atoi(teclado)); // Converte string para inteiro.
		libera_tela();
	}
}

int main( int argc, char *argv[]) {

        pthread_t t1, t2; // Criacao de threads, vão guardar os IDs das threads

        pthread_create(&t1, NULL, (void *) thread_mostra_status, NULL); // Criacao da thread que mostra status
        pthread_create(&t2, NULL, (void *) thread_le_teclado, NULL); // Thread que espera input do usuário

		/*
		    Parâmetros da função pthread_create:
				1. &thread: Onde está armazenado o ID da Thread.
				2. NULL: Atributos padrões.
					Atributos podem incluir coisas como prioridade, tamanho da pilha, etc.
				3. (void *) thread_mostra_status: um ponteiro para a função que a thread irá executar. A função deve ter o tipo de retorno void* e aceitar um único argumento do tipo void*.
				4. NULL: um ponteiro para os argumentos que serão passados para a função da thread. Se a função da thread não precisar de argumentos, pode ser NULL.
		*/

	// Espera as threads terminarem (neste caso, elas não terminam, então o programa ficará rodando indefinidamente)
	pthread_join( t1, NULL);
	pthread_join( t2, NULL);
}