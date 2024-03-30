#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
  int rc;
  struct stat result = {0};

  if (argc != 2) {
    printf("Write filename/directory to arguments\n");
    exit(1);
  }

  rc = syscall(SYS_stat, argv[1], &result);

  if (rc == -1) {
    perror("Problem with stat");
    exit(1);
  }

  printf("Тип файла: ");
  switch (result.st_mode & S_IFMT) {
  case S_IFBLK:
    printf("блочное устройство\n");
    break;
  case S_IFCHR:
    printf("символьное устройство\n");
    break;
  case S_IFDIR:
    printf("каталог\n");
    break;
  case S_IFIFO:
    printf("FIFO/канал\n");
    break;
  case S_IFLNK:
    printf("символьная ссылка\n");
    break;
  case S_IFREG:
    printf("обычный файл\n");
    break;
  case S_IFSOCK:
    printf("сокет\n");
    break;
  default:
    printf("неизвестно?\n");
    break;
  }

  printf("ID устройства с файлом: %lu\n", result.st_dev);
  printf("Inode номер: %lu\n", result.st_ino);
  printf("Права: %lo\n", (unsigned long)result.st_mode);
  printf("Количество жестких ссылок: %lu\n", result.st_nlink);
  printf("UID: %u\n", result.st_uid);
  printf("GID: %u\n", result.st_gid);
  printf("ID устройства: %lu\n", result.st_rdev);
  printf("Размер: %lu байт\n", result.st_size);

  printf("Время последнего доступа: %lu секунд\n", result.st_atime);
  printf("Время последнего изменения: %lu секунд\n", result.st_mtime);
  printf("Время последнего смены состояния: %lu секунд\n", result.st_ctime);

  return EXIT_SUCCESS;
}
