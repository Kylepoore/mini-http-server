#include <stdio.h>
#include "POST.h"

void POST(int conn, request req){
  printf("POST: %s\n",req.URI);
  fflush(stdout);
}
