#include "tree.cpp"
#include "middleend.cpp"

int Current = 0;
#define DATA tokenized_program[Current]->data.type
#define TYPE tokenized_program[Current]->type

int Number_white_spaces = 0;
int Number_new_lines = 0;

#define CHECK_NEW_LINE while (TYPE == TOKEN_SYMBOL && DATA == '\n') {\
    Current++;\
    Number_new_lines++;\
  }

#define PRINT { \
  CHECK_NEW_LINE\
  for (int i = 0; i < Number_white_spaces; ++i) \
    printf(" | ");\
  printf("%s:%d DATA:%d TYPE:%d\n", __PRETTY_FUNCTION__, Current, DATA, TYPE);\
  Number_white_spaces++;\
}

#define return_node { \
  Number_white_spaces--; \
  CHECK_NEW_LINE\
  for (int i = 0; i < Number_white_spaces; ++i) \
    printf(" | ");\
  printf("go_out:%d\n", Current);\
  return node; \
}

#define return_NULL { \
  Number_white_spaces--; \
  CHECK_NEW_LINE\
  return NULL;\
}

#define Assert(exp) if (!(exp)) { \
  PRINT_RED_E(BOLD("Error on line: %d\n"), Number_new_lines); \
  PRINT_CYAN_E(BOLD("%s\n"), #exp); \
  abort(); \
}

Node* GetG();
Node* GetF();

Node* GetOp();

Node* GetAssn();
Node* GetSys();
Node* GetFunc();
Node* GetCondition();

Node* GetWhile();
Node* GetIf();

///// For expressions ///////
Node* GetE();
Node* GetT();
Node* GetPow();
Node* GetP();

Node* GetN();
Node* GetF();
Node* GetId();
//////////////////////////////

Node* GetG() {
  PRINT;

  Node* node = NULL;
  Node** right_node = &(node);
  
  while (TYPE == TOKEN_GLOBAL_FUNCTION || TYPE == TOKEN_VARIABLE) {
    CHECK_NEW_LINE
    *right_node = CreateNode(FUNC_NULL, TOKEN_FUNCTION);
    CHECK_NEW_LINE
    
    if (TYPE == TOKEN_GLOBAL_FUNCTION) (*right_node)->left = GetF();
    else                               (*right_node)->left = GetAssn();
    CHECK_NEW_LINE
    
    if (right_node == NULL) break;
    right_node = &((*right_node)->right);
  }
  
  CHECK_NEW_LINE
  Assert(TYPE == TOKEN_SYSTEM && DATA == SYS_END_P);
  return_node;
}

Node* GetF() {
  PRINT;
  if (TYPE != TOKEN_GLOBAL_FUNCTION) return_NULL;
     
  Node* node = CreateNode(DATA, TOKEN_GLOBAL_FUNCTION);
  Current++;
 
  CHECK_NEW_LINE
  Assert(DATA == '('); Current++;
  if (DATA == ')') {
    Current++;
    node->right = GetOp();
   
    CHECK_NEW_LINE
    Assert(TYPE == TOKEN_SYSTEM && DATA == SYS_END_FUNC);
    Current++;
    
    return_node;
  }

  Node** left_node = &(node->left);
  
  while (1) {
    CHECK_NEW_LINE
    
    (*left_node) = CreateNode(NEXT_SYMBOL, TOKEN_SYMBOL);
    (*left_node)->right = GetId();
    if ((*left_node)->right == NULL) { 
      break;
      TreeDelete(left_node);
    }

    left_node = &((*left_node)->left);
    if (DATA == ',') 
      Current++;
    else
      break;
  }

  Assert(DATA == ')'); Current++;

  node->right = GetOp();
   
  CHECK_NEW_LINE
  Assert(TYPE == TOKEN_SYSTEM && DATA == SYS_END_FUNC);
  Current++;
  
  return_node;
}

