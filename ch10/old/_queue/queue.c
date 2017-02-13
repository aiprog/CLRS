#include <stdlib.h>

#include "queue.h"

queue *queue_new (int size)
{
  queue *que;

  if (size <= 0)
    return NULL;
  else if ((que = malloc (sizeof (queue))) == NULL)
    return NULL;
  else if ((que->array = malloc (sizeof (void *) * size)) == NULL)
  {
    /* Free 'que' when 'malloc' for 'que->array' fail. */
    free (que);   
    return NULL;
  }

  que->size = size;
  que->head = 0;
  que->tail = 0;

  return que;
}

int queue_size (queue *que)
{
  if (que == NULL)
    return -1;
  else
  {
    if (que->tail >= que->head)
      return que->tail - que->head;
    else
      return que->size - (que->head - que->tail);
  }
}

int queue_empty (queue *que)
{
  if (que == NULL)
    return -1;
  else
    return que->tail == que->head;
}

int queue_enqueue (queue *que, void *ele)
{
  void **new_array;
  int i, j;

  if (que == NULL || ele == NULL)
    return -1;
  else if ((que->tail + 1) % que->size == que->head)
  {	
    /* Expand the queue dynamically. */
    if ((new_array = malloc (sizeof (void *) * que->size * 2)) == NULL)
      return -1;
    else
    {
      i = que->head;
      j = 0;
      while (i != que->tail)
      {
	new_array[j] = que->array[i];
	i = (i + 1) % que->size;
	++j;
      }

      free (que->array);
      que->head = 0;
      que->tail = que->size - 1;
      que->size *= 2;
      que->array = new_array;
    }
  }

  que->array[que->tail] = ele;
  que->tail = (que->tail + 1) % que->size;

  return 0;
}

void *queue_dequeue (queue *que)
{
  void *ele;

  if (que == NULL || queue_empty (que))
    return NULL;
  else
  {
    ele = que->array[que->head];
    que->head = (que->head + 1) % que->size;

    return ele;
  }
}

int queue_free (queue *que)
{
  if (que == NULL)
    return -1;
  else
  {
    free (que->array);
    free (que);

    return 0;
  }
}
