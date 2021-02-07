#include "rbt.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

struct node_t 
{
  node_t* parent_;
  color_t color_;
  data_t  data_;
  node_t* left_;
  node_t* right_;
};

const char* DOT_FILENAME = "DoxyFiles/tree.dot";
///////////////////////////////////// PRINT FUNCTION ///////////////////////////////////////////
error_t PrintNodes(node_t* node, FILE* file) 
{
  error_t error = NO_ERROR;
  CHECK_ERROR(node, ERROR_PTR_IS_NULL);
  CHECK_ERROR(file, ERROR_PTR_IS_NULL);
  
  if (node->color_ == Red)
    fprintf(file, "\"%p\" [label=\"" PRINTF_DATA "\", style=filled, fillcolor=red, fontcolor=white]; \n", node, node->data_);
  else
    fprintf(file, "\"%p\" [label=\"" PRINTF_DATA "\", style=filled, fillcolor=black, fontcolor=white]; \n", node, node->data_);
  
  
  fprintf(file, "\"%p\" -> " ,node);
  
  if (node->left_ != NULL) 
  {
   fprintf(file, "\"%p\"\n", node->left_);
   PrintNodes(node->left_, file);
  }
  else
  {
    fprintf(file, "\"%p%s\"\n", node, "left");
    fprintf(file, "\"%p%s\" [label=\"%s\", style=filled, fillcolor=black, fontcolor=white]; \n", node, "left", "NULL");
  }

  fprintf(file, "\"%p\" -> " ,node);

  if (node->right_ != NULL) 
  {
    fprintf(file, "\"%p\"\n", node->right_);
    PrintNodes(node->right_, file); 
  }
  else
  {
    fprintf(file, "\"%p%s\"\n", node, "right");
    fprintf(file, "\"%p%s\" [label=\"%s\", style=filled, fillcolor=black, fontcolor=white]; \n", node, "right", "NULL");
  }

 return error;
}

error_t PrintTree(node_t* tree) 
{
  error_t error = NO_ERROR;
  CHECK_ERROR(tree, ERROR_PTR_IS_NULL);
  
  FILE* output = fopen(DOT_FILENAME, "w");
  CHECK_ERROR(output, ERROR_PTR_IS_NULL);
  
  fprintf(output, "digraph D {\n");
  error = PrintNodes(tree, output);
  if (error != NO_ERROR)
    return error;
  fprintf(output, "}");
   
  CHECK_ERROR(fclose(output), ERROR_PTR_IS_NULL);
  
  const char dot_command[] = "dot -Tps %s -o DoxyFiles/graph.ps";
  char dot_program[PATH_MAX + sizeof(dot_command)];
  sprintf(dot_program, dot_command, DOT_FILENAME);
  
  system(dot_program);
  system("xdg-open DoxyFiles/graph.ps");

  return error;
}
////////////////////////////////////////////////////////////////////////////////////////////////

error_t FillNode(node_t* new_node, node_t* parent, color_t color, data_t data, node_t* left, node_t* right)
{
  error_t error = NO_ERROR;
  CHECK_ERROR(new_node, ERROR_PTR_IS_NULL);

  new_node->parent_ = parent;
  new_node->color_  = color;
  CHECK_ERROR(color, ERROR_WITH_COLOR);

  new_node->data_   = data;
  new_node->left_   = left;
  new_node->right_  = right;

  return error;
}

error_t NewNode(node_t** new_node, node_t* parent, color_t color, data_t data, node_t* left, node_t* right)
{
  error_t error = NO_ERROR;
  CHECK_ERROR(new_node, ERROR_PTR_IS_NULL);

  *new_node = (node_t*) calloc(1, sizeof(node_t));
  CHECK_ERROR(new_node, ERROR_WITH_CALLOC);

  error = FillNode(*new_node, parent, color, data, left, right);

  if (error != NO_ERROR)
    return ERROR_DETECTED;
  
  return error;
}

node_t* Grandparent(node_t* node)
{
  if (node != NULL && node->parent_ != NULL)
    return node->parent_->parent_;
  else
    return NULL;
}

node_t * Uncle(node_t* node)
{
  node_t* grandpa = Grandparent(node);

  if (grandpa == NULL)
    return NULL;
  
  if (node->parent_ == grandpa->left_)
    return grandpa->right_;
  else
    return grandpa->left_;
}

error_t RotateLeft(node_t* node)
{
  CHECK_ERROR(node, ERROR_PTR_IS_NULL);
  CHECK_ERROR(node->right_, ERROR_PTR_IS_NULL);
  
  node_t* pivot = node->right_;
  pivot->parent_ = node->parent_;
  if (node->parent_ != NULL)
  {
    if (node->parent_->left_ == node)
      node->parent_->left_ = pivot;
    else
      node->parent_->right_ = pivot;
  }

  node->right_ = pivot->left_;
  if (pivot->left_ != NULL)
    pivot->left_->parent_ = node;

  node->parent_ = pivot;
  pivot->left_ = node;

  return NO_ERROR;
}

error_t RotateRight(node_t* node)
{
  CHECK_ERROR(node, ERROR_PTR_IS_NULL);
  CHECK_ERROR(node->left_, ERROR_PTR_IS_NULL);
  
  node_t* pivot = node->left_;
  pivot->parent_ = node->parent_;
  if (node->parent_ != NULL)
  {
    if (node->parent_->left_ == node)
      node->parent_->left_ = pivot;
    else
      node->parent_->right_ = pivot;
  }

  node->left_ = pivot->right_;
  if (pivot->right_ != NULL)
    pivot->right_->parent_ = node;

  node->parent_ = pivot;
  pivot->right_ = node;

  return NO_ERROR;
}

error_t Insert(node_t* tree, data_t data)
{
  CHECK_ERROR(tree, ERROR_PTR_IS_NULL); 

  // find the place for new data
  node_t* new_node = tree;
  node_t* new_node_parent = tree;
  while (new_node != NULL)
  {
    new_node_parent = new_node;

    // data is already exist in tree 
    if (data == new_node->data_)
    {
      WARNING("The node with this data:" PRINTF_DATA " is already in the tree", data);
      return NO_ERROR;
    }
    if (data > new_node->data_)
      new_node = new_node->right_;
    else
      new_node = new_node->left_;
  }
  
  if (NewNode(&new_node, new_node_parent, Red, data, NULL, NULL) != NO_ERROR)
    return ERROR_DETECTED;

  if (data > new_node_parent->data_)
    new_node_parent->right_ = new_node;
  else
    new_node_parent->left_ = new_node;

  return NO_ERROR;
}

int main()
{
  node_t* test_node = NULL;

  FillNode(test_node, NULL, Red, 17, NULL, NULL);
  //Insert(test_node, 17);

  //PrintTree(test_node);
  return 0;
}
