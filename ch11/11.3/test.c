#include <stdio.h>

int main ()
{
  int arr[5] = { 61, 62, 63, 64, 65 };
  float tmp;
  int i;

  for (i = 0; i < 5; ++i)
  {
    tmp = arr[i] * 0.61803398874;
    tmp -= (int) tmp;
    printf ("%d ", (int) (1000 * tmp));
  }
  printf ("\n");
  return 0;
}
