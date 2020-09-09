#include "tree.h"

#include <stdio.h>
#include <sort_lines.h>
#include "stack.h"
#include <linux/limits.h>

///////////////////////////////////// PRINT FUNCTION ///////////////////////////////////////////
void PrintNodes(Node* node, FILE* file) {
  assert(node != NULL);
  
  if (node->left != NULL) {
    fprintf(file, "\"%" TELEM_SYMBOL "\" -> " ,node->data);
    fprintf(file, "\"%" TELEM_SYMBOL "\" [label=\"no\"]; \n",node->left->data);
    PrintNodes(node->left, file);
  }

  if (node->right != NULL) {
    fprintf(file, "\"%" TELEM_SYMBOL "\" -> " ,node->data);
    fprintf(file, "\"%" TELEM_SYMBOL "\" [label=\"yes\"]; \n",node->right->data);
    PrintNodes(node->right, file); 
 }
}

int TreePrint(Tree* tree, const char* file_name) {
  assert(tree);
  
  FILE* output = NULL;
  if (OpenFile(file_name, "w", &output))
    return 0;
  
  fprintf(output, "digraph D {\n");
  PrintNodes(tree->root, output);
  fprintf(output, "}");
   
  fclose(output);
  
  char dot_program[PATH_MAX + 50];
  sprintf(dot_program, "dot -Tps %s -o DoxyFiles/graph.ps", file_name);
  
  system(dot_program);
  system("xdg-open DoxyFiles/graph.ps");

  return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////











///////////////////////////////////// WORK WITH TREE ///////////////////////////////////////////
int TreeInit(Tree* tree) {
  assert(tree != NULL);
  
  // tree
  Node* root = (Node*)calloc(1, sizeof(Node));
  if (root == NULL) return 1;

  root->left   = NULL;
  root->right  = NULL;
  root->data   = "";

  tree->root = root;
  tree->number_nodes = 0;
  
  return 0;
}

void TreeDisinit(Node* node) {
  assert(node);
    
  if (node->left  != NULL) TreeDisinit(node->left);
  if (node->right != NULL) TreeDisinit(node->right);

  free(node->data); 
  free(node);
  
  node->data = NULL;
}

int TreePush(Tree* tree, Node** node, tElem_t value) {
  assert(node);

  // if it is first element
  if (tree->number_nodes == 0) {

    tree->root->data = value;
    tree->number_nodes += 1;
    return 0;
  }
  
  // otherwise create new
  Node* new_node = (Node*)calloc(1, sizeof(Node));
  if (new_node == NULL)
    return 1;

  new_node->left   = NULL;
  new_node->right  = NULL;
  new_node->data   = (tElem_t)calloc(1, sizeof(value));
  new_node->data   = value;

  (*node) = new_node;
  tree->number_nodes += 1;

  return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////










///////////////////////////////////// FIND ELEMENT /////////////////////////////////////////////
int NodeFind(Node* node, tElem_t element, Stack_t** stack) {
  assert(stack);
  
  // We reach our element?
  if (TreeCompare(node->data, element) == 0) return 1;

  // Firstly, we analyse left tree, then right tree
  //
  // insert NOT before sign

  char* left_tree_string = (char*)calloc(strlen(node->data) + 5, sizeof(char));
  strcat(left_tree_string, "NOT ");
  strcat(left_tree_string, node->data);

  StackPush(*stack, left_tree_string);
  
  // recursive call left tree
  int error = 0;
  if (node->left != NULL) {
    if (NodeFind(node->left, element, stack) == 0) {
      char* temp_string = StackPop(*stack, &error);
      
    }
    else 
      return 1;
  }
  
  // right tree => delete NOT
  char* temp_string = StackPop(*stack, &error);
  char* right_tree_string = (char*)calloc(strlen(temp_string) - 4, sizeof(char));
  strcat(right_tree_string, &(temp_string[4]));
  free(temp_string);

  StackPush(*stack, right_tree_string);

  // recursive call right tree
  if (node->right != NULL) {
    if (NodeFind(node->right, element, stack) == 0) {
      char* temp_string = StackPop(*stack, &error);
      
      free(temp_string);
    }
    else 
      return 1;
  }
  
  return 0;
}

int TreeFindElement(Tree* tree, tElem_t element, Stack_t* stack) {
  assert(tree);
  
  StackInit(stack, tree->number_nodes);
 
  return NodeFind(tree->root, element, &stack);
}
////////////////////////////////////////////////////////////////////////////////////////////////



#include "SaveFunction/save.h"
#include "LoadFunction/load.h"



int not_main() {
  
  
  Tree tree = {};
  TreeInit(&tree);

  TreePush(&tree, &(tree.root), "Green");
  TreePush(&tree, &(tree.root->left), "Yelow");
  TreePush(&tree, &(tree.root->right), "White");
  TreePush(&tree, &(tree.root->left->left), "Orange");
  TreePush(&tree, &(tree.root->left->right), "Blue");

  TreePrint(&tree, "DoxyFiles/tree.doxy");
  TreeSave(&tree, "my_tree", 2);  

  TreeLoad(&tree, "my_tree", 2);
  
  //TreePrint(&tree, "doxy_file");

  TreeDisinit(tree.root);
  return 0;
}
