#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void print_solution (void *solu, int size, int beg, int end)
{
  int split;

  if (beg == end)
    printf ("A%d", beg + 1);
  else
  {
    printf ("(");
    split = ((int (*)[size]) solu)[beg][end - beg - 1];
    print_solution (solu, size, beg, split);
    print_solution (solu, size, split + 1, end);
    printf (")");
  }
}

void *matrix_chain_order (int *dime, int size)
{
  int (*memo)[size], (*solu)[size - 1];
  int i, j, k;
  int s, t;
  int temp;

  memo = malloc (sizeof (int) * size * size);
  solu = malloc (sizeof (int) * (size - 1) * (size - 1));

  for (i = 0; i < size; ++i)
    memo[i][0] = 0;

  for (i = 1; i < size; ++i)
  {
    for (j = 0; j < size - i; ++j)
    {
      s = i - 1;
      t = j + 1;
      memo[j][i] = INT_MAX;                                                     
      for (k = j; k < j + i; ++k)
      {
	temp = memo[j][k - j] + memo[t][s] +  
	       dime[j] * dime[k + 1] * dime[j + i + 1];
	if (temp < memo[j][i])
	{
	  memo[j][i] = temp;
	  solu[j][i - 1] = k;
	}

	--s;
	++t;
      }
    }
  }

  print_solution (solu, size - 1, 0, size - 1);
  printf ("\n");

  return solu;
}

void *matrix_multi (void **matrixs, int *dime, int size, 
                    void *solu, int beg, int end)
{
  int row1, col1, col2;
  int split;
  int i, j, k;

  if (beg == end)
    return matrixs[beg];
  
  split = ((int (*)[size]) solu)[beg][end - beg - 1];
  row1 = dime[beg];
  col1 = dime[split + 1];
  col2 = dime[end + 1];

  int (*matrix1)[col1] = matrix_multi (matrixs, dime, size, 
                                       solu, beg, split);
  int (*matrix2)[col2] = matrix_multi (matrixs, dime, size, 
                                       solu, split + 1, end);
  int (*matrix3)[col2] = malloc (sizeof (int) * row1 * col2);

  for (i = 0; i < row1; ++i)
  {
    for (j = 0; j < col2; ++j)
    {
      for (k = 0; k < col1; ++k)
	matrix3[i][j] += matrix1[i][k] * matrix2[k][j];
    }
  }

  return matrix3;
}

int main ()
{
  int dime[5] = { 2, 3, 2, 2, 4};
  int matrix1[2][3] = { 1, 1, 1, 1, 1, 1 };
  int matrix2[3][2] = { 1, 1, 1, 1, 1, 1 };
  int matrix3[2][2] = { 1, 1, 1, 1 };
  int matrix4[2][4] = { 1, 2, 3, 4, 1, 2, 3, 4 };
  void *matrixs[4];
  int row, col;
  int i, j;

  matrixs[0] = matrix1;
  matrixs[1] = matrix2;
  matrixs[2] = matrix3;
  matrixs[3] = matrix4;
  row = dime[0];
  col = dime[4];
  int (*matrix)[col] = matrix_multi (matrixs, dime, 4, 
                                     matrix_chain_order (dime, 4), 0, 3);

  for (i = 0; i < row; ++i)
  {
    for (j = 0; j < col; ++j)
      printf ("%d\t", matrix[i][j]);
    printf ("\n");
  }

  return 0;
}
