#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insert_sort (int *arr, int beg, int end)
{
  int tmp;
  int i, j;

  for (i = beg + 1; i <= end; ++i)
  {
    tmp = arr[i];
    for (j = i - 1; j >= beg && arr[j] > tmp; --j)
      arr[j + 1] = arr[j];
    arr[j + 1] = tmp;
  }
}

int get_median (int *arr1, int *arr2, int beg1, int end1, int beg2, int end2)
{
  int median1, median2;

  /* If there is only one element in each array,
   * return the smaller. */
  if (end1 == beg1)
  {
    if (arr1[beg1] <= arr2[beg2])
      return arr1[beg1];
    else
      return arr2[beg2];
  }

  /* Compute the medians of the two arrays. */
  median1 = (beg1 + end1) / 2;
  median2 = (beg2 + end2) / 2;
  /* When there are odd elemets in each array. */
  if ((end1 - beg1 + 1) % 2)
  {
    if (arr1[median1] <= arr2[median2])
    {
      /* When 'arr1[median1]' is in the middle of 'arr2[median2 - 1]'
       * and 'arr2[median2]', we can make sure that 'arr1[median1]' is the median
       * of the two arrays. */
      if (arr1[median1] >= arr2[median2 - 1])
	return arr1[median1];
      /* Otherwise, the median must be in 'arr1[median1 + 1...endl1]' or
       * 'arr2[beg2...median2 - 1]'. */
      else
	return get_median (arr1, arr2, median1 + 1, end1, beg2, median2 - 1);
    }
    else
    {
      if (arr2[median2] >= arr1[median1 - 1])
	return arr2[median2];
      else
	return get_median (arr1, arr2, beg1, median1 - 1, median2 + 1, end2);
    }
  }
  /* When there are even elements in each array. */
  else
  {
    if (arr1[median1] >= arr2[median2])
    {
      if (arr1[median1] <= arr2[median2 + 1])
	return arr1[median1];
      /* In order to keep the two subarrays have equal size,
       * we pass 'median1' not 'median1 - 1'. */
      else
	return get_median (arr1, arr2, beg1, median1, median2 + 1, end2);
    }
    else
    {
      if (arr2[median2] <= arr1[median1 + 1])
	return arr2[median2];
      else
	return get_median (arr1, arr2, median1 + 1, end1, beg2, median2);
    }
  }
}

#define	NUM	100000

int main ()
{
  int *arr;
  int median, pos, tmp;
  int i;

  /* Generate test data. */
  arr = malloc (sizeof (int) * NUM);
  for (i = 0; i < NUM; ++i)
    arr[i] = i + 1;
  /* Permutate test data. */
  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
  {
    pos = i + rand () % (NUM - i);

    tmp = arr[pos];
    arr[pos] = arr[i];
    arr[i] = tmp;
  }

  /* Sort the two subarray. */
  insert_sort (arr, 0, NUM / 2 - 1);
  insert_sort (arr, NUM / 2, NUM - 1);

  /* Get the median. */
  median = get_median (arr, arr, 0, NUM / 2 - 1, NUM / 2, NUM - 1);

  /* Verify. */
  if (median != NUM / 2)
  {
    printf ("Failed\n%d\n", median);
    exit (1);
  }
  printf ("Success\n");

  /* Free the memory. */
  free (arr);

  return 0;
}
