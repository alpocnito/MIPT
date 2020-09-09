#include "tree.cpp"

#define Fprintf(...) { \
    for (int i = 0; i < New_lines; ++i) \
      fprintf(fpf, " "); \
    fprintf(__VA_ARGS__);\
  }

enum STATES {
  IN__FUNCTION = 0,
  OUT_FUNCTION,
  PUSH_ME,
  POP_TO_ME
};

// for \n

int New_lines = 0;

void Interpretate(Node* node, FILE* fpf, int state = 0) {
  if (!node) return;
  assert(fpf);
  
  switch (node->type) {
    
    case TOKEN_NUMBER:
      
      if (state == PUSH_ME) {
        fprintf(fpf, "%lg", node->data.value);
        break;
      }
      if (state == POP_TO_ME) {
        printf("POP to NUMBER\n");
        abort();
      }
      printf("What i have to do with this value?\n");
      abort();

    case TOKEN_OPERATOR:  
      
      if (node->data.type != OP_NULL) fprintf(fpf, "(");
      Interpretate(node->left, fpf, PUSH_ME);
       
      switch (node->data.type) {
        
        case OP_NULL:
         break;

        case OP_PLUS:
          fprintf(fpf, " + ");
          break;

        case OP_MINUS:
          fprintf(fpf, " - ");
          break;

        case OP_DIV:
          fprintf(fpf, " / ");
          break;

        case OP_MULT:
          fprintf(fpf, " * ");
          break;

        case OP_POW:
          fprintf(fpf, " ^ ");
          break;

        case OP_G:
          fprintf(fpf, " > ");
          break;

        case OP_GE:
          fprintf(fpf, " >= ");
          break;

        case OP_L:
          fprintf(fpf, " < ");
          break;

        case OP_LE:
          fprintf(fpf, " <= ");
          break;

        case OP_E:
          fprintf(fpf, " == ");
          break;

        case OP_NE:
          fprintf(fpf, " != ");
          break;

        default:
          printf("Unknown operator type: %d", node->data.type);
          abort();
      }
      Interpretate(node->right, fpf, PUSH_ME);
      if (node->data.type != OP_NULL) fprintf(fpf, ")");
      
      break;

    case TOKEN_VARIABLE:
 
      if (state == PUSH_ME) {
        fprintf(fpf, "%s", variables_names.list[node->data.type]);
        break;
      }
      if (state == POP_TO_ME) {
        Fprintf(fpf, "%s = ", variables_names.list[node->data.type]);
        break;
      }

      printf("What i have to do with this variable?\n");
      abort();

    case TOKEN_FUNCTION: {
      
      if (strcmp(functions[node->data.type].name, "get") == 0) {
        Fprintf(fpf, "get(");
        Interpretate(node->left, fpf, PUSH_ME);
        fprintf(fpf, ");\n");
        return;
      }

      if (strcmp(functions[node->data.type].name, "put") == 0) {
        Fprintf(fpf, "put(");
        Interpretate(node->left, fpf, PUSH_ME);
        fprintf(fpf, ");\n");
        return;
      }
      
      if (state != PUSH_ME) {
        Fprintf(fpf, "%s(", functions[node->data.type].name);
      }
      else {
        fprintf(fpf, "%s(", functions[node->data.type].name);
      }
      
      if (strcmp(functions[node->data.type].name, "deriv") == 0) {
        Interpretate(node->left, fpf, PUSH_ME);
        fprintf(fpf, ", ");
        Interpretate(node->right, fpf, PUSH_ME);
        fprintf(fpf, ")");
        return;
      }
  
      Interpretate(node->right, fpf, PUSH_ME);
      fprintf(fpf, ")");
      if (state != PUSH_ME)
        fprintf(fpf, ";\n");
      
      return;
    }
    case TOKEN_GLOBAL_FUNCTION: {
       
      if (state != PUSH_ME) {
        Fprintf(fpf, "%s(", functions_names.list[node->data.type]);
      }
      else {
        fprintf(fpf, "%s(", functions_names.list[node->data.type]);
      }      

      if (state !=  OUT_FUNCTION) {
        Node* args = node->left; 
        
        Interpretate(args->right, fpf, PUSH_ME);
        args = args->left;

        while (args != NULL) {
          fprintf(fpf, ", ");
          Interpretate(args->right, fpf, PUSH_ME);
          args = args->left;
        }
        fprintf(fpf, ")");

        if (state != PUSH_ME)
          fprintf(fpf, ";\n");
  
        return;
      }

      
      // Arguments
      char list_variables[MAX_NUMBER_VARIABLES] = {};
      int list_it = 0;

      Node* args = node->left;
      if (args == NULL) {
        fprintf(fpf, ")\n");
        New_lines++;
        Interpretate(node->right, fpf);
        New_lines--;
        fprintf(fpf, "endf");
        fprintf(fpf, "\n\n");
        break;
      }

      if (args->right->type == TOKEN_VARIABLE) {
        fprintf(fpf, "%s", variables_names.list[args->right->data.type]);
      }
      else {
        printf("Invalid variable in intitial\n");
        abort();
      }
      args = args->left;

      while (args != NULL) {
        fprintf(fpf, ", ");

        if (args->right->type == TOKEN_VARIABLE) {
          fprintf(fpf, "%s", variables_names.list[args->right->data.type]);
        }
        else {
          printf("Invalid variable in intitial\n");
          abort();
        }
        args = args->left;
      }
      fprintf(fpf, ")\n");
      
      New_lines++;
      Interpretate(node->right, fpf);
      New_lines--;
      fprintf(fpf, "endf");
      fprintf(fpf, "\n\n");
      }
      break;

    case TOKEN_SYMBOL:
      switch (node->data.type) {
        
        case '=':
          Interpretate(node->left, fpf, POP_TO_ME);
          Interpretate(node->right, fpf, PUSH_ME);
          fprintf(fpf, ";\n");
          break;

        default:
          printf("Unknown symbol: %c", node->data.type);
          abort();
      }
      break;

    case TOKEN_SYSTEM:
      switch (node->data.type) {
        
        case SYS_NULL:
          fprintf(fpf, "#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
          Interpretate(node->left, fpf, OUT_FUNCTION);
          Interpretate(node->right, fpf);
          break;

        case SYS_IF: {  
            Fprintf(fpf, "if ");
            Interpretate(node->left, fpf);
            Fprintf(fpf, "\n");
            New_lines++;
            
            Interpretate(node->right->left, fpf);
            New_lines--;
        
            if (node->right->right == NULL) {
              Fprintf(fpf, "endif\n");
              break;
            }
            fprintf(fpf, "\n");
            Fprintf(fpf, "else\n");
            New_lines++;
            Interpretate(node->right->right, fpf);
            New_lines--;

            Fprintf(fpf, "endif\n");
          }
          break;

        case SYS_WHILE: {
            Fprintf(fpf, "while "); 
            Interpretate(node->left, fpf);
            Fprintf(fpf, "\n");
            
            New_lines++;
            Interpretate(node->right, fpf);
            New_lines--;
            Fprintf(fpf, "endwhile\n");
          }
          break;
          
        case SYS_RETURN:
          Fprintf(fpf, "ret(");
          Interpretate(node->left, fpf, PUSH_ME);
          fprintf(fpf, ");\n");
          return;

        case SYS_END_P:
          fprintf(fpf, "endp");  
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

void FrontEndBack(const char* code, const char* funny_program_file) {
  assert(code);
  assert(funny_program_file);
  

  Node* tree = TreeLoad(code);
  NodePrint(tree, "HelpFiles/tree.doxy");
  
  int number_global = 0;
  Node* node = tree;
  while (node != NULL) {
    if (node->left->type == TOKEN_SYMBOL)
      number_global++;
    node = node->right;
  }
  
  FILE* fpf = fopen(funny_program_file, "w");

  Interpretate(tree, fpf, OUT_FUNCTION);
  fprintf(fpf, "endp");
  TreeDelete(&tree);
  fclose(fpf);
}

int main() {
  my_conv(); 
  FrontEndBack("HelpFiles/frontend.tree", "Programs/collect_program");
  return 0;
}
