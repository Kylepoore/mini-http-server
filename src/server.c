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

void process_request(int sockfd) {

}

int main (void) {
  int listen_fd;
  int new_fd;
  int status;
  int reuse_val = 1;
  char in_addr[INET6_ADDRSTRLEN];
  struct sigaction sa;
  struct addrinfo hints;
  struct addrinfo *res;
  struct addrinfo *p;
  struct sockaddr_storage cli_addr;
  socklen_t cli_addr_sz = sizeof cli_addr;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  /* Create socket */
  for (p = res; p != NULL; p = p->ai_next) {
    if ((listen_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    /* Prevent "Address already in use" error */
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, 
            &reuse_val, sizeof(int)) == -1) {
      perror("server: setsockopt");
      exit(1);
    }

    /* Assign socket address to socket */
    if (bind(listen_fd, res->ai_addr, res->ai_addrlen) == -1) {
      close(listen_fd);
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

  /* Make socket listen for connections */
  if (listen(listen_fd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  /* Setup main process to ignore the child process death signal,
     so we don't have to wait() for each one */
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("server: sigaction");
    exit(1);
  }

  printf("Listening for connections...");

  /* main accept() loop */
  while (1) {

    /* Wait for connection */
    new_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &cli_addr_sz);
    if (new_fd == -1) {
      perror("server: accept");
      continue;
    }

    /* Lookup connection's IP to print*/
    inet_ntop(cli_addr.ss_family, get_in_addr((struct sockaddr *)&cli_addr),
              in_addr, sizeof in_addr);
    printf("Got connection from %s\n", in_addr);
  
    /* Fork child process to serve connection */
    if (fork() == 0) {
      
      /* This is forked child process, close listening socket
         and process request */

      if (close(listen_fd) == -1) {
        perror("server: error closing listening socket in child");
        exit(EXIT_FAILURE);
      }

      process_request(new_fd);

      /* Close the accept()ed socket and exit */
      if (close(new_fd) == -1) {
        perror("server: error closing connection socket");
        exit(EXIT_FAILURE);
      }
      exit(EXIT_SUCCESS);
    } 

    /* This is the parent who doesn't need the accept()ed socket */
    if (close(new_fd) == -1) {
      perror("server: error closing connection socket in parent");
      exit(EXIT_FAILURE);
    }
  }
  return 0;
}