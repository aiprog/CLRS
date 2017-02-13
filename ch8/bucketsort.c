/* Bucket sort. */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node
{
  int value;
  struct node *next;
} NODE;

void bucketsort (int *arr, int len, int min, int max)
{
  NODE *tmp_arr, *tmp_list, *tmp_node, *node_ptr;
  int pos;
  int i, j;

  tmp_arr = malloc (sizeof (NODE) * len);
  /* Initialize head nodes. */
  for (i = 0; i < len; ++i)
    tmp_arr[i].next = NULL;

  /* Increase max by one, otherwise arr[i] may
   * map to n in tmp_arr. */
  ++max;
  /* Allocate all the memory for the lists at a time
   * to save time. */
  tmp_list = malloc (sizeof (NODE) * len);
  j = 0;
  for (i = 0; i < len; ++i)
  {
    /* Map arr[i] to tmp_arr[pos]. */
    pos = (int)((long)(arr[i] - min) * len / (max - min));
    node_ptr = &tmp_arr[pos];
    /* When insert, sort the list tmp_arr[pos] at the same time. */
    while (node_ptr->next != NULL && arr[i] > node_ptr->next->value)
      node_ptr = node_ptr->next;
    tmp_node = &tmp_list[j++];
    tmp_node->value = arr[i];
    tmp_node->next = node_ptr->next;
    node_ptr->next = tmp_node;
  }

  /* Concatenate the lists into arr. */
  j = 0;
  for (i = 0; i < len; ++i)
  {
    node_ptr = tmp_arr[i].next;
    while (node_ptr != NULL)
    {
      arr[j++] = node_ptr->value;
      node_ptr = node_ptr->next;
    }
  }

  free (tmp_arr);
  free (tmp_list);
}

#define NUM 10000000 

int main ()
{
  int *arr = malloc (sizeof (int) * NUM);
  int i;

  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
    arr[i] = 1000 + rand () % NUM;

  long beg = clock ();
  bucketsort (arr, NUM, 1000, 1000 + NUM - 1);
  long end = clock ();

  printf ("Time: %ld\n", (end - beg) / 1000);

  if (NUM <= 20)
  {
    for (i = 0; i < NUM; ++i)
      printf ("%d ", arr[i]);
    printf ("\n");
  }

  free (arr);

  return 0;
}
