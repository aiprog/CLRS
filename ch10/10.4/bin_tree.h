#ifndef	BIN_TREE_H
#define	BIN_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define	SIZE	40	/* Size of the tree. */
#define	STACK	100	/* Stack size. */

typedef struct _node
{
  int key;
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} node;

node *root;
int created = 1;	/* Nodes created. */
int printed = 1;	/* Nodes printed. */
int viable = 1;		/* Viable nodes to create. */

/* It will return a randomized binary tree.
 * Every node has the posibility to have none,
 * one or two children. */
node *create_tree_sub ()
{
  node *nd;

  if (created > SIZE)
    return NULL;

  /* We can return a NULL node only when the viable nodes are
   * more than one, or we may cann't create SIZE nodes. */
  if (viable > 1 && rand () % 3 == 0)
  {
    --viable;	/* Decrease viable nodes when we return a NULL nodes. */
    return NULL;
  }

  ++viable;	/* Otherwise, increase it. */
  nd = malloc (sizeof (node));
  nd->key = created++;
  nd->left = create_tree_sub ();
  if (nd->left)
    nd->left->parent = nd;
  nd->right = create_tree_sub ();
  if (nd->right)
    nd->right->parent = nd;

  return nd;
}

/* A wrapper of create_tree_sub (). */ 
node *create_tree ()
{
  node *root;

  srand (time (NULL));
  root = create_tree_sub ();
  root->parent = NULL;	/* The function of this wrapper. */

  return root;
}

/* Print preorderly. */
void print_tree_pre (node *root)
{
  if (root == NULL)
    return;

  printf ("%-5d", root->key);
  if (printed++ % 10 == 0)	/* Format the output. */
    printf ("\n");
  print_tree_pre (root->left);
  print_tree_pre (root->right);
}

/* Nonrecursive version of print_tree_pre (). */
void print_tree_pre_nonrec (node *root)
{
  node *st[STACK];	/* Stack. */
  int top;		/* Stack top. */
  node *cur;

  st[0] = root;
  top = 0;
  while (top >= 0)
  {
    cur = st[top--];	/* Pop. */
    while (cur)		/* Traverse along the left until it is NULL. */
    {
      printf ("%-5d ", cur->key);
      if (printed++ % 10 == 0)	/* Format the output. */
	printf ("\n");

      if (cur->right)
      {
	if (top == STACK - 1)
	{
	  printf ("Stack overflow!\n");
	  exit (1);
	}
	st[++top] = cur->right;	/* Push, save the right child. */
      }

      cur = cur->left;
    }
  }
}

/* A O(n)-time nonrecursive version of print_tree_pre ()
 * without stack space. */
void print_tree_pre_nonstk (node *root)
{
  node *cur;

  cur = root;
  while (1)
  {
    /* Traverse along the left until its left child is NULL. */
    do 
    {
      printf ("%-5d ", cur->key);
      if (printed++ % 10 == 0)	/* Format the output. */
        printf ("\n");
    } while (cur->left? cur = cur->left: NULL);

    /* Traverse its right child the same way if exists. */
    if (cur->right)
    {
      cur = cur->right;
      continue;
    }

    /* Walk up the tree until the root or a node with a unvisited
     * right child is found. */
    while (cur != root && 
	   (cur == cur->parent->right || !cur->parent->right))
      cur = cur->parent;

    if (cur == root)
      break;

    /* Traverse the right child the same way. */
    cur = cur->parent->right;
  }
}

void free_tree (node *root)
{
  if (root == NULL)
    return;

  free_tree (root->left);
  free_tree (root->right);
  free (root);	/* Must be the last. */
}

#endif
