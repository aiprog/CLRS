/*
 * 	A doubly linked list supports compactification.
 */

#ifndef	LIST_H
#define	LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef	struct
{
  int size;
  int *key;
  int *prev;
  int *next;
  int head;	/* List sentinel. */
  int avail;	/* Free list sentinel. */
} LIST;

LIST *list_new (int size)
{
  LIST *list;
  int i, j;

  if (size <= 0)
    return NULL;

  list = malloc (sizeof (LIST));
  size += 2;	/* Two sentinels. */
  list->size = size;
  list->key = malloc (sizeof (int) * size);
  list->prev = malloc (sizeof (int) * size);
  list->next = malloc (sizeof (int) * size);

  /* Initialize the list. */
  list->head = 0;
  list->prev[list->head] = list->head;
  list->next[list->head] = list->head;

  /* Initialize the free list. */
  list->avail = list->size - 1;		
  j = list->avail;
  for (i = list->size - 1; i > 0; --i)
  {
    list->next[i] = j;
    list->prev[j] = i;
    j = i;
  }
  list->next[list->avail] = j;
  list->prev[j] = list->avail;

  return list;
}

int list_insert (LIST *list, int key)
{
  int pos;

  pos = list->next[list->avail];
  if (list == NULL || pos == list->avail)	/* Out of space. */
    return -1;

  /* Remove a element from the free list. */
  list->next[list->avail] = list->next[pos];
  list->prev[list->next[pos]] = list->avail;

  /* Attach the element to the list. */
  list->key[pos] = key;
  list->next[pos] = list->next[list->head];
  list->prev[list->next[list->head]] = pos;
  list->prev[pos] = list->head;
  list->next[list->head] = pos;

  return 0;
}

int list_search (LIST *list, int key)
{
  int i;

  if (list == NULL)
    return -1;

  list->key[list->head] = key;	/* The list will be traversed at most once. */
  i = list->next[list->head];
  while (list->key[i] != key)
    i = list->next[i];

  if (i != list->head)	/* We found it. */	
    return i;
  else
    return -1;
}

void list_delete (LIST *list, int pos)
{
  if (list == NULL || pos == -1)
    return;

  /* Remove it from the list. */
  list->next[list->prev[pos]] = list->next[pos];
  list->prev[list->next[pos]] = list->prev[pos];

  /* Attach it to the free list. */
  list->next[pos] = list->next[list->avail];
  list->prev[list->next[list->avail]] = pos;
  list->prev[pos] = list->avail;
  list->next[list->avail] = pos;
}

void compactify (LIST *list)
{
  int ele;
  int i, j;
  int ip, in, jp, jn;
  int i_tmp, j_tmp;

  /* Count the sum of elements in the list. */
  ele = 1;
  i = list->next[list->head];
  while (i != list->head)
  {
    ++ele;
    i = list->next[i];
  }

  i = list->next[list->head];
  j = list->next[list->avail];
  while (1)
  {
    /* Look for the element will be exchanged in the list. */
    while (i != list->head && i < ele)
      i = list->next[i];
    if (i == list->head)
      break;

    /* Look for the element will be exchanged in the free list. */
    while (j != list->avail && j >= ele)
      j = list->next[j];

    /* Exchange the two elements. */
    list->key[j] = list->key[i];

    ip = list->prev[i];
    in = list->next[i];
    jp = list->prev[j];
    jn = list->next[j];

    list->prev[i] = jp;
    list->next[jp] = i;
    list->next[i] = jn;
    list->prev[jn] = i;

    list->prev[j] = ip;
    list->next[ip] = j;
    list->next[j] = in;
    list->prev[in] = j;

    i_tmp = i;
    j_tmp = j;
    i = list->next[j_tmp];
    j = list->next[i_tmp];
  }
}

void print_list (LIST *list)
{
  int i;

  if (list == NULL)
    return;

  i = list->next[list->head];
  while (i != list->head)
  {
    printf ("Pos: %d Value: %d\n", i, list->key[i]);
    i = list->next[i];
  }
}

void list_free (LIST *list)
{
  if (list == NULL)
    return;

  free (list->key);
  free (list->prev);
  free (list->next);
  free (list);
}

#endif
