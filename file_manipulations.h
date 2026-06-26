#ifndef FILE_MANIPULATIONS_H
#define FILE_MANIPULATIONS_H

#include "http_response_codes.h"

int validate_file(const char* filename, const char* WEB_ROOT, response_s* response_str);

#endif //FILE_MANIPULATIONS_H included
