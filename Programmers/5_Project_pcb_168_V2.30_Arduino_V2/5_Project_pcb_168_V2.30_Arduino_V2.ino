

/*****ATMEGA programmer V2.30B developed specially for the "Github_PCB-111000_V2" Project 


********runs on the ATMEGA168 and programs the ATMEGA 328 with thre mini-Os plus bootloader**********

Note: It can also program an Atmega 168 but this is not its purpose and testing for this case has been limited


The first time the program is allowed to run press -S- at the standard user prompt "P   P   P........"
This enables the remaining user prompts which are saved in a text file to be copied to the on-chip EEPROM

Having programed the on-chip EEPROM the standard user prompt is rienstated 
  Press "V" to retrieve version details
  Press P to program the mini-OS+bootloader FW for which the reset vector is 0x7000
  or press X to program single applications for which the reset vectors are zero
  
Mesage "ATMEGA328  detected." is displayed and the user prompt changes to "P/E/X    P/E/X....."

  If programming the EEPROM press 
  "E" or "e" to program addresses 5 to 0x3F5 if the target is an Atmega 328
  addresses 5 to 0x1FB if the target is an Atmega 168
  
  Press "D" to delete user space in the target EEPROM
  Press "d" to delete user space in the on-chip EEPROM
  
  Press "X" to revert to the standard prompt

  Press "P" to program the target

Having programmed a target device power cycle when requested and user prompt "R   R   R........" will be generated
press R to run the mini-OS auto cal routine


ON_CHIP EEPROM:   0x1FF and 0x1FE:  User cal bytes 
ATMEGA 168      0x1FD:        Default OSCCAL (the value built into the HW) 
          0x1FC       Set to 1 when used to program a target device, otherwise 0xFF
          
          Text strings required by the family of programmers are stored in 
          addresses 0 to 0x1FB. 

TARGET CHIP EEPROM reservations (ATMEGA 328)
0x0 Top of programmed text space  MSB       
0x1 Top of programmed text space  LSB       
0x2 Number of programmed data items         
0x3 Bottom of application EEPROM space  MSB
0x4 Bottom of application EEPROM space  LSB
Note: the ATMEGA 328 OS and 
bootloader strings are all stored in Flash so
it is not essential that its EEPROM be programmed

NOTE One extra line:
This Writes to the EEPROM of the target device to indicate 
that it has just been programmed and ensure that the auto cal routine runs


Possible Source of Trouble when programing the target EEPROM:  
Too much space may have been reserved for Target EEPROM strings
Search in .h file on RBL and reduce it to 127
Search in EEPROM subs on EEPROM_buffer and reduce the array size to 128

To run bootloader acquire the standard user prompt "P   P   P........" 
switch the DPDT to the right and press the reset switch.

*/





#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "Project_pcb_168_V2.30_Arduino.h"
#include "Resources/Project_pcb_168_V2.30_Arduino_SW_subs.c"
#include "Resources/Project_pcb_168_V2.30_Arduino_HW_subs.c"
#include "Resources/1_Basic_Timer_IO_subs.c"
#include "Resources/Project_pcb_168_V2.30_Arduino_EEPROM_subs.c"

#define wdr()  __asm__ __volatile__("wdr")
#define Version "\r\nProject_pcb_168_V2.30_Arduino"



/*void Text_to_EEPROM(int*, char);
char Text_from_EEPROM(int*);
void binUnwantedChars_dot (void);
void initialise_timers_for_cal_error(void);
void start_timers_for_cal_error(void);
long compute_error(char, char); 
void Minimise_error(int, char *, char *, long *, char*);
void auto_cal_168 (void);*/




//char cal_mode;
volatile char T1_OVF;

//char dot_counter=0, counter_1, counter_2;   
//char  OSCCAL_mem = 0;//OSCCAL_WV,
//long  error_mag; 
//int limit;







