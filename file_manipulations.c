#include "file_manipulations.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>



int validate_file(char *filename, const char *WEB_ROOT, response_s* response_str){
   
    //Default path
    if(strcmp(filename, "/") == 0) {
        strncpy(filename,"/index.html", 11);
        filename[11] = '\0';
    }else if(strncmp(filename, "/", 1) != 0){
        r400_bad_request(response_str);
        fprintf(stderr, "The reaquested path does not start with a '/'. Exiting.\n");
        return 1;
    }

    //1. snprintf() and 414
    
    //Whatever the filename is - we want to set our Server Directory to www/
    char full_path[512];

    int result = snprintf(full_path, sizeof(full_path),"www%s", filename);
    if(result >= (int)sizeof(full_path)) {
        //Means we received a malicious request (super long)
        perror("Request is too long\n");
        r414_uri_too_long(response_str);
        return 1;
    }

    //2. realpath() and 404 or 400/403

    //We want to CANONICALIZE the path by resolving any .. or . or collapsing double / and returning a flat absolute path
    //to then check if we are still inside of our allowed www directory.
    
    char resolved_path[PATH_MAX];
    if(realpath(full_path, resolved_path) == NULL){
        //File does not exist
        perror("File does not exist");
        r404_not_found(response_str);
        return 1;
    }else if(strncmp(resolved_path, WEB_ROOT, strlen(WEB_ROOT) != 0)){
        //Directory traversal attempted
        fprintf(stderr, "Directory Traversal Attempted! Aborting!\n");
        r400_bad_request(response_str);
        return 1;
    }


    //3. stat() and 403 or 200_ok
   
    //Create (init) the stat struct locally
    struct stat file_stat;

    if(stat(resolved_path, &file_stat) != 0) {
        // stat() failed. Most likely doesnt have the permissions
        perror( "Stat failed due to permission deny"); 
        r403_forbidden(response_str);
        return 1; 
    }

    //By now the stat struct should be populated
    //We check whether its a file or directory and then extract Content-Type and Content-Length.

    if(!S_ISREG(file_stat.st_mode)){
        r404_not_found(response_str);
        fprintf(stderr, "Not Found\n");
    }else if(S_ISDIR(file_stat.st_mode)){
        //Is Directory. Forbidden
        r403_forbidden(response_str);
        fprintf(stderr, "Trying to request a Directory.\n");
    }

    //File is valid and we can extract metadata for headers
    printf("file type and mode: %o\n File size is: %ld\n", file_stat.st_mode, (long)file_stat.st_size );

    //Reading the extension of the file requested                      
    char extension[16];
 
    char* last_dot = strrchr(resolved_path, '.');
    strncpy(extension, ++last_dot, sizeof(extension) - 1);
    extension[16] = '\0';
  
    printf("RealPath is: %s\n", resolved_path);

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
    response_str->content_length = file_stat.st_size;

    int array_len = sizeof(mime_types)/sizeof(mime_types[0]);
    for(int i = 0; i < array_len; i++){
          if(strcmp(extension, mime_types[i].extension) == 0){
            strncpy(response_str->content_type, mime_types[i].type, sizeof response_str->content_type - 1);
            response_str->content_type[sizeof response_str->content_type] = '\0';
          }
    }
 
    //Populating the response struct
    strncpy(filename, resolved_path, PATH_MAX - 1);
    filename[PATH_MAX-1] = '\0';
    printf("file_manipulations : resp_buf after filename strcpy is: %s\n", response_str->resp_buf);

    printf("file_manipulations : resp_buf before r200_ok is: %s\n", response_str->resp_buf);
    r200_ok(response_str);
    printf("file_manipulations : resp_buf after r200_ok is: %s\n", response_str->resp_buf);

    return 0;
}
