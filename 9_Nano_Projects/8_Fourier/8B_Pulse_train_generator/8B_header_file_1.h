

#include <avr/wdt.h>

char watch_dog_reset = 0;
char power_on_reset = 0;
char external_reset  = 0;
char reset_status;
char User_response;


#define switch_3_down  ((PIND & 0x80)^0x80)
#define switch_3_up   (PIND & 0x80)
#define switch_1_down ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PINB & 0x40)^0x40)
#define switch_2_up   (PINB & 0x40)



#define set_up_PCI_on_sw3           PCICR |= (1 << PCIE2);
#define enable_pci_on_sw3           PCMSK2 |= (1 << PCINT23);
#define disable_pci_on_sw3          PCMSK2 &= (~(1 << PCINT23));



/*****************************************************************************/
#define setup_HW_with_reset_analysis \
determine_reset_source;\
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
I2C_Tx_LED_dimmer();\
\
\
/*OPTIONAL Setup_HW code gives default ap*/\
Timer_T0_10mS_delay_x_m(1);\
I2C_TX_328_check();\
waiting_for_I2C_master;\
if (receive_byte_with_Nack()==1)\
{TWCR = (1 << TWINT);\
wdt_enable(WDTO_30MS);\
I2C_Tx_display();}\
else TWCR = (1 << TWINT);


#define determine_reset_source \
if (MCUSR & (1 << WDRF))reset_status = 2;\
if (MCUSR & (1 << PORF))reset_status = 1;\
if (MCUSR & (1 << EXTRF))reset_status = 3;\
if((reset_status == 2) && (!(eeprom_read_byte((uint8_t*)0x1FC))))reset_status = 4;\
if((reset_status == 2) && (eeprom_read_byte((uint8_t*)0x1FC) == 0x01))reset_status = 5;\
eeprom_write_byte((uint8_t*)0x1FC, 0xFF);\


/*****************************************************************************/
#define setup_watchdog \
\
MCUSR = 0;\
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;

#define wdr()  __asm__ __volatile__("wdr")


#define One_25ms_WDT_with_interrupt \
wdr();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = (1<< WDE) | (1 << WDIE) | (1 << WDP0) |  (1 << WDP1);

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


#define User_prompt_A \
while(1){\
do{Serial.write("R?    ");}  while((isCharavailable_A (50) == 0));\
User_response = waitforkeypress_A();\
if((User_response == 'R') || (User_response == 'r'))break;} Serial.write("\r\n");



/*****************************************************************************/
#define OSC_CAL \
if ((eeprom_read_byte((uint8_t*)0x1FE) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x1FE) < 0xF0) && (eeprom_read_byte((uint8_t*)0x1FE)\
== eeprom_read_byte((uint8_t*)0x1FF))) {OSCCAL = eeprom_read_byte((uint8_t*)0x1FE);}



/*****************************************************************************/
#define diagnostic_mode \




/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);


/*****************************************************************************/
#include "Resources_nano_projects/Subroutines/HW_timers.c"
#include "Resources_nano_projects/PC_comms/Basic_Rx_Tx_Arduino.c"
#include "Resources_nano_projects/Chip2chip_comms/I2C_subroutines_1.c"
#include "Resources_nano_projects/Chip2chip_comms/I2C_subroutines_2.c"
#include "Resources_nano_projects/Subroutines/FPN_DIY_IO.c"
#include "Resources_nano_projects/Chip2chip_comms/I2C_slave_Rx_Tx.c"
#include "Resources_nano_projects/PC_comms/Arduino_Rx_Tx_UNO_pcb.c"
#include "Resources_nano_projects/Subroutines/Random_and_prime_nos.c"




/******************************************************************************/
