#include <stdio.h>
#include "GET.h"

void GET(request req){
  printf("GET: \n%s\n",(char*)(&req));
}
