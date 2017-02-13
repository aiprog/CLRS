#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

int main ()
{
  queue *que;
  int size, flg, ele;
  void *vp;
  char option;

  flg = 1;
  while (flg)
  {
    printf ("Queue size: ");
    scanf ("%d", &size);

    if ((que = queue_new (size)) != NULL)
      flg = 0;
  }

  flg = 1;
  while (flg)
  {
    printf ("'i' for enqueue, 'd' for dequeue and 'q' for quit: ");
    while (isspace(option = getchar ()))
      ;	/* Skip spaces. */
 
    switch (option)
    {
      case 'i':
	printf ("Element to be enqueued: ");
	scanf ("%d", &ele);
	vp = malloc (sizeof (int));
	*(int *)vp = ele;
	if (queue_enqueue (que, vp) == 0)
	  printf ("Enqueue succeed.\n");
	else
	  printf ("Enqueue fail.\n");
	break;
      case 'd':
	if ((vp = queue_dequeue (que)) == NULL)
	  printf ("Dequeue fail.\n");
	else
	{
	  printf ("Dequeue succeed: %d\n", *(int *)vp);
	  free (vp);
	}
	break;
      case 'q':
	flg = 0;
	break;
      default:
	printf ("Unrecognized option: %c\n", option);
	break;
    }
  }

  queue_free (que);

  return 0;
}
