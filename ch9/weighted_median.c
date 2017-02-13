#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "select_linear.h"

int weighted_median (int *arr, int beg, int end, int weight)
{
  int i;
  int tmp, pivot;

  /* Get the median of 'arr[beg...end]'. */
  pivot = select_linear (arr, beg, end, (end - beg + 2) / 2);
  tmp = weight;
  for (i = beg; i < pivot && tmp < 50; ++i)
    tmp += arr[i];

  /* The weighted median is in 'arr[beg...pivot - 1]'. */
  if (i < pivot)
    return weighted_median (arr, beg, pivot - 1, weight);
  /* The 'arr[pivot]' is the weighted median. */
  else if (tmp + arr[pivot] >= 50)
    return pivot;
  /* The weighted median is in 'arr[pivot + 1...end]'. */
  else
    return weighted_median (arr, pivot + 1, end, tmp + arr[pivot]);
}

int main ()
{
  int arr[10] = { 0, 2, 5, 8, 9, 10, 11, 15, 19, 21 };
  int w_median, tmp, pos;
  int i;

  /* Permutate test data. */
  srand (time (NULL));
  for (i = 0; i < 10; ++i)
  {
    pos = i + rand () % (10 - i);

    tmp = arr[pos];
    arr[pos] = arr[i];
    arr[i] = tmp;
  }

  w_median = weighted_median (arr, 0, 9, 0);
  if (arr[w_median] != 15)
    printf ("Failed\n");
  else
    printf ("Success\n");

  return 0;
}
