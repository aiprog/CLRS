#include <stdio.h>
#include <string.h>

#define NUM 8

int graph[NUM][NUM] = {
                        { 0, 1, 1, 0, 0, 0, 0, 0 },
                        { 1, 0, 1, 0, 0, 0, 0, 0 },
                        { 1, 1, 0, 0, 0, 1, 0, 0 },
                        { 0, 0, 0, 0, 1, 1, 0, 0 },
                        { 0, 0, 0, 1, 0, 1, 0, 0 },
                        { 0, 0, 1, 1, 1, 0, 1, 1 },
                        { 0, 0, 0, 0, 0, 1, 0, 1 },
                        { 0, 0, 0, 0, 0, 1, 1, 0 }
                      };

int discover[NUM] = { 0 };
int discoverFlag[NUM] = { 0 };
int finishFlag[NUM] = { 0 };
int low[NUM] = { 0 };
int timeStamp = 0;
int biCompNum = 0;

void getArtiBridge(int vertex, int parent)
{
  int childNum = 0, artiFlag = 0, i;

  low[vertex] = discover[vertex] = ++timeStamp;
  for (i = 0; i < NUM; ++i)
    if (i == parent)          /* Don't mistake tree edges for back edges! */   
      continue;
    else if (graph[vertex][i] && !finishFlag[i])
    {
      if (!discover[i])       /* DFS tree edges. */
      {
        ++childNum;
        getArtiBridge (i, vertex);

        /* Does this subtree has a back edge to its father's ancensters? */
        if (low[i] >= discover[vertex])
          artiFlag = 1; 

        /* Does this subtree has a back edge to its father? */
        if (low[i] > discover[vertex])
          printf ("Bridge: %d->%d\n", vertex, i);

        /* Update low time of this vertex. */
        if (low[i] < low[vertex])
          low[vertex] = low[i];
      }
      else if (discover[i] < low[vertex])  /* DFS back edges. */
        low[vertex] = discover[i];         /* Update low time of this vertex. */
    }

  /* Print articulation points of root vertexes and nonroot vertexes. */
  if (parent == -1 && childNum > 1 || parent != -1 && artiFlag)
    printf ("Articulation point: %d\n", vertex);

  finishFlag[vertex] = 1;
}

void getBiconComponent(int vertex, int parent, int num)
{
  int i;

  discoverFlag[vertex] = 1;

  for (i = 0; i < NUM; ++i)
    if (i == parent)
      continue;
    else if (graph[vertex][i] && !finishFlag[i])
    {
      if (!discoverFlag[i])
      {
        if (low[i] < discover[vertex])
        {
          printf ("Biconnected component %d: %d->%d\n", num, vertex, i);
          getBiconComponent (i, vertex, num);
        }
        else if (low[i] == discover[vertex])
        {
          printf ("Biconnected component %d: %d->%d\n", ++biCompNum, vertex, i);
          getBiconComponent (i, vertex, biCompNum);
        }
        else
          getBiconComponent (i, vertex, ++biCompNum);
      }
      else
        printf ("Biconnected component %d: %d->%d\n", num, vertex, i);
    }

  finishFlag[vertex] = 1;
}

int main ()
{
  getArtiBridge (1, -1);
  memset (finishFlag, 0, sizeof (int) * NUM);
  getBiconComponent (1, -1, 0);

  return 0;
}


