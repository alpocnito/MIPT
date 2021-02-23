#ifndef MY_SYSTEM_H
#define MY_SYSTEM_H

#include <stdlib.h>
#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static int my_calloc_work = 1;
static int my_fopen_work  = 1;
static int my_system_work = 1;
static int my_fclose_work = 1;

enum SYSTEM_CALLS
{
  MY_CALLOC = 1,
  MY_FOPEN,
  MY_SYSTEM,
  MY_FCLOSE
};

void BreakSystemCall(int);
void RestoreSystemCall(int);

void* my_calloc(size_t num, size_t size);
FILE* my_fopen(const char* filename, const char* mode);
int   my_system(const char* cmd);
int   my_fclose(FILE* stream);

#pragma GCC diagnostic pop
#endif
