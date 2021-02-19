#include "color.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

typedef unsigned char color_t;
#define PRINTF_COLOR "%d"
const color_t Red   = 0;
const color_t Black = 1;

typedef int data_t;
#define PRINTF_DATA "%d"

struct node_t 
{
  node_t* parent_;
  color_t color_;
  data_t  data_;
  node_t* left_;
  node_t* right_;
};

struct tree_t
{
  node_t* root_;
};


typedef int error_t;
enum Procedures
{
  NO_ERROR = 0,
  ERROR_DETECTED,
  ERROR_PTR_IS_NULL,
  ERROR_CALLOC,
  ERROR_COLOR,
  ERROR_CLOSE,
  ERROR_SYSTEM,
  ERROR_LOGIC
};

#define ERROR(TYPE_ERROR, MSG, ...)                           \
  do {                                                        \
    PRINT(RED, BOLD("Critical error\n"));                     \
    PRINT(MAGENTA, BOLD(MSG), ##__VA_ARGS__)                  \
    PRINT(MAGENTA, BOLD("In the %s:"), __PRETTY_FUNCTION__);  \
    PRINT(MAGENTA, BOLD("%d in "), __LINE__);                 \
    PRINT(MAGENTA, BOLD("%s\n"), __FILE__);                   \
    return TYPE_ERROR;                                        \
  } while (0);
         
#define WARNING(MSG, ...)                                    \
  do {                                                       \
    PRINT(RED, N("Warning\n"));                              \
    PRINT(MAGENTA, BOLD(MSG), ##__VA_ARGS__)                 \
    PRINT(MAGENTA, BOLD("In the %s:"), __PRETTY_FUNCTION__); \
    PRINT(MAGENTA, BOLD("%d in "), __LINE__);                \
    PRINT(MAGENTA, BOLD("%s\n"), __FILE__);                  \
  } while(0);

error_t PrintNodes(node_t* node, FILE* file); 
error_t PrintTree(node_t* root);

error_t FillNode(node_t* new_node, node_t* parent, color_t color, data_t data, node_t* left, node_t* right);
error_t NewNode(node_t** new_node, node_t* parent, color_t color, data_t data, node_t* left, node_t* right);

node_t* Grandparent(node_t* node);
node_t* Uncle(node_t* node);
node_t* Sibling(node_t* node);

error_t RotateLeft(node_t* node);
error_t RotateRight(node_t* node);

error_t FindPlaceForNode(tree_t* tree, data_t data, node_t** new_node, node_t** new_node_parent);
error_t Insert(tree_t* tree, data_t data);
error_t InsertCase1(tree_t* tree, node_t* node);
error_t InsertCase2(tree_t* tree, node_t* node);
error_t InsertCase3(tree_t* tree, node_t* node);
error_t InsertCase4(tree_t* tree, node_t* node);

error_t ConstructTree(tree_t** tree);

error_t ReplaceNode(node_t* node, node_t* child);
error_t DeleteChild(node_t* node);

void DeleteCase1(node_t* node);
void DeleteCase2(node_t* node);
void DeleteCase3(node_t* node);
void DeleteCase4(node_t* node);
void DeleteCase5(node_t* node);
void DeleteCase6(node_t* node);
