#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM 10000000

/* Return 10^exp. */

int exp_10 (int exp)
{
  int i;

  i = 1;
  while (exp-- > 0)
    i *= 10;

  return i;
}

/* Return the digits of a number. */

int get_digits (int num)
{
  int j;

  j = 1;
  while (num /= 10)
    ++j;

  return j;
}

/* Counting sort. */

void counting_sort (int *arr, int beg, int end, int digits, int *res, int *values, int *cnt)
{
  int divisor;
  int i;

  /* Calculate the digits in position digits for each data
   * and let cnt[i] contains the number of elements equal to i. */
  memset (cnt, 0, sizeof (int) * 10);
  divisor = exp_10 (digits - 1);
  for (i = beg; i <= end; ++i)
  {
    values[i] = arr[i] / divisor % 10;
    ++cnt[values[i]];
  }

  /* Cnt[i] contains the number of elements less than or equal to i. */
  for (i = 1; i < 10; ++i)
    cnt[i] += cnt[i - 1];

  /* Place arr[i] in the proper position in res. */
  for (i = end; i >= beg; --i)
  {
    res[cnt[values[i]] - 1 + beg] = arr[i];
    --cnt[values[i]];
  }

  memcpy (arr + beg, res + beg, sizeof (int) * (end - beg + 1));
}

/* Radix sort. */

void radix_sort (int *arr, int beg, int end, int digits)
{
  int i;
  int *res, *values, cnt[10];

  res = malloc (sizeof (int) * NUM);
  values = malloc (sizeof (int) * NUM); 
  for (i = 1; i <= digits; ++i)
    counting_sort (arr, beg, end, i, res, values, cnt);
}

int main ()
{
  int *arr, *res, *digits, *cnt;
  int max_digits, tmp, pos;
  int i, j, k;

  arr = malloc (sizeof (int) * NUM);
  /* Generate test data and print them. */
  srand (time(NULL));
  for (i = 0; i < NUM; ++i)
    //arr[i] = rand () % (exp_10 (rand () % 8));
    arr[i] = rand () % NUM;
  if (NUM <= 50)
  {
    for (i = 0; i < NUM; ++i)
      printf ("%d ", arr[i]);
    printf ("\n");
  }

  long beg = clock ();
  digits = malloc (sizeof (int) * NUM);
  /* Calculate digits for each data and keep the maximum. */
  max_digits = 0;
  for (i = 0; i < NUM; ++i)
  {
    digits[i] = get_digits(arr[i]);
    if (digits[i] > max_digits)
      max_digits = digits[i];
  }

  cnt = malloc (sizeof (int) * max_digits);
  /* Let cnt[i] contains the number of elements with i + 1 digits. */
  memset (cnt, 0, sizeof (int) * max_digits);
  for (i = 0; i < NUM; ++i)
    ++cnt[digits[i] - 1];

  /* Let cnt[i] contains the number of elemnets with less than or equal 
   * to i + 1 digits. */
  for (i = 1; i < max_digits; ++i)
    cnt[i] += cnt[i - 1];

  res = malloc (sizeof (int) * NUM);
  /* Place arr[i] in the proper position and update digits.*/
  for (i = NUM - 1; i > -1; --i)
  {
    res[cnt[digits[i] - 1] - 1] = arr[i];
    --cnt[digits[i] - 1];
  }

  free (arr);
  free (digits);
  free (cnt);
 
  /* Radix sort the segments of res with equal digits. */
  j = 0;
  while (j < NUM)
  {
    i = j;
    ++j;
    k = get_digits (res[i]);
    while (j < NUM && k == get_digits(res[j]))
      ++j;

    radix_sort (res, i, j - 1, k);
  }
  long end = clock ();
  printf ("Time: %ld\n", (end - beg) / 1000);

  /* Print the result. */
  if (NUM <= 50)
  {
    for (i = 0; i < NUM; ++i)
      printf ("%d ", res[i]);
    printf ("\n");
  }

  free (res);

  return 0;
}
