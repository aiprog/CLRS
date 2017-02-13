#include <stdio.h>
#include <stdlib.h>

#define NUM 5

/* For sake of efficiency, use a list to store Euler tour. */
typedef struct node 
{
  int vertex;
  struct node *next;
} Node;

int graph[NUM][NUM] = {
                        { 0, 1, 0, 1, 0 },
                        { 1, 0, 1, 0, 0 },
                        { 1, 1, 0, 0, 1 },
                        { 0, 0, 1, 0, 1 },
                        { 0, 0, 1, 1, 0 }
                      };

/* Pointers to nodes for visited vertexes. */
Node *nodes[NUM];

Node *newNode (int vertex, Node *next)
{
  Node *node;

  node = malloc (sizeof (Node));
  node->vertex = vertex;
  node->next = next;

  return node;
}

int main ()
{
  Node *head, *ptr, *nd;
  int beg, path, i, j;

  beg = 0; /* Begin from vertex 0. Initial S1 is: 0. */
  nodes[0] = newNode (0, NULL);   /* The first node of S1. */
  head = newNode (-1, NULL);      /* The head node of S2. */

  do
  {
    path = beg;     /* Initial vertex for S2 is 0. */
    ptr = head;

    /* Traverse the graph to get S2. */
    do  
    {  
      for (i = 0; i < NUM; ++i)
      {
        if (graph[path][i]) 
        {
          graph[path][i] = 0;
          path = i;

          nd = newNode (i, NULL);
          if (!nodes[i])
            nodes[i] = nd;

          ptr->next = nd;
          ptr = ptr->next;

          break;
        }
      }
    } while (path != beg);

    /* Merge S1 and S2. */
    ptr->next = nodes[beg]->next;
    nodes[beg]->next = head->next;

    /* Find a new vertex to construct the new S2 if possible,
       otherwise we have found a Euler tour. */ 
    for (i = 0; i < NUM; ++i)
    {
      if (nodes[i])
      {
        for (j = 0; j < NUM; ++j)
          if (graph[i][j])
          {
            beg = i;
            break;
          }
      }

      if (j != NUM)
        break;
    }
  } while (i != NUM);

  /* Print S1, namely, Euler tour. */
  ptr = nodes[0];
  while (ptr)
  {
    printf ("%d", ptr->vertex);
    ptr = ptr->next;
  }
  printf ("\n");

  return 0;
}