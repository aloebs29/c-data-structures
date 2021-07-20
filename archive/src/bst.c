#include "bst.h"
#include <stdlib.h> // malloc, free
#include "helpers.h" // error_print_and_quit

// Private function prototypes
static bst_node_t * node_new(int value);
static void node_destroy(bst_node_t * node);
static void tree_destroy_nodes(bst_node_t * root);
static void tree_insert_by_node(bst_node_t * root, int val);
static void tree_count(bst_node_t * root, int * counter);
static void tree_count_nodes(bst_node_t * root, int * counter);
static void tree_search(bst_node_t * root, bool * found, int val);
static int tree_get_height(bst_node_t * root);
static int tree_get_min(bst_node_t * root);
static int tree_get_max(bst_node_t * root);
static int tree_traverse_and_append(bst_node_t * root, int * buffer, 
    int * index);
static void tree_remove(bst_node_t * parent, bst_node_t * current, int val);
static void tree_delete_node(bst_node_t * parent, bst_node_t * to_destroy);
static void tree_destroy_min(bst_node_t * parent, bst_node_t * root, 
    int * val_out, int * count_out);

static int max(int i1, int i2);

// Public functions
bst_t * bst_new()
{
  bst_t * tree = (bst_t *)malloc(sizeof(bst_t));

  tree->_root = NULL;

  return tree;
}

void bst_destroy(bst_t * tree)
{
  if (tree->_root)
  {
    tree_destroy_nodes(tree->_root);
  }
  free(tree);
}

void bst_insert(bst_t * tree, int val)
{
  if (!tree->_root)
  {
    tree->_root = node_new(val);
  }
  else
  {
    tree_insert_by_node(tree->_root, val);
  }
}

int bst_get_count(bst_t * tree)
{
  int counter = 0;
  if (!tree->_root)
  {
    return 0;
  }
  else
  {
    tree_count(tree->_root, &counter);
  }
  
  return counter;
}

int bst_get_node_count(bst_t * tree)
{
  int counter = 0;
  if (!tree->_root)
  {
    return 0;
  }
  else
  {
    tree_count_nodes(tree->_root, &counter);
  }
  
  return counter;
}

bool bst_is_in_tree(bst_t * tree, int val)
{
  bool found = false;
  if (!tree->_root)
  {
    return false;
  }
  else
  {
    tree_search(tree->_root, &found, val);
  }

  return found;
}

int bst_get_height(bst_t * tree)
{
  return tree_get_height(tree->_root);
}

int bst_get_min(bst_t * tree)
{
  if (!tree->_root)
  {
    error_print_and_quit("bst_get_min tree must not be empty.");
  }
  else
  {
    return tree_get_min(tree->_root);
  }
}

int bst_get_max(bst_t * tree)
{
  if (!tree->_root)
  {
    error_print_and_quit("bst_get_max tree must not be empty.");
  }
  else
  {
    return tree_get_max(tree->_root);
  }
}

int * bst_flatten(bst_t * tree)
{
  if (!tree->_root)
  {
    return NULL;
  }
  else
  {
    int size = bst_get_count(tree);
    int * array = (int *)malloc(sizeof(int) * size);
    int * index = (int *)malloc(sizeof(int));
    *index = 0;
    tree_traverse_and_append(tree->_root, array, index);
    return array;
  }
}

void bst_remove(bst_t * tree, int val)
{
  if (!tree->_root)
  {
    return;
  }
  else if (tree->_root->_key == val)
  {
    if (tree->_root->_count > 1)
    {
      tree->_root->_count--;
    }
    else
    {
      if (!tree->_root->_left && !tree->_root->_right)
      {
        node_destroy(tree->_root);
        tree->_root = NULL;
      }
      else
      {
        tree_remove(NULL, tree->_root, val);
      }
    }
  }
  else
  {
    tree_remove(NULL, tree->_root, val);
  }
}

// Private function definitions
static bst_node_t * node_new(int value)
{
  bst_node_t * node = (bst_node_t *)malloc(sizeof(bst_node_t));

  node->_key = value;
  node->_count = 1;
  node->_left = NULL;
  node->_right = NULL;

  return node;
}

static void node_destroy(bst_node_t * node)
{
  free(node);
}

static void tree_destroy_nodes(bst_node_t * root)
{
  if (root->_left)
  {
    tree_destroy_nodes(root->_left);
  }
  if (root->_right)
  {
    tree_destroy_nodes(root->_right);
  }
  node_destroy(root);
}

