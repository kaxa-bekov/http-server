#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "http_init_tcp.h"

#define PORT 2323


//This function spins up a listening TCP server and hands it over to main.c where the while loop begins.

int init_server(server_init_s* init_struct){
    printf("\nInitializing a server...\n");
    printf("Creating a server socket...\n");


    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0); //creating a server socket fd
    if(server_socket_fd < 0) {perror("Server socket was not created.\n"); return 1;}


    init_struct->server_socket_fd = server_socket_fd;

    printf("Server Socket was created succesfully.\n");
    printf("Creating the server address struct...\n");

    struct sockaddr_in server_address; //server address struct
    server_address.sin_family = AF_INET; //address family is set to internet
    server_address.sin_port = htons(PORT); //port number converted to network byte order
    server_address.sin_addr.s_addr = INADDR_ANY; //setting IP address of the socket to ANY (0.0.0.0)

    socklen_t server_address_length = sizeof server_address; // the server address struct length

    init_struct->sockaddr_in_address = server_address;
    init_struct->server_address_length = server_address_length;

    printf("Server address struct created succesfully.\n"); 
    printf("Binding the server socekt fd to the server addresss struct...\n");

    if(bind(server_socket_fd, (struct sockaddr*)&server_address, server_address_length) < 0) {perror("Server socket was not bound to the address struct.\n"); return 1;}


    printf("Server socket fd was succesfully bound to server address struct.\n");
    printf("Starting listening...\n");
    
    if(listen(server_socket_fd, 10)){perror("Server could not start listening.\n"); return 1;}


    printf("Server is listenning...\n");

    return 0;
}
