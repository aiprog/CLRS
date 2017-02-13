#include <stdio.h>
#include <stdlib.h>

void find_pos (int (*item)[2], int *arr, int beg, int end, int pos)
{
  int temp;
  int i, j;

  temp = arr[beg];
  i = beg;
  j = end;
  while (i < j)
  {
    while (i < j && item[arr[j]][1] > item[temp][1])
      --j;
    arr[i++] = arr[j];

    while (i < j && item[arr[i]][1] < item[temp][1])
      ++i;
    arr[j--] = arr[i];
  }
  if (i != j)
    ++j;
  arr[j] = temp;

  if (j == pos)
    return;
  else if (j < pos)
    find_pos (item, arr, j + 1, end, pos);
  else
    find_pos (item, arr, beg, j - 1, pos);
}

int fra_knapsack (int (*item)[2], int *arr, int beg, int end, int weight)
{
  int mid, mid_beg, mid_end;
  int weight_equal, weight_greater;
  int value, temp;
  int i, j;

  if (beg > end)
    return 0;

  mid = (beg + end) / 2;
  find_pos (item, arr, beg, end, mid);

  weight_equal = item[arr[mid]][0];
  i = beg;
  j = mid - 1;
  while (i < j)
  {
    while (i < j && item[arr[i]][1] != item[arr[mid]][1])
      ++i;

    while (i < j && item[arr[j]][1] == item[arr[mid]][1])
      --j;
    if (i == j)
      break;
   
    temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }

  mid_beg = mid;
  while (mid_beg > beg && item[arr[mid_beg - 1]][1] == item[arr[mid]][1])
  {
    --mid_beg;
    weight_equal += item[arr[mid_beg]][0];
  }

  mid_end = mid;
  i = mid + 1;
  j = end;
  while (i < j)
  {
    while (i < j && item[arr[i]][1] == item[arr[mid]][1])
      ++i;
  
    while (i < j && item[arr[j]][1] != item[arr[mid]][1])
      --j;
    if (i == j)
      break;

    temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }

  mid_end = mid;
  while (mid_end < end && item[arr[mid_end + 1]][1] == item[arr[mid]][1])
  {
    ++mid_end;
    weight_equal += item[arr[mid_end]][0];
  }

  weight_greater = 0;
  for (i = mid_end + 1; i <= end; ++i)
    weight_greater += item[arr[i]][0];

  if (weight < weight_greater)
    return fra_knapsack (item, arr, mid_end + 1, end, weight);
  else if (weight <= weight_greater + weight_equal)
  {
    value = 0;
    for (i = end; i >= mid_beg; --i)
    {
      if (item[arr[i]][0] < weight)
      {
	printf ("Item %d Weight: %d\n", arr[i], item[arr[i]][0]);
	weight -= item[arr[i]][0];
	value += item[arr[i]][1] * item[arr[i]][0];
      }
      else
      {
	printf ("Item %d Weight: %d\n", arr[i], weight);
	value += weight * item[arr[i]][1];

	return value;
      }
    }
  }
  else
  {
    value = 0;
    for (i = end; i >= mid_beg; --i)
    {
      printf ("Item %d Weight: %d\n", arr[i], item[arr[i]][0]);
      value += item[arr[i]][1] * item[arr[i]][0];
    }

    return value + fra_knapsack (item, arr, beg, mid_beg - 1, 
	                         weight - weight_greater - weight_equal);
  }

  return 0;
}

int main ()
{
  int item[14][2] = { 
                      { 5, 5 }, { 4, 4 }, { 3, 6 }, { 4, 6 }, { 5, 4 },
                      { 3, 6 }, { 3, 5 }, { 6, 4 }, { 5, 4 }, { 4, 4 },
		      { 5, 5 }, { 6, 4 }, { 3, 5 }, { 4, 5 }
                    };
  int arr[14] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
  int value;

  value = fra_knapsack (item, arr, 0, 13, 50);
  printf ("Max: %d\n", value);

  return 0;
}
