/*
 *  b_tree.c
 *
 *  This file implements b_tree.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "b_tree.h"

extern int ftruncate (int fd, off_t length);
extern int fileno (FILE *stream);

/* Allocate disk space for nodes. */
static long b_tree_disk_allocate (B_tree *tree)
{
  long offset;

  if (tree->list != NULL)
  {
    offset = tree->list->offset;
    tree->list = tree->list->next;
  }
  else
  {
    if (tree->offset == tree->file_size)
    {
      tree->file_size *= 2;
      ftruncate (fileno (tree->fp), tree->file_size);
    }

    offset = tree->offset;
    tree->offset += sizeof (B_tree_node);
  }

  return offset;
}

/* Write nodes to disks. */
static void b_tree_disk_write (B_tree *tree, B_tree_node *node)
{
  fseek (tree->fp, node->offset, SEEK_SET);
  fwrite (node, 1, sizeof (B_tree_node), tree->fp);
}

/* Read nodes from disks. */
static void b_tree_disk_read (B_tree *tree, B_tree_node *node)
{
  fseek (tree->fp, node->offset, SEEK_SET);
  fread (node, 1, sizeof (B_tree_node), tree->fp);
}

/* Initialize B-trees from files. */
B_tree *b_tree_init (char *path)
{
  FILE *fp;
  B_tree *tree;
  long file_size;

  if ((fp = fopen (path, "r+")) == NULL)
  {
    if ((fp = fopen (path, "w")) == NULL)
      return NULL;
    else
    {
      fclose (fp);
      if ((fp = fopen (path, "r+")) == NULL)
        return NULL;
    }
  }

  fseek (fp, 0, SEEK_END);
  file_size = ftell (fp);
  fseek (fp, 0, SEEK_SET);

  tree = malloc (sizeof (B_tree));
  tree->fp = fp;
  tree->list = NULL;

  if (file_size != 0)
  {
    tree->file_size = file_size;
    tree->offset = file_size;

    tree->root.offset = 0;
    b_tree_disk_read (tree, &tree->root);
  }
  else
  {
    tree->file_size = INIT_NUM * sizeof (B_tree_node);
    ftruncate (fileno (fp), tree->file_size);
    tree->offset = 0;

    tree->root.leaf = 1;
    tree->root.garbage = 0;
    tree->root.offset = b_tree_disk_allocate (tree);
    tree->root.parent = -1;
    tree->root.num = 0;
    b_tree_disk_write (tree, &tree->root);    
  }

  return tree;
}

/* Split nodes. */
static B_tree_node *b_tree_split_child (B_tree *tree, B_tree_node *node, 
                                        int index, B_tree_node *child)
{
  B_tree_node *temp;
  int i;

  child->num = DEGREE - 1;
  b_tree_disk_write (tree, child);

  temp = malloc (sizeof (B_tree_node));
  temp->leaf = child->leaf;
  temp->garbage = 0;
  temp->offset = b_tree_disk_allocate (tree);
  temp->parent = child->parent;
  temp->num = DEGREE - 1;
  for (i = 0; i < DEGREE - 1; ++i)
  {
    temp->keys[i] = child->keys[DEGREE + i];
  }
  if (temp->leaf == 0)
  {
    for (i = 0; i < DEGREE; ++i)
    {
      temp->offsets[i] = child->offsets[DEGREE + i];
    }
  }
  b_tree_disk_write (tree, temp);

  for (i = node->num - 1; i >= index; --i)
  {
    node->keys[i + 1] = node->keys[i];
    node->offsets[i + 2] = node->offsets[i + 1];
  }
  node->keys[index] = child->keys[DEGREE - 1];
  node->offsets[index + 1] = temp->offset;
  ++node->num;
  b_tree_disk_write (tree, node);

  return temp;
}

