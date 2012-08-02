/******************************************************************************/
/** @file		ND_ADC.h
	@date		2012-07-24
	@version	0.03
	@brief    	analoge to digital converter
  	@author		Daniel Tzschentke [2012-07-02]
*******************************************************************************/

#ifndef ND_ADC_H_
#define ND_ADC_H_

void ADC_InitDefault(void);
void ADC_Init(uint8_t adcChannels);
void ADC_DeInit(void);
void ADC_SelectChannel(uint8_t channel);
void ADC_StartConversion(void);
uint8_t ADC_GetResult(void);

#endif /* ND_ADC_H_ */


/*******************************************************************************
	Changelog
	2012-07-02	DTZ		first version of the adc with basic working functions
	2012-07-04	DTZ		channels 0-3 completed in ND_ADC_InitDefault()
*******************************************************************************/
