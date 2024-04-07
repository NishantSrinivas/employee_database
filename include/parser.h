#ifndef _PARSER_H_
#define _PARSER_H_

#include "common.h"
#include "employee.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#define MAGIC_VALUE 0xF1F2

typedef struct database_header {
  unsigned int magic;
  unsigned short version;
  unsigned int count;
  unsigned int file_size;
} database_header;

int create_database_header(int fd, database_header **header_out);
int validate_database_header(int fd, database_header **header_out);
int read_employees(int fd, database_header *dbh, Employee **emp_out);
int write_employees(int fd, database_header *dbh, Employee *emp,
                    const int emp_count);
#endif
