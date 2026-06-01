#include <stdio.h>
#include <string.h>
#include "http_response_codes.h"



int r200_ok(char* resp_buff){
    //Return 200 OK response
   
    return 0;
}  

int r400_bad_request(char* resp_buff){
    
    resp_buff = "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 135\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>404 Not Found</title></head>"
        "<body><h1>404 Not Found</h1></body>"
        "</html>";


    return 0;
}


int r403_forbidden(char* resp_buff){
    return 0;
}                                      
int r404_not_found(char* resp_buff){

    return 0;
}
int r405_methond_not_allowed(char* resp_buff){


    return 0;
}
int r414_uri_too_long(char* resp_buff){


    return 0;
}
int r500_internal_server_error(char* resp_buff){

    return 0;
}
