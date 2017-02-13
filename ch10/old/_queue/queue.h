#ifndef	QUEUE_H
#define	QUEUE_H

typedef struct
{
  int size;
  int head;
  int tail;
  void **array;
} queue;

queue *queue_new (int size);

int queue_size (queue *que);

int queue_empty (queue *que);

int queue_enqueue (queue *que, void *ele);

void *queue_dequeue (queue *que);

int queue_free (queue *que);

#endif
