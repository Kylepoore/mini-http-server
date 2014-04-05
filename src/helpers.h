/*

  helpers.h
  =========
  Author: Robert Correiro

  Interface to helper functions for our server.

*/

#ifndef HELPERS_H
#define HELPERS_H

#include <unistd.h>  /* ssize_t data type - used for a count of bytes */

ssize_t readline(int sockfd, void *vptr, size_t maxlen);
void strip(char *buffer);

#endif