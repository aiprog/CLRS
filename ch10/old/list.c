#include <stdio.h>
#include <stdlib.h>

#include "list.h"

node *node_new (int key)
{
  node *nd;

  if ((nd = malloc (sizeof (node))) == NULL)
    return NULL;
  else
  {
    nd->key = key;
    nd->prev = NULL;
    nd->next = NULL;

    return nd;
  }
}

int node_get_key (node *nd, int *key)
{
  if (nd == NULL || key == NULL)
    return -1;
  else
  {
    *key = nd->key;
    return 0;
  }
}

int node_set_key (node *nd, int key)
{
  if (nd == NULL)
    return -1;
  else
  {
    nd->key = key;
    return 0;
  }
}

int node_free (node *nd)
{
  if (nd == NULL)
    return -1;
  else
  {
    free (nd);
    return 0;
  }
}

list *list_new ()
{
  list *ls;

  if ((ls = malloc (sizeof (list))) == NULL)
    return NULL;
  else if ((ls->head = malloc (sizeof (node))) == NULL)
  {
    free (ls);
    return NULL;
  }
  else
  {
    ls->head->prev = ls->head;
    ls->head->next = ls->head;

    return ls;
  }
}

node *list_search (list *ls, int key)
{
  node *ptr;

  if (ls == NULL)
    return NULL;
  else
  {
    /* The list will be traversed at most one time. */
    ls->head->key = key;
    ptr = ls->head->next;
    while (ptr->key != key)
      ptr = ptr->next;

    if (ptr != ls->head)	/* We found the key. */
      return ptr;
    else			/* We didn't. */
      return NULL;
  }
}

int list_insert (list *ls, node *nd)
{
  if (ls == NULL || nd == NULL)
    return -1;
  else
  {
    nd->next = ls->head->next;
    ls->head->next->prev = nd;
    nd->prev = ls->head;
    ls->head->next = nd;

    return 0;
  }
}

int list_delete (list *ls, node *nd)
{
  if (ls == NULL || nd == NULL)
    return -1;
  else
  {
    nd->prev->next = nd->next;
    nd->next->prev = nd->prev;
    node_free (nd);

    return 0;
  }
}

int print (list *ls)
{
  node *ptr;

  if (ls == NULL)
    return -1;
  else
  {
    ptr = ls->head->next;
    while (ptr != ls->head)
    {
      printf ("%d ", ptr->key);
      ptr = ptr->next;
    }
    if (ls->head->next != ls->head)
      printf ("\n");

    return 0;
  }
}

int list_free (list *ls)
{
  node *ptr;

  if (ls == NULL)
    return -1;
  else
  {
    ptr = ls->head->next;
    while (ptr != ls->head)
    {
      ptr = ptr->next;
      node_free (ptr->prev);
    }
    node_free (ls->head);
    free (ls);

    return 0;
  }
}
