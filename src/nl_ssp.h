/******************************************************************************/
/** @file		nl_ssp.h
	@date		2012-07-14
	@version	0.03
  	@author		Daniel Tzschentke [2012-07-16]
  	@brief		Synchronous Serial Port (SSP) can be configured as
  					- Motorola SPI
  					- 4-wire TI SSI
  					- National Semiconductors Microwire
*******************************************************************************/
#ifndef NL_SSP_H_
#define NL_SSP_H_


/* public defines */

/* ssp modes */
/** @todo use more precise descriptions  */
#define SPI		0b00000000
#define SSI		0b00010000
#define MWB		0b00100000

#define PORT_0	0
#define PORT_1	1

/** @todo use more precise descriptions, like 'PORT_POWER_ENABLE' */
#define ENABLE	1
#define DISABLE 0

/* parameters for clock settings */
/** @todo use more precise descriptions  */
/** @todo use common terms for clock polarity*/
#define DONT_CARE		0b00000000
#define NORMALLY_LOW	0b00000000
#define NORMALLY_HIGH	0b01000000
#define FIRST_EDGE		0b00000000
#define SECOND_EDGE		0b10000000

/* clock divider */
#define CCLK_1		0b00000001
#define CCLK_2		0b00000010
#define CCLK_4		0b00000000
#define CCLK_8		0b00000011

/* public funchtions */
void SSP_PortPower(uint8_t sspPort, uint8_t power);
void SSP_BusType(uint8_t sspPort, uint8_t busType);
void SSP_FrameSize(uint8_t sspPort, uint8_t frameSize);
void SSP_ClockSettings(uint8_t sspPort,
						 	uint8_t clockDivider,
						 	uint8_t clockPrescale,
						 	uint8_t clockPolarity,
						 	uint8_t clockPhase,
						 	uint8_t serialClockRate);
void SSP_InitPins(void);
void SSP_ControlSettings(uint8_t sspPort,
							uint8_t loopbackMode,
							uint8_t sspController,
							uint8_t masterMode,
							uint8_t slaveBlocksMiso);
void SSP_WriteDataToFifo(uint8_t sspPort, uint16_t data);
int32_t SSP_ReadDataFromFifo(uint8_t sspPort);
int32_t SSP_CheckFifoBuffers(uint8_t sspPort);
int32_t SSP_Dma(uint8_t sspPort, uint8_t receiveDma, uint8_t transmitDma);



#endif /* NL_SSP_H_ */

/*******************************************************************************
	Changelog
	2012-07-16	DTZ		first version
	2012-07-17	DTZ		function prototypes added
	2012-07-24	DTZ		renamed from ND_SSP.h to nl_ssp.h
*******************************************************************************/
