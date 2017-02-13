#include <stdio.h>
#include <stdlib.h>

#define SIZE 14

typedef struct _node
{
  int hall_num;
  struct _node *next;
} Node;

Node *new_node (int hall_num, Node *next)
{
  Node *node;

  node = malloc (sizeof (Node));
  node->hall_num = hall_num;
  node->next = next;

  return node;
}

void quick_sort (int *endpoint, int *arr, int beg, int end)
{
  int temp;
  int i, j;
 
  if (end <= beg)
    return;

  i = beg;
  j = end;
  temp = arr[beg];
  while (j > i)
  {
    while (j > i && (endpoint[arr[j]] > endpoint[temp] || 
	             (endpoint[arr[j]] == endpoint[temp] && 
		     arr[j] % 2 == 0 && temp % 2 == 1)))
      --j;
    arr[i++] = arr[j];

    while (j > i && (endpoint[arr[i]] < endpoint[temp] ||
	             (endpoint[arr[i]] == endpoint[temp] &&
		     arr[i] % 2 == 1 && temp % 2 == 0)))
      ++i;
    arr[j--] = arr[i];
  }

  if (i != j)
    ++j;
  arr[j] = temp;

  quick_sort (endpoint, arr, beg, j - 1);
  quick_sort (endpoint, arr, j + 1, end);
}

void asp (int *endpoint, int *arr)
{
  Node *node[SIZE / 2], *head;
  int i;

  quick_sort (endpoint, arr, 0, SIZE - 1);

  head = new_node (SIZE - 1, NULL);
  for (i = SIZE - 2; i >= 0; --i)
    head = new_node (i, head);

  for (i = 0; i < SIZE; ++i)
  {
    if (arr[i] % 2 == 0)
    {
      node[arr[i] / 2] = head;
      head = head->next;
    }
    else
    {
      node[(arr[i] - 1) / 2]->next = head;
      head = node[(arr[i] - 1) / 2];
    }
  }

  for (i = 0; i < SIZE / 2; ++i)
    printf ("%d ", node[i]->hall_num);
  printf ("\n");
}

int main ()
{
  int endpoint[SIZE] = { 1, 4, 4, 6, 2, 4, 4, 8, 5, 7, 7, 8, 6, 7 };
  int arr[SIZE] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

  asp (endpoint, arr);

  return 0;
}
