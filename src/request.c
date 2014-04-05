#include "dispatch.h"
#include "request.h"
#include <string.h>

char *header_names[] = {
  "Accept",
  "Accept_Charset",
  "Accept_Encoding",
  "Accept_Language",
  "Accept_Datetime",
  "Authorization",
  "Cache_Control",
  "Cookie",
  "Content_Length",
  "Content_MD5",
  "Content_type",
  "Date",
  "Expect",
  "From",
  "Host",
  "If_Match",
  "If_Modified_Since",
  "If_None_Match",
  "If_Range",
  "If_Unmodified_Since",
  "Max-Forwards",
  "Origin",
  "Pragma",
  "Proxy_Authorization",
  "Range",
  "Referer",
  "TE",
  "User_Agent",
  "Via",
  "Warning"
};

request parse_request(char *buffer,int buflen){
  int i,j;
  int offset = 0;
  request req;
  int verb_length = strchr(buffer,' ') - buffer;
  strncpy(req.method,buffer,(METHOD_NAME_LENGTH > verb_length) ? verb_length : METHOD_NAME_LENGTH);
  offset += verb_length+1;
  int uri_length = strchr(buffer + offset,' ') - (buffer + offset);
  strncpy(req.URI,buffer+offset,(uri_length > 256) ? 256 : uri_length);
  offset += uri_length+1;
  int http_length = strchr(buffer + offset,'\n') - (buffer + offset);
  strncpy(req.version,buffer+offset,(http_length > HTTP_VERSION_LENGTH)?HTTP_VERSION_LENGTH:http_length);
  offset += http_length+1;
  for(i=0;i<16;i++){
    int line_length = strchr(buffer + offset,'\n') - (buffer + offset);
    if(line_length < 2) {offset += 2; break;}
    int name_length = strchr(buffer + offset,':') - (buffer + offset);
    for(j=0;j<num_headers;j++){
      if(!strncmp(header_names[j],buffer + offset,name_length)){
        req.headers[i].type = j;
        req.headers[i].value = buffer + offset;
      }
    }
    offset += line_length + 1;
  }
  req.body = buffer + offset;
  req.body_len = buflen - offset;
  return req;
}
