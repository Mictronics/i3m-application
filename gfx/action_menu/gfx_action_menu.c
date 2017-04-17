#include "gfx_action_menu.h"
#include "gfx_item_action.h"
#include "gfx_action_menu_dmi.h"
#include "gfx/gfx_components/gfx_label.h"
#include "gfx/action_menu/graphic_menu_theme/graphic_menu_theme.h"
#include "screen_saver/screen_saver.h"
#include "lib/syntax.h"

#define MAIN_MENU_ID 	0

enum display_state display_state;

struct gfx_frame *frame_present;
struct gfx_action_menu *present_menu;

bool ok_button = false;
bool left_button = false;
bool right_button = false;

void gfx_action_menu_init(void)
{
	set_menu_by_id(&present_menu, 0);
	present_menu->draw(present_menu);
}

static void update_action_visibility(struct gfx_item_action *action)
{
	if (action->type == ACTION_TYPE_SHOW_DMI_MENU) {
		action->visible = computer_data.details.direct_string != 0;
		return;
	} else if (action->type != ACTION_TYPE_SHOW_FRAME) {
		action->visible = true;
		return;
	}

	bool visible = true;
	list_foreach(struct gfx_information_node *, action->frame->information_head, info_node) {
		visible = info_node->information.is_valid(&info_node->information);
		if (!visible)
			break;
	}

	action->visible = visible;
}

static void gfx_action_menu_move_cursor(struct gfx_action_menu *action_menu)
{
	update_screen_timer();
	action_menu->draw(action_menu);
	graphic_menu_deselect_item(action_menu, action_menu->menu->last_selection);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
}

void gfx_action_menu_display(struct gfx_action_menu *action_menu)
{
	update_screen_timer();
	frame_present = 0;
	present_menu = action_menu;
	for (int i = 0; i < action_menu->menu->num_elements; i++)
		update_action_visibility(&action_menu->actions[i]);
	clear_screen();
	graphic_menu_format(action_menu);
	graphic_menu_select_item(action_menu, action_menu->menu->current_selection);
	gfx_mono_ssd1306_put_framebuffer();
	display_state = DISPLAY_MENU;
}

void gfx_redraw_current_frame(void)
{
	frame_present->draw(frame_present);
}

void gfx_show_screen_saver(enum display_state state)
{
	switch (state) {
	case DISPLAY_CLOCK:
		if (clock)
			switch_to_frame(clock, DISPLAY_CLOCK);
		break;
	case DISPLAY_DASHBOARD:
		if (dashboard)
			switch_to_frame(dashboard, DISPLAY_DASHBOARD);
		break;
	case DISPLAY_LOGO:
		show_logo(splash);
		break;
	default:
		break;
	}
}

struct gfx_frame *dashboard;
struct gfx_frame *clock;
struct gfx_frame *splash;

void show_logo(struct gfx_frame *frame)
{
	update_screen_timer();
	frame_present = frame;
	clear_screen();
	gfx_mono_generic_put_bitmap(&splash_bitmap, 0, 0);
	gfx_mono_ssd1306_put_framebuffer();
	display_state = DISPLAY_LOGO;
}

static void switch_to_frame(struct gfx_frame *frame, enum display_state new_state)
{
	update_screen_timer();
	disable_screen_saver_mode();
	frame_present = frame;
	frame->draw(frame);
	display_state = new_state;
}

static void gfx_action_menu_process_key(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame)
{
	reset_screen_saver();
	enable_screen_saver_mode();
	gfx_handle_key_pressed(action_menu, keycode, from_frame);
}

