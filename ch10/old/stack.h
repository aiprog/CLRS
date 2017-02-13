#ifndef	STACK_H
#define	STACK_H

typedef struct 
{
  int size;
  int top;
  void **array;
} stack;

stack *stack_new (int size);

int stack_size (stack *stk);

int stack_empty (stack *stk);

int stack_push (stack *stk, void *ele);

void *stack_pop (stack *stk);

int stack_free (stack *stk);

#endif
