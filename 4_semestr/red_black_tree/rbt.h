#ifndef RBT_H
#define RBT_H 1

#include "rbt_inner.h"

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
node_t* Find(tree_t* tree, data_t data);

////////////////////////////////////////////////
//!
//! Draw tree in new window
//!
//! [in] root - root of tree
//!
//! @return NO_ERROR, if all is alright; Some 
//!         error, if it occured
////////////////////////////////////////////////
error_t PrintTree(node_t* root);

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
error_t Insert(tree_t* tree, data_t data);

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
error_t Delete(node_t* node);

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
error_t ConstructTree(tree_t** tree);

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
error_t DestructTree(tree_t* tree);

////////////////////////////////////////////////
//!
//! For each node in tree DO job. The order is:
//!   left child, parent, right child (inorder)
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
error_t Foreach(node_t* root, int(*job)(node_t* node, void* extra), void* extra);

#endif
