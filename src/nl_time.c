/******************************************************************************/
/** @file		nl_time.c
	@date		2012-07-26
	@version	0.02
	@author		Stephan Schmitt [2012-06-18]
	@brief
  	@details
  				time = mantisse * 8^exp;

				14 bit = 2 bit exponent + 12 bit mantisse (unsigned)

				exp   inc     mantisse          time
				0     1      0 ... 4095        0 ...    4095   // bit 0 ... 11
				1     8    512 ... 4095     4096 ...   32760   // bit 12, 13, 14
				2    64    512 ... 4095    32768 ...  262080   // bit 15, 16, 17
				3   512    512 ... 4095   262144 ... 2096640   // bit 18, 19, 20

				range: 0 ... 2096640 ( = 2^21 - 512 )
				max. error: 0.02 % (513/512)

				For a base interval of 62.5 us (1/16 ms) the maximum time is
				131.04 s (2.18 min)
*******************************************************************************/




/*******************************************************************************
	@brief  	???
	@return		time...
*******************************************************************************/
unsigned int TIME_Compress(unsigned int time)
{
  if (time > 2096640)
    time = 2096640;

  if (time >> 15)
  {
    if (time >> 18)
    {
      return ( 0x3000 | (time >> 9) );
    }
    else
    {
      return ( 0x2000 | (time >> 6) );
    }
  }
  else
  {
    if (time >> 12)
    {
      return ( 0x1000 | (time >> 3) );
    }
    else
    {
      return time;
    }
  }
}


unsigned int TIME_Expand(unsigned int c_time)
{
  if (c_time > 16383)
    c_time = 16383;
  else if (c_time < 0)
    c_time = 0;

  int exp = (c_time >> 12);          // upper  2 bits
  int mantisse = (c_time & 0xFFF);   // lower 12 bits

  return (mantisse << (exp * 3) );
}


unsigned int TIME_Round(unsigned int time)
{
  if (time > 2096640)
    time = 2096640;

  if (time >> 15)
  {
    if (time >> 18)
    {
      return ( (time >> 9) << 9 );
    }
    else
    {
      return ( (time >> 6) << 6 );
    }
  }
  else
  {
    if (time >> 12)
    {
      return ( (time >> 3) << 3 );
    }
    else
    {
      return time;
    }
  }
}
