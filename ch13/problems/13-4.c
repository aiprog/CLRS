/*
 * 	An implementation of the treaps.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#define PRI(a)	((a) == NULL ? INT_MAX : (a)->pri)

typedef struct _node
{
  int key;
  int pri;
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} node;

typedef struct _tree
{
  node *root;
} tree;

node *new_node (int key)
{
  node *nod;

  nod = malloc (sizeof (node));
  nod->key = key;
  nod->pri = rand () % INT_MAX;
  nod->parent = nod->left = nod->right = NULL;

  return nod;
}

void free_node (node *nod)
{
  free (nod);
}

tree *new_tree ()
{
  tree *tre;

  srand (time (NULL));
  tre = malloc (sizeof (tree));
  tre->root = NULL;

  return tre;
}

void sub_free_tree (node *nod)
{
  if (nod == NULL)
    return;

  sub_free_tree (nod->left);
  sub_free_tree(nod->right);
  free_node (nod);
}

void free_tree (tree *tre)
{
  if (tre != NULL)
    sub_free_tree (tre->root);
  free (tre);
}

void sub_verify (node *root)
{
  if (root == NULL)
    return;

  if (root->left != NULL)
  {
    sub_verify (root->left);
    assert (root->left->pri >= root->pri);
  }
  if (root->right != NULL)
  {
    sub_verify (root->right);
    assert (root->right->pri >= root->pri);
  }
}

void verify (tree *tre)
{
  sub_verify (tre->root);
}

void left_rotate (tree *tre, node *nod)
{
  node *tmp = nod->right;
  tmp->parent = nod->parent;
  if (nod->parent == NULL)
    tre->root = tmp;
  else if (nod == nod->parent->left)
    nod->parent->left = tmp;
  else
    nod->parent->right = tmp;

  nod->right = tmp->left;
  if (nod->right != NULL)
    nod->right->parent = nod;
  nod->parent = tmp;
  tmp->left = nod;
}

void right_rotate (tree *tre, node *nod)
{
  node *tmp = nod->left;
  tmp->parent = nod->parent;
  if (nod->parent == NULL)
    tre->root = tmp;
  else if (nod == nod->parent->left)
    nod->parent->left = tmp;
  else
    nod->parent->right = tmp;

  nod->left = tmp->right;
  if (nod->left != NULL)
    nod->left->parent = nod;
  nod->parent = tmp;
  tmp->right = nod;
}

void insert (tree *tre, node *nod)
{
  node *ite, *parent;

  ite = tre->root;
  parent = NULL;
  while (ite != NULL)
  {
    parent = ite;
    if (nod->key <= ite->key)
      ite = ite->left;
    else
      ite = ite->right;
  }

  if (parent == NULL)
  {
    tre->root = nod;
    return;
  }
  else if (nod->key <= parent->key)
    parent->left = nod;
  else
    parent->right = nod;
  nod->parent = parent;

  ite = nod;
  while (ite != tre->root && ite->pri < ite->parent->pri)
  {
    if (ite == ite->parent->left)
      right_rotate (tre, ite->parent);
    else
      left_rotate (tre, ite->parent);
  }
}

void transplant (tree *tre, node *dest, node *src)
{
  if (src != NULL)
    src->parent = dest->parent;

  if (dest == tre->root)
    tre->root = src;
  else if (dest == dest->parent->left)
    dest->parent->left = src;
  else
    dest->parent->right = src;
}

void delete (tree *tre, node *nod)
{
  node *ite, *succ;

  if (nod->left == NULL)
  {
    ite = ite->right;
    transplant (tre, nod, nod->right);
  }
  else if (nod->right == NULL)
  {
    ite = ite->left;
    transplant (tre, nod, nod->left);
  }
  else
  {
    succ = nod->right;
    while (succ->left != NULL)
      succ = succ->left;
    ite = succ;

    if (succ != nod->right)
    {
      transplant (tre, succ, succ->right);
      succ->right = nod->right;
      succ->right->parent = succ;
    }
    transplant (tre, nod, succ);
    succ->left = nod->left;
  }

  if (ite == NULL)
    return;

  while (ite->pri > PRI (ite->left) || ite->pri > PRI (ite->right))
  {
    if (PRI (ite->left) < PRI (ite->right))
      right_rotate (tre, ite);
    else
      left_rotate (tre, ite);
  }
}

void sub_print (node *nod)
{
  if (nod == NULL)
    return;

  sub_print (nod->left);
  printf ("%d ", nod->key);
  sub_print (nod->right);
}

void print (tree *tre)
{
  if (tre->root == NULL)
    printf ("Empty");
  else
    sub_print (tre->root);
  printf ("\n");
}

node *search (tree *tre, int key)
{
  node *ite;

  ite = tre->root;
  while (ite != NULL && ite->key != key)
  {
    if (key < ite->key)
      ite = ite->left;
    else
      ite = ite->right;
  }

  return ite;
}

int main ()
{
  tree *tre;
  node *nod;
  int key;
  char option[10];
  int quit;

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
	nod = new_node (key);
	insert (tre, nod);
	verify (tre);
	print (tre);

	break;
      case 'd':
	printf ("Key: ");
	scanf ("%d", &key);
	if ((nod = search (tre, key)) != NULL)
	{
	  delete (tre, nod);
	  verify (tre);
	  print (tre);
	}
	else
	  printf ("Not found\n");

	break;
      case 'q':
	quit = 1;

	break;
      default:
	printf ("Unrecognized \'%c\'\n", option[0]);
    }
  } while (quit == 0);

  free_tree (tre);
  return 0;
}
