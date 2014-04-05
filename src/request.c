/*

  request.c
  =========
    
  Authors: Kyle Poore, Robert Correiro
 
  Functions to parse HTTP request headers.
 
*/

#include "request.h"  /* MAX_REQ_LINE_LENGTH  constant*/
#include "helpers.h"  /* readline(), strip() helpers */
#include "dispatch.h"

#include <stdio.h>  /* printf() */
#include <string.h>  /* strlen() */
#include <unistd.h>  /* ssize_t data type */

// char *header_names[] = {
//   "Accept",
//   "Accept_Charset",
//   "Accept_Encoding",
//   "Accept_Language",
//   "Accept_Datetime",
//   "Authorization",
//   "Cache_Control",
//   "Cookie",
//   "Content_Length",
//   "Content_MD5",
//   "Content_type",
//   "Date",
//   "Expect",
//   "From",
//   "Host",
//   "If_Match",
//   "If_Modified_Since",
//   "If_None_Match",
//   "If_Range",
//   "If_Unmodified_Since",
//   "Max-Forwards",
//   "Origin",
//   "Pragma",
//   "Proxy_Authorization",
//   "Range",
//   "Referer",
//   "TE",
//   "User_Agent",
//   "Via",
//   "Warning"
// };

int parse_req_line(char *buffer, request *req) {
  int i, len;
  char *offset;

  /* Parse HTTP verb */
  len = strchr(buffer, ' ') - buffer;
  for (i = 0; i < module_count; i++) {
    if (!strncmp(module_names[i], buffer, len)) {
      req->method_index = i;
      break;
    }
  }

  if (i == module_count) {
    req->method_index = 2;
  }

  offset = buffer + len;

  /* Seek offset to start of URI */
  while (*offset && isspace(*offset)) {
    offset++;
  }

  // TODO: handle null case - ptr to end of URI (first space after URI) 
  // could be null
  len = strchr(offset, ' ') - offset;
  req->URI = calloc(len + 1, sizeof(char));
  strncpy(req->URI, offset, len);
  return 0;
}

void parse_header_line(char *buffer, request *req) {
  // TODO: change to int, so you can check return code in parse_request

}

int parse_request(int conn_fd, request *req) {
  char buffer[MAX_REQ_LINE_LENGTH];
  int first_line = 1;

  ssize_t tmp;
  /* TODO: change while condition to terminate when we've parsed all lines 
     relevant to us, instead of going through each line */
  while ((tmp = readline(conn_fd, buffer, MAX_REQ_LINE_LENGTH - 1)) > 0) {
    strip(buffer);
    printf("buffer [%3zu]: %s\n", strlen(buffer), buffer);
    if (first_line) {
      if (parse_req_line(buffer, req)) {
        break;
      }
      first_line--;  /* turn flag off */
    }
    else {
      parse_header_line(buffer, req);  
    }
  }
}


// request parse_request(char *buffer,int buflen){
//   int i,j;
//   int offset = 0;
//   request req;
//   int verb_length = strchr(buffer,' ') - buffer;
//   strncpy(req.method,buffer,(METHOD_NAME_LENGTH > verb_length) ? verb_length : METHOD_NAME_LENGTH);
//   offset += verb_length+1;
//   int uri_length = strchr(buffer + offset,' ') - (buffer + offset);
//   strncpy(req.URI,buffer+offset,(uri_length > 256) ? 256 : uri_length);
//   offset += uri_length+1;
//   int http_length = strchr(buffer + offset,'\n') - (buffer + offset);
//   strncpy(req.version,buffer+offset,(http_length > HTTP_VERSION_LENGTH)?HTTP_VERSION_LENGTH:http_length);
//   offset += http_length+1;
//   for(i=0;i<16;i++){
//     int line_length = strchr(buffer + offset,'\n') - (buffer + offset);
//     if(line_length < 2) {offset += 2; break;}
//     int name_length = strchr(buffer + offset,':') - (buffer + offset);
//     for(j=0;j<num_headers;j++){
//       if(!strncmp(header_names[j],buffer + offset,name_length)){
//         req.headers[i].type = j;
//         req.headers[i].value = buffer + offset;
//       }
//     }
//     offset += line_length + 1;
//   }
//   req.body = buffer + offset;
//   req.body_len = buflen - offset;
//   return req;
// }