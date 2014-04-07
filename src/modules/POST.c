#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "POST.h"
#include "../helpers.h"

void POST(int conn, request req){
  vprintf("POST: %s\n",req.URI);
  char filename[1024];
  char *content_type;
  char buffer[1024];
  int status, died;
  int pid;
  chdir(root_path);
  memset(filename,'\0',1024);
  filename[0] = '.';
  filename[1] = '/';
  strncpy(filename+2,req.URI + 1,strlen(req.URI + 1));
  vprintf("exec: %s/%s\n",root_path,filename);
  if((pid = fork()) == -1){
    //something went wrong
    vprintf("Unable to fork!\n");
  }else if(pid == 0){
    //child
    vprintf("child about to exec()\n");
    execlp(filename,filename,(char*)NULL);
    vprintf("this shouldn't happen\n");
    exit(EXIT_FAILURE);
  }else{
    //parent
    vprintf("parent waiting for child\n");
    died = wait(&status);
    if(!status){
      vprintf("something went wrong.\n");
    }
  }
}
