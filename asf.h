/**
 * \file
 *
 * \brief Autogenerated API include file for the Atmel Software Framework (ASF)
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef ASF_H
#define ASF_H

/*
 * This file includes all API header files for the selected drivers from ASF.
 * Note: There might be duplicate includes required by more than one driver.
 *
 * The file is automatically generated and will be re-written when
 * running the ASF driver selector tool. Any changes will be discarded.
 */

#include "conf.h"
// From module: ADC - XMEGA A/AU Implementation
#include "ASF/xmega/drivers/adc/adc.h"

// From module: CPU specific features
#include "ASF/xmega/drivers/cpu/ccp.h"
#include "ASF/xmega/drivers/cpu/xmega_reset_cause.h"

// From module: Delay routines
#include "ASF/common/services/delay/delay.h"

// From module: GFX Monochrome - Monochrome Graphic Library
#include "ASF/common/services/gfx_mono/gfx_mono.h"

// From module: GPIO - General purpose Input/Output
#include "ASF/common/services/gpio/gpio.h"

// From module: Generic board support
#include "ASF/common/boards/board.h"

// From module: IOPORT - General purpose I/O service
#include "ASF/common/services/ioport/ioport.h"

// From module: Interrupt management - XMEGA implementation
#include "ASF/common/utils/interrupt.h"

// From module: NVM - Non Volatile Memory
#include "ASF/xmega/drivers/nvm/nvm.h"

// From module: PMIC - Programmable Multi-level Interrupt Controller
#include "ASF/xmega/drivers/pmic/pmic.h"

// From module: Part identification macros
#include "ASF/common/utils/parts.h"

// From module: SPI - Serial Peripheral Interface
#include "ASF/xmega/drivers/spi/spi.h"

// From module: SPI - XMEGA implementation
#include "ASF/common/services/spi/spi_master.h"
#include "ASF/common/services/spi/xmega_spi/spi_master.h"

// From module: SSD1306 OLED controller
#include "ASF/common/components/display/ssd1306/font.h"
#include "ASF/common/components/display/ssd1306/ssd1306.h"

// From module: STK600-RC064X LED support enabled
#include "ASF/xmega/boards/stk600/rc064x/led.h"

// From module: Sleep Controller driver
#include "ASF/xmega/drivers/sleep/sleep.h"

// From module: Sleep manager - XMEGA A/AU/B/D implementation
#include "ASF/common/services/sleepmgr/sleepmgr.h"
#include "ASF/common/services/sleepmgr/xmega/sleepmgr.h"

// From module: System Clock Control - XMEGA A1U/A3U/A3BU/A4U/B/C implementation
#include "ASF/common/services/clock/sysclk.h"

// From module: TWI - Two-Wire Interface - XMEGA implementation
//#include <twi_master.h>
//#include <twi_slave.h>
//#include <xmega_twi/twi_master.h>
//#include <xmega_twi/twi_slave.h>

//// From module: TWI - Two-wire Master and Slave Interface
//#include <twim.h>
//#include <twis.h>

// From module: XMEGA compiler driver
#include "ASF/xmega/utils/compiler.h"
#include "ASF/xmega/utils/status_codes.h"

#endif // ASF_H
