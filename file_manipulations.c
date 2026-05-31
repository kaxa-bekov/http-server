#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include "file_manipulations.h"
#include "http_response_codes.h"



int validate_file(const char *filename, int client, const char *WEB_ROOT){
   
    //Default path
    if(strcmp(filename, "/") == 0) {
        filename = "/index.html";
    }

    //1. snprintf() and 414
    
    //Whatever the filename is - we want to set our Server Directory to www/
    char full_path[512];

    int result = snprintf(full_path, sizeof(full_path),"www%s", filename);
    if(result >= (int)sizeof(full_path)) {
        //Means we received a malicious request (super long)
        perror("Request is too long");
        //r414_uri_too_long(client);
        return 1;
    }

    //2. realpath() and 404 or 400/403

    //We want to CANONICALIZE the path by resolving any .. or . or collapsing double / and returning a flat absolute path
    //to then check if we are still inside of out allowed www directory.
    
    char resolved_path[PATH_MAX];
    if(realpath(full_path, resolved_path) == NULL){
        //File does not exist
        perror("File does not exist");
        //r404_not_found(client);
        return 1;
    }else if(strncmp(resolved_path, WEB_ROOT, strlen(WEB_ROOT) != 0)){
        //Directory traversal attempted
        fprintf(stderr, "Directory Traversal Attempted! Aborting!\n");
        //r400_bad_request(client);
        return 1;
    }

    //3. stat() and 403 or 200_ok
   
    //Create (init) the stat struct locally
    struct stat file_stat;

    if(stat(resolved_path, &file_stat) != 0) {
        // stat() failed. Most likely doesnt have the permissions
        perror( "Stat failed due to permission deny"); 
        //r403_forbidden(client);
        return 1; 
    }

    //By now the stat struct should be populated
    //We check whether its a file or directory and then extract Content-Type and Content-Length.

    if(!S_ISREG(file_stat.st_mode)){
        //r404_not_found(client);
        fprintf(stderr, "Not Found\n");
    }else if(S_ISDIR(file_stat.st_mode)){
        //Is Directory. Forbidden
        //r403_forbidden(client);
        fprintf(stderr, "Trying to request a Directory.\n");
    }

    //File is valid and we can extract metadata for headers
    printf("file type and mode: %o\n File size is: %ld\n", file_stat.st_mode, (long)file_stat.st_size );

    printf("'/' path resolves to %s\n", filename);







    return 0;
}
