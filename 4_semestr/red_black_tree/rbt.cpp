#include "rbt.h"
#include "rbt_inner.h"

error_t TreeForeach(node_t* root, int(*job)(node_t* node, void* extra), void* extra)
{
  error_t error = NO_ERROR;

  if (root == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Tree root can not be NULL");
  
  if (root->left_ != NULL)
    TreeForeach(root->left_, job, extra);
  
  if (root->right_ != NULL)
    TreeForeach(root->right_, job, extra);
	
  if (job(root, extra) != 0)
    ERROR(ERROR_DETECTED, "Error in job");  
  
  return error;
}


error_t TreePrint(node_t* root, const char* temp_file) 
{
  error_t error = NO_ERROR;

  if (root == NULL)
    return NO_ERROR;

  FILE* output = my_fopen(temp_file, "w");
  if (output == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Output file can not be null");

  fprintf(output, "digraph D {\n");
  TreeForeach(root, PrintTree_job, output);
  //PrintNodes(root, output);
  fprintf(output, "}");
   
  if (my_fclose(output) == EOF)
    ERROR(ERROR_CLOSE, "Can not close output file");
 
  const char dot_command[] = "dot -Tps %s -o DoxyFiles/graph.ps";
  char dot_program[PATH_MAX + sizeof(dot_command)];
  sprintf(dot_program, dot_command, temp_file);
  
  if (my_system(dot_program) == -1)
    ERROR(ERROR_SYSTEM, "Can not execute");

  if (my_system("xdg-open DoxyFiles/graph.ps") == -1)
    ERROR(ERROR_SYSTEM, "Can not execute");

  return error;
}


error_t TreeConstruct(tree_t** tree)
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

error_t TreeDestruct(tree_t* tree)
{
  if (tree == NULL)
    return NO_ERROR;

  error_t error = TreeForeach(tree->root_, DestructTree_job, NULL);
  free(tree);
  
  return error;
}

error_t TreeInsert(tree_t* tree, data_t data)
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

error_t TreeDelete(node_t* node)
{
  error_t error = NO_ERROR;

  if (node == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Can not be null: node");
  
  if (node->left_ != NULL && node->right_ != NULL)
  {
    node_t* tnode = node->left_;
    while (tnode->right_ != NULL)
      tnode = tnode->right_;

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
      child->color_ = Black;

  FreeNode(node); 
  return error;
}

node_t* TreeFind(tree_t* tree, data_t data)
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

node_t* TreeGetRoot(tree_t* tree)
{
  if (tree != NULL)
    return tree->root_;

  return NULL;
}

data_t NodeGetData(node_t* node)
{
  if (node != NULL)
    return node->data_;

  return 0;
}
