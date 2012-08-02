/******************************************************************************/
/** @file		ND_ssp.c
	@date		2012-07-25
	@version	0.05
  	@author		Daniel Tzschentke [2012-07-16]
  	@brief		Synchronous Serial Port (SSP) can be configured as
  					- Motorola SPI
  					- Texas Instruments 4-wire SSI
  					- National Semiconductors Microwire bus
  				The LPC17xx has two SSP ports:
  					- SSP0
  					- SSP1
	@todo		add interrupt functions
	@todo		replace int8_t with boolean or enum if enable/disable is all
				thas chooseable
	@todo		improvement of the debug messages: read the values directly
				from the processors bits instead of the functions parameter.
				This could be also nice for a terminal readout
*******************************************************************************/
#include "system_LPC17xx.h"		// Core Clock
#include "stdint.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "nl_lpc17xx.h"
#include "nl_gpio.h"
#include "nl_ssp.h"
#include "nl_debug.h"


/* buffer for itoa */
char buf[10];


/******************************************************************************/
/**	@brief		function to turn on or off the ssp general port power
  				ssp can only work if port power is on
	@param		sspPort
					- PORT_0
					- PORT_1
	@param		power
					- ENABLE
					- DISABLE
*******************************************************************************/
void SSP_PortPower(uint8_t sspPort, uint8_t power)
{
	if (sspPort == PORT_0)
	{
		GPIO_BitVal(PCONP, PCSSP0, power);
		#if defined(HIGHDEBUG)
		if(power == ENABLE)
			DEBUG_SendMsg("ND_SSP_PortPower:      PORT 0 power enabled\n");
		else if(power == DISABLE)
			DEBUG_SendMsg("ND_SSP_PortPower:      PORT 0 power disabled\n");
		else
			DEBUG_SendMsg("ND_SSP_PortPower:      PORT 0 power unsure\n");
		#endif
	}
	else if (sspPort == PORT_1)
	{
		GPIO_BitVal(PCONP, PCSSP1, power);
		#if defined(HIGHDEBUG)
		if(power == ENABLE)
			DEBUG_SendMsg("ND_SSP_PortPower:      PORT 1 power enabled\n");
		else if(power == DISABLE)
			DEBUG_SendMsg("ND_SSP_PortPower:      PORT 1 power disabled\n");
		else
			DEBUG_SendMsg("ND_SSP_PortPower:      PORT 1 power unsure\n");
		#endif
	}
	else
	{
		#if defined(HIGHDEBUG)
		DEBUG_SendMsg("ND_SSP_PortPower:      Function called but no Port was selected.\n");
		#endif
	}

}



/******************************************************************************/
/**	@brief		Function to select the desired bus type.
	@param		sspPort
					- PORT_0
					- PORT_1
	@param		busType
					- SPI (Serial Peripheral Interface)
					- SSI (Synchronous Serial Interface)
					- MWB (Microwire Bus)
*******************************************************************************/
void SSP_BusType(uint8_t sspPort, uint8_t busType)
{
	if (sspPort == PORT_0)
	{
		GPIO_BitsVal(SSP0CR0, 0b00110000, busType);
		#if defined(HIGHDEBUG)
			DEBUG_SendMsg("ND_SSP_BusType:        PORT 0, Bus: ");
		#endif
	}
	else if (sspPort == PORT_1)
	{
		GPIO_BitsVal(SSP1CR0, 0b00110000, busType);
		#if defined(HIGHDEBUG)
			DEBUG_SendMsg("ND_SSP_BusType:        PORT 1, Bus: ");
		#endif
	}
	else
	{
		#if defined(HIGHDEBUG)
			DEBUG_SendMsg("ND_SSP_BusType:        PORT not valide");
		#endif
	}

	#if defined(HIGHDEBUG)
	if(busType == SPI)
		DEBUG_SendMsg("SPI\n");
	else if (busType == SSI)
		DEBUG_SendMsg("SSI\n");
	else if (busType == MWB)
		DEBUG_SendMsg("MWB\n");
	else
		DEBUG_SendMsg("Bus type not valide\n");
	#endif
}



