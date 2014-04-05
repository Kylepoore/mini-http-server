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

/* Reads a line from given socket */
ssize_t readline(int sockfd, void *vptr, size_t maxlen) {
  ssize_t n, chars_read;
  char c, *buffer;
  int cr = 0;
  buffer = vptr;

  for (n = 1; n < maxlen; n++) {
    vprintf("reading from socket: %d, ", sockfd);
    if ((chars_read = read(sockfd, &c, 1)) == 1) {
      *buffer++ = c;
      vprintf("%x\n",(int)c);
      /* Check for CR */
      if (c == '\r') {
        /* Found a CF, set flag */
        cr = 1;  
        continue;
      }

      /* If we've seen a CR, check for LF */
      if (cr == 1 && c == '\n') {
        /* Found a CRLF */
        
        /* If it's at the beginning, ignore it */
        if (n == 2) {
          continue;
        }
        /* Otherwise, it marks end-of-line */
        else {
          break;
        }
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

/* Removes trailing LWS (linear white space) from string */
void strip(char *buffer) {
  int sz = strlen(buffer) - 1;

  while (!isalnum(buffer[sz]) && sz >= 0) {
    buffer[sz--] = '\0';
  }
}
