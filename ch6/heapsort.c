/* Heapsort and a implementation of priority queue based on heap. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
  int *arr;
  int size;
  int len;
} PRIQUE;

/* Return the parent node. */

inline int parent (int i)
{
  return (i - 1) / 2;
}

/* Return the left child node. */

inline int left (int i)
{
  return 2 * i + 1;
}

/* Return the right child node. */

inline int right (int i)
{
  return 2 * i + 2;
}

/* Build a new max-heap at node i, assuming that
 * it's two subtrees are already max-heaps. */

void max_heapify (int *arr, int len, int i)
{
  int max;
  int l;
  int r;
  int tmp;

  while (1)
  {
    max = i;
    l = left (i);
    r = right (i);

    if (l < len && arr[l] > arr[max])
      max = l;
    if (r < len && arr[r] > arr[max])
      max = r;

    if (max != i)
    {
      tmp = arr[max];
      arr[max] = arr[i];
      arr[i] = tmp;

      i = max;
    }
    else
      break;
  }
}

/* Build a initial max-heap from a array. */

void build_max_heap (int *arr, int len)
{
  int i;

  /* Treat all leaves as 1-element heaps. */
  for (i = len / 2 - 1; i >= 0; --i)
    max_heapify (arr, len, i);
}

/* Heapsort a array. */

void heap_sort (int *arr, int len)
{
  int i;
  int tmp;

  build_max_heap (arr, len);

  for (i = len - 1; i > 0; --i)
  {
    tmp = arr[0];
    arr[0] = arr[i];
    arr[i] = tmp;

    max_heapify (arr, i, 0);
  }
}

/* Return the maximum in the queue. */

int heap_maximum (PRIQUE *que)
{
  return que->arr[0];
}

/* Remove and return the maximum in the queue. */

int heap_extract_max (PRIQUE *que)
{
  int max;

  if (que->size == 0)
  {
    printf ("Empty queue!\n");

    return -1;
  }

  max = que->arr[0];
  --que->size;
  que->arr[0] = que->arr[que->size];
  max_heapify (que->arr, que->size, 0);

  return max;
}

/* Increase the key of a element to a value in the queue. */

void heap_increase_key (PRIQUE *que, int i, int key)
{
  int cur;

  if (i < 0 || i > que->size)
  {
    printf ("Index of queue is out of range!\n");

    return;
  }
  if (key < que->arr[i - 1])
  {
    printf ("New key is smaller than current key!\n");

    return;
  }

  cur = i - 1;
  while (cur > 0 && que->arr[parent (cur)] < key)
  {
    que->arr[cur] = que->arr[parent (cur)];
    cur = parent (cur);
  }
  que->arr[cur] = key;
}

/* Insert a key to the queue. */

void max_heap_insert (PRIQUE *que, int key)
{
  int *new_arr;
  int new_len;

  if (que->size == que->len)
  {
    new_len = 2 * que->len + 1;
    new_arr = malloc (sizeof (int) * new_len);
    memcpy (new_arr, que->arr, sizeof (int) * que->len);

    if (que->len != 0)
      free (que->arr);
    que->len = new_len;
    que->arr = new_arr;
  }

  que->arr[que->size] = -1;
  ++que->size;

  heap_increase_key (que, que->size, key);
}

/* Delete a element from the queue. */

void heap_delete (PRIQUE *que, int i)
{
  if (i < 0 || i > que->size)
  {
    printf ("Invalid index!\n");

    return;
  }

  que->arr[i - 1] = que->arr[--que->size];
  max_heapify (que->arr, que->size, i - 1);
}

int main ()
{
  PRIQUE que;
  char cmd[10];
  int i;
  int key;

  que.size = 0;
  que.len = 0;

  while (1)
  {
    scanf ("%s", cmd);

    if (!strcmp (cmd, "insert"))
    {
      scanf ("%d", &key);
      max_heap_insert (&que, key);
    }
    else if (!strcmp (cmd, "delete"))
    {
      scanf ("%d", &i);
      heap_delete (&que, i);
    }
    else if (!strcmp (cmd, "increase"))
    {
      scanf ("%d%d", &i, &key);
      heap_increase_key (&que, i, key);
    }
    else if (!strcmp (cmd, "extract"))
      heap_extract_max (&que);
    else if (!strcmp (cmd, "print"))
    {
      for (i = 0; i < que.size; ++i)
	printf ("%d ", que.arr[i]);
      printf ("\n");
    }
    else if (!strcmp (cmd, "quit"))
      break;
    else
      printf ("Invalid command!\n");
  }

  return 0;
}
