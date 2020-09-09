#include "stdio.h"
#include "string.h"


///////////////////////////////////////////////////////////////////
// 
//                          Load Function
//
// tree      - empty tree
// file_name - there are from load tree
// mode      - type of the written tree
//                    0 - preorder
//                    1 - inorder
//                    2 - postorder
//
///////////////////////////////////////////////////////////////////
int TreeLoad(Tree* tree, const char* file_name, int mode = 0);

int PreorderLoadNodes(Tree* tree, Node** node, FILE* file);
int InorderLoadNodes(Tree* tree, Node** node, FILE* file);
int PostorderLoadNodes(Tree* tree, Node** node, FILE* file);

#include "load.cpp"
