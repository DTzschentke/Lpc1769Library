/*****************************************************************************/
/** @file		nl_gpio.h
	@date		2012-07-24
	@version 	0.08
	@brief		This module contains everything about general purpose inputs
				and outputs:
					- write mask
					- Pin direction (input/output)
					- Set and clear pin (high/low)
					- Read input pin (high/low)
					- use internal pull resistors (up/down/none/depending)
					- pin special functions (ADC, UART, SPI, ...)
				And also bit manipulations macros:
					- Bit set, clear, toggle
					- Bit read
				Use the init function to reset every pin value
				uses Pxxx instead of Px.xx
	@author		Daniel Tzschentke [2012-04-23]
	@todo		add PinTgl
	@todo 		add PinVal
	@todo		PinXXX_(__VA_ARGS__) in .c not in the interface
	@todo		_delay_clk in delay file
******************************************************************************/
#ifndef NL_GPIO_H_
#define NL_GPIO_H_

#include "stdint.h"		// uint8_t, uint16_t, ...
#include "nl_lpc17xx.h"



/*******************************************************************************
	public defines/functions
*******************************************************************************/
/* parameter for the pin mode function */
#define GPIO_PULL_UP	0b00000000
#define GPIO_PULL_DOWN	0b00000011
#define GPIO_FLOATING	0b00000010
#define GPIO_REPEATER	0b00000001

/* parameter for the pin function function */
#define GPIO_SF0	0
#define GPIO_SF1	1
#define GPIO_SF2	2
#define GPIO_SF3	3

/* prototypes */
void GPIO_Init(void);
void GPIO_PinMod(uint8_t port, uint8_t pin, uint32_t mode);
void GPIO_PinFnc(uint8_t port, uint8_t pin, uint8_t function);



/*******************************************************************************
	pin names			special functions (from PINSEL REGISTER)
*******************************************************************************/
/*							SF0		SF1			SF2			SF3				*/
#define	P000 0, 0		/*	P0[0]	RD1			TXD3		SDA1			*/
#define	P001 0, 1		/*	P0[1]	TD1			RXD3		SCL1			*/
#define	P002 0, 2		/*	P0[2]	TXD0		AD0[7]		-				*/
#define	P003 0, 3		/*	P0[3]	RXD0		AD0[6]						*/
#define	P004 0, 4		/*	P0[4]	I2SRX_CLK	RD2	CAP2[0]					*/
#define	P005 0, 5		/*	P0[5]	I2SRX_WS	TD2	CAP2[1]					*/
#define	P006 0, 6		/*	P0[6]	I2SRX_SDA	SSEL1		MAT2[0]			*/
#define	P007 0, 7		/*	P0[7]	I2STX_CLK	SCK1		MAT2[1]			*/
#define	P008 0, 8		/*	P0[8]	I2STX_WS	MISO1		MAT2[2]			*/
#define	P009 0, 9		/*	P0[9]	I2STX_SDA	MOSI1		MAT2[3]			*/
#define	P010 0, 10		/*	P0[10]	TXD2		SDA2		MAT3[0]			*/
#define	P011 0, 11		/*	P0[11]	RXD2		SCL2		MAT3[1]			*/

#define	P015 0, 15		/*	P0[15]	TXD1		SCK0		SCK				*/
#define	P016 0, 16		/*	P0[16]	RXD1		SSEL0		SSEL			*/
#define	P017 0, 17		/*	P0[17]	CTS1		MISO0		MISO			*/
#define	P018 0, 18		/*	P0[18]	DCD1		MOSI0		MOSI			*/
#define	P019 0, 19		/*	P0[19]	DSR1		SDA1						*/
#define	P020 0, 20		/*	P0[20]	DTR1		SCL1						*/
#define	P021 0, 21		/*	P0[21]	RI1			RD1							*/
#define	P022 0, 22		/*	P0[22]	RTS1		TD1							*/
#define	P023 0, 23		/*	P0[23]	AD0[0]		I2SRX_CLK	CAP3[0]			*/
#define	P024 0, 24		/*	P0[24]	AD0[1]		I2SRX_WS	CAP3[1]			*/
#define	P025 0, 25		/*	P0[25]	AD0[2]		I2SRX_SDA	TXD3			*/
#define	P026 0, 26		/*	P0[26]	AD0[3]		AOUT/RXD3					*/
#define	P027 0, 27		/*	P0[27]	SDA0		USB_SDA						*/
#define	P028 0, 28		/*	P0[28]	SCL0		USB_SCL						*/
#define	P029 0, 29		/*	P0[29]	USB_D+									*/
#define	P030 0, 30		/*	P0[30]	USB_D-									*/

