/******************************************************************************/
/** @file		nl_mmgen.c
	@date		2012-07-26
	@version	0.02
	@brief
  	@author		Stephan Schmitt [2012-06-14]
*******************************************************************************/
#include "stdint.h"

#include "nl_message.h"
#include "nl_mmgen.h"



/******************************************************************************/
/*	modul local variables													  */
/******************************************************************************/

static uint8_t Segment[NUM_VOICES] = {};                      // Index des aktuellen Segments

static uint32_t TimeCount[NUM_VOICES] = {};               // Zähler für die Segment-Zeiten, pro Voice

static uint32_t AttackTime[NUM_VOICES] = {};              // Segment times
static uint32_t Decay1Time[NUM_VOICES] = {};
static uint32_t Decay2Time[NUM_VOICES] = {};
static uint32_t ReleaseTime = 0;

static uint32_t Decay2Level[NUM_VOICES] = {};             // Breakpoint levels
static uint32_t SustainLevel[NUM_VOICES] = {};

static int NoDamper = 0;



/*******************************************************************************
	@brief  InitMMGen
*******************************************************************************/
void MMGEN_Init()
{
  int v;

  for (v = 0; v < NUM_VOICES; v++)
  {
    Decay1Time[v] = 1200;     // Exp Decay1 time  - 120 ms
    Decay2Time[v] = 12000;    // Exp Decay2 time  - 1200 ms
    SustainLevel[v] = 0;      // Decay2/Sustain breakpoint level
    ReleaseTime = 1200;       // Exp Release time  - 120 ms

	Segment[v] = 0;
    TimeCount[v] = 0;
  }
}



/*****************************************************************************
*	@brief  StartEnvs: is called when a key is pressed
*   @param  v:
*           key
*           vel
******************************************************************************/
void MMGEN_StartEnvs(uint8_t v, uint8_t key, uint8_t vel)
{
  Decay2Level[v] = 40 * vel;      // Decay1/Decay2 breakpoint level

  MESSAGE_SelectVoice0(v);

  MESSAGE_SelectParameter0(0);        //---- Pitch

  int p = 128 * (key + 36);   // value  128 * 36 ... 128 * 96
  MESSAGE_SetDestination(p);

  MESSAGE_SelectParameter0(1);        //---- Amplitude

  int t = 128 - vel;          // Attack time   12.7 ... 0.1 ms
  MESSAGE_SetTime(t);
  AttackTime[v] = t;

  int a = 128 * vel;          // Attack peak value   128 * 1 ... 128 * 127
  MESSAGE_SetDestination(a);

  Segment[v] = 0;             // starts Attack phase
  TimeCount[v] = 0;
}



/*****************************************************************************
*	@brief  ReleaseEnvs: is called when a key is released
*   @param  v:
*           vel:
******************************************************************************/
void MMGEN_ReleaseEnvs(uint8_t v, uint8_t vel)
{
  MESSAGE_SelectVoice0(v);

  MESSAGE_SelectParameter0(1);             //---- amplitude

  int t;

  if (NoDamper)
  {
    t = Decay2Time[v];
  }
  else
  {
    t = ReleaseTime + 15 * (64 - vel);    // shorter with higher velocity - 120 +/- 94.5 ms
  }

  MESSAGE_SetTime(t);

  MESSAGE_SetDestination(0);               // goes to silence

  Segment[v] = 3;                  // start Release phase
}



/*****************************************************************************
*	@brief  MovementsGenProcess
******************************************************************************/
void MMGEN_Process()     // called with every timer interval (100 us)
{
  int v;
  int s;

  for (v = 0; v < NUM_VOICES; v++)
  {
    s = Segment[v];

    if (s == 0)      //---- in segment 0 (Attack)
    {
      TimeCount[v]++;

      if (TimeCount[v] > AttackTime[v])  //---- starts segment 1 (Decay1)
      {
        MESSAGE_SelectVoice0(v);

        MESSAGE_SelectParameter0(1);      //---- amplitude
        MESSAGE_SetTime(Decay1Time[v]);
        MESSAGE_SetDestination(Decay2Level[v]);

        Segment[v] = 1;
        TimeCount[v] = 0;
      }
    }
    else if (s == 1)      //---- in segment 1 (Decay1)
    {
      TimeCount[v]++;

      if (TimeCount[v] > Decay1Time[v])  //---- starts segment 2 (Decay2)
      {
        MESSAGE_SelectVoice0(v);

        MESSAGE_SelectParameter0(1);      //---- amplitude
        MESSAGE_SetTime(Decay2Time[v]);
        MESSAGE_SetDestination(SustainLevel[v]);

        Segment[v] = 2;
      }
    }    // Only segment 0 (Attack) and segment 1 (Decay1) have a timed duration. Decay2 and Release are "open end".
  }
}



/*****************************************************************************
*	@brief  ApplyButtons
******************************************************************************/
void MMGEN_SetNoDamper(int state)
{
  int v;

  if (state)
  {
    for (v = 0; v < NUM_VOICES; v++)
    {
   	  if (Segment[v] == 3)
   	  {
        MESSAGE_SelectVoice0(v);
        MESSAGE_SelectParameter0(1);      //---- amplitude
        MESSAGE_SetTime(Decay2Time[v]);
        MESSAGE_SetDestination(0);
   	  }
    }
  }
  else
  {
    for (v = 0; v < NUM_VOICES; v++)
    {
      if (Segment[v] == 3)
      {
        MESSAGE_SelectVoice0(v);
        MESSAGE_SelectParameter0(1);      //---- amplitude
	    MESSAGE_SetTime(ReleaseTime);
        MESSAGE_SetDestination(0);
      }
    }
  }

  NoDamper = state;
}



/*****************************************************************************
*	@brief  ApplyButtons
******************************************************************************/
int MMGEN_ApplyButtons(uint32_t buttons)
{
  int applied = 0;

  switch (buttons)
  {
    case 0x000001:  applied = 1;  break;    //  1 -
    case 0x000002:  applied = 1;  break;    //  2
    case 0x000004:  applied = 1;  break;    //  3
    case 0x000008:  applied = 1;  break;    //  4
    case 0x000010:  applied = 1;  break;    //  5
    case 0x000020:  applied = 1;  break;    //  6
    case 0x000040:  applied = 1;  break;    //  7 -
    case 0x000080:  applied = 1;  break;    //  8
    case 0x000100:  applied = 1;  break;    //  9
    case 0x000200:  applied = 1;  break;    // 10
    case 0x000400:  applied = 1;  break;    // 11
    case 0x000800:  applied = 1;  break;    // 12
    case 0x001000:  applied = 1;  break;    // 13 -
    case 0x002000:  applied = 1;  break;    // 14
    case 0x004000:  applied = 1;  break;    // 15
    case 0x008000:  applied = 1;  break;    // 16
    case 0x010000:  applied = 1;  break;    // 17
    case 0x020000:  applied = 1;  break;    // 18
    case 0x040000:  applied = 1;  MMGEN_SetNoDamper(1);  break;    // 19 -
    case 0x080000:  applied = 1;  MMGEN_SetNoDamper(0);  break;    // 20
    case 0x100000:  applied = 1;  break;    // 21
    case 0x200000:  applied = 1;  break;    // 22
    case 0x400000:  applied = 1;  break;    // 23
    case 0x800000:  applied = 1;  break;    // 24
  }

  return applied;
}
