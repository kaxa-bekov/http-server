#include <netinet/in.h>

#ifndef  HTTP_INIT_TCP
#define HTTP_INIT_TCP

#define PORT 2323

typedef struct {
    struct sockaddr_in sockaddr_in_address;
    int server_socket_fd;
    int server_address_length;
    //May be add WEB_ROOT here 

} server_init_s;

int init_server(server_init_s* init_struct);


#endif  //HTTP_INIT_TCP included