int main (void){ 
unsigned int target_type = 0, target_type_M;  
char keypress;
char op_code = 0;
unsigned char fuse_H = 0;
int long cal_error;
//unsigned char OSCCAL_WV;

cal_factor= 0;                                          //Set to 1 by OSC_CAL if user cal is available
setup_HW;


if(!(eeprom_read_byte((uint8_t*)(0x1FC))))
{Auto_cal_168(0);
save_cal_values(OSCCAL_WV); 

sendString("\r\nDefault OSCCAL value ");
sendHex(10,OSCCAL_DV);

sendString("\r\nNew OSCCAL value ");
sendHex(10,OSCCAL_WV);
eeprom_write_byte((uint8_t*)(0x1FC),0xFF);
}


if (watchdog_reset == 1)watchdog_reset = 0;             //Set to 1 after mini-OS device has been calibrated

else

{if ((eeprom_read_byte((uint8_t*)0x1FC) == 1)\
&& (MCUSR & (1 << PORF)));                              //POR after programing target device
else  {wdt_enable(WDTO_60MS); while(1);}

while(1){                                               //User prompt is R
do{sendString("R   ");} 
while((isCharavailable(100) == 0));                     //Keypress R to complete target calibration
if(receiveChar() == 'R')break;}

eeprom_write_byte((uint8_t*)0x1FC, 0xFF);                 //Clear EEPROM location set by programmer
MCUSR &= (~(1 << PORF));

waiting_for_I2C_master;                                   //Receive cal data over I2C bus             
OSCCAL_WV = receive_byte_with_Ack();          
cal_error = receive_byte_with_Ack();          
cal_error = (cal_error << 8) + receive_byte_with_Nack();
clear_I2C_interrupt;

Text_target_cal; sendString("  ");
sendHex(10, OSCCAL_WV);sendChar('\t');
sendString("error ");sendHex(10, cal_error);                //Print out cal data 
if (cal_error < 1000)sendString(" OK\r\n");
 wdt_enable(WDTO_60MS); while(1);}                        //SW reset and jump straight to user prompt P       

while(1){
do{sendString("P   ");} 
while((isCharavailable(255) == 0));                         //User prompt is P



switch(receiveChar()){
case 'P': case 'p': fuse_H = 0xD0;  op_code =1;break;       //Program reset vector to 0x7000            
case 'X': case 'x': fuse_H = 0xD7; op_code =1;break;        //Program reset vector to zero                        
case 'v': case 'V': //Text_Version;
sendString (Version);
newline();        //Check on-chip program version
wdt_enable(WDTO_60MS); while(1);break;
case 'S': case 's': Prog_on_chip_EEPROM();                  //Program on-chip EEPROM
wdt_enable(WDTO_60MS); while(1);break;
default:break;}if(op_code)break;}                           //Only exit loop the program target 
    
boot_target;
Atmel_powerup_and_target_detect;        


Text_Atmega;

switch (target){
case 168: sendString ("168");  PageSZ = 0x40; PAmask = 0x1FC0; 
FlashSZ=0x2000; EE_top = 0x1FA; text_start = 0x5; break;            //Space set aside for Cal/status data   
case 328: sendString ("328"); PageSZ = 0x40; PAmask = 0x3FC0; 
FlashSZ=0x4000;break;    //EE_top = 0x3F5; text_start = 0x5;          //Top 10 locations reseved for mini-OS use    
default: wdt_enable(WDTO_1S); while(1);break;}                      //First 5 locations reserved for alloccation table


Text_detected;
Text_Press_P_or_E;

while(1){

do{sendString("P/E/X    ");} 
while((isCharavailable(255) == 0));
op_code = receiveChar();        

switch (op_code){
case 'e':   EE_top = 0x1FC; text_start = 0x5;  Prog_Target_EEPROM(); break;                                                //Program Target EEPROM 
case 'E':  EE_top = 0x3F4; text_start = 0x1FD;Prog_Target_EEPROM(); break;

case 'D':                                                     //Delete Target EEPROM
sendString("\r\nTarget "); Text_EEP_reset;  
if(waitforkeypress() == 'D'){
Text_10_sec_wait;                                             //sendString("10 sec wait");
if(target == 168){for (int m = 0; m <= 0x1FA;m++)
{Read_write_mem('I', m, 0xFF);}}
if(target == 328){for (int m = 0; m <= 0x3F5;m++)     
{Read_write_mem('I', m, 0xFF);}}
sendString(" Done\r\n");}wdt_enable(WDTO_60MS); while(1);break;

case 'd': 
sendString("\r\nOn-chip "); Text_EEP_reset;
if(waitforkeypress() == 'D'){
for (int m = 0; m <= 0x1FA;m++)                               //Delete on-chip EEPROM
eeprom_write_byte((uint8_t*)(m),0xFF); 
sendString(" Done\r\n");}wdt_enable(WDTO_60MS); while(1);break;

case 'p':                                                       //Program target
case 'P': break;
case 'x':                                                       //Escape
case 'X': wdt_enable(WDTO_60MS); while(1);break;
default: break;} 

if ((op_code == 'P') || (op_code == 'p')) break;            //Enter target programming mode
//op_code = waitforkeypress();
}   

Initialise_variables_for_programming_flash;

Text_Send_HexF;


while ((keypress = waitforkeypress()) != ':')                 //Ignore characters before the first ':'
{if (keypress == 'x'){wdt_enable(WDTO_60MS); while(1);}}      //X pressed to escape
UCSR0B |= (1<<RXCIE0); sei();                                 //Enable UART interrupt

(Atmel_config(Chip_erase_h, 0));                              //Only erase flash when hex file is on its way

Program_Flash();

Atmel_config(write_fuse_bits_H_h,fuse_H );                    //Write to config bytes
if(fuse_H == 0xD0)
{Atmel_config(write_extended_fuse_bits_h,0xFD );
Atmel_config(write_fuse_bits_h,0xC2 );}
if(fuse_H == 0xD7)
{Atmel_config(write_extended_fuse_bits_h,0xFF );
Atmel_config(write_fuse_bits_h,0xE2 );}
Atmel_config(write_lock_bits_h,0xEB );                        //Lock byte is non critical

Verify_Flash();  

if(fuse_H == 0xD0)                                            //If programming mini-OS
{Read_write_mem('I', 0x3F9, 1);                               //trigger target cal process
eeprom_write_byte((uint8_t*)0x1FC, 0x01);  
MCUSR &= (~(1 << PORF));}

newline();
sendString (Version);newline();                               //Print out version and config bytes
Text_Config;

sendHex(16, (byte)Atmel_config(read_extended_fuse_bits_h, 0));
sendHex(16, (byte)Atmel_config(read_fuse_bits_H_h,0));  
sendHex(16, (byte)Atmel_config(read_fuse_bits_h, 0));
sendHex(16, (byte)Atmel_config(read_lock_bits_h, 0));


Text_on_chip_cal; sendString("  ");                           //Print out on-chip cal byte
sendHex(16, OSCCAL);

if (cal_factor==1) 
sendString(" UC");                                          //User cal factor
else sendString(" DC");                                     //Default calfactor

Text_File_size;
Text_Program_Verification;                                  //Print out file sizes
sendHex(10,cmd_counter); sendSpace();sendString("d'loaded:"); 
sendSpace(); sendHex(10,prog_counter); sendString(" in:"); 
sendSpace(); sendHex(10,read_ops); sendString(" out");
newline();

if(fuse_H == 0xD7){wdt_enable(WDTO_60MS); while(1);}        //No cal process: Exit immediately      

Text_Auto_cal;
while(1);                                                   //Note: mini_OS requires POR to run
return 1;  }                                                //auto cal


