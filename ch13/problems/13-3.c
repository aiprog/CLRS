/*
 * 	A implementation of the AVL tree.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX(a, b)	((a) >= (b) ? (a): (b))
#define ABS(a)          ((a) >= 0 ? (a): -(a))

typedef struct _node
{
  int key;
  int height;
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} node;

typedef struct _tree
{
  node *root;
  node *nil;		/* Sentinel. */
} tree;

tree *new_tree ()
{
  tree *tre;

  tre = malloc (sizeof (tree));
  tre->nil = malloc (sizeof (tree));
  tre->root = tre->nil;
  tre->nil->height = 0;

  return tre;
}

void free_node (node *nod)
{
  free (nod);
}

void sub_free_tree (node *root, node *nil)
{
  if (root == nil)
    return;

  sub_free_tree (root->right, nil);
  sub_free_tree (root->left, nil);
  free_node (root);
}

void free_tree (tree *tre)
{
  sub_free_tree (tre->root, tre->nil);
  free_node (tre->nil);
  free (tre);
}

node *new_node (tree *tre, int key)
{
  node *nod;

  nod = malloc (sizeof (node));
  nod->key = key;
  nod->height = 1;
  nod->parent = nod->left = nod->right = tre->nil;

  return nod;
}

void left_rotate (tree *tre, node *nod)
{
  node *tmp;

  tmp = nod->right;
  tmp->parent = nod->parent;
  if (nod == tre->root)
    tre->root = tmp;
  else if (nod == nod->parent->left)
    nod->parent->left = tmp;
  else
    nod->parent->right = tmp;

  nod->right = tmp->left;
  if (nod->right != tre->nil)
    nod->right->parent = nod;
  tmp->left = nod;
  nod->parent = tmp; 

  /* Update height fields. */
  nod->height = MAX (nod->left->height, nod->right->height) + 1;
  tmp->height = MAX (tmp->left->height, tmp->right->height) + 1;
}

void right_rotate (tree *tre, node *nod)
{
  node *tmp;

  tmp = nod->left;
  tmp->parent = nod->parent;
  if (nod == tre->root)
    tre->root = tmp;
  else if (nod == nod->parent->left)
    nod->parent->left = tmp;
  else
    nod->parent->right = tmp;

  nod->left = tmp->right;
  if (nod->left != tre->nil)
    nod->left->parent = nod;
  tmp->right = nod;
  nod->parent = tmp; 

  /* Update height fields. */
  nod->height = MAX (nod->left->height, nod->right->height) + 1;
  tmp->height = MAX (tmp->left->height, tmp->right->height) + 1;
}

void sub_print (node *root, node *nil)
{
  if (root == nil)
    return;

  sub_print (root->left, nil);
  printf ("%d ", root->key);
  sub_print (root->right, nil);
}

void print (tree *tre)
{
  if (tre->root == tre->nil)
    printf ("Empty");
  else
    sub_print (tre->root, tre->nil);
  printf ("\n");
}

void sub_verify (node *root, node *nil)
{
  if (root == nil)
    return;

  sub_verify (root->left, nil);
  sub_verify (root->right, nil);
  assert (ABS (root->left->height - root->right->height) <= 1);
  assert (root->height == MAX (root->left->height, root->right->height) + 1);
}

/* Verify AVL tree properties. */
void verify (tree *tre)
{
  if (tre->root != tre->nil)
    sub_verify (tre->root, tre->nil);
}

node *search (tree *tre, int key)
{
  node *ite;

  ite = tre->root;
  while (ite != tre->nil && ite->key != key)
  {
    if (key < ite->key)
      ite = ite->left;
    else
      ite = ite->right;
  }

  return ite;
}

void insert (tree *tre, node *nod)
{
  node *ite, *parent, *sibling;

  ite = tre->root;
  parent = tre->nil;
  /* Find the position to insert. */
  while (ite != tre->nil)
  {
    parent = ite;
    if (nod->key <= ite->key)
      ite = ite->left;
    else
      ite = ite->right;
  }

  if (parent == tre->nil)	/* Empty tree. */
  {
    tre->root = nod;
    return;
  }

  /* Insert the node to the tree. */
  if (nod->key <= parent->key)
  {
    parent->left = nod;
    sibling = parent->right;
  }
  else
  {
    parent->right = nod;
    sibling = parent->left;
  }
  nod->parent = parent;
  ite = nod;

  /* Walk up the tree. */
  while (ite != tre->root && ite->height - sibling->height == 1)
  {
    ++parent->height;
    ite = parent;
    parent = parent->parent;

    if (ite == parent->left)
      sibling = parent->right;
    else
      sibling = parent->left;
  }

  /* Do rotations to balance the tree. */
  if (ite != tre->root && ite->height - sibling->height == 2)
  {
    if (ite == parent->left)
    {
      if (ite->right->height - ite->left->height == 1)
	left_rotate (tre, ite);
      right_rotate (tre, parent);
    }
    else
    {
      if (ite->left->height - ite->right->height == 1)
	right_rotate (tre, ite);
      left_rotate (tre, parent);
    }
  }
}

