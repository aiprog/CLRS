/*
 * 	An implementation of quicksort with the same comparisons as
 * 	the randomly built binary search tree.
 */

#include <stdio.h>
#include <string.h>

#include "bin_search_tree.h"

void quicksort (int *arr, int beg, int end)
{
  int tmp[10];
  int pivot;
  int i, j;

  if (beg >= end)
    return;

  /* Print the comparsions. */
  for (i = beg + 1; i <= end; ++i)
    printf ("%d %d\n", arr[i], arr[beg]);

  /* Save elements smaller than the pivot. */
  j = 0;
  for (i = beg + 1; i <= end; ++i)
  {
    if (arr[i] < arr[beg])
      tmp[j++] = arr[i];
  }

  /* Save the pivot. */
  pivot = beg + j;
  tmp[j++] = arr[beg];

  /* Save elements greater than the pivot. */
  for (i = beg + 1; i <= end; ++i)
  {
    if (arr[i] > arr[beg])
      tmp[j++] = arr[i];
  }

  /* Copy the partition result to the original array. */
  memcpy (arr + beg, tmp, sizeof (int) * (end - beg + 1));

  quicksort (arr, beg, pivot - 1);
  quicksort (arr, pivot + 1, end);
}

int main ()
{
  int i;
  int arr[10] = { 5, 3, 7, 4, 2, 9, 1, 8, 0, 6 };
  node *root;

  root = NULL;
  for (i = 0; i < 10; ++i)
    insert (&root, arr[i]);

  printf ("*************\n");

  quicksort (arr, 0, 9);

  return 0;
}
