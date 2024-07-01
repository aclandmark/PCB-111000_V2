
char reset_status;
char User_response;
char str_counter;
unsigned char SREG_BKP;

char digits[8], charH, charL ;
char Hours, Minutes, Seconds, deci_Secs;
long deci_sec_counter;


/*****************************************************************************/
#include <avr/wdt.h>

#define POR_reset                 1
#define WDT_reset                 2
#define External_reset            3
#define WDT_reset_with_flag       4
#define WDT_with_ISR_reset        5


/*****************************************************************************/
#define set_up_pci_on_sw1_and_sw3           PCICR |= (1 << PCIE2);
#define enable_pci_on_sw1_and_sw3           PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);
#define set_up_pci_on_sw2                   PCICR |= (1 << PCIE0);
#define enable_pci_on_sw2                   PCMSK0 |= (1 << PCINT6);

#define switch_1_down ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PINB & 0x40)^0x40)
#define switch_2_up   (PINB & 0x40)
#define switch_3_down  ((PIND & 0x80)^0x80)
#define switch_3_up   (PIND & 0x80)



/*****************************************************************************/
#define setup_HW_with_reset_analysis \
determine_reset_source;\
setup_watchdog_with_reset_analysis;\
set_up_I2C;\
ADMUX |= (1 << REFS0);\
set_up_switched_inputs;\
Set_LED_ports;\
Unused_I_O;\
eeprom_write_byte((uint8_t*)(0x1FD),OSCCAL);\
while (!(PIND & (1 << PD1)));\
Timer_T0_10mS_delay_x_m(5);\
OSC_CAL;\
setup_PC_comms_Basic(0,16);\
I2C_Tx_LED_dimmer();



/*****************************************************************************/
#define determine_reset_source \
if (MCUSR & (1 << WDRF)){reset_status = 2;}\
if (MCUSR & (1 << PORF))reset_status = 1;\
if (MCUSR & (1 << EXTRF))reset_status = 3;\
if((reset_status == 2) && (!(eeprom_read_byte((uint8_t*)0x1FA))))reset_status = 4;\
if((reset_status == 2) && (eeprom_read_byte((uint8_t*)0x1FA) == 0x01))reset_status = 5;\
eeprom_write_byte((uint8_t*)0x1FA, 0xFF);\
MCUSR = 0;




/*****************************************************************************/
#define setup_watchdog_with_reset_analysis \
\
wdr();\
SREG_BKP = SREG;\
cli();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;\
MCUSR = 0;\
SREG = SREG_BKP;


#define wdr()  __asm__ __volatile__("wdr")


#define SW_reset {wdt_enable(WDTO_30MS);while(1);}


#define Sixty_four_ms_WDT_with_interrupt \
wdr();\
SREG_BKP = SREG;\
cli();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = (1<< WDE) | (1 << WDIE) | (1 << WDP1);\
MCUSR=0;\
SREG = SREG_BKP;




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



#define Reset_Atmega328 \
PORTC |= (1 << PC3);\
DDRC |= (1 << PC3);\
_delay_ms(1);\
PORTC &= (~((1 << PC3)));\
_delay_ms(1);\
PORTC |= (1 << PC3);

#define Reset_I2C \
DDRB |= (1 << DDB4);\
PORTB &= (~(1 << PORTB4));
 



/*****************************************************************************/
#define OSC_CAL \
if ((eeprom_read_byte((uint8_t*)0x1FE) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x1FE) < 0xF0) && (eeprom_read_byte((uint8_t*)0x1FE)\
== eeprom_read_byte((uint8_t*)0x1FF))) {OSCCAL = eeprom_read_byte((uint8_t*)0x1FE);}



/*****************************************************************************/
#define User_prompt_Basic \
while(1){\
do{String_to_PC_Basic("R?    ");}  while((isCharavailable_Basic(50) == 0));\
User_response = waitforkeypress_Basic();\
if((User_response == 'R') || (User_response == 'r'))break;} String_to_PC_Basic("\r\n");



/*****************************************************************************/
/*#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;*/


#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
{int m = 0; while((!(TWCR & (1 << TWINT))) && (m++ < 5000))wdr();\
if (m >= 4999){sei(); while((!(TWCR & (1 << TWINT))));}}\
TWDR;



#define clear_I2C_interrupt \
TWCR = (1 << TWINT);





/************Define clock memory locations********************/
#define HoursH digits[7]
#define HoursL digits[6]
#define MinsH digits[5]
#define MinsL digits[4]
#define SecsH digits[3]
#define SecsL digits[2]
#define deci_SecsH digits[1]
#define deci_SecsL digits[0]



/***********Define clock display functions and modes***************/
#define reset_clock_1   {digits[0] = 0; for (int m = 1; m < 8; m++)digits[m] = '0'; }



#define AT_clock_mode 'C'
#define AT_exit_stop_watch 10, '1'
#define AT_mode_1 1

#define display_clock 'D', '1'
#define hide_clock 'D', '2'
#define pause_clock 'D', '3'
#define increment_seconds 'D', '5'
#define decrement_minutes 'D', '6'
#define decrement_seconds 'D', '7'
#define increment_minutes 'D', '8'

#define display_current_time 'E', '1'
#define display_stored_times 'E', '4'
#define store_time 'E', '2'
#define one100ms_mode 'E', '1'
#define ten_ms_mode 'E', '2'




/*****************************************************************************************/
#include "Resources_nano_projects\Subroutines\HW_timers.c"
//#include "Resources_nano_projects\PC_comms\Basic_Rx_Tx_Arduino.c"
//#include "Resources_nano_projects\PC_comms\Arduino_Rx_Tx_UNO_pcb.c"
#include "Resources_nano_projects\PC_comms\Basic_Rx_Tx_Basic.c"
#include "Resources_nano_projects\Chip2chip_comms\I2C_slave_Rx_Tx.c"
#include "Resources_nano_projects\Chip2chip_comms\I2C_subroutines_1.c"
//#include "Resources_nano_projects\Chip2chip_comms\I2C_subroutines_2.c"
#include "Resources_nano_projects\Subroutines\Random_and_prime_nos.c"

/*****************************************************************************/
