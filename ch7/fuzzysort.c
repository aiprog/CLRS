/* Fuzzy sorting of intervals. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Partition the array. */

void partition (int (*arr)[2], int beg, int end, int *p, int *r)
{
  /* Keep track of overlapping interval. */
  int min, max;
  int i, j, k;
  long tmp;

  min = arr[end][0];
  max = arr[end][1];
  i = beg - 1;
  j = beg - 1;
  for (k = beg; k < end; ++k)
  {
    if (arr[k][1] < min)
    {
      ++j;
      tmp = *(long *)arr[k];
      *(long *)arr[k] = *(long *)arr[j];
      *(long *)arr[j] = tmp;

      ++i;
      tmp = *(long *)arr[j];
      *(long *)arr[j] = *(long *)arr[i];
      *(long *)arr[i] = tmp;
    }
    /* [arr[k][0], arr[k][1]] and [min, max] have a overlapping interval. */
    else if (arr[k][0] <= max)
    {
      ++j;
      tmp = *(long *)arr[k];
      *(long *)arr[k] = *(long *)arr[j];
      *(long *)arr[j] = tmp;

      /* Update [min, max]. */
      if (arr[k][0] > min)
	min = arr[k][0];
      if (arr[k][1] < max)
	max = arr[k][1];
    }
  }

  ++j;
  tmp = *(long *)arr[k];
  *(long *)arr[k] = *(long *)arr[j];
  *(long *)arr[j] = tmp;

  *p = i + 1;
  *r = j;
}

/* Fuzzy sorting. */

void fuzzysort (int (*arr)[2], int beg, int end)
{
  int p, r;

  if (beg < end)
  {
    partition (arr, beg, end, &p, &r);

    fuzzysort (arr, beg, p - 1);
    fuzzysort (arr, r + 1, end);
  }
}

/* Print a character repeatly. */

void printn (int num, char ch)
{
  while (num-- >= 0)
    printf ("%c", ch);
}

#define NUM 20

int main ()
{
  int (*arr)[2] = malloc (sizeof (int) * 2 * NUM);
  int i;
  long beg, end;

  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
  {
    arr[i][0] = rand () % NUM;
    arr[i][1] = arr[i][0] + rand () % NUM + 1;
  }

  beg = clock ();
  fuzzysort (arr, 0, NUM - 1);
  end = clock ();

  printf ("Time: %ld\n", (end - beg) / 1000);

  if (NUM <= 20)
  {
    for (i = 0; i < NUM; ++i)
    {
      printn (arr[i][0] - 1, ' ');
      printf ("%d", arr[i][0]);
      printn (arr[i][1] - arr[i][0] - 1, '*');
      printf ("%d", arr[i][1]);
      printf ("\n");
    }
  }

  return 0;
}
