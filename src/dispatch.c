/*

  dispatch.c
  =========
    
  Authors: Kyle Poore, Robert Correiro
 
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "request.h"
#include "modules/GET.h"
#include "modules/POST.h"

int module_count = 2;

char *module_names[] = {"GET",
                        "POST"
                       };

void (*modules[])(int, request) = {GET,
                                   POST
                                  };

void dispatch(int conn, request req){
  if (req.method_index < module_count) {
    modules[req.method_index](conn, req);
  }
  else {
    perror("dispatch: unsupported HTTP method");
    exit(EXIT_FAILURE);
  }
}