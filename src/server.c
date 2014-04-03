#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dispatch.h"

void main(int argc, char **argv){
  if(argc < 3){
    printf("usage: method request\n");
    exit(EXIT_FAILURE);
  }
  dispatch(argv[1],argv[2]);
}
