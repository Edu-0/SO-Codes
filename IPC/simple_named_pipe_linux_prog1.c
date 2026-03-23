// C program to implement one side of FIFO
// This side writes first, then reads
//https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
 
int main()
{
    int fd;
 
    // FIFO file path
    char * myfifo = "/tmp/myfifo";
 
    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo, 0666); // mkfifo() creates a FIFO passing the pathname and permission mode as arguments.
 
    char arr1[80], arr2[80]; // Buffer to store data
    while (1)
    {
        // Open FIFO for write only
        fd = open(myfifo, O_WRONLY);
 
        // Take an input arr2ing from user.
        // 80 is maximum length
        fgets(arr2, 80, stdin); // fgets() é o equivalente ao getline() do C++. Ele lê uma linha de texto do stdin e armazena na variável arr2, com um limite de 80 caracteres.
 
        // Write the input arr2ing on FIFO
        // and close it
        write(fd, arr2, strlen(arr2)+1);
        /*
            O write() recebe os parâmetros: Descritor de arquivo (fd), buffer de dados (arr2) e o número de bytes a serem escritos (strlen(arr2)+1). O +1 inclui o caractere nulo de terminação de string.
            Descritor de arquivo (fd) é um inteiro que representa a posição do arquivo na tabela de arquivos abertos do sistema operacional. Ele é retornado pela função open() quando o arquivo é aberto com sucesso.
            Buffer de dados (arr2) é o endereço do buffer que contém os dados a serem escritos no arquivo. No caso, é a string armazenada em arr2.
        */
        close(fd); // Fecha o descritor de arquivo após a escrita. Isso é importante para liberar recursos do sistema e garantir que as alterações sejam salvas corretamente e que outros processos possam acessar o FIFO posteriormente. Apenas 1 processo pode abrir o FIFO para escrita, e ele deve fechá-lo para que outros processos possam abrir para escrita ou leitura.
 
        // Open FIFO for Read only
        fd = open(myfifo, O_RDONLY);
 
        // Read from FIFO
        read(fd, arr1, sizeof(arr1));
        /*
            Dados foram escritos no file descriptor (fd) usando a função write() no processo de escrita. Agora, o processo de leitura pode ler esses dados usando a função read().
            O arr1 receberá os dados lidos do FIFO.
            Eu n leio direto para imprimir, agora estou apenas transferindo/transcrevendo para o arr1, e depois imprimo o arr1.
        */
 
        // Print the read message
        printf("User2: %s\n", arr1); // Imprime a mensagem lida do FIFO, que foi escrita pelo processo de escrita. O arr1 contém a string lida do FIFO, e é exibida no console com o prefixo "User2: ".
        close(fd);
    }
    return 0;
}

/*
    Preciso ter um segundo processo rodando, que é o outro lado do FIFO, para que esse programa funcione corretamente.
    Ele será ligado pelo myfifo no path "/tmp/myfifo".
    open() bloqueia até alguém abrir para leitura ou escrita, dependendo do modo em que foi aberto.
*/