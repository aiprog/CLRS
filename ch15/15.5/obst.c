#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX(i, j)	((i) > (j) ? (i): (j))

void print_solu (void *root, int size, int parent, char *dir, int beg, int end)
{
  int (*memo)[size + 1] = root;
  int split;

  split = memo[end][beg - 1];
  if (parent == -1)
    printf ("k%d is the root\n", split + 1);
  else if (beg - end == 1)
  {
    printf ("d%d is the %s child of k%d\n", split + 1, dir, parent);
    return;
  }
  else
    printf ("k%d is the %s child of k%d\n", split + 1, dir, parent);

  print_solu (root, size, split + 1, "left", beg, split);
  print_solu (root, size, split + 1, "right", split + 2, end);
}

void obst (int *key, int *dummy, int size)
{
  int (*cost)[size + 1], (*weight)[size + 1], (*root)[size + 1];
  int i, j;
  int l, temp;

  cost = malloc (sizeof (int) * (size + 1) * (size + 1));
  weight = malloc (sizeof (int) * (size + 1) * (size + 1));
  root = malloc (sizeof (int) * (size + 1) * (size + 1));

  for (j = 0; j <= size; ++j)
  {
    cost[j][j] = dummy[j];
    weight[j][j] = dummy[j];
    root[j][j] = j - 1;
  }

  for (l = 1; l <= size; ++l)
  {
    for (i = 0; i <= size - l; ++i)
    {
      weight[i + l][i] = weight[i + l - 1][i] + key[i + l - 1] + dummy[i + l];
      cost[i + l][i] = INT_MAX;
      for (j = MAX (i, root[i + l - 1][i]); j <= root[i + l][i + 1]; ++j)
      {
	temp = cost[j][i] + cost[i + l][j + 1] + weight[i + l][i];
	if (temp < cost[i + l][i])
	{
	  cost[i + l][i] = temp;
	  root[i + l][i] = j;
	}
      }
    }
  }

  printf ("Min: %d\n", cost[size][0]);
  print_solu (root, size, -1, NULL, 1, size);  
}

int main ()
{
  int key[5] = { 15, 10, 5, 10, 20 };
  int dummy[6] = { 5, 10, 5, 5, 5, 10 };

  obst (key, dummy, 5);

  return 0;
}
