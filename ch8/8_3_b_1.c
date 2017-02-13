#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM 10000000
#define LEN 10

/* Counting sort. */

void counting_sort (char **arr, int beg, int end, int digit, char **res, int *cnt)
{
  int left, right;
  int i;

  if (beg == end)
    return;

  memset (cnt, 0, sizeof (int) * 123);
  /* Cnt[i] contains the number of elements equal to 'a' + i. */
  for (i = beg; i <= end; ++i)
    ++cnt[arr[i][digit]];

  /* Cnt[i] contains the number of elements less than
   * or equal to 'a' + i. */
  for (i = 1; i < 123; ++i)
    cnt[i] += cnt[i - 1];

  /* Place arr[i] in the proper position in res. */
  for (i = end; i >= beg; --i)
  {
    res[cnt[arr[i][digit]] - 1 + beg] = arr[i];
    --cnt[arr[i][digit]];
  }

  memcpy (arr + beg, res + beg, sizeof (char*) * (end - beg + 1));

  left = beg;
  /* Only consider strings with at least (digit + 1) characters. */
  while (left < end && arr[left][digit] == '\0')
    ++left;
  while (left < end)
  {
    right = left;
    while (right <= end && arr[right][digit] == arr[left][digit])
      ++right;
    --right;

    /* Call count_sort on subarray recursively. */
    counting_sort (arr, left, right, digit + 1, res, cnt);
    left = right + 1;
  }
}

int main ()
{
  char **arr, **res;
  int len, cnt[122];
  int i;

  arr = malloc (sizeof (char*) * NUM);
  /* Generate test data and print them. */
  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
  {
    len = rand () % LEN + 1;
    arr[i] = malloc (sizeof (char) * (len + 1));
    arr[i][len] = '\0';
    while (--len > -1)
      arr[i][len] = 'a' + rand () % 26;
    if (NUM <= 100)
      printf ("%s ", arr[i]);
  }
  if (NUM <= 100)
    printf ("\n");

  res = malloc (sizeof (char*) * NUM);
  /* Counting sort. */
  long beg = clock ();
  counting_sort (arr, 0, NUM - 1, 0, res, cnt);
  long end = clock ();
  printf ("Time: %d\n", (end - beg) / 1000);

  /* Print the result. */
  if (NUM <= 100)
  {
    for (i = 0; i < NUM; ++i)
      printf ("%s ", arr[i]);
    printf ("\n");
  }

  /* Free the memory. */
  for (i = 0; i < NUM; ++i)
    free (arr[i]);
  free (arr);
  free (res);

  return 0;
}