void transplant (tree *tre, node *dest, node *src)
{
  src->parent = dest->parent;
  if (dest == tre->root)
    tre->root = src;
  else if (dest == dest->parent->left)
    dest->parent->left = src;
  else
    dest->parent->right = src;
}

/*  'ite' points to the node whose height was decreased by one 
 *  because of the deletion. And 'sibling' is the sibling of 'ite'. */
void delete (tree *tre, node *nod)
{
  node *ite, *succ, *sibling;
  int lsub, rsub;

  if (nod->left == tre->nil)		
  {
    ite = nod->right;
    transplant (tre, nod, ite);
  }
  else if (nod->right == tre->nil)
  {
    ite = nod->left;
    transplant (tre, nod, ite);
  }
  else
  {
    /* Find the successor of 'nod'. */
    succ = nod->right;
    while (succ->left != tre->nil)
      succ = succ->left;

    succ->height = nod->height;
    ite = succ->right;
    ite->parent = succ;

    /* When the successor is the right child of 'nod'. */
    if (succ != nod->right)
    {
      transplant (tre, succ, succ->right);
      succ->right = nod->right;
      succ->right->parent = succ;
    }
    transplant (tre, nod, succ);
    succ->left = nod->left;
    succ->left->parent = succ;
  }

  /* When the tree is empty or 'ite' is the root. */
  if ((ite == tre->nil && ite->parent == tre->nil) || ite == tre->root)
    return;

  while (ite != tre->root)
  {
    /* Calculate the sibling of 'ite'. */
    if (ite == ite->parent->left)
      sibling = ite->parent->right;
    else
      sibling = ite->parent->left;
   
    /* Walk up the tree when the heights of 'ite' and 'sibling' are equal. */
    if (sibling->height - ite->height == 0)
    {
      --ite->parent->height;
      ite = ite->parent;
    }
    else if (sibling->height - ite->height == 2)
    {
      lsub = sibling->height - sibling->left->height;
      rsub = sibling->height - sibling->right->height;

      if (sibling == sibling->parent->left)
      {
	if (lsub == 1 && rsub == 1)
	{
	  right_rotate (tre, sibling->parent);
	  return;
	}
	else if (lsub == 1 && rsub == 2)
	{
	  right_rotate (tre, sibling->parent);
	  ite = sibling;
	}
	else
	{
	  left_rotate (tre, sibling);
	  right_rotate (tre, sibling->parent->parent);
	  ite = sibling->parent;
	}
      }
      else	/* It is symmetric with IF clause. */
      {
	if (rsub == 1 && lsub == 1)
	{
	  left_rotate (tre, sibling->parent);
	  return;
	}
	else if (rsub == 1 && lsub == 2)
	{
	  left_rotate (tre, sibling->parent);
	  ite = sibling;
	}
	else
	{
	  right_rotate (tre, sibling);
	  left_rotate (tre, sibling->parent->parent);
	  ite = sibling->parent;
	}
      }
    }
    else
      break;
  }
}

int main ()
{
  tree *tre;
  node *tmp;
  char option[10];
  int quit, key;

  tre = new_tree ();
  quit = 0;
  do
  {
    printf ("'i' for insert, 'd' for delete and 'q' for quit: ");
    scanf ("%s", option);
    switch (option[0])
    {
      case 'i':
	printf ("Key: ");
	scanf ("%d", &key);
	tmp = new_node (tre, key);
	insert (tre, tmp);
	verify (tre);
	print (tre);

	break;
      case 'd':
	printf ("Key: ");
	scanf ("%d", &key);
	tmp = search (tre, key);
	if (tmp == tre->nil)
	  printf ("Not found!\n");
	else
	{
	  delete (tre, tmp);
	  verify (tre);
	  print (tre);
	}

	break;
      case 'q':
	quit = 1;

	break;
      default:
	printf ("Unrecognized \'%c\'\n", option[0]);
    }
  } while (quit == 0);

  return 0;
}
