#include "rbt.h"

error_t Foreach(node_t* root, int(*job)(node_t* node, void* extra), void* extra)
{
  error_t error = NO_ERROR;

  if (root == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Tree root can not be NULL");
  if (job == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Job can not be NULL");
  
  if (root->left_ != NULL)
    Foreach(root->left_, job, extra);
  
  if (job(root, extra) != 0)
    ERROR(ERROR_DETECTED, "Error in job");  
  
  if (root->right_ != NULL)
    Foreach(root->right_, job, extra);
	
  return error;
}


const char* DOT_FILENAME = "DoxyFiles/tree.dot";
error_t PrintTree(node_t* root) 
{
  error_t error = NO_ERROR;

  if (root == NULL)
    return NO_ERROR;

  FILE* output = my_fopen(DOT_FILENAME, "w");
  if (output == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Output file can not be null");


  fprintf(output, "digraph D {\n");
  Foreach(root, PrintTree_job, output);
  //PrintNodes(root, output);
  fprintf(output, "}");
   
  if (my_fclose(output) == EOF)
    ERROR(ERROR_CLOSE, "Can not close output file");
 
  const char dot_command[] = "dot -Tps %s -o DoxyFiles/graph.ps";
  char dot_program[PATH_MAX + sizeof(dot_command)];
  sprintf(dot_program, dot_command, DOT_FILENAME);
  
  if (my_system(dot_program) == -1)
    ERROR(ERROR_SYSTEM, "Can not execute");

  if (my_system("xdg-open DoxyFiles/graph.ps") == -1)
    ERROR(ERROR_SYSTEM, "Can not execute");


  return error;
}


error_t ConstructTree(tree_t** tree)
{
  error_t error = NO_ERROR;

  if (tree == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Can not be null: *tree");

  *tree = (tree_t*)my_calloc(1, sizeof(tree));
  if (*tree == NULL)
    ERROR(ERROR_CALLOC, "Can not be calloced: *tree");

  (*tree)->root_ = NULL;

  return error;
}

error_t DestructTree(tree_t* tree)
{
  if (tree == NULL)
    return NO_ERROR;

  return Foreach(tree->root_, DestructTree_job, NULL);
}

error_t Insert(tree_t* tree, data_t data)
{
  error_t error = NO_ERROR;
   
  if (tree == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Tree can not be null");

  // if tree is empty
  node_t* new_node = NULL;
  node_t* new_node_parent = NULL;
  if (!FindPlaceForNode(tree, data, &new_node, &new_node_parent))
    return error;

  if (NewNode(&new_node, new_node_parent, Red, data, NULL, NULL) != NO_ERROR)
    return ERROR_DETECTED;
  
  if (new_node_parent != NULL)
  {
    // Placing new node
    if (data > new_node_parent->data_)
      new_node_parent->right_ = new_node;
    else
      new_node_parent->left_ = new_node;
  }
  else
  {
    // tree root is not exist
    tree->root_ = new_node;
  }

  InsertCase1(tree, new_node);
  return error;
}

error_t Delete(node_t* node)
{
  error_t error = NO_ERROR;

  if (node == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Can not be null: node");
  
  if (node->left_ != NULL && node->right_ != NULL)
  {
    node_t* tnode = node->left_;
    while (tnode->right_ != NULL)
      tnode = tnode->right_;

    assert(!(tnode->data_ > node->right_->data_ || tnode->data_ < node->left_->data_));
    /*if (tnode->data_ > node->right_->data_ || tnode->data_ < node->left_->data_)
    {
      tnode = node->right_;
     
      while (tnode->left_ != NULL)
        tnode = tnode->left_;

      if (tnode->data_ > node->right_->data_ || tnode->data_ < node->left_->data_)
      {
        printf("?????????????????????????????\n");
        abort();
      }
    }*/

    node->data_ = tnode->data_;
    node = tnode;
  }
  
  node_t* child = NULL;
  if (node->right_ == NULL)
    child = node->left_;
  else
    child = node->right_;
  
  if (child == NULL)
  {
    if (node->color_ == Black)
      DeleteCase1(node);
    ReplaceNode(node, child);
    FreeNode(node);
    return error;
  }

  ReplaceNode(node, child);
  if (node->color_ == Black)
  {
    if (child->color_ == Red)
      child->color_ = Black;
    else
      DeleteCase1(child);
  }

  FreeNode(node); 
  return error;
}

node_t* Find(tree_t* tree, data_t data)
{
  if (tree == NULL)
    return NULL;
  
  node_t* cur_node = tree->root_;
  while (cur_node != NULL)
  {
    if (data == cur_node->data_)
      return cur_node;

    if (data < cur_node->data_)
      cur_node = cur_node->left_;
    else 
      cur_node = cur_node->right_;
  }
  
  return NULL;
}
