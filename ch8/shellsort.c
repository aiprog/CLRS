#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void shellsort (int *arr, int len)
{
  int d, tmp;
  int i, j;

  /* Initial increment is 'len / 2', and  halved every loop until 1. */ 
  for (d = len / 2; d >= 1; d /= 2)
  {
    for (i = d; i < len; ++i)
    {
      tmp = arr[i];
      for (j = i - d; j >= 0 && arr[j] > tmp; j -= d)
	arr[j + d] = arr[j];
      arr[j + d] = tmp;
    }
  }
}

#define	NUM	10000000

int main ()
{
  int *arr;
  int tmp, pos;
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

  shellsort (arr, NUM);

  /* Verify. */
  for (i = 0; i < NUM; ++i)
    if (arr[i] != i)
    {
      printf ("Failed\n");
      exit (1);
    }
  printf ("Success\n");

  /* Free the memory. */
  free (arr);

  return 0;
}
