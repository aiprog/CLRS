#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNKNOWN		-1
#define FORBIDDEN	-2

void print_solu (void *Solu, int size, int beg, int end)
{
  int (*solu)[size];

  solu = Solu;
  if (solu[beg][end] == end)
    return;

  printf ("%d ", solu[beg][end]);
  print_solu (solu, size, solu[beg][end], end);
}

void lsp_sub (void *Graph, int size, void *Memo, void *Solu, int beg, int end)
{
  int (*graph)[size], (*memo)[size], (*solu)[size];
  int i;
  int temp;

  graph = Graph;
  memo = Memo;
  solu = Solu;
  if (memo[beg][end] != UNKNOWN)
    return;

  for (i = 0; i < size; ++i)
  {
    if (i != beg && graph[beg][i] != UNKNOWN)
    {
      lsp_sub (graph, size, memo, solu, i, end);
      if (memo[i][end] != FORBIDDEN)
      {
	temp = graph[beg][i] + memo[i][end];
	if (temp > memo[beg][end])
	{
	  memo[beg][end] = temp;
	  solu[beg][end] = i;
	}
      }
    }
  }

  if (memo[beg][end] == UNKNOWN)
    memo[beg][end] = FORBIDDEN;
}

void lsp (void *graph, int size, int beg, int end)
{
  int (*memo)[size], (*solu)[size];
  int i, j;

  memo = malloc (sizeof (int) * size * size);
  solu = malloc (sizeof (int) * size * size);

  for (i = 0; i < size; ++i)
  {
    for (j = 0; j < size; ++j)
    {
      if (i != j)
	memo[i][j] = UNKNOWN;
      else
	memo[i][j] = 0;
    }
  }

  lsp_sub (graph, size, memo, solu, beg, end);
  if (memo[beg][end] != FORBIDDEN)
  {
    printf ("Max: %d\n", memo[beg][end]);
    printf ("Path: %d ", beg);
    print_solu (solu, size, beg, end);
    printf ("%d\n", end);
  }
  else
    printf ("No path found!\n");
}

int main ()
{
  int graph[6][6] = { { 0,  5,  4,  2, -1, -1 },
    		      {-1,  0, -1, -1, -1,  5 },
		      {-1, -1,  0, -1,  4, -1 },
		      {-1,  1,  1,  0,  3,  4 },
		      {-1, -1, -1, -1,  0,  3 },
		      {-1, -1, -1, -1, -1,  0 }
		    };

  lsp (graph, 6, 0, 5);

  return 0;
}
