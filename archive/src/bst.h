#ifndef _BST_H
#define _BST_H

#include <stdbool.h> // bool

typedef struct bst_node {
  int _key;
  int _count; // to keep track of duplicates
  struct bst_node * _left;
  struct bst_node * _right;
} bst_node_t;

typedef struct {
  bst_node_t * _root;
} bst_t;

// Creates an empty binary search tree
bst_t * bst_new();
// Destroys tree
void bst_destroy(bst_t * tree);
// Inserts value into tree
void bst_insert(bst_t * tree, int val);
// Returns number of items in the tree (includes duplicates)
int bst_get_count(bst_t * tree);
// Returns number of nodes in the tree (this will implicitly exclude duplicates)
int bst_get_node_count(bst_t * tree);
// Returns true if item is in tree
bool bst_is_in_tree(bst_t * tree, int val);
// Returns height of the tree
int bst_get_height(bst_t * tree);
// Returns minimum value in tree
int bst_get_min(bst_t * tree);
// Returns maximum value in tree
int bst_get_max(bst_t * tree);
// Returns an array containing the values in the tree in ascending order -- must
// be freed by user
int * bst_flatten(bst_t * tree);
// Removes one instance of the given value from the tree, if it exists
void bst_remove(bst_t * tree, int val);


#endif // _BST_H
