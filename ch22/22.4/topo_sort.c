#include <stdio.h>

#define NUM 5

int graph[NUM][NUM] = { 
                        { 0, 1, 1, 1, 0 },
                        { 0, 0, 0, 1, 1 },
                        { 0, 0, 0, 0, 1 },
                        { 0, 0, 1, 0, 1 },
                        { 0, 0, 0, 0, 0 }
                      };

int visited[NUM] = { 0 };         
int result[NUM] = { 0 }, ptr = 0;

void dfsVisit (int vertex)
{
  int i;

  visited[vertex] = 1;
  for (i = 0; i < NUM; ++i)
    if (graph[vertex][i] && !visited[i])
      dfsVisit(i);
  result[ptr++] = vertex;
}

void topoSort ()
{
  int i;

  for (i = 0; i < NUM; ++i)
    if (!visited[i])
      dfsVisit (i);

  for (i = NUM - 1; i > -1; --i)
    printf ("%d ", result[i]);
  printf ("\n");
}

int main ()
{
  topoSort ();

  return 0;
}