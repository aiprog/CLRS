/*
 * 	Queue implemented by two stacks.
 */

#ifndef	QUEUE_H
#define	QUEUE_H

#include "stack.h"

typedef struct
{
  stack *stk1;
  stack *stk2;
} queue;

queue *queue_new (int size);

int queue_size (queue *que);

int queue_empty (queue *que);

int queue_enqueue (queue *que, void *ele);

void *queue_dequeue (queue *que);

int queue_free (queue *que);

#endif
