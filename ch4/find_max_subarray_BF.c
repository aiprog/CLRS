#include <stdio.h>

void find_max_subarray(int *arr, int beg, int end, int *left, int *right, int *sum)
{
    int cur_sum;
    int i, j;

    *left = beg;
    *right = beg;
    *sum = arr[beg];
    for (i = beg; i <= end; ++i)
    {
	cur_sum = 0;
	for (j = i; j <= end; ++j)
	{
	    cur_sum += arr[j];
	    if (cur_sum > *sum)
	    {
		*left = i;
		*right = j;
		*sum = cur_sum;
	    }
	}
    }
}


main()
{
    int left, right, sum;
    int arr[16] = { 13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7 };

    find_max_subarray(arr, 0, 15, &left, &right, &sum);
    printf("left: %d right: %d sum: %d\n", left, right, sum);
}
