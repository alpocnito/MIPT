#include "my_system.h"

void BreakSystemCall(int number)
{
  switch (number)
  {
    case MY_CALLOC: my_calloc_work = 0; break;
    case MY_FOPEN:  my_fopen_work  = 0; break;
    case MY_SYSTEM: my_system_work = 0; break;
    case MY_FCLOSE: my_fclose_work = 0; break;
    default: break;
  }
}

void RestoreSystemCall(int number)
{
  switch (number)
  {
    case MY_CALLOC: my_calloc_work = 1; break;
    case MY_FOPEN:  my_fopen_work  = 1; break;
    case MY_SYSTEM: my_system_work = 1; break;
    case MY_FCLOSE: my_fclose_work = 1; break;
    default: break;
  }
}

void* my_calloc(size_t num, size_t size)
{
  if (!my_calloc_work)
    return NULL;

  return calloc(num, size);
}

FILE* my_fopen(const char* filename, const char* mode)
{
  if (!my_fopen_work)
    return NULL;

  return fopen(filename, mode);
}

int my_system(const char* cmd)
{
  if (!my_system_work)
    return -1;

  return system(cmd);
}

int my_fclose(FILE* stream)
{
  if (!my_fclose_work)
    return EOF;

  return fclose(stream);
}
