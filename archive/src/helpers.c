#include "helpers.h"
#include <stdio.h> // printf
#include <stdlib.h> // exit, EXIT_FAILURE

// Public functions
void error_print_and_quit(const char * err_msg)
{
  printf(err_msg);
  exit(EXIT_FAILURE);
}