#include "rbt.h"

error_t Foreach(node_t* root, int(*job)(node_t* node, data_t data, void* extra), void* extra)
{
  error_t error = NO_ERROR;

  if (root == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Tree root can not be NULL");
  if (job == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Job can not be NULL");
  
  if (root->left_ != NULL)
    Foreach(root->left_, job, extra);
  
  if (job(root, data, extra) != 0)
    ERROR(ERROR_DETECTED, "Error in job");  
  
  if (root->right_ != NULL)
    Foreach(root->right_, job, extra);
	
  return error;
}
const char* DOT_FILENAME = "DoxyFiles/tree.dot";
///////////////////////////////////// PRINT FUNCTION ///////////////////////////////////////////
void PrintNodes(node_t* node, FILE* file) 
{
  assert(node);
  assert(file);

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
}

error_t PrintTree(node_t* root) 
{
  error_t error = NO_ERROR;

  if (root == NULL)
    return NO_ERROR;

  FILE* output = fopen(DOT_FILENAME, "w");
  if (output == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Can not be null: %s\n", "output");


  fprintf(output, "digraph D {\n");
  PrintNodes(root, output);
  fprintf(output, "}");
   
  if (fclose(output) == EOF)
    ERROR(ERROR_CLOSE, "Can not be closed: %s\n", "output");
 
  const char dot_command[] = "dot -Tps %s -o DoxyFiles/graph.ps";
  char dot_program[PATH_MAX + sizeof(dot_command)];
  sprintf(dot_program, dot_command, DOT_FILENAME);
  
  if (system(dot_program) == -1)
    ERROR(ERROR_SYSTEM, "Can not be executed\n");

  if (system("xdg-open DoxyFiles/graph.ps") == -1)
    ERROR(ERROR_SYSTEM, "Can not be executed\n");


  return error;
}

int PrintTree_job(node_t* node, data_t data, void* extra)
{
  if (node == NULL)
    return NULL;
  if (extra == NULL)
    return 1;
  
  FILE* file = (FILE*)extra; 

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
  
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////

error_t ConstructTree(tree_t** tree)
{
  error_t error = NO_ERROR;

  if (tree == NULL)
    ERROR(ERROR_PTR_IS_NULL, "Can not be null: *tree\n");

  *tree = (tree_t*)calloc(1, sizeof(tree));
  if (*tree == NULL)
    ERROR(ERROR_CALLOC, "Can not be calloced: *tree\n");

  (*tree)->root_ = NULL;

  return error;
}

int DestructTree_job(node_t* node, data_t data, void* extra)
{
  if (node == NULL)
    return 0;

  if (node->parent_ != NULL)
    if (node->parent_->left_ == node)
      node->parent_->left_ = NULL
    else
      node->parent_->left_ = NULL

  node->data_ = 0;
  node->color_ = 0;
  node->left_ = NULL;
  node->left_ = NULL;
  node->parent_ = NULL;

  free(node);
	
  return 0;
}
error_t DestructTree(tree_t* tree)
{
  error_t error = NO_ERROR;

  if (tree == NULL)
    return NO_ERROR;

  return Foreach(tree->root_, DestructTree_job, NULL);
}

void FillNode(node_t* new_node, node_t* parent, color_t color, data_t data, node_t* left, node_t* right)
{
  assert(new_node); 
  assert(color == Red || color == Black);

  new_node->parent_ = parent;
  new_node->color_  = color;
  new_node->data_   = data;
  new_node->left_   = left;
  new_node->right_  = right;
}

error_t NewNode(node_t** new_node, node_t* parent, color_t color, data_t data, node_t* left, node_t* right)
{
  error_t error = NO_ERROR;
  
  assert(new_node);

  *new_node = (node_t*) calloc(1, sizeof(node_t));
  if (new_node == NULL)
    ERROR(ERROR_CALLOC, "Can not be null: %s\n", "node->left_");

  FillNode(*new_node, parent, color, data, left, right);

  return error;
}

void FreeNode(node_t* node)
{
  assert(node);

  if (node->parent_ != NULL)
    assert( !(node->parent_->left_ == node || node->parent_->right_ == node) );

  node->data_ = 0;
  node->color_ = 0;
  node->left_ = NULL;
  node->left_ = NULL;
  node->parent_ = NULL;

  free(node);
}

node_t* Grandparent(node_t* node)
{
  if (node != NULL && node->parent_ != NULL)
    return node->parent_->parent_;
  else
    return NULL;
}

node_t* Uncle(node_t* node)
{
  node_t* grandpa = Grandparent(node);

  if (grandpa == NULL)
    return NULL;
  
  if (node->parent_ == grandpa->left_)
    return grandpa->right_;
  else
    return grandpa->left_;
}

node_t* Sibling(node_t* node)
{
  if (node == NULL || node->parent_ == NULL)
    return NULL; 

  if (node == node->parent_->left_)
    return node->parent_->right_;
  else
    return node->parent_->left_;
}

void RotateLeft(node_t* node)
{
  assert(node);
  assert(node->right_);

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
}

void RotateRight(node_t* node)
{
  assert(node);
  assert(node->left_);

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
}

bool FindPlaceForNode(tree_t* tree, data_t data, node_t** new_node, node_t** new_node_parent)
{
  assert(tree);
  assert(new_node);
  assert(new_node_parent);

  *new_node = tree->root_;
  *new_node_parent = tree->root_;
  while (*new_node != NULL)
  {
    *new_node_parent = *new_node;

    // data is already exist in tree 
    if (data == (*new_node)->data_)
    {
      WARNING("The node with this data: " PRINTF_DATA " is already in the tree", data);
      return 0;
    }
    if (data > (*new_node)->data_)
      *new_node = (*new_node)->right_;
    else
      *new_node = (*new_node)->left_;
  }

  return 1;
}

///////////// CASE 4: parent is red, uncle is black /////////
void InsertCase4(tree_t* tree, node_t* node)
{
  assert(node);
  assert(tree);
 
  node_t* g = Grandparent(node);

  // Rotates to put tree in special order
  if ( (node == node->parent_->right_) && (node->parent_ == g->left_) )
  {
    RotateLeft(node->parent_);
    node = node->left_;
  }
  else if ( (node == node->parent_->left_) && (node->parent_ == g->right_) )
  {
    RotateRight(node->parent_);
    node = node->right_;
  }
 
  node->parent_->color_ = Black;
  g->color_ = Red;
  if ( (node == node->parent_->left_) && (node->parent_ == g->left_) )
  {
    if (tree->root_ == g)
      tree->root_ = g->left_;
    RotateRight(g);
  }
  else
  {
    if (tree->root_ == g)
      tree->root_ = g->right_;
    RotateLeft(g);
  }
}

//////////// CASE 3: parent and uncle are red ////////////////
void InsertCase3(tree_t* tree, node_t* node)
{
  assert(node);
  assert(tree);

  node_t* u = Uncle(node), *g;
  if ( (u != NULL) && (u->color_ == Red) )
  {
    node->parent_->color_ = Black;
    u->color_ = Black;
    g = Grandparent(node);
    g->color_ = Red;
    InsertCase1(tree, g);
  }
  else
    InsertCase4(tree, node);
}

///////////// CASE 2: parent is black ////////////////////////
void InsertCase2(tree_t* tree, node_t* node)
{
  assert(node);
  assert(tree);

  if (node->parent_->color_ != Black)
    InsertCase3(tree, node);
}


///////////// CASE 1: new_node == root of tree //////////////////
void InsertCase1(tree_t* tree, node_t* node)
{
  assert(node);
  assert(tree);

  if (node->parent_ == NULL)
  {
    node->color_ = Black;
  }
  else
    InsertCase2(tree, node);
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

void ReplaceNode(node_t* node, node_t* child)
{
  assert(node);
  assert(node->parent_);
 
  if (child != NULL)
    child->parent_ = node->parent_;
  if (node == node->parent_->left_)
    node->parent_->left_ = child;
  else
    node->parent_->right_ = child;
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


///////////// CASE 1: node == root of tree //////////////////////
void DeleteCase1(node_t* node)
{
  assert(node);
  
  if (node->parent_ != NULL)
    DeleteCase2(node);
}

///////////// CASE 2: sibling is red //////////////////////
void DeleteCase2(node_t* node)
{
  assert(node);
  
  node_t* sib = Sibling(node);
  assert(sib);

  if (sib->color_ == Red)
  {
    node->parent_->color_ = Red;
    sib->color_ = Black;
    if (node == node->parent_->left_)
      RotateLeft(node->parent_);
    else
      RotateRight(node->parent_);
  }
  
  DeleteCase3(node);
}


///////////// CASE 3: parent, sibling and grandfather red //////////////////////
void DeleteCase3(node_t* node)
{
  assert(node);

  node_t* sib = Sibling(node);
  assert(sib);

  if ( (node->parent_->color_ == Black) && (sib->color_ == Black) )
  {
    if ( (sib->left_ == NULL || sib->left_->color_ == Black) &&
         (sib->right_ == NULL || sib->right_->color_ == Black) )
    {
      sib->color_ = Red;
      DeleteCase1(node->parent_); 
    }
    else
    {
      DeleteCase4(node);
    }
  }
  else 
    DeleteCase4(node);
}

///////////// CASE 4: sibling and his childs are black, but parent is red //////////
void DeleteCase4(node_t* node)
{
  assert(node);

  node_t* sib = Sibling(node);
  assert(sib);
  
  if ( (node->parent_->color_ == Red) && (sib->color_ == Black) )
  {
     if ( (sib->left_ == NULL || sib->left_->color_ == Black) &&
         (sib->right_ == NULL || sib->right_->color_ == Black) )
    {
      sib->color_ = Red;
      node->parent_->color_ = Black;
    }
    else
    {
      DeleteCase5(node);
    }
  }
  else 
    DeleteCase5(node);
}


///////////// CASE 5: sibling black, his left child is red, other is black, and node is left child of parent //////////
void DeleteCase5(node_t* node)
{
  assert(node);

  node_t* sib = Sibling(node);
  assert(sib);

  if  (sib->color_ == Black) 
  {
		if ((node == node->parent_->left_) && (sib->right_ == NULL || sib->right_->color_ == Black) 
                                       && (sib->left_->color_ == Red)) 
    { 
			sib->color_ = Red;
		  sib->left_->color_ = Black;
			RotateRight(sib);
		}
    else if ((node == node->parent_->right_) && (sib->left_ == NULL || sib->left_->color_ == Black) 
                                             && (sib->right_->color_ == Red)) 
    {
			sib->color_ = Red;
			sib->right_->color_ = Black;
			RotateLeft(sib);
		}
	}
	DeleteCase6(node);
}


/// LAST CASE ////
void DeleteCase6(node_t* node)
{
  assert(node);

	node_t* sib = Sibling(node);
  assert(sib);

	sib->color_ = node->parent_->color_;
  node->parent_->color_ = Black;

	if (node == node->parent_->left_) 
  {
    assert(sib->right_);
    
    sib->right_->color_ = Black;
		RotateLeft(node->parent_);
	} 
  else 
  {
    assert(sib->left_);
		
    sib->left_->color_ = Black;
		RotateRight(node->parent_);
	}
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


  














