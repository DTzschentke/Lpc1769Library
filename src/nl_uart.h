/*****************************************************************************/
/** @file		uart0.h
	@date		2012-07-26
	@brief
	@version	0.03
    @details  	modified version of uart0.h from Code Red Technologies Ltd.
  	@author		Daniel Tzschentke [2012-04-20]
******************************************************************************/
#ifndef ND_UART_H_
#define ND_UART_H_

#include "stdint.h"

#define UART_PORT_0 	0
#define UART_PORT_2 	2
#define UART_PORT_3 	3

/* global functions */
void UART_Init(uint32_t port, uint32_t baudrate);
void UART_SendChar(char c);
char UART_GetChar(void);
void UART_PrintString(char *pcString);

#endif /* UART_H_ */

/*******************************************************************************
	Changelog
	2012-04-20	0.01	DTZ		first version
	2012-07-24	0.02	DTZ		renamed from ND_UART0.h to nl_uart.h
	2012-07-26	0.03	DTZ		UART_PORT_x added
*******************************************************************************/
