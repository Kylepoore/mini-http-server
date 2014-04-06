/*
  
  process_request.c
  =================

  Author: Robert Correiro
 
  Implementation of function to process HTTP requests.

*/

#include "request.h"  /* parse_request() */
#include "helpers.h" /* vprintf() */
#include <stdlib.h>  /* exit() and EXIT_FAILURE */
#include <stdio.h>  /* printf() */

/* Process HTTP request */
void process_request(int conn_fd) {
  request req;

  vprintf("Initializing request struct\n");
  init_request(&req);

  vprintf("About to parse_request()...\n");

  if (parse_request(conn_fd, &req) == -1) {
    perror("parse_request");
    exit(EXIT_FAILURE);
  }
  dispatch(conn_fd, req);
}