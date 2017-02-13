#include <stdio.h>
#include <stdlib.h>

int binary_search (int (*activity)[3], int end, int key)
{
  int i, j, k;

  i = 0;
  j = end;
  while (1)
  {
    k = (i + j) / 2;
    if (activity[k][1] <= key && activity[k + 1][1] > key)
      return k;
    else if (activity[k][1] > key)
      j = k;
    else
      i = k + 1;
  }
}

void print_solu (int (*Solu)[2], int pos)
{
  int (*solu)[2] = Solu;

  if (pos == -1)
    return;

  print_solu (solu, solu[pos][1]);
  if (solu[pos][0])
    printf (" %d", pos + 1);
}

void asp (int (*activity)[3], int size)
{
  int *memo, (*solu)[2];
  int i, j;
  int temp;

  memo = malloc (sizeof (int) * size);
  solu = malloc (sizeof (int) * size * 2);

  memo[0] = activity[0][2];
  solu[0][0] = 1;
  solu[0][1] = -1;

  for (i = 1; i < size; ++i)
  {
    if (activity[i][0] >= activity[i - 1][1])
    {
      memo[i] = memo[i - 1] + activity[i][2];
      solu[i][0] = 1;
      solu[i][1] = i - 1;
    }
    else if (activity[i][0] < activity[0][1])
    {
      memo[i] = activity[i][2];
      solu[i][0] = 1;
      solu[i][1] = -1;
    }
    else 
    {
      memo[i] = memo[i - 1];
      solu[i][0] = 0;
      solu[i][1] = i - 1;

      j = binary_search (activity, i - 1, activity[i][0]);
      temp = memo[j] + activity[i][2];
      if (temp > memo[i])
      {
	memo[i] = temp;
	solu[i][0] = 1;
	solu[i][1] = j;
      }
    }
  }

  printf ("Max: %d\n", memo[size - 1]);
  printf ("Solu:");
  print_solu (solu, size - 1);
  printf ("\n");
}

int main ()
{
  int activity[7][3] = { { 1, 4, 1 }, { 2, 4, 2 }, { 4, 6, 3 }, { 5, 7, 2 },
                         { 6, 7, 2 }, { 4, 8, 10 }, { 7, 8, 3 }
		       };

  asp (activity, 7);

  return 0;
}
