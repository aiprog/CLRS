#include <stdio.h>

#define NUM 5

int graph[NUM][NUM] = { 
                        { 0, 1, 1, 1, 0 },
                        { 0, 0, 0, 1, 1 },
                        { 0, 0, 0, 0, 1 },
                        { 0, 0, 1, 0, 1 },
                        { 0, 0, 0, 0, 0 }
                      };

int inDegree[NUM] = { 0 };
int zeroDegreeVertex[NUM] = { 0 }, ptr = -1;

void topoSort ()
{
  int i, j;

  /* Get vertexes' in-degrees. */
  for (i = 0; i < NUM; ++i)     
    for (j = 0; j < NUM; ++j)
      if (graph[i][j])
        ++inDegree[j];

  /* Save vertexes with 0 in-degree. */ 
  for (i = 0; i < NUM; ++i)     
    if (!inDegree[i])
      zeroDegreeVertex[++ptr] = i;

  /* Remove a 0 in-degree vertex, decrease its adjacent vertexes' in-degree
     by 1 and save new 0 in-degree vertexes if necessary. */ 
  while (ptr != -1)                             
  {                                              
    printf ("%d ", j = zeroDegreeVertex[ptr--]);
    for (i = 0; i < NUM; ++i)
      if (graph[j][i] && !--inDegree[i])
        zeroDegreeVertex[++ptr] = i;
  }
  printf ("\n");
}

int main ()
{
  topoSort ();

  return 0;
}