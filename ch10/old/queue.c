#include <stdlib.h>

#include "queue.h"

queue *queue_new (int size)
{
  queue *que;

  if (size <= 0)
    return NULL;
  else if ((que = malloc (sizeof (queue))) == NULL)
    return NULL;
  else if ((que->stk1 = stack_new (size / 2)) == NULL)
  {
    /* Free 'que' when 'stack_new' for 'que->stk1' fail. */
    free (que);
    return NULL;
  }
  else if ((que->stk2 = stack_new (size / 2)) == NULL)
  {
    /* Free 'que' and 'que->stk1' when 'stack_new' for 'que->stk2' fail. */
    free (que);
    stack_free (que->stk1);
    return NULL;
  }

  return que;
}

int queue_size (queue *que)
{
  if (que == NULL)
    return -1;
  else
    return stack_size (que->stk1) + stack_size (que->stk2);
}

int queue_empty (queue *que)
{
  if (que == NULL)
    return -1;
  else
    return stack_empty (que->stk1) && stack_empty (que->stk2);
}

int queue_enqueue (queue *que, void *ele)
{
  if (que == NULL || ele == NULL)
    return -1;
  else
  {
    stack_push (que->stk2, ele);
    return 0;
  }
}

void *queue_dequeue (queue *que)
{
  if (que == NULL)
    return NULL;
  else if (!stack_empty (que->stk1))
    return stack_pop (que->stk1);
  else
  {
    while (!stack_empty (que->stk2))
      stack_push (que->stk1, stack_pop (que->stk2));

    return stack_pop (que->stk1);
  }
}

int queue_free (queue *que)
{
  if (que == NULL)
    return -1;
  else
  {
    stack_free (que->stk1);
    stack_free (que->stk2);
    free (que);

    return 0;
  }
}
