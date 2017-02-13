#include <stdio.h>
#include <stdlib.h>

void print_solu (int *seq, int *link, int end)
{
  if (end == 0)
  {
    printf ("%d ", seq[0]);
    return;
  }
  else
  {
    print_solu (seq, link, link[end]);
    printf ("%d ", seq[end]);
  }
}

void lmis (int *seq, int size)
{
  int *memo, *link;
  int beg, mid, end;
  int ptr;
  int i;

  memo = malloc (sizeof (int) * size);
  link = malloc (sizeof (int) * size);

  memo[0] = 0;
  link[0] = -1;
  ptr = 0;
  for (i = 1; i < size; ++i)
  {
    if (seq[i] >= seq[memo[ptr]])
    {
      link[i] = memo[ptr];
      memo[++ptr] = i;
    }
    else if (seq[i] < seq[memo[0]])
      memo[0] = i;
    else
    {
      beg = 0;
      end = ptr;
      while (1)
      {
	mid = (beg + end) / 2;
	if (seq[i] >= seq[memo[mid]] && seq[i] < seq[memo[mid + 1]])
	{
	  memo[mid + 1] = i;
	  link[i] = memo[mid];
	  break;
	}
	else if (seq[i] < seq[memo[mid]])
	  end = mid;
	else
	  beg = mid + 1;
      }
    }
  }

  printf ("Max: %d\n", ptr + 1);
  print_solu (seq, link, memo[ptr]);
  printf ("\n");
}

int main ()
{
  int seq[10] = { 1, 3, 3, 3, 4, 4, 2, 3, 4, 5 };

  lmis (seq, 10);

  return 0;
}
