#include "lexical_analysis.cpp"

void PrintNodes(Node* node, FILE* file);
void PrintData(FILE* file, Node* node);
int NodePrint(Node* node, const char* file_name);

void TreeDelete(Node** node) {
  if (node == NULL)    return;
  if ((*node) == NULL) return;
  
  if ((*node)->left)  TreeDelete(&((*node)->left));
  if ((*node)->right) TreeDelete(&((*node)->right));
  
  Free((*node));

  (*node) = NULL;
}

Node* CreateNode(char value, char type) {
  Node* new_node = (Node*)Calloc(1, sizeof(Node), new_node);
  assert(new_node);

  new_node->data.type = value;

  new_node->left   = NULL;
  new_node->right  = NULL;
  new_node->type   = type;

  return new_node;
}

Node* CreateNodeNumber(double value, char type) {
  Node* new_node = (Node*)Calloc(1, sizeof(Node), new_node);
  assert(new_node);

  new_node->data.value = value;

  new_node->left   = NULL;
  new_node->right  = NULL;
  new_node->type   = type;

  return new_node;
}

int NodeFind(Node* node, double element, char type) {
  assert(node);

  // We reach our element?
  if (type == TOKEN_NULL) {
    if (node->data.value == element) return 1;
  }
  else {
    if (node->type == type) return 1;
  }

  // recursive call left tree
  if (node->left != NULL)
    if (NodeFind(node->left, element, type) == 1)
      return 1;
 
  // recursive call right tree
  if (node->right != NULL)
    if (NodeFind(node->right, element, type) == 1)
      return 1;

  return 0;
}


///////////////////////////////////////////////////////////////////////////////////
//
//                            Enhance Insert function
//
// if left_data  == POISON_VALUE_PUSH, insert left
// if right_data == POISON_VALUE_PUSH, insert right
//
///////////////////////////////////////////////////////////////////////////////////
Node* TreeSetNode(Node* his_left, Node* his_right, char his_data, char his_type, \
                  double left_data = POISON_VALUE_PUSH, double right_data = POISON_VALUE_PUSH) {            
  
  Node* node_to_set = CreateNode(his_data, his_type);
  
  if (left_data == POISON_VALUE_PUSH && right_data == POISON_VALUE_PUSH) {
    node_to_set->left  = his_left;
    node_to_set->right = his_right;
  }

  if (left_data == POISON_VALUE_PUSH && right_data != POISON_VALUE_PUSH) {
    node_to_set->left = his_left;
    node_to_set->right = CreateNode(right_data, TOKEN_NUMBER);   
  }

  if (left_data != POISON_VALUE_PUSH && right_data == POISON_VALUE_PUSH) {
    node_to_set->left = CreateNode(left_data, TOKEN_NUMBER);
    node_to_set->right = his_right;
  }

  if (left_data != POISON_VALUE_PUSH && right_data != POISON_VALUE_PUSH) {
    node_to_set->left  = CreateNode(left_data, TOKEN_NUMBER);
    node_to_set->right = CreateNode(right_data, TOKEN_NUMBER);
  }  

  return node_to_set;
}  


Node* Copy(Node* node) {
  if (node == NULL) return NULL;

  Node* new_node = (Node*)Calloc(1, sizeof(Node), new_node);
  if (new_node == NULL) {
    //printf("Can not create");
    return NULL;
  }

  (new_node)->type = (node)->type;
  (new_node)->data.value = (node)->data.value;

  (new_node)->left  = Copy( ((node)->left) );
  (new_node)->right = Copy( ((node)->right) );

  return new_node;
}

////////////////////////////////////////////////////////////////////////////////////
//
//                              Print Function
//
///////////////////////////////////////////////////////////////////////////////////
int NodePrint(Node* node, const char* file_name) {
  assert(node);
  
  // printing
  FILE* output = NULL;
  if (OpenFile(file_name, "w", &output))
    return 0;
  
  fprintf(output, "digraph D {\n");
  PrintNodes(node, output);
  fprintf(output, "}");
   
  fclose(output);
  
  // calling system
  char dot_program[PATH_MAX + 50];
  sprintf(dot_program, "dot -Tps %s -o HelpFiles/graph.ps", file_name);
  
  system(dot_program);
  system("xdg-open HelpFiles/graph.ps");

  return 1;
}


