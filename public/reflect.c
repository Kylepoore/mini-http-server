#include <stdio.h>
#include <string.h>
int main(int argc,char **argv){
  char buffer[256+1];
  memset(buffer,'\0',sizeof(buffer));
  int bytes = 0;
  printf("Child: Welcome to the Child\n");
  fprintf(stderr,"Welcome to the Child\n");
  while(fgets(buffer,255,stdin) != NULL){
    bytes += strlen(buffer);
    buffer[strlen(buffer)] = 0;
    fprintf(stderr,"%s\n",buffer);
    printf("Child: %s\n",buffer);
  }
  printf("bytes: %d\n",bytes);
  return 0;  
  
}