/******************************************************************************/
/**	@brief		choose the data bits per frame
	@param		sspPort
					- PORT_0
					- PORT_1
	@param		frameSize in bits: values can be from 4 ..  16
*******************************************************************************/
void SSP_FrameSize(uint8_t sspPort, uint8_t frameSize)
{
	#if defined(HIGHDEBUG)
	if ( (frameSize < 4) || (frameSize > 16) )
	{
		DEBUG_SendMsg("ND_SSP_FrameSize:      No valid frame size\n");
	}
	#endif

	frameSize--;	// to fit to bit coding from datasheet

	if (sspPort == PORT_0)
	{
		GPIO_BitsVal(SSP0CR0, 0b1111, frameSize);
		#if defined(HIGHDEBUG)
		DEBUG_SendMsg("ND_SSP_FrameSize:      PORT 0, FrameSize: ");
		itoa((frameSize+1), buf, 10);
		DEBUG_SendMsg(buf);
		DEBUG_SendMsg("\n");
		#endif
	}
	else if (sspPort == PORT_1)
	{
		GPIO_BitsVal(SSP1CR0, 0b1111, frameSize);
		#if defined(HIGHDEBUG)
		DEBUG_SendMsg("ND_SSP_FrameSize:      PORT 1, FrameSize: ");
		itoa((frameSize+1), buf, 10);
		DEBUG_SendMsg(buf);
		DEBUG_SendMsg("\n");
		#endif
	}
	else
	{
		#if defined(HIGHDEBUG)
		DEBUG_SendMsg("ND_SSP_FrameSize:      PORT not available\n");
		#endif
	}
}



