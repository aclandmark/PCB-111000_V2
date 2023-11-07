

char watch_dog_reset = 0;
char MCUSR_copy;
char User_response;
char num_as_string[12];


volatile char clock_mode;                                        //
volatile char  TWI_flag;

volatile char ms_counter;                                  //Increments every 10mS 
volatile char old_clock_mode;                                   //Used to restore mode when display is re-activated
char digits[8], charH, charL;                             //Holds characters to be displayed
char Hours, Minutes, Seconds;
  
long sec_counter;                                         //Display time is based on sec_counter 
long sec_counter_V;                                       //Volatile version of sec_counter (updated by TWI ISR)
long clock_time_long;                                     //Saves time (sec + ms * 100); used to switch between clock * SW 
char payload;                                             //No of characters to send over I2C (8 every sec & 2 otherwise)
unsigned char sec_counter_save;                                   //Set to 1 every second (indicates that sec_counter is to be updated from sec_counter_V)
unsigned char display_clear;                                        //Records status of display (blank or active)






/*****************************************************************************/
#include <avr/wdt.h>



/*****************************************************************************/
#define setup_HW_for_clock_SW \
MCUSR_copy = MCUSR;\
if (MCUSR_copy & (1 << PORF))\
{MCUSR_copy = (1 << PORF);\
/*eeprom_write_byte((uint8_t*)0x3F5,0);Initialise random generator memory */\
}\
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
Serial.begin(115200);\
while (!Serial);\
sei();\
I2C_Tx_LED_dimmer();



/*****************************************************************************/
#define setup_watchdog \
if (MCUSR & (1 << WDRF))watch_dog_reset = 1;\
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;

#define wdr()  __asm__ __volatile__("wdr")

#define SW_reset {wdt_enable(WDTO_30MS);while(1);}




/*****************************************************************************/
#define set_up_I2C \
TWAR = 0x02;


/*****************************************************************************/
#define set_up_switched_inputs \
MCUCR &= (~(1 << PUD));\
DDRD &= (~((1 << PD2)|(1 << PD7)));\
PORTD |= ((1 << PD2) | (1 << PD7));\
DDRB &= (~(1 << PB6));\
PORTB |= (1 << PB6);



/*****************************************************************************/
#define Unused_I_O \
MCUCR &= (~(1 << PUD));\
DDRB &= (~((1 << PB2)|(1 << PB7)));\
DDRC &= (~((1 << PC0)|(1 << PC1)|(1 << PC2)));\
DDRD &= (~((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6)));\
PORTB |= ((1 << PB2)|(1 << PB7));\
PORTC |= ((1 << PC0)|(1 << PC1)|(1 << PC2));\
PORTD |= ((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6));



/*****************************************************************************/
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



/*****************************************************************************/
#define OSC_CAL \
if ((eeprom_read_byte((uint8_t*)0x1FE) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x1FE) < 0xF0) && (eeprom_read_byte((uint8_t*)0x1FE)\
== eeprom_read_byte((uint8_t*)0x1FF))) {OSCCAL = eeprom_read_byte((uint8_t*)0x1FE);}



/*****************************************************************************/
#define User_prompt_A \
while(1){\
do{Serial.write("R?    ");}  while((isCharavailable_A(50) == 0));\
User_response = Serial.read();\
if((User_response == 'R') || (User_response == 'r'))break;} Serial.write("\r\n");



/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);


/***********Define clock display functions and modes***************/
#define switch_3_down  ((PIND & 0x80)^0x80)
#define switch_3_up   (PIND & 0x80)
#define switch_1_down ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PINB & 0x40)^0x40)
#define switch_2_up   (PINB & 0x40)


#define     set_up_PCI                    PCICR |= ((1 << PCIE0) | (1 << PCIE2))
#define     enable_PCI                    PCMSK0 |= (1 << PCINT6);    PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);
#define     disable_PCI_on_sw1_and_sw3     PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));
#define     clear_PCI_on_sw1_and_sw3      PCIFR |= (1<< PCIF2);
#define     disable_PCI_on_sw2            PCMSK0 &= (~(1 << PCINT6));

#define     disable_PCI_on_sw1            PCMSK2 &= (~((1 << PCINT18)))
#define hold_PCI_on_sw1_and_sw3         PCICR &= (~(1 << PCIE2));
#define restore_PCI_on_sw1_and_sw3      PCICR |= (1 << PCIE2);


/**********************************************************************************************************************/
#define shift_display_left   for (int p = 7;  (p); p--)\
digits[p] = digits[p-1];







/*****************************************************************************************/
#include "Resources_nano_projects\Subroutines\HW_timers.c"
#include "Resources_nano_projects\PC_comms\Basic_Rx_Tx_Arduino.c"
#include "Resources_nano_projects\PC_comms\Arduino_Rx_Tx_UNO_pcb.c"
#include "Resources_nano_projects\Chip2chip_comms\I2C_slave_Rx_Tx.c"
#include "Resources_nano_projects\Chip2chip_comms\I2C_subroutines_1.c"
#include "Resources_nano_projects\Chip2chip_comms\I2C_subroutines_2.c"
#include "Resources_nano_projects\Subroutines\Random_and_prime_nos.c"

/*****************************************************************************/
