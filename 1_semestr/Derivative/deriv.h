#include "string.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <color.h>
#include <sort_lines.h>
#include <linux/limits.h>

typedef double Elem_t;
#define ELEM_SYMBOL "lg"
#define POISON_VALUE_PUSH 1337

const int MAX_LEN_ANSWER    = 100;
const int MAX_LEN_NODE_DATA = 50;

enum DataTypes {
  TYPE_NULL = 0,
  TYPE_NUMBER,
  TYPE_OPERATOR,
  TYPE_VARIABLE,
  TYPE_FUNCTION,
  TYPE_SYSTEM
};

enum OperatorTypes {
  OPERATOR_PLUS = 0,
  OPERATOR_MINUS,
  OPERATOR_MULT,
  OPERATOR_DIV,
  OPERATOR_POW,
  OPERATOR_NULL
};

enum SystemTypes {
  OPERATOR = 0,
  OPERATOR_PRINT
};

const int NUMBER_OPERATORS = 6;
const char operators[NUMBER_OPERATORS + 1] = "+-*/^ ";

enum FunctionType {
  FUNCTION_SIN = 0,
  FUNCTION_COS,
  FUNCTION_TAN,
  FUNCTION_COT,
  FUNCTION_ACOS,
  FUNCTION_ASIN,
  FUNCTION_ATAN,
  FUNCTION_COSH,
  FUNCTION_SINH,
  FUNCTION_TANH,
  FUNCTION_EXP,
  FUNCTION_LOGE,
  FUNCTION_LOG10,
  FUNCTION_NULL,
};

/////////////////// TREE /////////////////////
struct Node {
  char type;
  
  union {
    char   type;
    Elem_t value;
  } data;

  Node*   left;
  Node*   right;
};

//////////////////////////////////////////////


int TreeCompare(Elem_t first, Elem_t second) {
  return first == second;
}

void ClearStdin() {
  while (getchar() != '\n');
}

void SkipSpaces(FILE* file) {
  assert(file);
  
  char c = '-';
  while ((c = fgetc(file)) == ' ' || c == '\t' || c == '\n');
  
  ungetc(c, file);
}


/////////////////// FUNCTIONS /////////////////
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

const int NUMBER_FUNCTIONS = 13;
const int MAX_LEN_FUNCTION = 5 + 1;

struct function {
  char name[MAX_LEN_FUNCTION];
  double (*func)(double);
};

struct function functions[NUMBER_FUNCTIONS] = {
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
                                  {"log10", &log10}
                                };
//////////////////////////////////////////////



void PrintData(FILE* file, Node* node);
void PrintNodes(Node* node, FILE* file);


void  TreeDelete(Node** node);
Node* CreateNode(Elem_t value, char type);
Node* TreeSetNode(Node* his_left, Node* his_right, Elem_t his_data, char his_type,\
                   Elem_t left_data = POISON_VALUE_PUSH,\
                   Elem_t right_data = POISON_VALUE_PUSH);
Node* Copy(Node* node);


int NodeFind(Node* node, Elem_t element, char type);


int  TreeSave(Node* root, const char* file_name);
void SaveNodes(Node* node, FILE* file);
int  SaveData(FILE* file, Node* node);


int CreateLatex (const char* file_name);
int EndLatex    (const char* file_name);
int LatexNodes  (int print_symbols, Node* node, FILE* file, int same_priority);
int LatexData   (Node* node, FILE* file);

int AppendMessageLatex(const char* file_name, const char* message);
int AppendFormulaLatex(Node* node, const char* file_name, int special_symbols = 1);


double CalculateNode(Node* root);

#define DEBUG
#include "debug.h"