/* Insert keys into nonfull nodes. */
static void b_tree_insert_nonfull (B_tree *tree, B_tree_node *node, long key)
{
  B_tree_node *sibling, *next, child;
  int i;

  if (node->leaf == 1)
  {
    for (i = node->num - 1; i >= 0 && node->keys[i] > key; --i)
    {
      node->keys[i + 1] = node->keys[i];
    }
    node->keys[i + 1] = key;
    ++node->num;
    b_tree_disk_write (tree, node);
  }
  else
  {
    for (i = node->num - 1; i >= 0 && node->keys[i] > key; --i)
      ;
    ++i;

    child.offset = node->offsets[i];
    b_tree_disk_read (tree, &child);

    next = &child;
    if (child.num == 2 * DEGREE - 1)
    {
      sibling = b_tree_split_child (tree, node, i, &child);
      if (key > node->keys[i])
        next = sibling;
      else
        free (sibling);
    }

    b_tree_insert_nonfull (tree, next, key);
  }
}

/* Insert keys into nodes. */
void b_tree_insert (B_tree *tree, long key)
{
  B_tree_node child;

  if (tree->root.num == 2 * DEGREE - 1)
  {
    memcpy (&child, &tree->root, sizeof (B_tree_node));
    child.leaf = 1;
    child.garbage = 0;
    child.offset = b_tree_disk_allocate (tree);
    child.parent = tree->root.offset;

    tree->root.leaf = 0;
    tree->root.num = 0;
    tree->root.offsets[0] = child.offset;

    free (b_tree_split_child (tree, &tree->root, 0, &child));
  }

  b_tree_insert_nonfull (tree, &tree->root, key);
}

static void b_tree_search_sub (B_tree *tree, B_tree_node *root, long key,
                               B_tree_node **node, int *index)
{
  static B_tree_node child;
  int i;

  for (i = 0; i < root->num && key > root->keys[i]; ++i)
    ;

  if (i < root->num && key == root->keys[i])
  {
    *node = root;
    *index = i;
  }
  else if (root->leaf == 0)
  {
    child.offset = root->offsets[i];
    b_tree_disk_read (tree, &child);
    b_tree_search_sub (tree, &child, key, node, index);
  }
  else
    *node = NULL;
}

/* Search. */
void b_tree_search (B_tree *tree, long key, B_tree_node **node, int *index)
{
  b_tree_search_sub (tree, &tree->root, key, node, index);
}

/* Print nodes. */
static void print_node (B_tree_node *node)
{
  int i;
  printf ("leaf %d garbage %d num %d\n", node->leaf, node->garbage, node->num);
  for (i = 0; i < node->num; ++i)
    printf ("%ld ", node->keys[i]);
  printf ("\n");
}

/* Merge two nodes. */
static void b_tree_merge (B_tree *tree, B_tree_node *node, int index,
                          B_tree_node *l_child, B_tree_node *r_child)
{
  Free_node *f_node;
  int i;

  l_child->keys[l_child->num] = node->keys[index];
  l_child->offsets[l_child->num + 1] = r_child->offsets[0];
  for (i = 0; i < r_child->num; ++i)
    l_child->keys[l_child->num + 1 + i] = r_child->keys[i];
  if (l_child->leaf == 0)
  {
    for (i = 0; i < r_child->num; ++i)
      l_child->offsets[l_child->num + 2 + i] = r_child->offsets[i];
  }
  l_child->num += 1 + r_child->num;

  f_node = malloc (sizeof (Free_node));
  f_node->offset = r_child->offset;
  f_node->next = tree->list;
  tree->list = f_node;

  r_child->garbage = 1;
  b_tree_disk_write (tree, r_child);

  for (i = index + 1; i < node->num; ++i)
  {
    node->keys[i - 1] = node->keys[i];
    node->offsets[i] = node->offsets[i + 1];
  }
  --node->num;
  if (node->num == 0)
  {
    memcpy (&tree->root, l_child, sizeof (B_tree_node));
    tree->root.offset = 0;
    tree->root.parent = -1;

    f_node = malloc (sizeof (Free_node));
    f_node->offset = l_child->offset;
    f_node->next = tree->list;
    tree->list = f_node;

    l_child->garbage = 1;
    b_tree_disk_write (tree, l_child);
  }
}

