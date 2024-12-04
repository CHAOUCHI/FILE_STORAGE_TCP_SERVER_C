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

int client_port = 4000;
int server_port = 5000;

int main(int argc, char** argv){
    // Get shell param port
    // User specified a client port
    if(argc >= 2){ 
        client_port = atoi(argv[1]);
        if(client_port == 0){
            printf("Invalid client port in 1st command option\n");
            return EXIT_FAILURE;
        }
    }
    // User specified a server port
    if(argc >= 3){
        server_port = atoi(argv[2]);
        if(server_port == 0){
            printf("Invalid server port in 2nd command option\n");
            return EXIT_FAILURE;
        }
    }

    printf("Server port : %d\n",server_port);
    printf("Client port : %d\n",client_port);


    int client_fd = initTCPClient(client_port,"127.0.0.1",server_port);
    if(client_fd < 0) return EXIT_FAILURE;

    /**
     * SEND FILE TO SERVER
     */
    // Open the file to be send
    FILE* file_to_send_fd = fopen("original/chat.png","rb"); // Read binary only Access
    
    // Get file size (bytes)
    fseek(file_to_send_fd,0,SEEK_END);
    int file_size = ftell(file_to_send_fd);
    printf("The file is %d bytes\n",file_size);
    
    // Reposition the cursor at the start of the file to read after
    fseek(file_to_send_fd,0,SEEK_SET);

    // Load the file bytes to a buffer
    void* file_to_send_buffer = malloc(file_size);

    fread(file_to_send_buffer,file_size,1,file_to_send_fd);
    
    // send file
    send(client_fd,file_to_send_buffer,file_size,0);perror("send");

    close(client_fd);

    free(file_to_send_buffer);






}