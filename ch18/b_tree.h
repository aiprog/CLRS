/*
 *  b_tree.h
 *
 *  A simple implementation of B-trees.
 */

#ifndef B_TREE_H
#define B_TREE_H

#define DEGREE     3      /* Minimum degree. */
#define INIT_NUM   1000   /* Initial node numbers. */

typedef struct free_node
{
  long offset;
  struct free_node *next;
} Free_node;

typedef struct
{
  int leaf;
  int garbage;
  long offset;
  long parent;
  int num;
  long keys[2 * DEGREE - 1];
  long offsets[2 * DEGREE];
} B_tree_node;

typedef struct
{
  FILE *fp;
  long file_size;
  long offset;
  B_tree_node root;
  Free_node *list;
} B_tree;

/* Initialize B-trees from files. */
B_tree *b_tree_init (char *path);

/* Insert. */
void b_tree_insert (B_tree *tree, long key);

/* Search. */
void b_tree_search (B_tree *tree, long key, B_tree_node **node, int *index);

/* Delete. */
void b_tree_delete (B_tree *tree, long key);

/* Print. */
void print_b_tree (B_tree *tree);

/* Clean up and compact. */
void b_tree_finish (B_tree *tree);

#endif