void PrintData(FILE* file, Node* node) {
  assert(file);
  assert(node);
  
  switch (node->type) {
    
    case TOKEN_NUMBER:
      fprintf(file, "\"%p\" [label=\"%lg\",  \
                             color=darkseagreen2,\
                             style=filled,   \
                             shape=egg,      \
                             fontname=bold,  \
                             fontsize=20]\n", node, node->data.value);
      break;

    case TOKEN_VARIABLE:
      fprintf(file, "\"%p\" [label=\"%s\",  \
                             color=darkolivegreen1,\
                             shape=egg,     \
                             style=filled,  \
                             fontsize=20]\n", node, variables_names.list[node->data.type]);
      break;

    case TOKEN_OPERATOR: {
      
      char shape[20] = "diamond";
      char color[20] = "gold";
      char style[20] = "filled";
      switch (node->data.type) {
        
        case OP_NULL:
          strcpy(shape, "circle");
          strcpy(color, "black");
          strcpy(style, "bold");
          break;  

        case OP_PLUS:
          stpcpy(color, "gold");
          break;

       case OP_MINUS:
          strcpy(color, "orchid2");
          break;
       
       case OP_MULT:
          strcpy(color, "pink");
          break;

       case OP_DIV:
          strcpy(color, "peachpuff");
          break;
       
       case OP_POW:
          strcpy(color, "ivory");
          break;
/*
       case OP_DERIV:
          strcpy(color, "gray78");
          break;*/
      
       default:
          strcpy(shape, "parallelogram");
          strcpy(color, "gray56");
      }

      fprintf(file, "\"%p\" [label=\"%s\",  \
                             color=%s,      \
                             shape=%s,      \
                             style=%s,      \
                             fixedsize=true \
                             fontname=bold, \
                             fontsize=15]\n", node, operators[node->data.type], color, shape, style);
      }
      break;

    case TOKEN_FUNCTION: {
      fprintf(file, "\"%p\" [label=\"%s\",     \
                             color=steelblue1, \
                             shape=box,        \
                             style=filled,     \
                             fontname=bold]\n", node, functions[node->data.type].name);
      }
      break;

    case TOKEN_GLOBAL_FUNCTION: {
      fprintf(file, "\"%p\" [label=\"%s\",     \
                             color=bisque,     \
                             shape=box,        \
                             style=filled,     \
                             fontname=bold]\n", node, functions_names.list[node->data.type]);
      }
      break;

    case TOKEN_SYSTEM: {
      
      char shape[20] = "pentagon";
      char color[20] = "bisque";
      char style[20] = "filled";
      switch (node->data.type) {
        
        case SYS_NULL:
          strcpy(shape, "circle");
          strcpy(color, "black");
          strcpy(style, "bold");
          break;
  
        case SYS_WHILE:
          stpcpy(shape, "pentagon");
          break;

       case SYS_IF:
          strcpy(shape, "hexagon");
          break;
       
       case SYS_IF_ELSE:
          strcpy(shape, "hexagon");
          break;

       case SYS_RETURN:
          strcpy(shape, "septagon");
          break;
      }

      fprintf(file, "\"%p\" [label=\"%s\",     \
                             color=%s,         \
                             shape=%s,         \
                             style=%s,         \
                             fontname=bold]\n", node, system_names[node->data.type], color, shape, style);
      }
      break;
    
    case TOKEN_SYMBOL: {
       
      char shape[20] = "circle";
      if (node->data.type == '=') 
        strcpy(shape, "larrow");

      fprintf(file, "\"%p\" [label=\"%c\",     \
                             color=black,      \
                             style=bold,       \
                             shape=%s,         \
                             fontname=bold]\n", node, node->data.type, shape);
      }
      break;


    default:
      printf("Tree is broken\nUnknown type: %d\n", node->type);
      break;
  }
}  


