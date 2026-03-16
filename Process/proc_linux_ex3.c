#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    pid_t pid, pid1;

    /* fork a child process */
    pid = fork(); // Ambos processos (pai e filho) executam a partir daqui. O filho continua a partir do ponto onde o fork() foi chamado, mas com um valor de retorno diferente.
    if (pid < 0) { 
      /* error occurred */ 
      fprintf(stderr, "Fork Failed"); 
      return 1;
    }
    else if (pid == 0) { /* child process */
      pid1 = getpid();
      printf("child: pid = %d\n",pid); /* A */
      printf("child: pid1 = %d\n",pid1); /* B */
    }
    else { /* parent process */
      pid1 = getpid();
      printf("parent: pid = %d\n",pid); /* C */
      printf("parent: pid1 = %d\n",pid1); /* D */
      wait(NULL);
    }

    return 0;
}
/*
child: pid = 0
child: pid1 = 33072
parent: pid = 33072
parent: pid1 = 33008

1. pid = fork();
   - Cria um novo processo filho.
   - No **pai**, fork() retorna o PID do filho (ex: 33072).
   - No **filho**, fork() retorna 0.

2. if (pid == 0) → bloco do filho:
   - O filho entra aqui porque fork() retornou 0.
   - pid1 = getpid();
     → pid1 recebe o **PID real do filho** (ex: 33072).
   - printf("child: pid = %d\n", pid);
     → imprime 0, porque fork() retorna 0 no filho.
   - printf("child: pid1 = %d\n", pid1);
     → imprime o PID real do filho.

3. else → bloco do pai:
   - O pai entra aqui porque fork() retornou o PID do filho.
   - pid1 = getpid();
     → pid1 recebe o **PID real do pai** (ex: 33008).
   - printf("parent: pid = %d\n", pid);
     → imprime o PID do filho criado (ex: 33072).
   - printf("parent: pid1 = %d\n", pid1);
     → imprime o PID do pai.
   - wait(NULL);
     → pai espera o filho terminar.

Resumo dos valores:
- pid: retorno do fork()
    * 0 no filho
    * PID do filho no pai
- pid1: getpid()
    * PID real do processo que está executando a linha
- Ordem de saída pode variar dependendo do agendamento do sistema/debug.
*/