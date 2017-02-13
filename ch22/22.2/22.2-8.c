typedef struct node
{
  struct node *child;
  struct node *next;
} Node;

int treeHeight (Node *tree, int *diameter)
{
  Node *ite;
  int h, h1, h2;

  if (!tree->child)             /* Node without child. */
    return 0;
  else if (!tree->child->next)  /* Node with one child. */
  {
    h = treeHeight (tree->child);
    /* The largest shortest-path distance of this node is h + 1. */
    if (h + 1 > *diameter)   
      *diameter = h + 1;             

    return h + 1;
  }
  else                          /* Node with more than one child. */
  {
    h1 = h2 = 0;
    /* Find the first(h1) and the second(h2) large heights of subtrees. */
    for (ite = tree->child; ite; ite = ite->next)
    {
      h = treeHeight (ite);
      if (h > h1)
      {
        h1 = h;
        h2 = h1;
      }
      else if (h > h2)
        h2 = h;
    }

    /* The largest shortest-path distance of this node is h1 + h2 + 2. */
    if (h1 + h2 + 2 > *diameter)
      *diameter = h1 + h2 + 2;

    return h1 + 1;
  }
}

int treeDiameter (Node *tree)
{
  int diameter;

  diameter = 0;
  treeHeight (tree, &diameter);

  return diameter;
}