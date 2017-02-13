#ifndef DY_HUF_TREE_H
#define DY_HUF_TREE_H

#include <stdio.h>
#include <stdlib.h>

#define CHA_SET	128

typedef struct _node
{
  int ch;
  int fre;
  int dep;
  struct _node *left;
  struct _node *right;
  struct _node *parent;
} Node;

Node *table[CHA_SET];
Node *root;

Node *new_node (int ch, int fre, int dep, Node *left, Node *right, Node *parent);

Node *get_sibling (Node *node);

void update_depth (Node *node, int inc);

void exchange_node (Node *node1, Node *node2);

void initialize_tree ();

Node *search_tree (Node *node, Node *parent, int fre);

void adjust_tree (Node *node);

void get_cost (Node *node, int *cost);

void print_tree (Node *node);

#endif
