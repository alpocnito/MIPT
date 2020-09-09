#include "language.h"

const int INIT_SIZE = 100;
const int MAX_LEN_VARIABLE = 30;
const int EXPAND_COEF = 2;

enum States {
  IN_FUNCTION = 0,
  OUTSIDE_FUNCTION = 1
};

struct List_names {
  char** list;
  int size;
};


Token** tokenized_program = NULL;
List_names variables_names = {};
List_names functions_names = {};

#define CURRENT tokenized_program[current_token]
#define LAST_VARIABLE variables_names.list[variables_names.size]
#define LAST_FUNCTION functions_names.list[functions_names.size]

int  FindFunction(char name_function[MAX_LEN_VARIABLE]);

void FunctionAppend(char function[MAX_LEN_VARIABLE], int* position);
void VariableAppend(char variable[MAX_LEN_VARIABLE], int* position);
int  TokenAppend(char type, char symbol, double number = 0);

int    SkipWhiteSpaces(char* buffer, size_t it, size_t len, int* current_token);
size_t LexicalAnalysis(char* buffer, size_t buffer_size);
void   PrintTokens(int number_tokens);
void   ClearAll(int number_tokens);





//////////////////////////////////////////////////////////////
//
//    Give the number of function from FunctionTypes
//
//////////////////////////////////////////////////////////////
int FindFunction(char name_function[MAX_LEN_VARIABLE]) {
  assert(name_function);

  for (int i = 0; i < NUMBER_FUNCTIONS; ++i)
    if (strcmp(name_function, functions[i].name) == 0)
      return i;

  return -1;
}

//////////////////////////////////////////////////////////////
//
//    Append new function name in the functions_names.list
//
//////////////////////////////////////////////////////////////
void FunctionAppend(char function[MAX_LEN_VARIABLE], int* position) {
  assert(function);
  
  //searching
  for (int i = 0; i < functions_names.size; ++i)
    if (strcmp(function, functions_names.list[i]) == 0) {
      *position = i;
      return;
    }

  //appending
  *position = functions_names.size;
  LAST_FUNCTION = (char*)Calloc(MAX_LEN_VARIABLE, sizeof(char), LAST_FUNCTION);
  assert(LAST_FUNCTION);

  strcpy(LAST_FUNCTION, function);
  functions_names.size++;
}


//////////////////////////////////////////////////////////////
//
//    Append new variable name in the variables_names.list
//
//////////////////////////////////////////////////////////////
void VariableAppend(char variable[MAX_LEN_VARIABLE], int* position) {
  assert(variable);
  
  //printf("%d\n", variables_names.size);
  //printf("%s\n", variable);
  
  //searching
  for (int i = 0; i < variables_names.size; ++i)
    if (strcmp(variable, variables_names.list[i]) == 0) {
      *position = i;
      return;
    }

  //appending 
  *position = variables_names.size;
  LAST_VARIABLE = (char*)calloc(MAX_LEN_VARIABLE, sizeof(char));
  assert(LAST_VARIABLE); 

  strcpy(LAST_VARIABLE, variable);
  variables_names.size++;
}


//////////////////////////////////////////////////////////////
//
//       Append new token in the tokenized_program
//
//////////////////////////////////////////////////////////////
int TokenAppend(char type, char symbol, double number) {
  static int current_token = 0;
  //printf("~~%d~%d\n", symbol, current_token); 
  CURRENT = (Token*)calloc(1, sizeof(Token));
  
  assert(CURRENT);

  if (type == TOKEN_NUMBER) {
    CURRENT->type = TOKEN_NUMBER;
    CURRENT->data.value = number;
  }
  else {
    CURRENT->type = type;
    CURRENT->data.type = symbol;
  }
  current_token++;
  return current_token;
}


int SkipWhiteSpaces(char* buffer, size_t it, size_t len, int* current_token) {
  assert(buffer);
  
  while (len > it && (buffer[it] == ' '  || 
                      buffer[it] == '\n' || 
                      buffer[it] == '\t')) {
    if (buffer[it] == '\n')
      *current_token = TokenAppend(TOKEN_SYMBOL, '\n');
    it++;
  }

  if (len > it && buffer[it] == '#')  {
    while (len > it && buffer[it] != '\n')
      it++;
    it = SkipWhiteSpaces(buffer, it, len, current_token);
  }
  return it;
}




