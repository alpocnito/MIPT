#include <time.h>
#include <stdlib.h>

#define dL TakeDerivative(node->left)
#define dR TakeDerivative(node->right)
#define cL Copy( node->left )
#define cR Copy( node->right )
#define cN Copy( node )

#define DIG(number) CreateNodeNumber(number, TOKEN_NUMBER)

#define L (*node)->left
#define R (*node)->right
#define NUMBER(node)   ((node)->type == TOKEN_NUMBER)
#define VARIABLE(node) ((node)->type == TOKEN_VARIABLE)


#define PLUS(left, right) \
  TreeSetNode(left, right, OP_PLUS, TOKEN_OPERATOR)

#define MINUS(left, right) \
  TreeSetNode(left, right, OP_MINUS, TOKEN_OPERATOR)

#define MULT(left, right) \
  TreeSetNode(left, right, OP_MULT, TOKEN_OPERATOR)

#define DIV(left, right) \
  TreeSetNode(left, right, OP_DIV, TOKEN_OPERATOR)

#define POW(left, right) \
  TreeSetNode(left, right, OP_POW, TOKEN_OPERATOR)
    
#define RETURN(sign) \
  return new_node;


int SimplifyPlus (Node** node);
int SimplifyMinus(Node** node); 
int SimplifyMult (Node** node); 
int SimplifyDiv  (Node** node);
int SimplifyPow  (Node** node); 