#define	P100 1, 0		/*	P1[0]	ENET_TXD0								*/
#define	P101 1, 1		/*	P1[1]	ENET_TXD1								*/
#define	P104 1, 4		/*	P1[4]	ENET_TX_EN								*/
#define	P108 1, 8		/*	P1[8]	ENET_CRS								*/
#define	P109 1, 9		/*	P1[9]	ENET_RXD0								*/
#define	P110 1, 10		/*	P1[10]	ENET_RXD1								*/
#define	P114 1, 14		/*	P1[14]	ENET_RX_ER								*/
#define	P115 1, 15		/*	P1[15]	ENET_REF_CLK							*/
#define	P116 1, 16		/*	P1[16]	ENET_MDC								*/
#define	P117 1, 17		/*	P1[17]	ENET_MDIO								*/
#define	P118 1, 18		/*	P1[18]	USB_UP_LED	PWM1[1]		CAP1[0]			*/
#define	P119 1, 19		/*	P1[19]	MCOA0		USB_PPWR	CAP1[1]			*/
#define	P120 1, 20		/*	P1[20]	MCI0		PWM1[2]		SCK0			*/
#define	P121 1, 21		/*	P1[21]	MCABORT		PWM1[3]		SSEL0			*/
#define	P122 1, 22		/*	P1[22]	MCOB0		USB_PWRD	MAT1[0]			*/
#define	P123 1, 23		/*	P1[23]	MCI1		PWM1[4]		MISO0			*/
#define	P124 1, 24		/*	P1[24]	MCI2		PWM1[5]		MOSI0			*/
#define	P125 1, 25		/*	P1[25]	MCOA1		MAT1[1]						*/
#define	P126 1, 26		/*	P1[26]	MCOB1		PWM1[6]		CAP0[0]			*/
#define	P127 1, 27		/*	P1[27]	CLKOUT		USB_OVRCR	CAP0[1]			*/
#define	P128 1, 28		/*	P1[28]	MCOA2		PCAP1[0]	MAT0[0]			*/
#define	P129 1, 29		/*	P1[29]	MCOB2		PCAP1[1]	MAT0[1]			*/
#define	P130 1, 30		/*	P1[30]	VBUS		AD0[4]						*/
#define	P131 1, 31		/*	P1[31]	SCK1		AD0[5]						*/

#define	P200 2, 0		/*	P2[0]	PWM1[1]		TXD1						*/
#define	P201 2, 1		/*	P2[1]	PWM1[2]		RXD1						*/
#define	P202 2, 2		/*	P2[2]	PWM1[3]		CTS1		TRACEDATA[3]	*/
#define	P203 2, 3		/*	P2[3]	PWM1[4]		DCD1		TRACEDATA[2]	*/
#define	P204 2, 4		/*	P2[4]	PWM1[5]		DSR1		TRACEDATA[1]	*/
#define	P205 2, 5		/*	P2[5]	PWM1[6]		DTR1		TRACEDATA[0]  	*/
#define	P206 2, 6		/*	P2[6]	PCAP1[0]	RI1			TRACECLK		*/
#define	P207 2, 7		/*	P2[7]	RD2			RTS1						*/
#define	P208 2, 8		/*	P2[8]	TD2			TXD2						*/
#define	P209 2, 9		/*	P2[9]	USB_		CONNECT		RXD2			*/
#define	P210 2, 10		/*	P2[10]	EINT0		NMI							*/
#define	P211 2, 11		/*	P2[11]	EINT1		I2STX_CLK					*/
#define	P212 2, 12		/*	P2[12]	EINT2		I2STX_WS					*/
#define	P213 2, 13		/*	P2[13]	EINT3		I2STX_SDA					*/

