#ifndef HTTP_RESPONSE_CODES
#define HTTP_RESPONSE_CODES
//Might include stddef.h just for the size_t type if remove other headers from the source file
typedef struct {
    char *resp_buf;
    size_t content_length;
    char* content_type;
} response_s;

int r200_ok(response_s *response_struct);
int r400_bad_request(response_s *response_struct);
int r403_forbidden(response_s *response_struct);
int r404_not_found(response_s *response_struct);
int r405_method_not_allowed(response_s *response_struct);
int r414_uri_too_long(response_s *response_struct);
int r500_internal_server_error(response_s *response_struct);

#endif //HTTP_RESPONSE_CODES defined
