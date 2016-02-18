/*
 * display_render.c
 *
 *  Created on: Oct 25, 2015
 *      Author: arkadi
 */

#include "display_render.h"

bool is_valid_char(char ch)
{
	return ch != '\0' && ch != '\n';
}

void clear_string_background(uint8_t length, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font)
{
	gfx_mono_draw_filled_rect(x, y, length, font->height + 3, GFX_PIXEL_CLR);
}

uint8_t length_P(char *str)
{
	uint8_t count = 0;
	uint8_t temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));
	while (is_valid_char(temp_char)) {
		count++;
		temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));
	}
	return count;
}

uint8_t draw_string_in_buffer(char *str, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font, uint8_t old_len)
{
	uint8_t print_length = 0;
	uint8_t j = 0;
	uint8_t length = strlen(str) * (font->width + 1);

	if (length >= GFX_MONO_LCD_WIDTH) {
		gfx_mono_draw_string(str, 0, y, &sysfont); /* clear all line */
	} else {

		glcd_set_font_from_font(font);

		length += font->width * 2;
		clear_string_background(max(old_len, length), x, y, font);

		while (str[j] != '\0')
		{
			if (x > 120 || str[j] == '\n')
			{
				y += font->height + 2;
				x = 0;
				if (str[j] == '\n')
					j ++;
				continue;
			}

			glcd_draw_char_xy(x, y, str[j]);

			print_length += 8;
			x += font->width + 1;
			j++;
		}
	}
	return print_length;
}

uint8_t draw_string_in_buffer_P(char *str, uint8_t x, uint8_t y, struct glcd_FontConfig_t *font)
{
	uint8_t print_length = 0;
	uint8_t length = length_P(str) * (font->width + 1);

	if (length >= GFX_MONO_LCD_WIDTH) {
			gfx_mono_draw_string(str, 0, y, &sysfont);
	} else {
		if (x == 0)
			x = (GFX_MONO_LCD_WIDTH - length - 30) / 2;

		uint8_t temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));

		glcd_set_font_from_font(font);

		while (temp_char){
			if (temp_char == '\n' || x > 120){
				draw_string_in_buffer_P(str, x, y + font->height + 2, font);
				break;
			}

			glcd_draw_char_xy(x, y, temp_char);

			x += font->width +  1;
			print_length += 8;
			temp_char = PROGMEM_READ_FAR_BYTE((uint8_t PROGMEM_PTR_T)(str++));
		}

		glcd_write();
	}
	return print_length;
}
