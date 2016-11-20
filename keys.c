/*
 * keys.c
 *
 * Created: 09.10.2016 15:06:14
 *  Author: Wolfie
 */ 

#include "keys.h"

volatile uint8_t key_state;							// debounced and inverted key state
// bit = 1; key pressed
volatile uint8_t key_press;							// key press detect

volatile uint8_t key_rpt;							// key long press and repeat

ISR( TIMER0_OVF_vect)								// interrupt service routine on timer0 overflow
{
	static uint8_t ct0 = 0xFF, ct1 = 0xFF, rpt;
	uint8_t i;
	
	//TCNT0 = (uint8_t)(int16_t)-(F_CPU / 1024 * 10e-3 + 0.5);  // preload for 10ms
	
	i = key_state ^ ~KEY_PIN;						// key changed?
	ct0 = ~( ct0 & i);								// reset or count ct0
	ct1 = ct0 ^ (ct1 & i);							// reset or count ct1
	i &= ct0 & ct1;									// count until roll over
	key_state ^= i;									//then toggle debounced state
	key_press |= key_state & i;						// 0->1: key press detect
	
	if( (key_state & REPEAT_MASK) == 0 )            // check repeat function
	rpt = REPEAT_START;						        // start delay
	if( --rpt == 0 ){
		rpt = REPEAT_NEXT;                          // repeat delay
		key_rpt |= key_state & REPEAT_MASK;
	}
}

// check if a key has been pressed. Each pressed key is reported only once
uint8_t get_key_press(uint8_t key_mask)
{
	cli();											// disable interrupts while reading keys
	key_mask &= key_press;							// read keys
	key_press ^= key_mask;							// clear keys
	sei();											// enable interrupts again
	return key_mask;
}


// check if a key has been pressed long enough such that the
// key repeat functionality kicks in. After a small setup delay
// the key is reported being pressed in subsequent calls
// to this function. This simulates the user repeatedly
// pressing and releasing the key.
//
uint8_t get_key_rpt( uint8_t key_mask )
{
	cli();                                          // read and clear atomic !
	key_mask &= key_rpt;                            // read key(s)
	key_rpt ^= key_mask;                            // clear key(s)
	sei();											// enable interrupt again
	return key_mask;
}

// check if a key is pressed right now
uint8_t get_key_state( uint8_t key_mask)
{
	key_mask &= key_state;
	return key_mask;
}