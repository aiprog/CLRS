/*
 * 	An implemetation of binary search tree.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
  int key;
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} node;

void print (node *root)
{
  if (root != NULL)
  {
    print (root->left);
    printf ("%d ", root->key);
    print (root->right);
  }
}

node *search (node *root, int key)
{
  while (root != NULL && key != root->key)
  {
    if (key < root->key)
      root = root->left;
    else
      root = root->right;
  }

  return root;
}

node *minimum (node *root)
{
  if (root != NULL)
  {
    while (root->left != NULL)
      root = root->left;
  }

  return root;
}

node *maximum (node *root)
{
  if (root != NULL)
  {
    while (root->right != NULL)
      root = root->rigth;
  }

  return root;
}

node *successor (node *cur)
{
  if (cur != NULL)
  {
    if (cur->right != NULL)
      return minimum (cur->right);

    while (cur->parent != NULL && cur->parent->right == cur)
      cur = cur->parent;
    cur = cur->parent;
  }

  return cur;
}

node *predecessor (node *cur)
{
  if (cur != NULL)
  {
    if (cur->left != NULL)
      return maximum (cur->left);

    while (cur->parent != NULL && cur->parent->left == cur)
      cur = cur->parent;
    cur = cur->parent;
  }

  return cur;
}

void insert (node **root, int key)
{
  node *tmp, *parent;

  tmp = malloc (sizeof (node));
  tmp->left = NULL;
  tmp->right = NULL;

  parent = NULL;
  while (*root != NULL)
  {
    parent = *root;
    if (key < *root->key || (key == *root->key && rand () % 2 == 0))
      *root = *root->left;
    else
      *root = *root->right;
  }

  if (parent == NULL)	/* Empty tree. */
    *root = tmp;
  else if (key < parent->key || (key == parent->key && rand () % 2 == 0))
    parent->left = tmp;
  else
    parent->right = tmp;
  tmp->parent = parent;
}

void transplant (node **root, node *dest, node *src)
{
  if (dest->parent == NULL)
    *root = src;
  else if (dest == dest->parent->left)
    dest->parent->left = src;
  else
    dest->parent->right = src;

  if (src != NULL)
    src->parent = dest->parent;
}

void delete (node **root, node *del)
{
  node *tmp;

  if (del->left == NULL)
    transplant (root, del, del->right);
  else if (del->right == NULL)
    transplant (root, del, del->left);
  else
  {
    tmp = minimum (del->right);
    if (tmp->parent != del)
    {
      transplant (root, tmp, tmp->right);
      del->right->parent = tmp;
      tmp->right = del->right;
    }
    transplant (root, del, tmp);
    del->left->parent = tmp;
    tmp->left = del->left;
  }
}
