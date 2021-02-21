#pragma once

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
//! Finds node with value = data and returns is
//!
//! [in] tree - tree, where to find
//! [in] data - value to find
//!
//! @return pointer to node with value = data,
//!         NULL, if this value does not exist in
//!         tree
////////////////////////////////////////////////
error_t PrintTree(node_t* root);

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
error_t Insert(tree_t* tree, data_t data);

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
error_t ConstructTree(tree_t** tree);

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
error_t Delete(node_t* node);