static void b_tree_delete_sub (B_tree *tree, B_tree_node *node, long key)
{
  B_tree_node l_child, r_child, child, temp;
  int l_flag, r_flag, i, j;

  for (i = 0; i < node->num && key > node->keys[i]; ++i)
    ;

  if (i < node->num && key == node->keys[i])
  {
    if (node->leaf == 1)
    {
      for (j = i + 1; j < node->num; ++j)
        node->keys[j - 1] = node->keys[j];
      --node->num;
      b_tree_disk_write (tree, node);
    }
    else
    {
      l_child.offset = node->offsets[i];
      b_tree_disk_read (tree, &l_child);

      if (l_child.num > DEGREE - 1)
      {
        memcpy (&temp, &l_child, sizeof (B_tree_node));
        while (temp.leaf == 0)
        {
          temp.offset = temp.offsets[temp.num];
          b_tree_disk_read (tree, &temp);
        }

        node->keys[i] = temp.keys[temp.num - 1];
        b_tree_disk_write (tree, node);

        temp.keys[temp.num - 1] = key;
        b_tree_disk_write (tree, &temp);

        b_tree_delete_sub (tree, &temp, key);
      }
      else
      {
        r_child.offset = node->offsets[i + 1];
        b_tree_disk_read (tree, &r_child);

        if (r_child.num > DEGREE - 1)
        {
          memcpy (&temp, &r_child, sizeof (B_tree_node));
          while (temp.leaf == 0)
          {
            temp.offset = temp.offsets[0];
            b_tree_disk_read (tree, &temp);
          }

          node->keys[i] = temp.keys[0];
          b_tree_disk_write (tree, node);

          temp.keys[0] = key;
          b_tree_disk_write (tree, &temp);

          b_tree_delete_sub (tree, &temp, key);
        }
        else
        {
          if (node->num == 1)
          {
            b_tree_merge (tree, node, i, &l_child, &r_child);
            b_tree_delete_sub (tree, &tree->root, key);
          }
          else
          {
            b_tree_merge (tree, node, i, &l_child, &r_child);
            b_tree_disk_write (tree, node);
            b_tree_delete_sub (tree, &l_child, key);
          }
        }
      }
    }
  }
  else
  {
    child.offset = node->offsets[i];
    b_tree_disk_read (tree, &child);

    if (child.num > DEGREE - 1)
      b_tree_delete_sub (tree, &child, key);
    else
    {
      l_flag = 0;
      if (i > 0)
      {
        l_child.offset = node->offsets[i - 1];
        b_tree_disk_read (tree, &l_child);

        if (l_child.num > DEGREE - 1)
        {
          l_flag = 1;

          for (j = DEGREE - 2; j >= 0; --j)
            child.keys[j + 1] = child.keys[j];
          child.keys[0] = node->keys[i - 1];
          if (child.leaf == 0)
          {
            for (j = DEGREE - 1; j >= 0; --j)
              child.offsets[j + 1] = child.offsets[j];
            child.offsets[0] = l_child.offsets[l_child.num];
          }
          ++child.num;
          b_tree_disk_write (tree, &child);

          node->keys[i - 1] = l_child.keys[l_child.num - 1];
          b_tree_disk_write (tree, node);

          --l_child.num;
          b_tree_disk_write (tree, &l_child);

          b_tree_delete_sub (tree, &child, key);
        }
      }

      r_flag = 0;
      if (i == 0 || (i != node->num && l_flag == 0))
      {
        r_child.offset = node->offsets[i + 1];
        b_tree_disk_read (tree, &r_child);

        if (r_child.num > DEGREE - 1)
        {
          r_flag = 1;

          child.keys[DEGREE - 1] = node->keys[i];
          child.offsets[DEGREE] = r_child.offsets[0];
          ++child.num;
          b_tree_disk_write (tree, &child);

          node->keys[i] = r_child.keys[0];
          b_tree_disk_write (tree, node);

          for (j = 1; j < r_child.num; ++j)
            r_child.keys[j - 1] = r_child.keys[j];
          if (r_child.leaf == 0)
          {
            for (j = 1; j <= r_child.num; ++j)
              r_child.offsets[j - 1] = r_child.offsets[j];
          }
          --r_child.num;
          b_tree_disk_write (tree, &r_child);

          b_tree_delete_sub (tree, &child, key);
        }
      }

      if (l_flag == 0 && r_flag == 0)
      {
        if (i == node->num)
        {
          memcpy (&r_child, &child, sizeof (B_tree_node));
          memcpy (&child, &l_child, sizeof (B_tree_node));
          --i;
        }

        if (node->num == 1)
        {
          b_tree_merge (tree, node, i, &child, &r_child);
          b_tree_delete_sub (tree, &tree->root, key);
        }
        else
        {
          b_tree_merge (tree, node, i, &child, &r_child);
          b_tree_disk_write (tree, node);
          b_tree_disk_write (tree, &child);
          b_tree_delete_sub (tree, &child, key);
        }
      }
    }
  }
}

