#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

main()
{
    int a, *bin_a;
    int b, *bin_b;
    int c, *bin_c;
    int bits;
    int tmp;
    int i;
    int flg;

    scanf("%d %d", &a, &b);
    tmp = (a > b)? a:b;
    bits = 1;
    while (tmp / 2)
    {
	++bits;
	tmp /= 2;
    }

    bin_a = malloc(sizeof(int) * bits);
    bin_b = malloc(sizeof(int) * bits);
    bin_c = malloc(sizeof(int) * (bits + 1));

    i = 0;
    tmp = a;
    while (tmp != 0)
    {
	bin_a[i++] = tmp % 2;
	tmp /= 2;
    }
    memset(bin_a + i, 0 , sizeof(int) * (bits - i));

    i = 0;
    tmp = b;
    while (tmp != 0)
    {
	bin_b[i++] = tmp % 2;
	tmp /= 2;
    }
    memset(bin_b + i, 0, sizeof(int) * (bits - i));

    flg = 0;
    for (i = 0; i < bits; ++i)
    {
	tmp = bin_a[i] + bin_b[i] + flg;
	if (tmp == 0 || tmp == 1)
	{
	    bin_c[i] = tmp;
	    flg = 0;
	}
	else if (tmp == 2)
	{
	    bin_c[i] = 0;
	    flg = 1;
	}
	else
	{
	    bin_c[i] = 1;
	    flg = 1;
	}
    }
    bin_c[bits] = flg;

    c = 0;
    for (i = bits; i >= 0; --i)
	c = 2 * c + bin_c[i];

    printf("c: %d\n", c);
}
