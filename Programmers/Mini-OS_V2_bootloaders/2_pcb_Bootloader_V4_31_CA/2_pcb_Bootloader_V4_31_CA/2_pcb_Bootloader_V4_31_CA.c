

/********  ATMEGA pcb bootloader runs in the boot space of the ATMEGA328**********/
/*Compile it using optimisation level Os ONLY  (Optimise for size)
It shares the ATMEGA 328 with the mini-OS which runs in the application code space.
Rx/Tx work at 57.6k
The ATMEGA 328 is programmed using "5_Project_pcb_168_V2.30_Arduino_V2"

EEPROM reservations
0x3FF	user cal if set
0x3FE	user cal if set
0x3FD	Default cal supplied by Atmel
0x3FC	If 'x' pressed at p/r prompt 0x3FC is set to 1 for diagnostic mode.  
		if 'r' is pressed, normal mode runs
0x3FB	Controls brightness
0x3FA	No longer used.
*/

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>



#define LED_2_on	 			PORTD &= (~(1 << PD7));
#define LED_2_off	 			PORTD |= (1 << PD7);
#define setup_leds				DDRD |= (1 << DDD7); PORTD |= (1 << PD7); PORTC = 0x07;	
#define Start_LED_Activity		PORTD &= (~(1 << PD7));		
#define Halt_LED_Activity		PORTD |= (1 << PD7);	

#include "../../../Bootloader_resources/Bootloader_header_file.h"
#include "../../../Bootloader_resources/Bootloader_HW_subs.c"
#include "../../../Bootloader_resources/Bootloader_SW_subs.c"
#include "../../../Bootloader_resources/Bootloader_main.c"


