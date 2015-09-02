/*
 * Fp-utils.h
 *
 *  Created on: Aug 26, 2015
 *      Author: arkadi
 */

#ifndef FP_UTILS_H_
#define FP_UTILS_H_


#include "asf.h"
#include "adc/adc.h"
#include "layout.h"

typedef void (*func_ptr)(char **);

enum information_type{
	SHOW_POWER_STATE,
	SHOW_VOLTAGE,
	SHOW_CPU_TEMPERTURE,
	SHOW_GPU_TEMPERTURE,
	SET_BOIS_STATE
};

enum action_type {
	ACTION_TYPE_NONE,
	ACTION_TYPE_SHOW_MENU,
	ACTION_TYPE_SHOW_FRAME,
	ACTION_TYPE_SET_BIOS_STATE
};

enum power_state{
	POWER_ON,
	POWER_STR,
	POWER_STD,
	POWER_OFF
};

extern enum power_state current_power_state;

void update_power_state();

void set_state(char **data);

void update_data_by_type(enum information_type type, char **data);


#endif /* FP_UTILS_H_ */
