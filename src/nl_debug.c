/******************************************************************************/
/** @file		ND_global.c
	@date		2012-07-25
	@version	0.02
  	@author		Daniel Tzschentke [2012-07-24]
	@brief		This file handles:
					- debug message service
					- debug/error led
					- reset of error leds
	@details	This file is merged from previous files
					- ND_global.c
					- ND_Error.c
	@todo		use printf for debug messages
*******************************************************************************/
#include "stdlib.h" 			// itoa
#include "nl_gpio.h"
#include "nl_debug.h"
#include "nl_board.h"
#include "system_LPC17xx.h"		// system core clock

#if defined(DEBUG_VIA_UART_0) || defined(DEBUG_VIA_UART_2) || \
	defined(DEBUG_VIA_UART_3)
	#include "nl_uart.h"
#endif

char buf[10];	// makeshift - not needed if printf is used



/******************************************************************************/
/**	@brief	Initialization of the error leds and the error reset button
			and uart initialization
*******************************************************************************/
void DEBUG_Init(void)
{
	/* LEDs as output */
	GPIO_PinOpt(DEBUG_LED_ERROR);
	GPIO_PinOpt(DEBUG_LED_TASK);
	/* turn LEDs of */
	GPIO_PinClr(DEBUG_LED_ERROR);
	GPIO_PinClr(DEBUG_LED_TASK);
	/* button as input */
	GPIO_PinMod(DEBUG_BUTTON_TASK_ERROR_RESET, GPIO_PULL_UP);
	GPIO_PinIpt(DEBUG_BUTTON_TASK_ERROR_RESET);

	#if defined (DEBUG_VIA_UART_0)
		UART_Init(UART_PORT_0, DEBUG_INTERFACE_SPEED);
	#elif defined (DEBUG_VIA_UART_2)
		UART_Init(UART_PORT_2, DEBUG_INTERFACE_SPEED);
	#elif defined (DEBUG_VIA_UART_3)
		UART_Init(UART_PORT_3, DEBUG_INTERFACE_SPEED);
	#endif
}



/******************************************************************************/
/**	@brief	With this function the Error Led on the emphase mainboard can be
			turned on. The programm will stay until the Task_Error_Reset button
			is pressed.
*******************************************************************************/
void DEBUG_ErrorLed(void)
{
	GPIO_PinSet(DEBUG_LED_ERROR);
	while (GPIO_PinGet(DEBUG_BUTTON_TASK_ERROR_RESET) == 1);
	GPIO_PinClr(DEBUG_LED_ERROR);
}



/******************************************************************************/
/**	@brief	With this function the Task Overflow Led on the emphase mainboard
			can be turned on.
	@todo	What functionality should be offered here?
*******************************************************************************/
void DEBUG_TaskOverflowLed(void)
{

}



/******************************************************************************/
/**	@brief	wrapper function - if a different debug interface is used,
			just change it here.
	@todo	where to select the hw transfer interface?
*******************************************************************************/
void DEBUG_SendMsg(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART_SendChar(pcString[i]); // print each character
		i++;
	}
}



/******************************************************************************/
/**	@brief	With this function the Task Overflow Led on the emphase mainboard
			can be turned on.
	@todo	What functionality should be offered here?
*******************************************************************************/
void DEBUG_WelcomeMsg(void)
{
	#if defined(HIGHDEBUG) || defined(LOWDEBUG)
	/* print welcome message */
	UART_PrintString("\n\n\n");
	UART_PrintString("#####################################################################################\n");
	UART_PrintString("#                         NONLINEAR LABS - Emphase Keyboard                         #\n");
	UART_PrintString("####################################################################################\n\n");
	UART_PrintString(__DATE__);
	UART_PrintString(" - ");
	UART_PrintString(__TIME__);
	UART_PrintString("\n\n");
	/* print sytem clock in Mhz */
	UART_PrintString("SystemCoreClock:      ");
	itoa((SystemCoreClock/1000000), buf, 10);
	UART_PrintString(buf);
	UART_PrintString(" MHz \n");
	/* print system tick time */
	UART_PrintString("SysTick every:        ");
	itoa((SystemCoreClock/SYSTICK_VAL), buf, 10);
	UART_PrintString(buf);
	UART_PrintString(" us\n\n");
	#endif

	#if defined(LOWDEBUG)
	UART_PrintString("ND_GLOBAL_Init:       Init succeeded.\n\n");
	#endif
}






/*******************************************************************************
	Changelog
	2012-07-18	DTZ		first version
	2012-07-25	DTZ		made the debug interface selectable
*******************************************************************************/
