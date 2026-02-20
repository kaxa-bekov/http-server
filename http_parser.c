#include <stdio.h>
#include <string.h>
#include "http_parser.h"


//Definition
int tokenizer(char *request_str, request_s *reqstr){


    char *rem_buff = request_str;   //Traveling pointer
    char *space;                    //Space
    char *rnrn;                     //New Line characters and an empty line check
    char *delim;                    //Colon delimiter

    //Getting the first line (request line)
    rnrn = strchr(rem_buff, '\r');
    if(rnrn == NULL) {perror("rnrn is NULL. Cant write to NULL\n");return 1;}
    *rnrn = 0;
    char *req_line = rem_buff;
    printf("Request Line: %s\n", req_line);
    rem_buff = rnrn + 2;

    //Disecting the request line
    space = strchr(req_line, ' ');
    if(space == NULL) {perror("space is null in req_line"); return 1; }
    *space = 0;
    reqstr->method = req_line;
    req_line = space + 1;
    space = strchr(req_line, ' ');
    if(space == NULL) {perror("space is null in req_line"); return 1; }
    *space = 0;
    reqstr->path = req_line;
    req_line = space + 1;
    reqstr->proto = req_line;


    //Getting the remeaining lines
    char *headers[] = {"HOST", "USER-AGENT", "ACCEPT"};

    int cmp = 1;
    
    while(cmp != 0){
        rnrn = strchr(rem_buff, '\r');
        if(rnrn == NULL){perror("rnrn in NULL in the loop"); return 1;}
        *rnrn = 0;
        delim = strchr(rem_buff, ':');
        if(delim == NULL){perror("delim in NULL in the loop\n\r\n"); return 1;}
        *delim = 0;

        for(int i = 0;i < 3;i++){
            int head_cmp = strncasecmp(rem_buff, headers[i], 15);
            if(head_cmp == 0){
                switch (i) {
                    case 0:
                        reqstr->headers.host = delim + 2;
                        break;
                    case 1:
                        reqstr->headers.user_agent = delim + 2;
                        break;
                    case 2:
                        reqstr->headers.accept = delim + 2;
                        break;
                    default:
                        break;
                }
                break;
            }
       }

        //printf("%s: %s\n", rem_buff, (delim + 2));
        rem_buff = rnrn + 2;
        cmp = strncmp(rem_buff, "\r\n", 2);
    }


}

