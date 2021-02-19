#include "rbt.h"

void CrashTests();
void InsertTests();

int main()
{
  CrashTests();
  InsertTests();
  return 0;
}

void CrashTests()
{
  tree_t* tree = NULL;
  ConstructTree(&tree);
  Insert(tree, 19);

  RotateLeft(NULL);
  RotateRight(NULL);

  RotateLeft(tree->root_);
  RotateRight(tree->root_);

  PrintNodes(tree->root_, NULL);
  PrintNodes(NULL, NULL);
  
  // remove file
  //PrintTree(tree->root_);

  ConstructTree(NULL);
  FillNode(NULL,          NULL, 3, 1, NULL, NULL);
  FillNode(tree->root_,   NULL, 3, 1, NULL, NULL);
  NewNode(NULL,           NULL, 3, 1, NULL, NULL);
  NewNode(&(tree->root_), NULL, 3, 1, NULL, NULL);
  
  Grandparent(NULL);
  Uncle(NULL);
  Sibling(NULL);

  FindPlaceForNode(NULL, 1, NULL, NULL);
  FindPlaceForNode(tree, 1, NULL, NULL);
  FindPlaceForNode(tree, 1, &(tree->root_), NULL);

  Insert(NULL, 1);
  
  ReplaceNode(NULL, NULL);
  ReplaceNode(NULL, tree->root_);
  
  DeleteChild(NULL);
  
  printf("Crash tests completed!\n");

  return; 
}

void InsertTests()
{ 
  tree_t* tree = NULL;
  ConstructTree(&tree);
  
  Insert(tree, 19);
  Insert(tree, 18);
  Insert(tree, 17);
  Insert(tree, 16);
  Insert(tree, 15);
  Insert(tree, 14);
  Insert(tree, 13);
  Insert(tree, 12);
  Insert(tree, 11);
  Insert(tree, 10);
  Insert(tree, 9);
  Insert(tree, 8);
  
  Insert(tree, 20);
  Insert(tree, 21);
  Insert(tree, 22);
  Insert(tree, 23);
  Insert(tree, 24);
  Insert(tree, 25);
  Insert(tree, 26);
  Insert(tree, 27);
  Insert(tree, 28);
  Insert(tree, 29);
  
  tree_t* tree1 = NULL;
  ConstructTree(&tree1);
  
  Insert(tree1, 19);
  Insert(tree1, 18);
  Insert(tree1, 20);
  Insert(tree1, 17);
  Insert(tree1, 21);
  Insert(tree1, 16);
  Insert(tree1, 22);
  Insert(tree1, 20);
 
  return;
}





















