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
#include "global.c"

int server_port = 5000;
int server_fd = -1;

int main(int argc, char** argv){
    // Get shell param server port
    if(argc > 1){
        server_port = atoi(argv[1]);
        if(server_port == 0){
            printf("Invalid port in command option\n");
            return EXIT_FAILURE;
        }
    }
    
    // socket, bind, listen
    int server_fd = initTCPServer(server_port);
    if(server_fd < 0) return EXIT_FAILURE;

    printf("Server listen on port %d\n",server_port);
    // accept
    int client_fd = waitForClientToConnect(server_fd);
    if(client_fd == ERR_SERVER_WAIT_CLIENT_CONNECTION) return EXIT_FAILURE;
    printf("Client accepted %d\n",client_fd);



    /**
     * Recv file name from client
     */
    char file_name[BUFSIZ];memset(file_name,0,BUFSIZ);

    recv(client_fd,file_name,BUFSIZ,0);

    printf("File name to upload : %s\n",file_name);

    // serialize path to the new file
    char path_to_file[BUFSIZ]; memset(path_to_file,0,BUFSIZ);
    sprintf(path_to_file,"cloud/%s",file_name);
    printf("File path to upload : %s\n",path_to_file);


    /**
     * Create new file
     */
    FILE* file_to_upload = fopen(path_to_file,"wb");


    /**
     * RECV FILE FROM CLIENT
     */
    void* client_request = malloc(MAX_RECV_BUFFER_SIZE);
    int recv_return_value = recv(client_fd,client_request,MAX_RECV_BUFFER_SIZE,0);perror("recv()");
    printf("recv_return_value : %d\n",recv_return_value);
    

    /**
     * Wrtie on new file
     */
    fwrite(client_request,MAX_RECV_BUFFER_SIZE,1,file_to_upload);


    fclose(file_to_upload);
    free(client_request);
    close(client_fd);
    close(server_fd);

}