void PrintNodes(Node* node, FILE* file) {
  assert(node);
  assert(file);

  PrintData(file, node);

  if (node->left != NULL) {
    PrintData(file, node->left);

    fprintf(file, "\"%p\" -> " ,node);
    fprintf(file, "\"%p\"\n", node->left);
    PrintNodes(node->left, file);
  }

  if (node->right != NULL) {
    PrintData(file, node->right);
    
    fprintf(file, "\"%p\" -> " ,node);
    fprintf(file, "\"%p\"\n",node->right);
    PrintNodes(node->right, file); 
 }
}


////////////////////////////////////////////////////////////////////////////////////
//
//                              Save Functions
//
////////////////////////////////////////////////////////////////////////////////////
void PreorderSaveNodes(Node* node, FILE* file) {
  assert(file);
  
  if (node == NULL) {
    fprintf(file, "@");
    return;
  }

  fprintf(file, "{");
   
  switch (node->type) {
    
    case TOKEN_NUMBER:
      fprintf(file, "%lg", node->data.value);
      break;

    case TOKEN_VARIABLE:
      fprintf(file, "%s", variables_names.list[node->data.type]);
      break;

    case TOKEN_OPERATOR:
      fprintf(file, "%s", operators[node->data.type]);
      break;

    case TOKEN_FUNCTION:
      fprintf(file, "%s", functions[node->data.type].name);
      break;

    case TOKEN_GLOBAL_FUNCTION:
      fprintf(file, "$%s", functions_names.list[node->data.type]);
      break;

    case TOKEN_SYSTEM:
      fprintf(file, "%s", system_names[node->data.type]);
      break;
 
    case TOKEN_SYMBOL:
      fprintf(file, "%c", node->data.type);
      break;
   
    default:
      printf("Tree is broken\nUnknown type: %d\n", node->type);
      break;
  }
  //fprintf(file, "{");
  
  if (node->left == NULL && node->right == NULL) {
    fprintf(file, "}");
    return;
  }

  PreorderSaveNodes(node->left,  file);
  PreorderSaveNodes(node->right, file);
  
  fprintf(file, "}");
}

int TreeSave(Node* tree, const char* file_name) {
  assert(tree);

  FILE* output = NULL;
  if (OpenFile(file_name, "w", &output))
    return 1;
  
  PreorderSaveNodes(tree, output);

  fclose(output);
  return 0;
}


int SkipSpaces(char* buffer, size_t* current, size_t buffer_size) {
  assert(buffer);
  assert(current);
  
  size_t current_val = *current;
  char c = '-';
  while ( current_val < buffer_size && ((c = buffer[current_val]) == ' ' || c == '\t' || c == '\n'))
    current_val++;
  
  if (buffer_size == (*current)) return -1;
  *current = current_val;

  return 0;
}


