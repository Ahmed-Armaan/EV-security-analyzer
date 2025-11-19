#ifndef _SCHEDULER_H
#define _SCHEDULER_H
#include <stdint.h>
#define MAX_TASK 4
#define MAX_PRIORITIES 3

typedef void (*task)(void);

typedef struct tcb{
	task fn;
	int priority;
	int ready;
}tcb_t;

tcb_t *get_task(void);
int insert_task(task task_fn, int priority);
int update_priority(task task_fn, int priority);
int scheduler_ready(task task_fn);
int scheduler_unready(task task_fn);
void scheduler_init(void);

#endif
