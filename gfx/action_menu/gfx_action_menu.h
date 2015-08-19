/*
 * actionMenu.h
 *
 *  Created on: Aug 9, 2015
 *      Author: arkadi
 */

#ifndef GFX_GFX_ACTION_MENU_H_
#define GFX_GFX_ACTION_MENU_H_
#include "../../asf.h"
#include "../../utils.h"
#include "../../power-state/power-state.h"
#include "../../adc/adc.h"
#include "../../ASF/common/services/gfx_mono/sysfont.h"
#include "../../config/cnf_blk_header.h"




struct gfx_action_menu_t * presentMenu;


typedef enum gfx_item_action_type_t {
	ACTION_TYPE_NONE,
	ACTION_TYPE_SHOW_MENU,
	ACTION_TYPE_SHOW_DATA_FROM_FUNC,
	ACTION_TYPE_SHOW_DATA,
	ACTION_TYPE_SHOW_SPLASH
} item_action_type;

typedef struct item_data_t {
	PROGMEM_STRING_T title;
	char * text;
} item_data;


typedef struct item_action_t {
	item_action_type type;
	uint8_t menuId;
	struct gfx_action_menu_t * menu;
	struct item_data_t data;
	information_type info_type;
	struct gfx_mono_bitmap  splashData;
	gfx_mono_color_t PROGMEM_T * splash;
} item_action;


typedef struct gfx_action_menu_t {
	struct gfx_mono_menu * menu;
	uint8_t id;
	bool visible;
	item_action actions[GFX_MONO_MENU_ELEMENTS_PER_SCREEN];
	struct gfx_action_menu_t * parent;
}gfx_action_menu;

void gfx_action_menu_init(struct gfx_action_menu_t *actionMenu);
uint8_t gfx_action_menu_process_key(struct gfx_action_menu_t *actionMenu, uint8_t keycode);

#endif /* GFX_GFX_ACTION_MENU_H_ */
