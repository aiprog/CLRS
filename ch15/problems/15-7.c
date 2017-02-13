#include <stdio.h>
#include <stdlib.h>

#define SIZE 5
#define	LEN  2

int graph[SIZE][SIZE] = { { -1,  4,  4,  3, -1 },
                          { -1, -1, -1, -1,  5 },
			  { -1,  5, -1,  6,  5 },
			  { -1, -1, -1, -1,  5 },
			  { -1, -1, -1, -1, -1 }
                        };

double pro[SIZE][SIZE] = { { -1, 0.2, 0.3, 0.5, -1 },
                           { -1,  -1,  -1,  -1, 1.0 },
			   { -1, 0.7,  -1, 0.1, 0.2 },
			   { -1,  -1,  -1,  -1, 1.0 },
			   { -1,  -1,  -1,  -1, -1 }
			 };

int sound[LEN] = { 4, 5 };

double (*memo)[SIZE];
int (*solu)[SIZE];


void print_solu (int vertex, int pos)
{
  if (pos >= LEN)
    return;

  printf ("->A%d", solu[pos][vertex]);
  print_solu (solu[pos][vertex], pos + 1);
}

void va_sub (int vertex, int pos)
{
  double temp;
  int i;

  memo[pos][vertex] = -1;
  for (i = 0; i < SIZE; ++i)
  {
    if (graph[vertex][i] == sound[pos])
    {
      if (memo[pos + 1][i] == 0)
	va_sub (i, pos + 1);
      if (memo[pos + 1][i] != -1)
      {
	temp = pro[vertex][i] * memo[pos + 1][i];
	if (temp > memo[pos][vertex])
	{
	  memo[pos][vertex] = temp;
	  solu[pos][vertex] = i;
	}
      }
    }
  }
}

void va (int vertex)
{
  int i, j;

  memo = malloc (sizeof (double) * LEN * SIZE);
  solu = malloc (sizeof (int) * LEN * SIZE);

  for (i = 0; i < SIZE; ++i)
  {
    for (j = 0; j < LEN - 1; ++j)
      memo[j][i] = 0;

    memo[LEN - 1][i] = -1;
    for (j = 0; j < SIZE; ++j)
    {
      if (graph[i][j] == sound[LEN - 1] && pro[i][j] > memo[LEN - 1][i])
      {
	memo[LEN - 1][i] = pro[i][j];
	solu[LEN - 1][i] = j;	
      }
    }
  }

  va_sub (vertex, 0);

  if (memo[0][vertex] != -1)
  {
    printf ("Max: %lf\n", memo[0][vertex]);
    printf ("Paht: A%d", vertex);
    print_solu (0, 0);
    printf ("\n");
  }
  else
    printf ("Path not found!\n");
}

int main ()
{
  va (0);

  return 0;
}
