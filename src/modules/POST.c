/*
POST.c
Author: Kyle Poore

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "POST.h"
#include "../helpers.h"

void POST(int conn, request req){
  vprintf("POST: %s\n",req.URI);
  vprintf("req.body : ");
  if(verbose){
    if(write(1,req.body,req.content_length) < req.content_length){
      printf("incomplete write!\n");
    }
  }
  vprintf("\n");
  int fdfrom[2], fdto[2];
  char filename[1024];
  char *content_type;
  char buffer[1024];
  int status, died;
  int pid;
  if(chdir(root_path)){
    perror("chdir:");
  }
  memset(filename,'\0',1024);
  filename[0] = '.';
  filename[1] = '/';
  strncpy(filename+2,req.URI + 1,strlen(req.URI + 1));
  vprintf("exec: %s/%s\n",root_path,filename);
  if(pipe(fdfrom) == -1){
    exit(EXIT_FAILURE);
  }
  if(pipe(fdto) == -1){
    exit(EXIT_FAILURE);
  }
  if((pid = vfork()) == -1){
    //something went wrong
    vprintf("Unable to fork!\n");
    exit(EXIT_FAILURE);
  }else if(pid == 0){
    //child
    vprintf("child about to exec()\n");
    dup2(fdto[0],0);
    dup2(fdfrom[1],1);

    close(fdto[1]);
    close(fdfrom[0]);
    execl(filename,filename,(char*)NULL);
    fprintf(stderr,"this shouldn't happen\n");
    exit(EXIT_FAILURE);
  }else{
    //parent
    close(fdfrom[1]);
    close(fdto[0]);
    vprintf("parent waiting for child\n");
    char buffer[1024];
    memset(buffer,'\0',1024);
    unsigned int bytes_read;
    char lengthbuff[100];
    if(write(fdto[1],req.body,req.content_length) < req.content_length){
      fprintf(stderr,"error writing to child!\n");
      exit(EXIT_FAILURE);
    }
    close(fdto[1]);
    char *response = "HTTP/1.1 200 OK\r\n";
    char *content_type = "Content-Type: text/plain\r\n";
    char *transfer_encoding = "Transfer-Encoding: chunked\r\n\r\n";
    send(conn,response,strlen(response),0); 
    vprintf("Send: %s",response);
    send(conn,content_type,strlen(content_type),0);
    vprintf("Send: %s",content_type);
    send(conn,transfer_encoding,strlen(transfer_encoding),0);
    vprintf("Send: %s",transfer_encoding);

    while((bytes_read = read(fdfrom[0],buffer,1024))){
      if(bytes_read < 1){
        break;
      }
      memset(lengthbuff,'\0',100);
      sprintf(lengthbuff,"%x\r\n",bytes_read);
      vprintf("Parent: %s",lengthbuff);
      send(conn,lengthbuff,strlen(lengthbuff),0);
      vprintf("Parent: %s",buffer);
      send(conn,buffer,bytes_read,0);
      vprintf("Parent: \r\n");
      send(conn,"\r\n",2,0);
      memset(buffer,'\0',1024);
    }
    vprintf("Parent: 0\r\n\r\n");
    send(conn,"0\r\n\r\n",5,0);
    died = wait(&status);
    vprintf("Child exited: %d\n",status);
  }
}
