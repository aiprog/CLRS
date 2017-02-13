/*
 *  veb_tree.c
 *
 *  This file implements veb_tree.h
 */

#include <stdio.h>
#include <stdlib.h>

#include "veb_tree.h"

/* Return a new tree. */
veb_tree_node *new_veb_tree (int expo)
{
  veb_tree_node *node;
  int clu_expo, sum_expo;
  int i;

  node = malloc (sizeof (veb_tree_node));
  node->expo = expo;
  node->max = node->min = -1;
  if (expo == 1)
  {
    node->cluster = NULL;
    node->summary = NULL;
  }
  else
  {
    clu_expo = sum_expo = expo / 2;
    if (expo % 2 == 1)
      ++sum_expo;

    node->cluster = malloc ((1 << sum_expo) * sizeof (veb_tree_node *));
    for (i = 0; i < 1 << sum_expo; ++i)
    {
      node->cluster[i] = new_veb_tree (clu_expo);
    }
    node->summary = new_veb_tree (sum_expo);
  }

  return node;
}

/* Minimum. */
int veb_tree_minimum (veb_tree_node *node)
{
  return node->min;
}

/* Maximum. */
int veb_tree_maximum (veb_tree_node *node)
{
  return node->max;
}

/* Get the cluster number. */
static int veb_tree_high (veb_tree_node *node, int key)
{
  int clu_expo;

  clu_expo = node->expo / 2;

  return key >> clu_expo;
}

/* Get the offset in the cluster. */
static int veb_tree_low (veb_tree_node *node, int key)
{
  int clu_expo, bits;

  clu_expo = node->expo / 2;
  bits = 8 * sizeof (int) - clu_expo;

  return key & (~(unsigned) 0 << bits >> bits);
}

/* Get the key from its cluster number and offset. */
static int veb_tree_index (veb_tree_node *node, int high, int low)
{
  int clu_expo;

  clu_expo = node->expo / 2;

  return (high << clu_expo) + low;
}

/* Test whether a key is in the tree. */
int veb_tree_member (veb_tree_node *node, int key)
{
  veb_tree_node *child;

  if (node->min == -1)
    return 0;
  else if (key == node->min || key == node->max)
    return 1;
  else if (node->expo == 1)
    return 0;
  else
  {
    child = node->cluster[veb_tree_high (node, key)];
    return veb_tree_member (child, veb_tree_low (node, key));
  }
}

/* Successor. */
int veb_tree_successor (veb_tree_node *node, int key)
{
  int high, low, index, successor;
  veb_tree_node *child;

  if (node->min == -1)
    return -1;
  else if (node->expo == 1)
  {
    if (key == 0 && node->max == 1)
      return 1;
    else
      return -1;
  }
  else if (key < node->min)
    return node->min;
  else
  {
    high = node->summary->max;
    if (high == -1)
    {
      if (key < node->max)
        return node->max;
      else
        return -1;
    }
    low = node->cluster[high]->max;
    index = veb_tree_index (node, high, low);

    if (key >= index && key < node->max)
      return node->max;
    else
    {
      high = veb_tree_high (node, key);
      low = veb_tree_low (node, key);

      if (low < node->cluster[high]->max)
      {
        child = node->cluster[high];
        successor = veb_tree_successor (child, low);

        return veb_tree_index (node, high, successor);
      }
      else if ((successor = veb_tree_successor (node->summary, high)) != -1)
      {
        child = node->cluster[successor];
        return veb_tree_index (node, successor, veb_tree_minimum (child));
      }
      else
        return -1;
    }
  }
}

/* Predecessor. */
int veb_tree_predecessor (veb_tree_node *node, int key)
{
  int high, low, index, predecessor;
  veb_tree_node *child;

  if (node->min == -1)
    return -1;
  else if (node->expo == 1)
  {
    if (key == 1 && node->min == 0)
      return 1;
    else
      return -1;
  }
  else if (key > node->max)
    return node->max;
  else
  {
    high = node->summary->min;
    if (high == -1)
    {
      if (key > node->min)
        return node->min;
      else
        return -1;
    }
    low = node->cluster[high]->min;
    index = veb_tree_index (node, high, low);

    if (key <= index && key > node->min)
      return node->min;
    else
    {
      high = veb_tree_high (node, key);
      low = veb_tree_low (node, key);

      if (low > node->cluster[high]->min)
      {
        child = node->cluster[high];
        predecessor = veb_tree_predecessor (child, low);

        return veb_tree_index (node, high, predecessor);
      }
      else if ((predecessor = veb_tree_predecessor (node->summary, high)) 
               != -1)
      {
        child = node->cluster[predecessor];
        return veb_tree_index (node, predecessor, veb_tree_maximum (child));
      }
      else
        return -1;
    }
  }
}

