/*
 * vel_v_alloc.c
 *
 *  Created on: 03.07.2012
 *      Author: ssc
 */


#include "nl_mmgen.h"
#include "nl_keybed.h"
#include "nl_voices.h"


/******************************************************************************/
/*	modul local variables													  */
/******************************************************************************/

static uint8_t OldestVoice;    // Index der am frühesten ausgeschalteten Voice (bei Vollbelegung: am frühesten eingeschaltet)
static uint8_t YoungestVoice;  // Index der zuletzt ausgeschalteten Voice (bei Vollbelegung: zuletzt eingeschaltet)

// Arrays mit folgenden Elementen pro Voice:

static uint8_t VoiceState[NUM_VOICES] = {};   // 1 = key on, 0 = key released
static uint8_t NextYounger[NUM_VOICES] = {};  // Index der danach belegten Voice

// Array für Tasten:

static uint8_t KeyVoice[NUM_KEYS] = {};    // Nummer der zuletzt belegten Voice



/*****************************************************************************/
/**	@brief    InitVoices: initialisation for the voice allocation variables and arrays
******************************************************************************/
void VOICES_InitVoices()
{
  uint8_t i;

  for (i = 0; i < NUM_VOICES; i++)
  {
    VoiceState[i] = 0;
  }

  OldestVoice = 0;
  YoungestVoice = NUM_VOICES - 1;

  for (i = 0; i < NUM_VOICES - 1; i++)      // Voice:       0  1  2  3  4  5  6  7
  {                                         // NextYounger: 1  2  3  4  5  6  7  0
    NextYounger[i] = i + 1;                 // Oldest:      X
  }                                         // Youngest:                         X

  NextYounger[YoungestVoice] = 0;           // closed circle ?

  for (i = 0; i < NUM_KEYS; i++)
  {
    KeyVoice[i] = 0;
  }

  MMGEN_Init();
}



/*****************************************************************************/
/**	@brief   KeyDown: applies velocity curve, allocates a voice and starts its envelopes
	@param   diff: time between closing of the two switches per key
             key
******************************************************************************/
void VOICES_KeyDown(uint32_t diff, uint8_t key)
{
  uint8_t vel;

  if (diff < 3050)
  {
    if (diff < 160)
    {
      if (diff < 20)               // 0 ... 19 Ticks
      {
        vel = 127;                 // very fast key-down -> max. Vel
      }
      else                         // 20 ... 159 Ticks
      {
        vel = 137 - (diff >> 1);   // = 127 - (diff -  20)/  2
      }
    }
    else
    {
      if (diff < 768)              // 160 ... 767 Ticks
      {
        vel =  67 - (diff >> 4);   // =  57 - (diff - 160)/ 16
      }
      else                         // 768 ... 3049 Ticks
      {
        vel =  25 - (diff >> 7);   // =  19 - (diff - 768)/128
      }
    }
  }
  else                             // 3050 ...  Ticks
  {
    vel = 1;                       // very slow key-down -> min. Vel
  }

  uint8_t v = OldestVoice;

  MMGEN_StartEnvs(v, key, vel);

  // voice allocation

  if (VoiceState[v] == 1)              // die älteste Voice ist belegt und damit alle Voices -> Voice Stealing
  {
    NextYounger[YoungestVoice] = v;    // die bisher jüngste Voice bekommt den Zeiger auf die neue Voice
    YoungestVoice = v;                 // die neu belegte Voice ist jetzt die jüngste
  }
  else
  {
    VoiceState[v] = 1;
  }

  OldestVoice = NextYounger[v];    // die bisher zweitälteste Voice ist jetzt die älteste Voice

  KeyVoice[key] = v;               // Update der Belegungs-Liste der Tasten
}



/*****************************************************************************/
/**	@brief   KeyUp: applies velocity curve, de-allocates a voice and and puts its envelopes into release mode
	@param   diff: time between closing of the two switches per key
             key
******************************************************************************/
void VOICES_KeyUp(uint32_t diff, uint8_t key)
{
  uint8_t vel;

  if (diff < 3050)
  {
    if (diff < 160)
    {
      if (diff < 20)               // 0 ... 19 Ticks
      {
        vel = 127;                 // very fast key-up -> max. Vel
      }
      else                         // 20 ... 159 Ticks
      {
        vel = 137 - (diff >> 1);   // = 127 - (diff -  20)/  2
      }
    }
    else
    {
      if (diff < 768)              // 160 ... 767 Ticks
      {
        vel =  67 - (diff >> 4);   // =  57 - (diff - 160)/ 16
      }
      else                         // 768 ... 3049 Ticks
      {
        vel =  25 - (diff >> 7);   // =  19 - (diff - 768)/128
      }
    }
  }
  else                             // 3050 ...  Ticks
  {
    vel = 1;                       // very slow key-up -> min. Vel
  }

  uint8_t v = KeyVoice[key];

  MMGEN_ReleaseEnvs(v, vel);

  // voice allocation

  VoiceState[v] = 0;
  NextYounger[YoungestVoice] = v;  // die bisher jüngste Voice bekommt den Zeiger auf diese Voice
  YoungestVoice = v;               // diese Voice ist jetzt die zuletzt releaste und damit jüngste Voice
}
