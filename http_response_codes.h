#include <stdio.h>
#include <string.h>


#ifndef HTTP_RESPONSE_CODES
#define HTTP_RESPONSE_CODES

int r200_ok(int client_fd);
int r400_bad_request(int client_fd);
int r403_forbidden(int client_fd);
int r404_not_found(int client_fd);
int r405_methond_not_allowed(int client_fd);
int r414_uri_too_long(int client_fd);
int r500_internal_server_error(int client_fd);



#endif //HTTP_RESPONSE_CODES defined
