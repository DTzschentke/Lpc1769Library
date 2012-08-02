/******************************************************************************/
/** @file		nl_init.h
	@date		2012-07-24
	@version	0.01
  	@author		Daniel Tzschentke [2012-07-24]
	@brief		general initialization for the processor
*******************************************************************************/

#ifndef NL_INIT_H_
#define NL_INIT_H_

#include <cr_section_macros.h>
#include <NXP/crp.h>

#include "cortexM3.h"
#include "lpc17xx.h"		// register names with pointer arithmetic
#include "nl_lpc17xx.h"		// register names without pointer arithmetic
#include "nl_config.h"

#include "stdint.h"				// uintXX_t

//#include "core_cm3.h"			// Systick??



/******************************************************************************/
/**	@brief  switchable includes -> switch in config.h
*******************************************************************************/
#ifdef ADC_INIT
	#include "nl_adc.h"
#endif
#ifdef BOARD_INIT
	#include "nl_board.h"
#endif
#ifdef UART_INIT
	#include "nl_uart.h"
#endif
#ifdef MIDI_INIT
	#include "nl_midi.h"
#endif
#ifdef DEBUG_INIT
	#include "nl_debug.h"
#endif
#ifdef GPIO_INIT
	#include "nl_gpio.h"
#endif
#ifdef SYSTEM_INIT
	#include "system_LPC17xx.h"
#endif
#ifdef SSP_INIT
	#include "nl_ssp.h"
#endif
#ifdef KEYBED_INIT
	#include "nl_keybed.h"
#endif


/* template
#ifdef _INIT
	#include "nl_.h"
#endif
*/

/* global functions */
void INIT_SystemInit(void);

#endif /* NL_INIT */

/*******************************************************************************
	Changelog
	2012-07-24	DTZ		first version
*******************************************************************************/
