#include "../include/dbcontroller.h"

int main(int argc, char *argv[]) {
  int c;
  bool make_new_db = false;
  char *db_name = NULL;
  int db_fd = COMMON_ERROR;
  database_header *dbhdr = NULL;
  bool add_employees = false;
  char employee_details[100];
  bool show_employees = false;

  while ((c = getopt(argc, argv, "nf:ae:s")) != -1) {
    switch (c) {
    case ('n'):
      make_new_db = true;
      break;
    case ('f'):
      db_name = optarg;
      break;
    case ('a'):
      add_employees = true;
      break;
    case ('e'):
      // printf("number of employees to add = %d\n", optarg);
      // employees_details = optarg;
      strncpy(employee_details, optarg, strlen(optarg));
      break;
    case ('s'):
      show_employees = true;
      break;
    case ('?'):
      printf("Unkwown flag -%c\n", c);
      break;
    default:
      return COMMON_ERROR;
    }
  }

  if (db_name == NULL) {
    printf("Database file name must be provided\n");
    return COMMON_ERROR;
  }

  if (make_new_db) {
    db_fd = create_database(db_name);
    if (db_fd == COMMON_ERROR) {
      printf("Database cannot be created\n");
      return COMMON_ERROR;
    }
  } else {
    db_fd = open_database(db_name);
    if (db_fd == COMMON_ERROR) {
      printf("Database cannot be opened\n");
      return COMMON_ERROR;
    }

    int res = validate_database_header(db_fd, &dbhdr);

    if (res == COMMON_ERROR) {
      printf("Database header validation failed\n");
      return COMMON_ERROR;
    }
  }

  // Load Employees into memory
  if (show_employees) {
    Employee *employees = NULL;
    read_employees(db_fd, dbhdr, &employees);
    printf("Number of employees = %d\n", dbhdr->count);

    for (int i = 0; i < dbhdr->count; ++i) {
      printf("-----\n");
      display_details(&employees[i]);
      printf("-----\n");
    }
  }

  if (add_employees) {

    // Read Employees from User
    Employee *emps = NULL;
    if (parse_employee(employee_details, &emps) == COMMON_ERROR) {
      printf("Could Not Collect Employee details\n");
      return -1;
    }
    // Write to Database
    if (write_employees(db_fd, dbhdr, emps, 1) == COMMON_ERROR) {
      printf("Could Not Write Employee details into Database\n");
      return -1;
    }
    free(emps);
  }

  close(db_fd);
  free(dbhdr);
  return 0;
}
