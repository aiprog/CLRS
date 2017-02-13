/*
 *  fib_heap.h
 *
 *  A simple implementation of Fibonacci Heap.
 */

#ifndef   FIB_HEAP_H
#define   FIB_HEAP_H

typedef struct node
{
  int key;
  int mark;
  int degree;
  struct node *parent;
  struct node *left;
  struct node *right;
  struct node *child;
} Fib_heap_node;

typedef struct
{
  Fib_heap_node *min;
  int num;
} Fib_heap;

/* Create a heap. */
Fib_heap *new_fib_heap ();

/* Free a heap. */
void free_fib_heap (Fib_heap *heap);

/* Print a heap. */
void fib_heap_print (Fib_heap *heap);

/* Search. */
Fib_heap_node *fib_heap_search (Fib_heap *heap, int key);

/* Insert. */
void fib_heap_insert (Fib_heap *heap, int key);

/* Get the minimum. */
int fib_heap_minimum (Fib_heap *heap);

/* Extract the minimum. */
Fib_heap_node *fib_heap_extract_min (Fib_heap *heap);

/* Union. */
Fib_heap *fib_heap_union (Fib_heap *heap1, Fib_heap *heap2);

/* Decrease. */
void fib_heap_decrease_key (Fib_heap *heap, Fib_heap_node *node, int key);

/* Delete. */
void fib_heap_delete (Fib_heap *heap, Fib_heap_node *node);

#endif