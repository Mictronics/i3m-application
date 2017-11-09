/*
 * display_render.h|
 *
 *  Created on: Oct 25, 2015
 *      Author: arkadi
 */
#ifndef DISPLAY_DISPLAY_RENDER_H_
#define DISPLAY_DISPLAY_RENDER_H_

#include "glcd-0.5.2/glcd_font.h"
#include "glcd-0.5.2/glcd_text.h"
#include "gfx/gfx_components/gfx_text.h"
#include <string.h>

struct gfx_font {
	PROGMEM_BYTE_ARRAY_T source;
	uint8_t width;
	uint8_t height;
	bool is_numeric_only;
};

uint8_t draw_string_in_buffer(char *ch, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font);

void clear_screen(void);

#endif