/******************************************************************************/
/**	@brief	clock rate, polarity and phase -> only used in spi mode
	@param	sspPort
				- PORT_0
				- PORT_1
	@param	clockDivider 	(divides the CPU clock CCLK through 1, 2, 4 or 8
							as source for the peripheral ssp clock)
				- CCLK_1
				- CCLK_2
				- CCLK_4
				- CCLK_8
	@param	clockPrescale
				- 2..254		ONLY even values!
								(HINT: consult datasheet for correct values)
	@param	clockPolarity
				- NORMALLY_LOW	(clock between frames is low)
				- NORMALLY_HIGH	(clock between frames is high)
				- DONT_CARE 	(if SPI is not used)
	@param	clockPhase
				- FIRST_EDGE	(bit will be send/received on first edge of
								 the clock)
				- SECOND_EDGE	(bit will be send/received on second edge of
								 the clock)
				- DONT_CARE 	(if SPI is not used)
	@todo 	change BitsVal to BitVal if possible
*******************************************************************************/
void SSP_ClockSettings(uint8_t sspPort,
						uint8_t clockDivider,
						uint8_t clockPrescale,
						uint8_t clockPolarity,
						uint8_t clockPhase,
						uint8_t serialClockRate)
{
	/* settings for ssp port 0 or 1 */
	if (sspPort == PORT_0)
	{
		GPIO_BitsVal(SSP0CR0, 0b01000000, clockPolarity);
		GPIO_BitsVal(SSP0CR0, 0b10000000, clockPhase);
		GPIO_BitsVal(PCLKSEL1, 0xC00, (clockDivider << 10));
		GPIO_BitsVal(SSP0CPSR, 0xFF, clockPrescale);
		#if defined HIGHDEBUG
			DEBUG_SendMsg("ND_SSP_ClockSettings:  PORT 0\n");
		#endif
	}
	else if (sspPort == PORT_1)
	{
		GPIO_BitsVal(SSP1CR0, 0b01000000, clockPolarity);
		GPIO_BitsVal(SSP1CR0, 0b10000000, clockPhase);
		GPIO_BitsVal(PCLKSEL0, 0x300000, (clockDivider << 20));
		GPIO_BitsVal(SSP1CPSR, 0xFF, clockPrescale);
		#if defined HIGHDEBUG
			DEBUG_SendMsg("ND_SSP_ClockSettings:  PORT 1\n");
		#endif
	}
	else
	{
		#if defined HIGHDEBUG
			DEBUG_SendMsg("ND_SSP_ClockSettings:  PORT not available\n");
		#endif
	}



	#if defined(HIGHDEBUG)
	/* check if clock divider value is in range */
	uint8_t tempClockDivider = 0;
	if (clockDivider == CCLK_1)
	{
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockDivider:  CCLK/1\n");
		tempClockDivider = 1;
	}
	else if (clockDivider == CCLK_2)
	{
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockDivider:  CCLK/2\n");
		tempClockDivider = 2;
	}
	else if (clockDivider == CCLK_4)
	{
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockDivider:  CCLK/4\n");
		tempClockDivider = 4;
	}
	else if (clockDivider == CCLK_8)
	{
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockDivider:  CCLK/8\n");
		tempClockDivider = 8;
	}
	else
	{
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockDivider:  out of range\n");
	}
	/* check if clock prescale value is in range */
	if ((clockPrescale < 2) || (clockPrescale > 254))
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockPrescale: out of range\n");
	else
	{
		if ((clockPrescale % 2) == 1)
			DEBUG_SendMsg("ND_SSP_ClockSettings:  clockPrescale: must be an even number\n");
		else
		{
			DEBUG_SendMsg("ND_SSP_ClockSettings:  clockPrescale: ");
			itoa((clockPrescale), buf, 10);
			DEBUG_SendMsg(buf);
			DEBUG_SendMsg("\n");
		}
	}

	/* check if clock phase variable is in range */
	if (clockPhase == FIRST_EDGE)
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockPhase:    FIRST_EDGE\n");
	else if (clockPhase == SECOND_EDGE)
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockPhase:    SECONDE_EDGE\n");
	else
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockPhase:    not valid\n");


	/* check if clock polarity variable is in range */
	if(clockPolarity == NORMALLY_LOW)
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockPolarity: NORMALLY_LOW\n");
	else if(clockPolarity == NORMALLY_HIGH)
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockPolarity: NORMALLY_HIGH\n");
	else
		DEBUG_SendMsg("ND_SSP_ClockSettings:  clockPolarity: not valid\n");

	//DEBUG_SendMsg("NOTE: Clock polarity and phase are only important in SPI mode.\n");

		DEBUG_SendMsg("ND_SSP_ClockSettings:  resulting bus clock: ");
		itoa((SystemCoreClock/tempClockDivider/clockPrescale/1000), buf, 10);
		DEBUG_SendMsg(buf);
		DEBUG_SendMsg(" kHz\n");
	#endif
}



/******************************************************************************/
/**	@brief		test pins to test the ssp (spi) bus
	@todo		add all possible pins
				do the pins have to be config. as input/output/pull-resistor
*******************************************************************************/
void SSP_InitPins(void)
{
	/* special function: SSP */
	GPIO_PinFnc(P006, GPIO_SF2);
	GPIO_PinFnc(P007, GPIO_SF2);
	GPIO_PinFnc(P008, GPIO_SF2);
	GPIO_PinFnc(P009, GPIO_SF2);
}



