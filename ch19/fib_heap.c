/*
 *  fib_heap.c
 *
 *  This file implements fib_heap.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "fib_heap.h"

/* Create a heap. */
Fib_heap *new_fib_heap ()
{
  Fib_heap *heap;

  heap = malloc (sizeof (Fib_heap));
  heap->min = NULL;
  heap->num = 0;

  return heap;
}

static void free_fib_heap_root (Fib_heap_node *node)
{
  Fib_heap_node *ite;

  if (node->child != NULL)
  {
    node->child->left->right = NULL;
    for (ite = node->child; ite != NULL; ite = ite->right)
    {
      free_fib_heap_root (ite);
    }
  }

  free (node);
}

/* Free a heap. */
void free_fib_heap (Fib_heap *heap)
{
  Fib_heap_node *ite;

  if (heap->min != NULL)
  {
    heap->min->left->right = NULL;
    for (ite = heap->min; ite != NULL; ite = ite->right)
    {
      free_fib_heap_root (ite);
    }
  }

  free (heap);
}

static void fib_heap_print_root (Fib_heap_node *node)
{
  Fib_heap_node *ite;

  printf ("%d ", node->key);
  if (node->child != NULL)
  {
    ite = node->child;
    do
    {
      fib_heap_print_root (ite);
      ite = ite->right;
    } while (ite != node->child);
  }
}

/* Print a heap. */
void fib_heap_print (Fib_heap *heap)
{
  Fib_heap_node *ite;

  if (heap->min != NULL)
  {
    ite = heap->min;
    do
    {
      fib_heap_print_root (ite);
      ite = ite->right;
    } while (ite != heap->min);
  }
  printf ("\n");
}

static Fib_heap_node *fib_heap_search_root (Fib_heap_node *node, int key)
{
  Fib_heap_node *ite, *retval;

  retval = NULL;
  if (node->key == key)
    retval = node;
  else if (node->child != NULL && key > node->key)
  {
    ite = node->child;
    do
    {
      if ((retval = fib_heap_search_root (ite, key)) != NULL)
        break;
      ite = ite->right;
    } while (ite != node->child);
  }

  return retval;
}

/* Search. */
Fib_heap_node *fib_heap_search (Fib_heap *heap, int key)
{
  Fib_heap_node *ite, *retval;

  retval = NULL;
  if (heap->min != NULL)
  {
    ite = heap->min;
    do
    {
      if ((retval = fib_heap_search_root (ite, key)) != NULL)
        break;
      ite = ite->right;
    } while (ite != heap->min);
  }

  return retval;
}

/* Insert. */
void fib_heap_insert (Fib_heap *heap, int key)
{
  Fib_heap_node *node;

  node = malloc (sizeof (Fib_heap_node));
  node->key = key;
  node->mark = node->degree = 0;
  node->parent = node->left = node->right = node->child = NULL;

  if (heap->min == NULL)
  {
    node->left = node->right = node;
    heap->min = node;
  }
  else
  {
    node->left = heap->min->left;
    node->left->right = node;
    node->right = heap->min;
    node->right->left = node;

    if (node->key < heap->min->key)
      heap->min = node;
  }
  ++heap->num;
}

/* Get the minimum. */
int fib_heap_minimum (Fib_heap *heap)
{
  if (heap->min == NULL)
    return -1;
  return heap->min->key;
}

