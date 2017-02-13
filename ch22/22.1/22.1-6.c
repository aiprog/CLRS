#include <stdio.h>

int findUniversialSink (int *Graph, int num)
{
  int (*graph)[num] = (int (*)[num]) Graph, cur;
  int i;

  cur = 0;  /* Keep the candidate vertex. */
  for (i = 1; i < num; ++i)
  {
    if (graph[cur][i])
      cur = i;
  }

  /* Verification. */
  for (i = 0; i < num; ++i)
  {
    if (i != cur && (graph[cur][i] || !graph[i][cur]))
      return -1;
  }

  return cur;
}

int main ()
{
  int graph[4][4] = 
  {
    { 0, 1, 1, 1 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 0 },
    { 1, 1, 1, 0 }
  };

  printf ("%d\n", findUniversialSink ((int *) graph, 4));

  return 0;
}