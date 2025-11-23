#include "../inc/scheduler.h"
#include <stddef.h>

static tcb_t task_list[MAX_TASK];
//tcb_t *scheduler_task_list = task_list;
static int task_count = 0;
static int last_served_for_prio[MAX_PRIORITIES];

void scheduler_init(void) {
	for (int i = 0; i < MAX_TASK; ++i) {
		task_list[i].fn = NULL;
		task_list[i].priority = 0;
		task_list[i].ready = 0;
	}

	for (int p = 0; p < MAX_PRIORITIES; ++p) last_served_for_prio[p] = -1;
}

int insert_task(task task_fn, int priority){
	if (task_fn == NULL) return -1;
	if (priority < 0 || priority >= MAX_PRIORITIES) return -1;
	if (task_count >= MAX_TASK) return -1;

	int id = task_count++;
	task_list[id].fn = task_fn;
	task_list[id].priority = priority;
	task_list[id].ready = 1;
	return id;
}

static int find_best_priority(void) {
	int best_prio = -1;
	int found = 0;

	for (int i = 0; i < task_count; ++i) {
		if (task_list[i].ready && task_list[i].priority > best_prio) {
			best_prio = task_list[i].priority;
			found = 1;
		}
	}

	return found ? best_prio : -1;
}

tcb_t* get_task(void){
	if(task_count == 0) return NULL;
	int best_prio = find_best_priority();
	if(best_prio < 0) return NULL;

	int last_task_idx = last_served_for_prio[best_prio];
	int next_task_idx = (last_task_idx + 1) % task_count;

	for(int i = 0; i < task_count; i++){
		int idx = (next_task_idx + i) % task_count;

		if(task_list[idx].ready && task_list[idx].priority == best_prio){
			last_served_for_prio[best_prio] = idx;
			return &task_list[idx];
		}
	}

	return NULL;
}

int update_priority(task task_fn, int priority){
	if (task_fn == NULL) return -1;
	if (priority < 0 || priority >= MAX_PRIORITIES) return -1;

	for (int i = 0; i < task_count; ++i) {
		if (task_list[i].fn == task_fn) {
			task_list[i].priority = priority;
			return i;
		}
	}

	return -1;
}

int scheduler_ready(task task_fn){
	if (task_fn == NULL) return -1;
	for (int i = 0; i < task_count; ++i) {
		if (task_list[i].fn == task_fn) {
			task_list[i].ready = 1;
			return i;
		}
	}

	return -1;
}

int scheduler_unready(task task_fn){
	if (task_fn == NULL) return -1;
	for (int i = 0; i < task_count; ++i) {
		if (task_list[i].fn == task_fn) {
			task_list[i].ready = 0;
			return i;
		}
	}

	return -1;
}
