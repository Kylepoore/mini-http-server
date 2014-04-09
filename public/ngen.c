#include <stdio.h>

int main(int argc, char **argv){
  int i;
  for(i=0;i<1000;i++){
    printf("Child: %d\n",i);
  }
  return 0;

}
