#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM 10000000

void divide_and_conquer (int (*red)[2], int (*blue)[2], int beg, int end)
{
  int pivot, mid, jug, water;
  int tmp[2];
  int i, j;

  if (beg >= end)
    return;

  /* Chose the middle of 'red[beg][1]',
   * 'red[mid][1]' and 'red[end][1]' as the pivot. */
  mid = (beg + end) / 2;
  if ((red[beg][1] - red[mid][1] ) * 
      (red[beg][1] - red[end][1]) < 0)
    pivot = beg;
  else if ((red[mid][1] - red[beg][1]) *
           (red[mid][1] - red[end][1]) < 0)
    pivot = mid;
  else
    pivot = end;
  if (pivot != beg)
  {
    tmp[0] = red[beg][0];
    tmp[1] = red[beg][1];
    red[beg][0] = red[pivot][0];
    red[beg][1] = red[pivot][1];
    red[pivot][0] = tmp[0];
    red[pivot][1] = tmp[1];
  }

  jug = red[beg][0];
  water = red[beg][1];
  i = beg;
  j = end;
  while (i < j)
  {
    while (i < j && red[j][1] > water)
      --j;
    red[i][0] = red[j][0];
    red[i][1] = red[j][1];

    while (i < j && red[i][1] < water)
      ++i;
    red[j][0] = red[i][0];
    red[j][1] = red[i][1];
  }
  red[i][0] = jug;
  red[i][1] = water;
  /* Now, 'red[0...i-1][1]' are smaller than 'red[i][1]',
   * and 'red[i...end][1]' are greater than 'red[i][1]'. */

  for (pivot = beg; pivot <= end; ++pivot)
    if (blue[pivot][1] == water)
      break;

  if (pivot != beg)
  {
    tmp[0] = blue[beg][0];
    tmp[1] = blue[beg][1];
    blue[beg][0] = blue[pivot][0];
    blue[beg][1] = blue[pivot][1];
    blue[pivot][0] = tmp[0];
    blue[pivot][1] = tmp[1];
  }
  jug = blue[beg][0];
  water = blue[beg][1];
  i = beg;
  j = end;
  while (i < j)
  {
    while (i < j && blue[j][1] > water)
      --j;
    blue[i][0] = blue[j][0];
    blue[i][1] = blue[j][1];

    while (i < j && blue[i][1] < water)
      ++i;
    blue[j][0] = blue[i][0];
    blue[j][1] = blue[i][1];
  }
  blue[i][0] = jug;
  blue[i][1] = water;
  /* Now, 'blue[0...i-1][1]' are smaller than 'blue[i][1]',
   * and 'blue[i...end][1]' are greater than 'blue[i][1]'. */

  /* Divide and conquer. */
  divide_and_conquer (red, blue, beg, i - 1);
  divide_and_conquer (red, blue, i + 1, end);
}

int main ()
{
  int (*red)[2], (*blue)[2];
  int tmp;
  int i, j;

  red = malloc (sizeof (int) * 2 * NUM);
  blue = malloc (sizeof (int) * 2 * NUM);
  /* Generate test data and print them. */
  for (i = 0; i < NUM; ++i)
  {
    red[i][0] = i;
    red[i][1] = i;
    blue[i][0] = i;
    blue[i][1] = i;
  }
  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
  {
    j = rand () % (NUM - i) + i;
    tmp = red[i][1];
    red[i][1] = red[j][1];
    red[j][1] = tmp;

    j = rand () % (NUM - i) + i;
    tmp = blue[i][1];
    blue[i][1] = blue[j][1];
    blue[j][1] = tmp;
  }
  if (NUM <= 50)
  {
    for (i = 0; i < NUM; ++i)
      printf ("%d ", red[i][1]);
    printf ("\n");
    for (i = 0; i < NUM; ++i)
      printf ("%d ", blue[i][1]);
    printf ("\n");
  }

  long beg = clock ();
  /* Divide and conquer. */
  divide_and_conquer (red, blue, 0, NUM - 1);
  long end = clock ();
  printf ("Time: %ld\n", (end - beg) / 1000);

  /* Print the result. */
  if (NUM <= 50)
  {
    for (i = 0; i < NUM; ++i)
      printf ("%d ", red[i][1]);
    printf ("\n");
    for (i = 0; i < NUM; ++i)
      printf ("%d ", blue[i][1]);
    printf ("\n");
  }

  /* Free the memory. */
  free (red);
  free (blue);

  return 0;
}
