#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "GET.h"
#include "../helpers.h"

void GET(int conn, request req){
  char filename[1024];
  char buffer[1024];
  FILE *fp;
  vprintf("GET: %s\n",req.URI);
  chdir(root_path);
  strncpy(filename,req.URI + 1,strlen(req.URI + 1));
  fp = fopen(filename,"r");
  vprintf("%s/%s\n",root_path,filename);
  if(fp != NULL){
    //file exists
    req.status = 200;
    fseek(fp,0,SEEK_END);
    int length = ftell(fp);
    fseek(fp,0,SEEK_SET);
    char *response = "HTTP/1.1 200 OK\r\n";
    vprintf("HTTP/1.1 200 OK\r\n");
    char *content_type = "Content-Type: text/html\r\n";
    vprintf("Content-Type: text/html\r\n");
    char content_length[32];
    sprintf(content_length,"Content-Length: %d\r\n\r\n",length);
    vprintf("Content-Length: %d\r\n\r\n",length);
    int bytes_read = 0;

    send(conn,response,strlen(response),0); 
    send(conn,content_type,strlen(content_type),0);
    send(conn,content_length,strlen(content_length),0);

    while(1){
      bytes_read = fread(buffer,1,1024,fp);
      if(!bytes_read) break;
      vprintf("%s",buffer);
      send(conn,buffer,1024,0);
      if(bytes_read < 1024) break;
    }
    vprintf("sent file!\n");
  }else{
    vprintf("file not found! =)\n");
    
  }
}
