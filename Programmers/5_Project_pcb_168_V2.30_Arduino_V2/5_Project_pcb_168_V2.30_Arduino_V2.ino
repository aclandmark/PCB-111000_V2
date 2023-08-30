

/*****ATMEGA programmer V2.30B developed specially for the "Github_PCB-111000_V2" Project 


********runs on the ATMEGA168 and programs the ATMEGA 328 with the mini-Os plus bootloader**********

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

NOTE One extra line:
This Writes to the EEPROM of the target device to indicate 
that it has just been programmed and ensure that the auto cal routine runs


To run bootloader acquire the standard user prompt "P   P   P........" 
switch the DPDT to the right and press the reset switch.

*/




#include "Project_pcb_168_V2.30_Arduino.h"

#define Version "\r\nProject_pcb_168_V2.30_Arduino"

//volatile char T1_OVF;




int main (void){ 
unsigned int target_type = 0, target_type_M;  
char keypress, sign;
char op_code = 0;
unsigned char fuse_H = 0;
int long cal_error;
long error_up, error_down, error_percent;

cal_factor= 0;                                                  //Set to 1 by OSC_CAL if user cal is available
setup_HW;



/*************************************UNO calibrates Atmega168 if necessary*******************************************/

if(!(eeprom_read_byte((uint8_t*)(0x1FC))))                      //Cleared by 8_UNO_AVR_Programmer_V3 
{Auto_cal_168(0);                                                 //Search starting from 0xF0
  OSCCAL += 1;                                                  //Check performance for OSCCAL_WV +/- 1
  sei();
  error_up = compute_error_UNO(0,2,0);                          //Check that OSCCAL_WV is not at a
  OSCCAL -=2; error_down = compute_error_UNO(0,2,0);            //discontinuity on the
  
  OSCCAL += 1;                                                  //Restore OSCCAL to OSCCAL_WV
  if ((error_up > 500) || (error_down > 500))                     //500
  {sendString
  ("\r\nPoor result: Searching for alternative value\r\n");
  Auto_cal_168(1);} 
save_cal_values(OSCCAL_WV); 

sendString("\r\nDefault OSCCAL value ");
Num_to_PC(10,OSCCAL_DV);
sendString("\r\nNew OSCCAL value ");
Num_to_PC(10,OSCCAL_WV);

newline();

error_percent = compute_error_UNO(0,2,1)*100/32768;
if(!(error_percent)){sendString("\r\nError less than 1%\r\n");}
 else 
 { sendString("\r\nError = "); Num_to_PC(10,error_percent);
  sendString("%\r\n");}

sendString("-x- to check cal (15 Sec pause) or AOK to escape\r\n\r\n");
if (waitforkeypress() == 'x')

{timer_T0_sub(T0_delay_20ms);
OSCCAL -= 20;
osccal_MIN = OSCCAL;                                          //Compute cal error for 41 values of OSCCAL
  for(int m = 0; m <= 40; m++)
  {cal_error = compute_error_UNO(1,2,1);OSCCAL++;}
  OSCCAL = OSCCAL_WV;                                           //Restore working value of OSCCAL
  
 for(int m = 0; m <= 40; m++)                                    //Print table of values
  {Num_to_PC(10,osccal_MIN); osccal_MIN++;
    sendChar('\t');Num_to_PC(10,buffer[m]);
    sendChar('\t');
    percentage_error = buffer[m];
    Num_to_PC(10,percentage_error*100/32768);sendChar('%');
    newline();
    timer_T0_sub(T0_delay_20ms);
    timer_T0_sub(T0_delay_20ms);
  timer_T0_sub(T0_delay_20ms);}}

  
eeprom_write_byte((uint8_t*)(0x1FC),0xFF);}



/*************Program Entry point when Atmega 168 is fitted to PCB111000_V2************************************/

if (watchdog_reset == 1)watchdog_reset = 0;             //Set to 1 after mini-OS device has been calibrated
else



/********************************Read Atmega 328 (mini-OS device) calibration**************************************/

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
Num_to_PC(10, OSCCAL_WV);
sendChar('\t');
sendString("error "); 
 
 cal_error = cal_error/625;
 if(!(cal_error))sendString("less than 1%\r\n\r\n");
 else {Num_to_PC(10, cal_error);sendString("%\r\n\r\n");}
 
 
 wdt_enable(WDTO_60MS); while(1);}                        //SW reset and jump straight to user prompt P       



/************************Program entry point to program mini-OS or reinstate Atmega168 eeprom*************/

