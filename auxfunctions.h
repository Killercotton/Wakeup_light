/*
 * auxfunctions.h
 *
 * Created: 09.10.2016 20:03:44
 *  Author: Wolfie
 */ 


#ifndef AUXFUNCTIONS_H_
#define AUXFUNCTIONS_H_

#define F_CPU					16000000UL

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include "i2cmaster.h"
#include <stdint.h>
#include <avr/interrupt.h>

#define clock_high				PORTB|=(1<<3)
#define clock_low				PORTB&=~(1<<3)
#define data_high				PORTB|=(1<<4)
#define data_low				PORTB&=~(1<<4)
#define strobe_high				PORTB|=(1<<5)
#define strobe_low				PORTB&=~(1<<5)
#define ds1307					208

#define LED_DDR					DDRB
#define LED_PORT				PORTB
#define LED0					1

extern void clk_pulse(void);

extern uint8_t convert_nr(uint8_t number);

extern void shift_number(uint8_t number);
#endif /* AUXFUNCTIONS_H_ */