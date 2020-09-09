#include "string.h"

enum MODE {
  PREORDER,
  INORDER,
  POSTORDER
};

typedef char* tElem_t;
#define TELEM_SYMBOL "s"

const int MAX_LEN_ANSWER    = 100;
const int MAX_LEN_NODE_DATA = 50;

struct Node {
  tElem_t data;
  Node*   left;
  Node*   right;
};

struct Tree {
  Node*  root;
  size_t number_nodes;
};


int TreeCompare(tElem_t first, tElem_t second) {
  return strcmp(first, second); 
}

void ClearStdin() {
  while (getchar() != '\n');
}
