#include "tree.cpp"

enum STATES {
  IN__FUNCTION = 0,
  OUT_FUNCTION,
  PUSH_ME,
  POP_TO_ME
};

// for IF, ELSE, WHILE...
int global_counter = 0;

void Interpretate(Node* node, FILE* ams, int state = 0) {
  if (!node) return;
  assert(ams);
  
  switch (node->type) {
    
    case TOKEN_NUMBER:
      
      if (state == PUSH_ME) {
        fprintf(ams, "push %lg\n", node->data.value);
        break;
      }
      if (state == POP_TO_ME) {
        printf("POP to NUMBER\n");
        abort();
      }
      printf("What i have to do with this value?\n");
      abort();

    case TOKEN_OPERATOR:  
      
      Interpretate(node->left, ams, PUSH_ME);
      Interpretate(node->right, ams, PUSH_ME);
       
      switch (node->data.type) {
        
        case OP_NULL:
         break;

        case OP_PLUS:
          fprintf(ams, "add\n\n");
          break;

        case OP_MINUS:
          fprintf(ams, "sub\n\n");
          break;

        case OP_DIV:
          fprintf(ams, "div\n\n");
          break;

        case OP_MULT:
          fprintf(ams, "mult\n\n");
          break;

        case OP_POW:
          fprintf(ams, "pwr\n\n");
          break;

        case OP_G:
          fprintf(ams, "jae");
          break;

        case OP_GE:
          fprintf(ams, "ja");
          break;

        case OP_L:
          fprintf(ams, "jbe");
          break;

        case OP_LE:
          fprintf(ams, "jb");
          break;

        case OP_E:
          fprintf(ams, "jne");
          break;
        
        case OP_NE:
          fprintf(ams, "je");
          break;


        default:
          printf("Unknown operator type: %d", node->data.type);
          abort();
      }
      break;

    case TOKEN_VARIABLE:
 
      if (state == PUSH_ME) {
        fprintf(ams, "push v%02d\n", node->data.type);
        break;
      }
      if (state == POP_TO_ME) {
        fprintf(ams, "pop v%02d\n", node->data.type);
        break;
      }

      printf("What i have to do with this variable?\n");
      abort();

    case TOKEN_FUNCTION: {
      
      if (strcmp(functions[node->data.type].name, "get") == 0) {
        fprintf(ams, "put\n");
        Interpretate(node->left, ams, POP_TO_ME);
        return;
      }

      if (strcmp(functions[node->data.type].name, "put") == 0) {
        Interpretate(node->left, ams, PUSH_ME);
        fprintf(ams, "print\npop\n");
        return;
      }
      
      Node* args = node->right;
      
      if (args->type == TOKEN_NUMBER)
        fprintf(ams, "push %lg\n", args->data.value);
      if (args->type == TOKEN_VARIABLE)
        fprintf(ams, "push v%02d\n", args->data.type);

      fprintf(ams, "call func_%s\n", functions[node->data.type].name);
      if (state == PUSH_ME)
          fprintf(ams, "push cx\n");
      }
      return;

    case TOKEN_GLOBAL_FUNCTION: {
      
      if (state != OUT_FUNCTION) {
        Node* args = node->left;
        while (args != NULL) {
          
          Interpretate(args->right, ams, PUSH_ME);
          args = args->left;
        }

        fprintf(ams, "call func_%s\n", functions_names.list[node->data.type]);
        if (state == PUSH_ME)
          fprintf(ams, "push cx\n");
        
        return;
      }

      fprintf(ams, ":func_%s\n", functions_names.list[node->data.type]); 
      fprintf(ams, "pop bx\n");
      
      // Arguments
      char list_variables[MAX_NUMBER_VARIABLES] = {};
      int list_it = 0;

      Node* args = node->left;
      while (args != NULL) {
        
        if (args->right->type == TOKEN_VARIABLE)
          list_variables[list_it++] = args->right->data.type;
        else {
          printf("Invalid variable in intitial: %d\n", args->type);
          abort();
        }
        args = args->left;
      }
      
      for (int i = list_it - 1; i >= 0; --i)
        fprintf(ams, "pop v%02d\n", list_variables[i]);
      
      fprintf(ams, "push bx\n");

      Interpretate(node->right, ams);
      fprintf(ams, "\n\n");
      }
      break;

    case TOKEN_SYMBOL:
      switch (node->data.type) {
        
        case '=':
          Interpretate(node->right, ams, PUSH_ME);
          Interpretate(node->left, ams, POP_TO_ME);
          break;

        default:
          printf("Unknown symbol: %c", node->data.type);
          abort();
      }
      break;

    case TOKEN_SYSTEM:
      switch (node->data.type) {
        
        case SYS_NULL:
          fprintf(ams, ";----------------------------------------\n\n");
          Interpretate(node->left, ams, OUT_FUNCTION);
          Interpretate(node->right, ams);
          break;

        case SYS_IF: {
            int if_counter = global_counter++;
            fprintf(ams, "                   ;Start if\n");

            Interpretate(node->left, ams);
            fprintf(ams, " begin_else_%d\n", if_counter);
            
            fprintf(ams, ":begin_if_%d\n", if_counter);
            Interpretate(node->right->left, ams);
            fprintf(ams, "jmp end_if_%d\n", if_counter);

            fprintf(ams, ":begin_else_%d\n", if_counter);
            Interpretate(node->right->right, ams);

            fprintf(ams, ":end_if_%d\n", if_counter);
            fprintf(ams, "                   ;End if\n\n");
          }
          break;

        case SYS_WHILE: {
            int while_counter = global_counter++;
            
            fprintf(ams, ":begin_condition_%d\n", while_counter);
            Interpretate(node->left, ams);
            fprintf(ams, " end_while_%d\n", while_counter);
            
            Interpretate(node->right, ams);
            fprintf(ams, "jmp begin_condition_%d\n", while_counter);

            fprintf(ams, ":end_while_%d\n", while_counter);
          }
          break;
          
        case SYS_RETURN:
          Interpretate(node->left, ams, PUSH_ME);
          fprintf(ams, "pop cx\n");
          fprintf(ams, "return\n");
          return;

        case SYS_END_P:
          return;
        
        default:
          printf("Unknown SYS_TYPE: %d", node->data.type);
          abort();
      }
      break;

    default:
      printf("Unknown type: %d", node->type);
      abort();
  }
}

void BackEnd(const char* code, const char* ams_file) {
  assert(code);
  assert(ams_file);
  
  Node* tree = TreeLoad(code);
  NodePrint(tree, "HelpFiles/tree.doxy");
  
  int number_global = 0;
  Node* node = tree;
  while (node != NULL) {
    if (node->left->type == TOKEN_SYMBOL)
      number_global++;
    node = node->right;
  }
  
  FILE* ams = fopen(ams_file, "w");
  
  fprintf(ams, "push %d\n", number_global);
  fprintf(ams, "pop dx\n");
  fprintf(ams, "call func_main\n");
  fprintf(ams, "end\n");
  
  Interpretate(tree, ams, OUT_FUNCTION);

  TreeDelete(&tree);
}

int main() {
 
  BackEnd("HelpFiles/frontend.tree", "HelpFiles/tree.ams");
  return 0;
}
