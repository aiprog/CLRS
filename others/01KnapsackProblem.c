#include <stdlib.h>
#include <stdio.h>

void knapsackProblem (int capacity, int *weight, int *value, int num)
{
  int (*table)[capacity + 1] = malloc (sizeof (int) * num * (capacity + 1));
  int i, j;

  // Initialize F[1][0] to F[1][c]
  for (i = 0; i <= capacity; ++i)
  {
    if (i <= weight[0])
      table[0][i] = 0;
    else
      table[0][i] = value[0];
  }

  // F[i][j] = MAX { vi + F[i - 1][j - wi], F[i - 1][j] }
  for (i = 1; i < num; ++i)
  {
    for (j = 0; j <= capacity; ++j)
    {
      if (j >= weight[i] && 
          table[i - 1][j - weight[i]] + value[i] > table[i - 1][j])
        table[i][j] = table[i - 1][j - weight[i]] + value[i];
      else
        table[i][j] = table[i - 1][j];
    }
  }

  i = num - 1;
  j = capacity;

  // Print the selection
  do
  {
    if (table[i][j] != table[i - 1][j])
    {
      j -= weight[i];
      printf ("%d ", value[i]);
    }

    --i;
  } while (i > 0);

  printf ("\nTotal value: %d\n", table[num - 1][capacity]);

  free (table);
}

int main ()
{
  int weight[5] = { 8, 6, 3, 2, 4 }, value[5] = { 14, 8, 9, 2, 7 };

  knapsackProblem (10, weight, value, 5);

  return 0;
}