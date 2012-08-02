/******************************************************************************/
/** @file		ND_ADC.c
	@date		2012-07-24
	@version	0.03
	@brief    	analoge to digital converter
  	@author		Daniel Tzschentke [2012-07-02]
  	@todo 		extend this library
  	@todo 		make it more comfortable
*******************************************************************************/

#include "stdint.h"
#include "nl_lpc17xx.h"
#include "nl_gpio.h"
#include "nl_adc.h"



/******************************************************************************/
/**	@brief		default Init function for the ADC
	@details	only channel 0-3 are initialized
	@todo		replace pin mode inits with 'macro functions'
	@todo
*******************************************************************************/
void ADC_InitDefault(void)
{
	/* turn on ADC power and clock - set PCADC Power Clock Control Bit */
	GPIO_BitSet(PCONP, PCADC);

	GPIO_BitSet(AD0CR, 21);				// enable ADC in AD0CR, Bit PDN

	/* set clock: PCLK_ADC = CCLK/8; CCLK = 100 MHz => PCLK_ADC = 12.5 MHz */
	GPIO_BitSet(PCLKSEL0,24);
	GPIO_BitSet(PCLKSEL0,25);

	/** @todo	 divide by 100 - not sure if this is right -> bit 15:8 */
	AD0CR |= 0x6400;

	/* enable all ADC-Channels */

	/* ADC Channel 0 - P023 */
	GPIO_BitClr(PINMODE1, 14);			// disable internal pull up or
	GPIO_BitSet(PINMODE1, 15);			// pull down resistor
	GPIO_BitSet(PINSEL1, 14);			// select GPIO function as AD-Input (F01)
	GPIO_BitClr(PINSEL1, 15);

	/* ADC Channel 1 - P024 */
	GPIO_BitClr(PINMODE1, 16);			// disable internal pull up or
	GPIO_BitSet(PINMODE1, 17);			// pull down resistor
	GPIO_BitSet(PINSEL1, 16);			// select GPIO function as AD-Input (F01)
	GPIO_BitClr(PINSEL1, 17);

	/* ADC Channel 2 - P025 */
	GPIO_BitClr(PINMODE1, 18);			// disable internal pull up or
	GPIO_BitSet(PINMODE1, 19);			// pull down resistor
	GPIO_BitSet(PINSEL1, 18);			// select GPIO function as AD-Input (F01)
	GPIO_BitClr(PINSEL1, 19);

	/* ADC Channel 3 - P026 */
	GPIO_BitClr(PINMODE1, 20);			// disable internal pull up or
	GPIO_BitSet(PINMODE1, 21);			// pull down resistor
	GPIO_BitSet(PINSEL1, 20);			// select GPIO function as AD-Input (F01)
	GPIO_BitClr(PINSEL1, 21);

	#if 0
	/* ADC Channel 4 - P130 */

	/* ADC Channel 5 - P131 */

	GPIO_BitClr(PINMODE3, 30);			// disable internal pull up or
	GPIO_BitSet(PINMODE3, 31);			// pull down resistor
	GPIO_BitSet(PINSEL3, 30);			// select GPIO function as AD-Input
	GPIO_BitSet(PINSEL3, 31);


	/* ADC Channel 6 - P003 */

	/* ADC Channel 7 - P002 */
	#endif
}



/******************************************************************************/
/**	@brief		Init function for the ADC
	@details	- ADC Channel 0 - P023
				- ADC Channel 1 - P024
				- ADC Channel 2 - P025
				- ADC Channel 3 - P026
				- ADC Channel 4 - P130
				- ADC Channel 5 - P131
				- ADC Channel 6 - P003
				- ADC Channel 7 - P002
	@param		adcChannels - 8 bit binary value [ch.7|ch.6|..|ch.0]
								- 1 Enable Channel
								- 0 Disable Channel
							example: 	turn on channel 2 and 5:
										ND_ADC_Init(0b00100100);
	@todo		ADC: make a variable ADC_Init function with default as parameter
				and selectable channels to initilize
*******************************************************************************/
void ADC_Init(uint8_t adcChannels)
{
#if 0
/* turn on ADC power and clock - set PCADC Power Clock Control Bit */
	GPIO_BitSet(PCONP, PCADC);
	/* enable ADC in AD0CR, Bit PDN */
	GPIO_BitSet(AD0CR, 21);

	/** @todo ADC clock is fixed at the moment -> make it variable */
	/* set clock: PCLK_ADC = CCLK/8; CCLK = 100 MHz => PCLK_ADC = 12.5 MHz */
	GPIO_BitSet(PCLKSEL0,24);
	GPIO_BitSet(PCLKSEL0,25);
	/* divide by 100 - not sure if this is right -> bit 15:8 */
	AD0CR |= 0x6400;


	//if()
	{
		/* select GPIO P1.31 as an AD-Input */
		GPIO_BitSet(PINSEL3, 30);
		GPIO_BitSet(PINSEL3, 31);
		/* P1.31 has no internal pull up or pull down resistor */
		GPIO_BitClr(PINMODE3, 30);
		GPIO_BitSet(PINMODE3, 31);
	}
#endif
}



/******************************************************************************/
/**	@brief		De-Init function for the ADC
*******************************************************************************/
void ADC_DeInit(void)
{
	/* clear PDN bit */
	/* clear PCADC bit */
	/* gpios as inputs again */
}



/******************************************************************************/
/**	@brief		Select ADC Channel
	@param		channel values from 0 to 7
*******************************************************************************/
void ADC_SelectChannel(uint8_t channel)
{
	/* select channel zero as default*/
	AD0CR &= 0xFFFFFF00;
	/* select new one */
	GPIO_BitSet(AD0CR, channel);
}



/******************************************************************************/
/**	@brief		Starts the conversion and repeats it till stop function is
 				called?
*******************************************************************************/
void ADC_StartConversion(void)
{
	GPIO_BitSet(AD0CR, 24);
}



/******************************************************************************/
/**	@brief		Reads the result from the last converted channel
	@details	For reading an explicite channel use the GetResultFromChannel
				function
*******************************************************************************/
uint8_t ADC_GetResult(void)
{
	uint16_t value = AD0GDR;
	value = (value >> 9) & 0x7F;
	return (uint8_t) value;
}
