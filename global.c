#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>

#define MAX_RECV_BUFFER_SIZE 300000

#define ERR_INIT_SOCKET -100
#define ERR_INIT_BIND -101
#define ERR_SERVER_INIT_LISTEN -102
#define ERR_SERVER_WAIT_CLIENT_CONNECTION -200

#define ERR_CLIENT_INIT_CONNECT -300

/**
 * @param int port - Le port du serveur
 * @return return an int server_fd or one of the following constant if an error occured : ERR_SERVER_INIT_SOCKET, ERR_SERVER_INIT_BIND, ERR_SERVER_INIT_LISTEN
 */
int initTCPServer(int port){
      /**
     * Définition des infos du serveur
     * port et ip
     */
    struct sockaddr_in server_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(port)
    };


    int server_fd = socket(AF_INET,SOCK_STREAM,0);
    perror("socket");
    if (server_fd==-1){
        return ERR_INIT_SOCKET;
    }

    int check_error = bind(server_fd,(struct sockaddr*)&server_addr,sizeof server_addr);
    perror("bind");
    if(check_error==-1){
        return ERR_INIT_BIND;
    }

    check_error = listen(server_fd,BUFSIZ);
    perror("listen");
    if(check_error==-1){
        return ERR_SERVER_INIT_LISTEN;
    }

    return server_fd;
 
}


/**
 * FONCTION BLOQUANTE
 * @param int server_fd - the server file descriptor provided by the fonction initTCPServer.
 * @return int client_fd - the client file descriptor or ERR_SERVER_FAILED_ACCEPT_CLIENT on error.
 */
int waitForClientToConnect(int server_fd){
    struct sockaddr_in client_addr;
    socklen_t len;

    int client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&len);
    if(client_fd==-1){
        perror("accept");
        return ERR_SERVER_WAIT_CLIENT_CONNECTION;
    }

    return client_fd;
}


int initTCPClient(int client_port, char* server_ip,int server_port){

    int client_fd = socket(AF_INET,SOCK_STREAM,0);

    perror("socket");
    if (client_fd == -1){
        return ERR_INIT_SOCKET;
    }

    struct sockaddr_in client_addr={
        .sin_addr.s_addr=INADDR_ANY,
        .sin_family=AF_INET,
        .sin_port=htons(client_port)
    };

    int check_error = bind(client_fd,(struct sockaddr*)&client_addr,sizeof client_addr);
    perror("bind");
    if(check_error==-1){
        return ERR_INIT_BIND;
    }


    struct sockaddr_in server_addr={
        .sin_addr.s_addr=inet_addr(server_ip),
        .sin_family=AF_INET,
        .sin_port=htons(server_port)
    };

    /**
     * Connexion au serveur
     */
    check_error = connect(client_fd,(struct sockaddr*)&server_addr,sizeof server_addr);
    perror("connect");
    if(check_error == -1){
        return ERR_CLIENT_INIT_CONNECT;
    }

    return client_fd;
}


/**
 * @return the filename or NULL
 */
char* filePathTofileName(char* file_path){
    char* file_name = NULL;
    for (int i = strlen(file_path)-1; i >= 0; i--)
    {
        if(file_path[i] == '/'){
           file_name = &(file_path[i+1]);
        }
    }
    return file_name;
    
}