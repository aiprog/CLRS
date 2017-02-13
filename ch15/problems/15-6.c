#include <stdio.h>
#include <stdlib.h>

typedef struct _node
{
  char *name;
  int rating;
  struct _node *left, *sibling;
  int memo, solu;
} Node;

void pcp (Node *root)
{
  int son, grandson;
  Node *ite1, *ite2;

  if (root->memo >= 0)
    return;
  else if (root->left == NULL)
  {
    root->memo = root->rating;
    root->solu = 1;
    return;
  }

  son = grandson = 0;
  for (ite1 = root->left; ite1 != NULL; ite1 = ite1->sibling)
  {
    pcp (ite1);
    son += ite1->memo;
    for (ite2 = ite1->left; ite2 != NULL; ite2 = ite2->sibling)
      grandson += ite2->memo;
  }

  if (root->rating + grandson > son)
  {
    root->memo = root->rating + grandson;
    root->solu = 1;
  }
  else
  {
    root->memo = son;
    root->solu = 0;
  }
}

void print_solu (Node *root)
{
  Node *ite1, *ite2;
  int flag;

  flag = root->solu == 1? 1: 0;
  if (flag)
    printf ("%s ", root->name);
  for (ite1 = root->left; ite1 != NULL; ite1 = ite1->sibling)
  {
    if (!flag)
      print_solu (ite1);
    else
    {
      for (ite2 = ite1->left; ite2 != NULL; ite2 = ite2->sibling)
	print_solu (ite2);
    }
  }
}

void build_tree (Node **root, int size, char **name, int *rating, int *pos)
{
  if (*pos == size)
    return;

  if (name[*pos] == NULL)
  {
    *root = NULL;
    ++*pos;
  }
  else
  {
    *root = malloc (sizeof (Node));
    (*root)->name = name[*pos];
    (*root)->rating = rating[*pos];
    ++*pos;
    build_tree (&(*root)->left, size, name, rating, pos);
    build_tree (&(*root)->sibling, size, name, rating, pos);
    (*root)->memo = (*root)->solu = -1;
  }
}

int main ()
{
  Node *root;
  int pos;
  char *name[29] = { "name1", "name2", "name5", NULL, "name6", "name12", NULL, 
                     NULL, NULL, "name3", "name7", NULL, "name8", NULL,
		     "name9", "name13", NULL, "name14", NULL, NULL, "name10", 
		     NULL, NULL, "name4", "name11", NULL, NULL, NULL, NULL 
                   };
  int rating[29] = { 3, 4, 3, 0, 5, 6, 
                     0, 0, 0, 5, 4, 0, 
		     3, 0, 3, 5, 0, 6, 
		     0, 0, 5, 0, 0, 6, 
		     5, 0, 0, 0, 0 
                   };
  pos = 0;
  build_tree (&root, 29, name, rating, &pos);
  pcp (root);
  printf ("Max: %d\n", root->memo);
  print_solu (root);
  printf ("\n");

  return 0;
}
