/*
 * message.h
 *
 *  Created on: 15.06.2012
 *      Author: ssc
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_


#include "stdint.h"


/******************************************************************************/
/*	public functions														  */
/******************************************************************************/

void MESSAGE_SelectVoice0(int v);
void MESSAGE_SelectParameter0(int p);
void MESSAGE_SetTime(int t);
void MESSAGE_SetCurve(uint8_t type, int curvature);
void MESSAGE_SetDestination(int d);

void MESSAGE_SendMidiCmd();


#endif /* MESSAGE_H_ */
