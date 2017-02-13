#include <stdio.h>
#include <stdlib.h>

#include "heap.h"

#define		SIZE		4

int start[SIZE] = { 0, 0, 1, 9 };
int duration[SIZE] = { 1, 3, 2, 8 };

int compare (void *arg1, void *arg2)
{
	if (duration[*(int *) arg1] < duration[*(int *) arg2])
		return 1;
	else if (duration[*(int *) arg1] > duration[*(int *) arg2])
		return -1;
	else
		return 0;
}

void task_scheduling ()
{
	int tasks[SIZE];
	int cur_task, next_task, elapse, *temp;
	int i;

	for (i = 0; i < SIZE; ++i)
		tasks[i] = i;

	new_heap (SIZE, compare);

	cur_task = 0;
	next_task = 1;
	elapse = 0;
	while (1)
	{
		if (cur_task != -1 && duration[cur_task] == 0)
		{
			temp = (int *) extract ();
			if (temp == NULL)
				cur_task = -1;
			else
				cur_task = *temp;
		}

		while (next_task < SIZE && start[next_task] == elapse)
		{
			if (cur_task == -1)
			{
				cur_task = next_task;
			}
			else if (duration[next_task] < duration[cur_task])
			{
				insert (tasks + cur_task);
				cur_task = next_task;
			}
			else
			{
				insert (tasks + next_task);
			}

			++next_task;
		}

		if (cur_task == -1)
		{
			if (next_task == SIZE)
				break;
			printf ("idle ");
		}
		else
		{
			--duration[cur_task];
			printf ("%d ", cur_task);
		}

		++elapse;
	}
	printf ("\n");

	free_heap ();
}

int main ()
{
	task_scheduling ();

	return 0;
}