#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define	COP	1
#define REP	2
#define	DEL	3
#define	INS	4

#define	COP_C	1
#define	REP_C	3
#define	DEL_C	4
#define	INS_C	4


void print_solu (void *Solu, int len1, int i, int j)
{
  int (*solu)[len1 + 1];

  if (i == 0 && j == 0)
    return;

  solu = Solu;
  switch (solu[j][i])
  {
    case COP:
      print_solu (solu, len1, i - 1, j - 1);
      printf ("copy ");

      break;
    case REP:
      print_solu (solu, len1, i - 1, j - 1);
      printf ("replace ");

      break;
    case DEL:
      print_solu (solu, len1, i - 1, j);
      printf ("delete ");

      break;
    case INS:
      print_solu (solu, len1, i, j - 1);
      printf ("insert ");

      break;
  }
}

void ed (char *src, char *dest)
{
  int len1, len2;

  len1 = strlen (src);
  len2 = strlen (dest);

  int (*memo)[len1 + 1], (*solu)[len1 + 1];
  int i, j;
  int temp;

  memo = malloc (sizeof (int) * (len1 + 1) * (len2 + 1));
  solu = malloc (sizeof (int) * (len1 + 1) * (len2 + 1));

  memo[0][0] = 0;
  for (i = 1; i <= len1; ++i)
  {
    memo[0][i] = DEL_C * i;
    solu[0][i] = DEL;
  }
  for (i = 1; i <= len2; ++i)
  {
    memo[i][0] = INS_C * i;
    solu[i][0] = INS;
  }

  for (j = 1; j <= len2; ++j)
  {
    for (i = 1; i <= len1; ++i)
    {
      memo[j][i] = INT_MAX;

      if (src[i - 1] == dest[j - 1])	/* Copy. */
      {
        temp = memo[j - 1][ i - 1] + COP_C;
        if (temp < memo[j][i])
        {
	  memo[j][i] = temp;
	  solu[j][i] = COP;
        }
      }
      else				/* Replace. */
      {
        temp = memo[j - 1][i - 1] + REP_C;
        if (temp < memo[j][i])
        {
	  memo[j][i] = temp;
	  solu[j][i] = REP;
        }
      }

      /* Delete. */
      temp = memo[j][i - 1] + DEL_C;
      if (temp < memo[j][i])
      {
	memo[j][i] = temp;
	solu[j][i] = DEL;
      }

      /* Insert. */
      temp = memo[j - 1][i] + INS_C;
      if (temp < memo[j][i])
      {
        memo[j][i] = temp;
        solu[j][i] = INS;
      }
    }
  }

  printf ("Min: %d\n", memo[len2][len1]);
  printf ("Operations: ");
  print_solu (solu, len1, len1, len2);
  printf ("\n");
}

int main ()
{

  ed ("GATCGGCAT", "CAATGTGAATC");

  return 0;
}
