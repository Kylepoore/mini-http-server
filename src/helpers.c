/*

  helpers.c
  =========
  Author: Robert Correiro

  Helper functions for server.

*/

#include "helpers.h"

#include <stdlib.h>
#include <string.h>  /* strlen(), isalnum() */
#include <errno.h>  /* EINTR */
#include <ctype.h>  /* toupper() */

/* Reads a line from given socket */
ssize_t readline(int sockfd, void *vptr, size_t maxlen) {
  ssize_t n, chars_read;
  char c, *buffer;
  int cr = 0;
  buffer = vptr;

  for (n = 1; n < maxlen; n++) {
    //vprintf("reading from socket: %d, ", sockfd);
    if ((chars_read = read(sockfd, &c, 1)) == 1) {
      *buffer++ = c;
     // vprintf("%x\n",(int)c);
      

      /* Check for CR */
      if (c == '\r') {
        /* Found a CF, set flag */
        cr = 1;  
        continue;
      }

      /* If we've seen a CR, check for LF */
      if (cr == 1 && c == '\n') {
        /* Found a CRLF */

        /* Check blank line case */
        if (n == 2) {
          return n;
        }
        else {
          break;
        }
      }

      /* Otherwise, check for just LF case */
      if (c == '\n') {
        break;
      }

      /* Saw a CR but no LF, so reset flag */
      if (cr == 1) {
        cr = 0;
      }
    } 
    else if (chars_read == 0) {
      /* Didn't read anything from first call to read */
      if (n == 1) {  
        return 0;
      } 
      else {
        break;
      }
    } 
    else {
      if (errno == EINTR) {
        continue;
      }
      /* Shouldn't get here */
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  /* Null terminate */
  *buffer = 0;
  return n;
}

int read_body(int sockfd, char *buffer, int length){
  int chars_read;
  buffer = malloc(length * sizeof(char));
  if ((chars_read = read(sockfd, buffer, length)) == length){
    return length;
  }else if(chars_read < 0){
    fprintf(stderr,"Error: unable to read %d bytes from body!\n",length);
    exit(EXIT_FAILURE);
  }else{
    return chars_read;
  }
}

/* Removes trailing LWS (linear white space) from string */
void strip(char *str) {
  int sz = strlen(str) - 1;

  while (!isalnum(str[sz]) && sz >= 0) {
    str[sz--] = '\0';
  }
}

void upcase(char *str) {
  while (*str) {
    *str = toupper(*str);
    str++;
  }
}
