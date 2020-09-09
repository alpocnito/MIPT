#include "stdio.h"
#include "string.h"


///////////////////////////////////////////////////////////////////
// 
//                          Save Function
//
// tree      - tree to save
// file_name - there save tree
// mode      - type of the saving format
//                    0 - preorder
//                    1 - inorder
//                    2 - postorder
//
///////////////////////////////////////////////////////////////////
int TreeSave(Tree* tree, const char* file_name, int mode = 0);

void PreorderSaveNodes(Node* node, FILE* file);
void InorderSaveNodes(Node* node, FILE* file);
void PostorderSaveNodes(Node* node, FILE* file);

#include "save.cpp"
