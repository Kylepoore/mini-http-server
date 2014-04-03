#include <string.h>
#include <stdio.h>
#include "GET.h"
#include "POST.h"

#define METHOD_NAME_LENGTH 10

int module_count = 2;

char *module_names[] = {"GET",
                        "POST"
                       };

void (*modules[])(char*) = {GET,
                            POST
                           };

void dispatch(char *method, char *request){
  int i;
  for(i=0;i<module_count;i++){
    if(!strncmp(method,module_names[i],METHOD_NAME_LENGTH)){
      modules[i](request);
      break;
    }
  }
  if(i == module_count){
    printf("error: unrecognized method.\n");
  }
}
