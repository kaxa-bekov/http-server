#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include "file_manipulations.h"
#include "http_response_codes.h"



int validate_file(const char *filename, const char *WEB_ROOT, response_s* response_str){
   
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
        response_str->response_code = r414_uri_too_long;
        return 1;
    }

    //2. realpath() and 404 or 400/403

    //We want to CANONICALIZE the path by resolving any .. or . or collapsing double / and returning a flat absolute path
    //to then check if we are still inside of our allowed www directory.
    
    char resolved_path[PATH_MAX];
    if(realpath(full_path, resolved_path) == NULL){
        //File does not exist
        perror("File does not exist");
        response_str->response_code = r404_not_found;
        return 1;
    }else if(strncmp(resolved_path, WEB_ROOT, strlen(WEB_ROOT) != 0)){
        //Directory traversal attempted
        fprintf(stderr, "Directory Traversal Attempted! Aborting!\n");
        response_str->response_code = r400_bad_request;
        return 1;
    }

    //3. stat() and 403 or 200_ok
   
    //Create (init) the stat struct locally
    struct stat file_stat;

    if(stat(resolved_path, &file_stat) != 0) {
        // stat() failed. Most likely doesnt have the permissions
        perror( "Stat failed due to permission deny"); 
        response_str->response_code = r403_forbidden;
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


    //Reading the extension of the file requested                      
    char extension[16];
 
    char* last_dot = strrchr(resolved_path, '.');
    strncpy(extension, ++last_dot, sizeof(extension));
  
    //Mapping the extension to a MIME type
    struct MimeType{
           const char* extension;
           const char* type;
    };
  
    struct MimeType mime_types[] = {
          {"html", "text/html"},
          {"htm", "text/html"},
          {"css", "text/css"},
          {"js", "application/javascript"},
          {"png", "image/png"},
          {"jpg", "image/jpeg"},
          {"jpeg", "image/jpeg"},
          {"gif", "image/gif"},
          {"json", "application/json"},
    };
    //Size and Mime Type
    size_t content_length = file_stat.st_size;
    char content_type[32];
  
    int array_len = sizeof(mime_types)/sizeof(mime_types[0]);
  
    for(int i = 0; i < array_len; i++){
          if(strcmp(extension, mime_types[i].extension) == 0){
          strncpy(content_type, mime_types[i].type, sizeof(content_type))    ;        
          }
    }
  
      
    printf("Content-Length: %ld\n", content_length);
    printf("Extension: %s\n", extension);
    printf("Content-Type: %s\n", content_type);



    //Populating the response struct
    response_str->content_length = content_length;
    response_str->content_type = content_type;
    response_str->response_code = r200_ok;

    return 0;
}
