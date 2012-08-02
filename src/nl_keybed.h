/******************************************************************************/
/**	@file		ND_KEYBED.h
	@brief    	Scans Keys of the Fatar Keyboard TP8S 61 Keys
    @details  	Modul discription:
				This module uses the mios32 library -> www.ucapps.de if
				MIOS32-switch is enabled.
				If not, it uses per default the empasis-board configuration.
				
				It could be used without any special board,	but the programmer 
				has to take care about some things:
					1. The pin direction and pin number have to be adapted in
						- KEYBED_Init
						- GetKeys
						- SetLine
					2. A function that can handle midi communication has to be 
					   provided in:
						- ND_KEYBOARD_DetectNotes [Midi Note On/Off]
				As output pins 4+1 pins are needed and 8 input pins
	@date		2012-07-26
	@version	0.02
  	@author		Daniel Tzschentke [2012-05-29]
*******************************************************************************/


#ifndef	ND_KEYBED_H_
#define ND_KEYBED_H_


/******************************************************************************/
/*	global defines															  */
/******************************************************************************/

#define NUM_KEYS 64


/******************************************************************************/
/*	public functions														  */
/******************************************************************************/

void KEYBED_Init(void);
void KEYBED_Process(void);


#endif /* ND_KEYBOARD_H_ */


/*******************************************************************************
	Changelog
	2012-05-29	0.01	DTZ		first version
	2012-07-26	0.02	DTZ		renaming during new name conventions
*******************************************************************************/
