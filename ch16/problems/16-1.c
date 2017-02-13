#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void coin_changing (int money, int *coins, int size)
{
  int *memo, *solu;
  int i, j;

  memo = malloc (sizeof (int) * (money + 1));
  solu = malloc (sizeof (int) * (money + 1));
  for (i = 1; i <= money; ++i)
  {
    memo[i] = INT_MAX;
  }
  memo[0] = 0;

  for (i = 0; i < money; ++i)
  {
    if (memo[i] == INT_MAX)
      continue;

    for (j = 0; j < size; ++j)
    {
      if (i + coins[j] <= money && memo[i] + 1 < memo[i + coins[j]])
      {
        memo[i + coins[j]] = memo[i] + 1;
        solu[i + coins[j]] = coins[j];
      }
    }
  }

  if (memo[money] != INT_MAX)
  {
    i = money;
    while (i != 0)
    {
      printf ("%d ", solu[i]);
      i -= solu[i];
    }
    printf ("\n");
  }
  else
    printf ("No solution.\n");

  free (memo);
  free (solu);
}

int main ()
{
  int coins[3] = { 5, 10, 15 };

  coin_changing (61, coins, 3);

  return 0;
}