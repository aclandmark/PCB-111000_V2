

char watch_dog_reset = 0;
char User_response;
char num_as_string[12];
char str_counter;

char digits[8], charH, charL ;
char Hours, Minutes, Seconds, deci_Secs;
long deci_sec_counter;

/*****************************************************************************/
#include <avr/wdt.h>



/*****************************************************************************/
#define setup_HW_Arduino_IO \
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
#define setup_HW_Arduino_IO_Extra \
setup_HW_Arduino_IO;\
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
#include "Resources_nano_projects\PC_comms\Basic_Rx_Tx_Arduino.c"
#include "Resources_nano_projects\PC_comms\Arduino_Rx_Tx_UNO_pcb.c"
#include "Resources_nano_projects\Chip2chip_comms\I2C_slave_Rx_Tx.c"
#include "Resources_nano_projects\Chip2chip_comms\I2C_subroutines_1.c"
//#include "Resources_nano_projects\Chip2chip_comms\I2C_subroutines_2.c"
#include "Resources_nano_projects\Subroutines\Random_and_prime_nos.c"

/*****************************************************************************/
