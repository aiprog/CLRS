#include <stdio.h>
#include <limits.h>

#define NUM 5

typedef struct _node
{
  int peer;       /* 'peer' and this node connect VS with V - VS. */
  int weight;     /* The current minimum weight. */ 
  int select;     /* Whether it is added to VS. */
} Node;

Node nodes[NUM];

int graph[NUM][NUM] = { 
                        { 0, 1, 2, 0, 0 }, 
                        { 1, 0, 2, 1, 2 }, 
                        { 2, 2, 0, 0, 4 }, 
                        { 0, 1, 0, 0, 3 }, 
                        { 0, 2, 4, 3, 0 }
                      };

void Prim (int root)
{
  int minNode, weight, cnt, i;

  for (i = 0; i < NUM; ++i)
  {
    nodes[i].peer = -1;
    nodes[i].weight = INT_MAX;
    nodes[i].select = 0;
  }

  /* Start from 'root'. */
  minNode = root; cnt = 1;
  do
  {
    /* Add miniNode to VS, and update the minimum  weight of vertices in 
       V - VS. */
    nodes[minNode].select = 1;
    for (i = 0; i < NUM; ++i)
    {
      weight = graph[minNode][i];
      if (!nodes[i].select && weight && weight < nodes[i].weight)
      {
        nodes[i].peer = minNode;
        nodes[i].weight = weight;
      }
    }

    /* Choose the vertex having a minimum weight edge connecting VS with V - VS
       in V - VS. Add the vertex and the edge to VS and ES respectively. */ 
    minNode = -1; weight = INT_MAX;
    for (i = 0; i < NUM; ++i)
    {
      if (!nodes[i].select && nodes[i].weight < weight)
      {
        minNode = i;
        weight = nodes[i].weight;
      }
    }
    printf ("Edge: %d->%d\n", minNode, nodes[minNode].peer);
  } while (++cnt < NUM);
}

int main ()
{
  Prim (4);

  return 0;
}