#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>

#define soft_assert(condition) if ((condition) == 0) { printf("Fatal error in: %s\nFunction: %s\n\n", #condition, function); abort(); }
#define hard_assert(condition) if ((condition) == 0) { StackDump(stack, "Error in: " #condition, file, line, function, 0); abort(); }
#define Assert(stack) IsStackOk(stack, __FILE__, __LINE__, __PRETTY_FUNCTION__)

typedef long long Kanary_t; //!< protection for stack
typedef double      Elem_t;  //!< type of stack elements. Change StackDump to
#define STACK_SYMBOL "%lg"

const char             LEVEL_SECURITY  = 0;         //!< if 0, will dump the stack on errors, if 1, will printf small line of errors
const size_t           INIT_STACK_SIZE = 10;        //!< initial size of the stack
const size_t           EXPAND_COEF     = 2;         //!< new size of the stack = old size * EXPAND_COEF
const Elem_t           POISON_VALUE    = 0;         //!< values for empty data elements
const Kanary_t         KANARY_CONST    = 0xBEDABABA;//!< prtection for stack

struct Stack_t {
  Kanary_t           chicken1;
  size_t             size;
  size_t             number_elem;
  unsigned long long hash;
  char*              data;
  Kanary_t           chicken2;
};

bool StackInit    (Stack_t* stack, size_t stack_size = INIT_STACK_SIZE);
void StackDisinit (Stack_t* stack);

void StackDump  (Stack_t* stack, const char reason[], const char* file, int line, const char* function, bool stack_ok);
void PrintStack (Stack_t* stack);

unsigned long long StackHash(Stack_t* stack);
unsigned long long Hash(const void* const struc, const size_t nbytes);

void IsStackOk(Stack_t* stack, const char* file, int line, const char* function);

bool   StackPush    (Stack_t* stack, Elem_t value);
bool   ExpandStack  (Stack_t* stack);
Elem_t StackPop     (Stack_t* stack, int* error);
bool   SqueezeStack (Stack_t* stack);