/***************************************************************************************************************************************************/
ISR(USART_RX_vect){
  unsigned char Rx_Hex_char=0;
  unsigned char Rx_askii_char;
  int local_pointer;
    
Rx_askii_char = receiveChar();
switch (Rx_askii_char){
case '0':  Rx_Hex_char = 0x00; break;                         //Convert askii chars received from hex file to binary digits
case '1':  Rx_Hex_char = 0x01; break;
case '2':  Rx_Hex_char = 0x02; break;
case '3':  Rx_Hex_char = 0x03; break;
case '4':  Rx_Hex_char = 0x04; break;
case '5':  Rx_Hex_char = 0x05; break;
case '6':  Rx_Hex_char = 0x06; break;
case '7':  Rx_Hex_char = 0x07; break;
case '8':  Rx_Hex_char = 0x08; break;
case '9':  Rx_Hex_char = 0x09; break;
case 'A':  Rx_Hex_char = 0x0A; break;
case 'B':  Rx_Hex_char = 0x0B; break;
case 'C':  Rx_Hex_char = 0x0C; break;
case 'D':  Rx_Hex_char = 0x0D; break;
case 'E':  Rx_Hex_char = 0x0E; break;
case 'F':  Rx_Hex_char = 0x0F; break;
case ':':  counter = 0;  break;
default: break;}

switch (counter){
case 0x0:   break;                                        //Detect -:- at start of new line
case 0x1:   tempInt1 = Rx_Hex_char<<4;  break;            //Acquire first digit 
case 0x2:   tempInt1 += Rx_Hex_char;                      //Acquire second digit and combine with first to obtain number of commands in line
      char_count = 9 + ((tempInt1) *2);                   //Calculate line length in terms of individual characters
      local_pointer = w_pointer++;                        //Update pointer to array "store"
      store[local_pointer] = tempInt1; break;             //Save the number of commands in the line to the array  
case 0x3:   tempInt1 = Rx_Hex_char<<4;  break;            //Next 4 digits give the address of the first command in the line
case 0x4: tempInt1 += Rx_Hex_char; 
      tempInt1=tempInt1<<8; break;                        //Acquire second digit and combine it with first 
case 0x5: tempInt1 += Rx_Hex_char<<4;  break;             //Continue for third digit
case 0x6:   tempInt1 += Rx_Hex_char;                      //Acquire final digit and caculate address of next command 
      local_pointer = w_pointer++;                        //Update pointers to array "store"
      store[local_pointer] = tempInt1; break;             //Save address of next command to array "store"
case 0x7:   break;                                        //chars 7 and 8 are not used
case 0x8:   break;
default:  break;}

if ((counter > 8)&&(counter < char_count)){               //Continue to acquire, decode and store commands
if ((counter & 0x03) == 0x01){tempInt1 = Rx_Hex_char<<4;} //Note: Final two chars at the end of every line are ignored
if ((counter & 0x03) == 0x02)  {tempInt1 += Rx_Hex_char;}
if ((counter & 0x03) == 0x03)  {tempInt2 = Rx_Hex_char<<4;}
if ((counter & 0x03) == 0x0)    {tempInt2+= Rx_Hex_char; 
                tempInt2=tempInt2<<8;tempInt1+=tempInt2;
local_pointer = w_pointer++;  
store[local_pointer] = tempInt1; cmd_counter++;}}

counter++;
w_pointer = w_pointer & 0b00011111; }                       //Overwrites array after 32 entries



