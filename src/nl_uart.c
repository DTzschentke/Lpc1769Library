/******************************************************************************/
/** @file		nl_uart.c
	@date		2012-07-26
	@version	0.04
  	@author		Daniel Tzschentke [2012-04-23]
  	@todo		add uart 1 and 2 and make these as parameters for function calls
*******************************************************************************/
#include "stdint.h"
#include "system_LPC17xx.h"
#include "nl_lpc17xx.h"
#include "nl_uart.h"
#include "nl_gpio.h"
#include "nl_config.h"

#define LSR_RDR		0x01
#define LSR_THRE	0x20



/*****************************************************************************/
/** @brief		UART initialization
    @details	To avoid speed errors at 1 Mbaud this case is treated seperatly
  	@param		baudrate - UART speed in baud
  	@param		port - UART port, currently only
  					- UART_PORT_0
  					- UART_PORT_2
  					- UART_PORT_3
  	@tode		use a variable for LPC_UARTx to make this function shorter
******************************************************************************/
void UART_Init(uint32_t port, uint32_t baudrate)
{
	if (port == UART_PORT_0)
	{
		#if 1
		/* Turn on power to UART0 - p.63 */
		GPIO_BitSet(PCONP, PCUART0);
		/* set UART0 clock: pclk = cclk  - p.56 */
		GPIO_BitSet(PCLKSEL0, 6);
		GPIO_BitClr(PCLKSEL0, 7);
		/* baud rate calculation */
		if (baudrate == 1000000)
		{	/* this is a special calculation for an error rate:
			   0% at 1 MHz baudrate */
			/* clock settings */
			U0DLM = 0;
			U0DLL = 0x05;
			U0FDR = 0b01000001;		// MUL=4, DIV=1
			U0FCR = 0x07;			// Enable and reset TX and RX FIFO
		}
		else
		{
			uint32_t fdiv = ( SystemCoreClock/64 ) / baudrate ;
			/* clock settings */
			U0DLM = fdiv / 256;
			U0DLL = fdiv % 256;
			U0FCR = 0x07;			// Enable and reset TX and RX FIFO
		}
		/* UART format settings */
		U0LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
		/* set UART0 pins to UART function */
		GPIO_PinFnc(P002, GPIO_SF1);		// P002 = TXD0
		GPIO_PinFnc(P003, GPIO_SF1);		// P003 = RXD0
		#endif
	}
	else if (port == UART_PORT_2)
	{
		/* Turn on power to UART2 - p.63 */
		GPIO_BitSet(PCONP, PCUART2);

		/* UART format settings */
		U2LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0

		/* set UART2 pins to UART function */
		GPIO_PinFnc(P010, GPIO_SF1);		// P010 = TXD2
		GPIO_PinFnc(P011, GPIO_SF1);		// P011 = RXD2

		/* set UART2 clock: pclk = cclk  - p.57 */
		GPIO_BitSet(PCLKSEL1, 16);
		GPIO_BitClr(PCLKSEL1, 17);

		/* this is a special calculation for an error rate: 0% at 1 MHz baudrate */
		if (baudrate == 1000000)
		{
			/* clock settings */
			U2DLM = 0;
			U2DLL = 5;
			U2FDR = 0b01000001;	// MUL=4, DIV=1
			U2FCR = 0x07;			// Enable and reset TX and RX FIFO
		}
		else
		{
			uint32_t fdiv = ( SystemCoreClock/64 ) / baudrate ;
			/* clock settings */
			U2DLM = fdiv / 256;
			U2DLL = fdiv % 256;
			U2FCR = 0x07;			// Enable and reset TX and RX FIFO
		}
	}
	else if (port == UART_PORT_3)
	{
		/* Turn on power to UART3 - p.63 */
		GPIO_BitSet(PCONP, PCUART3);

		/* UART format settings */
		U3LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0

		/* set UART3 pins to UART function */
		GPIO_PinFnc(P000, GPIO_SF2);		// P000 = TXD3
		GPIO_PinFnc(P001, GPIO_SF2);		// P001 = RXD3

		/* set UART3 clock: pclk = cclk  - p.57 */
		GPIO_BitSet(PCLKSEL1, 18);
		GPIO_BitClr(PCLKSEL1, 19);

		/* this is a special calculation for an error rate: 0% at 1 MHz baudrate */
		if (baudrate == 1000000)
		{
			/* clock settings */
			U3DLM = 0;
			U3DLL = 5;
			U3FDR = 0b01000001;	// MUL=4, DIV=1
			U3FCR = 0x07;			// Enable and reset TX and RX FIFO
		}
		else
		{
			uint32_t fdiv = ( SystemCoreClock/64 ) / baudrate ;
			/* clock settings */
			U3DLM = fdiv / 256;
			U3DLL = fdiv % 256;
			U3FCR = 0x07;			// Enable and reset TX and RX FIFO
		}
	}
	else
	{
		/** @todo add debug message here: UART port not available */
	}
}



// ***********************
// Function to send character over UART
void UART_SendChar(char c)
{
	#if defined (DEBUG_VIA_UART_0)
		while( (U0LSR & LSR_THRE) == 0 );	// Block until tx empty
		U0THR = c;
	#elif defined (DEBUG_VIA_UART_2)
		while( (U2LSR & LSR_THRE) == 0 );	// Block until tx empty
		U2THR = c;
	#elif defined(DEBUG_VIA_UART_3)
		while( (U3LSR & LSR_THRE) == 0 );	// Block until tx empty
		U3THR = c;
	#endif
	
}



// ***********************
// Function to get character from UART
char UART_GetChar(void)
{
	char c;
	while( (U0LSR & LSR_RDR) == 0 );  // Nothing received so just block
	c = U0RBR; // Read Receiver buffer register
	return c;
}



// ***********************
// Function to prints the string out over the UART
void UART_PrintString(char *pcString)
{
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0)
	{
		UART_SendChar(pcString[i]); // print each character
		i++;
	}
}




/*******************************************************************************
	Changelog
	2012-04-20	0.01	DTZ		first version taken from Code Red Techn. lib
	2012-07-17	0.02	DTZ		renaming from ND_UART0 to nl_uart
	2012-07-25	0.03	DTZ		error correction developed from new names
	2012-07-26	0.04	DTZ		UART_PORT_x added
*******************************************************************************/

