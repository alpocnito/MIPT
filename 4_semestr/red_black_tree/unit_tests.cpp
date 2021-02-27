#include "rbt.h"
#include "rbt_inner.h"

int Bad_job(node_t* node, void* extra)
{
  return 1;
}


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
  TreeConstruct(&tree);
  TreeInsert(tree, 19);
  TreeFind(tree, 18);
  TreeGetRoot(tree);
  TreeGetRoot(NULL);
  NodeGetData(TreeGetRoot(tree));
  NodeGetData(NULL);

  TreeFind(NULL, 19);
  TreeConstruct(NULL);
 
 /*
  Grandparent(NULL);
  Uncle(NULL);
  Sibling(NULL);
*/
  TreeInsert(NULL, 1);
  TreeDelete(NULL);
  
  TreePrint(NULL, NULL);
  BreakSystemCall(MY_SYSTEM);
  TreePrint(TreeGetRoot(tree), "DoxyFiles/tree.dot");
  RestoreSystemCall(MY_SYSTEM);
  
  BreakSystemCall(MY_FOPEN);
  TreePrint(TreeGetRoot(tree), "DoxyFiles/tree.dot");
  RestoreSystemCall(MY_FOPEN);

  BreakSystemCall(MY_FCLOSE);
  TreePrint(TreeGetRoot(tree), "DoxyFiles/tree.dot");
  RestoreSystemCall(MY_FCLOSE);

  BreakSystemCall(MY_CALLOC);
  TreeInsert(tree, 1);
  
  tree_t* temp_tree = (tree_t*)3;
  TreeConstruct(&temp_tree);
  RestoreSystemCall(MY_CALLOC);

  TreeForeach(NULL, Bad_job, NULL);
  TreeForeach(tree->root_, Bad_job, NULL);

  TreeDestruct(tree);
  TreeDestruct(NULL);
  
  printf("Crash tests completed!\n");
}

void Tests()
{ 
  tree_t* tree = NULL;
  TreeConstruct(&tree);
  
  TreeInsert(tree, 19);
  TreeInsert(tree, 18);
  TreeInsert(tree, 17);
  TreeInsert(tree, 16);
  TreeInsert(tree, 15);
  TreeInsert(tree, 14);
  TreeInsert(tree, 13);
  TreeInsert(tree, 12);
  TreeInsert(tree, 11);
  TreeInsert(tree, 10);
  TreeInsert(tree, 9);
  TreeInsert(tree, 8);
  
  TreeInsert(tree, 20);
  TreeInsert(tree, 21);
  TreeInsert(tree, 22);
  TreeInsert(tree, 23);
  TreeInsert(tree, 24);
  TreeInsert(tree, 25);
  TreeInsert(tree, 26);
  TreeInsert(tree, 27);
  TreeInsert(tree, 28);
  TreeInsert(tree, 29);
  
  TreeDelete(TreeFind(tree, 16));
  TreeDelete(TreeFind(tree, 23));
  TreeDelete(TreeFind(tree, 22));
  TreeDelete(TreeFind(tree, 14));
  TreeDelete(TreeFind(tree, 17));
  TreeDelete(TreeFind(tree, 18));
  TreeDelete(TreeFind(tree, 21));
  TreeDelete(TreeFind(tree, 13));
  
  TreeInsert(tree, 16);
  TreeInsert(tree, 23);
  TreeInsert(tree, 22);
  TreeInsert(tree, 14);
  TreeInsert(tree, 17);
  TreeInsert(tree, 18);
  TreeInsert(tree, 21);
  TreeInsert(tree, 7);
  TreeInsert(tree, 6);
  TreeInsert(tree, 5);
  TreeInsert(tree, 4);
  TreeInsert(tree, 3);
  TreeInsert(tree, 2);
  TreeInsert(tree, 2);
   
  TreeDelete(TreeFind(tree, 25));
  TreeDelete(TreeFind(tree, 17));
  TreeDelete(TreeFind(tree, 15));
  TreeDelete(TreeFind(tree, 18));
  TreeDelete(TreeFind(tree, 16));
  TreeDelete(TreeFind(tree, 9));
  TreeDelete(TreeFind(tree, 29));
  TreeDelete(TreeFind(tree, 19));
  TreeDelete(TreeFind(tree, 20));
  TreeDelete(TreeFind(tree, 2));
  TreeDelete(TreeFind(tree, 5));
  
  TreeInsert(tree, 9);
  TreeInsert(tree, 2);
  TreeInsert(tree, 5);
  TreeInsert(tree, 25);
  TreeInsert(tree, 17);
  TreeInsert(tree, 15);
  TreeInsert(tree, 18);
  TreeInsert(tree, 16);
  TreeInsert(tree, 29);
  TreeInsert(tree, 19);
  TreeInsert(tree, 20);
  
  TreeDelete(TreeFind(tree, 6));
  TreePrint(TreeGetRoot(tree), "DoxyFiles/tree.dot");
  TreeDestruct(tree);
  
  tree_t* tree1 = NULL;
  TreeConstruct(&tree1);
  
  TreeInsert(tree1, 16);
  TreeInsert(tree1, 18);
  TreeInsert(tree1, 19);
  TreeInsert(tree1, 14);
  TreeInsert(tree1, 17);
  TreeInsert(tree1, 15);
  TreeDestruct(tree1);

  printf("Normal tests completed!\n");
}
