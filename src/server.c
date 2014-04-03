/*
** server.c
**
** Author: Robert Correiro
*/

#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* exit */
#include <string.h>  /* memset */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <signal.h> /* sigaction */

#define PORT "8080"
#define BACKLOG 10  // number of pending connections server queue holds

void sigchld_handler(int s) {
  // Wait for all dead processes
  // Use a non-blocking call so this signal handler won't block
  // in a child was cleaned up elsewhere.
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

// returns ptr to in_addr (IPv4) or in6_addr (IPv6) struct for inet_ntop()
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &( ((struct sockaddr_in *)sa) -> sin_addr );
  }
  return &( ((struct sockaddr_in6 *)sa) -> sin6_addr );
}

int main (void) {
  int sockfd;
  int new_fd;
  int status;
  int reuse_val = 1;
  char in_addr[INET6_ADDRSTRLEN];
  struct sigaction sa;
  struct addrinfo hints;
  struct addrinfo *res;
  struct addrinfo *p;
  struct sockaddr_storage their_addr;
  socklen_t their_addr_sz = sizeof their_addr;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  for (p = res; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
            &reuse_val, sizeof(int)) == -1) {
      perror("server: setsockopt");
      exit(1);
    }

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    return 2;
  }

  freeaddrinfo(res);

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("server: sigaction");
    exit(1);
  }

  printf("Listening for connections...");
  while (1) {
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &their_addr_sz);
    if (new_fd == -1) {
      perror("server: accept");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              in_addr, sizeof in_addr);
    printf("Got connection from %s\n", in_addr);
  
    if (!fork()) {
      // child doesn't need listening socket
      close(sockfd);

      // child's clean up
      close(new_fd);
      exit(0);
    } 
    // parent doesn't need this socket
    close(new_fd);
  }
  return 0;
}