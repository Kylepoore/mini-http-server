/*

  helpers.h
  =========
  Author: Robert Correiro

  Interface to helper functions for our server.

*/

#ifndef HELPERS_H
#define HELPERS_H

#include <unistd.h>  /* ssize_t data type - used for a count of bytes */
#include <stdio.h>

#define vprintf(format, ...) do {            \
  if (verbose)                               \
    fprintf(stderr, format, ##__VA_ARGS__);  \
} while (0);

extern int verbose;

ssize_t readline(int sockfd, void *vptr, size_t maxlen);
void strip(char *buffer);

#endif