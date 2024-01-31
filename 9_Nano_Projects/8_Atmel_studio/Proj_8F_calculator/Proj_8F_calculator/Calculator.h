




#include <avr/wdt.h>
#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

int uart_putchar_local(char c, FILE *mystr_output);
void Float_string_from_IO(void);


void scroll_display_zero(void);
void shift_display_left(void);
float acquire_fpn(void);


int Float_from_IO(FILE *mystr_input);
FILE uart_input_float_from_IO = FDEV_SETUP_STREAM(NULL, Float_from_IO, _FDEV_SETUP_READ);
FILE  mystdout = FDEV_SETUP_STREAM (uart_putchar_local, NULL, _FDEV_SETUP_WRITE);


/****************************************************************************************************************************/
char decimal_digit_Basic (char);
void I2C_Tx_8_byte_array(char*);
char waitforkeypress_Basic (void);
void reverse (char*);
int strLength (char*);
char wait_for_return_key_Basic(void);
void I2C_Tx_any_segment_clear_all(void);
void FPN_string_KBD_to_display(char*);
void display_8_digit_num_string(char*, float);
char arithmetic_op(char);

char watch_dog_reset = 0;



/****************************************************************************************************************************/
#define switch_1_down ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PINB & 0x40)^0x40)
#define switch_2_up   (PINB & 0x40)
#define switch_3_down  ((PIND & 0x80)^0x80)
#define switch_3_up   (PIND & 0x80)

#define setup_and_enable_PCI		PCICR |= ((1 << PCIE0) | (1 << PCIE2));\
									PCMSK0 |= (1 << PCINT6);    PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);
#define clear_PCI_on_sw1_and_sw3	PCIFR |= (1<< PCIF2);
#define disable_pci_on_sw1_and_sw3	PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));
#define disable_pci_on_sw2			PCMSK0 &= (~(1 << PCINT6)); 	

#define clear_digits				{for(int m = 0; m<=7; m++)digits[m]=0;}				
#define shift_digits_left			{for (int n = 0; n < 7; n++){digits[7-n] = digits[6-n];}}
#define Init_display_for_pci_data_entry \
clear_digits;\
digits[0] = '0';\
I2C_Tx_8_byte_array(digits);

/****************************************************************************************************************************/
#define setup_HW \
setup_watchdog;\
set_up_I2C;\
ADMUX |= (1 << REFS0);\
set_up_switched_inputs;\
Set_LED_ports;\
Unused_I_O;\
eeprom_write_byte((uint8_t*)(0x1FD),OSCCAL);\
while (!(PIND & (1 << PD1)));\
Timer_T0_10mS_delay_x_m(5);\
OSC_CAL;\
setup_PC_comms_Basic(0,16);



/****************************************************************************************************************************/
#define setup_watchdog \
if (MCUSR & (1 << WDRF))watch_dog_reset = 1;\
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;

#define wdr()  __asm__ __volatile__("wdr")

#define SW_reset {wdt_enable(WDTO_30MS);while(1);}



/****************************************************************************************************************************/
#define set_up_I2C \
TWAR = 0x02;



/****************************************************************************************************************************/
#define set_up_switched_inputs \
MCUCR &= (~(1 << PUD));\
DDRD &= (~((1 << PD2)|(1 << PD7)));\
PORTD |= ((1 << PD2) | (1 << PD7));\
DDRB &= (~(1 << PB6));\
PORTB |= (1 << PB6);



/****************************************************************************************************************************/
#define Unused_I_O \
MCUCR &= (~(1 << PUD));\
DDRB &= (~((1 << PB2)|(1 << PB7)));\
DDRC &= (~((1 << PC0)|(1 << PC1)|(1 << PC2)));\
DDRD &= (~((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6)));\
PORTB |= ((1 << PB2)|(1 << PB7));\
PORTC |= ((1 << PC0)|(1 << PC1)|(1 << PC2));\
PORTD |= ((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6));



/****************************************************************************************************************************/
#define Set_LED_ports   DDRB = (1 << DDB0) | (1 << DDB1);
#define LEDs_on       PORTB |= (1 << PB0)|(1 << PB1);
#define LEDs_off      PORTB &= (~((1 << PB0)|(1 << PB1)));
#define LED_1_on      PORTB |= (1 << PB1);
#define LED_1_off     PORTB &= (~( 1<< PB1));
#define LED_2_off     PORTB &= (~(1 << PB0));
#define LED_2_on      PORTB |= (1 << PB0);

#define Toggle_LED_1 \
if (PORTB & (1 << PB1)){LED_1_off;}\
else {PORTB |= (1 << PB1);}



/****************************************************************************************************************************/
#define OSC_CAL \
if ((eeprom_read_byte((uint8_t*)0x1FE) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x1FE) < 0xF0) && (eeprom_read_byte((uint8_t*)0x1FE)\
== eeprom_read_byte((uint8_t*)0x1FF))) {OSCCAL = eeprom_read_byte((uint8_t*)0x1FE);}



/****************************************************************************************************************************/
#define diagnostic_mode \




/****************************************************************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);



/****************************************************************************************************************************/
#include "../../Resources_AVR_projects/Subroutines/HW_timers.c"
#include "../../Resources_AVR_projects/PC_comms/Basic_Rx_Tx_Basic.c"
#include "../../Resources_AVR_projects/Chip2chip_comms/I2C_subroutines_1.c"
#include "../../Resources_AVR_projects/Chip2chip_comms/I2C_slave_Rx_Tx.c"
#include "../../Resources_AVR_projects/PC_comms/IO_to_and_from_strings.c"




/****************************************************************************************************************************/