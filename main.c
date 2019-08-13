#include <stdio.h>

int main(int argc, char **argv)
{
  printf("Test!\r\n");
  
  // Wait
  char c;
  c = getchar();
  return 0;
}