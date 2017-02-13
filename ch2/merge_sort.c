#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

void merge(int *arr, int beg, int mid, int end)
{
    int i, j;
    int len_l, len_r;
    int *arr_l;
    int *arr_r;

    len_l = mid - beg + 1;
    len_r = end - mid;

    arr_l = malloc(sizeof(int) * len_l);
    arr_r = malloc(sizeof(int) * len_r);

    j = 0;
    for (i = beg; i < mid + 1; ++i)
	arr_l[j++] = arr[i];

    j = 0;
    for (i = mid + 1; i < end + 1; ++i)
	arr_r[j++] = arr[i];

    i = 0;
    j = 0;
    while (i < len_l && j < len_r)
    {
	if (arr_l[i] <= arr_r[j])
	    arr[beg++] = arr_l[i++];
	else
	    arr[beg++] = arr_r[j++];
    }

    while (i < len_l)
	arr[beg++] = arr_l[i++];

    while (j < len_r)
	arr[beg++] = arr_r[j++];
}

void merge_sort(int *arr, int beg, int end)
{
    int mid;

    if (beg < end)
    {
	mid = (beg + end) / 2;

	merge_sort(arr, beg, mid);
	merge_sort(arr, mid + 1, end);

	merge(arr, beg, mid, end);
    }
}

main()
{
    int num;
    int *arr;
    int i;

    while (1)
    {
	scanf("%d", &num);
	if (num == 0)
	    break;

	arr = malloc(sizeof(int) * num);
	srand((unsigned)time(NULL));
	for (i = 0; i < num; ++i)
	    arr[i] = rand() % num;

	merge_sort(arr, 0, num - 1);
	for (i = 0; i < num; ++i)
	    printf("%d ", arr[i]);
	printf("\n");
	free(arr);
    }
}
