#include <stdio.h>

#define	SIZE	100

int arr1[SIZE], arr2[SIZE], arr3[SIZE];
int top;

void insert (int key, int data)
{
  arr1[key] = ++top;
  arr2[top] = key;
  arr3[key] = data;
}

int search (int key)
{
  if (arr1[key] <= top && arr2[arr1[key]] == key)
    return arr3[key];
  else
    return -1;
}

int delete (int key)
{
  if (arr1[key] > top || arr2[arr1[key]] != key)
    return -1;

  if (arr1[key] == top)
    --top;
  else
  {
    arr1[arr2[top]] = arr1[key];
    arr2[arr1[key]] = arr2[top];
    --top;
  }

  return 0;
}

void print ()
{
  int i;

  for (i = 0; i <= top; ++i)
    printf ("key: %d data: %d\n", arr2[i], arr3[arr2[i]]);
}

int main ()
{
  char option[10];
  int flg;
  int key, data;

  top = -1;
  flg = 0;
  while (!flg)
  {
    printf ("'i' for insert, 's' for search, "
       	    "'d' for delete and 'q' for quit: ");
    scanf ("%s", option);

    switch (option[0])
    {
      case 'i':
	printf ("Key and data: ");
	scanf ("%d %d", &key, &data);

	insert (key, data);
	print ();

	break;
      case 's':
	printf ("Key: ");
	scanf ("%d", &key);

	data = search (key);
	if (data == -1)
	  printf ("Failed.\n");
	else
	  printf ("Successful. Key: %d data: %d\n", key, data);

	break;
      case 'd':
	printf ("Key: ");
	scanf ("%d", &key);

	data = delete (key);
	if (data == -1)
	  printf ("Failed.\n");
	else
	  printf ("Successful.\n");
	print ();

	break;
      case 'q':
	flg = 1;

	break;
      default:
	printf ("Unrecognized option, try again.\n");

	break;
    }
  }

  return 0;
}
