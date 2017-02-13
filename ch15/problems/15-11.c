#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define	COST(x)		(1 * (x))
#define	MIN(x, y) 	((x) < (y)? (x): (y))
#define	MAX(x, y)	((x) > (y)? (x): (y))

void print_solu (int *demand, int month, int total_demand, int product,
                 void *Solu, int ite, int unsold)
{
  int (*solu)[total_demand] = Solu;
  int temp;

  if (ite == 0)
  {
    printf (" %d", solu[ite][unsold]);
    return;
  }

  temp = unsold;
  unsold = unsold + demand[ite] - solu[ite][unsold];
  print_solu (demand, month, total_demand, product, solu, ite - 1, unsold);
  printf (" %d", solu[ite][temp]);
}

void ip (int *demand, int month, int product, int cost)
{
  int total_demand, future_demand;
  int i, j, k;
  int lower, upper, unsold_min, unsold_max, temp;

  total_demand = 0;
  for (i = 0; i < month; ++i)
    total_demand += demand[i];

  int (*memo)[total_demand] = malloc (sizeof (int) * total_demand * month);
  int (*solu)[total_demand] = malloc (sizeof (int) * total_demand * month);

  future_demand = total_demand - demand[0];
  upper = MIN (future_demand - product, MAX (0, product - demand[0]));
  upper = MAX (0, upper);
  for (i = 0; i <= upper; ++i)
  {
    memo[0][i] = COST (i) + cost * MAX (0, demand[0] - product);
    solu[0][i] = i + demand[0];
  }
  unsold_min = 0;
  unsold_max = upper;

  for (i = 1; i < month; ++i)
  {
    future_demand -= demand[i];
    if (unsold_max + product <= demand[i])
    {
      memo[i][0] = INT_MAX;
      for (j = unsold_min; j <= unsold_max; ++j)
      {
	temp = memo[i - 1][j] + cost * (demand[i] - j - product);
	if (temp < memo[i][0])
	{
	  memo[i][0] = temp;
	  solu[i][0] = demand[i] - j;
	}
      }
      unsold_min = unsold_max = 0;
    }
    else
    {
      lower = MAX (0, unsold_max - demand[i]);
      upper = MIN (future_demand - product, unsold_max + product - demand[i]);
      upper = MAX (0, upper);
      for (j = lower; j <= upper; ++j)
      {
        memo[i][j] = INT_MAX;
	if (j == 0)
	{
	  for (k = unsold_min; k <= MIN (unsold_max, demand[i]); ++k)
	  {
	    temp = memo[i - 1][k] + cost * MAX (0, demand[i] - product - k);
	    if (temp < memo[i][j])
	    {
	      memo[i][j] = temp;
	      solu[i][j] = demand[i] - k;
	    }
	  }
	}
	else
	{
          for (k = MAX (unsold_min, demand[i] + j - product); 
	       k <= MIN (unsold_max, demand[i] + j); ++k)
          {
	    temp = memo[i - 1][k] + COST (j);
	    if (temp < memo[i][j])
	    {
	      memo[i][j] = temp;
	      solu[i][j] = j + demand[i] - k;
	    }
          }
        }
      }
      unsold_min = lower;
      unsold_max = upper;
    }
  }
  
  printf ("Min: %d\n", memo[month - 1][0]);
  printf ("Product: ");
  print_solu (demand, month, total_demand, product, solu, month - 1, 0);
  printf ("\n");
}

int main ()
{
  int demand[5] = { 2, 1, 4, 3, 5 };

  ip (demand, 5, 3, 3);

  return 0;
}
