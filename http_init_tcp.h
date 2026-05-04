

#ifndef  HTTP_INIT_TCP
#define HTTP_INIT_TCP

typedef struct {
    void sockaddr_in_address;
    int server_socket_fd;
    int server_address_length;


} server_init_s

int init_server(server_init_s* init_struct);


#endif
