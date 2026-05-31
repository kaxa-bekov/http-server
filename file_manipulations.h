#include <sys/stat.h>

#ifndef FILE_MANIPULATIONS_H
#define FILE_MANIPULATIONS_H

typedef struct {
    int conten_length;
    char content_type;
} metadata_header_struct;

int validate_file(const char* filename, int client_fd, const char* WEB_ROOT);

#endif //FILE_MANIPULATIONS_H included
