#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
  int key;
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} node;

typedef struct _tree
{
  node *root;
} tree;

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
}

void print (node *nod)
{
  if (nod == NULL)
    return;

  print (nod->left);
  printf ("%d ", nod->key);
  print (nod->right);
}

void enumerate (tree *tre, int beg, int end)
{
  node *ite, *pivot;

  ite = tre->root;
  while (1)
  {
    if (ite->key < beg)
      ite = ite->right;
    else if (ite->key > end)
      ite = ite->left;
    else
      break;
  }
  pivot = ite;

  ite = pivot->left;
  while (ite != NULL)
  {
    if (beg > ite->key)
      ite = ite->right;
    else if (beg < ite->key)
    {
      printf ("%d ", ite->key);
      print (ite->right);
      ite = ite->left;
    }
    else
    {
      printf ("%d ", beg);
      print (ite->right);
      break;
    }
  }

  printf ("%d ", pivot->key);

  iite = pivot->right;
  while (ite != NULL)
  {
    if (end > ite->key)
    {
      printf ("%d ", ite->key);
      print (ite->left);
      ite = ite->right;
    }
    else if (end < ite->key)
      ite = ite->left;
    else
    {
      printf ("%d ", end);
      print (ite->left);
      break;
    }
  }
  printf ("\n");
}

int main ()
{
  tree *tre;
  node *nod;
  int arr[15] = { 3, 6, 5, 2, 9, 7, 8, 15, 10, 13, 1, 14, 12, 4, 11 };
  int i;

  tre = malloc (sizeof (tre));
  tre->root = NULL;
  for (i = 0; i < 15; ++i)
  {
    nod = malloc (sizeof (node));
    nod->key = arr[i];
    nod->parent = nod->left = nod->right = NULL;
    insert (tre, nod);
  }

  enumerate (tre, 4, 10);
  enumerate (tre, 1, 6);
  enumerate (tre, 1, 15);
  enumerate (tre, 5, 5);

  return 0;
}
