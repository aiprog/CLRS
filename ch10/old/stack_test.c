#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

int main ()
{
  stack *stk;
  int size, flg, ele;
  void *vp;
  char option;

  flg = 1;
  while (flg)
  {
    printf ("Stack size: ");
    scanf ("%d", &size);

    if ((stk = stack_new (size)) != NULL)
      flg = 0;
  }

  flg = 1;
  while (flg)
  {
    printf ("'i' for push, 'd' for pop and 'q' for quit: ");
    while (isspace(option = getchar ()))
      ;	/* Skip spaces. */
 
    switch (option)
    {
      case 'i':
	printf ("Element to be pushed: ");
	scanf ("%d", &ele);
	vp = malloc (sizeof (int));
	*(int *)vp = ele;
	if (stack_push (stk, vp) == 0)
	  printf ("Push succeed.\n");
	else
	  printf ("Push fail.\n");
	break;
      case 'd':
	if ((vp = stack_pop (stk)) == NULL)
	  printf ("Pop fail.\n");
	else
	{
	  printf ("Pop succeed: %d\n", *(int *)vp);
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

  stack_free (stk);

  return 0;
}
