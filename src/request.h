/*

  request.h
  =========
    
  Authors: Kyle Poore, Robert Correiro
 
  Implementation of functions to parse HTTP request headers.
 
*/

#ifndef REQUEST_H
#define REQUEST_H

#define METHOD_NAME_LENGTH  8
//#define HTTP_VERSION_LENGTH 16
//#define HEADER_NAME_LENGTH  32

#define MAX_REQ_LINE_LENGTH 1024

// typedef enum header_type{
//   Accept,
//   Accept_Charset,
//   Accept_Encoding,
//   Accept_Language,
//   Accept_Datetime,
//   Authorization,
//   Cache_Control,
//   Cookie,
//   Content_Length,
//   Content_MD5,
//   Content_type,
//   Date,
//   Expect,
//   From,
//   Host,
//   If_Match,
//   If_Modified_Since,
//   If_None_Match,
//   If_Range,
//   If_Unmodified_Since,
//   Max_Forwards,
//   Origin,
//   Pragma,
//   Proxy_Authorization,
//   Range,
//   Referer,
//   TE,
//   User_Agent,
//   Via,
//   Warning,
//   num_headers
// } header_type;


// typedef struct header {
//   header_type type;
//   char *value;
// } header;

// typedef struct request{
//   char method[METHOD_NAME_LENGTH];
//   char URI[256];
//   char version[HTTP_VERSION_LENGTH];
//   header headers[16];
//   char *body;
//   int body_len;
// }request;

typedef struct request {
  int method_index;
  int status;
  char *URI;
  // char version[HTTP_VERSION_LENGTH];
  // header headers[16];
  // char *body;
  // int body_len;
} request;

int parse_request(int conn_fd, request *req);

#endif //REQUEST_H