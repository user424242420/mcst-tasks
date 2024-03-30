#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Write file in args\n");
  }

  int rc;

  rc = syscall(SYS_open, argv[1], O_RDONLY);
  if (rc == -1) {
    fprintf(stderr, "open failed, errno = %d\n", errno);
    fprintf(
        stderr, "Sys errlist: %s\n",
        strerror(
            errno)); // sys_errlist был удален и поэтому используется strerror
    perror("Perror: ");
  }
  return EXIT_SUCCESS;
}