/******************************************************************************/
/**	@brief		general transmission control settings
	@param		sspPort
					- PORT_0
					- PORT_1
	@param		LoopbackMode
					- ENABLE
					- DISABLE
	@param		SspController
					- ENABLE
					- DISABLE
	@param		MasterMode
					- ENABLE 	(Master)
					- DISABLE	(SLAVE)
	@param		SlaveBlocksMiso
					- ENABLE
					- DISABLE
	@todo 		change parameter datatypes to boolean
*******************************************************************************/
void SSP_ControlSettings(uint8_t sspPort,
							uint8_t loopbackMode,
							uint8_t sspController,
							uint8_t masterMode,
							uint8_t slaveBlocksMiso)
{
	if (sspPort == PORT_0)
	{
		GPIO_BitVal(SSP0CR1, 0, loopbackMode);
		GPIO_BitVal(SSP0CR1, 1, sspController);
		GPIO_BitVal(SSP0CR1, 2, (!(masterMode)));
		GPIO_BitVal(SSP0CR1, 3, slaveBlocksMiso);
		#if defined(HIGHDEBUG)
		DEBUG_SendMsg("ND_SSP_ControlSettings: PORT 0\n");
		#endif
	}
	else if (sspPort == PORT_1)
	{
		GPIO_BitVal(SSP1CR1, 0, loopbackMode);
		GPIO_BitVal(SSP1CR1, 1, sspController);
		GPIO_BitVal(SSP1CR1, 2, (!(masterMode)));
		GPIO_BitVal(SSP1CR1, 3, slaveBlocksMiso);
		#if defined(HIGHDEBUG)
		DEBUG_SendMsg("ND_SSP_ControlSettings: PORT 1\n");
		#endif
	}
	else
	{
		#if defined(HIGHDEBUG)
		DEBUG_SendMsg("ND_SSP_ControlSettings: PORT not available\n");
		#endif
	}

	#if defined(HIGHDEBUG)
	/* check loopback directly from bits */
	if (BitGet(SSP0CR1, 0) == ENABLE)
		DEBUG_SendMsg("ND_SSP_ControlSettings: loopbackMode:    ENABLE\n");
	else if (BitGet(SSP0CR1, 0) == DISABLE)
		DEBUG_SendMsg("ND_SSP_ControlSettings: loopbackMode:    DISABLE\n");
	else
		DEBUG_SendMsg("ND_SSP_ControlSettings: loopbackMode:    Not valide\n");

	/* check sspController directly from bits */
	if (BitGet(SSP0CR1, 1) == ENABLE)
		DEBUG_SendMsg("ND_SSP_ControlSettings: sspController:   ENABLE\n");
	else if (BitGet(SSP0CR1, 1) == DISABLE)
		DEBUG_SendMsg("ND_SSP_ControlSettings: sspController:   DISABLE\n");
	else
		DEBUG_SendMsg("ND_SSP_ControlSettings: sspController:   Not valide\n");

	/* check masterMode directly from bits */
	if (BitGet(SSP0CR1, 2) == ENABLE)
		DEBUG_SendMsg("ND_SSP_ControlSettings: masterMode:      Master\n");
	else if (BitGet(SSP0CR1, 2) == DISABLE)
		DEBUG_SendMsg("ND_SSP_ControlSettings: masterMode:      Slave\n");
	else
		DEBUG_SendMsg("ND_SSP_ControlSettings: masterMode:      Not valide\n");

	if (BitGet(SSP0CR1, 3) == ENABLE)
		DEBUG_SendMsg("ND_SSP_ControlSettings: SlaveBlocksMiso: ENABLE\n");
	else if (BitGet(SSP0CR1, 3) == DISABLE)
		DEBUG_SendMsg("ND_SSP_ControlSettings: SlaveBlocksMiso: DISABLE\n");
	else
		DEBUG_SendMsg("ND_SSP_ControlSettings: SlaveBlocksMiso: Not valide\n");
	#endif

}



/******************************************************************************/
/**	@brief		writes data to the fifo output buffer - data will be send
				automaticly
	@param		sspPort
					- PORT_0
					- PORT_1
	@param		data (16 bit value which should be send)
	@return		status
					-  0:	everything all right
					- -1:	ssp port not available
					- -2:	fifo buffer is full
	@todo		delete returns and replace with debugging informations
*******************************************************************************/
void SSP_WriteDataToFifo(uint8_t sspPort, uint16_t data)
{
	if (sspPort == PORT_0)
	{
		if ((GPIO_BitGet(SSP0SR, 1)) == 1)		// TNF == 1?
		{
			GPIO_BitsVal(SSP0DR, 0xFFFF, data);
		}
		else
		{
			#if defined(HIGHDEBUG)
			DEBUG_SendMsg("ND_SSP_WriteDataToFifo: FIFO output buffer is full.\n");
			#endif
		}
	}
	else if (sspPort == PORT_1)
	{
		if ((GPIO_BitGet(SSP1SR, 1)) == 1)		// TNF == 1?
		{
			GPIO_BitsVal(SSP1DR, 0xFFFF, data);
		}
		else
		{
			#if defined(HIGHDEBUG)
			DEBUG_SendMsg("ND_SSP_WriteDataToFifo: FIFO output buffer is full.\n");
			#endif
		}
	}
	else
	{
		#if defined(HIGHDEBUG)
		DEBUG_SendMsg("ND_SSP_ControlSettings: PORT not available\n");
		#endif
	}
}



