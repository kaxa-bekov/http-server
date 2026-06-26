#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "http_parser.h"

//Definition
int tokenizer(char *request_str, request_s *request_struct, response_s *response_struct){

    char *rem_buff = request_str;   //Traveling pointer
    char *rnrn;                     //New Line characters and an empty line check
    char *space;                    //Space
    char *empty_line;               //The empty line terminating the request
    
    char *body;                     //This will hold the reaminder of the request after the empty line.


    //Finding the empty line
    size_t empty_l;
    empty_line = strstr(rem_buff, "\r\n\r\n");
    empty_l = 4;
    if(!empty_line) { empty_line = strstr(rem_buff, "\n\n"); empty_l = 2;}
    if(!empty_line) {
        r400_bad_request(response_struct);
        fprintf(stderr, "Missing or malformed empty line. Exiting!\n");
        return 1;
        //return bad request instead
    }
    //Trimming the body section off
    body = empty_line + (int)empty_l;

    //Getting the first line (request line)
    char *req_line = rem_buff;   
    rnrn = strchr(rem_buff, '\n');
    if(rnrn == NULL) {fprintf(stderr, "Could not find '\\n' char when disecting the request line\n");return 1;}
    
    if(*(rnrn - 1) != '\r') {
        printf("Character preeceding the first '\\n' is not an '\\r'\n");
        *rnrn = 0;
        rem_buff = rnrn + 1;
    }else{
        *(--rnrn) = 0;
        rem_buff = rnrn + 2;
    }
    
    int i, num_spaces = 0;

    for(i = 0; i < (int)strlen(req_line); i++){
       printf("req_line[%d] is %c\n", i, req_line[i]);
        if(req_line[i] == ' '){
            if(req_line[i-1] == ' ' || i == 0){
                fprintf(stderr, "Bad spaces from loop.\n");
                return 1;
                //Return bad request instead
            }
            num_spaces++;
        }
    }
    
    printf("Num of spaces: %d\n", num_spaces);

    if(num_spaces != 2){
        fprintf(stderr, "Wrong number of spaces.\n");
        return 1;
        //Return bad request instead
    }


    printf("Request Line: %s\n", req_line);

    //Disecting the request line
    space = strchr(req_line, ' ');
    if(space == NULL) {fprintf(stderr, "Character look up returned NULL in first space of the request line.\n"); return 1; }
    *space = 0;
    while(*(space + 1) == ' '){ space++; }
    request_struct->method = req_line;
    req_line = ++space;
    space = strchr(req_line, ' ');
    if(space == NULL) {fprintf(stderr, "Character look up returned NULL in the second space of the request line.\n"); return 1; }
    *space = 0;
    while(*(space + 1) == ' '){ space++; }
    request_struct->path = req_line;
    req_line = ++space;
    request_struct->proto = req_line;
    if(strchr(req_line, ' ') != NULL) {fprintf(stderr, "Something went wrong, found too many spaces in the request line!\n");}

    //Getting the remeaining lines
    while(true)
    {
        rnrn = strchr(rem_buff, '\n');
        if(rnrn == NULL){fprintf(stderr, "Character look up returned NULL search for an '\\n' of the headers. (rem_buff:%s) \n", rem_buff); return 1;} 
        //Fires when there is no newline char (LF or CRLF)

        if(rnrn > rem_buff){
            if(*(rnrn - 1) != '\r'){  
                *rnrn = 0;
            }else{
                *(rnrn - 1) = *rnrn = 0;            
            }
        }       

        //Populating the headers struct portion
        disect_heads(rem_buff, request_struct);

        if((rnrn + 1) >= empty_line) {break;}

        rem_buff = ++rnrn;
    }
    return 0;
    //End of Tokenizer
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
    
    char *headers[] = {"host", "user-agent", "accept"};
    char *delim;
    int head_cmp;
    int i = 0;

    delim = strchr(rm_bf, ':');
    if(delim == NULL){fprintf(stderr, "Character look up returned NULL in search  for a delimiter of the headers. (rem_buff:%s) \n", rm_bf); return;}
    *delim = 0;
    while(*(delim + 1) == ' ') {delim++;}
    
    remove_spaces(rm_bf);

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

void remove_spaces(char *buff){
    
    char *read = buff, *write = buff;

    while(*write){
        if(*read != ' '){
            *write++ = *read;
        }
        read++;
    }
    *write = '\0';
}
