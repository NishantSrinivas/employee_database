#ifndef _FILE_H_
#define _FILE_H_

#include "parser.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int create_database(const char *db_file_name);
int open_database(const char *db_file_name);

#endif
