#include <stdio.h>
#include <stdlib.h>

#include "compactify.h"

int main ()
{
  LIST *list;
  char option[10];
  int key;

  list = list_new (5);
  option[0] = 1;

  while (option[0] != 'q')
  {
    printf ("'i' for insert, 'd' for delete, "
	    "'c' for compactify and 'q' for quit: ");
    scanf ("%s", option);

    switch (option[0])
    {
      case 'i':
	printf ("Key: ");
	scanf ("%d", &key);

	list_insert (list, key);
	print_list (list);

	break;
      case 'd':
	printf ("Key: ");
	scanf ("%d", &key);

	list_delete (list, list_search (list, key));
	print_list (list);

	break;
      case 'c':
	compactify (list);
	print_list (list);

	break;
      case 'q':
	/* Empty. */

	break;
      default:
	printf ("Unrecognized option %c\n", option[0]);

	break;
    }
  }

  list_free (list);

  return 0;
}
