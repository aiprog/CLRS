#include "dy_huf_tree.h"

Node *new_node (int ch, int fre, int dep, Node *left, Node *right, Node *parent)
{
  Node *node;

  node = malloc (sizeof (Node));
  node->ch = ch;
  node->fre = fre;
  node->dep = dep;
  node->left = left;
  node->right = right;
  node->parent = parent;

  return node;
}

Node *get_sibling (Node *node)
{
  if (node == node->parent->left)
    return node->parent->right;
  else
    return node->parent->left;
}

void update_depth (Node *node, int inc)
{
  if (node == NULL)
    return;

  node->dep += inc;
  update_depth (node->left, inc);
  update_depth (node->right, inc);
}

void exchange_node (Node *node1, Node *node2)
{
  Node *temp;
  int inc;

  inc = node2->dep - node1->dep;
  if (inc != 0)
  {
    update_depth (node1, inc);
    update_depth (node2, -inc);
  }
  
  temp = node1->parent;
  node1->parent = node2->parent;
  if (node2 == node2->parent->left)
    node1->parent->left = node1;
  else
    node1->parent->right = node1;

  node2->parent = temp;
  if (node1 == temp->left)
    node2->parent->left = node2;
  else
    node2->parent->right = node2;
}

Node *search_tree (Node *node, Node *parent, int fre)
{
  Node *result;

  if (node == NULL || node == parent)
    return NULL;

  if (node->dep < parent->dep)
  {
    result = search_tree (node->left, parent, fre);
    if (result != NULL)
      return result;
    else
      return search_tree (node->right, parent, fre);
  }
  else if (node->fre == fre)
    return node;
  else if (node->fre > parent->fre && node->ch == -1)
  {
    if (node->left->fre == fre)
      return node->left;
    else if (node->right->fre == fre)
      return node->right;
  }

  return NULL;
}

void initialize_tree ()
{
  int dep;

  root = new_node (0, 0, CHA_SET - 1, NULL, NULL, NULL);
  table[0] = root;
  for (dep = CHA_SET - 1; dep >= 1; --dep)
  {
    table[CHA_SET - dep] = new_node (CHA_SET - dep, 0, dep, NULL, NULL, NULL);
    root = new_node (-1, 0, dep - 1, root, table[CHA_SET - dep], NULL);
    root->left->parent = root;
    root->right->parent = root;
  }
}

void adjust_tree (Node *node)
{
  Node *sibling, *result;

  if (node == root)
  {
    ++node->fre;
    return;
  }

  sibling = get_sibling (node);
  if (sibling->fre > node->fre)
  {
    ++node->fre;
    adjust_tree (node->parent);
    return;
  }

  result = search_tree (root, node->parent, node->fre);
  if (result == NULL)
  {
    ++node->fre;
    adjust_tree (node->parent);
  }
  else
  {
    exchange_node (node, result);
    adjust_tree (node);
  }
}

void get_cost (Node *node, int *cost)
{
  if (node->ch != -1)
    return;

  *cost += node->fre;
  get_cost (node->left, cost);
  get_cost (node->right, cost);
}

void print_tree (Node *node)
{
  if (node == NULL)
  {
    printf ("NULL\n");
    return;
  }

  printf ("%d\n", node->fre);
  if (node->fre != 0)
  {
    print_tree (node->left);
    print_tree (node->right);
  }
  else
    printf ("NULL\nNULL\n");
}
