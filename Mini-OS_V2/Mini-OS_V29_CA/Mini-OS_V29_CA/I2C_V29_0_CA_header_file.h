
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>


/*********************************************************************************************************************/


#define initialise_IO \
MCUCR &= (~(1 << PUD));\
DDRB = 0;\
DDRC = 0;\
DDRD = 0;\
PORTB = 0xFF;\
PORTC = 0xFF;\
PORTD = 0xFF;\
PORTC &= (~(1 << PC3));



/*********************************************************************************************************************/
#define	digit_3			PORTB |= (1 << PB0)
#define	digit_2			PORTB |= (1 << PB2)
#define	digit_1			PORTB |= (1 << PB3)
#define	digit_0			PORTB |= (1 << PB4)
#define	toggle_digit_0	PORTB ^= (1 << PB4)


#define	digit_7		PORTB |= (1 << PB5)
#define	digit_6		PORTC |= (1 << PC0)
#define	digit_5		PORTC |= (1 << PC1)
#define	digit_4		PORTC |= (1 << PC2)

#define	seg_a 	(1 << PB1)
#define	seg_b 	(1 << PD2)
#define	seg_c 	(1 << PD3)
#define	seg_d 	(1 << PD4)
#define	seg_e 	(1 << PD5)
#define	seg_f 	(1 << PD6)
#define	seg_g 	(1 << PD7)



/*********************************************************************************************************************/
#define set_digit_drivers \
DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);\
DDRC |= (1 << DDC0) | (1 << DDC1) | (1 << DDC2);\
DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);

#define clear_digits PORTB &= (~((1 << PB0) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5)));\
PORTC &= (~((1 << PC0) | (1 << PC1) | (1 << PC2)));

#define clear_display  PORTB |= (seg_a ); \
PORTD |= (seg_b | seg_c | seg_d | seg_e | seg_f | seg_g);


#define clear_display_buffer  for(int m = 0; m<=7; m++)display_buf[m] = 0;


/*********************************************************************************************************************/
#define one_U 	PORTD &= ~(seg_b)
#define ONE_U 	PORTD &= ~(seg_f)
#define one_L 	PORTD &= ~(seg_c)
#define ONE_L 	PORTD &= ~(seg_e)
#define ONE		PORTD &= (~(seg_e | seg_f))
#define one 	PORTD &= (~(seg_b | seg_c))
#define two 	PORTB &= (~(seg_a)); PORTD &= (~(seg_b | seg_d | seg_e | seg_g))
#define three 	PORTB &= (~(seg_a)); PORTD &= (~(seg_b | seg_c | seg_d | seg_g))
#define four 	PORTD &= (~(seg_b | seg_c | seg_f | seg_g))
#define five 	PORTB &= (~(seg_a)); PORTD &= (~(seg_c | seg_d | seg_f | seg_g))
#define six 	PORTD &= (~(seg_c | seg_d | seg_e | seg_f | seg_g))
#define seven 	PORTB &= (~(seg_a)); PORTD &= (~(seg_b | seg_c ))
#define eight 	PORTB &= (~(seg_a));  PORTD &= (~(seg_b | seg_c | seg_d | seg_e | seg_f | seg_g))
#define nine	PORTB &= (~(seg_a)); PORTD &= (~(seg_b | seg_c | seg_f | seg_g))
#define zero	PORTB &= (~(seg_a)); PORTD &= (~(seg_b | seg_c | seg_d | seg_e | seg_f ))
#define decimalP	PORTB &= (~(seg_a));	PORTD &= (~(seg_b | seg_e | seg_f | seg_g ))

