/*
 * finalwakeup.c
 *
 * Created: 09.10.2016 15:04:06
 * Author : Wolfie
 */ 

#include "auxfunctions.h"
#include "keys.h"

int main(void)
{
    //uint8_t setAlarm = 0;
    uint8_t hourAlarm = 14;
    uint8_t minuteAlarm = 58;
    uint8_t hourbcd = 0;
    uint8_t minutebcd = 0;
    uint8_t ret = 0;
    uint8_t alarm = 0;
    uint8_t hourdec = 0;
    uint8_t minutedec = 0;
    
    KEY_DDR &= ~ALL_KEYS;							// configure key ports as input
    
    TCCR0B |= (1<<CS02);							// system clock/256 as timer input
    
    TIMSK0 |= (1<<TOIE0);							// enable timer overflow interrupt
    
    volatile uint16_t counter = 0;					// counter to fade LED
    volatile uint16_t intensity =0;					// intensity of LED
	
    DDRB |= (1 << DDB1);							// PB1 is now an output

    ICR1 = 0xFFFF;									// set TOP to 16bit

    OCR1A = intensity;								// set PWM for 25% duty cycle @ 16bit

    TCCR1A |= (1 << COM1A1)|(1 << COM1B1);			// set none-inverting mode

    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12)|(1 << WGM13);			// set Fast PWM mode using ICR1 as TOP
    
    TCCR1B |= (1 << CS10);							// START the timer with no prescaler
    
    DDRB |= (1 << 5) | (1 << 4) | (1 << 3);			//set pin 13, 12, 11 to output; 13=strobe input, 12=Data, 11=clock
	
    i2c_init();										//initialize i2c bus
    
    clock_low;										// set clock and data low, strobe high
    strobe_high;
    data_low;
    
    shift_number(0);								//set display to 0000
    shift_number(0);
    shift_number(0);
    shift_number(0);
    
    ret = i2c_start(0xD0 + I2C_WRITE);				//issue i2c start condition and send address and direction
    if(ret)											// failed to issue start condition, possibly no device found
    {
	    i2c_stop();
	    shift_number(9);
	    shift_number(9);
	    shift_number(9);
	    shift_number(9);
    }
    else											// issuing start condition ok, device accessible
    {
	    i2c_write(0x00);							// write address = 5
	    i2c_stop();
	    
	    sei();										// globally enable interrupts
	    
	    while(1)
	    {
		    alarm ^= get_key_press(192);			// check key3 and key2
		    
		    if(alarm & 1)							// if alarm bit 0 is high, fade on light
		    {
				if(counter < UINT16_MAX)
				{
					counter++;
				}
			    if(intensity < UINT16_MAX && counter == 1000 )
			    {
				    intensity++;
				    OCR1A = intensity;
				    counter = 0;
			    }
		    }
		    
		    if(alarm & 128 && hourAlarm == hourdec && minuteAlarm == minutedec)	// if alarm bit 8 is high
		    {
			    alarm = 1;														// turn on alarm
				counter = 0;
		    }
		    
		    if(alarm & 64)								// if alarm bit 7 is high, show alarm time
		    {
			    shift_number(minuteAlarm % 10);
			    shift_number(minuteAlarm / 10);
			    shift_number(hourAlarm % 10);
			    shift_number(hourAlarm / 10);
			    
			    if( get_key_press(1<<KEY1) || get_key_rpt(1<<KEY1))
			    {
				    if(minuteAlarm < 59)
				    {
					    minuteAlarm++;
				    }
				    else
				    {
					    minuteAlarm = 0;
				    }
			    }
			    
			    if( get_key_press(1<<KEY0) || get_key_rpt(1<<KEY0))
			    {
				    if(hourAlarm < 23)
				    {
					    hourAlarm++;
				    }
				    else
				    {
					    hourAlarm = 0;
				    }
			    }
				/*
				if(get_key_press(1<<KEY4) || get_key_rpt(1<<KEY4))
				{
					if(hourAlarm > 0)
					{
						hourAlarm--;
					}
					else
					{
						hourAlarm = 23;
					}
				}
				
				if(get_key_press(1<<KEY5) || get_key_rpt(1<<KEY5))
				{
					if(minuteAlarm > 0)
					{
						minuteAlarm--;
					}
					else
					{
						minuteAlarm = 59;
					}
				}
				*/
			    _delay_ms(500);
		    }
		    else
		    {
			    // interrupts while i2c is busy?
			    i2c_start_wait(0xD0 + I2C_WRITE);
			    i2c_write(0x01);
			    i2c_rep_start(0xD0 + I2C_READ);
			    //second = i2c_read(1);
			    minutebcd = i2c_read(1);
			    hourbcd = i2c_read(0);
			    i2c_stop();
			    hourdec = hourbcd & 15;
			    hourdec = hourdec + (hourbcd >> 4) * 10;
			    minutedec = minutebcd & 15;
			    minutedec = minutedec + (minutebcd >> 4) * 10;
			    shift_number(minutedec % 10);
			    shift_number(minutedec / 10);
			    shift_number(hourdec % 10);
			    shift_number(hourdec / 10);
		    }
		    
	    }
    } 
}

