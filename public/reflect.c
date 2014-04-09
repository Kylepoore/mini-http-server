#include <stdio.h>
#include <string.h>
int main(int argc,char **argv){
  char buffer[256+1];
  memset(buffer,'\0',sizeof(buffer));
  int bytes;
  printf("Child: Welcome to the Child\n");
  fprintf(stderr,"Welcome to the Child\n");
  while(bytes = fgets(buffer,255,stdin) > 0){
    buffer[strlen(buffer)-1] = 0;
    fprintf(stderr,"%s",buffer);
    printf("Child: %s\n",buffer);
  }
  printf("bytes %d\n",bytes);
  return 0;  
  
}
