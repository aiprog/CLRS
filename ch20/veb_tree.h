/*
 *  veb_tree.h
 *
 *  A simple implementation of van Emde Boas Tree.
 */

#ifndef   VEB_TREE_H
#define   VEB_TREE_H

typedef struct _veb_tree_node
{
  int expo;
  int min;
  int max;
  struct _veb_tree_node **cluster;
  struct _veb_tree_node *summary;
} veb_tree_node;

/* Return a new tree. */
veb_tree_node *new_veb_tree (int expo);

/* Minimum. */
int veb_tree_minimum (veb_tree_node *node);

/* Maximum. */
int veb_tree_maximum (veb_tree_node *node);

/* Test whether a key is in the tree. */
int veb_tree_member (veb_tree_node *node, int key);

/* Successor. */
int veb_tree_successor (veb_tree_node *node, int key);

/* Predecessor. */
int veb_tree_predecessor (veb_tree_node *node, int key);

/* Insert. */
void veb_tree_insert (veb_tree_node *node, int key);

/* Delete. */
void veb_tree_delete (veb_tree_node *node, int key);

/* Free a tree. */
void free_veb_tree (veb_tree_node *node);

#endif