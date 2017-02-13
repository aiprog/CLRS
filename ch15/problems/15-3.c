#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>

#define DISTANCE(x, y) sqrt((position[x][0] - position[y][0]) *	\
    (position[x][0] - position[y][0]) + (position[x][1] - position[y][1]) * \
    (position[x][1] - position[y][1]))

void betsp (int (*position)[2], int size)
{
  double *memo;
  int *solu, *path;
  int i, j, flag;
  double temp, dis;

  memo = malloc (sizeof (double) * size);
  solu = malloc (sizeof (int) * size);
  path = malloc (sizeof (int) * size);

  memo[0] = solu[0] = 0;
  memo[1] = 2 * DISTANCE (0, 1);
  solu[1] = 0;

  for (i = 2; i < size; ++i)
  {
    memo[i] = DBL_MAX;
    dis = 0;
    for (j = i - 2; j >= 0; --j)
    {
      dis += DISTANCE (j + 1, j + 2);
      temp = memo[j + 1] - DISTANCE (j, j + 1) + DISTANCE (j, i) + dis;
      if (temp < memo[i])
      {
	memo[i] = temp;
	solu[i] = j;
      }
    }
  }

  printf ("Min: %lf\n", memo[size - 1]);

  flag = -1;
  memset (path, 0, sizeof (int) * size);
  for (i = size - 1; i != 1; i = solu[i] + 1)
  {
    path[solu[i]] = path[i] = flag;
    if (flag == 1)
    {
      for (j = solu[i] + 1; j < i; ++j)
        path[j] = -1;
    }

    flag = -flag;
  }

  printf ("Path: 0->");
  for (i = 1; i < size - 1; ++i)
  {
    if (path[i] == -1)
      printf ("%d->", i);
  }
  printf ("%d->", size - 1);
  for (i = size - 2; i > 0; --i)
  {
    if (path[i] != -1)
      printf ("%d->", i);
  }
  printf ("0\n");
}

int main ()
{
  int position[9][2] = { { 0, 6 }, { 1, 0 }, { 2, 3 }, { 3, 3 }, { 4, 3 },
                         { 5, 4 }, { 6, 1 }, { 7, 5 }, { 8, 2 } 
                       };

  betsp (position, 9);

  return 0;
}
