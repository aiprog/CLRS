#include <stdio.h>

int binary_search(int *arr, int beg, int end, int key)
{
    int mid;
    int pos;

    if (beg > end)
	return -1;

    mid = (beg + end) / 2;
    if(key == arr[mid])
	return mid;
    else if (key < arr[mid])
    {
	if ((pos = binary_search(arr, beg, mid - 1, key)) != -1)
	    return pos;
    }
    else
	return binary_search(arr, mid + 1, end, key);
}

main()
{
    int key;
    int arr[5] = { 0, 1, 2, 3, 4 };

    while (1)
    {
	scanf("%d", &key);
	if (key == -1)
	    break;

	printf("%d\n", binary_search(arr, 0, 4, key));
    }
}
