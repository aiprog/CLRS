#include <stdio.h>

void selection_sort(int *arr, int num);

main()
{
    int arr[10];
    int i;

    srand((unsigned)time(NULL));
    for (i = 0; i < 10; ++i)
	arr[i] = rand() % 10;

    selection_sort(arr, 10);
    for (i = 0; i < 10; ++i)
	printf("%d ", arr[i]);
    printf("\n");
}

void selection_sort(int *arr, int num)
{
    int i, j;
    int pos;
    int temp;

    for (i = 0; i < num - 1; ++i)
    {
	pos = i;
	for (j = i + 1; j < num; ++j)
	{
	    if (arr[j] < arr[pos])
		pos = j;
	}

	if (pos != i)
	{
	    temp = arr[pos];
	    arr[pos] = arr[i];
	    arr[i] = temp;
	}
    }
}

