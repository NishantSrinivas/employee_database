#ifndef _EMPLOYEE_H_
#define _EMPLOYEE_H_

#include "common.h"
#include <string.h>

typedef struct Employee {
  char name[50];
  unsigned int age;
  char department[50];
  unsigned int income;
} Employee;

void display_details(const Employee *emp);
int parse_employee(char *employee_details, Employee **emps);

#endif
