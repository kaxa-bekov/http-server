#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "http_response_codes.h"

//Declaring the needed types and functions

typedef struct {
    char *host;
    char *user_agent;
    char *accept;
} headers_s;

typedef struct {
    char *method;
    char *path;
    char validated_path[512];
    char *proto;
    headers_s headers;
} request_s;

int tokenizer(char *request_string, request_s* request_struct, response_s *response_struct);

void disect_heads(char* remaining_buffer, request_s* request_struct, response_s * response_struct);

void remove_spaces(char *buffer);

#endif ///HTTP_PARSER_H included
