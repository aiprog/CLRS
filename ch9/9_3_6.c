#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "select_linear.h"

void quantiles (int *arr, int beg, int end, int n, int *quan, int *pos)
{
  int len, pivot;
  int left_n, right_n;

  /* The size of 'arr' must be diviable by 'n'. */
  len = end - beg + 1;
  if (len % n)
  {
    printf ("Beg: %d End: %d\n", beg, end);
    printf ("Can't divide evenly\n");
    exit (1);
  }

  /* Return 'end' directly if we shoule divide the array into one part. */
  if (n == 1)
  {
    quan[(*pos)++] = end;
    return;
  }

  /* Calculate the number of parts of the two subarrays. */
  left_n = n / 2;
  right_n = n - left_n;

  /* Divide the array. */
  pivot = select_linear (arr, beg, end, left_n * (len / n));
  /* Call 'quantiles' recursively on two subarrays. */
  quantiles (arr, beg, pivot, left_n, quan, pos);
  quantiles (arr, pivot + 1, end, right_n, quan, pos);
}

#define	ARR	100
#define	QUAN	10

int main ()
{
  int *arr, *quan;
  int tmp, pos;
  int i;

  /* Generate test data. */
  arr = malloc (sizeof (int) * ARR);
  for (i = 0; i < ARR; ++i)
    arr[i] = i + 1;
  /* Permutate test data. */
  srand (time (NULL));
  for (i = 0; i < ARR; ++i)
  {
    pos = i + rand () % (ARR - i);

    tmp = arr[pos];
    arr[pos] = arr[i];
    arr[i] = tmp;
  }

  quan = malloc (sizeof (int) * 10);
  pos = 0;
  quantiles (arr, 0, ARR - 1, QUAN, quan, &pos);

  /* Verify. */
  for (i = 0; i < QUAN; ++i)
  {
    if (quan[i] != (i + 1) * (ARR / QUAN) - 1)
    {
      printf ("Failed\n");
      exit (1);
    }
  }
  printf ("Success\n");

  /* Free the memory. */
  free (arr);
  free (quan);

  return 0;
}

