/******************************************************************************/
/** @file		nl_debug.h
	@date		2012-07-24
	@version	0.01
  	@author		Daniel Tzschentke [2012-07-24]
	@brief		debug message service
*******************************************************************************/
#ifndef NL_DEBUG_H_
#define NL_DEBUG_H_



void DEBUG_Init(void);
void DEBUG_ErrorLed(void);
void DEBUG_TaskOverflowLed(void);
void DEBUG_SendMsg(char *pcString);
void DEBUG_WelcomeMsg(void);



#endif /* NL_DEBUG */
/*******************************************************************************
	Changelog
	2012-07-24	DTZ		first version
*******************************************************************************/
