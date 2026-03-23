//Servidor pipe (testado usando WSL)
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "/tmp/pipeso"

int main()
{
    /*
        sockfd: descritor de arquivo do socket do servidor.
        newsockfd: descritor de arquivo da conexão aceita do cliente.
        len: tamanho da estrutura de endereço do socket.
        local: estrutura de endereço do socket do servidor.
        remote: estrutura de endereço do socket do cliente.
        buffer: buffer para armazenar dados lidos do cliente e enviados de volta.
    */
    int sockfd, newsockfd, len;
    struct sockaddr_un local, remote;
    char buffer[1024];

    // Create socket
    // Um socket é um canal de comunicação entre processos, geralmente entre a rede ou local. O socket é criado usando a função socket(), que retorna um descritor de arquivo para o socket criado.
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0); // O socket é criado usando a família AF_UNIX (para comunicação local) e o tipo SOCK_STREAM (para comunicação orientada a conexão). O protocolo é definido como 0, o que significa que o sistema escolherá o protocolo apropriado para a combinação de família e tipo.
    /*
        1º argumento (AF_UNIX): Especifica a família de endereços para o socket. AF_UNIX é usado para comunicação local entre processos no mesmo sistema.
        2º argumento (SOCK_STREAM): Especifica o tipo de socket. SOCK_STREAM indica que o socket é orientado a conexão, ou seja, estabelece uma conexão confiável entre o cliente e o servidor.
        3º argumento (0): Especifica o protocolo a ser usado. O valor 0 indica que o sistema deve escolher o protocolo apropriado para a combinação de família e tipo.
    */
    if (sockfd < 0)
    {
        perror("Falha em criar o pipe");
        return 1;
    }

    // Bind socket to local address
    memset(&local, 0, sizeof(local)); // A função memset() é usada para preencher a estrutura local com zeros, garantindo que todos os campos sejam inicializados corretamente antes de serem usados.
    /*
        1º argumento (&local): O endereço de memória da estrutura local, que é o destino onde os bytes serão escritos.
        2º argumento (0): O valor a ser preenchido em cada byte da estrutura local. Neste caso, 0 é usado para inicializar todos os bytes com zero.
        3º argumento (sizeof(local)): O número de bytes a serem preenchidos, que é o tamanho da estrutura local. Isso garante que toda a estrutura seja preenchida com zeros.
        sizeof(local) é quantos bytes a struct ocupa na memória.
    */
    local.sun_family = AF_UNIX; // Define a família de endereços do socket como AF_UNIX, indicando que é um socket local para comunicação entre processos no mesmo sistema.
    strncpy(local.sun_path, SOCK_PATH, sizeof(local.sun_path) - 1); // Copia a string SOCK_PATH para o campo sun_path da estrutura local, garantindo que não haja estouro de buffer. O tamanho máximo é definido como sizeof(local.sun_path) - 1 para deixar espaço para o caractere nulo de terminação de string.
    unlink(local.sun_path); // Remove o arquivo de socket existente no caminho especificado por local.sun_path. Isso é necessário para evitar erros ao tentar vincular o socket a um caminho que já existe. O unlink() é usado para remover o arquivo de socket, garantindo que o servidor possa criar um novo socket sem conflitos.
    len = strlen(local.sun_path) + sizeof(local.sun_family); // Calcula o tamanho da estrutura de endereço do socket, que é a soma do tamanho do campo sun_path (o caminho do socket) e do campo sun_family (a família de endereços). Isso é necessário para a função bind() para associar o socket ao endereço local corretamente.
    
    /*
        sun_path é o caminho no sistema de arquivos onde o socket será criado: /tmp/socket_file
        sun_family é a família de endereços do socket, que é AF_UNIX para comunicação local.
        O unlink() remove o arquivo de socket existente para evitar conflitos ao criar um novo socket. -> unlink("/tmp/socket_file") Sockets do tipo AF_UNIX usam um caminho e caso haja um arquivo de socket existente no caminho especificado, o bind() falhará.
        Se já existir: bind() vai falhar (error: “Address already in use”)
    */
    
    if (bind(sockfd, (struct sockaddr *)&local, len) < 0) // bind() associa o socket a um endereço local. Liga o socket ao caminho especificado em local.sun_path, dado pelo SOCK_PATH, que pode ser por exemplo "/tmp/socket_file"
    // O que estou dizendo ao sistema: esse socket vai ficar disponível nesse endereço
    // Sem socket, ngm sabe onde encontrar.

    /*
        1º argumento (sockfd): O descritor do socket, socket criado com socket().
        2º argumento ((struct sockaddr *)&local): Endereço do socket, passando a struct local, preciso do cast pq o bind() espera um sockaddr. O meu sockaddr_un tem um AF_UNIX e o caminho.
        3º argumento (len): Tamanho da estrutura de endereço. O SO precisa saber quantos bytes ler.
    */

    {
        perror("Falha em capturar o socket");
        close(sockfd);
        return 1;
    }

    // Listen for connections
    if (listen(sockfd, 5) < 0) // Espera conexões de clientes. O segundo argumento (5) indica quantos clientes podem esperar na fila enquanto o servidor ainda não aceitou (accept). Se chegar a mais dq 5, eles podem receber erro ou serem recusados.
    {
        perror("Falha em escutar o socket");
        close(sockfd);
        return 1;
    }

    printf("Servidor Named pipe ouvindo em %s...\n", SOCK_PATH);

    // Accept connections
    memset(&remote, 0, sizeof(remote)); // memset() é usado para limpar a estrutura remote, garantindo que todos os campos sejam inicializados com zeros antes de serem usados. Isso é importante para evitar dados residuais que possam causar comportamento inesperado ao aceitar conexões de clientes.
    /*
        Local: Endereço do próprio socket do servidor, usado para bind() e listen().
        Remote: Endereço do socket do cliente que se conectou, recebido quando fiz accept(). Permite saber de onde vieram os dados.
    */
    len = sizeof(remote); // Define o tamanho da estrutura de endereço do cliente, que é necessário para a função accept() para preencher corretamente a estrutura remote com as informações do cliente que se conectou.
    newsockfd = accept(sockfd, (struct sockaddr *)&remote, &len); // Aceita uma conexão de cliente. A função accept() bloqueia até que um cliente se conecte ao socket do servidor. Quando um cliente se conecta, accept() retorna um novo descritor de arquivo (newsockfd) para a conexão estabelecida, e preenche a estrutura remote com as informações do cliente.
    // As informações do cliente são: o endereço do socket do cliente (remote) e o tamanho da estrutura de endereço (len). O servidor pode usar essas informações para se comunicar com o cliente.
    if (newsockfd < 0)
    {
        perror("Falha em aceitar coneccao");
        close(sockfd);
        return 1;
    }
    // No accept o programa para e fica esperando um cliente se conectar. Quando um cliente se conecta, o programa continua a execução.
    printf("Cliente conectado!\n"); // Indica que um cliente se conectou ao servidor, permitindo que o servidor prossiga para ler os dados enviados pelo cliente e processá-los conforme necessário.

    // Read data from client
    if (read(newsockfd, buffer, sizeof(buffer)) < 0) // Lê os dados enviados pelo cliente. A função read() é usada para ler os dados do cliente a partir do descritor de arquivo newsockfd, que representa a conexão estabelecida com o cliente. Os dados lidos são armazenados no buffer, e o tamanho máximo de leitura é definido como sizeof(buffer) para evitar estouro de buffer.
    {
        perror("Falha em ler do socket"); // Indica que houve um erro ao tentar ler os dados do cliente, o que pode ocorrer por várias razões, como uma conexão interrompida ou um problema de comunicação. O servidor fecha a conexão com o cliente (newsockfd) e o socket do servidor (sockfd) antes de retornar um código de erro.
        close(newsockfd);
        close(sockfd);
        return 1;
    }

    printf("Dado recebido: %s\n", buffer); // Exibe os dados recebidos do cliente, permitindo que o servidor veja o conteúdo enviado pelo cliente antes de processá-lo ou enviar uma resposta de volta.

    // Process data
    // In this example, we just convert the string to uppercase
    for (int i = 0; i < strlen(buffer); i++)
    {
        buffer[i] = toupper(buffer[i]);
    }

    // Write processed data back to client
    if (write(newsockfd, buffer, strlen(buffer) + 1) < 0) // Envia os dados processados de volta para o cliente. A função write() é usada para enviar os dados armazenados no buffer de volta para o cliente através do descritor de arquivo newsockfd. O tamanho dos dados enviados é definido como strlen(buffer) + 1 para incluir o caractere nulo de terminação de string, garantindo que o cliente receba a string completa.
    {
        perror("Falha em escrever no socket");
        close(newsockfd);
        close(sockfd);
        return 1;
    }

    printf("Dado enviado de volta para o cliente.\n"); // Indica que os dados processados foram enviados de volta para o cliente, confirmando que a comunicação foi bem-sucedida.

    // Close sockets and exit
    close(newsockfd); // Fecha a conexão com o cliente, liberando os recursos associados a essa conexão específica.
    close(sockfd); // Fecha o socket do servidor, liberando os recursos associados ao socket e permitindo que o servidor seja encerrado corretamente.
    return 0;
}