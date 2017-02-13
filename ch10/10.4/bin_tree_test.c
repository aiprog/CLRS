#include <stdio.h>
#include <stdlib.h>

#include "bin_tree.h"

int main ()
{
  root = create_tree ();
  print_tree_pre_nonstk (root);
  free_tree (root);

  return 0;
}