////////////////////////////////////////////////////////////////////////////////////
//
//                              Load Functions
//
////////////////////////////////////////////////////////////////////////////////////
int LoadData(Node** node, char* read_word) {
  assert(node);
  assert(*node);
  assert(read_word);
  
  // if empty line
  if (read_word[0] == '\0') {
    printf("Error while reading tree\n");
    abort();
  }

  for (int i = 0; i < NUMBER_OPERATORS; ++i)
      if (strcasecmp(read_word, operators[i]) == 0) {
        (*node)->data.type = i;
        (*node)->type = TOKEN_OPERATOR;
        return 1;
      }
 
  for (int i = 0; i < NUMBER_SYSTEMS; ++i)
      if (strcasecmp(read_word, system_names[i]) == 0) {
        (*node)->data.type = i;
        (*node)->type = TOKEN_SYSTEM;
        return 1;
      }
  
  for (int i = 0; i < NUMBER_FUNCTIONS; ++i)
      if (strcasecmp(read_word, functions[i].name) == 0) {
        (*node)->data.type = i;
        (*node)->type = TOKEN_FUNCTION;
        return 1;
      }
  
  // if ; symbol
  if (read_word[1] == '\0' && read_word[0] == ';') {
    (*node)->type = TOKEN_FUNCTION;
    (*node)->data.type = FUNC_NULL;
    return 1;
  }
  
  // if one symbol
  if (read_word[0] == '=' || read_word[0] == NEXT_SYMBOL) {
    (*node)->type = TOKEN_SYMBOL;
    (*node)->data.type = read_word[0];
    return 1;
  } 
 
  // if function  
  if (read_word[0] == '$') {
    (*node)->type = TOKEN_GLOBAL_FUNCTION;
    
    int function_number = 0;
    FunctionAppend(read_word + 1, &function_number);
    (*node)->data.type = function_number;
    return 1;
  }
  
  /*// if strange symbol
  if (!isalpha(read_word[0])) {
    printf("Error while reading tree: %c\n", read_word[0]);
    abort();
  } */
 
  // see it as variable
  (*node)->type = TOKEN_VARIABLE;
  
  int variable_number = 0;
  VariableAppend(read_word, &variable_number);
  (*node)->data.type = variable_number;
  return 1;
}


int PreorderLoadNodes(Node** node, char* buffer, size_t* current, size_t buffer_size) {
  assert(buffer);
  assert(current);
  assert(node);
  assert(*node);
   
  SkipSpaces(buffer, current, buffer_size);

  // saving data
  int  read_bytes = 0;
  char read_word[MAX_LEN_VARIABLE] = "kill";
  char first_char = buffer[(*current)];
  
   if (isdigit(first_char)) {
    sscanf(buffer + (*current), "%lg%n", &((*node)->data.value), &read_bytes);
    (*node)->type = TOKEN_NUMBER;

    (*current) += read_bytes;
    //printf("found_v: %lg\n", (*node)->data.value);
  }
  else {
    sscanf(buffer + (*current), "%[^{}@]%n", read_word, &read_bytes);
    (*current) += read_bytes;
    
    LoadData(node, read_word);
    //printf("found_t: %d\n", (*node)->data.type);
  } 
    
  SkipSpaces(buffer, current, buffer_size);
 
  first_char = buffer[(*current)++];
  if (first_char == '}') {
    return 0;
  }

  // left tree
  assert(first_char == '{' || first_char == '@'); 
  if (first_char == '{') { 
    (*node)->left = CreateNode(FUNC_NULL, TOKEN_FUNCTION);
    PreorderLoadNodes(&((*node)->left), buffer, current, buffer_size);
  }

  SkipSpaces(buffer, current, buffer_size);
  
  // right tree
  first_char = buffer[(*current)++];
  //printf("---%c\n", first_char);
  assert(first_char == '@' || first_char == '{');
  if (first_char == '{') { 
    (*node)->right = CreateNode(FUNC_NULL, TOKEN_FUNCTION);
    PreorderLoadNodes(&((*node)->right), buffer, current, buffer_size);
  }
  
  SkipSpaces(buffer, current, buffer_size); 
  
  assert(buffer[(*current)++] == '}'); 
  return 0;
}


Node* TreeLoad(const char* file_name) {
  assert(file_name);

  Node* tree = CreateNode(FUNC_NULL, TOKEN_FUNCTION);
   
  variables_names.list = (char**)Calloc(INIT_SIZE, sizeof(variables_names.list[0]), variables_names.list);
  functions_names.list = (char**)Calloc(INIT_SIZE, sizeof(functions_names.list[0]), functions_names.list);
  
  char*  buffer = NULL;
  size_t buffer_size = ReadFile(file_name, &buffer) - 1;
  size_t current = 0;
  
  char first_char = buffer[current++];
    if (first_char != '{') return NULL;

  PreorderLoadNodes(&(tree), buffer, &current, buffer_size);

  Free(buffer);
  return tree;
}
