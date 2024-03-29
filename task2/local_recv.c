#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define LISTEN_BACKLOG 5
#define MY_SOCK_PATH "/tmp/test_socket"
#define BUFFER_SIZE 1024

int main(void) {
  struct sockaddr_un server_addr, peer_addr;
  memset(&server_addr, 0, sizeof(struct sockaddr_un));

  server_addr.sun_family = AF_UNIX;
  strncpy(server_addr.sun_path, MY_SOCK_PATH, sizeof(server_addr.sun_path) - 1);

  socklen_t peer_addr_size;

  int unix_socket, error, cfd, result;
  char buffer[BUFFER_SIZE];

  unix_socket = socket(AF_UNIX, SOCK_STREAM, 0);

  if (bind(unix_socket, (const struct sockaddr *)&server_addr,
           sizeof(struct sockaddr_un)) == -1) {
    printf("BIND ERROR\n");
  };

  if (listen(unix_socket, LISTEN_BACKLOG) == -1) {
    printf("LISTEN ERROR");
    // handle error listen
  }
  cfd = accept(unix_socket, (struct sockaddr *)NULL, NULL);

  result = read(cfd, buffer, BUFFER_SIZE);
  // printf("b %d\n", strncmp(buffer, "TEST", sizeof(buffer)));
  printf("SERVER: r %d %s\n", result, buffer);
  close(cfd);
  close(unix_socket);
  return EXIT_SUCCESS;
}
