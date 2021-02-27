#ifndef RBT_H
#define RBT_H 1

#include <stdlib.h>
#include <stdio.h>

struct node_t;
typedef struct node_t node_t;

struct tree_t;
typedef struct tree_t tree_t;

typedef unsigned char color_t;
typedef int data_t;

typedef int error_t;

////////////////////////////////////////////////
//!
//! Finds node with value = data and returns is
//!
//! [in] tree - tree, where to find
//! [in] data - value to find
//!
//! @return pointer to node with value = data,
//!         NULL, if this value does not exist in
//!         tree
////////////////////////////////////////////////
node_t* TreeFind(tree_t* tree, data_t data);

////////////////////////////////////////////////
//!
//! Draw tree in new window
//!
//! [in] root - root of tree
//!
//! @return NO_ERROR, if all is alright; Some 
//!         error, if it occured
////////////////////////////////////////////////
error_t TreePrint(node_t* root, const char* temp_file);

////////////////////////////////////////////////
//!
//! Insert node with new value in the tree
//!
//! [in] tree - tree, where to insert
//! [in] data - value to insert
//!
//! @return NO_ERROR, if all is alright; Some 
//!         error, if it occured
////////////////////////////////////////////////
error_t TreeInsert(tree_t* tree, data_t data);

////////////////////////////////////////////////
//!
//! Delete node with value in the tree
//!
//! [in] tree - tree, where node is exist
//! [in] data - value of deleting node
//!
//! @return NO_ERROR, if all is alright; Some 
//!         error, if it occured
////////////////////////////////////////////////
error_t TreeDelete(node_t* node);

////////////////////////////////////////////////
//!
//! Creates new tree
//!
//! [in] tree - where save this tree. Data should
//!             not be allocated
//!
//! @return pointer to node with value = data,
//!         NULL, if this value does not exist in
//!         tree
////////////////////////////////////////////////
error_t TreeConstruct(tree_t** tree);

////////////////////////////////////////////////
//!
//! Deletes tree, frees memory
//!
//! [in] tree - tree to delete
//!
//! @return pointer to node with value = data,
//!         NULL, if this value does not exist in
//!         tree
////////////////////////////////////////////////
error_t TreeDestruct(tree_t* tree);

////////////////////////////////////////////////
//!
//! For each node in tree DO job. The order is:
//!   left child, right child, parent (postorder)
//!
//! [in] node - tree root to bypass
//! [in] job - function, which is worked under
//!            for each node. From the smallest to
//!            the biggest node in tree. Should 
//!            return 0, if all is alright. Else
//!            error is created
//! [in] extra - extra data for job
//!
//! @return pointer to node with value = data,
//!         NULL, if this value does not exist in
//!         tree
////////////////////////////////////////////////
error_t TreeForeach(node_t* root, int(*job)(node_t* node, void* extra), void* extra);

////////////////////////////////////////////////
//!
//! Returns poiner to the root
//!
//! [in] tree - pointer to the tree
//!
//! @return pointer to node with value;
//!         NULL, if tree = NULL
////////////////////////////////////////////////
node_t* TreeGetRoot(tree_t* tree);

////////////////////////////////////////////////
//!
//! Returns data of the node
//!
//! [in] node - pointer to the nnode
//!
//! @return data; 0, if node = NULL
////////////////////////////////////////////////
data_t NodeGetData(node_t* node);

#endif
