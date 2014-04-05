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

void (*modules[])(request) = {GET,
                            POST
                           };

void dispatch(request req){
  int i;
  for(i=0;i<module_count;i++){
    if(!strncmp(req.method,module_names[i],METHOD_NAME_LENGTH)){
      modules[i](req);
      break;
    }
  }
  if(i == module_count){
    printf("error: unrecognized method.\n");
  }
  
}
