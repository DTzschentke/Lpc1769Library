/*
 * mmgen.h
 *
 *  Created on: 14.06.2012
 *      Author: ssc
 */


#ifndef MMGEN_H_
#define MMGEN_H_


#include "stdint.h"


/******************************************************************************/
/*	global defines															  */
/******************************************************************************/

#define NUM_VOICES 8   // maximale Zahl der Voices (Array-Größe) (= NUMBER_OF_KEYS ?)


/******************************************************************************/
/*	public functions														  */
/******************************************************************************/

void MMGEN_Init();
void MMGEN_StartEnvs(uint8_t v, uint8_t key, uint8_t vel);   // wird beim Drücken einer Taste aufgerufen
void MMGEN_ReleaseEnvs(uint8_t v, uint8_t vel);              // wird beim Loslassen einer Taste aufgerufen
void MMGEN_Process();                            // called with every timer interval (100 us)

int  MMGEN_ApplyButtons(uint32_t buttons);
void MMGEN_SetNoDamper(int state);


#endif /* MMGEN_H_ */
