#include <stdio.h>
#include <stdlib.h>

int price[10] = { 1, 5, 8, 9, 10, 17, 17, 20, 24, 30 };

void rod_cutting (int n)
{
  int *memo, *cutting;
  int revenue, new_revenue;
  int i, j;

  memo = malloc (sizeof (int) * (n + 1));
  memo[0] = 0;
  cutting = malloc (sizeof (int) * n);
  for (i = 1; i <= n; ++i)
  {
    revenue = 0;
    for (j = 1; j <= i; ++j)
    {
      new_revenue = price[j - 1] + memo[i - j];
      if (revenue < new_revenue)
      {
	revenue = new_revenue;
	cutting[i - 1] = j;
      }
    }
    memo[i] = revenue;
  }

  printf ("Pieces: ");
  i = n;
  while (i > 0)
  {
    printf ("%d ", cutting[i - 1]);
    i -= cutting[i - 1];
  }
  printf ("\nRevenue: %d\n", memo[n]);
}

int main ()
{
  rod_cutting (1);
  rod_cutting (4);
  rod_cutting (7);
  rod_cutting (10);

  return 0;
}
