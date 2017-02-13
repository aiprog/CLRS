#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main ()
{
  list *ls;
  int key, ch;

  ls = list_new ();
  ch = 1;
  while (ch != 'q')
  {
    printf ("Choose 's' for search, 'i' for insert, "
       	    "'d' for delete and 'q' for quit:  ");
    ch = getchar ();
    getchar ();		/* Skip the carriage return. */
    switch (ch)
    {
      case 's':
	printf ("The key: ");
	scanf ("%d", &key);
	getchar ();	/* Skip the carriage return. */

	if (!node_get_key (list_search (ls, key), &key))
	  printf ("Key %d found.\n", key);
	else
	  printf ("Key %d not found.\n", key);
	print (ls);

	break;
      case 'i':
	printf ("The key: ");
	scanf ("%d", &key);
	getchar ();	/* Skip the carriage return. */

	list_insert (ls, node_new (key));
	print (ls);

	break;
      case 'd':
	printf ("The key: ");
	scanf ("%d", &key);
	getchar ();	/* Skip the carriage return. */

	if (!list_delete (ls, list_search (ls, key)))
	  printf ("Key %d was deleted successfully.\n", key);
	else
	  printf ("Deletion failed.\n");
	print (ls);

	break;
      case 'q':
	/* Empty. */
	break;
      default:
	printf ("Unrecognized option %c, try again.\n", ch);
    }
  }

  list_free (ls);

  return 0;
}
