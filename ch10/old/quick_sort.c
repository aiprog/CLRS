/*
 * 	Improved quick sort that performs well when the input
 * 	is ordered or equal.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quick_sort (int *arr, int beg, int end)
{
  int mid, pivot;
  int tmp;
  int i, j;

  if (beg >= end)
    return;

  /* Resist to ordered input. */
  mid = (beg + end) / 2;
  if (1.0 * (arr[beg] - arr[mid]) * (arr[beg] - arr[end]) < 0)
    pivot = beg;
  else if (1.0 * (arr[mid] - arr[beg]) * (arr[mid] - arr[end]) < 0)
    pivot = mid;
  else
    pivot = end;

  tmp = arr[pivot];
  arr[pivot] = arr[beg];
  arr[beg] = tmp;

  i = beg;
  j = end;
  /* Resist to equal input. */
  while (i < j)
  {
    while (i < j && arr[j] > tmp)
      --j;
    arr[i++] = arr[j];

    while (i < j && arr[i] < tmp)
      ++i;
    arr[j--] = arr[i];
  }
  arr[j + 1] = tmp;

  if (i - 1 == j)
    ++i;
  quick_sort (arr, beg, j);
  quick_sort (arr, i, end);
}

#define	NUM	20

int main ()
{
  int *arr;
  long beg, end;
  int i;

  /* Generate test data. */
  srand (time (NULL));
  arr = malloc (sizeof (int) * NUM);
  for (i = 0; i < NUM; ++i)
    arr[i] = rand () % NUM;
  if (NUM <= 20)
  {
    printf ("Input: ");
    for (i = 0; i < NUM; ++i)
      printf ("%d ", arr[i]);
    printf ("\n");
  }

  beg = clock ();
  quick_sort (arr, 0, NUM - 1);
  end = clock ();
  printf ("Time: %ldms\n", (end - beg) / 1000);

  /* Print the result. */
  if (NUM <= 20)
  {
    printf ("Output: ");
    for (i = 0; i < NUM; ++i)
      printf ("%d ", arr[i]);
    printf ("\n");
  }

  /* Free the memory. */
  free (arr);

  return 0;
}
