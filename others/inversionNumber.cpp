#include <iostream>
#include <cstring>

using std::cout;
using std::endl;

int inversionNumber (int *arr, int beg, int end)
{
  if (beg == end)
    return 0;

  // Conquer a[0] to a[i / 2] and a[i / 2 + 1] to a[i] recursively
  int mid = (beg + end) / 2;
  int sum = inversionNumber (arr, beg, mid) + 
            inversionNumber (arr, mid + 1, end);

  int *tmp = new int[end - beg + 1];
  int i = beg, j = mid + 1, k = 0;

  // Count inversed pairs between a[0] to a[i / 2] and a[i / 2 + 1] to a[i]
  while (i <= mid && j <= end)
  {
    if (arr[i] <= arr[j])
      tmp[k++] = arr[i++];
    else
    {
      tmp[k++] = arr[j++];
      sum += mid - i + 1;
    }
  }

  if (i <= mid)
    memcpy (tmp + k, arr + i, end - beg + 1 - k);
  else
    memcpy (tmp + k, arr + j, end - beg + 1 - k);

  memcpy (arr + beg, tmp, end - beg + 1);
  delete tmp;

  return sum;
}

int main ()
{
  int arr[12] = { 3, 3, 1, 1, 2, 2, 4, 5, 6, 4, 5, 5 };
  printf ("Inversion number: %d\n", inversionNumber (arr, 0, 11));

  return 0;
}