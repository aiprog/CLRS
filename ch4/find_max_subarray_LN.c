#include <stdio.h>

void find_max_subarray(int *arr, int beg, int end, int *left, int *right, int *sum)
{
    int right_sum;
    int max_right_sum;
    int max_right_beg;
    int i;

    *left = beg;
    *right = beg;
    *sum = arr[beg];
    right_sum = 0;
    max_right_sum = 0;
    max_right_beg = -1;
    for (i = beg + 1; i <= end; ++i)
    {
	if (right_sum + arr[i] > 0 && right_sum + *sum > max_right_sum)
	{
	    *sum += right_sum + arr[i];
	    *right = i;
	    right_sum = 0;
	    max_right_sum = 0;
	    max_right_beg = -1;
	}
	else if (max_right_sum + arr[i] > *sum && max_right_sum >= *sum + right_sum)
	{
	    *sum = max_right_sum + arr[i];
	    if (max_right_beg != -1)
	    	*left = max_right_beg;
	    else
		*left = i;
	    *right = i;
	    right_sum = 0;
	    max_right_sum = 0;
	    max_right_beg = -1;
	}
	else
	{
	    right_sum += arr[i];
	    if (max_right_sum == 0)
	    {
		if (arr[i] > 0)
		{
		    max_right_sum = arr[i];
		    max_right_beg = i;
		}
	    }
	    else
	    {
		if (arr[i] > 0) 
		    max_right_sum += arr[i];
		else if (arr[i] < 0)
		{
		    if (max_right_sum + arr[i] > 0)
			max_right_sum += arr[i];
		    else
		    {
			max_right_sum = 0;
			max_right_beg = -1;
		    }
		}
	    }
	}
    }
}   

main()
{
    int left, right, sum;
    int arr[6] = { -5, -3, -1, -4, -6, -7 };

    find_max_subarray(arr, 0, 5, &left, &right, &sum);
    printf("left: %d right: %d sum: %d\n", left, right, sum);
}
