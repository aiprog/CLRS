#include <stdio.h>

int main ()
{
  int *pt = NULL;

  int (*p)[3] = (int (*)[3]) pt;

  return 0;
}