Node* TakeDerivative(Node* node) { 
  assert(node);
  
  switch ((node)->type) {
    
    case TOKEN_NUMBER: {
       
      Node* new_node = CreateNodeNumber(0, TOKEN_NUMBER);
      return new_node;
    }
    case TOKEN_VARIABLE: {
      
      Node* new_node = CreateNodeNumber(1, TOKEN_NUMBER);
      return new_node;
    }

    case TOKEN_OPERATOR: {
        
        switch (node->data.type) {
          
          case OP_PLUS: {
            Node* new_node = PLUS(dL, dR);
            
            RETURN('+');
          }

          case OP_MINUS: {
            Node* new_node = MINUS(dL, dR);
            
            RETURN('-');
          }

          case OP_DIV: {
            Node* new_node = DIV( MINUS(MULT(dL, cR), MULT(cL, dR)), MULT(cR, cR) );
            
            RETURN('/');
          }
          
          case OP_MULT: {
            Node* new_node = PLUS(MULT(dL, cR), MULT(cL, dR));
            
            RETURN("*");
          }

          case OP_POW: {
            Node* new_node = NULL; 
            if (NodeFind(node->left, 0, TOKEN_VARIABLE))
              new_node = MULT(MULT(POW(cL, MINUS(cR, DIG(1)) ), cR), dL);
            
            else {
              Node* ln_a = CreateNode(FUNC_LOGE, TOKEN_FUNCTION);
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

    case TOKEN_FUNCTION: {
        
      if (node->data.type == FUNC_COS) {
        node->data.type = FUNC_SIN;
       
        Node* new_node = MULT(MINUS(DIG(0), cN), dR);
        
        node->data.type = FUNC_COS;
        RETURN("cos");
      }
    
      if (node->data.type == FUNC_SIN) {
        node->data.type = FUNC_COS;
        
        Node* new_node = MULT(cN, dR);
        
        node->data.type = FUNC_SIN;
        RETURN("sin");
      }

      if (node->data.type == FUNC_TAN) {
        node->data.type = FUNC_COS;
        
        Node* new_node = MULT(DIV(DIG(1), POW(cN, DIG(2))), dR);
        
        node->data.type = FUNC_TAN;
        RETURN("tan");
      }
 
      if (node->data.type == FUNC_COT) {
        node->data.type = FUNC_SIN;
        
        Node* new_node = MULT(MINUS(DIG(0), DIV(DIG(1), POW(cN, DIG(2)))), dR);
        
        node->data.type = FUNC_COT;
        RETURN("cotan");
      }     

      if (node->data.type == FUNC_ACOS) {
        Node* new_node = MULT(MINUS(DIG(0), DIV(DIG(1), POW(MINUS(DIG(1), POW(cR, DIG(2))), DIG(0.5)))), dR);

        RETURN("acos");
      }
      
      if (node->data.type == FUNC_ASIN) {
        Node* new_node = MULT(DIV(DIG(1), POW(MINUS(DIG(1), POW(cR, DIG(2))), DIG(0.5))), dR);

        RETURN("asin");
      }

      if (node->data.type == FUNC_ATAN) {
        Node* new_node = MULT(DIV(DIG(1), PLUS(DIG(1), POW(cR, DIG(2)) )), dR);

        RETURN("atan");
      }

      if (node->data.type == FUNC_SINH) {
        node->data.type = FUNC_COSH;
        
        Node* new_node = MULT(cN, dR);
        
        node->data.type = FUNC_SINH;
        RETURN("sinh");
      }
      
      if (node->data.type == FUNC_COSH) {
        node->data.type = FUNC_SINH;
        
        Node* new_node = MULT(cN, dR);
        
        node->data.type = FUNC_COSH;
        RETURN("cosh");
      }
      
      if (node->data.type == FUNC_TANH) {
        node->data.type = FUNC_COSH;
        
        Node* new_node = MULT(DIV(DIG(1), POW(cN, DIG(2))), dR);
        
        node->data.type = FUNC_TANH;
        RETURN("tanh");
      }

      if (node->data.type == FUNC_LOGE) {
        Node* new_node = MULT(DIV(DIG(1), cR), dR);
        
        RETURN("loge");
      }
      
      if (node->data.type == FUNC_LOG10) {
        Node* ln_10 = CreateNode(FUNC_LOGE, TOKEN_FUNCTION);
        ln_10->right = CreateNode(10, TOKEN_NUMBER);

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
    (*node)->type = TOKEN_NUMBER;\
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
  if ((*node)->type == TOKEN_NUMBER || (*node)->type == TOKEN_VARIABLE) return 0;

  int was_simplified = 0;
  Node* old_tree = Copy(*node);
  
  // Simplify left
  if ((*node)->left != NULL) 
  if (!NUMBER(L) && !VARIABLE(L)) 
    was_simplified |= Simplify(&(L));

  // Simplify right
  if ((*node)->right != NULL)
  if (!NUMBER(R) && !VARIABLE(R)) 
    was_simplified |= Simplify(&(R));
  
  if ((*node)->type == TOKEN_OPERATOR)
  switch ( (*node)->data.type ) {
      case OP_PLUS:
        was_simplified |= SimplifyPlus(node);
        break;
      case OP_MINUS:
        was_simplified |= SimplifyMinus(node); 
        break;
      case OP_MULT:
        was_simplified |= SimplifyMult(node); 
        break;
      case OP_DIV:
        was_simplified |= SimplifyDiv(node);
        break;
      case OP_POW:
        was_simplified |= SimplifyPow(node); 
        break;
      
      default:
        TreeDelete(&old_tree);
        return 0;
    }
  
  TreeDelete(&old_tree);
  return was_simplified;
}

void MiddleEndSimplify(Node** tree) {
  assert(tree);
  if (*tree == NULL) return;

  if ((*tree)->type == TOKEN_OPERATOR)
    while (Simplify(tree) != 0); 
  
  else {
    MiddleEndSimplify( &((*tree)->left) );
    MiddleEndSimplify( &((*tree)->right) );
  }
}

void MiddleEndDeriv(Node** tree) {
  assert(tree);
  if (*tree == NULL) return;

  if ((*tree)->type == TOKEN_FUNCTION && (*tree)->data.type == 16) {
    
    //NodePrint((*tree)->right, "HelpFiles/tree.doxy");
    Node* new_node = TakeDerivative((*tree)->right);
    TreeDelete(tree);
    *tree = new_node; 
  }
  else {
    MiddleEndDeriv( &((*tree)->left) );
    MiddleEndDeriv( &((*tree)->right) );
  }
}