/***************************************************************************************************************************************************/
void Program_Flash (void){

new_record();                         //Start reading first record which is being downloaded to array "store" 
start_new_code_block();                   //Initialise new programming block (usually starts at address zero but not exclusivle so)
Program_record();                     //Coppy commands from array "store" to the page_buffer                            
      
    
while(1){   
new_record();                       //Continue reading subsequent records
if (record_length==0)break;                 //Escape when end of hex file is reached


if (Hex_address == HW_address){               //Normal code: Address read from hex file equals HW address and lines contains 8 commands
switch(short_record){
case 0: if (space_on_page == (PageSZ - line_offset))    //If starting new page
      {page_address = (Hex_address & PAmask);}    //get new page address
      break;

case 1: start_new_code_block();               //Short line with no break in file (often found in WinAVR hex files).
    short_record=0;break;}}
    
    
if(Hex_address != HW_address){                //Break in file
  if (section_break){                   //Section break: always found when two hex files are combined into one                    
    if((Flash_flag) && (!(orphan)))
    {write_page_SUB(page_address);}           //Burn contents of the partially full page buffer to flash
    if(orphan) 
    write_page_SUB(page_address + PageSZ);}       //Burn outstanding commands to the next page in flash     
    
  if(page_break)                      //In practice page breaks and short jumps are rarely if ever found                      
    {if((Flash_flag) && (!(orphan)))          //Burn contents of the partially filled page buffer to flash
    {write_page_SUB(page_address);}                           
    orphan = 0;}
    
  start_new_code_block();                 //A new code block is always required where there is a break in the hex file.
  short_record=0;}
    
Program_record();}                      //Continue filling page_buffer
    

cli();  
UCSR0B &= (~(1<<RXCIE0));                 //download complete, disable UART Rx interrupt
LEDs_off;       
while(1){if (isCharavailable(1)==1)receiveChar();
    else break;}                    //Clear last few characters of hex file
  
if((Flash_flag) && (!(orphan)))
{write_page_SUB(page_address);}               //Burn final contents of page_buffer to flash
if(orphan) {write_page_SUB(page_address + PageSZ);}}




