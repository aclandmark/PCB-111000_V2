
/*

UNO sketch 8_UNO_AVR_programmer_V3

This sketch is used to help setup a PCB-111000_V2
It programs an Atmega 168 with  "5_Project_pcb_168_V2.30_Arduino_V2".
The Atmega 168 is then soldered onto a PCB-111000_V2 board along with all the other components except the displays.
It is then used to program the Atmega 328 with the the mini-OS and "Hello World" strings.

Step 1
Connect a UNO to a PC and upload sketch "8_UNO_AVR_programmer_V3"

Step 2
Connect the UNO to an Atmega 168 as follows using prototype pcb

 UNO                    Atmega 168
D0 (RX)                  Pin 2
D1 (Tx)                  Pin 3
11                       Pin 17
12                       Pin 18
13                       Pin 19
A3                        Pin 1
GND                       Pins 8 & 22
+5V                       Pins 7 & 20
Optioal LED + resistor from pin 8 to GND of the UNO 


Step 3 Program the Atmega 168

Reconnect the UNO to a PC with the Br@y++ terminal program open and set to 38400Baud plus default settings
Press -s- at the "s  s  s  s  s....." prompt to get
"Atmega 168 detected.
Press -p- to program flash, -r- to run target, -d- to clear the target EEPROM or -x- to escape."

Press-p- and send "5_Project_pcb_168_V2.30_Arduino_V2.ino.eightanaloginputs"
Follow instructions to calibrate the Atmega 168

At "P/S  P/S  P/S......" prompt press -s- and send the "on-chip_strings" file

The Atmega 168 can now be soldered onto the PCB-111000_V2 pcb

*/






#include "UNO_AVR_programmer.h"
#define Version "\r\nUNO_programmer_V3.0\r\n\
Used to program the PCB-111000_V2 Atmega168.\r\n" 

int main (void){ 

setup_328_HW;                                                     //see "Resources\ATMEGA_Programmer.h"

Reset_L;                                                          //Put target in reset state to dissable UART

while(1){
do{sendString("s  ");} 
while((isCharavailable(255) == 0));                               //User prompt 
if(receiveChar() == 's')break;}

Atmel_powerup_and_target_detect;                                  //Leave target in programming mode                              


sendString(" detected.\r\nPress -p- to program flash, \
-r- to run target, -d- to clear the target EEPROM or -x- to escape.");

while(1){
op_code = waitforkeypress();
switch (op_code){

case 'r':  
sendString("\r\nSet 57600 Baud and then press AK\r\n");
waitforkeypress();
Exit_programming_mode;
break;                                                           //Wait for UNO reset

case 't': 
sendString("\r\nSet 57600 Baud and then press AK\r\n");
waitforkeypress();
set_cal_clock();break;

case 'd':                                                       //Delete contents of the EEPROM
sendString("\r\nReset EEPROM! D or AOK to escape");             //but leave cal data.
newline();
if(waitforkeypress() == 'D'){
sendString("10 sec wait");
for (int m = 0; m <= EE_top; m++)                                //Includes cal bytes  
{Read_write_mem('I', m, 0xFF);}                                 //Write 0xFF to all EEPROM loactions bar the top 3
sendString(" Done\r\n");}
SW_reset;break;

case 'x': SW_reset; break;
default: break;} 

if ((op_code == 'p')||(op_code == 'P')) break;} 
sendString("\r\nSend hex file (or x to escape).\r\n");

Program_Flash_Hex();
Verify_Flash_Hex();


sendString (Version);
newline();


if ((Read_write_mem('O', 0x1FF, 0) > 0x0F)\
&&  (Read_write_mem('O', 0x1FF, 0) < 0xF0) && (Read_write_mem('O', 0x1FF, 0)\
== Read_write_mem('O', 0x1FE, 0))) {

  sendString("Atmega168 already calibrated. Result:  ");
sendHex  (10, Read_write_mem('O', 0x1FE, 0));
sendString("\r\n Press AK to repeat or reset UNO\r\n\r\n");
  waitforkeypress();}


sendString("To calibrate set 57600 Baud and then press AK \r\n\
UNO puts Square wave with 65.536mS period on PB5\r\n\
else reset UNO\r\n\r\n");
waitforkeypress();
set_cal_clock();

return 1;}





/***************************************************************************************************************************************************/
ISR(USART_RX_vect){upload_hex();}

/***************************************************************************************************************************************************/
ISR(TIMER2_OVF_vect) {                                          //Timer2 times out and halts at the end of the text file
if(text_started == 3)                                           //Ignore timeouts occurring before start of file download
  {endoftext -= 1; TCCR2B = 0; TIMSK2 &= (~(1 << TOIE2));       //Shut timer down
  inc_w_pointer; store[w_pointer] = 0;                          //Append two '\0' chars to the end of the text
  inc_w_pointer; store[w_pointer] = 0; }}



/***************************************************************************************************************************************************/
void set_cal_clock(void){

Read_write_mem('I', 0x1FC, 0);
_delay_ms(10);
UCSR0B &= (~((1 << RXEN0) | (1<< TXEN0)));
initialise_IO;
Set_LED_ports;
LEDs_off;
DDRB |= 1 << DDB5;
PORTB &= (~(1 << PORTB5));                                       //Output low
TCNT0 = 0;
TCCR0B = (1 << CS02) | (1 << CS00);                             //7.8125 KHz clock counts to 256 in 32.768mS                    

Reset_H;

while(1){
while(!(TIFR0 & (1<<TOV0)));
TIFR0 |= (1<<TOV0);
PORTB ^= (1 << PORTB5);}}






/***************************************************************************************************************************************************/
