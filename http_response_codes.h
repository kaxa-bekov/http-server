#ifndef HTTP_RESPONSE_CODES
#define HTTP_RESPONSE_CODES


typedef struct {
    //The response codes are function pointers in the response struct
    int (*response_code)(char*);
    size_t content_length;
    char* content_type;

} response_s;

int r200_ok(char* resp_buff);
int r400_bad_request(char* resp_buff);
int r403_forbidden(char* resp_buff);
int r404_not_found(char* resp_buff);
int r405_methond_not_allowed(char* resp_buff);
int r414_uri_too_long(char* resp_buff);
int r500_internal_server_error(char* resp_buff);

//int generate_response

#endif //HTTP_RESPONSE_CODES defined
