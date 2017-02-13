/*
 * 	An implementation of the red-black tree.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
  int key;
  int color;
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} node;

typedef struct _rb_tree
{
  node *root;
  node *nil;
} rb_tree;

rb_tree *new_rb_tree ()
{
  rb_tree *tree;
 
  tree = malloc (sizeof (rb_tree));
  tree->nil = malloc (sizeof (node));
  tree->nil->color = 1;
  tree->root = tree->nil;

  return tree;
}

void left_rotate (rb_tree *tree, node *x)
{
  node *y;

  y = x->right;
  y->parent = x->parent;
  if (x->parent == tree->root)
    tree->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;

  x->right = y->left;
  if (x->right != tree->nil)
    x->right->parent = x;

  y->left = x;
  x->parent = y;
}

void right_rotate (rb_tree *tree, node *y)
{
  node *x;

  x = y->left;
  x->parent = y->parent;
  if (y->parent == tree->nil)
    tree->root = x;
  else if (y == y->parent->left)
    y->parent->left = x;
  else
    y->parent->right = x;

  y->left = x->right;
  if (y->left != tree->nil)
    y->left->parent = y;

  x->right = y;
  y->parent = x;
}

void insert_fixup (rb_tree *tree, node *z)
{
  node *grandp;

  while (z->parent->color == 0)
  {
    grandp = z->parent->parent;
    if (z->parent == grandp->left)
    {
      if (grandp->right->color == 0)
      {
	z->parent->color = 1;
	grandp->color = 0;
	grandp->right->color = 1;

	z = grandp;
      }
      else
      {
	if (z == z->parent->right)
	{
	  z = z->parent;
	  left_rotate (tree, z->parent);
	}

	z->parent->color = 1;
	grandp->color = 0;
	left_rotate (tree, grandp);
      }
    }
    else
    {
      if (grandp->left->color == 0)
      {
	z->parent->color = 1;
	grandp->color = 0;
	grandp->left->color = 1;

	z = grandp;
      }
      else
      {
	if (z == z->parent->left)
	{
	  z = z->parent;
	  right_rotate (tree, z->parent);
	}

	z->parent->color = 1;
	grandp->color = 0;
	right_rotate (tree, grandp);
      }
    }
  }

  tree->root->color = 1;
}

void insert (rb_tree *tree, int key)
{
  node *x, *parent, *ptr;

  x = malloc (sizeof (node));
  x->key = key;
  x->color = 0;
  x->left = tree->nil;
  x->right = tree->nil;

  parent = tree->nil;
  ptr = tree->root;
  while (ptr != tree->nil)
  {
    parent = ptr;
    if (key <= ptr->key)
      ptr = ptr->left;
    else
      ptr = ptr->right;
  }

  if (parent == tree->nil)
    tree->root = x;
  else if (key <= parent->key)
    parent->left = x;
  else
    parent->right = x;
  x->parent = parent;

  insert_fixup (tree, x);
}

void transplant (rb_tree *tree, node *u, node *v)
{
  if (u == tree->root)
    tree->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;

  v->parent = u->parent;
}

void delete (rb_tree *tree, node *z)
{
  node *y, *x;
  int color;

  y = z;
  color = y->color;
  if (z->left == tree->nil)
  {
    x = z->right;
    transplant (tree, z, x)
  }
  else if (z->right == tree->nil)
  {
    x = z->left;
    transplant (tree, z, x);
  }
  else
  {
    y = minimum (tree, z->right);
    color = y->color;
    x = y->right;
    if (y->parent == z)
      x->parent = y;
    else
    {
      transplant (tree, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant (tree, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (color == 1)
    delete_fixup (tree, x);
}

void delete_fixup (rb_tree *tree, node *x)
{
  node *sibling;

  while (x != tree->root && x->color == 1)
  {
    if (x == x->parent->left)
    {
      sibling = x->parent->right;
      if (sibling->color == 0)
      {
	x->parent->color = 0;
	sibling->color = 1;
	left_rotate (tree, x->parent);
	sibling = x->parent->right;
      }

      if (sibling->left->color == 1 && sibling->right->color == 1)
      {
	sibling->color = 0;
	x = x->parent;
      }
      else
      {
	if (sibling->right->color == 1)
	{
	  sibling->left->color = 1;
	  sibling->color = 0;
	  right_rotate (tree, sibling);
	  sibling = x->parent->right;
	}
	sibling->right->color = 1;
	sibling->color = x->parent->color;
	x->parent->color = 1;
	left_rotate (tree, x->parent);
	x = tree->root;
      }
    }
    else
    {
      sibling = x->parent->left;
      if (sibling->color == 0)
      {
	x->parent->color = 0;
	sibling->color = 1;
	right_rotate (tree, x->parent);
	sibling = x->parent->left;
      }

      if (sibling->left->color == 1 && sibling->right->color == 1)
      {
	sibling->color = 0;
	x = x->parent;
      }
      else
      {
	if (sibling->left->color == 1)
	{
	  sibling->right->color = 1;
	  sibling->color = 0;
	  left_rotate (tree, sibling);
	}
	sibling->left->color = 1;
	sibling->color = x->parent->color;
	x->parent->color = 1;
	right_rotate (tree, x->parent);
	x = tree->root;
      }
    }
  }
  x->color = 1;
}
