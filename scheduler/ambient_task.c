#include "scheduler.h"
#include "power/power.h"
#include "twi/twi_master.h"
#include "twi/i2c_buffer.h"
#include "work-queue/work.h"
#include "layout.h"

#define MAX_AMBIENT_UPDATE_FAIL	2

#define AMBIENT_TWI_ADDRESS  		0x4C
#define AMBIENT_TEMPERATURE_ADDRESS 0x00

static void update_ambient_temp(void *data)
{
	static uint8_t ambient_update_fail_count = 0;

    if (current_power_state != POWER_ON)
        return;

    if (TWI_read_reg(AMBIENT_TWI_ADDRESS, AMBIENT_TEMPERATURE_ADDRESS, &i2c_buffer.layout.ambt, 2)) {
		ambient_update_fail_count = 0;
		i2c_buffer.layout.ambs = 1;
	} else if (ambient_update_fail_count == MAX_AMBIENT_UPDATE_FAIL) {
		i2c_buffer.layout.ambs = 0;
	} else  {
		ambient_update_fail_count++;
		update_ambient_temp(NULL);
	}

	computer_data.details.ambient_temp_set = i2c_buffer.layout.ambs;
	computer_data.details.ambient_temp = i2c_buffer.layout.ambt;
}

static struct work ambient_work = { .do_work = update_ambient_temp };

static double ambient_get_recur_period(void)
{
	return 4;
}

struct scheduler_task ambient_tick_task = {
    .work = &ambient_work,
    .get_recur_period = ambient_get_recur_period,
};
