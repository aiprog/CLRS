#include <stdlib.h>
#include <string.h>

#include "stack.h"

stack *stack_new (int size)
{
  stack *stk;

  if (size <= 0)
    return NULL;
  else if ((stk = malloc (sizeof (stack)))== NULL)
    return NULL;
  else if ((stk->array = malloc (sizeof (void *) * size)) == NULL)
  {
    /* Free 'stk' when 'malloc' for 'stk->array' fail. */
    free (stk);
    return NULL;
  }

  stk->size = size;
  stk->top = -1;

  return stk;
}

int stack_size (stack *stk)
{
  if (stk == NULL)
    return -1;
  else
    return stk->top + 1;
}

int stack_empty (stack *stk)
{
  if (stk == NULL)
    return -1;
  else
    return stk->top == -1;
}

int stack_push (stack *stk, void *ele)
{
  void **new_array;

  if (stk == NULL ||ele == NULL)
    return -1;
  else if (stk->top == stk->size - 1)
  {    
    /* Expand the stack dynamically. */
    if ((new_array = malloc (sizeof (void *) * stk->size * 2)) == NULL)
      return -1;
    else
    {
      memcpy (new_array, stk->array, sizeof (void *) * (stk->top + 1));
      free (stk->array);
      stk->size *= 2;
      stk->array = new_array;
    }
  }
  stk->array[++stk->top] = ele;

  return 0;
}

void *stack_pop (stack *stk)
{
  if (stk == NULL || stack_empty (stk))
    return NULL;
  else
    return stk->array[stk->top--];
}

int stack_free (stack *stk)
{
  if (stk == NULL)
    return -1;
  else
  {
    free (stk->array);
    free (stk);

    return 0;
  }
}
