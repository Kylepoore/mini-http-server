#include <stdio.h>
#include "POST.h"

void POST(request req){
  printf("POST: \n%s\n",(char*)(&req));
}
