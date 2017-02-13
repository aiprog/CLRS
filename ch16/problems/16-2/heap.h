#ifndef HEAP_H
#define HEAP_H

void new_heap (int Size, int (*Compare)(void *, void *));

void free_heap ();

int is_empty ();

void insert (void *data);

void *extract ();

#endif