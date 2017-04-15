/*
 * uart.h
 *
 *  Created on: Nov 15, 2015
 *      Author: arkadi
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include <stdio.h>

void uart_init(void);
void uart_send_char(char c);
void uart_send_string(char *text);
void uart_send_num(uint32_t num, uint8_t radix);

#endif /* UART_UART_H_ */
