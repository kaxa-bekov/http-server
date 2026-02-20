
#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

//Declaring the needed types and functions

typedef struct {
    char *host;
    char *user_agent;
    char *accept;
} headers_s;

typedef struct {
    char *method;
    char *path;
    char *proto;
    headers_s headers;
} request_s;

int tokenizer(char *request_string, request_s *reqstr);

#endif ///HTTP_PARSER_H included