/* Delete. */
void b_tree_delete (B_tree *tree, long key)
{
  B_tree_node *node;
  int index;

  b_tree_search (tree, key, &node, &index);
  if (node != NULL)
    b_tree_delete_sub (tree, &tree->root, key);
}

static void print_b_tree_sub (B_tree *tree, B_tree_node *node)
{
  B_tree_node child;
  int i;

  for (i = 0; i <= node->num; ++i)
  {
    if (node->leaf == 0)
    {
      child.offset = node->offsets[i];
      b_tree_disk_read (tree, &child);

      print_b_tree_sub (tree, &child);
    }
    if (i != node->num)
      printf ("%ld ", node->keys[i]);
  }
}

/* Print. */
void print_b_tree (B_tree *tree)
{
  print_b_tree_sub (tree, &tree->root);
  printf ("\n");
}

/* Clean up and compact. */
void b_tree_finish (B_tree *tree)
{
  Free_node *temp;
  B_tree_node parent, node;
  int i;

  while (tree->offset > 0)
  {
    node.offset = tree->offset - sizeof (B_tree_node);
    b_tree_disk_read (tree, &node);

    if (node.garbage == 1)
      tree->offset -= sizeof (B_tree_node);
    else
    {
      while (tree->list != NULL && tree->list->offset >= tree->offset)
      {
        temp = tree->list;
        tree->list = tree->list->next;
        free (temp);
      }

      if (tree->list != NULL)
      {
        parent.offset = node.parent;
        b_tree_disk_read (tree, &parent);

        for (i = 0; parent.offsets[i] != node.offset; ++i)
          ;
        parent.offsets[i] = tree->list->offset;
        b_tree_disk_write (tree, &parent);

        node.offset = tree->list->offset;
        b_tree_disk_write (tree, &node);

        temp = tree->list;
        tree->list = tree->list->next;
        free (temp);

        tree->offset -= sizeof (B_tree_node);
      }
      else
        break;
    }
  }

  while (tree->list != NULL)
  {
    temp = tree->list;
    tree->list = tree->list->next;
    free (temp);
  }

  if (tree->root.num == 0)
    tree->offset = 0;

  ftruncate (fileno (tree->fp), tree->offset);
  fclose (tree->fp);
  free (tree);
}

/* Test. */
static void b_tree_test ()
{
  B_tree *tree;
  B_tree_node *node;
  char option[10];
  long key;
  int index;

  if ((tree = b_tree_init ("B-tree")) == NULL)
  {
    printf ("Initialization failed\n");
    return;
  }

  option[0] = 0;
  while (option[0] != 'q')
  {
    printf ("option: ");
    scanf ("%s", option);
    if (option[0] == 'i')
    {
      printf ("key: ");
      scanf ("%ld", &key);
      b_tree_insert (tree, key);
    }
    else if (option[0] == 's')
    {
      printf ("key: ");
      scanf ("%ld", &key);
      b_tree_search (tree, key, &node, &index);
      if (node != NULL)
        printf ("found\n");
      else
        printf ("not found\n");
    }
    else if (option[0] == 'd')
    {
      printf ("key: ");
      scanf ("%ld", &key);
      b_tree_delete (tree, key);
    }
    else if (option[0] == 'p')
      print_b_tree (tree);
    else if (option[0] == 'q')
      ;
    else
      printf ("unrecognized option\n");
  }

  b_tree_finish (tree);
}