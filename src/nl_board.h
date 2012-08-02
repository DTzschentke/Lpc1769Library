/******************************************************************************/
/** @file		nl_board.h
	@date		2012-07-25
	@version	0.02
  	@author		Daniel Tzschentke [2012-07-24]
	@brief		
*******************************************************************************/
#ifndef NL_BOARD_H_
#define NL_BOARD_H_

#include "nl_config.h"


/* public functions */
void BOARD_Init(void);


/* board specific definitions */
#ifdef BOARD_EMPHASE_V01_REV_A

	/* debug pins */
	#define DEBUG_LED_ERROR					P019
	#define DEBUG_LED_TASK					P020
	#define DEBUG_BUTTON_TASK_ERROR_RESET	P209

	/* keybed pins */
	#define KEYBED_LINE_A		P120
	#define KEYBED_LINE_B		P121
	#define KEYBED_LINE_C		P122
	#define KEYBED_LINE_D		P123
	#define KEYBED_LINE_EN_DIS	P118
	#define KEYBED_KEY_0		P200
	#define KEYBED_KEY_1		P201
	#define KEYBED_KEY_2		P202
	#define KEYBED_KEY_3		P203
	#define KEYBED_KEY_4		P204
	#define KEYBED_KEY_5		P205
	#define KEYBED_KEY_6		P206
	#define KEYBED_KEY_7		P207
#elif BOARD_PROTOTYPE
	/* keybed pins */
	#define KEYBED_LINE_A		P004
	#define KEYBED_LINE_B		P005
	#define KEYBED_LINE_C		P006
	#define KEYBED_LINE_D		P007
	#define KEYBED_LINE_EN_DIS	P008
	#define KEYBED_KEY_0		P200
	#define KEYBED_KEY_1		P201
	#define KEYBED_KEY_2		P202
	#define KEYBED_KEY_3		P203
	#define KEYBED_KEY_4		P204
	#define KEYBED_KEY_5		P205
	#define KEYBED_KEY_6		P206
	#define KEYBED_KEY_7		P207
#endif



#endif /* NL_BOARD_H_ */
/*******************************************************************************
	Changelog
	2012-07-24	DTZ		first version
	2012-07-25	DTZ		prototype added
*******************************************************************************/
