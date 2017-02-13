/* Shellsort. */

#include <stdio.h>

void shell_sort (int *arr, int len)
{
  int gap;
  int i;
  int j;
  int tmp;

  for (gap = len / 2; gap > 0; gap /= 2)
  {
    for (i = gap; i < len; ++i)
    {
      tmp = arr[i];

      for (j = i; j >= gap & tmp < arr[j - gap]; j -= gap)
        arr[j] = arr[j - gap];

      arr[j] = tmp;
    }
  }
}

int main()
{
  int i;
  int arr[10] = { 4, 6, 2, 1, 5, 9, 8, 0, 3, 7 };

  shell_sort (arr, 10);

  for (i = 0; i < 10; ++i)
    printf ("%d ", arr[i]);
  printf ("\n");
}
