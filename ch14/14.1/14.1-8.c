#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _node
{
  int key;
  int size;
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} node;

typedef struct _tree
{
  node *root;
} tree;

void transplant (tree *tre, node *dest, node *src)
{
  if (src != NULL)
    src->parent = dest->parent;

  if (dest->parent == NULL)
    tre->root = src;
  else if (dest == dest->parent->left)
    dest->parent->left = src;
  else
    dest->parent->right = src;
}

int delete (tree *tre, node *nod)
{
  node *ite, *succ;
  int sum;	/* The number of nodes with keys greater than 'nod'. */

  sum = 0;
  /* All keys of nodes in the right subtree of 'nod' are greater than 
   * 'nod'. */
  if (nod->right != NULL)
    sum += nod->right->size;

  /* Walk up the tree and update 'sum'. */
  ite = nod;
  while (ite != tre->root)
  {
    --ite->size;
    if (ite == ite->parent->left)
    {
      ++sum;
      if (ite->parent->right != NULL)
	sum += ite->parent->right->size;
    }
    ite = ite->parent;
  }
  --ite->size;

  if (nod->left == NULL)
    transplant (tre, nod, nod->right);
  else if (nod->right == NULL)
    transplant (tre, nod, nod->left);
  else
  {
    succ = nod->right;
    while (succ->left != NULL)
    {
      --succ->size;
      succ = succ->left;
    }

    if (succ != nod->right)
    {
      transplant (tre, succ, succ->right);
      succ->size = nod->size;
      succ->right = nod->right;
      succ->right->parent = succ;
    }
    transplant (tre, nod, succ);
    succ->left = nod->left;
    succ->left->parent = succ;
  }

  return sum;
}

void insert (tree *tre, node **nod, int *arr, int num, int pos, int *sum)
{
  node *ite, *parent, *ins;
										
  /* If 'arr[pos]' is a right endpoint, then we delete the corresponding
   * left endpoint and calculate the intervals that intersect with current
   * interval. */
  if (arr[pos] >= num)
  {
    *sum += delete (tre, nod[arr[pos] - num]);
    return;
  }

  /* Otherwise, we insert the left endpoint into the tree. */
  ins = nod[arr[pos]];
  ite = tre->root;
  parent = NULL;
  while (ite != NULL)
  {
    parent = ite;
    if (ins->key <= ite->key)
      ite = ite->left;
    else
      ite = ite->right;
    ++parent->size;
  }

  if (parent == NULL)
  {
    tre->root = ins;
    return;
  }
  else if (ins->key <= parent->key)
    parent->left = ins;
  else
    parent->right = ins;
  ins->parent = parent;
}

void sub_quick_sort (int *arr, int beg, int end, node **nod)
{
  int pivot, tmp;
  int i, j;

  if (beg >= end)
    return;

  pivot = beg + rand () % (end - beg + 1);
  tmp = arr[pivot];
  arr[pivot] = arr[beg];
  arr[beg] = tmp;

  i = beg;
  j = end;
  while (i < j)
  {
    while (i < j && nod[arr[j]]->key > nod[tmp]->key)
      --j;
    arr[i++] = arr[j];

    while (i < j && nod[arr[i]]->key < nod[tmp]->key)
      ++i;
    arr[j--] = arr[i];
  }

  if (j == i - 2)
    --i;
  arr[i] = tmp;

  sub_quick_sort (arr, beg, i - 1, nod);
  sub_quick_sort (arr, i + 1, end, nod);
}

void quick_sort (int *arr, int beg, int end, node **nod)
{
  srand (time (NULL));
  sub_quick_sort (arr, beg, end, nod);
}

int main ()
{
  /* Test intervals: [6, 9], [3, 10], [14, 19] ... */
  int key[20] = { 6, 3, 14, 13, 11, 1, 5, 2, 18, 15, 9, 10, 19, 16, 12, 4, 8, 7, 20, 17 };
  int arr[20];
  node *nod[20];
  int sum, i;
  tree *tre;

  for (i = 0; i < 20; ++i)
  {
    arr[i] = i;
    nod[i] = malloc (sizeof (node));
    nod[i]->key = key[i];
    nod[i]->size = 1;
    nod[i]->parent = nod[i]->left = nod[i]->right = NULL;
  }

  /* Sort all endpoints. */
  quick_sort (arr, 0, 19, nod);

  sum = 0;
  tre = malloc (sizeof (tree));
  tre->root = NULL;
  for (i = 0; i < 20; ++i)
    insert (tre, nod, arr, 10, i, &sum);

  printf ("Sum: %d\n", sum);

  return 0;
}

