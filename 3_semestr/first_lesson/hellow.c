#include <unistd.h>
#include <stdlib.h>

int main()
{
  const char* buf = "Hello";
  size_t buf_size = 6;

  write(STDOUT_FILENO, buf, buf_size);

  return 0;
}
