#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>

#include "http_init_tcp.h"
#include "http_parser.h"
#include "file_manipulations.h"
#include "http_response_codes.h" //makefile
                                 
#define BUFFER_SIZE 1024

server_init_s init_struct;

//Web Root directory
char web_root[PATH_MAX];
const char *WEB_ROOT = web_root;

int main(){
    
    //Request struct init
    request_s request_struct;
    //Response struct init
    response_s response_struct;
    //Initializing the Web Root directory
    if(realpath("www", web_root) == NULL){
        perror("WWW directory doesnt exist");
        return 1;
    }


    //TODO Create a dedicated pathname buffer to write to from file_manipulations. Do not write anything to the original request line

    char req_buff[BUFFER_SIZE + 1];
    //Initializing a TCP server
    init_server(&init_struct);
    
    int server_fd = init_struct.server_socket_fd;

/*------------------------------------------------------------------------*/

    while(true){
        printf("------------------------------Waiting for new connection--------------------------\n\n");    

        struct sockaddr_in client_address;  //might move these 2 declarations outside of the loop
        socklen_t client_addr_len = sizeof client_address;
        
        int conn_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_addr_len);

        if(conn_fd < 0){ perror("Accept Failed!\n"); return 1;}

        //Reading a request
        ssize_t read_bytes = read(conn_fd, req_buff, BUFFER_SIZE);
       
        if(read_bytes < 0){ perror("Read Failure!\n"); return 1;}

        req_buff[read_bytes] = '\0';
        printf("Read %ld bytes.\nContent:\n%s-\n--------------------------\n", read_bytes, req_buff);
       
        //Calling parsing function 
        int token = tokenizer(req_buff, &request_struct, &response_struct); //Might need a return tokenizer result
        
        if(token == 1){
            if(write(conn_fd, response_struct.resp_buf, response_struct.content_length) == -1){
                perror("Writing bad response failed.\n");
                return 1;
            }
            close(conn_fd);
            continue;
        }

        printf("\nTokenizator output:\nMethod: %s\nPath: %s\nProtocol: %s\nHost: %s\nUser-Agent: %s\nAccept: %s\n\n\n", request_struct.method, request_struct.path, request_struct.proto, request_struct.headers.host, request_struct.headers.user_agent, request_struct.headers.accept);

        //Validating the requested file
        int validation_result = validate_file(request_struct.path, WEB_ROOT, &response_struct);
        if(validation_result == 1){
            if(write(conn_fd, response_struct.resp_buf, response_struct.content_length) == -1){
                perror("Writing bad response failed.\n");
                return 1;
            }
            close(conn_fd);
            continue;
        }

        //Getting connection information
        char *host_ip = inet_ntoa(client_address.sin_addr);
        unsigned int host_port = ntohs(client_address.sin_port);

        printf("Connected to host %s on port %u\n", host_ip, host_port);

        printf("resp_buf right before writing to conn_fd is: %s\nAnd the content_length is: %ld\n", response_struct.resp_buf, response_struct.content_length);
        //Writing a response and headers
        ssize_t write_bytes = write(conn_fd, response_struct.resp_buf, strlen(response_struct.resp_buf));
        
        if(write_bytes < 0){perror("Write Failed!");return 1;}

        printf("Wrote %ld bytes of response_buf.\nContent:\n%s\n", strlen(response_struct.resp_buf), response_struct.resp_buf);

        printf("The filename after the validation is: %s\n", request_struct.path);
        printf("Protocol: %s\n", request_struct.proto);

        int file_D = open(request_struct.path, O_RDONLY);

        if(file_D == -1){fprintf(stderr, "File could not be opened!\n"); return 1;}
        
        lseek(file_D, 0, SEEK_SET);

        char file_buffer[4096];

        ssize_t file_read_bytes = read(file_D, file_buffer, 4096);

        ssize_t file_write_bytes = write(conn_fd, file_buffer, file_read_bytes);


    
        //Nullifying the request & response struct
        //str_nullifier(&request_struct);

        //Close the socket and free the request buffer
        close(conn_fd);
    }
/*--------------------------------------------------------------------*/

    close(server_fd);
    return 0;
}
