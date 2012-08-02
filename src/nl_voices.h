/*
 * vel_v_alloc.h
 *
 *  Created on: 03.07.2012
 *      Author: ssc
 */

#ifndef VOICES_H_
#define VOICES_H_

#include "stdint.h"


/******************************************************************************/
/*	public functions														  */
/******************************************************************************/
void VOICES_Init();
void VOICES_KeyDown(uint32_t diff, uint8_t key);
void VOICES_KeyUp(uint32_t diff, uint8_t key);



#endif /* VEL_V_ALLOC_H_ */
