/******************************************************************************/
/** @file		nl_keybed.h
	@date		2012-07-26
	@version	0.03
  	@author		Daniel Tzschentke [2012-05-29]
	@brief		keyboard scan
*******************************************************************************/


#include "nl_gpio.h"
/** @todo make the uart include dependant from the used interface*/
#include "nl_uart.h"
#include "nl_lpc17xx.h"

#include "nl_voices.h"
#include "nl_keybed.h"
#include "nl_board.h"


/******************************************************************************/
/*	modul local variables													  */
/******************************************************************************/
	
static uint32_t timerTick = 0;		// timebase for all the functions

static uint8_t keyState[NUM_KEYS] = {};
/*	0 = 000: key off, fully released
	1 = 001: key off, between the two contact points
	3 = 011: key off, fully pressed -> will go on
	4 = 100: key on, fully released -> will go off
	5 = 101: key on, between the two contact points
	7 = 111: key on, fully pressed                   */

static uint32_t keyTime[NUM_KEYS] = {};	   // multiples of 100 us

static uint8_t upperSwitches = 0;


/******************************************************************************/
/*	modul local prototypes																  */
/******************************************************************************/

uint8_t GetKeys(void);
void SetLine(uint8_t line);
void DetectNotes(uint8_t key, uint8_t state);
void ReadKeysFromOddLine(uint8_t key);
void Delay(uint16_t delay);



/******************************************************************************/
/*	functions																  */
/******************************************************************************/



/******************************************************************************/
/**	@brief  	General initialization of the input and output pins. (public)
	@todo		KEYBED_Init(): Pin-Names should be defined in nl_board.h
*******************************************************************************/
void KEYBED_Init(void)
{
	/* output pins for the line decoder */
	GPIO_PinOpt(KEYBED_LINE_A);
	GPIO_PinOpt(KEYBED_LINE_B);
	GPIO_PinOpt(KEYBED_LINE_C);
	GPIO_PinOpt(KEYBED_LINE_D);
	GPIO_PinOpt(KEYBED_LINE_EN_DIS);	// Enable Pin of the Line Decoder

	/* explicite states */
	GPIO_PinClr(KEYBED_LINE_A);
	GPIO_PinClr(KEYBED_LINE_B);
	GPIO_PinClr(KEYBED_LINE_C);
	GPIO_PinClr(KEYBED_LINE_D);
	GPIO_PinSet(KEYBED_LINE_EN_DIS);	// turn decoder off (inverted logic)

	/* input pins for the keys*/
	GPIO_PinIpt(KEYBED_KEY_0);
	GPIO_PinIpt(KEYBED_KEY_1);
	GPIO_PinIpt(KEYBED_KEY_2);
	GPIO_PinIpt(KEYBED_KEY_3);
	GPIO_PinIpt(KEYBED_KEY_4);
	GPIO_PinIpt(KEYBED_KEY_5);
	GPIO_PinIpt(KEYBED_KEY_6);
	GPIO_PinIpt(KEYBED_KEY_7);

	/* no pull ups for the inputs */
	GPIO_PinMod(KEYBED_KEY_0, GPIO_FLOATING);
	GPIO_PinMod(KEYBED_KEY_1, GPIO_FLOATING);
	GPIO_PinMod(KEYBED_KEY_2, GPIO_FLOATING);
	GPIO_PinMod(KEYBED_KEY_3, GPIO_FLOATING);
	GPIO_PinMod(KEYBED_KEY_4, GPIO_FLOATING);
	GPIO_PinMod(KEYBED_KEY_5, GPIO_FLOATING);
	GPIO_PinMod(KEYBED_KEY_6, GPIO_FLOATING);
	GPIO_PinMod(KEYBED_KEY_7, GPIO_FLOATING);

	SetLine(1);
}


/******************************************************************************/
/**	@brief  	general keyboard process function - it should be called 
				regularly (public)
	@details	for a high time resolution 100 us are great
				for a low time resolution 500 are acceptable
*******************************************************************************/

void KEYBED_Process(void)
{
	timerTick++;
	
	upperSwitches = GetKeys();
	SetLine(0);
	ReadKeysFromOddLine(0);
	SetLine(3);
	Delay(22);

	upperSwitches = GetKeys();
	SetLine(2);
	ReadKeysFromOddLine(8);
	SetLine(5);
	Delay(22);

	upperSwitches = GetKeys();
	SetLine(4);
	ReadKeysFromOddLine(16);
	SetLine(7);
	Delay(22);

	upperSwitches = GetKeys();
	SetLine(6);
	ReadKeysFromOddLine(24);
	SetLine(9);
	Delay(22);

	upperSwitches = GetKeys();
	SetLine(8);
	ReadKeysFromOddLine(32);
	SetLine(11);
	Delay(22);

	upperSwitches = GetKeys();
	SetLine(10);
	ReadKeysFromOddLine(40);
	SetLine(13);
	Delay(22);

	upperSwitches = GetKeys();
	SetLine(12);
	ReadKeysFromOddLine(48);
	SetLine(15);
	Delay(22);

	upperSwitches = GetKeys();
	SetLine(14);
	ReadKeysFromOddLine(56);
	SetLine(1);
	Delay(22);
}


