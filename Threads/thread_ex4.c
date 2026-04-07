#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h> 
  
// Let us create a global variable to change it in threads 
int g = 0; 
  
// The function to be executed by all threads 
void *myThreadFun(void *vargp){ 
    // Store the value argument passed to this thread 
    int *myid = (int *)vargp; 
  
    // Let us create a static variable to observe its changes 
    static int s = 0; 
  
    // Change static and global variables 
    ++s; ++g; 
  
    // Print the argument, static and global variables 
    printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g); 
} 
  
int main(){ 
    int ids[3]; 
    pthread_t tid[3]; 
  
    // Let us create three threads 
    for (int i = 0; i < 3; i++) {
        ids[i] = i;
        pthread_create(&tid[i], NULL, myThreadFun, &ids[i]); 
    }
    for (int i = 0; i < 3; i++)
        pthread_join(tid[i], NULL);
    pthread_exit(NULL); 
    return 0; 
}

/*
Programa é rápido, então é difícil perceber a execução da thread, mas apareceu 1 não sequencial:

eduardo@Edurdo:~/SO-Codes/Threads$ ./te4
Thread ID: 0, Static: 2, Global: 2
Thread ID: 1, Static: 4, Global: 4
Thread ID: 2, Static: 6, Global: 6
eduardo@Edurdo:~/SO-Codes/Threads$ ./te4
Thread ID: 0, Static: 2, Global: 2
Thread ID: 2, Static: 6, Global: 6 <- Aqui o 2 executou antes do 1
Thread ID: 1, Static: 4, Global: 4
eduardo@Edurdo:~/SO-Codes/Threads$ ./te4
Thread ID: 0, Static: 2, Global: 2
Thread ID: 1, Static: 4, Global: 4
Thread ID: 2, Static: 6, Global: 6
eduardo@Edurdo:~/SO-Codes/Threads$ ./te4
Thread ID: 0, Static: 2, Global: 2
Thread ID: 2, Static: 6, Global: 6 <- Aqui o 2 executou antes do 1
Thread ID: 1, Static: 4, Global: 4
eduardo@Edurdo:~/SO-Codes/Threads$ ./te4
Thread ID: 0, Static: 2, Global: 2
Thread ID: 1, Static: 4, Global: 4
Thread ID: 2, Static: 6, Global: 6
eduardo@Edurdo:~/SO-Codes/Threads$ ./te4
Thread ID: 0, Static: 2, Global: 2
Thread ID: 1, Static: 4, Global: 4
Thread ID: 2, Static: 6, Global: 6
eduardo@Edurdo:~/SO-Codes/Threads$ ./te4
Thread ID: 0, Static: 2, Global: 2
Thread ID: 1, Static: 4, Global: 4
Thread ID: 2, Static: 6, Global: 6
eduardo@Edurdo:~/SO-Codes/Threads$ ./te4
Thread ID: 0, Static: 2, Global: 2
Thread ID: 1, Static: 4, Global: 4
Thread ID: 2, Static: 6, Global: 6
*/