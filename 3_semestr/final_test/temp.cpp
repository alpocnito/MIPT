#include <stdio.h>
#include <unistd.h>

int main()
{
  printf("Hello\n");
  fork();
  printf(", World!\n");
  return 0;
}
