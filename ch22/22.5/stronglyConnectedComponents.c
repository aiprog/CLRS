#include <stdio.h>
#include <string.h>

#define NUM 5

int graph[NUM][NUM] = { 
                        { 0, 1, 1, 0, 0 },
                        { 0, 0, 0, 1, 0 },
                        { 0, 0, 0, 0, 1 },
                        { 1, 0, 0, 0, 1 },
                        { 0, 0, 1, 0, 0 }
                      };
int reversed[NUM][NUM] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };

int visited[NUM] = { 0 };
int order[NUM] = { 0 }, ptr = NUM;

void dfsVisit (int (*Graph)[NUM], int vertex, int printFlag)
{
  int i;

  visited[vertex] = 1;
  for (i = 0; i < NUM; ++i)
    if (Graph[vertex][i] && !visited[i])
      dfsVisit (Graph, i, printFlag);
  order[--ptr] = vertex;

  if (printFlag)
    printf ("%d ", vertex);
}

void finishOrder ()
{
  int i;

  for (i = 0; i < NUM; ++i)
    if (!visited[i])
      dfsVisit (graph, i, 0);
}

void getSCC ()
{
  int i, j;

  /* Computer the finishing time order. */
  finishOrder ();

  /* Reverse the original graph. */
  for (i = 0; i < NUM; ++i)
    for (j = 0; j < NUM; ++j)
      if (graph[i][j])
        reversed[j][i] = 1;

  /* Perform DFS on reversed graph in the finishing time order. */
  memset (visited, 0, sizeof (int) * NUM);
  for (i = 0; i < NUM; ++i)
    if (!visited[order[i]])
    {
      dfsVisit (reversed, order[i], 1);
      printf ("\n");
    }
}

int main ()
{
  getSCC ();

  return 0;
}