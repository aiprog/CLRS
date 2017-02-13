#include <stdio.h>

int hash_str_to_int (char *key, int m)
{
  long long dividend, remainder;	/* 64 bit integer. */
  char *ptr;
  int i;

  remainder = 0;
  ptr = key;
  do
  {
    dividend = 0;
    for (i = 0; *ptr != '\0' && i != 4; ++i)
    {
      dividend = 128 * dividend + *ptr;
      ++ptr;
    }

    remainder <<= i * 7;
    dividend += remainder;
    remainder = dividend % m;

  } while (i == 4);

  return remainder;
}

int main ()
{
  int value;

  value = hash_str_to_int ("hello world!\n", 1234567);
  printf ("%d\n", value);

  return 0;
}