size_t LexicalAnalysis(char* buffer, size_t buffer_size) {
  assert(buffer);
  
  // Init result containers
  tokenized_program    = (Token**)Calloc(INIT_SIZE, sizeof(tokenized_program[0]),    tokenized_program);
  variables_names.list =  (char**)Calloc(INIT_SIZE, sizeof(variables_names.list[0]), variables_names.list);
  functions_names.list =  (char**)Calloc(INIT_SIZE, sizeof(functions_names.list[0]), functions_names.list);

  int current_token    = 0;
  int current_function = 0;
  
  int len_tokens    = INIT_SIZE;
  int len_variables = INIT_SIZE;
  int len_functions = INIT_SIZE;
  
  // main while
  size_t current_char = 0;
  int state = OUTSIDE_FUNCTION;
  while (current_char < buffer_size) {
    
    current_char = SkipWhiteSpaces(buffer, current_char, buffer_size, &current_token);
    if (current_char == buffer_size) break;

    //printf("%c-%d\n", buffer[current_char], current_char);
    
    // expand containers if need
    if (current_token + 2 == len_tokens) {
      tokenized_program = (Token**)my_realloc(tokenized_program, len_tokens, \
                                              len_tokens * EXPAND_COEF, sizeof(tokenized_program[0]));
      len_tokens *= EXPAND_COEF;
    }

    if (variables_names.size + 2 == len_variables) {
      variables_names.list = (char**)my_realloc(variables_names.list, len_variables, \
                                           len_variables * EXPAND_COEF, sizeof(variables_names.list[0]));
      len_variables *= EXPAND_COEF;
    }
 
    if (current_function + 2 == len_functions) {
      functions_names.list = (char**)my_realloc(functions_names.list, len_functions, \
                                           len_functions * EXPAND_COEF, sizeof(functions_names.list[0]));
      len_functions *= EXPAND_COEF;
    }   

    // if TOKEN_NUMBER
    if (isdigit(buffer[current_char])) {
      
      int digit_size = 0;
      double digit = 0;
      sscanf(buffer + current_char, "%lg%n", &digit, &digit_size);

      current_char += digit_size;
      current_token = TokenAppend(TOKEN_NUMBER, 0, digit);
      continue;
    }

    if (isalnum(buffer[current_char])) {
      
      int word_size = 0;
      char word[MAX_LEN_VARIABLE] = "-";
      sscanf(buffer + current_char, "%[a-zA-Z0-9_]%n", word, &word_size);
      
      current_char += word_size;
      current_char = SkipWhiteSpaces(buffer, current_char, buffer_size, &current_token);
      
      // if Special word
      if (strcmp(word, "endf")  == 0) { 
        current_token = TokenAppend(TOKEN_SYSTEM, SYS_END_FUNC);  
        state = OUTSIDE_FUNCTION; 
        continue; 
      }
      if (strcmp(word, "func") == 0)
        continue;

      int j = 0;
      for (j = 0; j < NUMBER_SYSTEMS; ++j)
        if (strcmp(word, system_names[j]) == 0) { 
          current_token = TokenAppend(TOKEN_SYSTEM, j);
          break;
        }
      if (j != NUMBER_SYSTEMS) continue;


      
      int number_function = FindFunction(word);
      
      // if TOKEN_GLOBAL_FUNCTION 
      if (buffer[current_char] == '(' && (state == OUTSIDE_FUNCTION || number_function < 0)) {
        state = IN_FUNCTION;
        
        FunctionAppend(word, &number_function);
        current_token = TokenAppend(TOKEN_GLOBAL_FUNCTION, number_function);
        continue;
     }
      
      // if TOKEN_FUNCTION
      if (buffer[current_char] == '(' && state == IN_FUNCTION) {
        
        assert(number_function >= 0);
        current_token = TokenAppend(TOKEN_FUNCTION, number_function);
        continue;
      }

      // if TOKEN_VARIABLE
      else {
                
        int number_variable = 0;
        VariableAppend(word, &number_variable);
        current_token = TokenAppend(TOKEN_VARIABLE, number_variable);
        continue;
      }
    }
    
    if (buffer[current_char] == '+') {
      current_char++;
      current_token = TokenAppend(TOKEN_OPERATOR, OP_PLUS);
      continue;
    }
    
    if (buffer[current_char] == '-') {
      current_char++;
      current_token = TokenAppend(TOKEN_OPERATOR, OP_MINUS);
      continue;
    }
    
    if (buffer[current_char] == '*') {
      current_char++;
      current_token = TokenAppend(TOKEN_OPERATOR, OP_MULT);
      continue;
    }
    
    if (buffer[current_char] == '/') {
      current_char++;
      current_token = TokenAppend(TOKEN_OPERATOR, OP_DIV);
      continue;
    }
    
    if (buffer[current_char] == '^') {
      current_char++;
      current_token = TokenAppend(TOKEN_OPERATOR, OP_POW);
      continue;
    }
    
    if (buffer[current_char] == '>') {
      current_char++;
      if (buffer[current_char] == '=') {
        current_token = TokenAppend(TOKEN_OPERATOR, OP_GE);
        current_char++;
      }
      else
        current_token = TokenAppend(TOKEN_OPERATOR, OP_G);
      continue;
    }
    
    if (buffer[current_char] == '<') {
      current_char++;
      if (buffer[current_char] == '=') {
        current_token = TokenAppend(TOKEN_OPERATOR, OP_LE);
        current_char++;
      }
      else
        current_token = TokenAppend(TOKEN_OPERATOR, OP_L);
      continue;
    }

    if (buffer[current_char] == '=' && buffer[current_char+1] == '=') {
      current_char += 2;
      current_token = TokenAppend(TOKEN_OPERATOR, OP_E);
      continue;
    }
        
    current_token = TokenAppend(TOKEN_SYMBOL, buffer[current_char]);
    current_char++;
  }
  
  printf("---------------------------------------------------------\n");
  for (int i = 0; i < functions_names.size; ++i)
    printf("%s\n", functions_names.list[i]);
  printf("---------------------------------------------------------\n");
  for (int i = 0; i < variables_names.size; ++i)
    printf("%s\n", variables_names.list[i]);
  printf("---------------------------------------------------------\n");
 
  return current_token;
}

