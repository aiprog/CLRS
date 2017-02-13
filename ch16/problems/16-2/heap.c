#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

#define PARENT(x)     (((x) - 1) / 2)
#define L_CHILD(x)    ((x) * 2 + 1)
#define R_CHILD(x)    ((x) * 2 + 2)

static void **heap;
static int size, num;
static int (*compare)(void *, void *);

void new_heap (int Size, int (*Compare)(void *, void *))
{
  size = Size;
  num = 0;
  heap = malloc (sizeof (void *) * size);
  compare = Compare;
}

void free_heap ()
{
  free (heap);
}

int is_empty ()
{
	return num == 0;
}

static void heapify (int root)
{
  int max;
  void *temp;

  max = L_CHILD (root);
  if (max >= num)
    return;

  if (R_CHILD (root) < num && compare (heap[R_CHILD (root)], heap[max]) > 0)
    max = R_CHILD (root);

  if (compare (heap[max], heap[root]) > 0)
  {
    temp = heap[root];
    heap[root] = heap[max];
    heap[max] = temp;

    heapify (max);
  }
}

void insert (void *data)
{
  void *temp;
  int i;

  if (num == size)
  {
    printf ("Heap overflow.\n");
    exit (1);
  }

  heap[num++] = data;
  for (i = num - 1; i != 0; i = PARENT (i))
  {
    if (compare (heap[i], heap[PARENT (i)]) > 0)
    {
      temp = heap[PARENT (i)];
      heap[PARENT (i)] = heap[i];
      heap[i] = temp;
    }
    else
      break;
  }
}

void *extract ()
{
  void *temp;

  if (num == 0)
    return NULL;

  temp = heap[0];
  heap[0] = heap[--num];
  heapify (0);

  return temp;
}