/***************************************************************************************************************************************************/
void Verify_Flash (void){ 
int star_counter, dot_counter;
signed int phys_address;

dot_counter = 0;
read_ops = 0;
Hex_cmd = 0;
star_counter = 0;
phys_address = 0;

while(1){
Hex_cmd = (byte)Read_write_mem('L',phys_address, 0x0);  
Hex_cmd = (Hex_cmd<<8) + (byte)(Read_write_mem('H',phys_address, 0x0)); 
phys_address++; 
       
if (phys_address == FlashSZ)break;

if (Hex_cmd != 0xFFFF){
read_ops += 1; 
star_counter += 1;}
else 
{dot_counter += 1;}

if (!( star_counter - 200)){sendChar('*');
star_counter = 0;}

if (!( dot_counter - 200)){sendChar('.');
dot_counter = 0;}
}}



/*********************************************************************************/

ISR (PCINT0_vect){                                               //UNO provides time standard   Pin change interrupt on SCK pin
                    
      if (!(TCCR1B)) {
      TCNT1_sum = 0;
      TCNT1 = 0;
      TCCR1B = prescaller_setting;}                               //1MHz clock
  
    else {TCCR1B = 0; int_counter += 1;
      error_sum = error_sum + TCNT1_sum - 32768 + TCNT1;}}
 







/*********************************************************************************/
/*ISR(TIMER2_OVF_vect) {
long TCNT1_BKP, overflow = 0, target_res;

target_res = 62500;
TCCR1B = 0;             //Halt T1
TCNT1_BKP = TCNT1;          //Copy the value of TCNT1
TCNT1 = 0;              //Clear TCNT1
TCCR1B = 1;             //Get T1 running again ASAP (Note T2 has not stopped running)
            //Get T1 running again ASAP (Note T2 has not stopped running)
if(EA_counter < cal_mode)T1_OVF = 0;          //Ignore first 5 results
else
{
switch(T1_OVF){
case 0: overflow = 0; break;
case 1: overflow = 65546; break;
case 2: overflow = 131072; break;}
error_SUM = error_SUM + (TCNT1_BKP - target_res + overflow);
T1_OVF = 0;}
EA_counter++;}*/

/*************************************************************/
//ISR(TIMER1_OVF_vect) {T1_OVF += 1;}
