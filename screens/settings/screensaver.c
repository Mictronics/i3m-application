/*
 * screensaver.c
 *
 * Created: 3/18/2017 9:28:35 PM
 *  Author: Nikita
 */

#include "gfx/gfx_assets.h"
#include "gfx/gfx_gui_control.h"
#include "gfx/gfx_components/gfx_graphic_menu.h"
#include "gfx/gfx_components/gfx_information.h"
#include "eeprom/eeprom_layout.h"
#include "settings.h"
#include "layout.h"

#define SCREEN_SAVER_TIME_STEP		1
#define MIN_SCREEN_SAVER_TIME		5
#define MAX_SCREEN_SAVER_TIME_MIN	15

void screen_saver_off(void);
void screen_saver_on(void);
uint8_t screen_saver_get_time_seconds(void);

typedef enum {
	UNIT_SECONDS = 0x00,
	UNIT_MINUTES = 0x40
} screen_saver_unit_e;

static screen_saver_unit_e screen_saver_unit = SCREEN_SAVER_DEFAULT_UNIT;
static uint8_t screen_saver_enabled = SCREEN_SAVER_DEFAULT_ENABLE;
static uint8_t screen_saver_time = SCREEN_SAVER_DEFAULT_TIME;

void screen_saver_off(void){
	ssd1306_display_off();
}

void screen_saver_on(void){
	ssd1306_display_on();
}


uint8_t screen_saver_get_time_seconds(void){
	if(!screen_saver_enabled) {
		return 0;
	}

	if(screen_saver_unit == UNIT_MINUTES) {
		return screen_saver_time * 60;
	} else {
		return screen_saver_time;
	}
}

static uint8_t eeprom_get_screen_saver_time(void)
{
	uint8_t t = eeprom_read_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS);
	screen_saver_unit = t & 0xC0; // get unit
	screen_saver_time = t & 0x3F; // get time value

	if(screen_saver_unit == UNIT_SECONDS && screen_saver_time > 59) {
		// Zero is a valid value
		screen_saver_unit = UNIT_SECONDS;
		return SCREEN_SAVER_DEFAULT_TIME;
	} else if (screen_saver_unit == UNIT_MINUTES && screen_saver_time > MAX_SCREEN_SAVER_TIME_MIN)
	{
		screen_saver_unit = UNIT_SECONDS;
		return SCREEN_SAVER_DEFAULT_TIME;
	}
	return screen_saver_time;
}

static void eeprom_increase_screen_saver_time(void)
{
	uint8_t sst = eeprom_get_screen_saver_time();
	if(screen_saver_unit == UNIT_SECONDS) {
		if(sst < MIN_SCREEN_SAVER_TIME) {
			sst = MIN_SCREEN_SAVER_TIME;
		} else if(sst < 59) {
			++sst;
		} else {
			screen_saver_unit = UNIT_MINUTES;
			sst = 1;
		}
	} else if(screen_saver_unit == UNIT_MINUTES) {
		if(sst < MAX_SCREEN_SAVER_TIME_MIN) {
			++sst;
		} else {
			return;
		}
	} else {
		// Invalid unit
		return;	
	}
	screen_saver_enabled = 1;
	screen_saver_time = sst;
	eeprom_write_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS, screen_saver_unit | sst);
	eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, screen_saver_enabled);
}

static void eeprom_decrease_screen_saver_time(void)
{
	uint8_t sst = eeprom_get_screen_saver_time();
	if(screen_saver_unit == UNIT_MINUTES) {
		if(sst > 1) {
			--sst;
		} else {
			screen_saver_unit = UNIT_SECONDS;
			sst = 59;
		}
	} else if(screen_saver_unit == UNIT_SECONDS) {
		if(sst > MIN_SCREEN_SAVER_TIME) {
			--sst;
		} else {
			sst = 0;
			screen_saver_enabled = 0;
		}
	} else {
		// Invalid unit
		return;
	}
	screen_saver_time = sst;
	eeprom_write_byte(SCREEN_SAVER_TIME_EEPROM_ADDRESS, screen_saver_unit | sst);
	eeprom_write_byte(SCREEN_SAVER_CONFIG_EEPROM_ADDRESS, screen_saver_enabled);
}

static void handle_screen_saver_buttons(struct gfx_information *info, uint8_t key)
{
	switch (key) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		gfx_switch_to_current_menu();
		return;
	case GFX_MONO_MENU_KEYCODE_DOWN:
		eeprom_decrease_screen_saver_time();
		break;
	case GFX_MONO_MENU_KEYCODE_UP:
		eeprom_increase_screen_saver_time();
		break;
	}

	//Yes it is kind of fucked up that we update brightness by changing contrast.
	//TODO: figure out why this is implemented this way and hopefully fix it.
	//ssd1306_set_contrast(eeprom_get_brightness_value());
	gfx_redraw_current_frame();
}

static void sprintf_screen_saver(struct gfx_information *info, char *output_str)
{
	if (!screen_saver_enabled) {
		sprintf(output_str, "OFF");
	} else if(screen_saver_unit == UNIT_SECONDS) {
		sprintf(output_str, "%2ds  ", screen_saver_time);
	} else if (screen_saver_unit == UNIT_MINUTES)
	{
		sprintf(output_str, "%2dmin", screen_saver_time);
	}
}

static void set_screen_saver_draw_graphic_signs(struct gfx_information *info)
{
	if(screen_saver_unit == UNIT_SECONDS) {
		draw_control_signs_numeric(screen_saver_time, 0, 60);
	} else if (screen_saver_unit == UNIT_MINUTES)
	{
		draw_control_signs_numeric(screen_saver_time, 0, MAX_SCREEN_SAVER_TIME_MIN);
	}
}

int gfx_information_init_set_screen_saver(struct gfx_information *info)
{
	eeprom_get_screen_saver_time();
	info->to_string = sprintf_screen_saver;
	info->draw_controls = set_screen_saver_draw_graphic_signs;
	info->handle_buttons = handle_screen_saver_buttons;
	return 0;
}