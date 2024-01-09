

#include <avr/wdt.h>

#define POR_reset                 1
#define WDT_reset                 2
#define External_reset            3
#define WDT_reset_with_flag       4
#define WDT_with_ISR_reset        5

char reset_status;
char User_response;
char str_counter;
unsigned char SREG_BKP;

volatile char Data_Entry_complete, digit_entry;
volatile char scroll_control;
char digits[8];

#define set_up_PCI      PCICR |= ((1 << PCIE0) | (1 << PCIE2))
#define enable_PCI      PCMSK0 |= (1 << PCINT6);    PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);
#define disable_PCI     PCMSK0 &= (~(1 << PCINT6));    PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));
#define clear_PCI_on_sw1_and_sw3   PCIFR |= (1<< PCIF2);

#define disable_PCI_on_sw1  PCMSK2 &= (~(1 << PCINT18));
#define disable_PCI_on_sw3  PCMSK2 &= (~(1 << PCINT23));
#define enable_PCI_on_sw1  PCMSK2 |= (1 << PCINT18);
#define enable_PCI_on_sw3  PCMSK2 |= (1 << PCINT23);

#define switch_1_down ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PINB & 0x40)^0x40)
#define switch_2_up   (PINB & 0x40)
#define switch_3_down  ((PIND & 0x80)^0x80)
#define switch_3_up   (PIND & 0x80)

//Note Switch terminations are sw1:PD2  sw2:PB6 sw3:PD7 

#define Init_display_for_pci_data_entry \
clear_digits;\
digits[0] = '0';\
I2C_Tx_8_byte_array(digits);

#define clear_digits {for(int m = 0; m<=7; m++)digits[m]=0;}
#define shift_digits_left {for (int n = 0; n < 7; n++){digits[7-n] = digits[6-n];}}


/*****************************************************************************/
#define setup_HW_Arduino \
determine_reset_source;\
setup_watchdog_A;\
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
I2C_Tx_LED_dimmer();\
\
/*OPTIONAL Setup_HW code gives default ap*/\
\
Timer_T0_10mS_delay_x_m(1);\
I2C_TX_328_check();\
waiting_for_I2C_master;\
if (receive_byte_with_Nack()==1)\
{TWCR = (1 << TWINT);\
Serial.write("\r\nPress\r\n\
1 for OS version\r\n\
2 for system data\r\n\
3 Message from the OS (x to escape)\r\n\
4 Default project\r\n\
0 to escape\r\n");\
switch (waitforkeypress_A()){\
case '0':break;\
case '1':I2C_Rx_get_version_A('0');break;\
case '2':I2C_Rx_get_version_A('1');break;\
case '3':str_counter = 0; do\
{Read_Hello_world_string_A();newline_A();\
str_counter += 1;\
waitforkeypress_A();}\
while (str_counter < 3);\
break;\
case '4':wdt_enable(WDTO_30MS);\
I2C_Tx_display(); break;}}\
else TWCR = (1 << TWINT);



/**********************************************************************************************************/
#define determine_reset_source \
if (MCUSR & (1 << WDRF)){reset_status = 2;}\
if (MCUSR & (1 << PORF))reset_status = 1;\
if (MCUSR & (1 << EXTRF))reset_status = 3;\
if((reset_status == 2) && (!(eeprom_read_byte((uint8_t*)0x1FC))))reset_status = 4;\
if((reset_status == 2) && (eeprom_read_byte((uint8_t*)0x1FC) == 0x01))reset_status = 5;\
eeprom_write_byte((uint8_t*)0x1FC, 0xFF);\
MCUSR = 0;



/*****************************************************************************/
#define setup_watchdog_A \
\
wdr();\
SREG_BKP = SREG;\
cli();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;\
MCUSR = 0;\
SREG = SREG_BKP;

#define wdr()  __asm__ __volatile__("wdr")

#define One_25ms_WDT_with_interrupt \
wdr();\
SREG_BKP = SREG;\
cli();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = (1<< WDE) | (1 << WDIE) | (1 << WDP0) |  (1 << WDP1);\
MCUSR=0;\
SREG = SREG_BKP;

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
do{Serial.write("R?    ");}  while((isCharavailable_A (50) == 0));\
User_response = Serial.read();\
if((User_response == 'R') || (User_response == 'r'))break;} Serial.write("\r\n");





/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)))wdr();\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);



/*****************************************************************************/
#include "Resources_nano_projects\Subroutines\HW_timers.c"
#include "Resources_nano_projects\PC_comms\Basic_Rx_Tx_Arduino.c"
#include "Resources_nano_projects\Chip2chip_comms\I2C_subroutines_1.c"
#include "Resources_nano_projects\Chip2chip_comms\I2C_slave_Rx_Tx.c"
#include "Resources_nano_projects\Subroutines\Random_and_prime_nos.c"
#include "Resources_nano_projects\PC_comms\Arduino_Rx_Tx_UNO_pcb.c"
#include "Resources_nano_projects\Subroutines\FPN_DIY_IO.c"
#include "Resources_nano_projects\PC_comms\KBD_to_display.c"


/******************************************************************************/
