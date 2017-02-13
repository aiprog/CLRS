#include <stdio.h>
#include <stdlib.h>

void print_solu (int budget, void *Cost, void *Solu, 
                 int candidate, int position, int min_cost)
{
  int temp;

  int (*cost)[candidate] = Cost, (*solu)[budget + 1] = Solu;
  if (position == 0)
  {
    printf (" %d", solu[0][min_cost]);
    return;
  }

  if (solu[position][min_cost] == -1)
    temp = min_cost;
  else
    temp = min_cost - cost[position][solu[position][min_cost]];

  print_solu (budget, cost, solu, candidate, position - 1, temp); 
  printf (" %d", solu[position][min_cost]);
}

void sfabp (int budget, void *Vorp, void *Cost, int position, int candidate)
{
  int (*memo)[budget + 1], (*solu)[budget + 1];
  int (*vorp)[candidate] = Vorp, (*cost)[candidate] = Cost;
  int i, j, k;
  int max_vorp, min_cost, temp;

  memo = malloc (sizeof (int) * (budget + 1) * position);
  solu = malloc (sizeof (int) * (budget + 1) * position);

  for (i = 0; i < position; ++i)
  {
    for (j = 0; j <= budget; ++j)
      memo[i][j] = -1;
  }

  memo[0][0] = 0;
  solu[0][0] = -1;
  for (i = 0; i < candidate; ++i)
  {
    if (cost[0][i] <= budget)
    {
      memo[0][cost[0][i]] = vorp[0][i];
      solu[0][cost[0][i]] = i;
    }
  }

  for (i = 0; i < position - 1; ++i)
  {
    for (j = 0; j <= budget; ++j)
    {
      if (memo[i][j] == -1)
	continue;

      memo[i + 1][j] = memo[i][j];
      solu[i + 1][j] = -1;

      for (k = 0; k < candidate; ++k)
      {
	temp = j + cost[i + 1][k];
	if (temp > budget)
	  continue;

	if (memo[i][j] + vorp[i + 1][k] > memo[i + 1][temp])
	{
	  memo[i + 1][temp] = memo[i][j] + vorp[i + 1][k];
	  solu[i + 1][temp] = k;
	}
      }
    }
  }

  max_vorp = -1;
  for (i = 0; i <= budget; ++i)
  {
    if (memo[position - 1][i] > max_vorp)
    {
      max_vorp = memo[position - 1][i];
      min_cost = i;
    }
  }

  printf ("Max VORP: %d \nCost: %d\n", max_vorp, min_cost);
  printf ("Solution:");
  print_solu (budget, cost, solu, candidate, position - 1, min_cost);
  printf ("\n");
}

int main ()
{
  int vorp[3][3] = { { 1, 2, 3 },
                     { 1, 2, 3 },
		     { 1, 2, 3 }
		   };
  int cost[3][3] = { { 2, 3, 5 },
                     { 1, 3, 4 },
		     { 3, 5, 6 }
                   };

  sfabp (1, vorp, cost, 3, 3);

  return 0;
}
