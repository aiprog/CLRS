#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _node
{
  int key;
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} node;

int path[100];
int len = -1;

void bin_tree (node *head, int *nodes, int *viables)
{
  node *tmp;
  int i;

  for (i = 0; i < 2; ++i)
  {
    if (*nodes == 0)
      return;

    if (*viables > 1 && rand () % 3 == 0)
    {
      tmp = NULL;
      --*viables;
    }
    else
    {
      tmp = malloc (sizeof (node));
      tmp->key = *nodes;
      tmp->parent = head;
      tmp->left = NULL;
      tmp->right = NULL;

      ++*viables;
      --*nodes;
    }

    if (i == 0)
      head->left = tmp;
    else
      head->right = tmp;

    if (tmp != NULL)
      bin_tree (tmp, nodes, viables);
  }
}

node *generate_bin_tree (int nodes)
{
  node *head;
  int viables;

  srand (time (NULL));
  head = malloc (sizeof (node));
  head->key = nodes--;
  head->parent = head->left = head->right = NULL;
  viables = 2;
  bin_tree (head, &nodes, &viables);

  return head;
}

void print_inorder_rec (node *head)
{
  if (head == NULL)
    return;

  print_inorder_rec (head->left);
  printf ("%d ", head->key);
  print_inorder_rec (head->right);
}

void print_postorder_rec (node *head)
{
  if (head == NULL)
    return;

  print_postorder_rec (head->left);
  print_postorder_rec (head->right);
  printf ("%d ", head->key);
}

void print_paths_rec (node *head)
{
  int i;

  path[++len] = head->key;

  if (head->left == NULL && head->right == NULL)
  {
    for (i = 0; i <= len; ++i)
      printf ("%d ", path[i]);
    printf ("\n");
  }
  else
  {
    if (head->left != NULL)
      print_paths_rec (head->left);
    if (head->right != NULL)
      print_paths_rec (head->right);
  }

  --len;
}

void print_inorder_stk (node *head)
{
  node *stack[100];
  int top;

  stack[0] = head;
  top = 0;
  while (1)
  {
    while (stack[top]->left != NULL)
    {
      stack[top + 1] = stack[top]->left;
      ++top;
    }

    while (top > -1 && stack[top]->right == NULL)
    {
      printf ("%d ", stack[top]->key);
      --top;
    }

    if (top != -1)
    {
      printf ("%d ", stack[top]->key);
      stack[top] = stack[top]->right;
    }
    else
      break;
  }
}

void print_postorder_stk (node *head)
{
  node *stack[100];
  int top;

  stack[0] = head;
  top = 0;
  while (1)
  {
    while (stack[top]->left != NULL)
    {
      stack[top + 1] = stack[top]->left;
      ++top;
    }

    if (stack[top]->right == NULL)
    {
      while (top > -1 && (stack[top]->right == NULL || 
	                  stack[top]->right == stack[top + 1]))
      {
        printf ("%d ", stack[top]->key);
        --top;
      }
    }

    if (top != -1)
    {
      stack[top + 1] = stack[top]->right;
      ++top;
    }
    else
      break;
  }
}

void print_inorder (node *head)
{
  node *cur;

  cur = head;
  while (1)
  {
    while (cur->left != NULL)
      cur = cur->left;
    printf ("%d ", cur->key);

    if (cur->right == NULL)
    {
      if (cur == head)
	break;
      
      while (cur->parent != head && (cur->parent->right == NULL || 
	                             cur->parent->right == cur))
      {
        cur = cur->parent;
	if (cur->right == NULL)
          printf ("%d ", cur->key);
      }
      
    }
    else
    {
      cur = cur->right;
      continue;
    }

    if (cur->parent != head || (head->right != cur && head->right != NULL))
    {
      printf ("%d ", cur->parent->key);
      cur = cur->parent->right;
    }
    else
    {
      if (head->right == NULL)
        printf ("%d ", head->key);
      break;
    }
  }
}

void print_postorder (node *head)
{
  node *cur;

  cur = head;
  while (1)
  {
    while (cur->left != NULL)
      cur = cur->left;

    if (cur->right == NULL)
    {
      printf ("%d ", cur->key);

      if (cur == head)
	break;
      
      while (cur->parent != head && (cur->parent->right == NULL || 
	                             cur->parent->right == cur))
      {
        cur = cur->parent;
        printf ("%d ", cur->key);
      }
      
    }
    else
    {
      cur = cur->right;
      continue;
    }

    if (cur->parent != head || (head->right != cur && head->right != NULL))
      cur = cur->parent->right;
    else
    {
      printf ("%d ", head->key);
      break;
    }
  }
}

int main ()
{
  node *head;

  head = generate_bin_tree (10);
  /*print_inorder_rec (head);
  printf ("\n");
  print_inorder (head);
  printf ("\n");*/
  print_paths_rec (head);

  return 0;
}
