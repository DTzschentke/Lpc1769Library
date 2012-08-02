/******************************************************************************/
/** @file		nl_gpio.c
	@date		2012-07-25
	@version	0.05
  	@author		Daniel Tzschentke [2012-04-23]
  	@todo		complete init and mode functions with Port 3 and 4
*******************************************************************************/
#include "nl_lpc17xx.h"
#include "nl_gpio.h"
#include "stdint.h"



/******************************************************************************/
/**	@brief	Resets all GPIO functions - should be called at the beginning
*******************************************************************************/
void GPIO_Init(void)
{
	/* mask -> all GPIO pins can be set and cleared */
	FIO0MASK = 0x00000000;
	FIO1MASK = 0x00000000;
	FIO2MASK = 0x00000000;
	FIO3MASK = 0x00000000;
	FIO4MASK = 0x00000000;

	/* turn off internal pull up/down resistors -> float */
	PINMODE0 = 0xAAAAAAAA;
	PINMODE1 = 0xAAAAAAAA;
	PINMODE2 = 0xAAAAAAAA;
	PINMODE3 = 0xAAAAAAAA;
	PINMODE4 = 0xAAAAAAAA;

	/* set all pins as GPIO (Function 00) */
	PINSEL0 = 0x00000000;
	PINSEL1 = 0x00000000;
	PINSEL2 = 0x00000000;
	PINSEL3 = 0x00000000;
	PINSEL4 = 0x00000000;

	/* set all pins as input to prevent short circuits */
	FIO0DIR = 0x00000000;
	FIO1DIR = 0x00000000;
	FIO2DIR = 0x00000000;
	FIO3DIR = 0x00000000;
	FIO4DIR = 0x00000000;
}



/******************************************************************************/
/**	@brief	Sets the mode of a pin - described in mode
	@param	port,pin	 	Pxxx, e.g. P012
	@param 	mode
				- PULL_UP		(pull up resistor)
	 	 		- PULL_DOWN		(pull down resistor)
	 	 		- FLOATING		(no resistor)
	 	 		- REPEATER		(resistor depending on the last state)
	@example	PinMode(P012, PULL_UP);
*******************************************************************************/
void GPIO_PinMod(uint8_t port, uint8_t pin, uint32_t mode)
{
	switch (port)
	{
		case 0:
		{
			if (pin < 16)
			{
				GPIO_BitClr(PINMODE0, (2 * pin));			// clear bits
				GPIO_BitClr(PINMODE0, (2 * pin + 1));
				PINMODE0 |= (mode << (2 * pin));			// set mode
			}
			else
			{
				GPIO_BitClr(PINMODE1, (2 * pin - 32));		// clear bits
				GPIO_BitClr(PINMODE1, (2 * pin - 31));
				PINMODE1 |= (mode << (2 * pin - 32));		// set mode
			}
		}
		break;

		case 1:
		{
			if (pin < 16)
			{
				GPIO_BitClr(PINMODE2, (2 * pin));			// clear bits
				GPIO_BitClr(PINMODE2, (2 * pin + 1));
				PINMODE2 |= (mode << (2 * pin));		// set mode
			}
			else
			{
				GPIO_BitClr(PINMODE3, (2 * pin - 32));		// clear bits
				GPIO_BitClr(PINMODE3, (2 * pin - 31));
				PINMODE3 |= (mode << (2 * pin -32));		// set mode
			}
		}
		break;

		case 2:
		{
			GPIO_BitClr(PINMODE4, (2 * pin));				// clear bits
			GPIO_BitClr(PINMODE4, (2 * pin + 1));
			PINMODE4 |= (mode << (2 * pin));			// set mode
		}
		break;
	}
}






/******************************************************************************/
/**	@brief		selects the function of a GPIO
	@param		port,pin	 	Pxxx, e.g. P012
	@param 		function (see datasheet or PinNames.h for function info)
					- GPIO_SF0 	(always GPIO)
					- GPIO_SF1	(special function 1)
					- GPIO_SF2	(special function 2)
					- GPIO_SF3	(special function 3)
	@example	PinFunction(P012, GPIO_F2);
	@todo 		replace BitClr with BitsVal
*******************************************************************************/
void GPIO_PinFnc(uint8_t port, uint8_t pin, uint8_t function)
{
	switch (port)
	{
		case 0:
		{
			if (pin < 16)
			{
				GPIO_BitClr(PINSEL0, (2 * pin));
				GPIO_BitClr(PINSEL0, (2 * pin + 1));
				PINSEL0 |= (function << (2 * pin));
			}
			else
			{
				GPIO_BitClr(PINSEL1, (2 * pin - 32));
				GPIO_BitClr(PINSEL1, (2 * pin - 31));
				PINSEL1 |= (function << (2 * pin - 32));
			}
		}
		break;

		case 1:
		{
			if (pin < 16)
			{
				GPIO_BitClr(PINSEL2, (2 * pin));
				GPIO_BitClr(PINSEL2, (2 * pin + 1));
				PINSEL2 |= (function << (2 * pin));
			}
			else
			{
				GPIO_BitClr(PINSEL3, (2 * pin - 32));
				GPIO_BitClr(PINSEL3, (2 * pin - 31));
				PINSEL3 |= (function << (2 * pin -32));
			}
		}
		break;

		case 2:
		{
			GPIO_BitClr(PINSEL4, (2 * pin));
			GPIO_BitClr(PINSEL4, (2 * pin + 1));
			PINSEL4 |= (function << (2 * pin));
		}
		break;
	}
}




/*******************************************************************************
	Changelog
	2012-07-05	DTZ		PinMode Function added and tested
	2012-07-17	DTZ		Changed function name 'PinMode' to 'ND_GPIO_PinMode'
						during unification
						added pin function function
	2012-07-24	DTZ		renameing
	2012-07-25	DTZ		BitClr function named to GPIO_BitClr
 *******************************************************************************/
