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

int partition (int *arr, int beg, int end)
{
  int tmp;

  tmp = arr[beg];
  while (end > beg)
  {
    while (end > beg && arr[end] >= tmp)
      --end;
    arr[beg] = arr[end];

    while (end > beg && arr[beg] <= tmp)
      ++beg;
    arr[end] = arr[beg];
  }
  arr[beg] = tmp;

  return beg;
}

int select_linear (int *arr, int beg, int end, int order)
{
  int *medians;
  int tmp, pivot, value, num;
  int i, j, k;

  if (beg == end)
    return beg;

  /* Calculate the number of groups. */
  num = (end - beg + 1) / 5;
  if ((end - beg + 1) % 5)
    ++num;
  medians = malloc (sizeof (int) * num);

  /* Sort each group and store the median of each group. */
  k = 0;
  i = beg;
  while (i <= end)
  {
    j = i + 4;
    if (j > end)
      j = end;

    insert_sort (arr, i, j);
    medians[k++] = arr[(i + j) / 2];

    i = j + 1;
  }

  /* Compute the median of medians. */
  pivot = select_linear (medians, 0, num - 1, (num + 1) / 2);
  value = medians[pivot];
  free (medians);

  /* Look for the pivot with the same value as 
   * the median of medians of 'arr'. */
  j = beg;
  for (i = 0; i < num - 1; ++i)
  {
    if (arr[j + 2] == value)
    {
      pivot = j + 2;
      break;
    }
    j += 5;
  }
  if (i == num - 1)
    pivot = (j + end) / 2;

  tmp = arr[beg];
  arr[beg] = arr[pivot];
  arr[pivot] = tmp;
  /* Partition, it return the position not the value. */
  pivot = partition (arr, beg, end);

  /* When the pivot is the value we need, return it directly. */
  if (pivot - beg + 1 == order)
    return pivot;
  /* When the value is in the left subarray. */
  else if (pivot - beg + 1 > order)
    return select_linear (arr, beg, pivot - 1, order);
  /* When the value is in the right subarray. */
  else
    return select_linear (arr, pivot + 1, end, order - (pivot - beg + 1));
}

#define	NUM	10000

int main ()
{
  int *arr;
  int pos, tmp;
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

    tmp = arr[i];
    arr[i] = arr[pos];
    arr[pos] = tmp;
  }

  /* Verify. */
  for (i = 1; i < 10000; ++i)
  {
    if (arr[select_linear (arr, 0, NUM - 1, i)] != i)
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

