/******************************************************************************/
/** @file		nl_message.c
	@date		2012-07-26
	@version	0.02
  	@author		Stephan Schmitt [2012-06-15]
	@brief
*******************************************************************************/
#include "stdint.h"
#include "nl_midi.h"

#include "nl_time.h"
#include "nl_message.h"


/******************************************************************************/
/*	modul local defines														  */
/******************************************************************************/

#define BUFFER_SIZE 32   // room for 32 3-byte messages


/******************************************************************************/
/*	modul local variables													  */
/******************************************************************************/

static int Buffer[BUFFER_SIZE] = {};

static uint8_t BufferReadPointer = 0;
static uint8_t BufferWritePointer = 0;
static uint8_t BufferNotEmpty = 0;

static int LastVoice0 = -1;
static int LastParameter0 = -1;


/*******************************************************************************
*	@brief 	SelectVoice0
*   @param  v: voice Id (12 bits, 0 ... 4095)
*******************************************************************************/

void MESSAGE_SelectVoice0(int v)
{
  if (v != LastVoice0)                                // avoids repeatedly selecting the same voice
  {
    Buffer[BufferWritePointer] = 0x800000             // MIDI status 8 (Note Off), MIDI channel 0, "000"
                               | ((v << 1) & 0x1F00)  // upper 5 bits
                               | (v & 0x7F);          // lower 7 bits

    BufferWritePointer++;

    if (BufferWritePointer == BUFFER_SIZE)
    {
      BufferWritePointer = 0;
    }

    BufferNotEmpty = 1;

    LastVoice0 = v;
  }
}


/*******************************************************************************
	@brief  SelectParameter0
	@param  p: parameter Id (12 bits, 0 ... 4095)
*******************************************************************************/

void MESSAGE_SelectParameter0(int p)
{
  if (p != LastParameter0)                            // avoids repeatedly selecting the same parameter
  {
    Buffer[BufferWritePointer] = 0x804000             // MIDI status 8 (Note Off), MIDI channel 0, "010"
                               | ((p << 1) & 0x1F00)  // upper 5 bits
                               | (p & 0x7F);          // lower 7 bits

    BufferWritePointer++;

    if (BufferWritePointer == BUFFER_SIZE)
    {
      BufferWritePointer = 0;
    }

    BufferNotEmpty = 1;

    LastParameter0 = p;
  }
}


/*******************************************************************************
	@brief  SetTime
	@param  t: linear time (is converted to the compressed format)
******************************************************************************/

void MESSAGE_SetTime(int t)
{
  t = TIME_Compress(t);   /// man könnte auch den Code hier einfügen und das Shiften/Maskieren/Verodern in einem Schritt

  Buffer[BufferWritePointer] = 0x900000             // MIDI status 9 (Note On), MIDI channel 0
	                         | ((t << 1) & 0x7F00)  // upper 7 bits
	                         | (t & 0x7F);          // lower 7 bits

  BufferWritePointer++;

  if (BufferWritePointer == BUFFER_SIZE)
  {
    BufferWritePointer = 0;
  }

  BufferNotEmpty = 1;
}


/*****************************************************************************
*	@brief  SetCurve
*   @param  type: (5 bit, 0 ... 31)
*           curvature: (9 bit, 0 ... 511)
******************************************************************************/

void MESSAGE_SetCurve(uint8_t type, int curvature)
{
  Buffer[BufferWritePointer] = 0xA00000			                                    // MIDI status A (Poly AT), MIDI channel 0
	                         | ((type & 0x1F) << 10) | ((curvature << 1) & 0x0300)  // 5 bits + upper 2 bits
	                         | (curvature & 0x7F);                                  // lower 7 bits

  BufferWritePointer++;

  if (BufferWritePointer == BUFFER_SIZE)
  {
    BufferWritePointer = 0;
  }

  BufferNotEmpty = 1;
}


/*****************************************************************************
*	@brief  SetDestination
*   @param  d: (14 bit, 0 ... 16383)
******************************************************************************/

void MESSAGE_SetDestination(int d)
{
  Buffer[BufferWritePointer] = 0xB00000			    // MIDI status B (Control Change), MIDI channel 0
		                     | ((d << 1) & 0x7F00)  // upper 7 bits
		                     | (d & 0x7F);          // lower 7 bits

  BufferWritePointer++;

  if (BufferWritePointer == BUFFER_SIZE)
  {
    BufferWritePointer = 0;
  }

  BufferNotEmpty = 1;
}


/*****************************************************************************/
/**	@brief  	Sends a 3-byte MIDI message if the buffer is not empty
******************************************************************************/

void MESSAGE_SendMidiCmd()
{
  if (BufferNotEmpty)
  {
	int b = Buffer[BufferReadPointer];

    MIDI_SendByte( (b & 0xFF0000) >> 16 );
    MIDI_SendByte( (b & 0xFF00) >> 8 );
    MIDI_SendByte(  b & 0xFF );

    BufferReadPointer++;

    if (BufferReadPointer == BUFFER_SIZE)
    {
      BufferReadPointer = 0;
    }

    if (BufferReadPointer == BufferWritePointer)
    {
      BufferNotEmpty = 0;
    }
  }
}


/*******************************************************************************
	Changelog
	2012-06-15	SSC		first version
	2012-07-26	DTZ		renaming during new name conventions
*******************************************************************************/

