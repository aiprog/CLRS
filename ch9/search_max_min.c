#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void search_max_min (int *arr, int size, int *max, int *min)
{
  int ma, mi;
  int i;

  /* If the 'size' is even, 'arr[0]' and 'arr[1]' are
   * the initial value for '*max' and '*min", otherwise
   * 'arr[0]' is the initial value for '*max' and '*min'. */
  if (size % 2)
  {
    *max = arr[0];
    *min = arr[0];
    i = 1;
  }
  else
  {
    if (arr[0] > arr[1])
    {
      *max = arr[0];
      *min = arr[1];
    }
    else
    {
      *max = arr[1];
      *min = arr[0];
    }
    i = 2;
  }

  while (i < size)
  {
    /* Find the maximum 'ma' and the minimum 'mi'
     * of 'arr[i]' and 'arr[i + 1]', then compare
     * 'ma' with '*max' and 'mi' with '*min' to
     * obtain the new maximum and minimum. */
    if (arr[i] > arr[i + 1])
    {
      ma = arr[i];
      mi = arr[i + 1];
    }
    else
    {
      ma = arr[i + 1];
      mi = arr[i];
    }

    if (ma > *max)
      *max = ma;
    if (mi < *min)
      *min = mi;

    i += 2;
  }
}

int main ()
{
  int max, min;
  int arr[100];
  int i;

  /* Generate the text data. */
  srand (time (NULL));
  max = -1;
  min = 1000;
  for (i = 0; i < 100; ++i)
  {
    arr[i] = rand () % 1000;
    if (arr[i] > max)
      max = arr[i];
    if (arr[i] < min)
      min = arr[i];
  }
  printf ("Max: %d Min: %d\n", max, min);

  search_max_min (arr, 100, &max, &min);

  /* Print the result. */
  printf ("Max: %d Min: %d\n", max, min);

  return 0;
}
