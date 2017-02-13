#include <stdio.h>
#include <stdlib.h>

void asp (int (*activity)[2], int size)
{
  int finish;
  int i;

  finish = 0;
  for (i = 0; i < size; ++i)
  {
    if (activity[i][0] >= finish)
    {
      printf ("%d ", i + 1);
      finish = activity[i][1];
    }
  }

  printf ("\n");
}

int main ()
{
  int activity[11][2] = { { 1, 4 }, { 3, 5 }, { 0, 6 },
			  { 5, 7 }, { 3, 9 }, { 5, 9 },
			  { 6, 10 }, { 8, 11 }, { 8, 12 },
			  { 2, 14 }, { 12, 16 }
                        };

  asp (activity, 11);

  return 0;
}


