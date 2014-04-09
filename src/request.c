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
#include <stdlib.h>  /* exit(), malloc(), calloc() */
#include <string.h>  /* strlen(), strchr() */
#include <unistd.h>  /* ssize_t data type */
#include <sys/select.h>  /* select() */

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

  if(strstr(buffer, "HTTP/1.1")) {
    strcpy(req->version, "HTTP/1.1\0");
  }
  else {
    strcpy(req->version, "HTTP/1.0\0");
  }
}

void parse_header_line(char *buffer, request *req) {
  int len;
  char *offset, *header;

  offset = strchr(buffer, ':');
  if (offset == NULL) {
    req->status = 400;
    req->done = 1;
    return;
  }

  len = offset - buffer;
  header = calloc(len + 1, sizeof(char));
  strncpy(header, buffer, len);
  upcase(header);

  vprintf("header: %s\n", header);

  /* Get past ':' and white space, point to start of header value */
  offset++;
  while (*offset && isspace(*offset)) {
    offset++;
  }
  /* If the value isn't there, bad request */
  if (*offset == '\0') {
    req->status = 400;
    req->done = 1;
    return;
  }

  vprintf("offset: %s\n", offset);

  if (!strcmp(header, "HOST")) {
    req->host = malloc(strlen(offset) + 1);
    strcpy(req->host, offset);
  }
  else if (!strcmp(header, "CONTENT-LENGTH")) {
    req->content_length = atoi(offset);
  }
  else if (!strcmp(header, "USER-AGENT")) {
    req->user_agent = malloc(strlen(offset) + 1);
    strcpy(req->user_agent, offset);
  }

  free(header);
}

void parse_req_body(char *buffer, request *req) {
  req->body = buffer;
  req->done = 1;
}

int parse_request(int conn_fd, request *req) {
  char buffer[MAX_REQ_LINE_LENGTH];
  int first_line = 1, rv, req_has_body = 0;
  fd_set readfds;
  struct timeval tv;


  /* Set timeout to 5.0 sec */
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  do {
    /* Clear and reset file descriptor set */
    FD_ZERO(&readfds);
    FD_SET(conn_fd, &readfds);

    rv = select(conn_fd + 1, &readfds, NULL, NULL, &tv);

    if (rv == -1) {
      perror("select");
      exit(EXIT_FAILURE);
    }
    else if (rv == 0) {
      /* Timed out */
      return -1;
    }
    else {
      readline(conn_fd, buffer, MAX_REQ_LINE_LENGTH - 1);
      strip(buffer);
      vprintf("buffer [%3zu]: %s\n", strlen(buffer), buffer);

      if (first_line) {
        parse_req_line(buffer, req);
        first_line--;  /* turn flag off */
      }
      else if (!strncmp(buffer, "\r\n", 2)) {
        /* Blank line case */
        vprintf("Blank line!\n");
        /* Check if Content-Length header was received */
        if (req->content_length <= 0) {
          req->done = 1;
        }
        else {
          /* Else got it, and know to look for the request body */
          req_has_body = 1;
          req->done = 1;
        }
      }
      else if (req_has_body) {
        parse_req_body(buffer, req);
      }
      else {
        parse_header_line(buffer, req);  
      }
    }
  } while(req->done == 0);
  if(req->content_length > 0){
    req->content_length = read_body(conn_fd,&(req->body),req->content_length);
  }
}

void init_request(request *req) {
  req->method_index = -1;
  req->status = 200;
  req->URI = NULL;
  req->host = NULL;
  req->content_length = -1;
  req->user_agent = NULL;
  req->done = 0;
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
