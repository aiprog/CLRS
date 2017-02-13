#include <stdio.h>
#include <stdlib.h>

void shell_sort(int *arr, int n)
{
    int i, j, k;
    int step;
    int tmp;

    for (step = n / 2; step > 0; step /= 2)
    {
	for (i = 0; i < step; ++i)
	{
	    for (j = i + step; j < n; j += step)
	    {
		k = j - step;
		tmp = arr[j];
		while (k >= i && arr[k] > tmp)
		{
		    arr[k + step] = arr[k];
		    k -= step;
		}
		arr[k + step] = tmp;
	    }
	}
    }
}

main()
{
    int *arr;
    int num;
    int i;

    srand((unsigned)time(NULL));
    while (1)
    {
	scanf("%d", &num);
	if (num == 0)
	    break;

	arr = malloc(sizeof(int) * num);
	for (i = 0; i < num; ++i)
	    arr[i] = rand() % num;

	shell_sort(arr, num);
	for(i = 0; i < num; ++i)
	    printf("%d ", arr[i]);
	printf("\n");
    }
}