static void tree_insert_by_node(bst_node_t * root, int val)
{
  if (root->_key == val)
  {
    root->_count++;
  }
  else if (root->_key > val)
  {
    if (root->_left)
    {
      tree_insert_by_node(root->_left, val);
    }
    else
    {
      root->_left = node_new(val);
    }
  }
  else // key < val
  {
    if (root->_right)
    {
      tree_insert_by_node(root->_right, val);
    }
    else
    {
      root->_right = node_new(val);
    }
  }
}

static void tree_count(bst_node_t * root, int * counter)
{
  if (root->_left)
  {
    tree_count_nodes(root->_left, counter);
  }
  if (root->_right)
  {
    tree_count_nodes(root->_right, counter);
  }

  (*counter) += root->_count;
}

static void tree_count_nodes(bst_node_t * root, int * counter)
{
  if (root->_left)
  {
    tree_count_nodes(root->_left, counter);
  }
  if (root->_right)
  {
    tree_count_nodes(root->_right, counter);
  }

  (*counter)++;
}

static void tree_search(bst_node_t * root, bool * found, int val)
{
  if (*found)
  {
    return;
  }

  if (root->_key == val)
  {
    *found = true;
    return;
  }
  else if (root->_key > val)
  {
    if (root->_left)
    {
      tree_search(root->_left, found, val);
    }
  }
  else // key < val
  {
    if (root->_right)
    {
      tree_search(root->_right, found, val);
    }
  }
}

static int tree_get_height(bst_node_t * root)
{
  if (!root)
  {
    return 0;
  }
  else
  {
    return 1 + max(tree_get_height(root->_left), tree_get_height(root->_right));
  }
}

static int tree_get_min(bst_node_t * root)
{
  if (root->_left)
  {
    return tree_get_min(root->_left);
  }
  else
  {
    return root->_key;
  }
}

static int tree_get_max(bst_node_t * root)
{
  if (root->_right)
  {
    return tree_get_max(root->_right);
  }
  else
  {
    return root->_key;
  }
}

static int tree_traverse_and_append(bst_node_t * root, int * buffer, 
    int * index)
{
  if (root->_left)
  {
    tree_traverse_and_append(root->_left, buffer, index);
  }
  int val = root->_key;
  int target_index = *index + root->_count;
  while (*index < target_index)
  {
    buffer[*index] = val;
    (*index)++;
  }
  if (root->_right)
  {
    tree_traverse_and_append(root->_right, buffer, index);
  }
}

static void tree_remove(bst_node_t * parent, bst_node_t * current, int val)
{
  if (current->_key > val)
  {
    if (current->_left)
    {
      tree_remove(current, current->_left, val);
    }
  }
  else if (current->_key < val)
  {
    if (current->_right)
    {
      tree_remove(current, current->_right, val);
    }
  }
  else // key == val
  {
    if (current->_count > 1)
    {
      current->_count--;
    }
    else
    {
      tree_delete_node(parent, current);
    }
  }
}

static void tree_delete_node(bst_node_t * parent, bst_node_t * to_destroy)
{
  // If current is leaf
  if (!to_destroy->_left && !to_destroy->_right)
  {
    if (parent->_left == to_destroy)
    {
      parent->_left = NULL;
    }
    else
    {
      parent->_right = NULL;
    }
    node_destroy(to_destroy);
  }
  // Only child is left
  else if (to_destroy->_left && !to_destroy->_right)
  {
    to_destroy->_key = to_destroy->_left->_key;
    to_destroy->_count = to_destroy->_left->_count;
    node_destroy(to_destroy->_left);
    to_destroy->_left = NULL;
  }
  // Only child is right
  else if (!to_destroy->_left && to_destroy->_right)
  {
    to_destroy->_key = to_destroy->_right->_key;
    to_destroy->_count = to_destroy->_right->_count;
    node_destroy(to_destroy->_right);
    to_destroy->_right = NULL;
  }
  // Has left & right children
  else
  {
    int new_val;
    int new_count;
    // destroy successor
    tree_destroy_min(to_destroy, to_destroy->_right, &new_val, &new_count);
    to_destroy->_key = new_val;
    to_destroy->_count = new_count;
  }
}

static void tree_destroy_min(bst_node_t * parent, bst_node_t * root, 
    int * val_out, int * count_out)
{
  if (!root->_left)
  {
    if (parent->_left == root)
    {
      parent->_left = NULL;
    }
    else
    {
      parent->_right = NULL;
    }
    *val_out = root->_key;
    *count_out = root->_count;
    node_destroy(root);
  }
  else
  {
    tree_destroy_min(root, root->_left, val_out, count_out);
  }
}

static int max(int i1, int i2)
{
  if (i1 > i2)
  {
    return i1;
  }
  else
  {
    return i2;
  }
}