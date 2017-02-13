/*
 * 	An implementation of the persistent binary search tree.
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
  int key;
  struct _node *left;
  struct _node *right;
} node;

typedef struct _tree
{
  node *past[50];	/* Past versions. */
  int num;		/* The number of past versions. */
  node *root;
} tree;

tree *new_tree ()
{
  tree *tre;

  tre = malloc (sizeof (tree));
  tre->num = -1;
  tre->root = NULL;

  return tre;
}

node *new_node (int key, node *left, node *right)
{
  node *nod;

  nod = malloc (sizeof (node));
  nod->key = key;
  nod->left = left;
  nod->right = right;

  return nod;
}

void free_node (node *nod)
{
  free (nod);
}

void transplant (tree *tre, node *parent, node *dest, node *src)
{
  if (parent == NULL)
    tre->root = src;
  else
  {
    if (dest == parent->left)
      parent->left = src;
    else
      parent->right = src;
  }
}

void insert (tree *tre, node *nod)
{
  node *ite, *new_root, *new_ite, *tmp;

  if (tre->root == NULL)	/* Empty tree. */
  {
    tre->past[++tre->num] = NULL;
    tre->root = nod;
    return;
  }

  new_root = new_node (tre->root->key, NULL, NULL);
  new_ite = new_root;
  ite = tre->root;
  while (ite != NULL)
  {
    if (nod->key <= ite->key)
    {
      if (ite->left != NULL)	/* Copy a node to the new tree*/
      {
	tmp = new_node (ite->left->key, NULL, NULL);
	new_ite->left = tmp;
      }
      else			/* Insert 'nod' to the new tree. */
	new_ite->left = nod;

      new_ite->right = ite->right;
      new_ite = new_ite->left;
      ite = ite->left;
    }
    else
    {
      if (ite->right != NULL)
      {
	tmp = new_node (ite->right->key, NULL, NULL);
	new_ite->right = tmp;
      }
      else
	new_ite->right = nod;

      new_ite->left = ite->left;
      new_ite = new_ite->right;
      ite = ite->right;
    }
  }

  /* Update related data. */
  tre->past[++tre->num] = tre->root;
  tre->root = new_root;
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

void delete (tree *tre, node *nod)
{
  node *ite, *new_root, *new_ite, *new_parent, *tmp;
  node *nod_parent, *nod_copy;

  new_root = new_node (tre->root->key, 0, 0);
  new_ite = new_root;
  new_parent = NULL;
  ite = tre->root;

  /* Search 'nod' and copy needed nodes to the new tree. */
  while (ite->key != nod->key)
  {
    if (nod->key < ite->key)
    {
      tmp = new_node (ite->left->key, NULL, NULL);
      new_ite->left = tmp;
      new_ite->right = ite->right;
      new_parent = new_ite;
      new_ite = tmp;

      ite = ite->left;
    }
    else
    {
      tmp = new_node (ite->right->key, NULL, NULL);
      new_ite->right = tmp;
      new_ite->left = ite->left;
      new_parent = new_ite;
      new_ite = tmp;

      ite = ite->right;
    }
  }

  /* Update related data. */
  tre->past[++tre->num] = tre->root;
  tre->root = new_root;

  if (ite->left == NULL)	/* The left child of 'nod' is NULL. */
  {
    new_ite->right = ite->right;
    transplant (tre, new_parent, new_ite, new_ite->right);
    free_node (new_ite);
  }
  else if (ite->right == NULL)	/* The right child of 'nod' is NULL. */
  {
    new_ite->left = ite->left;
    transplant (tre, new_parent, new_ite, new_ite->left);
    free_node (new_ite);
  }
  else				/* 'nod' has two children. */
  {
    /* Save for future use. */
    nod_parent = new_parent;
    nod_copy = new_ite;

    /* Set the left child of 'nod'. */
    new_ite->left = ite->left;

    /* Copy the right child of 'nod' to the new tree. */
    tmp = new_node (ite->right->key, NULL, ite->right->right);
    new_ite->right = tmp;
    new_parent = new_ite;
    new_ite = tmp;

    /* Copy nodes between 'nod->right->left' and 'successor (nod)'
     * to the new tree. */
    ite = ite->right;
    while (ite->left != NULL)
    {
      tmp = new_node (ite->left->key, NULL, ite->left->right);
      new_ite->left = tmp;
      new_parent = new_ite;
      new_ite = new_ite->left;

      ite = ite->left;
    }

    /* To delete 'nod'. */
    if (nod_copy->right != new_ite)
    {
      transplant (tre, new_parent, new_ite, new_ite->right);
      new_ite->right = nod_copy->right;
    }
    transplant (tre, nod_parent, nod_copy, new_ite);
    new_ite->left = nod_copy->left;
    free_node (nod_copy);
  }
}

void sub_print (node *root)
{
  if (root == NULL)
    return;

  sub_print (root->left);
  printf ("%d ", root->key);
  sub_print (root->right);
}

void print (tree *tre)
{
  int i;

  for (i = 0; i <= tre->num; ++i)
  {
    printf ("*************%d*******************\n", i);
    if (tre->past[i] != NULL)
      sub_print (tre->past[i]);
    else
      printf ("Empty");
    printf ("\n");
  }
  printf ("**********************************\n");
  if (tre->root != NULL)
    sub_print (tre->root);
  else
    printf ("Empty");
  printf ("\n");
}

int main ()
{
  tree *tre;
  node *tmp;
  char option[10];
  int key, quit;

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
	tmp = new_node (key, 0, 0);
	insert (tre, tmp);
	print (tre);

	break;
      case 'd':
	printf ("Key: ");
	scanf ("%d", &key);
	if ((tmp = search (tre, key)) != NULL)
	{
	  delete (tre, search (tre, key));
	  print (tre);
	}
	else
	  printf ("Not found!\n");

	break;
      case 'q':
	quit = 1;

	break;
      default:
	printf ("Unrecognized option \'%c\'\n", option[0]);
    }
  } while (quit == 0);

  return 0;
}
