#include "http_response_codes.h"

#include <stdio.h>   //Might remove after test how many bytes
#include <string.h>  //Might need for snprintf ing the 200 ok headers

int r200_ok(response_s *response_struct){
    //Return 200 OK respose
    snprintf(response_struct->resp_buf, 200, "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>200 OK</title></head>"
        "<body><h1>200 OK</h1></body>"
        "</html>", response_struct->content_type, (long)response_struct->content_length);
    return 0;
}  

int r400_bad_request(response_s *response_struct){
    strncpy(response_struct->resp_buf, "HTTP/1.1 400 Bad Request\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 135\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>400 bad Request</title></head>"
        "<body><h1>400 Bad Request</h1></body>"
        "</html>", 200);
    response_struct->resp_buf[200] = '\0';
	response_struct->content_length = strlen(response_struct->resp_buf);
	strncpy(response_struct->content_type, "text/html", 10);
    response_struct->content_type[9] = '\0';
    return 0;
}

int r403_forbidden(response_s *response_struct){
    strncpy(response_struct->resp_buf, "HTTP/1.1 403 Forbidden\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 135\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>403 Forbidden</title></head>"
        "<body><h1>403 Forbidden</h1></body>"
        "</html>", 200);
    response_struct->resp_buf[200] = '\0';
	response_struct->content_length = strlen(response_struct->resp_buf);
	strncpy(response_struct->content_type, "text/html", 10);
    response_struct->content_type[9] = '\0';
    return 0;
}                                      
int r404_not_found(response_s *response_struct){
    strncpy(response_struct->resp_buf, "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 135\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>404 Not Found</title></head>"
        "<body><h1>404 Not Found</h1></body>"
        "</html>", 200);
    response_struct->resp_buf[200] = '\0';
	response_struct->content_length = strlen(response_struct->resp_buf);
	strncpy(response_struct->content_type, "text/html", 10);
    response_struct->content_type[9] = '\0';
    return 0;
}
int r405_method_not_allowed(response_s *response_struct){
    strncpy(response_struct->resp_buf, "HTTP/1.1 405 Method Not Allowed\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 135\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>405 Method Not Allowed</title></head>"
        "<body><h1>405 Method Not Allowed</h1></body>"
        "</html>", 200);
    response_struct->resp_buf[200] = '\0';
	response_struct->content_length = strlen(response_struct->resp_buf);
	strncpy(response_struct->content_type, "text/html", 10);
    response_struct->content_type[9] = '\0';
    return 0;
}
int r414_uri_too_long(response_s *response_struct){
    strncpy(response_struct->resp_buf, "HTTP/1.1 414 URI Too Long\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 135\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>414 URI Too Long</title></head>"
        "<body><h1>414 URI Too Long</h1></body>"
        "</html>", 200);
    response_struct->resp_buf[200] = '\0';
	response_struct->content_length = strlen(response_struct->resp_buf);
	strncpy(response_struct->content_type, "text/html", 10);
    response_struct->content_type[9] = '\0';
    return 0;
}
int r500_internal_server_error(response_s *response_struct){
    strncpy(response_struct->resp_buf, "HTTP/1.1 500 Internal Server Error\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 135\r\n"
        "\r\n"
        "<!DOCTYPE html>"
        "<html>"
        "<head><title>500 Internal Server Error</title></head>"
        "<body><h1>500 Internal Server Error</h1></body>"
        "</html>", 200);
    response_struct->resp_buf[200] = '\0';
	response_struct->content_length = strlen(response_struct->resp_buf);
	strncpy(response_struct->content_type, "text/html", 10);
    response_struct->content_type[9] = '\0';
    return 0;
}
