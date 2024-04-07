#include "../include/file.h"
#include <stdio.h>

int create_database(const char *db_file_name) {
  int fd_check = open(db_file_name, O_RDONLY);
  if (fd_check != -1) {
    close(fd_check);
    printf("Database file already exits\n");
    return -1;
  }

  int fd = open(db_file_name, O_RDWR | O_CREAT, 0644);
  if (fd == -1) {
    perror("open");
    return -1;
  }

  database_header *newHeader =
      (database_header *)calloc(1, sizeof(database_header));

  if (create_database_header(fd, &newHeader) == -1) {
    printf("Oh no new header could not be created\n");
    free(newHeader);
    close(fd);
    return -1;
  }

  newHeader->count = htonl(newHeader->count);
  newHeader->file_size = htonl(newHeader->file_size);
  newHeader->magic = htonl(newHeader->magic);
  newHeader->version = htons(newHeader->version);

  lseek(fd, 0, SEEK_SET);
  write(fd, newHeader, sizeof(database_header));

  free(newHeader);
  newHeader = NULL;

  return fd;
}

int open_database(const char *db_file_name) {
  int fd = open(db_file_name, O_RDWR, 0644);
  if (fd == -1) {
    perror("open");
    return -1;
  }

  return fd;
}
