#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "http_parser.h"

#define PORT 2323
#define BUFFER_SIZE 1024

request_s rq_s;

int main(){
    char buff[BUFFER_SIZE];

    char resp_buff[] = "HTTP/1.0 200 OK\r\n"
        "Server: sserver-c\r\n"
        "Content-type: text/html\r\n\r\n"
        "<html>hello, world!</html>\r\n";

    size_t write_size = strlen(resp_buff);


    int s_socket_fd = socket(AF_INET, SOCK_STREAM, 0); //creating a socket fd

    if(s_socket_fd < 0){perror("Socket was NOT created!");return 1;}
    
    printf("Socket successfully created!\n");

    struct sockaddr_in addr; //creating an address structure and initializing the members.
    addr.sin_family = AF_INET; //Internet Address family
    addr.sin_port = htons(PORT); //Port number converted to Network Byte Order format
    addr.sin_addr.s_addr = INADDR_ANY; //Setting the actual IP of the socket to ANY (0.0.0.0)

    socklen_t addrlen = sizeof addr; 
    
    int bind_result = bind(s_socket_fd,(struct sockaddr *)&addr, addrlen);  //Binding the sockaddr_in structure to our socket (s_socket_fd)

    if(bind_result < 0){perror("Bind Failed!");return 1;}

    printf("Socket successfully bound to address!\n");

    int listen_result = listen(s_socket_fd, 10);

    if(listen_result < 0){perror("Listen Failed!");return 1;}

    printf("Server is listening...\n");

    //socklen_t addr_len = sizeof(addr);

    while(true){

        printf("------------------------------Waiting for new connection--------------------------\n\n");    
        
        int conn_fd = accept(s_socket_fd, (struct sockaddr *)&addr, &addrlen);

        if(conn_fd < 0){
            perror("Accept Failed!\n");
            return 1;
        }

        //Reading a request
        ssize_t read_bytes = read(conn_fd, buff, BUFFER_SIZE);
        
        if(read_bytes < 0){
            perror("Read Failure!\n");
            return 1;
        }

        printf("Read %d bytes.\nContent:\n%s\n", read_bytes, buff);
       
        //Disect the request in to headers and further in to Method, Path, and other Headers.
        //char *rem_buff = buff;
        //char *space = strchr(rem_buff, 32);
        //printf("\nThe address of the first byte in the request ('G') is %p.\nThe address of the first space (' ') character (32) is %p.\n", buff, space);
        //*space = 0;
        //char *method = rem_buff;
        //rem_buff = space + 1;
        //printf("Method is %s\nThe remaining request is\n%s\n", method, rem_buff);
        //space = strchr(rem_buff, 32);
        //*space = 0;
        //char *path = rem_buff;
        //rem_buff = space + 1;
        //printf("Path is %s\nThe remaining request is\n%s\n", path, rem_buff);
        //space = strchr(rem_buff, 32);
        //*space = 0;
        //char *proto = rem_buff;
        //rem_buff = space + 1;
        //printf("Protocol is %s\nThe remaining request is\n%s\n", proto, rem_buff);
        
        //Using a custom header file to disect the request
        
        tokenizer(buff, &rq_s);

        printf("Tokenizator output:\nMethod: %s\nPath: %s\nProtocol: %s\nHost: %s\nUser-Agent: %s\nAccept: %s\n\n\n", rq_s.method, rq_s.path, rq_s.proto, rq_s.headers.host, rq_s.headers.user_agent, rq_s.headers.accept);


        //Getting the connected host information
        int sockname_result = getsockname(conn_fd, (struct sockaddr *)&addr, &addrlen);
        
        if(sockname_result < 0){perror("getsockname Failed!\n");return 1;}

        char *host_ip = inet_ntoa(addr.sin_addr);
        unsigned int host_port = ntohs(addr.sin_port);

        printf("Connected to host %s on port %u\n", host_ip, host_port);

        //Writing a response
        ssize_t write_bytes = write(conn_fd, resp_buff, write_size);
        
        if(write_bytes < 0){
            perror("Write Failed!");
            return 1;
        }

        printf("Wrote %d bytes.\nContent:\n%s", write_bytes, resp_buff);

        //Close the socket
        close(conn_fd);
    }

    close(s_socket_fd);
    return 0;
}
