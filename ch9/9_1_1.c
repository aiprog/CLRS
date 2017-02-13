#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void search_2th_min (int *arr, int beg, int end, int *min, int *sec)
{
  int arr_a, arr_b;
  int start_a, start_b;
  int tmp, size;
  int sub_min, sub_sec;
  int i, j;

  size = end - beg + 1;
  if (size == 2)
  {
    if (arr[beg] > arr[beg + 1])
    {
      *sec = beg;
      *min = beg + 1;
    }
    else
    {
      *sec = beg + 1;
      *min = beg;
    }

    return;
  }

  /* Divide 'arr' into two subarrays starting at 
   * 'arr[arr_a]' and 'arr[arr_b]' respectively, then compare
   * them from 'arr[start_a]' and 'arr[start_b]' and exchange the smaller
   * one into the former. */
  arr_a = beg;
  if (size % 2)
  {
    start_a = arr_a + 1;
    arr_b = beg + size / 2 + 1;
  }
  else
  {
    start_a = arr_a;
    arr_b = beg + size / 2;
  }
  start_b = arr_b;

  i = start_a;
  j = start_b;
  while (j <= end)
  {
    if (arr[i] > arr[j])
    {
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
    }
    ++i;
    ++j;
  }

  /* Now, the smallest is in array 'arr[arr_a]' and the second 
   * smallest can be in array 'arr[arr_a]' or 'arr[arr_b]'.
   * If it is in 'arr[arr_b]', it must has the offset 'sub_min - start_a'. */
  search_2th_min (arr, beg, arr_b - 1, &sub_min, &sub_sec);

  if (arr[start_b + sub_min - start_a] < arr[sub_sec])
    *sec = start_b + sub_min - start_a;
  else
    *sec = sub_sec;
  *min = sub_min;
}

#define	NUM	100

int main ()
{
  int *arr;
  int min, sec, tmp;
  int i, j;

  arr = malloc (sizeof (int) * NUM);
  min = NUM;
  sec = NUM;
  /* Generate test data. */
  for (i = 0; i < NUM; ++i)
    arr[i] = i;
  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
  {
    j = rand () % NUM;

    tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
  }
  printf ("Min: %d Sec: %d\n", 0, 1);

  search_2th_min (arr, 0, NUM - 1, &min, &sec);

  /* Print the result. */
  printf ("Min: %d Sec: %d\n", arr[min], arr[sec]);

  /* Free the memory. */
  free (arr);

  return 0;
}