Node* GetOp() {
  PRINT;
  Node* node = CreateNode(OP_NULL, TOKEN_OPERATOR);
  Node* right_node = node;
  
  int first_iteration = 1;
  while (1) {
    
    CHECK_NEW_LINE
   
    if (TYPE == TOKEN_VARIABLE) {
      if (!first_iteration) {
        right_node->right = CreateNode(OP_NULL, TOKEN_OPERATOR);
        right_node = right_node->right;
      }
      right_node->left = GetAssn();
      first_iteration = 0;
    }

    else if (TYPE == TOKEN_SYSTEM && 
       (DATA == SYS_IF || DATA == SYS_WHILE)) {
      if (!first_iteration) {
        right_node->right = CreateNode(OP_NULL, TOKEN_OPERATOR);
        right_node = right_node->right;
      }
      right_node->left = GetSys();
      first_iteration = 0;
    }
    
    else if (TYPE == TOKEN_GLOBAL_FUNCTION || TYPE == TOKEN_FUNCTION) {
      if (!first_iteration) {
        right_node->right = CreateNode(OP_NULL, TOKEN_OPERATOR);
        right_node = right_node->right;
      }
      right_node->left = GetFunc();
      if (DATA == ';') Current++;
      first_iteration = 0;
    }
    
    else if (TYPE == TOKEN_SYSTEM && DATA == SYS_RETURN) {
      if (!first_iteration) {
        right_node->right = CreateNode(OP_NULL, TOKEN_OPERATOR);
        right_node = right_node->right;
      }
  
      right_node->left = CreateNode(DATA, TYPE);
      Current++;
      
      Assert(DATA == '('); Current++;
      
      if (DATA == ')') 
        right_node->left->left = CreateNodeNumber(0, TOKEN_NUMBER);
      else
        right_node->left->left = GetE();
      
      Assert(DATA == ')'); Current++;

      first_iteration = 0;
      if (DATA == ';') Current++;
    }
    
    else {
      break;
    }

  }

  return_node;
}

Node* GetAssn() {
  PRINT;
  Assert(tokenized_program[Current+1]->data.type == '=');
  
  Node* node = CreateNode('=', TOKEN_SYMBOL);
  node->left = CreateNode(DATA, TYPE);
  Current += 2;

  node->right = GetE();

  Assert(DATA == ';'); Current++;

  return_node;
}

Node* GetSys() {
  PRINT;
  Node* node = NULL;

  if (DATA == SYS_IF) {
    node = GetIf();
    return_node;
  }

  if (DATA == SYS_WHILE) {
    node = GetWhile();
    return_node;
  }
  
  return_NULL;
}

Node* GetFunc() {
  PRINT;
  
  Node* node = CreateNode(DATA, TYPE);
  Current++;
  CHECK_NEW_LINE
    
  Assert(DATA == '(');
  Current++;
  Node** left_node = &(node->left);
  
  if (node->type == TOKEN_FUNCTION) {
    
    if (node->data.type == 14 || node->data.type == 15)
      node->left = GetE();
    else
      node->right = GetE();
    
    Assert(DATA == ')');
    Current++;

    return_node;
  }
  
  while (1) {
    CHECK_NEW_LINE
    
    (*left_node) = CreateNode(NEXT_SYMBOL, TOKEN_SYMBOL);
    (*left_node)->right = GetE();
    if ((*left_node)->right == NULL) { 
      break;
      TreeDelete(left_node);
    }

    left_node = &((*left_node)->left);
    if (DATA == ',') 
      Current++;
    else
      break;
  }

  Assert(DATA == ')');
  Current++;

  return_node;
}

Node* GetIf() {
  PRINT;
  Assert((TYPE == TOKEN_SYSTEM) && (DATA == SYS_IF));
  Current++;
  
  Node* node = CreateNode(SYS_IF, TOKEN_SYSTEM);

  Assert(DATA == '('); Current++;
  node->left = GetCondition();
  Assert(DATA == ')'); Current++;
  
  node->right = CreateNode(SYS_IF_ELSE, TOKEN_SYSTEM);
  node->right->left = GetOp();
  
  if (DATA == SYS_ELSE && TYPE == TOKEN_SYSTEM) {
    Current++;
    node->right->right = GetOp();
  }

  Assert(TYPE == TOKEN_SYSTEM && DATA == SYS_END_IF); Current++;
  return_node;
}

Node* GetWhile() {
  PRINT;
  Assert((TYPE == TOKEN_SYSTEM) && (DATA == SYS_WHILE));
  Current++;
  
  Node* node = CreateNode(SYS_WHILE, TOKEN_SYSTEM);

  Assert(DATA == '('); Current++;
  node->left = GetCondition();
  Assert(DATA == ')'); Current++;
  
  node->right = GetOp();
  
  Assert(TYPE == TOKEN_SYSTEM && DATA == SYS_END_WHILE); Current++;
  return_node;
}

