#include <stdio.h>
#include <string.h>
#include "http_parser.h"


//Definition
int tokenizer(char *request_str, request_s *reqstr){


    char *rem_buff = request_str;   //Traveling pointer
    char *rnrn;                     //New Line characters and an empty line check
    char *space;                    //Space
    char *delim;                    //Colon delimiter

    
   

    //Getting the first line (request line)
    char *req_line = rem_buff;   
    rnrn = strchr(rem_buff, '\n');
    if(rnrn == NULL) {fprintf(stderr, "Character look up returned NULL in first new line char search.\n");return 1;}
    if(*(rnrn - 1) != '\r') {
        printf("Character preeceding the first '\\n' is not an '\\r'\n");
        *rnrn = 0;
        rem_buff = rnrn + 1;
    }else{
        *(--rnrn) = 0;
        rem_buff = rnrn + 2;
    }
    
    printf("Request Line: %s\n", req_line);
    printf("rem_buff beginning test. %c\n", *rem_buff);

    //Disecting the request line
    space = strchr(req_line, ' ');
    if(space == NULL) {fprintf(stderr, "Character look up returned NULL in first space of the request line.\n"); return 1; }
    *space = 0;
    while(*(space + 1) == ' '){ space++; }
    reqstr->method = req_line;
    req_line = ++space;
    space = strchr(req_line, ' ');
    if(space == NULL) {fprintf(stderr, "Character look up returned NULL in the second space of the request line.\n"); return 1; }
    *space = 0;
    while(*(space + 1) == ' '){ space++; }
    reqstr->path = req_line;
    req_line = ++space;
    reqstr->proto = req_line;


    //Getting the remeaining lines
    while(true){
        rnrn = strchr(rem_buff, '\n');
        if(rnrn == NULL){fprintf(stderr, "Character look up returned NULL search for an '\\n' of the headers\n"); return 1;}
        if(rnrn > rem_buff){
            if(*(rnrn - 1) != '\r'){  
                *rnrn = 0;
            }else{
                *(rnrn - 1), *rnrn = 0;            
            }
        }
        //Populating the headers struct portion
        disect_heads(rem_buff, reqstr);
        rem_buff = ++rnrn;
        if(strncmp(rem_buff, "\r\n", 2) == 0 || strncmp(rem_buff, "\n\n", 2) == 0){
            printf("Exiting the while loop slicing the headers lines. Comparison returned 0!\nComparing to '\\r\\n': %d\nComparing to '\\n\\n': %d\n",  strncmp(rem_buff, "\r\n",2), strncmp(rem_buff, "\n\n",2));
            break;
        }
    }
}

//struct nullifier  helper function definition
void str_nullifier(request_s *req_s){

    req_s->method = NULL;
    req_s->path = NULL;
    req_s->proto = NULL;
    req_s->headers.host = NULL;
    req_s->headers.user_agent = NULL;
    req_s->headers.accept = NULL;

}

void disect_heads(char* rm_bf, request_s* req_s){
    
    char *headers[] = {"host", "user-agent", "accept "};
    char *delim;
    int head_cmp;
    int i = 0;

    delim = strchr(rm_bf, ':');
    if(delim == NULL){fprintf(stderr, "Character look up returned NULL in search  for a delimiter of the headers\n"); return;}
    *delim = 0;
    
    //...remove whitespaces from from rm_bf at this point 
    
    while(*(delim + 1) == ' ') {delim++;}

    for(i = 0;i < 3;i++) {
        head_cmp = strncasecmp(rm_bf, headers[i], 15);
        if(head_cmp == 0){
            switch(i){
                case 0:
                    req_s->headers.host = ++delim;
                    break;
                case 1:
                    req_s->headers.user_agent = ++delim;
                    break;
                case 2:
                    req_s->headers.accept = ++delim;
                    break;
                default:
                    break;
            }
        }
    }
}











