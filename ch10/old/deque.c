/*
 * 	A application of deque.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define	NUM	10
#define	SPAN	3

int main (void)
{
  int array[NUM], deque[NUM], result[NUM];
  int head, tail;
  int i, j;

  /* Generate test data. */
  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
    array[i] = rand () % (2 * NUM);

  head = 0;
  tail = 0;
  j = 0;
  for (i = 0; i < NUM; ++i)
  {
    /* Dequeue on the tail until it isn't smaller than the tail or
     * the deque is empty. Then enqueue it on the tail. */
    while (tail > head && array[deque[tail - 1]] > array[i])
      --tail;
    deque[tail++] = i;

    if (i >= SPAN - 1)
    {
      /* The smallest is on the head of the deque. */
      result[j++] = array[deque[head]];
      /* When the head is out of the range, dequeue it. */
      if (i - SPAN + 1 == deque[head])
	++head;
    }
  }

  /* Print the input. */
  printf ("Span: %d\n", SPAN);
  printf ("Input: ");
  for (i = 0; i < NUM; ++i)
    printf ("%d ", array[i]);
  printf ("\n");

  /* Print the output. */
  printf ("Output: ");
  for (i = 0; i < j; ++i)
    printf ("%d ", result[i]);
  printf ("\n");

  return 0;
}
