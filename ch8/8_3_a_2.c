#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM 20

/* Return 10^exp. */

int exp_10 (int exp)
{
  int res;

  res = 1;
  while (exp--)
    res *= 10;

  return res;
}

/* Counting sort. */

void counting_sort (int *arr, int beg, int end, int digit, int *values, int *res)
{
  int cnt[10] = { 0 };
  int i;

  /* Calculate the digit in the position digit of arr[i] and
   * let cnt[i] contains the number of elements equal to i.*/
  for (i = beg; i <= end; ++i)
  {
    values[i] = arr[i] / exp_10 (digit - 1) % 10;
    ++cnt[values[i]];
  }

  /* Cnt[i] contains the number of elements less than or equal
   * to i. */
  for (i = 1; i < 10; ++i)
    cnt[i] += cnt[i - 1];

  /* Place arr[i] in the proper position in res. */
  for (i = end; i >= beg; --i)
  {
    res[cnt[values[i]] - 1] = arr[i];
    --cnt[values[i]];
  }

  memcpy (&arr[beg], res, sizeof (int) * (end - beg + 1));
}

/* Radix sort. */

void radix_sort (int *arr)
{
  int *res, *values;
  int beg, end;
  int digit;
  int i;

  values = malloc (sizeof (int) * NUM);
  res = malloc (sizeof (int) * NUM);

  beg = 0;
  end = NUM - 1;
  digit = 1;
  while (beg < NUM - 1)
  {
    counting_sort (arr, beg, end, digit, values, res);
    /* Only sort numbers with at least (digit + 1) digits. */
    while (beg < NUM - 1 && arr[beg] / exp_10 (digit) == 0)
      ++beg;
    ++digit;
  }

  free (res);
  free (values);
}

int main ()
{
  int *arr;
  int i;

  arr = malloc (sizeof (int) * NUM);
  /* Generate test data and print them. */
  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
  {
    if (NUM <= 50)
      arr[i] = rand () % (exp_10 (rand () % 5));
    else
      arr[i] = rand () % NUM;
    if (NUM <= 50)
      printf ("%d ", arr[i]);
  }
  if (NUM <= 50)
    printf ("\n");

  long beg = clock ();
  /* Radix sort. */
  radix_sort (arr);
  long end = clock ();
  printf ("Time: %ld\n", (end - beg) / 1000);

  /* Print the result. */
  if (NUM <= 50)
  {
    for (i = 0; i < NUM; ++i)
      printf ("%d ", arr[i]);
    printf ("\n");
  }

  free (arr);

  return 0;
}