#define	P325 3, 25		/*	P3[25]	MAT0[0]		PWM1[2]						*/
#define	P326 3, 26		/*	P3[26]	STCLK		MAT0[1]		PWM1[3]			*/

#define	P428 4, 28		/*	P4[28]	RX_MCLK		MAT2[0]		TXD3			*/
#define	P429 4, 29		/*	P4[29]	TX_MCLK		MAT2[1]		RXD3			*/



/*******************************************************************************
	public pin and bit macros
*******************************************************************************/
/** @brief	Mask to make Pins available for read and write access */
#define GPIO_PinMkA(...)				GPIO_PinMkA_(__VA_ARGS__)
#define GPIO_PinMkA_(reg, bit)		((FIO ## reg ## MASK) &= ( ~(1 << (bit))))

/** @brief	Mask to disable write access */
#define GPIO_PinMkD(...)				GPIO_PinMkD_(__VA_ARGS__)
#define GPIO_PinMkD_(reg, bit)		((FIO ## reg ## MASK) |= (1 << (bit)))

/**	@brief	Configure GPIO Pin as Input -> 0 */
#define GPIO_PinIpt(...)				GPIO_PinIpt_(__VA_ARGS__)
#define GPIO_PinIpt_(reg, bit)		((FIO ## reg ## DIR) &= (~(1 << (bit))))

/** @brief	Configure GPIO Pin as Output -> 1 */
#define GPIO_PinOpt(...)				GPIO_PinOpt_(__VA_ARGS__)
#define GPIO_PinOpt_(reg, bit)		((FIO ## reg ## DIR) |= (1 << (bit)))

/** @brief	Set GPIO output HIGH */
#define GPIO_PinSet(...)				GPIO_PinSet_(__VA_ARGS__)
#define GPIO_PinSet_(reg, bit)		((FIO ## reg ## SET) |= (1 << (bit)))

/** @brief	Set GPIO output LOW */
#define GPIO_PinClr(...)				GPIO_PinClr_(__VA_ARGS__)
#define GPIO_PinClr_(reg, bit)		((FIO ## reg ## CLR) |= (1 << (bit)))

/** @brief	Read Input Pin and return value */
#define GPIO_PinGet(...)				GPIO_PinGet_(__VA_ARGS__)
#define GPIO_PinGet_(reg, bit)		(!!((FIO ## reg ## PIN) & (1 << (bit))))

/** @brief	Clear bit without affecting other bits */
#define GPIO_BitClr(reg, bit)		((reg) &= (~(1 << (bit))))

/** @brief	Set bit without affecting other bits */
#define GPIO_BitSet(reg, bit)		((reg) |= (1 << (bit)))

/** @brief	Toggle bit */
#define GPIO_BitTgl(reg, bit)		((reg) ^= (1 << (bit)))

/** @brief	returns the value of bit (0 or 1) */
#define GPIO_BitGet(reg, bit)   	(!!((reg) & (1 << (bit))))

/** @brief	sets a bit combination in a register or a word
 	 	 	reg:	register/word name
 	 	 	mask:	define which bits are affected
 	 	 				- 1: bit will be affected
 	 	 				- 0: bit will not be affected
 	 	 	val:	the value or bit combination which will be written in into
 	 	 			the register */
#define GPIO_BitsVal(reg, mask, val) ((reg) = ( ((val) & (mask)) | ((reg) & (~(mask)))))

/** @brief	sets the bit in reg to val
			reg:	register/word name
 	 	 	bit:	bit in the register/word to set or clear
 	 	 	val:	1 or 0 */
#define GPIO_BitVal(reg, bit, val)   ((reg) = (((reg) & (~(1 << (bit)))) | ((1 & (val)) << (bit))))




/* delay one clock cycle */
#define _delay_clk      		asm volatile("nop")


#endif /* NL_GPIO_H_ */


/*******************************************************************************
	Changelog
	2012-07-06	DTZ		BitGet tested -> working
	2012-07-16	DTZ		BitsVal function added and tested
	2012-07-17	DTZ		BitVal function added and tested
						variable 'times' changed to 'bit' during unification
						function defines added
	2012-07-23	DTZ		merged ND_PinNames.h to GPIO.h
	2012-07-24	DTZ		renameing
*******************************************************************************/
