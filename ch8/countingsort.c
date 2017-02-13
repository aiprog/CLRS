/* Counting sort. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Counting sort. */

void countingsort (int *arr, int len, int k)
{
  int *res;
  int *tmp;
  int i;
  long beg, end;

  res = malloc (sizeof (int) * len);
  tmp = malloc (sizeof (int) * (k + 1));
  memset (tmp, 0, sizeof (int) * (k + 1));

  beg = clock ();
  /* Let tmp[i] contains the number of elements equal to i. */
  for (i = 0; i < len; ++i)
    ++tmp[arr[i]];
  end = clock ();
  printf ("Time: %ld\n", (end - beg) / 1000);

  beg = clock ();
  /* Let tmp[i] contains the number of elements less than or equal to i. */
  for (i = 1; i < k + 1; ++i)
    tmp[i] += tmp[i - 1];
  end = clock ();
  printf ("Time: %ld\n", (end - beg) / 1000);

  beg = clock ();
  /* Place arr[i] into the proper position in the res array
   * according to the number of elements less than or equal to arr[i]. */
  for (i = len - 1; i > -1; --i)
  {
    res[tmp[arr[i]] - 1] = arr[i];
    --tmp[arr[i]];
  }
  end = clock ();
  printf ("Time: %ld\n", (end - beg) / 1000);

  memcpy (arr, res, sizeof (int) * len);

  free (res);
  free (tmp);
}

/* A variant of counting sort, using only O(k) extra space but
 * unstable. */

void _countingsort (int *arr, int len, int k)
{
  int *cnt;
  int pos;
  int tmp;
  int i;
  long beg, end;


  cnt = malloc (sizeof (int) * (k + 1));
  memset (cnt, 0, sizeof (int) * (k + 1));

  beg = clock ();
  /* Let cnt[i] contains the number of elements equal to i. */
  for (i = 0; i < len; ++i)
    ++cnt[arr[i]];
  end = clock ();
  printf ("Time: %ld\n", (end - beg) / 1000);

  beg = clock ();
  /* Let cnt[i] contains the number of elements less than or equal to i. */
  for (i = 1; i < k + 1; ++i)
    cnt[i] += cnt[i - 1];
  end = clock ();
  printf ("Time: %ld\n", (end - beg) / 1000);

  beg = clock ();
  i = len - 1;
  while (i > -1)
  {
    /* Reasonable position for arr[i]. */
    pos = cnt[arr[i]] - 1;
    /* We mustn't exchang arr[i] and arr[pos] when pos < i,
     * because when i decrease to pos latter, arr[pos] would be replaced
     * , which can cause errors. */
    if (pos > i)
    {
      --cnt[arr[i]];

      tmp = arr[i];
      arr[i] = arr[pos];
      arr[pos] = tmp;
    }
    else if (pos < i)
      --i;
    else
    {
      --cnt[arr[i]];
      --i;
    }
  }
  end = clock ();
  printf ("Time: %ld\n", (end - beg) / 1000);

  free (cnt);
}

#define NUM 100000000
#define MAX 100000000

int main ()
{
  int *arr = malloc (sizeof (int) * NUM);
  int i;

  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
  {
    arr[i] = rand () % MAX;
    if (NUM <= 50)
      printf ("%d ", arr[i]);
  }
  if (NUM <= 50)
    printf ("\n");

  long beg = clock ();
  countingsort (arr, NUM, MAX);
  long end = clock ();

  printf ("Time: %ld\n", (end - beg) / 1000);

  if (NUM <= 50)
  {
    for (i = 0; i < NUM; ++i)
      printf ("%d ", arr[i]);
    printf ("\n");
  }

  free (arr);

  return 0;
}
