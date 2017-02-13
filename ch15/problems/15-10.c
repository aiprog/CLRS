#include <stdio.h>
#include <stdlib.h>

void print_solu (void *Solu, int kind, int cur_year, int cur_kind)
{
  int (*solu)[kind] = Solu;

  if (cur_year == 1)
  {
    printf (" %d", solu[cur_year][cur_kind]);
    return;
  }

  print_solu (solu, kind, cur_year - 1, solu[cur_year][cur_kind]);
  printf (" %d", solu[cur_year][cur_kind]);
}

void pais (void *Rate, int year, int kind, 
           int fee_static, int fee_dynamic, int bonus)
{
  double (*rate)[kind] = Rate, (*memo)[kind];
  int i, j, k;
  int (*solu)[kind], max_kind;
  double max_bonus, temp;

  memo = malloc (sizeof (double) * kind * year);
  solu = malloc (sizeof (int) * kind * year);

  for (i = 0; i < year; ++i)
    memo[0][i] = bonus * rate[0][i];

  for (i = 1; i < year; ++i)
  {
    for (j = 0; j < kind; ++j)
      memo[i][j] = -1;
  }

  for (i = 0; i < year - 1; ++i)
  {
    for (j = 0; j < kind; ++j)
    {
      for (k = 0; k < kind; ++k)
      {
	if (k == j)
	  temp = (memo[i][j] - fee_static) * rate[i + 1][k];
	else
	  temp = (memo[i][j] - fee_dynamic) * rate[i + 1][k];

	if (temp > memo[i + 1][j])
	{
	  memo[i + 1][k] = temp;
	  solu[i + 1][k] = j;
	}
      }
    }
  }

  max_bonus = -1;
  for (i = 0; i < kind; ++i)
  {
    if (memo[year - 1][i] > max_bonus)
    {
      max_bonus = memo[year - 1][i];
      max_kind = i;
    }
  }

  printf ("Max: %lf\n", max_bonus);
  printf ("Solu:");
  print_solu (solu, kind, year - 1, max_kind);
  printf (" %d\n", max_kind);
}

int main ()
{
  double rate[3][3] = { { 1.1, 1.2, 1.3 },
                        { 1.2, 1.1, 1.2 },
			{ 1.2, 1.3, 1.1 }
		      };
  
  pais (rate, 3, 3, 10, 40, 100);

  return 0;
}