void gfx_handle_key_pressed(struct gfx_action_menu *action_menu, uint8_t keycode, bool from_frame)
{
	struct gfx_item_action *selected_action;
	switch(keycode) {
	case GFX_MONO_MENU_KEYCODE_ENTER:
		selected_action = &action_menu->actions[(action_menu->menu)->current_selection];
		frame_present = 0;
		switch (selected_action->type) {
		case ACTION_TYPE_SHOW_FRAME:
			switch_to_frame(selected_action->frame, DISPLAY_FRAME);
			break;
		case ACTION_TYPE_SHOW_MENU:
			if (from_frame && selected_action->menu_id == MAIN_MENU_ID)
				break;
			selected_action->menu->draw(selected_action->menu);
			break;
		case ACTION_TYPE_SHOW_DMI_MENU:
			if (!computer_data.details.direct_string) {
				present_menu->draw(present_menu);
				break;
			}

			set_dmi_menu();
			if (is_dmi_set)
				gfx_action_menu_display(&dmi_menu);
			break;
		default:
			break;
		}
		break;
	default:
		if (from_frame && ((keycode == GFX_MONO_MENU_KEYCODE_DOWN && action_menu->menu->current_selection == 0) ||
					(keycode == GFX_MONO_MENU_KEYCODE_UP && action_menu->menu->current_selection == action_menu->menu->num_elements - 2)))
			return;
		 gfx_mono_menu_process_key(action_menu->menu, keycode, action_menu->is_progmem);
		 if (from_frame)
			 gfx_action_menu_process_key(action_menu, GFX_MONO_MENU_KEYCODE_ENTER, true);
		 else
			 gfx_action_menu_move_cursor(present_menu);
		 break;
	}
}

void handle_back_to_menu(void)
{
	reset_screen_saver();
	clear_screen();
	frame_present = 0;
	enable_screen_saver_mode();
	present_menu->draw(present_menu);
}

void gfx_display_msg(char *msg)
{
	uint16_t font_id = fonts_size > 1 ? 2 : GLCD_FONT_SYSFONT_5X7;
	display_state = DISPLAY_WAIT_FOR_USER_ACK;
	clear_screen();
	uint8_t msg_x = GFX_MONO_LCD_WIDTH / 2 - ((strlen(msg) * (get_font_by_type(font_id))->width) / 2);
	uint8_t msg_y = 20;
	draw_string_in_buffer(msg, msg_x, msg_y, get_font_by_type(font_id));
	gfx_mono_ssd1306_put_framebuffer();
}

static void handle_side_button(uint8_t keycode)
{
	if (!frame_present) {
		gfx_action_menu_process_key(present_menu, keycode, false);
		return;
	}

	if (frame_present->information_head && frame_present->information_head->information.handle_buttons)
		frame_present->information_head->information.handle_buttons(keycode);
	else
		frame_present->handle_buttons(keycode);
}

static void handle_buttons(void *data)
{
	if (ok_button) {
		handle_side_button(GFX_MONO_MENU_KEYCODE_ENTER);
		return;
	}

	if (left_button) {
		handle_side_button(GFX_MONO_MENU_KEYCODE_DOWN);
		return;
	}

	if (right_button) {
		handle_side_button(GFX_MONO_MENU_KEYCODE_UP);
		return;
	}
}

struct work button_work = { .do_work = handle_buttons, .data = NULL, .next = NULL, };

void handle_button_pressed(void)
{
	left_button = gpio_pin_is_high(FP_LEFT_BUTTON);
	right_button = gpio_pin_is_high(FP_RIGHT_BUTTON);
	ok_button = gpio_pin_is_high(FP_OK_BUTTON);
	insert_work(&button_work);
}

static void update_screen(void *data)
{
	if (display_state == DISPLAY_WAIT_FOR_USER_ACK)
		return;

	if (display_state == DISPLAY_MENU)
		present_menu->draw(present_menu);
	else
		frame_present->draw(frame_present);
}

static struct work update_screen_work = { .do_work = update_screen };

static double screen_get_recur_period(void)
{
    return 1;
}

struct scheduler_task screen_sec_task = {
    .work = &update_screen_work,
    .get_recur_period = screen_get_recur_period,
};