void PrintTokens(int number_tokens) {
  printf("      TOKENS\n");
  
  for (int i = 0; i < number_tokens; ++i) {
    printf("%2d ", i);
    switch (tokenized_program[i]->type) {
      
      case TOKEN_OPERATOR:
        printf("OPERATOR: ");
        printf("%s\n", operators[tokenized_program[i]->data.type - 1]);
        break;

      case TOKEN_VARIABLE:
        printf("VARIABLE: ");
        printf("%d\n", tokenized_program[i]->data.type);
        break;
    

      case TOKEN_FUNCTION:
        printf("FUNCTION: ");
        printf("%d\n", tokenized_program[i]->data.type);
        break;
   

      case TOKEN_GLOBAL_FUNCTION: 
        printf("GLOBAL_FUNCTION: ");
        printf("%d\n", tokenized_program[i]->data.type);
        break;
    

      case TOKEN_SYMBOL:
        printf("SYMBOL: ");
        printf("%c\n", tokenized_program[i]->data.type);
        break;

      case TOKEN_SYSTEM:
        printf("SYSTEM: ");
        printf("%d\n", tokenized_program[i]->data.type);
        break;
    
      case TOKEN_NUMBER:
        printf("NUMBER: ");
        printf("%lg\n", tokenized_program[i]->data.value);
        break;

      default:
        printf("Unknown TOKEN\n");
        abort();
        break;
    }
  }
}

void ClearAll(int number_tokens) {
  
  for (int i = 0; i < functions_names.size; ++i)
    Free(functions_names.list[i]);
  Free(functions_names.list);
  functions_names.list = NULL;
  functions_names.size = 0;

  for (int i = 0; i < variables_names.size; ++i)
    Free(variables_names.list[i]);
  Free(variables_names.list);
  variables_names.list = NULL;
  variables_names.size = 0;
  
  if (tokenized_program != NULL) {
    for (int i = 0; i < number_tokens; ++i)
      Free(tokenized_program[i]);
    Free(tokenized_program);
    tokenized_program = NULL;
  }
}


