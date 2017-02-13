#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define	SIZE 1000

void print_solu (char *str, int size, void *Solu, int beg, int end)
{
  int (*solu)[size];
  
  solu = Solu;
  if (beg == end)
  {
    printf ("%c ", str[beg]);
    return;
  }
  else if (solu[beg][end] == -1)
    print_solu (str, size, solu, beg, end - 1);
  else
  {
    printf ("%c ", str[end]);
    print_solu (str, size, solu, solu[beg][end] + 1, end - 1);
    printf ("%c ", str[end]);
  }
}

void lps (char *str, int size)
{
  int (*memo)[size], (*solu)[size];
  int i, j, k;
  int temp;

  memo = malloc (sizeof (int) * size * size);
  solu = malloc (sizeof (int) * size * size);

  for (i = 0; i < size; ++i)
    memo[i][i] = 1;

  for (k = 2; k <= size; ++k)
  {
    for (i = 0; i <= size - k; ++i)
    {
      memo[i][k - 1 + i] = memo[i][k - 2 + i];
      solu[i][k - 1 + i] = -1;
      for (j = i; j < k - 1 + i; ++j)
      {
	if (str[j] == str[k - 1 + i])
	{
	  temp = memo[j + 1][k - 2 + i] + 2;
	  if (temp > memo[i][k - 1 + i])
	  {
	    memo[i][k - 1 + i] = temp;
	    solu[i][k - 1 + i] = j;
	  }

	  break;
	}
      }
    }
  }

  printf ("Max: %d\n", memo[0][size - 1]);
  if (SIZE < 20)
  {
    print_solu (str, size, solu, 0, size - 1);
    printf ("\n");
  }
}

int main ()
{
  char *str;
  int i;
  int beg, end;

  str = malloc (sizeof (char) * SIZE);
  srand (time (NULL));
  for (i = 0; i < SIZE; ++i)
    str[i] = 'A' + rand () % 26;

  beg = clock ();
  lps (str, SIZE);
  end = clock ();
  printf ("Time: %dms\n", (end - beg) / 1000);

  return 0;
}
