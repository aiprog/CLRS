#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void print_solu (int *point, void *Solu, int size, int beg, int end)
{
  int (*solu)[size] = Solu;
  int split;

  if (beg + 1 == end)
    return;

  split = solu[end][beg];
  printf ("%d ", point[split]);
  print_solu (point, solu, size, beg, split);
  print_solu (point, solu, size, split, end);
}

void bs (int *point, int size)
{
  int (*memo)[size], (*solu)[size];
  int i, j, k, l;
  int temp;

  memo = malloc (sizeof (int) * size * size);
  solu = malloc (sizeof (int) * size * size);

  for (i = 0; i < size - 1; ++i)
    memo[i + 1][i] = 0;

  for (l = 2; l < size; ++l)
  {
    for (i = 0; i < size - l; ++i)
    {
      j = i + l;
      memo[j][i] = INT_MAX;
      for (k = i + 1; k < i + l; ++k)
      {
	temp = memo[k][i] + memo[j][k] + point[j] - point[i];
	if (temp < memo[j][i])
	{
	  memo[j][i] = temp;
	  solu[j][i] = k;
	}
      }
    }
  }

  printf ("Min: %d\n", memo[size - 1][0]);
  printf ("Split: %d ", point[0]);
  print_solu (point, solu, size, 0, size - 1);
  printf ("%d\n", point[size - 1]);
}

int main ()
{
  int point[5] = { 0, 2, 8, 10, 20 };

  bs (point, 5);

  return 0;
}
