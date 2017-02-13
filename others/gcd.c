/*
 * gcd.c
 *
 * 	Given a and b, find x, y and z so that ax + by = z, and z is the
 * 	greatest common divisor of x and y.
 */

#include <stdio.h>
#include <stdlib.h>

void gcd (int a, int b, int *x, int *y, int *z)
{
  int x0, x1, y0, y1, q, r, cnt;

  if ((r = a % b) == 0)
  {
    *x = 0; *y = 1; *z = b;
    return;
  }

  x0 = 0; x1 = 1;
  y0 = 1; y1 = a / b;

  cnt = 1; a = b; b = r;
  *x = 1; *y = y1; *z = r;
  while ((r = a % b) != 0)
  {
    q = a / b;
    *x = q * x1 + x0;
    x0 = x1; x1 = *x;
    *y = q * y1 + y0;
    y0 = y1; y1 = *y;

    a = b; b = r;
    ++cnt;
  }

  if (cnt % 2 == 1)
    *y = -*y;
  else
    *x = -*x;
  *z = b;
}

int main ()
{
  int x, y, z;

  gcd (140, 30, &x, &y, &z);
  printf ("%d %d %d\n", x, y, z);

  return 0;
}