while(1){
do{sendString("P/S   ");} 
while((isCharavailable(255) == 0));  

switch(receiveChar()){

case 'S': case 's': Prog_on_chip_EEPROM();                  //Program on-chip EEPROM
wdt_enable(WDTO_60MS); while(1);break;

case 'v': case 'V': sendString (Version);
newline(); wdt_enable(WDTO_60MS); while(1);break;

case 'P': case 'p': fuse_H = 0xD0;  op_code =1; break;       //Program reset vector to 0x7000            
case 'X': case 'x': fuse_H = 0xD7; op_code =1; break;        //Program reset vector to zero                        

default:break;}if(op_code)break;}                           //Only exit loop the program target 
    
boot_target;
Atmel_powerup_and_target_detect;        
Text_Atmega;

switch (target){
case 168: sendString ("168: Not supported"); wdt_enable(WDTO_1S); break;  
case 328: sendString ("328"); PageSZ = 0x40; PAmask = 0x3FC0; 
FlashSZ=0x4000;break;      
default: wdt_enable(WDTO_1S); while(1);break;}  

Text_detected;
Text_Press_P_or_E;

while(1){

do{sendString("p/e    ");} 
while((isCharavailable(255) == 0));
op_code = receiveChar();        

switch (op_code){
           
case 'e':  EE_top = 0x3F6; text_start = 0x5; Prog_Target_EEPROM(); break;  

case 'D':                                                     //Delete Target EEPROM
sendString("\r\nTarget "); Text_EEP_reset;  
if(waitforkeypress() == 'D'){
Text_10_sec_wait;                                             //sendString("10 sec wait");
for (int m = 0; m <= 0x3F5;m++)     
{Read_write_mem('I', m, 0xFF);}
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

if ((op_code == 'P') || (op_code == 'p')) break;}            //Enter target programming mode

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

Num_to_PC(16, (byte)Atmel_config(read_extended_fuse_bits_h, 0));sendString("  ");
Num_to_PC(16, (byte)Atmel_config(read_fuse_bits_H_h,0));  sendString("  ");
Num_to_PC(16, (byte)Atmel_config(read_fuse_bits_h, 0));sendString("  ");
Num_to_PC(16, (byte)Atmel_config(read_lock_bits_h, 0));newline();

Text_File_size;
Text_Program_Verification;                                  //Print out file sizes
Num_to_PC(10,cmd_counter); 
sendSpace();sendString("d'loaded:"); 
sendSpace(); 
Num_to_PC(10,prog_counter); 
sendString(" in:"); 
sendSpace(); 
Num_to_PC(10,read_ops); 
sendString(" out");
newline();

if(fuse_H == 0xD7){wdt_enable(WDTO_60MS); while(1);}        //No cal process: Exit immediately      

newline();
Text_Auto_cal;
while(1);                                                   //Pause execution for print string
return 1;}


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

new_record();                                           //Start reading first record which is being downloaded to array "store" 
start_new_code_block();                                 //Initialise new programming block (usually starts at address zero but not exclusivle so)
Program_record();                                       //Coppy commands from array "store" to the page_buffer                            
      
    
while(1){   
new_record();                                           //Continue reading subsequent records
if (record_length==0)break;                             //Escape when end of hex file is reached


if (Hex_address == HW_address){                         //Normal code: Address read from hex file equals HW address and lines contains 8 commands
switch(short_record){
case 0: if (space_on_page == (PageSZ - line_offset))    //If starting new page
      {page_address = (Hex_address & PAmask);}          //get new page address
      break;

case 1: start_new_code_block();                         //Short line with no break in file (often found in WinAVR hex files).
    short_record=0;break;}}
    
    
if(Hex_address != HW_address){                          //Break in file
  if (section_break){                                   //Section break: always found when two hex files are combined into one                    
    if((Flash_flag) && (!(orphan)))
    {write_page_SUB(page_address);}                     //Burn contents of the partially full page buffer to flash
    if(orphan) 
    write_page_SUB(page_address + PageSZ);}             //Burn outstanding commands to the next page in flash     
    
  if(page_break)                                        //In practice page breaks and short jumps are rarely if ever found                      
    {if((Flash_flag) && (!(orphan)))                    //Burn contents of the partially filled page buffer to flash
    {write_page_SUB(page_address);}                           
    orphan = 0;}
    
  start_new_code_block();                               //A new code block is always required where there is a break in the hex file.
  short_record=0;}
    
Program_record();}                                      //Continue filling page_buffer
    

cli();  
UCSR0B &= (~(1<<RXCIE0));                             //download complete, disable UART Rx interrupt
LEDs_off;       
while(1){if (isCharavailable(1)==1)receiveChar();
    else break;}                                      //Clear last few characters of hex file
  
if((Flash_flag) && (!(orphan)))
{write_page_SUB(page_address);}                       //Burn final contents of page_buffer to flash
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
 




/************************************************************************************/
