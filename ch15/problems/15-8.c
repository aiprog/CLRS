#include <stdio.h>
#include <stdlib.h>

void print_solu (void *Disruption, void *Memo, int col, int c, int r)
{
  int (*disruption)[col] = Disruption, (*memo)[col] = Memo;
  int beg;
  int temp;

  if (c == 0)
  {
    printf ("%d ", r);
    return;
  }

  if (r != 0)
    beg = r - 1;
  else
    beg = 0;

  temp = memo[c][r] - disruption[c][r];
  while (1)
  {
    if (memo[c - 1][beg] == temp)
      break;
    ++beg;
  }

  print_solu (disruption, memo, col, c - 1, beg);
  printf ("%d ", r);
}

void icbsc (void *Disruption, int row, int col)
{
  int (*disruption)[col], (*memo)[col];
  int i, j;
  int temp;

  disruption = Disruption;
  memo = malloc (sizeof (int) * row * col);
  for (i = 0; i < col; ++i)
    memo[0][i] = disruption[0][i];

  for (i = 1; i < row; ++i)
  {
    temp = memo[i - 1][0];
    if (memo[i - 1][1] < temp)
      temp = memo[i - 1][1];
    memo[i][0] = temp + disruption[i][0];

    temp = memo[i - 1][col - 2];
    if (memo[i - 1][col - 1] < temp)
      temp = memo[i - 1][col - 1];
    memo[i][col - 1] = temp + disruption[i][col - 1];

    for (j = 1; j < col - 1; ++j)
    {
      temp = memo[i - 1][j - 1];
      if (memo[i - 1][j] < temp)
	temp = memo[i - 1][j];
      if (memo[i - 1][j + 1] < temp)
	temp = memo[i - 1][j + 1];
      memo[i][j] = temp + disruption[i][j];
    }
  }

  temp = 0;
  for (i = 1; i < col; ++i)
  {
    if (memo[row - 1][i] < memo[row - 1][temp])
      temp = i;
  }
  printf ("Min: %d\n", memo[row - 1][temp]);
  printf ("Position: ");
  print_solu (disruption, memo, col, row - 1, temp);
  printf ("\n");
}

int main ()
{
  int disruption[4][4] = { 
    			   { 0, 1, 1, 3 },
                           { 3, 2, 1, 4 },
			   { 2, 1, 3, 1 },
			   { 3, 3, 2, 2 }
			 };

  icbsc (disruption, 4, 4);

  return 0;
}
