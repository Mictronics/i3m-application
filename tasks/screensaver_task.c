/*
 * screensaver_task.c
 *
 * Created: 19.02.2023 21:05:50
 *  Author: Tesla
 */ 
#include "scheduler/scheduler.h"
#include "work-queue/work.h"
#include "asf.h"

void screen_saver_task_reset(void);
extern uint8_t screen_saver_get_time_seconds(void);
extern void screen_saver_off(void);
volatile int screen_saver_timeout = 0;

void screen_saver_task_reset(void) {
	screen_saver_timeout = 0;
}

/**
 * Called with 1s tick increasing the screen saver timeout counter.
 *
 */
static void save_screen(void *data)
{
	//ioport_toggle_pin_level(FP_DBG_CLK_OUT_PIN);
	uint8_t t = screen_saver_get_time_seconds();
	if(t == 0) {
		screen_saver_timeout = 0;
		return;
	}
	if(screen_saver_timeout < t) {
		++screen_saver_timeout;
	} else {
		screen_saver_off();
	}
	
}

static struct work screen_saver_work = { .do_work = save_screen };

static double screen_saver_get_recur_period(void)
{
	return 1; // 1s recurrence
}

struct scheduler_task screen_saver_tick_task = {
	.work = &screen_saver_work,
	.get_recur_period = screen_saver_get_recur_period,
};
