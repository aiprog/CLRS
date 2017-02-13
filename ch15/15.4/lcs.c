#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define	LEN	15000

void print_solu (void *memo, char *str1, char *str2, 
                 int len1, int len2, int i, int j)
{
  int (*arr)[len2 + 1];

  if (i == 0 || j == 0)
    return;

  arr = memo;
  if (str1[i - 1] == str2[j - 1])
  {
    print_solu (memo, str1, str2, len1, len2, i - 1, j - 1);
    printf ("%c ", str1[i - 1]);
  }
  else if (arr[i - 1][j] == arr[i][j])
    print_solu (memo, str1, str2, len1, len2, i - 1, j);
  else
    print_solu (memo, str1, str2, len1, len2, i, j - 1);
}

void lcs_nonrec (char *str1, char *str2, int len1, int len2)
{
  ++len1;
  ++len2;

  int (*memo)[len2] = malloc (sizeof (int) * len1 *len2);
  int i, j;

  for (i = 0; i < len1; ++i)
    memo[i][0] = 0;
  for (j = 0; j < len2; ++j)
    memo[0][j] = 0;

  for (i = 1; i < len1; ++i)
  {
    for (j = 1; j < len2; ++j)
    {
      if (str1[i - 1] == str2[j - 1])
	memo[i][j] = memo[i - 1][j - 1] + 1;
      else if (memo[i - 1][j] > memo[i][j - 1])
	memo[i][j] = memo[i - 1][j];
      else
	memo[i][j] = memo[i][j - 1];
    }
  }

  printf ("Max: %d\n", memo[len1 - 1][len2 - 1]);
  if (LEN < 10)
  {
    print_solu (memo, str1, str2, len1 - 1, len2 - 1, len1 - 1, len2 - 1);
    printf ("\n");
  }
}

void lcs_rec_sub (void *memo, char *str1, char *str2, 
                  int len1, int len2, int i, int j)
{
  int (*arr)[len2 + 1];

  arr = memo;
  if (arr[i][j] >= 0)
    return;

  if (str1[i - 1] == str2[j - 1])
  {
    lcs_rec_sub (memo, str1, str2, len1, len2, i - 1, j - 1);
    arr[i][j] = arr[i - 1][j - 1] + 1;
  }
  else
  {
    lcs_rec_sub (memo, str1, str2, len1, len2, i - 1, j);
    lcs_rec_sub (memo, str1, str2, len1, len2, i, j - 1);

    if (arr[i - 1][j] > arr[i][j - 1])
      arr[i][j] = arr[i - 1][j];
    else
      arr[i][j] = arr[i][j - 1];
  }
}

/*void lcs_rec_sub (void *memo, char *str1, char *str2, 
                  int len1, int len2, int i, int j)
{
  int (*arr)[len2 + 1];
  int k;

  arr = memo;
  if (arr[i][j] >= 0)
    return;

  lcs_rec_sub (memo, str1, str2, len1, len2, i - 1, j);
  for (k = j; k > 0; --k)
  {
    if (str2[k - 1] == str1[i - 1])
      break;
  }
  if (k != 0)
  {
    lcs_rec_sub (memo, str1, str2, len1, len2, i - 1, k - 1);
    arr[i][k] = arr[i - 1][k - 1] + 1;
    if (arr[i][k] > arr[i - 1][j])
      arr[i][j] = arr[i][k];
    else
      arr[i][j] = arr[i - 1][j];
  }
  else
    arr[i][j] = arr[i - 1][j];
}*/

void lcs_rec (char *str1, char *str2, int len1, int len2)
{
  ++len1;
  ++len2;

  int (*memo)[len2] = malloc (sizeof (int) * len1 *len2);
  int i, j;

  for (i = 0; i < len1; ++i)
    memo[i][0] = 0;
  for (j = 0; j < len2; ++j)
    memo[0][j] = 0;
  for (i = 1; i < len1; ++i)
  {
    for (j = 1; j < len2; ++j)
      memo[i][j] = -1;
  }

  lcs_rec_sub (memo, str1, str2, len1 - 1, len2 - 1, len1 - 1, len2 - 1);

  printf ("Max: %d\n", memo[len1 - 1][len2 - 1]);
  if (LEN < 10)
  {
    print_solu (memo, str1, str2, len1 - 1, len2 - 1, len1 - 1, len2 - 1);
    printf ("\n");
  }
}

int main ()
{
  char *str1, *str2;
  int beg, end;
  int i;

  str1 = malloc (sizeof (char) * LEN);
  str2 = malloc (sizeof (char) * LEN);

  srand (time (NULL));
  for (i = 0; i < LEN; ++i)
  {
    str1[i] = rand () % 4 + 'A';
    str2[i] = rand () % 4 + 'A';
  }

  beg = clock ();
  lcs_nonrec (str1, str2, LEN, LEN);
  end = clock ();
  printf ("Time: %dms\n", (end - beg) / 1000);

  beg = clock ();
  lcs_rec (str1, str2, LEN, LEN);
  end = clock ();
  printf ("Time: %dms\n", (end - beg) / 1000);

  return 0;
}
