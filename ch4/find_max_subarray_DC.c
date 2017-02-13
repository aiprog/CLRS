#include <stdio.h>

void find_max_crossing_subarray(int *arr, int beg, int end, int *left, int *right, int *sum)
{
    int mid;
    int left_sum, right_sum;
    int max_left_sum, max_right_sum;
    int i;

    mid = (beg + end) / 2;

    *left = mid;
    left_sum = arr[mid];
    max_left_sum = arr[mid];
    for (i = mid - 1; i >= beg; --i)
    {
	left_sum += arr[i];
	if (left_sum > max_left_sum)
	{
	    max_left_sum = left_sum;
	    *left = i;
	}
    }

    *right = mid + 1;
    right_sum = arr[mid + 1];
    max_right_sum = arr[mid + 1];
    for (i = mid + 2; i <= end; ++i)
    {
	right_sum += arr[i];
	if (right_sum > max_right_sum)
	{
	    max_right_sum = right_sum;
	    *right = i;
	}
    }

    *sum = max_left_sum + max_right_sum;
}

void find_max_subarray(int *arr, int beg, int end, int *left, int *right, int *sum)
{
    int mid;
    int left_left, left_right, right_left, right_right, cross_left, cross_right;
    int max_left_sum, max_right_sum, max_cross_sum;

    if (beg == end)
    {
	*left = beg;
	*right = end;
	*sum = arr[beg];

	return;
    }

    mid = (beg + end) / 2;

    find_max_subarray(arr, beg, mid, &left_left, &left_right, &max_left_sum);
    find_max_subarray(arr, mid + 1, end, &right_left, &right_right, &max_right_sum);
    find_max_crossing_subarray(arr, beg, end, &cross_left, &cross_right, &max_cross_sum);

    if (max_left_sum > max_right_sum && max_left_sum > max_cross_sum)
    {
	*left = left_left;
	*right = left_right;
	*sum = max_left_sum;
    }
    else if (max_right_sum > max_left_sum && max_right_sum > max_cross_sum)
    {
	*left = right_left;
	*right = right_left;
	*sum = max_right_sum;
    }
    else
    {
	*left = cross_left;
	*right = cross_right;
	*sum = max_cross_sum;
    }
}

main()
{
    int left, right, sum;
    int arr[16] = { 13, -3, -25, 20, -3, -16, -23, 18, 20, -7, 12, -5, -22, 15, -4, 7 };

    find_max_subarray(arr, 0, 15, &left, &right, &sum);
    printf("left: %d right: %d sum: %d\n", left, right, sum);
}
