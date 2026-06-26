#ifndef HTTP_RESPONSE_CODES
#define HTTP_RESPONSE_CODES

#include <stddef.h>

typedef struct {
    char resp_buf[4096];
    size_t content_length;
    char content_type[32];
} response_s;

int r200_ok(response_s *response_struct);
int r400_bad_request(response_s *response_struct);
int r403_forbidden(response_s *response_struct);
int r404_not_found(response_s *response_struct);
int r405_method_not_allowed(response_s *response_struct);
int r414_uri_too_long(response_s *response_struct);
int r500_internal_server_error(response_s *response_struct);

#endif //HTTP_RESPONSE_CODES defined
