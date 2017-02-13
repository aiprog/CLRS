/*
 * 	A doubly linked list implemented by three arrays.
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
  int avail;	/* The free stack top. */
} LIST;

LIST *list_new (int size)
{
  LIST *list;

  if (size <= 0)
    return NULL;

  list = malloc (sizeof (LIST));
  ++size;	/* A sentinel. */
  list->size = size;
  list->key = malloc (sizeof (int) * size);
  list->prev = malloc (sizeof (int) * size);
  list->next = malloc (sizeof (int) * size);

  /* Initialize the list. */
  list->head = 0;
  list->prev[list->head] = list->head;
  list->next[list->head] = list->head;

  list->avail = 1;	/* Initial stack top. */				

  return list;
}

int list_insert (LIST *list, int key)
{
  if (list == NULL || list->avail == list->size)	/* Out of space. */
    return -1;

  /* Attach the element to the list. */
  list->key[list->avail] = key;
  list->next[list->avail] = list->next[list->head];
  list->prev[list->next[list->head]] = list->avail;
  list->prev[list->avail] = list->head;
  list->next[list->head] = list->avail;

  ++list->avail;	/* Updata stack top. */

  return 0;
}

int list_search (LIST *list, int key)
{
  int i;

  if (list == NULL)
    return -1;

  list->key[list->head] = key;	/* The list will be traverse at most once. */
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
  int top;	/* The boundary of the list. */

  if (list == NULL || pos == -1)
    return;

  top = list->avail - 1;
  list->key[pos] = list->key[top];	/* Move boundary key to 'pos'. */
 
  /* Deattach the 'pos'. */
  list->next[list->prev[pos]] = list->next[pos];
  list->prev[list->next[pos]] = list->prev[pos];

  /* Remove the boundary element and 
   * reattach the 'pos' to the position of boundary element. */
  if (top != pos) 	/* Only when the deleted element is not the boundary. */
  {
    list->prev[pos] = list->prev[top];
    list->next[list->prev[top]] = pos;
    list->next[pos] = list->next[top];
    list->prev[list->next[top]] = pos;
  }

  --list->avail;	/* Updata the stack top. */
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
