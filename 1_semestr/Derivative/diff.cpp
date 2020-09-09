#include "diff_tree.cpp"

#include <time.h>
#include <stdlib.h>

#define dL TakeDerivative(node->left)
#define dR TakeDerivative(node->right)
#define cL Copy( node->left )
#define cR Copy( node->right )
#define cN Copy( node )

#define DIG(number) CreateNode(number, TYPE_NUMBER)

#define L (*node)->left
#define R (*node)->right
#define NUMBER(node)   (node->type == TYPE_NUMBER)
#define VARIABLE(node) (node->type == TYPE_VARIABLE)


#define PLUS(left, right) \
  TreeSetNode(left, right, OPERATOR_PLUS, TYPE_OPERATOR)

#define MINUS(left, right) \
  TreeSetNode(left, right, OPERATOR_MINUS, TYPE_OPERATOR)

#define MULT(left, right) \
  TreeSetNode(left, right, OPERATOR_MULT, TYPE_OPERATOR)

#define DIV(left, right) \
  TreeSetNode(left, right, OPERATOR_DIV, TYPE_OPERATOR)

#define POW(left, right) \
  TreeSetNode(left, right, OPERATOR_POW, TYPE_OPERATOR)
    
#define RETURN(sign) \
  AppendMessageLatex("tree.tex", "\nDerivative of the " #sign": ");\
  AppendMessageLatex("tree.tex", "$$");\
  AppendFormulaLatex(node, "tree.tex", 0);\
  AppendMessageLatex("tree.tex", " = ");\
  AppendFormulaLatex(new_node, "tree.tex", 0);\
  AppendMessageLatex("tree.tex", "$$");\
  return new_node;





const char* SimplifyPhrases[] = \
        { "Easy to see", "Obviously, ", "Some easy transformations: ", "Using the rules can be tricky!", "So, ", "Simplify more: ", "So that is our next step", "The process of finding a derivative is called Differentiation: ", "Use the chain rule: ", "Make sure your algebra is up to par: ", "I'am so tired: ", "This is a lot of work for such a simple function: ", "Then the entire problem can be expressed as: "};
const int NumberSimplifyPhrases = sizeof(SimplifyPhrases) / sizeof(char*);

int SimplifyPlus (Node** node);
int SimplifyMinus(Node** node); 
int SimplifyMult (Node** node); 
int SimplifyDiv  (Node** node);
int SimplifyPow  (Node** node); 


Node* TakeDerivative(Node* node) { 
  assert(node);
  
  switch ((node)->type) {
    
    case TYPE_NUMBER: {
       
      Node* new_node = CreateNode(0, TYPE_NUMBER);
      return new_node;
    }
    case TYPE_VARIABLE: {
      
      Node* new_node = CreateNode(1, TYPE_NUMBER);
      return new_node;
    }

    case TYPE_OPERATOR: {
        
        switch (node->data.type) {
          
          case OPERATOR_PLUS: {
            Node* new_node = PLUS(dL, dR);
            
            RETURN('+');
          }

          case OPERATOR_MINUS: {
            Node* new_node = MINUS(dL, dR);
            
            RETURN('-');
          }

          case OPERATOR_DIV: {
            Node* new_node = DIV( MINUS(MULT(dL, cR), MULT(cL, dR)), MULT(cR, cR) );
            
            RETURN('/');
          }
          
          case OPERATOR_MULT: {
            Node* new_node = PLUS(MULT(dL, cR), MULT(cL, dR));
            
            RETURN("*");
          }

          case OPERATOR_POW: {
            Node* new_node = NULL; 
            if (NodeFind(node->left, 0, TYPE_VARIABLE))
              new_node = MULT(MULT(POW(cL, MINUS(cR, DIG(1)) ), cR), dL);
            
            else {
              Node* ln_a = CreateNode(FUNCTION_LOGE, TYPE_FUNCTION);
              ln_a->right = cL;

              new_node = MULT( MULT(cN, ln_a), dR);
            }

            RETURN("pow");
          }
          
          default:
            printf("Unknown operator");
            abort();
      }
      
    }
    break;

    case TYPE_FUNCTION: {
        
      if (node->data.type == FUNCTION_COS) {
        node->data.type = FUNCTION_SIN;
       
        Node* new_node = MULT(MINUS(DIG(0), cN), dR);
        
        node->data.type = FUNCTION_COS;
        RETURN("cos");
      }
    
      if (node->data.type == FUNCTION_SIN) {
        node->data.type = FUNCTION_COS;
        
        Node* new_node = MULT(cN, dR);
        
        node->data.type = FUNCTION_SIN;
        RETURN("sin");
      }

      if (node->data.type == FUNCTION_TAN) {
        node->data.type = FUNCTION_COS;
        
        Node* new_node = MULT(DIV(DIG(1), POW(cN, DIG(2))), dR);
        
        node->data.type = FUNCTION_TAN;
        RETURN("tan");
      }
 
      if (node->data.type == FUNCTION_COT) {
        node->data.type = FUNCTION_SIN;
        
        Node* new_node = MULT(MINUS(DIG(0), DIV(DIG(1), POW(cN, DIG(2)))), dR);
        
        node->data.type = FUNCTION_COT;
        RETURN("cotan");
      }     

      if (node->data.type == FUNCTION_ACOS) {
        Node* new_node = MULT(MINUS(DIG(0), DIV(DIG(1), POW(MINUS(DIG(1), POW(cR, DIG(2))), DIG(0.5)))), dR);

        RETURN("acos");
      }
      
      if (node->data.type == FUNCTION_ASIN) {
        Node* new_node = MULT(DIV(DIG(1), POW(MINUS(DIG(1), POW(cR, DIG(2))), DIG(0.5))), dR);

        RETURN("asin");
      }

      if (node->data.type == FUNCTION_ATAN) {
        Node* new_node = MULT(DIV(DIG(1), PLUS(DIG(1), POW(cR, DIG(2)) )), dR);

        RETURN("atan");
      }

      if (node->data.type == FUNCTION_SINH) {
        node->data.type = FUNCTION_COSH;
        
        Node* new_node = MULT(cN, dR);
        
        node->data.type = FUNCTION_SINH;
        RETURN("sinh");
      }
      
      if (node->data.type == FUNCTION_COSH) {
        node->data.type = FUNCTION_SINH;
        
        Node* new_node = MULT(cN, dR);
        
        node->data.type = FUNCTION_COSH;
        RETURN("cosh");
      }
      
      if (node->data.type == FUNCTION_TANH) {
        node->data.type = FUNCTION_COSH;
        
        Node* new_node = MULT(DIV(DIG(1), POW(cN, DIG(2))), dR);
        
        node->data.type = FUNCTION_TANH;
        RETURN("tanh");
      }

      if (node->data.type == FUNCTION_LOGE) {
        Node* new_node = MULT(DIV(DIG(1), cR), dR);
        
        RETURN("loge");
      }
      
      if (node->data.type == FUNCTION_LOG10) {
        Node* ln_10 = CreateNode(FUNCTION_LOGE, TYPE_FUNCTION);
        ln_10->right = CreateNode(10, TYPE_NUMBER);

        Node* new_node = MULT(DIV(DIG(1), MULT(cR, ln_10)), dR); 
        RETURN("log10");
      }

    }
  
    default:
      printf("Unknown function");
      abort();
    }
    
  return node;
}







#define CLEAR \
  TreeDelete(&L); \
  TreeDelete(&R);\
  L = NULL;\
  R = NULL;\
  return 1;

#define ASSIGN(direction) \
  {\
    Node** old_node = node;\
    Node* new_node = Copy((*node)->direction);\
    TreeDelete(old_node);\
    *node = new_node;\
    return 1;\
  }

#define NODE_TO(number) \
  {\
    (*node)->data.value = number; \
    (*node)->type = TYPE_NUMBER;\
    CLEAR\
  }


int SimplifyPlus(Node** node) {
  assert(L);
  assert(R);
  
  if (NUMBER(L) && NUMBER(R))
    NODE_TO(L->data.value + R->data.value);
  
  if (NUMBER(L))
    if (L->data.value == 0)
      ASSIGN(right);    
  
  if (NUMBER(R))
    if (R->data.value == 0)
      ASSIGN(left);

  return 0;
}
      
int SimplifyMinus(Node** node) {
  assert(L);
  assert(R);
  
  if (NUMBER(L) && NUMBER(R))
    NODE_TO(L->data.value - R->data.value);
  
  if (NUMBER(R))
    if (R->data.value == 0)
      ASSIGN(left);

  return 0;
}
      
     
int SimplifyMult(Node** node) {
  assert(L);
  assert(R);
  
  if (NUMBER(L) && NUMBER(R))
    NODE_TO(L->data.value * R->data.value);
   
  if (NUMBER(L)) {
    
    if (L->data.value == 0)
      NODE_TO(0);
    
    if (L->data.value == 1)
      ASSIGN(right);
  } 
  
  if (NUMBER(R)) {
 
    if (R->data.value == 0)
      NODE_TO(0);

    if (R->data.value == 1) 
      ASSIGN(left);
  } 

  return 0;
}
   
int SimplifyDiv(Node** node) {
  assert(L);
  assert(R);
  
  if (NUMBER(R) && R->data.value == 0) {
    printf("Division on zero!!!\n");
    return 0;
  }

  if (NUMBER(L) && NUMBER(R))
    NODE_TO(L->data.value / R->data.value);
   
  if (NUMBER(L))
    if (L->data.value == 0)
      NODE_TO(0);

  if (NUMBER(R))
    if (L->data.value == 1)
      ASSIGN(right);

  return 0;
}
     
int SimplifyPow(Node** node) {
  assert(L);
  assert(R);
  
  if (NUMBER(L) && NUMBER(R))
    NODE_TO(pow(L->data.value, R->data.value));
    
   
  if (NUMBER(L)) {
    
    if (L->data.value == 0)
      NODE_TO(0);
    
    if (L->data.value == 1)
      NODE_TO(1);
  } 
  
  if (NUMBER(R)) {
    
    if (R->data.value == 0)
      NODE_TO(1);
    
    if (R->data.value == 1)
      ASSIGN(left);
  } 

  return 0;
}
 
int Simplify(Node** node) {
  assert(node);
  assert(*node);
  
  // if we want Simplify NUMBER or VARIABLE
  if ((*node)->type == TYPE_NUMBER || (*node)->type == TYPE_VARIABLE) return 0;

  int was_simplified = 0;
  Node* old_tree = Copy(*node);
  
  // Simplify left
  if ((*node)->type != TYPE_FUNCTION) 
  if (!NUMBER(L) && !VARIABLE(L)) 
    was_simplified |= Simplify(&(L));
  
  // Simplify right
  if (!NUMBER(R) && !VARIABLE(R)) 
    was_simplified |= Simplify(&(R));
  
  if ((*node)->type == TYPE_OPERATOR)
  switch ( (*node)->data.type ) {
      case OPERATOR_PLUS:
        was_simplified |= SimplifyPlus(node);
        break;
      case OPERATOR_MINUS:
        was_simplified |= SimplifyMinus(node); 
        break;
      case OPERATOR_MULT:
        was_simplified |= SimplifyMult(node); 
        break;
      case OPERATOR_DIV:
        was_simplified |= SimplifyDiv(node);
        break;
      case OPERATOR_POW:
        was_simplified |= SimplifyPow(node); 
        break;
      
      default:
        printf("Unknown operator");
        return 0;
    }
  
/*  if (was_simplified) {
    static int seed = 0;

    int NumberRandomPhrase = seed % NumberSimplifyPhrases;
    const char* RandomPhrase = SimplifyPhrases[NumberRandomPhrase];
    seed++;
    
    AppendMessageLatex("tree.tex", RandomPhrase);
    AppendMessageLatex("tree.tex", "$$");
    AppendFormulaLatex(old_tree, "tree.tex", 0);
    AppendMessageLatex("tree.tex", " = ");
    AppendFormulaLatex(*node, "tree.tex", 0);
    AppendMessageLatex("tree.tex", "$$");
  }*/
  
  
  TreeDelete(&old_tree);
  return was_simplified;
}










int main() { 
  StartDebug(); 
  
  Node* equation = TreeLoad("easy_tree");

  CreateLatex("tree.tex");
  AppendFormulaLatex(equation, "tree.tex");
  
  Node* deriv_equation = TakeDerivative(equation);
  TreeDelete(&(equation));
  equation = TakeDerivative(deriv_equation);
  TreeDelete(&(deriv_equation));
  deriv_equation = TakeDerivative(equation);
  
  //NodePrint(deriv_equation, "tree.doxy");

  AppendMessageLatex("tree.tex", "\n\\newline\\newline{\\Huge Let's simplify}\n");
  AppendFormulaLatex(deriv_equation, "tree.tex");
  while (Simplify(&deriv_equation) != 0); 
  
  AppendMessageLatex("tree.tex", "\\newline\\newline\\newline{\\Large And now we get answer in very simple ans usable form: } {\\large");
  AppendFormulaLatex(deriv_equation, "tree.tex");
  AppendMessageLatex("tree.tex", "}\n$$\\bigoplus$$\n"); 

  EndLatex("tree.tex");
  TreeSave(deriv_equation, "new_tree");  
  
  TreeDelete(&(deriv_equation));
  TreeDelete(&(equation));

  StopDebug();
  return 0;
}
