//Cliente pipe - testado no WSL
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "/tmp/pipeso"

int main()
{
    int sockfd, len;
    struct sockaddr_un remote;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Falha em criar o socket");
        return 1;
    }

    // Connect to server
    memset(&remote, 0, sizeof(remote)); // Limpo memória colocando 0 para retirar lixo.
    remote.sun_family = AF_UNIX;
    strncpy(remote.sun_path, SOCK_PATH, sizeof(remote.sun_path) - 1);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    // Sun path e sun family é o endereço do socket.
    // I &remote é o que dirá que o endereço é o que está se conectando, e len é o tamanho do endereço.
    if (connect(sockfd, (struct sockaddr *)&remote, len) < 0)
    {
        perror("Falha em conectar no servidor");
        close(sockfd);
        return 1;
    }

    printf("Conectado ao servidor!\n");

    // Send data to server
    printf("Entre com o dado a ser enviado: ");
    fgets(buffer, sizeof(buffer), stdin); // Lê uma linha de entrada do usuário e armazena no buffer. A função fgets() é usada para ler uma string do stdin (entrada padrão) e armazená-la no buffer, garantindo que não haja estouro de buffer ao limitar a leitura ao tamanho do buffer. O tamanho máximo de leitura é definido como sizeof(buffer).
    if (write(sockfd, buffer, strlen(buffer) + 1) < 0)
    {
        perror("Falha em escrever no socket");
        close(sockfd);
        return 1;
    }

    printf("Dado enviado ao servidor.\n");

    // Read data from server
    if (read(sockfd, buffer, sizeof(buffer)) < 0)
    {
        perror("Falha em ler do socket");
        close(sockfd);
        return 1;
    }

    printf("Dado recebido: %s\n", buffer); // Imprime o que foi lido do servidor no buffer, permitindo que o cliente veja a resposta enviada pelo servidor após processar os dados enviados pelo cliente.

    // Close socket and exit
    close(sockfd);
    return 0;
}