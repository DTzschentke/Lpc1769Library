/******************************************************************************/
/** @file		nl_config.h
	@date		2012-07-24
	@version	0.01
  	@author		Daniel Tzschentke [2012-07-24]
	@brief		main user interface file - configuration of most parameters like
					- processor board
*******************************************************************************/
#ifndef NL_CONFIG_H_
#define NL_CONFIG_H_



/******************************************************************************/
/**	@brief	user can choose the used board - currently only:
			- BOARD_EMPHASE_V01_REV_A
*******************************************************************************/
#define BOARD_EMPHASE_V01_REV_A
//#define BOARD_PROTOTYPE



/******************************************************************************/
/**	@brief	switches for the initialization routines
			enable or disable here
*******************************************************************************/
#define ADC_INIT
#define BOARD_INIT
#define GPIO_INIT
#define SSP_INIT
#define SYSTEM_INIT
#define SYSTICK_INIT
#define KEYBED_INIT
#define UART_INIT
#define MIDI_INIT
#define DEBUG_INIT


/******************************************************************************/
/**	@brief	select MIDI interface
*******************************************************************************/
//#define MIDI_VIA_UART_0
//#define MIDI_VIA_UART_2
//#define MIDI_VIA_UART_3
//#define MIDI_INTERFACE_SPEED	115200


/******************************************************************************/
/**	@brief  debug interface	- select the prefered debug hardware connection,
			currently only :
				- DEBUG_INTERFACE_UART_0
			further possible options:
				- #define DEBUG_INTERFACE_UART_1
				- #define DEBUG_INTERFACE_UART_2
				- #define DEBUG_INTERFACE_USB
*******************************************************************************/
#define DEBUG_VIA_UART_0
//#define DEBUG_VIA_UART_2
//#define DEBUG_VIA_UART_3
#define DEBUG_INTERFACE_SPEED	1000000




/******************************************************************************/
/**	@brief	System specific values
	@todo	make cpuClock a define value?!
*******************************************************************************/
//#define SYSTICKVALUE 100000




/******************************************************************************/
/**	@brief  debug level
 	 	 	LOWDEBUG:	low debug level - shows only the most important messages

 	 	 	HIGHDEBUG:	high debug level - every message will be shown
 	 	 				if highdebug is enable lowdebug will be turned on
 	 	 				automaticly
 	 	 				high debug works on a function level
*******************************************************************************/
//#define LOWDEBUG
#define HIGHDEBUG
//#define NODEBUG






/******************************************************************************/
/**	@brief	System Tick: The interrupt function SysTick_Handler() in main is
						 called every SYSTICK_TIME micro seconds.
	@todo	The systick value should be calculated automatically and changed if
			the cpu frequency changes. The CPU frequency is stored as a uint32
			but systick is a define -> How to solve this problem?
*******************************************************************************/
#define SYSTICK_VAL  2500		// *0.01 us @ 100 MHz
//uint32_t sysTickVal = 0;
//SYSTICK_TIME * (SystemCoreClock/1000000);


/* safety */

#if defined (MIDI_VIA_UART_0) && defined (DEBUG_VIA_UART_0) || \
defined (MIDI_VIA_UART_2) && defined (DEBUG_VIA_UART_2) || \
defined (MIDI_VIA_UART_3) && defined (DEBUG_VIA_UART_3)
#error	MIDI and DEBUG have the same UART PORT.
#endif

#ifdef HIGHDEBUG
#	ifndef LOWDEBUG
#		define	LOWDEBUG
#	endif
#endif


#endif /* NL_CONFIG_H_ */
/*******************************************************************************
	Changelog
	2012-07-24	DTZ		first version
*******************************************************************************/
