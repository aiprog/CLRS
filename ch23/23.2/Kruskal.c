#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM 5

typedef struct _node
{
  int parent;
  int rank;
} Node;

int graph[NUM][NUM] = { 
                        { 0, 1, 2, 0, 0 }, 
                        { 1, 0, 2, 1, 2 }, 
                        { 2, 2, 0, 0, 4 }, 
                        { 0, 1, 0, 0, 3 }, 
                        { 0, 2, 4, 3, 0 }
                      };

Node set[NUM];      /* Use array to store trees. */
int edgeNum = 0;
int (*edge)[2];
int (*temp)[2];

/* Make NUM one node trees. */
void makeSet ()
{
  int i;

  for (i = 0; i < NUM; ++i)
  {
    set[i].parent = -1;   /* Root nodes. */
    set[i].rank = 1;
  }
}

/* Unite two trees by their root nodes. */
void linkSet (int node1, int node2)
{
  /* Union by rank. */
  if (set[node1].rank <= set[node2].rank)
  {
    set[node1].parent = node2;
    if (set[node1].rank == set[node2].rank)
      ++set[node2].rank;
  }
  else
    set[node2].parent = node1;
}

/* Find root nodes. */
int findSet (int node)
{
  if (set[node].parent == -1)
    return node;

  /* Path compression. */
  return set[node].parent = findSet (set[node].parent);
}

/* Unite two tree. */
void unionSet (int node1, int node2)
{
  linkSet (findSet (node1), findSet (node2));
}

void divideAndMergeSort (int beg, int end)
{
  int mid, i, j, k;

  if (beg == end)
    return;

  mid = (beg + end) / 2;
  divideAndMergeSort (beg, mid);
  divideAndMergeSort (mid + 1, end);

  i = beg; j = mid + 1; k = 0;
  while (i <= mid && j <= end)
  {
    if (graph[edge[i][0]][edge[i][1]] <= graph[edge[j][0]][edge[j][1]])
    {
      temp[k][0] = edge[i][0];
      temp[k][1] = edge[i][1];

      ++i;
    }
    else
    {
      temp[k][0] = edge[j][0];
      temp[k][1] = edge[j][1];

      ++j;
    }

    ++k;
  }

  if (i <= mid)
    memcpy (&temp[k], edge[i], sizeof (int) * 2 * (mid - i + 1));
  else
    memcpy (&temp[k], edge[j], sizeof (int) * 2 * (end - j + 1));
  memcpy (&edge[beg], temp, sizeof (int) * 2 * (end - beg + 1));
}

int main ()
{
  int set1, set2, cnt, i, j;

  /* Calculate the number of edges. */  
  for (i = 0; i < NUM; ++i)
    for (j = i; j < NUM; ++j)
    {
      if (graph[i][j])
        ++edgeNum;
    }

  edge = (int (*)[2])malloc (sizeof (int) * 2 * edgeNum);
  temp = (int (*)[2])malloc (sizeof (int) * 2 * edgeNum);

  /* Store all edges. */
  for (i = 0; i < NUM; ++i)
    for (j = i; j < NUM; ++j)
    {
      if (graph[i][j])
      {
        edge[k][0] = i;
        edge[k][1] = j;
      }
    }

  /* Sort edges into nondecreasiing order by weight. */
  divideAndMergeSort (0, edgeNum - 1);

  makeSet ();

  /* Try to add every edge into the edges set of a minimum-spanning-tree. */ 
  cnt = 0;
  for (i = 0; cnt != NUM - 1 && i < edgeNum; ++i)
  {
    set1 = findSet (edge[i][0]);
    set2 = findSet (edge[i][1]);

    /* Two vertices belong to different sets. */
    if (set1 != set2)
    {
      /* Unites those two sets and add it to the edges set. */
      unionSet (set1, set2);
      printf ("Edge: %d->%d\n", edge[i][0], edge[i][1]);

      ++cnt;
    }
  }

  return 0;
}