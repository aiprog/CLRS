#include <cstdio>
#include <stack>

using std::stack;

#define SZ 8
#define MAX 100
#define MIN -100

typedef struct _node
{
  int minWeight = MAX;
  int predecessor = -1;
  int cycleWeight = MIN;
} Node;

void DFS (int (*graph)[SZ], Node *info, int node)
{
  if (info[node].minWeight != MIN)
  {
    info[node].minWeight = MIN;
    for (int i = 0; i < SZ; ++i)
      if (graph[node][i] != MAX)
        DFS (graph, info, i);
  }
}

bool findCycle (int (*graph)[SZ], Node *info, stack<int> &path, int node)
{
  for (int i = 0; i < SZ; ++i)
  {
    if (graph[node][i] != MAX && info[i].minWeight == MIN)
    {
      if (info[i].cycleWeight != MIN)
      {
        if (info[i].cycleWeight > info[node].cycleWeight + graph[node][i])
        {
          path.push (i);
          return true;
        }
      }
      else
      {
        path.push (i);
        info[i].cycleWeight = info[node].cycleWeight + graph[node][i];

        if (findCycle (graph, info, path, i))
          return true;
      }
    }
  }

  return false;
}

int main ()
{
  int graph[SZ][SZ] = {
                        MAX, 1, MAX, MAX, MAX, MAX, MAX, 1,
                        MAX, MAX, -1, MAX, MAX, MAX, MAX, MAX,
                        MAX, MAX, MAX, -1, MAX, MAX, MAX, MAX,
                        MAX, 1, MAX, MAX, 1, MAX, MAX, MAX,
                        MAX, MAX, MAX, MAX, MAX, MAX, MAX, MAX,
                        MAX, MAX, MAX, MAX, 1, MAX, -2, MAX,
                        MAX, MAX, MAX, MAX, MAX, MAX, MAX, MAX,
                        MAX, MAX, MAX, MAX, MAX, MAX, 1, MAX,
                       };

  Node *info = new Node[SZ];
  int src = 0;
  info[src].minWeight = 0;

  for (int n = 0; n < SZ - 1; ++n)
  {
    for (int i = 0; i < SZ; ++i)
      if (info[i].minWeight != MAX)
        for (int j = 0; j < SZ; ++j)
          if (graph[i][j] != MAX)
          {
            int weight = info[i].minWeight + graph[i][j];
            if (weight < info[j].minWeight)
            {
              info[j].minWeight = weight;
              info[j].predecessor = i;
            }
          }
  }

  for (int i = 0; i < SZ; ++i)
    if (info[i].minWeight != MAX && info[i].minWeight != MIN)
      for (int j = 0; j < SZ; ++j)
        if (graph[i][j] != MAX && info[j].minWeight != MIN &&
            info[j].minWeight > info[i].minWeight + graph[i][j])
          DFS (graph, info, j);

  for (int i = 0; i < SZ; ++i)
  {
    if (info[i].minWeight == MAX)
      printf ("MAX ");
    else if (info[i].minWeight == MIN)
      printf ("MIN ");
    else
      printf ("%d ", info[i].minWeight);
  }
  printf ("\n");

  stack<int> path;
  for (int i = 0; i < SZ; ++i)
  {
    if (info[i].minWeight == MIN)
    {
      info[i].cycleWeight = 0;
      path.push (i);

      if (findCycle (graph, info, path, i))
        break;
    }
  }

  int top = path.top ();
  printf ("%d ", top);

  path.pop ();
  while (path.top () != top)
  {
    printf ("%d ", path.top ());
    path.pop ();
  }
  printf ("\n");

  delete[] info;

  return 0;
}
