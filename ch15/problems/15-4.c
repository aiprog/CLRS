#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#define CUBE(x) ((x) * (x) * (x))

#define LEN	10
#define SIZE	50
#define	WIDTH	30

void print_solu (char **words, int size, int *solu, int pos)
{
  int i;

  if (pos == 0)
    return;

  print_solu (words, size, solu, pos - solu[pos]);
  for (i = pos - solu[pos] + 1; i <= pos; ++i)
  {
    printf ("%s", words[i - 1]);
    if (i < pos)
      printf (" ");
  }
  printf ("\n");
}

void pn (char **words, int size, int width)
{
  int *len, *memo, *solu;
  int sum, temp, min;
  int i, j;

  len = malloc (sizeof (int) * size);
  memo = malloc (sizeof (int) * (size + 1));
  solu = malloc (sizeof (int) * (size + 1));

  for (i = 0; i < size; ++i)
    len[i] = strlen (words[i]);

  memo[0] = 0;
  solu[0] = 0;
  for (i = 1; i <= size; ++i)
  {
    sum = len[i - 1];
    memo[i] = memo[i - 1] + CUBE (width - sum);
    solu[i] = 1;
    for (j = i - 1; j >= 1; --j)
    {
      sum += len[j - 1] + 1;
      if (sum > width)
	break;

      temp = memo[j - 1] + CUBE (width - sum);
      if (temp < memo[i])
      {
	memo[i] = temp;
	solu[i] = i - j + 1;
      }
    }
  }

  sum = len[size - 1];
  min = memo[size - 1];
  temp = size;
  for (i = size - 1; i >= 1; --i)
  {
    sum += len[i - 1] + 1;
    if (sum > width)
      break;

    if (memo[i - 1] < min)
    {
      min = memo[i - 1];
      temp = i;
    }
  }

  printf ("Min: %d\n\n", min);
  for (i = 0; i < WIDTH; ++i)
    printf ("#");
  printf ("\n");
  print_solu (words, size, solu, temp - 1);
  for (i = temp; i <= size; ++i)
  {
    printf ("%s", words[i - 1]);
    if (i < size)
      printf (" ");
  }
  printf ("\n");
}

int main ()
{
  char **words;
  int len;
  int i, j;

  srand (time (NULL));
  words = malloc (sizeof (char *) * SIZE);
  for (i = 0; i < SIZE; ++i)
  {
    len = rand () % LEN + 1;
    words[i] = malloc (sizeof (char) * (len + 1));
    for (j = 0; j < len; ++j)
      words[i][j] = 'a' + rand () % 26;
    words[i][len] = '\0';
  }

  pn (words, SIZE, WIDTH);

  return 0;
}
