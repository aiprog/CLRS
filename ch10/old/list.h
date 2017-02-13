/*
 * 	Doubly linked list with sentinal.
 */

#ifndef	LIST_H
#define	LIST_H

typedef struct _node
{
  int key;
  struct _node *prev;
  struct _node *next;
} node;

typedef struct
{
  node *head;
} list;

node *node_new (int key);

int node_get_key (node *nd, int *key);

int node_set_key (node *nd, int key);

int node_free (node *nd);

list *list_new ();

node *list_search (list *ls, int key);

int list_insert (list *ls, node *nd);

int list_delete (list *ls, node *nd);

int print (list *ls);

int list_free (list *ls);

#endif
