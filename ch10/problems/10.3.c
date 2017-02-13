#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define	SIZE	1000000000
#define	TEST	10

void search_linearly (int key)
{
  int i;

  i = 0;
  while (i++ != key)
    ;
}

void search_randomly (int key)
{
  int ran;
  int i;

  i = 0;
  while (i != key)
  {
    ran = rand () % SIZE;
    if (ran > i && ran <= key)
    {
      if (ran == key)
	break;
      else
	i = ran;
    }

    ++i;
  }
}

void search_bin (int beg, int end, int key)
{
  int mid;

  if (end < beg)
    return;

  mid = (end + beg) / 2;
  if (key == mid)
    return;
  else if (key < mid)
    search_bin (beg, mid - 1, key);
  else
    search_bin (mid + 1, end, key);
}

int main ()
{
  int beg, end;
  int ran;
  int i;

  srand (time (NULL));
  for (i = 0; i < TEST; ++i)
  {
    ran = rand () % SIZE;
    printf ("key: %d\n", ran);

    beg = clock ();
    search_linearly (ran);
    end = clock ();
    printf ("beg: %d end: %d #1:%dms\n", beg, end, (end - beg) / 1000);

    beg = clock ();
    search_randomly (ran);
    end = clock ();
    printf ("beg: %d end: %d #2:%dus\n", beg, end, end - beg);

    beg = clock ();
    search_bin (0, SIZE, ran);
    end = clock ();
    printf ("beg: %d end: %d #2:%dus\n\n", beg, end, end - beg);
  }

  return 0;
}
