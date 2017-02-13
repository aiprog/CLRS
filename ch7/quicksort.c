/* Randomized quicksort. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Partition the array by a random pivot. */

int partition (int *arr, int beg, int end)
{
  int pivot, mid;
  int i, j;
  int tmp;
  static int flg = 0;

  if (flg == 0)
  {
    flg = 1;
    srand (time (NULL));
  }

  pivot = beg + rand () % (end - beg + 1);
  /*mid = (beg + end) / 2;
  if ((arr[beg] - arr[mid]) *
      (arr[beg] - arr[end]) < 0)
    pivot = beg;
  else if ((arr[mid] - arr[beg]) *
           (arr[mid] - arr[end]) < 0)
    pivot = mid;
  else
    pivot = end;

  if (pivot != beg)
  {
    tmp = arr[beg];
    arr[beg] = arr[pivot];
    arr[pivot] = tmp;
  }

  pivot = arr[beg];*/

  tmp = arr[pivot];
  arr[pivot] = arr[end];
  arr[end] = tmp;

  pivot = end;
  i = beg - 1;
  for (j = beg; j < end; ++j)
  {
    if (arr[j] <= arr[pivot])
    {
      ++i;

      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
    }
  }

  ++i;

  tmp = arr[i];
  arr[i] = arr[pivot];
  arr[pivot] = tmp;

  return i;
}

/* C. A. R. Hoare partition. */

int hoare_partition (int *arr, int beg, int end)
{
  int pivot;
  int i, j;
  int tmp;

  pivot = arr[beg];
  i = beg - 1;
  j = end + 1;

  while (1)
  {
    do 
    {
      --j;
    } while (arr[j] > pivot);

    do
    {
      ++i;
    } while (arr[i] < pivot);

    if (i < j)
    {
      tmp = arr[j];
      arr[j] = arr[i];
      arr[i] = tmp;
    }
    else
      return j;
  }
}

/* Another way to partition. */

int new_partition (int *arr, int beg, int end)
{
  int pivot, mid;
  int tmp;

  /*mid = (beg + end) / 2;
  if ((arr[beg] - arr[mid]) *
      (arr[beg] - arr[end]) < 0)
    pivot = beg;
  else if ((arr[mid] - arr[beg]) *
           (arr[mid] - arr[end]) < 0)
    pivot = mid;
  else
    pivot = end;*/
  pivot = beg + rand () % (end - beg + 1);

  //if (pivot != beg)
  //{
    tmp = arr[beg];
    arr[beg] = arr[pivot];
    arr[pivot] = tmp;
  //}

  pivot = arr[beg];
  while (end > beg)
  {
    while (end > beg && arr[end] >= pivot)
      --end;
    arr[beg] = arr[end];

    while (end > beg && arr[beg] <= pivot)
      ++beg;
    arr[end] = arr[beg];
  }
  arr[beg] = pivot;

  return beg;
}

/* Optimized version for arrays with many equal elements. 
 * All elements of arr[q...t] are equal.
 * Each element of arr[beg...q-1] is less than arr[q].
 * Each element of arr[r+1...end] is greater than arr[q] */

void opt_partition (int *arr, int beg, int end, int *q, int *t)
{
  int i, j, k;
  int tmp;

  i = beg - 1;
  j = beg - 1;
  for (k = beg; k < end; ++k)
  {
    if (arr[k] < arr[end])
    {
      ++j;
      tmp = arr[k];
      arr[k] = arr[j];
      arr[j] = tmp;

      ++i;
      tmp = arr[j];
      arr[j] = arr[i];
      arr[i] = tmp;
    }
    else if (arr[k] == arr[end])
    {
      ++j;
      tmp = arr[k];
      arr[k] = arr[j];
      arr[j] = tmp;
    }
  }

  ++j;
  tmp = arr[k];
  arr[k] = arr[j];
  arr[j] = tmp;

  *q = i + 1;
  *t = j;
}

/* Quicksort. */

void quicksort (int *arr, int beg, int end)
{
  int q, t;

  if (beg < end)
  {
    q = new_partition (arr, beg, end);
    //q = new_partition (arr, beg, end);
    t = q;

    quicksort (arr, beg, q - 1);
    quicksort (arr, t + 1, end);
  }

  /*while (beg < end)
  {
    q = partition (arr, beg, end);
    quicksort (arr, beg, q - 1);
    beg = q + 1;
  }*/
}

#define NUM 10000000

int main (int argc, char *argv[])
{
  //int arr[10] = { 4, 3, 9, 8, 5, 6, 7, 0, 1, 2 };
  int *arr = malloc (sizeof (int) * NUM);
  int i;

  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
    arr[i] = i;

  long beg = clock ();
  quicksort (arr, 0, NUM - 1);
  long end = clock ();

  printf ("time: %d\n", (end - beg) / 1000);
  
  //quicksort (arr, 0, NUM - 1);

  //printf ("%d\n", new_partition (arr, 0, NUM - 1));
  if (NUM <= 20)
  {
  for (i = 0; i < NUM; ++i)
    printf ("%d ", arr[i]);
  printf ("\n");
  }

  /*int q, t;
  opt_partition (arr, 0, 9, &q, &t);
  printf ("%d %d\n", q, t);
  for (i = 0; i < 10; ++i)
    printf ("%d ", arr[i]);
  printf ("\n");*/

  return 0;
}
