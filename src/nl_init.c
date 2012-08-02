/******************************************************************************/
/** @file		nl_init.c
	@date		2012-07-27
	@version	0.03
  	@author		Daniel Tzschentke [2012-07-24]
	@brief		- general initialization for the processor
				- all the init functions are called from here, depending on
				  which init switch is enabled/disabled in config.h
				- usually its not necessary to edit this file
*******************************************************************************/
#include "nl_init.h"


void INIT_SystemInit(void)
{
	#ifdef GPIO_INIT
		GPIO_Init();
	#endif
	#ifdef BOARD_INIT
		BOARD_Init();
	#endif
	#ifdef SYSTICK_INIT
		SysTick_Config(SYSTICK_VAL);
	#endif
	#ifdef SYSTEM_INIT
		SystemInit();
		SystemCoreClockUpdate();
	#endif
	#ifdef KEYBED_INIT
		KEYBED_Init();
	#endif
	#ifdef MIDI_INIT
		MIDI_Init();
	#endif
	#ifdef DEBUG_INIT
		DEBUG_Init();
	#endif
}




/*******************************************************************************
	Changelog
	2012-07-24	DTZ		first version
	2012-07-25	DTZ		was not compileable during include mistakes -> working
	2012-07-27	DTZ		init functions added
*******************************************************************************/
