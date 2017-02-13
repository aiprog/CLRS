#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM 10000000
#define LEN 10

typedef struct _list
{
  char *str;
  struct _list *next;
} LIST;

typedef struct _node
{
  LIST list;
  int cnt;
  struct _node *next[26];
} NODE;

/* Insert a string into the tree. */

void insert_node (NODE *head, char *str)
{
  LIST *new_node;
  int i;

  /* When string segmentation str[0...i] is
   * already in the tree, go to str[0...i+1],
   * otherwise create a new node for str[i]. */
  for (i = 0; str[i] != '\0'; ++i)
  {
    if (head->next[str[i] - 'a'] == NULL)
    {
      head->next[str[i] - 'a'] = malloc (sizeof (NODE));
      memset (head->next[str[i] - 'a'], 0, sizeof (NODE));
    }
    head = head->next[str[i] - 'a'];
  }

  /* Append 'str' to node 'head'. */
  new_node = malloc (sizeof (LIST));
  new_node->str = str;
  new_node->next = head->list.next;
  head->list.next = new_node;
  ++head->cnt;
}

/* Preorder traversal the tree. */

void pre_traversal (NODE *head, char **arr, int *pos)
{
  LIST *ptr;
  int i;

  if (head == NULL)
    return;

  /* Place strings in node 'head' into 'arr'. */
  if (head->cnt != 0)
  {
    ptr = head->list.next;
    for (i = *pos + head->cnt - 1; i >= *pos; --i)
    {
      arr[i] = ptr->str;
      ptr = ptr->next;
    }
    *pos += head->cnt;
  }

  /* Call 'pre_traversal' on subtrees recursively. */
  for (i = 0; i < 26; ++i)
    pre_traversal (head->next[i], arr, pos);
}

/* Free the tree. */

void free_tree (NODE *head)
{
  LIST *ptr, *del;
  int i;

  if (head == NULL)
    return;

  for (i = 0; i < 26; ++i)
    free_tree (head->next[i]);

  if (head->cnt != 0)
  {
    ptr = head->list.next;
    while (ptr != NULL)
    {
      del = ptr;
      ptr = ptr->next;
      free (del);
    }
  }

  free (head);
}

int main ()
{
  char **arr;
  int len, pos;
  int i;
  NODE *head;

  arr = malloc (sizeof (char*) * NUM);
  /* Generate test data and print them. */
  srand (time (NULL));
  for (i = 0; i < NUM; ++i)
  {
    len = rand () % LEN + 1;
    arr[i] = malloc (sizeof (char) * (len + 1));
    arr[i][len] = '\0';
    while (--len > -1)
      arr[i][len] = 'a' + rand () % 26;
    if (NUM <= 100)
      printf ("%s ", arr[i]);
  }
  if (NUM <= 100)
    printf ("\n");

  long beg = clock ();
  /* Construct dictionary tree. */
  head = malloc (sizeof (NODE));
  memset (head, 0, sizeof (NODE));
  for (i = 0; i < NUM; ++i)
    insert_node (head, arr[i]);
  long mid = clock ();
  printf ("Time: %ld\n", (mid - beg) / 1000);

  /* Preorder traversal the tree to sort the arr. */
  pos = 0;
  pre_traversal (head, arr, &pos); 
  long end = clock ();
  printf ("Time: %ld\n", (end - mid) / 1000);

  /* Print the result. */
  if (NUM <= 100)
  {
    for (i = 0; i < NUM; ++i)
      printf ("%s ", arr[i]);
    printf ("\n");
  }

  /* Free the memory. */
  free_tree (head);
  for (i = 0; i < NUM; ++i)
    free(arr[i]);
  free (arr);

  return 0;
}
