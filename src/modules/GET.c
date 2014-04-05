#include <stdio.h>
#include "GET.h"

void GET(int conn, request req){
  printf("GET: %s\n",req.URI);
  fflush(stdout);
}