/******************************************************************************/
/**	@brief  	reads and returns of 8 pins (private)
 	@details	Wrapper function: Reads the state of the output bus matrix UC_KEY
	@return 	8 input lines in a byte
*******************************************************************************/
uint8_t GetKeys(void)
{
	#if defined (BOARD_EMPHASE_V01_REV_A) || defined (BOARD_PROTOTYPE)
		return FIO2PIN0;
		//	return LPC_GPIO2->FIOPIN0;
	#endif
}



/*****************************************************************************/
/**	@brief		very sloppy delay function
				dalay=4 takes about 1 us
******************************************************************************/

void Delay(uint16_t delay)
{
	uint8_t cnt = 0;
	for (cnt=0; cnt < delay; cnt++)
	{
		_delay_clk;
		_delay_clk;
	}
}


/******************************************************************************/
/**	@brief  	Sends the number of a Line (0...15) binary coded to 4 outputs
				(private)
	@details 	The four pins drive a 'binary to 1-of-n hardware decoder'.
	@param		line number - 0..15
	@todo		This function can be optimized?!
*******************************************************************************/

void SetLine(uint8_t line)
{		
	GPIO_PinSet(KEYBED_LINE_EN_DIS);					/* turn decoder off (inverted logic) */
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	#ifdef BOARD_EMPHASE_V01_REV_A
		FIO1CLR2 = 0b11110000;		/* reset all 4 pins */
		FIO1SET2 = line << 4;		/* set the encoded line */
	#elif BOARD_PROTOTYPE
		FIO0CLR0 = 0b11110000;		/* reset all 4 pins */
		FIO0SET0 = line << 4;		/* set the encoded line */
	#endif
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	_delay_clk;
	GPIO_PinClr(KEYBED_LINE_EN_DIS);					/* enable decoder ic */
}


/******************************************************************************/
/**	@brief  	handles changes of the key state
	@param		key
				state
*******************************************************************************/

void DetectNotes(uint8_t key, uint8_t state)
{
	if (state == 1)										// upper contact closed or lower contact opened
	{
		keyTime[key] = timerTick;						// start time measuring
		keyState[key] = (keyState[key] & 4) | 1;		// store changed state
	}
	else if (state == 3)								// lower contact closed
	{
		if (keyState[key] & 4)							// already on - no key-on event
		{
			keyState[key] = 7;							// store changed state
		}
		else
		{
			uint32_t diff = timerTick - keyTime[key];	// finish time measuring
																	
			VOICES_KeyDown(diff, key);
		
			keyState[key] = 7;							// key is down
		}
	}
	else												// state == 0: upper contact opened
	{
		if (!(keyState[key] & 4))						// already off - no key-off event
		{
			keyState[key] = 0;							// store changed state
		}
		else
		{
			uint32_t diff = timerTick - keyTime[key];	// finish time measuring

			VOICES_KeyUp(diff, key);
			
			keyState[key] = 0;							// key is up
		}
	}
}


/******************************************************************************/
/**	@brief  	reads the state of 8 lower switches; if the state of the key has changed, detectNotes() is called	(private)
	@param		key: base key of a group of 8 keys
*******************************************************************************/

void ReadKeysFromOddLine(uint8_t key)
{
	uint8_t lowerSwitches = GetKeys();

	uint8_t state = ((!(lowerSwitches & 128)) << 1) | (!(upperSwitches & 128));

	if (state != (keyState[key] & 3))
	  DetectNotes(key, state);

	key++;
	state = ((!(lowerSwitches & 64)) << 1) | (!(upperSwitches & 64));

	if (state != (keyState[key] & 3))
	  DetectNotes(key, state);

	key++;
	state = ((!(lowerSwitches & 32)) << 1) | (!(upperSwitches & 32));

	if (state != (keyState[key] & 3))
	  DetectNotes(key, state);

	key++;
	state = ((!(lowerSwitches & 16)) << 1) | (!(upperSwitches & 16));

	if (state != (keyState[key] & 3))
	  DetectNotes(key, state);

	key++;
	state = ((!(lowerSwitches & 8)) << 1) | (!(upperSwitches & 8));

	if (state != (keyState[key] & 3))
	  DetectNotes(key, state);

	key++;
	state = ((!(lowerSwitches & 4)) << 1) | (!(upperSwitches & 4));

	if (state != (keyState[key] & 3))
	  DetectNotes(key, state);

	key++;
	state = ((!(lowerSwitches & 2)) << 1) | (!(upperSwitches & 2));

	if (state != (keyState[key] & 3))
	  DetectNotes(key, state);

	key++;
	state = ((!(lowerSwitches & 1)) << 1) | (!(upperSwitches & 1));

	if (state != (keyState[key] & 3))
	  DetectNotes(key, state);
}



/*******************************************************************************
	Changelog
	2012-06-04					added switch to decide if MIOS32-Board or
								emphasis board is used -> not tested
	???							modularisation and mios compatibility
	2012-07-26	0.03	DTZ		renaming during new name conventions
								pin functions depending on the board type
*******************************************************************************/

