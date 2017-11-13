/*
 * scheduler.h
 *
 * Author: Nikita Kiryanov
 */

#ifndef SCHEDULER_SCHEDULER_H_
#define SCHEDULER_SCHEDULER_H_

#include <stdbool.h>

struct scheduler_task {
	struct work *work;
	//get_recur_period: returns recur period in seconds
	double (*get_recur_period)(void);
};

extern bool reset_screen_saver_req;

void tc_scheduler_init(void);
void tasks_init(void);

#endif
