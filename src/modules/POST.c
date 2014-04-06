#include <stdio.h>
#include "POST.h"
#include "../helpers.h"

void POST(int conn, request req){
  vprintf("POST: %s\n",req.URI);
}
