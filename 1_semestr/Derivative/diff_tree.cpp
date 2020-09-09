#include "deriv.h"
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
  sprintf(dot_program, "dot -Tps %s -o graph.ps", file_name);
  
  system(dot_program);
  system("xdg-open graph.ps");

  return 1;
}


void PrintData(FILE* file, Node* node) {
  assert(file);
  assert(node);

  switch (node->type) {
    
    case TYPE_NUMBER:
      fprintf(file, "\"%p\" [label=\"%" ELEM_SYMBOL "\", \
                             color=red,                  \
                             style=filled,               \
                             fontname=bold,              \
                             fontsize=20]\n", node, node->data.value);
      break;

    case TYPE_VARIABLE:
      fprintf(file, "\"%p\" [label=\"%c\",  \
                             color=green,   \
                             style=filled,  \
                             fontname=bold, \
                             fontsize=20]\n", node, node->data.type);
      break;

    case TYPE_OPERATOR:
      fprintf(file, "\"%p\" [label=\"%c\",  \
                             color=magenta, \
                             style=filled,  \
                             fontname=bold, \
                             fontsize=20]\n", node, operators[node->data.type]);
      break;

    case TYPE_FUNCTION: {
      fprintf(file, "\"%p\" [label=\"%s\",     \
                             color=blue,       \
                             style=filled,     \
                             fontname=bold]\n", node, functions[node->data.type].name);
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












///////////////////////////////////////////////////////////////////////////////////
//
//                            Work with tree
//
///////////////////////////////////////////////////////////////////////////////////
void TreeDelete(Node** node) {
  if (node == NULL)    return;
  if ((*node) == NULL) return;
  
  if ((*node)->left)  TreeDelete(&((*node)->left));
  if ((*node)->right) TreeDelete(&((*node)->right));
  
  Free((*node));

  (*node) = NULL;
}

Node* CreateNode(Elem_t value, char type) {
  Node* new_node = (Node*)Calloc(1, sizeof(Node), new_node);
  assert(new_node);

  if (type == TYPE_NUMBER)
    new_node->data.value = value;
  else  
    new_node->data.type = value;

  new_node->left   = NULL;
  new_node->right  = NULL;
  new_node->type   = type;

  return new_node;
}


///////////////////////////////////////////////////////////////////////////////////
//
//                            Enhance Insert function
//
// if left_data  == POISON_VALUE_PUSH, insert left
// if right_data == POISON_VALUE_PUSH, insert right
//
///////////////////////////////////////////////////////////////////////////////////
Node* TreeSetNode(Node* his_left, Node* his_right, Elem_t his_data, char his_type, \
                  Elem_t left_data, Elem_t right_data) {            
  
  Node* node_to_set = CreateNode(his_data, his_type);
  
  if (left_data == POISON_VALUE_PUSH && right_data == POISON_VALUE_PUSH) {
    node_to_set->left  = his_left;
    node_to_set->right = his_right;
  }

  if (left_data == POISON_VALUE_PUSH && right_data != POISON_VALUE_PUSH) {
    node_to_set->left = his_left;
    node_to_set->right = CreateNode(right_data, TYPE_NUMBER);   
  }

  if (left_data != POISON_VALUE_PUSH && right_data == POISON_VALUE_PUSH) {
    node_to_set->left = CreateNode(left_data, TYPE_NUMBER);
    node_to_set->right = his_right;
  }

  if (left_data != POISON_VALUE_PUSH && right_data != POISON_VALUE_PUSH) {
    node_to_set->left  = CreateNode(left_data, TYPE_NUMBER);
    node_to_set->right = CreateNode(right_data, TYPE_NUMBER);
  }  

  return node_to_set;
}  


Node* Copy(Node* node) {
  if (node == NULL) return NULL;

  Node* new_node = (Node*)Calloc(1, sizeof(Node), new_node);
  if (new_node == NULL) {
    printf("Can not create");
    return NULL;
  }

  (new_node)->type = (node)->type;
  (new_node)->data.value = (node)->data.value;

  (new_node)->left  = Copy( ((node)->left) );
  (new_node)->right = Copy( ((node)->right) );

  return new_node;
}
  


///////////////////////////////////////////////////////////////////////////////////
//
//                            Find element function
//
//  type  = TYPE_NULL, if find by data
//  type != TYPE_NULL, if find by type
//
//  node - we are from to find
//  
//  return 1, if node exist
//  return 0, otherwise
///////////////////////////////////////////////////////////////////////////////////
int NodeFind(Node* node, Elem_t element, char type) {
  assert(node);

  // We reach our element?
  if (type == TYPE_NULL) {
    if (TreeCompare(node->data.value, element) == 0) return 1;
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




#include "recursive_fall.cpp"



///////////////////////////////////////////////////////////////////////////////////
//
//                              Save function
//
//  Save in this format: (((5) + (3)) / (sin(3)))
//
//  return 1, if all was OK
///////////////////////////////////////////////////////////////////////////////////
int TreeSave(Node* root, const char* file_name) {
  assert(root);
  assert(file_name);

  FILE* output = NULL;
  if (OpenFile(file_name, "w", &output)) return 0;
  
  SaveNodes(root, output);

  fclose(output);
  return 1;
}

int SaveData(FILE* file, Node* node) {
  assert(file);
  assert(node);

  switch (node->type) {
    
    case TYPE_NUMBER:
      fprintf(file, "%" ELEM_SYMBOL, node->data.value);
      break;

    case TYPE_OPERATOR:
      fprintf(file, "%c", node->data.type);
      break;

    case TYPE_VARIABLE:
      fprintf(file, "%c", node->data.type);
      break;

    case TYPE_FUNCTION:  
      fprintf(file, "%s", functions[node->data.type].name);
      break;
    
    default:
      return 0;
  }

  return 1;
}

void SaveNodes(Node* node, FILE* file) {
  assert(file);

  if (node == NULL) return;

  if (node->type == TYPE_OPERATOR) fprintf(file, "(");
  
  SaveNodes(node->left, file);
  assert(SaveData(file, node));
  SaveNodes(node->right, file);
  
  if (node->type == TYPE_OPERATOR) fprintf(file, ")");
}






///////////////////////////////////////////////////////////////////////////////////
//
//                       Create latex file of of the tree
//
// return 1, if all was OK
///////////////////////////////////////////////////////////////////////////////////
int CreateLatex(const char* file_name) {
  assert(file_name);

  FILE* output = NULL;
  if (OpenFile(file_name, "w", &output)) return 0;
  
  fprintf(output, "\\documentclass[12pt]{article}\n");
  fprintf(output, "\\begin{document}\n");
  fprintf(output, "\\title{Taking derivative and other maths perverts}\n");
  fprintf(output, "\\author{Ron Zaripov from MIPT}\n");
  fprintf(output, "\\maketitle\n");
  fprintf(output, "The derivative of a function of a real variable measures the sensitivity to change of the function value (output value) with respect to a change in its argument (input value). Derivatives are a fundamental tool of calculus. For example, the derivative of the position of a moving object with respect to time is the object's velocity: this measures how quickly the position of the object changes when time advances.\n");
  fprintf(output, "The derivative of a function of a single variable at a chosen input value, when it exists, is the slope of the tangent line to the graph of the function at that point. The tangent line is the best linear approximation of the function near that input value. For this reason, the derivative is often described as the instantaneous rate of change, the ratio of the instantaneous change in the dependent variable to that of the independent variable.\n");
  fprintf(output, "Derivatives may be generalized to functions of several real variables. In this generalization, the derivative is reinterpreted as a linear transformation whose graph is (after an appropriate translation) the best linear approximation to the graph of the original function. The Jacobian matrix is the matrix that represents this linear transformation with respect to the basis given by the choice of independent and dependent variables. It can be calculated in terms of the partial derivatives with respect to the independent variables. For a real-valued function of several variables, the Jacobian matrix reduces to the gradient vector.\n");
  fprintf(output, "\\newline\\newline \n");
  fprintf(output, "The process of finding a derivative is called differentiation. The reverse process is called antidifferentiation. The fundamental theorem of calculus relates antidifferentiation with integration. Differentiation and integration constitute the two fundamental operations in single-variable calculus.");
  fprintf(output, "\\newline \n \\newline \n");
  fprintf(output, "\\bigskip \\bigskip{\\Huge Rules for combined functions}\\newline\\newline\\bigskip\n");
  fprintf(output, "In many cases, complicated limit calculations by direct application of Newton's difference quotient can be avoided using differentiation rules. Some of the most basic rules are the following.\n");
  fprintf(output, "$$Constant: f(x) = C \\Longrightarrow  f'(x) = 0$$\n");
  fprintf(output, "$$Variable: f(x) = x \\Longrightarrow  f'(x) = 1$$\n");
  fprintf(output, "$$Power: f(x) = x^{n} \\Longrightarrow  f'(x) = n\\cdot x^{n-1}$$\n");
  fprintf(output, "$$Product: f(x) = \\frac{y}{x} \\Longrightarrow f'(x) = \\frac{y'\\cdot x - x'\\cdot y}{x^{2}}$$");
  fprintf(output, "$$f(x) = sin(x) \\Longrightarrow f'(x) = cos(x)$$\n");
  fprintf(output, "$$f(x) = cos(x) \\Longrightarrow f'(x) = -sin(x)$$\n");
  fprintf(output, "$$f(x) = tan(x) \\Longrightarrow f'(x) = (sec(x))^{2}$$\n");
  fprintf(output, "$$f(x) = cot(x) \\Longrightarrow f'(x) = -(csc(x))^{2}$$\n");
  fprintf(output, "$$f(x) = sec(x) \\Longrightarrow f'(x) = sec(x) \\cdot tan(x)$$\n");
  fprintf(output, "\\newline \\newline \\bigskip\n");
  fprintf(output, "\\begin{align*}\n");
  fprintf(output, "{\\Large Let's take derivative of simple function as an example}\\end{align*}\n");

  fclose(output);
  return 1;
}

int EndLatex(const char* file_name) {
  assert(file_name);
  
  FILE* output = NULL;
  if (OpenFile(file_name, "a", &output)) return 0;

  fprintf(output, "\\end{document}\n");

  fclose(output);
  return 1;
}

int AppendMessageLatex(const char* file_name, const char* message) {
  assert(message);
  assert(file_name);

  FILE* output = NULL;
  if (OpenFile(file_name, "a", &output)) return 0;
  
  fprintf(output, "%s", message);

  fclose(output);
  return 1;
}
 
int AppendFormulaLatex(Node* node, const char* file_name, int special_symbols) {
  assert(node);
  assert(file_name);

  FILE* output = NULL;
  if (OpenFile(file_name, "a", &output)) return 0;
  
  if (special_symbols) fprintf(output, "$$");
  LatexNodes(0, node, output, 1);
  if (special_symbols) fprintf(output, "$$");

  fclose(output);
  return 1;
}

int LatexData(Node* node, FILE* file) {
  assert(file);
  assert(node);

  switch (node->type) {
    
    case TYPE_NUMBER:
      fprintf(file, "%" ELEM_SYMBOL, node->data.value);
      return 5;

    case TYPE_OPERATOR:
      if (node->data.type == OPERATOR_MULT)
        fprintf(file, "\\cdot ");
      else
        fprintf(file, "%c", operators[node->data.type]);
      return 1;

    case TYPE_VARIABLE:
      fprintf(file, "%c", node->data.type);
      return 1;

    case TYPE_FUNCTION:
      fprintf(file, "%s", functions[node->data.type].name);
      return 5;
    
    default:
      return 0;
  }
}

int LatexNodes(int print_symbols, Node* node, FILE* file, int same_priority) {
  assert(file);
  if (node == NULL) return 0;
  
  if (print_symbols > 400) {
    //fprintf(file, "\\\\\n");
    print_symbols = 0;
  }

  if ( node->type == TYPE_OPERATOR && node->data.type == OPERATOR_DIV ) {
    fprintf(file, "\\frac{");

    print_symbols += LatexNodes(print_symbols, node->left, file, 1);
    fprintf(file, "}{");
    
    print_symbols += LatexNodes(print_symbols, node->right, file, 1);
    fprintf(file, "}");
  }
  else if ( node->type == TYPE_OPERATOR && node->data.type == OPERATOR_POW ) {
  
    fprintf(file, "(");
    print_symbols++;
    print_symbols += LatexNodes(print_symbols, node->left, file, 1);
    fprintf(file, ")^{");
    print_symbols++;
    
    print_symbols += LatexNodes(print_symbols, node->right, file, 1);
    fprintf(file, "}");
  }
  else if (node->type == TYPE_FUNCTION) {
    
    assert(LatexData(node, file));
    fprintf(file, "("); 
    print_symbols++;
    print_symbols += LatexNodes(print_symbols, node->right, file, 0);
    fprintf(file, ")"); 
    print_symbols++;
  }

  else {
    if (node->type == TYPE_OPERATOR && !same_priority) { fprintf(file, "("); print_symbols++; } 
    
    if (node->type == TYPE_OPERATOR && node->data.type == node->left->data.type)
      print_symbols += LatexNodes(print_symbols, node->left, file, 1);
    else
      print_symbols += LatexNodes(print_symbols, node->left, file, 0);
    
    print_symbols += LatexData(node, file);
    
    if (node->type == TYPE_OPERATOR && node->data.type == node->right->data.type)
      print_symbols += LatexNodes(print_symbols, node->right, file, 1);
    else
      print_symbols += LatexNodes(print_symbols, node->right, file, 0);
  
    if (node->type == TYPE_OPERATOR && !same_priority) { fprintf(file, ")"); print_symbols++; }
  }

  return print_symbols;
}




///////////////////////////////////////////////////////////////////////////////////
//
//                       Calculate tree
//
// return answer
///////////////////////////////////////////////////////////////////////////////////
double CalculateTree(Node* root) {
  assert(root);

  if (NodeFind(root, 0, TYPE_VARIABLE)) {
    PRINT_RED(N("Can not Calculate this\n"));
    return 0;
  }
  
  return CalculateNode(root);
}

double CalculateNode(Node* node) {
  if (node == NULL) return 0;
  
  if (node->type == TYPE_NUMBER) return node->data.value;

  double first  = CalculateNode(node->left);
  double second = CalculateNode(node->right);

  if (node->type == TYPE_FUNCTION) {
    
    if (node->data.type < NUMBER_FUNCTIONS) 
      return functions[node->data.type].func(first);

    printf("Unknown function: %c\n", node->data.type);
    return 0;
  }

  if (node->type == TYPE_OPERATOR) {
    switch (node->data.type) {

      case OPERATOR_PLUS:
        return first + second;

      case OPERATOR_MINUS:
        return first - second;

      case OPERATOR_DIV:
        return first / second;

      case OPERATOR_MULT:
        return first * second;

      case OPERATOR_POW:
        return pow(first, second);
    }
    
    printf("Unknown operator: %d\n", node->type);
    return 0;
  }

}
