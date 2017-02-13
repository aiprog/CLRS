// The Fenwick tree is stored in another array with the same size as the
// original array.

#include <iostream>

using std::cout;
using std::endl;

struct Node
{
  int value;     // Value in the original array
  int sum;       // Sum of Fenwick tree rooted in this node 
  int parent;    // Parent of this node
};

class FenwickTree
{
public:
  FenwickTree (int *array, int size)
  {
    int i;

    // Since nodes of a Fenwick tree are numbered from 1, we
    // allocate one more node.
    nodeArray = (Node *)malloc (sizeof (Node) * (size + 1));
    this->size = size + 1;

    // Initialize T[i] to A[i] and back up the original array.
    for (i = 1; i < size + 1; ++i)
      nodeArray[i].sum = nodeArray[i].value = array[i - 1];

    // Compute the parent of each node if exists, otherwise set
    // it to -1.
    for (i = 1; i < size + 1; ++i)
    {
      nodeArray[i].parent = i + lowBit (i);
      if (nodeArray[i].parent > size)
        nodeArray[i].parent = -1;
      else
        nodeArray[nodeArray[i].parent].sum += nodeArray[i].sum;
    }
  }

  ~FenwickTree ()
  {
    free (nodeArray);
  }

  // Change the value of an element.
  void setValue (int index, int value)
  {
    int increment;

    ++index;
    increment = value - nodeArray[index].value;
    nodeArray[index].value = value;

    // Update T[i] and all its ancestors.
    do
    {
      nodeArray[index].sum += increment;
      index = nodeArray[index].parent;
    } while (index != -1);
  }

  // Return the sum of a subarray.
  int sumOfInterval (int beg, int end)
  {
    return sumFrom0 (end) - sumFrom0 (beg - 1);
  }

private:
  // Assume that 'value' has n trailing zeroes. It returns 2 ^ n.
  int lowBit (int value)
  {
    return value & (value ^ (value - 1));
  }

  // Return the sum of A[1] to A[i].
  int sumFrom0 (int index)
  {
    int sum = 0;

    if (index < 0)
      return 0;

    // Sum up the roots of Fenwick trees formed by A[1] to A[i].
    ++index;
    do
    {
      sum += nodeArray[index].sum;
      index -= lowBit (index);
    } while (index > 0);

    return sum;
  }

private:
  Node *nodeArray;
  int size;
};

int main ()
{
  int array[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  FenwickTree tree (array, 10);

  cout << tree.sumOfInterval (3, 9) << endl;
  tree.setValue (6, 5);
  cout << tree.sumOfInterval (3, 9) << endl;
  tree.setValue (6, 5);
  cout << tree.sumOfInterval (3, 9) << endl;
  cout << tree.sumOfInterval (0, 3) << endl;
  
  return 0;
}