/* Insert. */
void veb_tree_insert (veb_tree_node *node, int key)
{
  int high, low, temp;

  if (node->min == -1)
  {
    node->min = node->max = key;
    return;
  }
  else if (key < node->min)
  {
    temp = key;
    key = node->min;
    node->min = temp;
  }
  else if (key > node->max)
  {
    temp = key;
    key = node->max;
    node->max = temp;
  }

  if (key != node->min && key != node->max)
  {
    high = veb_tree_high (node, key);
    low = veb_tree_low (node, key);

    if (node->cluster[high]->min == -1)
    {
      node->cluster[high]->min = node->cluster[high]->max = low;
      veb_tree_insert (node->summary, high);
    }
    else
      veb_tree_insert (node->cluster[high], low);
  }
}

/* Delete. */
void veb_tree_delete (veb_tree_node *node, int key)
{
  int high, low;

  if (node->min == node->max)
  {
    node->min = node->max = -1;
    return;
  }
  else if (node->min == key)
  {
    if (node->expo == 1 || (high = veb_tree_minimum (node->summary)) == -1)
    {
      node->min = node->max;
      return;
    }

    if (node->expo != 1)
    {
      node->min = veb_tree_index (node, high, node->cluster[high]->min); 
      key = node->min;
    }
  }
  else if (node->max == key)
  {
    if (node->expo == 1 || (high = veb_tree_maximum (node->summary)) == -1)
    {
      node->max = node->min;
      return;
    }

    if (node->expo != 1)
    {
      node->max = veb_tree_index (node, high, node->cluster[high]->max);
      key = node->max;
    }
  }

  high = veb_tree_high (node, key);
  low = veb_tree_low (node, key);

  if (node->cluster[high]->min == node->cluster[high]->max)
  {
    node->cluster[high]->min = node->cluster[high]->max = -1;
    veb_tree_delete (node->summary, high);
  }
  else
    veb_tree_delete (node->cluster[high], low);
}

/* Free a tree. */
void free_veb_tree (veb_tree_node *node)
{
  int i;

  if (node->expo > 1)
  {
    for (i = 0; i < 1 << node->summary->expo; ++i)
    {
      free_veb_tree (node->cluster[i]);
    }
    free (node->cluster);
    free_veb_tree (node->summary);
  }
  free (node);
}

/* Print. */
static void veb_tree_print (veb_tree_node *node, int base)
{
  int num, i;

  if (node->min == -1)
    return;
  else if (node->min == node->max)
  {
    printf ("%d ", base + node->min);
    return;
  }
  else
  {
    printf ("%d ", base + node->min);
    if (node->expo > 1)
    {
      num = 1 << (node->expo - node->summary->expo);
      for (i = 0; i < 1 << node->summary->expo; ++i)
      {
        veb_tree_print (node->cluster[i], base + i * num);
      }
    }
    printf ("%d ", base + node->max);
  }
}

/* Test. */
static void veb_tree_test ()
{
  veb_tree_node *tree;
  char option[10];
  int key;

  tree = new_veb_tree (4);
  option[0] = 0;
  while (option[0] != 'q')
  {
    printf ("option: ");
    scanf ("%s", option);

    if (option[0] == 'i')
    {
      printf ("key: ");
      scanf ("%d", &key);
      veb_tree_insert (tree, key);
    }
    else if (option[0] == 'm')
      printf ("%d %d\n", veb_tree_minimum (tree), veb_tree_maximum (tree));
    else if (option[0] == 't')
    {
      printf ("key: ");
      scanf ("%d", &key);
      printf ("%d\n", veb_tree_member (tree, key));
    }
    else if (option[0] == 'd')
    {
      printf ("key: ");
      scanf ("%d", &key);
      veb_tree_delete (tree, key);
    }
    else if (option[0] == 's')
    {
      printf ("key: ");
      scanf ("%d", &key);

      printf ("%d %d\n", veb_tree_predecessor (tree, key), 
              veb_tree_successor (tree, key));
    }
    else if (option[0] == 'p')
    {
      veb_tree_print (tree, 0);
      printf ("\n");
    }
    else if (option[0] != 'q')
      printf ("unrecognized option\n");
  }

  free_veb_tree (tree);
}