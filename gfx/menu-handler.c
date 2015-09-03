

#include "menu-handler.h"

#define is_key_selected(var,key) (var & key) == key
enum key_Select selected_Key;

#include "menu-handler.h"

//TODO: enter bootloader threw SW
void enter_bootloader_mode()
{
//	udc_detach();
//	udc_stop();
//	asm ("ldi r30, 0xFE\n"  /* Low byte to ZL */
//		  "ldi r31, 0x40\n" /* mid byte to ZH */
//		  "ldi r24, 0x00\n" /* high byte to EIND which lives */
//		  "out 0x3c, r24\n" /* at addr 0x3c in I/O space Extended Indirect register */
//		  "eijmp":  :: "r24", "r30", "r31");
}


void menu_handler()
{
	getPressedKey(&selected_Key);
	if (present_menu->visible){
		if (selected_Key == KEY_NONE) return;
		else if (selected_Key == KEY_HOLD)
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_BACK);
		else if (is_key_selected(selected_Key, KEY_SELECT))
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_ENTER);
		else if (is_key_selected(selected_Key, KEY_LEFT))
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_UP);
		else
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_DOWN);
	} else {
		if (is_key_selected(selected_Key, KEY_HOLD))
			gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_BACK);
	}
}

void delay()
{
	delay_ms(100);
}

bool is_bootloader_state()
{
	delay();
//	return ioport_get_value(FP_RIGHT_BUTTON) && ioport_get_value(FP_LEFT_BUTTON);
	return false;
}

void right_button_pressed()
{
	if (is_bootloader_state()) enter_bootloader_mode();
	else
		gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_DOWN);
}

void handle_bBack()
{
	if (present_menu->visible){
		if (present_menu->parent != NULL) gfx_action_menu_init(present_menu->parent);
	} else {
		gfx_action_menu_init(present_menu);
	}
}

void select_button_pressed()
{
	delay();
	if (!ioport_get_value(FP_OK_BUTTON)) handle_bBack();
	else
		gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_ENTER);
}

void left_button_pressed()
{
	if (is_bootloader_state()) enter_bootloader_mode();
	else
		gfx_action_menu_process_key(present_menu, GFX_MONO_MENU_KEYCODE_UP);
}

uint8_t size;

void action_types_init()
{
	struct gfx_action_menu *menu;
	struct gfx_item_action *action;
	for (int i=0; i < size; i++){
		menu = action_menus[i];
#ifdef DEBUG_MODE
		MSG_T_N("menu init in menu:",i)
#endif
		for (int j=0; j < menu->menu->num_elements; j++){
			action = &menu->actions[j];
			switch (action->type){
			case ACTION_TYPE_SHOW_MENU:
				set_menu_by_id(&(action->menu), action->menuId);
#ifdef DEBUG_MODE
				MSG_T_N("Menu id",action->menuId)
#endif
				break;
			default:
				break;
			}
		}
	}
}

void loadAction(struct gfx_item_action *action, struct cnf_action *cnfAction)
{
	struct cnf_action config_action;
	memcpy_P(&config_action, cnfAction,sizeof(struct cnf_action));
	action->type = config_action.type;
	switch(config_action.type){
	case ACTION_TYPE_SET_BIOS_STATE:
	case ACTION_TYPE_SHOW_FRAME:
#ifdef DEBUG_MODE
		MSG("action is frame")
#endif
		action->frame = malloc (sizeof(struct gfx_frame));
		gfx_frame_init(action->frame, config_action.frame);
	break;
	case ACTION_TYPE_SHOW_MENU:
#ifdef DEBUG_MODE
		MSG("action is menu")
#endif
		action->menuId = config_action.menuId;
		break;
	default:
		break;
	}
}

void load_config_block()
{
	struct cnf_blk config_block;
	struct cnf_menu config_menu;
	struct gfx_mono_menu *mono_menu;
	memcpy_P(&config_block,(void *) CONFIG_SECTION_ADDRESS, sizeof(struct cnf_blk));
	size = config_block.size;
	action_menus = malloc(sizeof (struct gfx_action_menu *) * config_block.size);
	for (int i=0; i < config_block.size; i++){
#ifdef DEBUG_MODE
		MSG_T_N("Config menu: " ,i)
#endif
		action_menus[i] = malloc(sizeof(struct gfx_action_menu));
		memcpy_P(&config_menu, config_block.menus[i], sizeof(struct cnf_menu));
		mono_menu = malloc(sizeof(struct gfx_mono_menu));
		memcpy_P(mono_menu, config_menu.menu, sizeof(struct gfx_mono_menu));
		action_menus[i]->menu= mono_menu;
		action_menus[i]->actions = malloc (sizeof(struct gfx_item_action) * mono_menu->num_elements);
		for (uint8_t j=0; j < mono_menu->num_elements; j++){
			loadAction(&(action_menus[i]->actions[j]), config_menu.actions[j]);
		}
	}
	action_types_init();
}

void set_menu_by_id(struct gfx_action_menu **menu, uint8_t index)
{
	if (index < size){
		*menu = action_menus[index];
		#ifdef DEBUG_MODE
			MSG_T_N("setting menu number: ", index)
		#endif
	}
}
