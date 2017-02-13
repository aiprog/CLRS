#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int randomized_select (int *arr, int beg, int end, int order)
{
  int pivot;
  int tmp;
  int i, j;

  srand (time (NULL));
  while (beg < end)
  {
    /* Chose a pivot randomly. */
    pivot = beg + rand () % (end - beg + 1);
    tmp = arr[pivot];
    arr[pivot] = arr[beg];
    arr[beg] = tmp;

    /* Partition. */
    i = beg;
    j = end;
    while (i < j)
    {
      while (i < j && arr[j] >= tmp)
	--j;
      arr[i] = arr[j];

      while (i < j && arr[i] <= tmp)
	++i;
      arr[j] = arr[i];
    }
    arr[i] = tmp;

    /* When the value is in pivot. */
    if (i - beg + 1 == order)
      return arr[i];
    /* When the value is in left subarray. */
    else if (i - beg + 1 > order)
      end = i - 1;
    /* When the value is in right shuarray. */
    else
    {
      order -= i - beg + 1;
      beg = i + 1;
    }
  }

  return arr[beg];
}

#define	NUM	10000000

int main ()
{
  int *arr;
  int key, pos, tmp;
  int i;

  /* Generate test data. */
  arr = malloc (sizeof (int) * NUM);
  for (i = 0; i < NUM; ++i)
    arr[i] = i;
  /* Permutate test data. */
  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
  {
    pos = i + rand () % (NUM - i);

    tmp = arr[pos];
    arr[pos] = arr[i];
    arr[i] = tmp;
  }

  /* Test program. */
  for (i = 0; i < 10; ++i)
  {
    if (randomized_select (arr, 0, NUM - 1, key) != key - 1)
    {
      printf ("Failed\n");
      exit (1);
    }
  }
  printf ("Success\n");

  /* Free the memory. */
  free (arr);

  return 0;
}
