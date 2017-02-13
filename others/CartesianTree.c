#include <stdio.h>

typedef struct _node
{
  int value;
  int parent;         // Its parent in Cartesian trees
  int leftSmaller;    // The left nearest smaller value
  int rightSmaller;   // The right nearest samller value
} Node;

void CartesianTreeV1 (Node *tree, int size)
{
  tree[0].parent = -1;
  for (int i = 1; i < size; ++i)
  {
    int j = i - 1, tmp = -1;
    // Find the last greater node in a bottom-up search
    while (j != -1 && tree[j].value > tree[i].value)
    {
      tmp = j;
      j = tree[j].parent;
    }

    // Insert tree[i] into the tree
    tree[i].parent = j;
    if (tmp != -1)
      tree[tmp].parent = i;
  }
}

void allNearestSmallerValues (Node *tree, int size)
{
  tree[0].leftSmaller = -1;
  for (int i = 1; i < size; ++i)
  {
    int j = i - 1;
    while (j != -1 && tree[j].value >= tree[i].value)
      j = tree[j].leftSmaller;
    tree[i].leftSmaller = j;
  }

  tree[size - 1].rightSmaller = -1;
  for (int i = size - 2; i >= 0; --i)
  {
    int j = i + 1;
    while (j != -1 && tree[j].value >= tree[i].value)
      j = tree[j].rightSmaller;
    tree[i].rightSmaller = j;
  }
}

void CartesianTreeV2 (Node *tree, int beg, int end, int parent)
{
  if (end < beg)
    return;

  // Construct the leftmost path
  int left = beg;
  while (tree[left].rightSmaller >= beg && tree[left].rightSmaller <= end)
  {
    tree[left].parent = tree[left].rightSmaller;
    CartesianTreeV2 (tree, left + 1, tree[left].rightSmaller - 1, left);
    left = tree[left].rightSmaller;
  }
  tree[left].parent = parent;

  // Construct the rightmost path
  int right = end;
  while (tree[right].leftSmaller >= beg && tree[right].leftSmaller <= end)
  {
    tree[right].parent = tree[right].leftSmaller;
    CartesianTreeV2 (tree, tree[right].leftSmaller + 1, right - 1, right);
    right = tree[right].leftSmaller;
  }

  // When there are more than one minimum, link the right to the left
  if (left != right)
  {
    tree[right].parent = left;
    CartesianTreeV2 (tree, left + 1, right - 1, right);
  }
}

void CartesianTreeV3 (Node *tree, int beg, int end)
{
  if (beg == end)
  {
    tree[beg].parent = -1;
    return;
  }

  int mid = (beg + end) / 2;
  CartesianTreeV3 (tree, beg, mid);
  CartesianTreeV3 (tree, mid + 1, end);

  // Merge two paths
  int left = mid, right = mid + 1, current = -1;
  while (left != -1 && right != -1)
  {
    int *smaller;
    if (tree[left].value >= tree[right].value)
      smaller = &left;
    else
      smaller = &right;

    if (current != -1)
      tree[current].parent = *smaller;
    current = *smaller;

    *smaller = tree[*smaller].parent;
  }

  if (left == -1)
    tree[current].parent = right;
  else
    tree[current].parent = left;
}

int main()
{
  int value[10] = { 4, 10, 2, 4, 1, 5, 9, 6, 5, 0 };
  Node tree[10];

  for (int i = 0; i < 10; ++i)
    tree[i].value = value[i];

  CartesianTreeV1 (tree, 10);

  //allNearestSmallerValues (tree, 10);
  //CartesianTreeV2 (tree, 0, 9, -1);

  //CartesianTreeV3 (tree, 0, 9);

  for (int i = 0; i < 10; ++i)
    printf ("%d ", tree[i].parent);
  printf ("\n");

  return 0;
}