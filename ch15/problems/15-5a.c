#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define	COP	1
#define REP	2
#define	DEL	3
#define	INS	4
#define	TWI	5
#define	KIL	6

#define	COP_C	2
#define	REP_C	2
#define	DEL_C	2
#define	INS_C	2
#define	TWI_C	3
#define	KIL_C	3

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
    case TWI:
      print_solu (solu, len1, i - 2, j - 2);
      printf ("twiddle ");
  }
}

void ed (char *src, char *dest)
{
  int len1, len2;

  len1 = strlen (src);
  len2 = strlen (dest);

  int (*memo)[len1 + 1], (*solu)[len1 + 1];
  int i, j;
  int temp, min, kill;

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

      /* Twiddle. */
      if (i >= 2 && j >= 2 && 
	  src[i - 2] == dest[j - 1] && src[i - 1] == dest[j - 2])
      {
        temp = memo[j - 2][i - 2] + TWI_C;
        if (temp < memo[j][i])
        {
	  memo[j][i] = temp;
	  solu[j][i] = TWI;
        }
      }
    }
  }

  min = memo[len2][len1];
  kill = len1;
  for (i = 1; i <= len1 - 1; ++i)
  {
    temp = memo[len2][i] + KIL_C;
    if (temp < min)
    {
      min = temp;
      kill = i;
    }
  }

  printf ("Min: %d\n", min);
  printf ("Operations: ");
  print_solu (solu, len1, kill, len2);
  if (kill != len1)
    printf ("kill");
  printf ("\n");
}

int main ()
{

  ed ("algorithm", "altruistic");

  return 0;
}
