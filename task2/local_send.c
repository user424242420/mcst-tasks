#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define LISTEN_BACKLOG 50
#define MY_SOCK_PATH "/tmp/test_socket"
#define BUFFER_SIZE 1024

int main(void) {
  struct sockaddr_un server_addr, peer_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_UNIX;
  socklen_t peer_addr_size;

  int unix_socket, error, cfd;
  char buffer[BUFFER_SIZE] = {0};

  strcpy(buffer, "TEST");

  unix_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  strncpy(server_addr.sun_path, MY_SOCK_PATH, sizeof(server_addr.sun_path) - 1);

  if (connect(unix_socket, (const struct sockaddr *)&server_addr,
              sizeof(struct sockaddr_un))) {
    printf("CONNECTION ERROR\n");
  };
  send(unix_socket, buffer, sizeof buffer, 0);

  printf("CLIENT: %d %s\n\n", strlen(buffer) + 1, buffer);

  close(cfd);
  close(unix_socket);
  unlink(MY_SOCK_PATH);

  return EXIT_SUCCESS;
}