/******************************************************************************/
/**	@brief		reads data from the fifo input buffer and returns the data
	@param		sspPort
					- PORT_0
					- PORT_1
	@return		status
					-  0..2^16	value from buffer, everthing all right
					- -1:		ssp port not available
					- -2:		fifo buffer is empty -> nothing to read
*******************************************************************************/
int32_t SSP_ReadDataFromFifo(uint8_t sspPort)
{
	if (sspPort == PORT_0)
	{
		if(!GPIO_BitGet(SSP0SR, 2))
		{
			return -2;
		}
		return (SSP0DR & 0xFFFF);
	}
	else if (sspPort == PORT_1)
	{
		if(!GPIO_BitGet(SSP1SR, 2))
		{
			return -2;
		}
		return (SSP1DR & 0xFFFF);
	}
	else
	{
		return -1;
	}
}



/******************************************************************************/
/**	@brief		checks if the buffer are empty, filled or full
	@param		sspPort
				- PORT_0
				- PORT_1
	@return		status
				- -1:	ssp port not available
				- 0b uuu4.3210: binary combination of the status bits
					-bit u: unused
					-bit 4: BSY - Busy	0:idle	1:currently sending/receiving
												  and/or TxFIFO is not empty
					-bit 3: RFF - Receive FIFO Full			1:full	0:not full
					-bit 2: RNE - Receive FIFO Not Empty 	0:empty	1:not empty
					-bit 1: TNF - Transmit FIFO Not Full 	0:full	1:not full
					-bit 0: TFE - Transmit FIFO Empty  		1:empty 0:not empty
					(see datasheet for details)
*******************************************************************************/
int32_t SSP_CheckFifoBuffers(uint8_t sspPort)
{
	if (sspPort == PORT_0)
	{
		return ((uint8_t) SSP0SR) & 0b00011111;
	}
	else if (sspPort == PORT_1)
	{
		return ((uint8_t) SSP1SR) & 0b00011111;
	}
	else
	{
		return -1;
	}
}



/******************************************************************************/
/**	@brief
	@details
	@param		sspPort
					- PORT_0
					- PORT_1
	@param		receiveDma
					- ENABLE
					- DISABLE
	@param		transmitDma
					- ENABLE
					- DISABLE
	@return		status
					-  0:	everything all right
					- -1:	ssp port not available
*******************************************************************************/
int32_t SSP_Dma(uint8_t sspPort, uint8_t receiveDma, uint8_t transmitDma)
{
	if (sspPort == PORT_0)
	{
		GPIO_BitVal(SSP0DMACR, 0, receiveDma);
		GPIO_BitVal(SSP0DMACR, 1, transmitDma);
		return 0;
	}
	else if (sspPort == PORT_1)
	{
		GPIO_BitVal(SSP1DMACR, 0, receiveDma);
		GPIO_BitVal(SSP1DMACR, 1, transmitDma);
		return 0;
	}
	else
	{
		return -1;
	}
}



/*******************************************************************************
	Changelog
	2012-07-16	DTZ		first version
	2012-07-17	DTZ		port enable and disable functions added
						ND_SSP_ControlSettings added
						ND_SSP_ClockSettings extended
						ND_SSP_WriteDataToFifo added
	2012-07-18	DTZ		ND_SSP_CheckFifoBuffers added
						ND_SSP_Dma added
	2012-07-19	DTZ		replaced return functions with debug messages
	2012-07-25	0.05 DTZ		function names matching to new name convention
*******************************************************************************/
