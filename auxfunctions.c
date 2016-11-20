/*
 * auxfunctions.c
 *
 * Created: 09.10.2016 20:04:49
 *  Author: Wolfie
 */ 

#include "auxfunctions.h"

// function that sends a clock pulse for the display
void clk_pulse(void)
{
	clock_high;
	clock_low;
}

// function that converts a single digit integer to the integer that leads to displaying the number as digit on the
// segment display
uint8_t convert_nr(uint8_t number)
{
	uint8_t digit = 0;
	switch(number)
	{
		case 0:
		digit = 3;
		break;
		case 1:
		digit = 207;
		break;
		case 2:
		digit = 37;
		break;
		case 3:
		digit = 133;
		break;
		case 4:
		digit = 201;
		break;
		case 5:
		digit = 145;
		break;
		case 6:
		digit = 17;
		break;
		case 7:
		digit = 199;
		break;
		case 8:
		digit = 1;
		break;
		case 9:
		digit = 129;
		break;
		default:
		;
	}
	return digit;
}

// function that shifts in a number which gets displayed on the first segment display and shifts other numbers
void shift_number(uint8_t number)
{
	uint8_t digit = 0;
	digit = convert_nr(number);
	uint8_t i, pattern, mask;
	mask = 1;
	//strobe_low;
	for(i = 0; i < 8; i++)
	{
		
		
		pattern = digit & mask;
		
		if(!pattern)
		{
			data_low;
		}
		else
		{
			data_high;
		}
		clk_pulse();
		mask = mask << 1;
	}
	strobe_high;
}