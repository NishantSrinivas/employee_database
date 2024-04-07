#include "../include/parser.h"
#include <stdio.h>
#include <unistd.h>

int create_database_header(int fd, database_header **header_out) {
  if (fd < 0) {
    printf("Bad file descriptor\n");
    return COMMON_ERROR;
  }
  // Create a new header
  database_header *newHeader =
      (database_header *)calloc(1, sizeof(database_header));
  if (newHeader == NULL) {
    printf("New Database Header Could Not be created\n");
    return -1;
  }

  // Set values in new header
  newHeader->magic = MAGIC_VALUE;
  newHeader->version = VERSION;
  newHeader->count = 0;
  newHeader->file_size = sizeof(database_header);

  // set new header in return
  *header_out = newHeader;

  return COMMON_SUCCESS;
}

int validate_database_header(int fd, database_header **header_out) {
  if (fd < 0) {
    printf("Bad file descriptor\n");
    return COMMON_ERROR;
  }
  // Allocate memory for header
  database_header *header =
      (database_header *)calloc(1, sizeof(database_header));

  if (header == NULL) {
    printf("Could not create a database header\n");
    return COMMON_ERROR;
  }

  // Get file stats
  struct stat dbHeaderStat = {0};
  if (fstat(fd, &dbHeaderStat) == -1) {
    perror("fstat");
    close(fd);
    free(header);
    return COMMON_ERROR;
  }

  // Read file contents into header
  if (read(fd, header, sizeof(database_header)) != sizeof(database_header)) {
    perror("read");
    free(header);
    close(fd);
    return COMMON_ERROR;
  }

  // Convert to host endian
  header->magic = ntohl(header->magic);
  header->version = ntohs(header->version);
  header->count = ntohl(header->count);
  header->file_size = ntohl(header->file_size);

  // Check file size
  if (dbHeaderStat.st_size != header->file_size) {
    printf("Corrupted header\n");
    close(fd);
    free(header);
    return COMMON_ERROR;
  }

  // Check if it's our database file
  if (header->magic != MAGIC_VALUE) {
    printf("Magic = %d\n", header->magic);
    printf("MAGIC_VALUE = %d\n", MAGIC_VALUE);
    printf("Not a database file\n");
    free(header);
    close(fd);
    return COMMON_ERROR;
  }

  // Check version number
  if (header->version != VERSION) {
    printf("Database version mismatch\n");
    free(header);
    close(fd);
    return COMMON_ERROR;
  }

  *header_out = header;

  return COMMON_SUCCESS;
}

int read_employees(int fd, database_header *dbh, Employee **emp) {
  if (fd < 0) {
    printf("Bad file descriptor\n");
    return COMMON_ERROR;
  }

  int employee_count = dbh->count;
  Employee *employees = (Employee *)calloc(employee_count, sizeof(Employee));

  if (employees == NULL) {
    printf("Could not load memory for employees\n");
    close(fd);
    return COMMON_ERROR;
  }

  int res = read(fd, employees, employee_count * sizeof(Employee));
  if (res == COMMON_ERROR) {
    perror("read");
    close(fd);
    return COMMON_ERROR;
  }

  for (int i = 0; i < employee_count; ++i) {
    employees[i].age = ntohl(employees[i].age);
    employees[i].income = ntohl(employees[i].income);
  }

  *emp = employees;

  return COMMON_SUCCESS;
}

int write_employees(int fd, database_header *dbh, Employee *emp,
                    const int emp_count) {
  if (fd < 0) {
    printf("Bad file descriptor\n");
    return COMMON_ERROR;
  }

  int res = lseek(fd, 0, SEEK_SET);
  if (res == -1) {
    perror("lseek");
    close(fd);
    return COMMON_ERROR;
  }

  dbh->count += emp_count;
  dbh->file_size += emp_count * sizeof(Employee);

  dbh->count = htonl(dbh->count);
  dbh->file_size = htonl(dbh->file_size);
  dbh->version = htons(dbh->version);
  dbh->magic = htonl(dbh->magic);

  write(fd, dbh, sizeof(database_header));

  res = lseek(fd, 0, SEEK_END);
  if (res == -1) {
    perror("lseek");
    close(fd);
    return COMMON_ERROR;
  }

  for (int i = 0; i < emp_count; ++i) {
    emp[i].age = htonl(emp[i].age);
    emp[i].income = htonl(emp[i].income);
  }

  write(fd, emp, sizeof(Employee) * emp_count);

  return COMMON_SUCCESS;
}
