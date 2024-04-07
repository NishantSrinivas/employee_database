#include "../include/employee.h"
#include <stdio.h>
#include <stdlib.h>

void display_details(const Employee *emp) {
  printf("Name: %s\n", emp->name);
  printf("Age: %d\n", emp->age);
  printf("Deparment: %s\n", emp->department);
  printf("Income: %d\n", emp->income);
}

int parse_employee(char *employee_details, Employee **emps) {
  Employee *emp = (Employee *)calloc(1, sizeof(Employee));

  if (emp == NULL) {
    printf("Could not allocate memory for employees\n");
    return COMMON_ERROR;
  }

  char *name = strtok(employee_details, ",");
  if (name == NULL) {
    printf("No Name Provided\n");
    free(emp);
    return COMMON_ERROR;
  }
  char *department = strtok(NULL, ",");
  if (department == NULL) {
    printf("No Department Provided\n");
    free(emp);
    return COMMON_ERROR;
  }
  char *age = strtok(NULL, ",");
  if (age == NULL) {
    printf("No Age Provided\n");
    free(emp);
    return COMMON_ERROR;
  }
  char *income = strtok(NULL, ",");
  if (income == NULL) {
    printf("No Income Provided\n");
    free(emp);
    return COMMON_ERROR;
  }

  strncpy(emp->name, name, strlen(name));
  strncpy(emp->department, department, strlen(department));
  emp->age = atoi(age);
  emp->income = atoi(income);

  *emps = emp;

  return COMMON_SUCCESS;
}
