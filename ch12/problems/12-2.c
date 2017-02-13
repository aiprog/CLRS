/*
 *	Radix trees.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct _node 
{
  int num;			/* Strings number. */
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} node;

char buf[20];
int top = -1;

void insert (node *root, char *string)
{
  int i;
  node *parent, *tmp;

  i = 0;
  while (root != NULL && string[i] != '\0')	/* String prefix stored. */
  {
    parent = root;
    if (string[i] == '0')
      root = root->left;
    else
      root = root->right;

    ++i;
  }

  if (root == NULL)		/* Store the remainder. */
  {
    --i;			/* string[i] isn't stored. */
    while (string[i] != '\0')
    {
      tmp = malloc (sizeof (node));
      tmp->num = 0;
      tmp->parent = parent;
      tmp->left = NULL;
      tmp->right = NULL;

      if (string[i] == '0')
	parent->left = tmp;
      else
	parent->right = tmp;
      parent = tmp;

      ++i;
    }

    root = parent;
  }

  ++root->num;	/* Increase the corresponding number. */
}

void print (node *root)
{
  int i;

  if (root == NULL)
    return;

  if (root->parent != NULL)
  {
    ++top;
    if (root == root->parent->left)
      buf[top] = '0';
    else
      buf[top] = '1';

    buf[++top] = '\0';
    for (i = 0; i < root->num; ++i)
      printf ("%s\n", buf);
    --top;
  }

  print (root->left);
  print (root->right);

  if (top >= 0)		/* Not root node. */
    --top;
}

void radix_trees_sort (char **bit_strings, int size)
{
  node *root;
  int i;

  root = malloc (sizeof (node));
  root->num = 0;
  root->parent = root->left = root->right = NULL;

  for (i = 0; i < size; ++i)
    insert (root, bit_strings[i]);

  print (root);
}

int main ()
{
  char *bit_strings[8] = { "0", "10", "011", "011", "100", "0", "011", "1011" };

  radix_trees_sort (bit_strings, 8);

  return 0;
}
