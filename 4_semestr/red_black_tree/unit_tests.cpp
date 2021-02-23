#include "rbt.h"

void CrashTests();
void Tests();

int main()
{
  CrashTests();
  Tests();
  return 0;
}

void CrashTests()
{
  tree_t* tree = NULL;
  ConstructTree(&tree);
  Insert(tree, 19);
  Find(tree, 18);

  Find(NULL, 19);

  ConstructTree(NULL);
  
  Grandparent(NULL);
  Uncle(NULL);
  Sibling(NULL);

  Insert(NULL, 1);
  Delete(NULL);
  
  PrintTree(NULL);
  BreakSystemCall(MY_SYSTEM);
  PrintTree(tree->root_);
  RestoreSystemCall(MY_SYSTEM);
  
  BreakSystemCall(MY_FOPEN);
  PrintTree(tree->root_);
  RestoreSystemCall(MY_FOPEN);

  BreakSystemCall(MY_FCLOSE);
  PrintTree(tree->root_);
  RestoreSystemCall(MY_FCLOSE);
  
  BreakSystemCall(MY_CALLOC);
  ConstructTree(&tree);
  RestoreSystemCall(MY_CALLOC);
 
  BreakSystemCall(MY_CALLOC);
  Insert(tree, 1);
  RestoreSystemCall(MY_CALLOC);

  Foreach(NULL, PrintTree_job, NULL);

  DestructTree(tree);
  printf("Crash tests completed!\n");
}

void Tests()
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
  
  Delete(Find(tree, 16));
  Delete(Find(tree, 23));
  Delete(Find(tree, 22));
  Delete(Find(tree, 14));
  Delete(Find(tree, 17));
  Delete(Find(tree, 18));
  Delete(Find(tree, 21));
  Delete(Find(tree, 13));

  Insert(tree, 16);
  Insert(tree, 23);
  Insert(tree, 22);
  Insert(tree, 14);
  Insert(tree, 17);
  Insert(tree, 18);
  Insert(tree, 21);
  Insert(tree, 7);
  Insert(tree, 6);
  Insert(tree, 5);
  Insert(tree, 4);
  Insert(tree, 3);
  Insert(tree, 2);
  Insert(tree, 2);
  
  Delete(Find(tree, 25));
  Delete(Find(tree, 17));
  Delete(Find(tree, 15));
  Delete(Find(tree, 18));
  Delete(Find(tree, 16));
  Delete(Find(tree, 9));
  Delete(Find(tree, 29));
  Delete(Find(tree, 19));
  Delete(Find(tree, 20));
  Delete(Find(tree, 2));
  Delete(Find(tree, 5));

  Insert(tree, 9);
  Insert(tree, 2);
  Insert(tree, 5);
  Insert(tree, 25);
  Insert(tree, 17);
  Insert(tree, 15);
  Insert(tree, 18);
  Insert(tree, 16);
  Insert(tree, 29);
  Insert(tree, 19);
  Insert(tree, 20);
  
  Delete(Find(tree, 6));
  PrintTree(tree->root_);
  DestructTree(tree);

  tree_t* tree1 = NULL;
  ConstructTree(&tree1);
  
  Insert(tree1, 16);
  Insert(tree1, 18);
  Insert(tree1, 19);
  Insert(tree1, 14);
  Insert(tree1, 17);
  Insert(tree1, 15);
  DestructTree(tree1);

  printf("Normal tests completed!\n");
}





















