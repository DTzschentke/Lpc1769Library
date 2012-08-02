/******************************************************************************/
/** @file		nl_midi.c
	@date		2012-07-27
	@version	0.01
  	@author		Daniel Tzschentke [2012-07-27]
	@brief		
*******************************************************************************/
#include "stdint.h"
#include "lpc17xx.h"
#include "nl_uart.h"
#include "nl_config.h"



void MIDI_Init(void)
{
	#if defined (MIDI_VIA_UART_0)
		UART_Init(UART_PORT_0, MIDI_INTERFACE_SPEED);
	#elif defined(MIDI_VIA_UART_2)
		UART_Init(UART_PORT_2, MIDI_INTERFACE_SPEED);
	#elif defined(MIDI_VIA_UART_3)
		UART_Init(UART_PORT_3, MIDI_INTERFACE_SPEED);
	#endif
}



/******************************************************************************/
/**	@brief		sends a MIDI command over UART
	@param		port - UART port, currently only:
  					- UART_PORT_0
  					- UART_PORT_2
  					- UART_PORT_3
	@tode		use a variable for LPC_UARTx to make this function shorter
*******************************************************************************/
void MIDI_SendByte(uint8_t midiByte)
{
	#if defined (MIDI_VIA_UART_0)
		LPC_UART0->THR = midiByte;
	#elif defined(MIDI_VIA_UART_2)
		LPC_UART2->THR = midiByte;
	#elif defined(MIDI_VIA_UART_3)
		LPC_UART3->THR = midiByte;
	#endif
	/** @todo  leaving the folling line out is experimental -
	   	   	   if a problem with uart occurs, try uncommenting */
	// while( (LPC_UART0->LSR & LSR_THRE) == 0 );	// Block until tx empty
}





/******************************************************************************/
/**	@brief  	Sends a 3-byte MIDI message
*******************************************************************************/
void MIDI_SendCommand()
{
	/** @todo add a complete midi command (3 Byte) */
}


/*******************************************************************************
	Changelog
	2012-07-27		DTZ		first version
*******************************************************************************/


