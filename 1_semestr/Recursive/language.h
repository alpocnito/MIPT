#include "string.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <color.h>
#include <sort_lines.h>
#include <linux/limits.h>
#include <locale.h>
#include "my_conv.cpp"

#define POISON_VALUE_PUSH 1337

const int MAX_NUMBER_VARIABLES = 100;

//setlocale(LC_ALL, "en-EN.ISO-8859-5");



enum TokenTypes {
  TOKEN_NULL = 0,
  TOKEN_NUMBER = 1,
  TOKEN_OPERATOR = 2,
  TOKEN_VARIABLE = 3,
  TOKEN_FUNCTION = 4,
  TOKEN_GLOBAL_FUNCTION = 5,
  TOKEN_SYSTEM = 6,
  TOKEN_SYMBOL = 7
};
const char NEXT_SYMBOL = ',';

enum OperatorType         {OP_NULL = 0, OP_PLUS, OP_MINUS, OP_MULT, OP_DIV, OP_POW, OP_G, OP_GE, OP_L, OP_LE, OP_E, OP_NE};
const char* operators[] = {"op",        "+",     "-",      "*",     "/",    "^",    ">",  ">=",  "<",  "<=",  "==", "!=" };

const int NUMBER_OPERATORS = sizeof(operators) / sizeof(char*);



const char* system_names[] = \
                 {";",         "if",   "else",   "endif",    "while",   "endwhile",    "endf",       "endp",    "ret",   "for",   "endfor",               "if-else" };
enum SystemTypes { SYS_NULL = 0, SYS_IF, SYS_ELSE, SYS_END_IF, SYS_WHILE, SYS_END_WHILE, SYS_END_FUNC, SYS_END_P, SYS_RETURN, SYS_FOR, SYS_END_FOR,          SYS_IF_ELSE};

const int NUMBER_SYSTEMS = sizeof(system_names) / sizeof(char*);
const int MAX_LEN_SYSTEM_NAME = 40;

enum FunctionTypes {
  FUNC_NULL = 0,
  FUNC_SIN,
  FUNC_COS,
  FUNC_TAN,
  FUNC_COT,
  FUNC_ACOS,
  FUNC_ASIN,
  FUNC_ATAN,
  FUNC_COSH,
  FUNC_SINH,
  FUNC_TANH,
  FUNC_EXP,
  FUNC_LOGE,
  FUNC_LOG10,
  FUNC_PRINT
};

////////////////// TOKENS ////////////////////
struct Token {
  int type;
  
  union {
    int   type;
    double value;
  } data;
};
//////////////////////////////////////////////

/////////////////// TREE /////////////////////
struct Node {
  int type;
  
  union {
    int    type;
    double value;
  } data;

  Node*   left;
  Node*   right;
};
//////////////////////////////////////////////

/////////////////// FUNCTIONS /////////////////
double my_tan(double x);
double my_cot(double x);
double my_print(double a);

double my_tan(double x) {
  assert(isfinite(x));
  assert(cos(x) != 0);

  return sin(x) / cos(x);
}

double my_cot(double x) {
  assert(isfinite(x));
  assert(x != 0);

  return cos(x) / sin(x);
}

double my_print(double a) {
  printf("%lg\n", a);
  return a;
}

const int MAX_LEN_FUNCTION = 5 + 1;

struct function {
  char name[MAX_LEN_FUNCTION];
  double (*func)(double);
};

struct function functions[] = {
                                  {";",     &sin},
                                  {"sin",   &sin},
                                  {"cos",   &cos},
                                  {"tan",   &my_tan},
                                  {"cot",   &my_cot},
                                  {"acos",  &acos},
                                  {"asin",  &asin},
                                  {"atan",  &atan},
                                  {"cosh",  &cosh},
                                  {"sinh",  &sinh},
                                  {"tanh",  &tanh},
                                  {"exp",   &exp},
                                  {"loge",  &log},
                                  {"log10", &log10},
                                  {"put",   &my_print},
                                  {"get",   &my_print},
                                  {"deriv", &my_print}
                                };

const int NUMBER_FUNCTIONS = sizeof(functions) / sizeof(functions[0]);
//////////////////////////////////////////////

//#define DEBUG
#include "debug.h"
char* my_realloc(void* old_array_void, size_t old_size, size_t new_size, size_t one_elem);

char* my_realloc(void* old_array_void, size_t old_size, size_t new_size, size_t one_elem) {
   assert(old_array_void != NULL);
   
   char* old_array = (char*)old_array_void;
   char* new_array = (char*)Calloc(new_size * one_elem, sizeof(char), new_array);
   
   for (size_t i = 0; i < one_elem * old_size; ++i)
      new_array[i] = old_array[i];
   
   Free(old_array_void);   
   return new_array;
}


#include "funny_language.h"
