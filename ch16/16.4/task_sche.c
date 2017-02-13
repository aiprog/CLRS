#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void quick_sort (int *index, int *weight, int beg, int end)
{
  int pivot, temp;
  int i, j;
  static int flag = 0;

  if (flag == 0)
  {
    srand (time (NULL));
    flag = 1;
  }

  if (end <= beg)
    return;

  pivot = beg + rand () % (end - beg + 1);
  temp = index[beg];
  index[beg] = index[pivot];
  index[pivot] = temp;
  pivot = index[beg];

  temp = weight[index[beg]];
  i = beg; j = end;
  while (i < j)
  {
    while (i < j && weight[index[j]] < temp)
      --j;
    index[i] = index[j];
    ++i;

    while (i < j && weight[index[i]] > temp)
      ++i;
    index[j] = index[i];
    --j;
  }
  if (i != j)
    ++j;
  index[j] = pivot;

  quick_sort (index, weight, beg, j - 1);
  quick_sort (index, weight, j + 1, end);
}

void task_sche (int *dl, int *weight, int size)
{
  int *index, *arr;
  int i, j;

  index = malloc (sizeof (int) * size);
  for (i = 0; i < size; ++i)
    index[i] = i;

  quick_sort (index, weight, 0, size - 1);

  arr = malloc (sizeof (int) * size);
  memset (arr, 0, sizeof (int) * size);
  for (i = 0; i < size; ++i)
  {
    for (j = dl[index[i]]; j < size; ++j)
    {
      ++arr[j];
      if (arr[j] > j)
	break;
    }

    if (j == size)
      printf ("%d ", index[i]);
    else
    {
      while (j >= dl[index[i]])
      {
	--arr[j];
	--j;
      }
    }
  }
  printf ("\n");

  free (index);
  free (arr);
}

int main ()
{
  int dl[7] = { 7, 1, 4, 4, 2, 4, 3 };
  int weight[7] = { 10, 30, 20, 70, 60, 50, 40 };

  task_sche (dl, weight, 7);

  return 0;
}
