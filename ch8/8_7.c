#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROW 100000
#define COL 10

/* Quick sort. If 'flg' isn't equal to 0, the result array is increasing,
 * otherwise the result array is decreasing. */
void quick_sort (int (*arr)[COL], int col, int beg, int end, int flg)
{
  int pivot, mid, tmp;
  int i, j;

  if (beg >= end)
    return;

  /* Prevent worst case when 'arr' is in order already. */
  mid = (beg + end) / 2;
  if (1.0 * (arr[beg][col] - arr[mid][col]) *
      (arr[beg][col] - arr[end][col]) < 0)
    pivot = beg;
  else if (1.0 * (arr[mid][col] - arr[beg][col]) *
           (arr[mid][col] - arr[end][col]) < 0)
    pivot = mid;
  else
    pivot = end;
  if (pivot != beg)
  {
    tmp = arr[beg][col];
    arr[beg][col] = arr[pivot][col];
    arr[pivot][col] = tmp;
  }

  /* Partition. */
  pivot = arr[beg][col];
  i = beg;
  j = end;
  while (i < j)
  {
    while (i < j && (flg && arr[j][col] >= pivot || !flg && arr[j][col] <= pivot))
      --j;
    arr[i][col] = arr[j][col];

    while (i < j && (flg && arr[i][col] <= pivot || !flg && arr[i][col] >= pivot))
      ++i;
    arr[j][col] = arr[i][col];
  }
  arr[i][col] = pivot;

  /* Divide and conquer. */
  quick_sort (arr, col, beg, i - 1, flg);
  quick_sort (arr, col, i + 1, end, flg);
}

/* Insert sort. */
void insert_sort (int (*arr)[COL], int col, int beg, int end)
{
  int tmp;
  int i, j;

  for (i = beg + 1; i <= end; ++i)
  {
    tmp = arr[i][COL];
    j = i - 1;
    while (j >= beg && arr[j][COL] > tmp)
      --j;
    arr[j + 1][COL] = tmp;
  }
}

/* Print the array. */
void print (int (*arr)[COL])
{
  int i, j;

  for (i = 0; i < ROW; ++i)
  {
    for (j = 0; j < COL; ++j)
      printf ("%5d", arr[i][j]);
    printf ("\n");
  }
  printf ("\n");
}

int main ()
{
  int (*arr)[COL], (*arr_tmp)[COL], *half_tmp;
  int tmp, flg;
  int i, j, k;

  /* Generate test data and print them. */
  arr = malloc (sizeof (int) * ROW * COL);
  srand (time (NULL));
  k = 0;
  for (i = 0; i < ROW; ++i)
    for (j = 0; j < COL; ++j)
      arr[i][j] = k++;
  for (i = 0; i < ROW; ++i)
    for (j = 0; j < COL; ++j)
    {
      k = rand () % (ROW * COL);
      tmp = arr[i][j];
      arr[i][j] = arr[k / COL][k % COL];
      arr[k / COL][k % COL] = tmp;
    }
  if (ROW * COL <= 100)
    print (arr);

  int beg = clock ();
  /* Step 1. */
  flg = 1;
  for (i = 0; i < COL; ++i)
  {
    quick_sort (arr, i, 0, ROW - 1, flg);
    flg = !flg;
  }

  /* Step 2. */
  arr_tmp = malloc (sizeof (int) * ROW * COL);
  k = 0;
  while (k < ROW * COL)
  {
      arr_tmp[k / COL][k % COL] = arr[k % ROW][k / ROW];
      ++k;
  }

  /* Step 3. */
  for (i = 0; i < COL; ++i)
    quick_sort (arr_tmp, i, 0, ROW - 1, 1);

  /* Step 4. */
  k = 0;
  while (k < ROW * COL)
  {
      arr[k % ROW][k / ROW] = arr_tmp[k / COL][k % COL];
      ++k;
  }

  /* Step 5. */
  for (i = 0; i < COL; ++i)
    quick_sort (arr, i, 0, ROW - 1, 1);

  /* Step 6. */
  half_tmp = malloc (sizeof (int) * (ROW - ROW / 2));
  for (i = ROW / 2, j = 0; i < ROW; ++i, ++j)
    half_tmp[j] = arr[i][COL - 1];
  k = ROW * COL - (ROW - ROW / 2) - 1;
  j = ROW * COL - 1;
  while (k >= ROW / 2)
  {
    arr[j % ROW][j / ROW] = arr[k % ROW][k / ROW];
    --k;
    --j;
  }

  /* Step 7. */
  for (i = 1; i < COL; ++i)
    quick_sort (arr, i, 0, ROW - 1, 1);

  /* Step 8. */
  k = ROW;
  j = ROW / 2;
  while (k < ROW * COL)
  {
    arr[j % ROW][j / ROW] = arr[k % ROW][k / ROW];
    ++k;
    ++j;
  }
  for (i = ROW / 2, j = 0; i < ROW; ++i, ++j)
    arr[i][COL - 1] = half_tmp[j];
  int end = clock ();

  /* Print the time consumption. */
  printf ("Time: %d\n", (end - beg) / 1000);

  /* Print the result. */
  if (ROW * COL <= 100)
    print (arr);

  /* Check the result. */
  k = 0;
  for (i = 0; i < COL; ++i)
    for (j = 0; j < ROW; ++j, ++k)
      if (arr[j][i] != k)
      {
	printf ("\033[0;32;31m***ERROR***\033[m\n");
	exit (1);
      }

  /* Free the memory. */
  free (arr);
  free (arr_tmp);
  free (half_tmp);

  return 0;
}