/* Extract the minimum. */
Fib_heap_node *fib_heap_extract_min (Fib_heap *heap)
{
  Fib_heap_node *min, *head, *tail, *ite, *temp;
  int max_degree, degree, i;

  if (heap->min == NULL)
    return NULL;
  max_degree = log (heap->num) / log ((1 + sqrt (5)) / 2);

  Fib_heap_node *array[max_degree];

  min = heap->min;
  head = heap->min->child;

  if (head != NULL)
  {
    tail = head->right;

    head->right = min->right;
    head->right->left = head;
    tail->left = min;
    tail->left->right = tail;

    ite = head;
    do
    {
      ite->parent = NULL;
      ite = ite->left;
    } while (ite != tail);
  }

  --heap->num;
  if (min->left == min)
  {
    heap->min = NULL;
    return min;
  }
  else
  {
    min->left->right = min->right;
    min->right->left = min->left;
  }

  memset (array, 0, max_degree * sizeof (Fib_heap_node *));

  ite = min->right;
  do
  {
    temp = ite;
    if (ite->right == NULL)
      printf ("NULL\n");
    ite = ite->right;

    degree = temp->degree;
    while (array[degree] != NULL)
    {
      if (temp->key < array[degree]->key)
      {
        tail = temp;
        temp = array[degree];
        array[degree] = tail;
      }

      if (array[degree]->child == NULL)
      {
        temp->left = temp->right = temp;
        array[degree]->child = temp;
      }
      else
      {
        temp->left = array[degree]->child->left;
        temp->left->right = temp;
        temp->right = array[degree]->child;
        temp->right->left = temp;
      }
      temp->mark = 0;
      temp->parent = array[degree];
      ++array[degree]->degree;

      temp = array[degree];
      array[degree] = NULL;
      ++degree;
    }
    array[degree] = temp;
  } while (ite != min->right);

  heap->min = NULL;
  for (i = 0; i < max_degree; ++i)
  {
    if (array[i] != NULL)
    {
      if (heap->min == NULL)
      {
        heap->min = array[i];
        array[i]->left = array[i]->right = array[i];
      }
      else
      {
        array[i]->left = heap->min->left;
        array[i]->left->right = array[i];
        array[i]->right = heap->min;
        array[i]->right->left = array[i];

        if (array[i]->key < heap->min->key)
          heap->min = array[i];
      }
    }
  }

  return min;
}

/* Union. */
Fib_heap *fib_heap_union (Fib_heap *heap1, Fib_heap *heap2)
{
  Fib_heap *heap;
  Fib_heap_node *temp;

  heap = new_fib_heap ();
  if (heap1->min == NULL)
  {
    heap->min = heap2->min;
    heap->num = heap2->num;
  }
  else if (heap2->min == NULL)
  {
    heap->min = heap1->min;
    heap->num = heap1->num;
  }
  else
  {
    temp = heap1->min->left;
    heap1->min->left = heap2->min->left;
    heap1->min->left->right = heap1->min;
    heap2->min->left = temp;
    heap2->min->left->right = heap2->min;

    if (heap1->min->key < heap2->min->key)
      heap->min = heap1->min;
    else
      heap->min = heap2->min;

    heap->num = heap1->num + heap2->num;
  }

  heap1->min = heap2->min = NULL;
  heap1->num = heap2->num = 0;

  free_fib_heap (heap1);
  free_fib_heap (heap2);

  return heap;
}

/* Decrease. */
void fib_heap_decrease_key (Fib_heap *heap, Fib_heap_node *node, int key)
{
  Fib_heap_node *parent, *ite;

  node->key = key;
  if (node->key < heap->min->key)
    heap->min = node;

  parent = node->parent;
  if (parent != NULL)
  {
    node->mark = 0;
    node->parent = NULL;
    node->left = heap->min->left;
    node->left->right = node;
    node->right = heap->min;
    node->right->left = node;

    ite = parent; 
    while (ite->mark == 1 && ite->parent != NULL)
    {
      parent = ite->parent;

      ite->mark = 0;
      ite->parent = NULL;
      ite->left = heap->min->left;
      ite->left->right = ite;
      ite->right = heap->min;
      ite->right->left = ite;

      ite = parent;
    }
  }
}

/* Delete. */
void fib_heap_delete (Fib_heap *heap, Fib_heap_node *node)
{
  fib_heap_decrease_key (heap, node, -1);
  free (fib_heap_extract_min (heap));
}

/* Test. */
static void fib_heap_test ()
{
  Fib_heap *heap;
  Fib_heap_node *node;
  char option[10];
  int key, value;

  heap = new_fib_heap ();
  option[0] = 0;
  while (option[0] != 'q')
  {
    printf ("option: ");
    scanf ("%s", option);

    if (option[0] == 'i')
    {
      printf ("key: ");
      scanf ("%d", &key);

      fib_heap_insert (heap, key);
    }
    else if (option[0] == 'm')
    {
      printf ("%d\n", fib_heap_minimum (heap));
    }
    else if (option[0] == 'e')
    {
      free (fib_heap_extract_min (heap));
    }
    else if (option[0] == 'd')
    {
      printf ("key: ");
      scanf ("%d", &key);
      printf ("value: ");
      scanf ("%d", &value);

      if ((node = fib_heap_search (heap, key)) != NULL)
      {
        fib_heap_decrease_key (heap, node, value);
      }
      else
        printf ("not found\n");
    }
    else if (option[0] == 'p')
    {
      fib_heap_print (heap);
    }
    else if (option[0] != 'q')
      printf ("unrecognized option\n");
  }

  free_fib_heap (heap);
}