Node* GetCondition() {
  PRINT;
  
  Node* node = CreateNode(OP_E, TOKEN_OPERATOR);
  node->left = GetE();
  
  node->data.type = DATA; Current++;

  node->right = GetE();
  return_node;
}

Node* GetE() {
  PRINT;
  Node* node = GetT();
  
  while (TYPE == TOKEN_OPERATOR &&
        (DATA == OP_PLUS || DATA == OP_MINUS)) {
    CHECK_NEW_LINE
    
    char op = DATA;
    Current++;
    Node* node2 = GetT();

    node = TreeSetNode(node, node2, op, TOKEN_OPERATOR);
  }

  return_node;
}

Node* GetT() {
  PRINT;
  Node* node = GetPow();

  while (TYPE == TOKEN_OPERATOR &&
        (DATA == OP_MULT || DATA == OP_DIV)) {
    CHECK_NEW_LINE
    
    char op = DATA;
    Current++;
    Node* node2 = GetPow();

    node = TreeSetNode(node, node2, op,  TOKEN_OPERATOR);
  }

  return_node;
}

Node* GetPow() {
 PRINT;
 Node* node = GetP();
 
 if (TYPE == TOKEN_OPERATOR && DATA == OP_POW) {
    CHECK_NEW_LINE
    
    Current++;
    Node* node2 = GetP();

    node = TreeSetNode(node, node2, OP_POW, TOKEN_OPERATOR);
  }

  return_node;
}

Node* GetP() {
  PRINT;
  Node* node = NULL;

  if (DATA == '(') {
    Current++;
    node = GetE();

    Assert(DATA == ')');
    Current++;
    Assert(node != NULL);
    return_node;
  }
  
  if (TYPE == TOKEN_NUMBER) {
    node = GetN();
    Assert(node != NULL);
    return_node;
  }

  if (TYPE == TOKEN_FUNCTION || TYPE == TOKEN_GLOBAL_FUNCTION) {
     
     // if deriv function
     if (DATA == 16) {
       node = CreateNode(DATA, TYPE);
       Current++;
       Current++;

       node->left = GetId();
       Current++;
      
       node->right = GetE();
       Current++;
       
       return_node;
     }


     node = GetFunc();
     Assert(node != NULL);
     return_node;
  }

  if (TYPE == TOKEN_VARIABLE) { 
    node = GetId();    
    Assert(node != NULL);
    return_node;
  }
   
  Assert(node != NULL);
  return_NULL;
}

Node* GetN() {
  PRINT;
  Node* node = CreateNodeNumber(tokenized_program[Current]->data.value, TYPE);
  Current++;

  return_node;
}

Node* GetId() {
  PRINT;
  assert(TYPE == TOKEN_VARIABLE);

  Node* node = CreateNode(DATA, TYPE);
  Current++;
  
  return_node;
}

void ClearEquation(char* input_equation) {
  
  int new_i = 0;
  for (int i = 0; i < 1000; ++i)
    if (input_equation[i] != ' ') {
      input_equation[new_i] = input_equation[i];
      new_i++;
    }
}

int main() {
  //StartDebug();
  char* buffer = NULL;
  int buf_size = ReadFile("Programs/program", &buffer);

  int number_tokens = LexicalAnalysis(buffer, buf_size);
  PrintTokens(number_tokens);

  Node* tree = GetG();
  //NodePrint(tree, "tree.doxy");
  TreeSave(tree, "HelpFiles/frontend.tree");

  TreeDelete(&tree); 
  Free(buffer);
  ClearAll(number_tokens);
  tree = TreeLoad("HelpFiles/frontend.tree");
  //NodePrint(tree, "tree.doxy");
  MiddleEndDeriv(&tree);
  MiddleEndSimplify(&tree);
  //NodePrint(tree, "tree.doxy");
  TreeSave(tree, "HelpFiles/frontend.tree");

  TreeDelete(&tree); 
  //ClearAll(number_tokens);
    
  //StopDebug();
  return 0;
}

