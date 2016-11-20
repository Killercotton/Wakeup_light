/*
 * keys.h
 *
 * Created: 09.10.2016 15:06:28
 *  Author: Wolfie
 */ 


#ifndef KEYS_H_
#define KEYS_H_

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU						16000000UL
#endif

#define KEY_DDR						DDRD
#define	KEY_PORT					PORTD
#define KEY_PIN						PIND
#define	KEY0						4
#define	KEY1						5
#define	KEY2						6
#define	KEY3						7
#define KEY4						3
#define KEY5						2
#define	ALL_KEYS					(1<<KEY0 | 1<<KEY1 | 1<<KEY2 | 1<<KEY3 )//| 1<<KEY4 | 1<<KEY5)

#define REPEAT_MASK					(1<<KEY1 | 1<<KEY0 )//| 1<<KEY4 | 1<<KEY5)
#define REPEAT_START				50
#define REPEAT_NEXT					20

#define LED_DDR						DDRB
#define LED_PORT					PORTB
#define LED0						1

extern volatile uint8_t key_state;							// debounced and inverted key state
															// bit = 1; key pressed
extern volatile uint8_t key_press;							// key press detect

extern volatile uint8_t key_rpt;							// key long press and repeat

// check if a key has been pressed. Each pressed key is reported only once
extern uint8_t get_key_press(uint8_t key_mask);

// check if a key is pressed right now
extern uint8_t get_key_state(uint8_t key_mask);

extern uint8_t get_key_rpt(uint8_t key_mask);
#endif /* KEYS_H_ */