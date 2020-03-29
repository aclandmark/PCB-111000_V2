

/*****ATMEGA programmer V2.30B developed specially for the bootloader pcb project 
********runs on the ATMEGA168 and programs the ATMEGA 328**********
At the user prompt press P to program the OS+bootloader FW or X to program single applications
If programming the EEPROM press 
"E" to program addresses 5 to 0x1FF or
"e" to program addresses 0x200 to 0x3FD or
"D" to delete the entire EEPROM up to 0x3FD (preserves cal bytes)
When verifying the target EEPROM press "r" or "R".

ON_CHIP EEPROM:  	User cal bytes if set are stored in addresses 0x1F7 and 0x1F8.
ATMEGA 168			Default OSCCAL (the value built into the HW) is stored in
					address 0x1F9.
					Text strings required by the family of programmers are stored in 
					addresses 0 to 0x1F6.	

TARGET CHIP EEPROM reservations (ATMEGA 328)
0x0	Top of programmed text space  MSB				
0x1	Top of programmed text space  LSB				
0x2 Number of programmed data items					
0x3 Bottom of application EEPROM space	MSB
0x4 Bottom of application EEPROM space	LSB
Note: the ATMEGA 328 OS and 
bootloaderstrings are all stored in Flash so
it is not essential that its EEPROM be programmed

NOTE One extra line:
This Writes to the EEPROM of the target device to indicate 
that it has just been programmed and ensure that the auto cal routine runs
*/

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

#include "Project_pcb_168_V2.30B.h"
#include "Project_pcb_168_V2.30B_SW_subs.c"
#include "Project_pcb_168_V2.30B_HW_subs.c"
#include "1_Basic_Timer_IO_subs.c"
#include "Project_pcb_168_V2_30B_EEPROM_subs.c"

#define wdr()  __asm__ __volatile__("wdr")
#define Version "Project_pcb_168_V2.30B"


 void Text_to_EEPROM(int*, char);
char Text_from_EEPROM(int*);
void binUnwantedChars_dot (void);




int main (void){ 
unsigned int target_type = 0, target_type_M;	
char temp_char;
char op_code;	
unsigned char fuse_H;
int long cal_error;
char OSCCAL_WV;


setup_HW;


/*****************Power-up and make contact with target****************************/
op_code =0;		
while(1){
do{sendString("P  ");} while((isCharavailable(255) == 0)); 
switch(receiveChar()){
case 'P': case 'p':	fuse_H = 0xD0; 	op_code =1;break;													//FF_cmd_counter_limit = 500;
case 'X': case 'x':	fuse_H = 0xD7; op_code =1;break; 													//FF_cmd_counter_limit = 10000;
case 'v': case 'V': sendString (Version);newline();wdt_enable(WDTO_60MS); while(1);break;
case 'R': case 'r':
waiting_for_I2C_master;									
OSCCAL_WV = receive_byte_with_Ack();					
cal_error = receive_byte_with_Ack();					
cal_error = (cal_error << 8) + receive_byte_with_Nack();
clear_I2C_interrupt;
sendString("\r\nTarget cal factor  ");
sendHex(16, OSCCAL_WV);sendChar('\t');
sendString("error ");sendHex(10, cal_error); sendString("\r\n");break;

case 'S': case 's': Prog_on_chip_EEPROM();wdt_enable(WDTO_60MS); while(1);break;
default:break;}if(op_code)break;}	
		
boot_target;
Atmel_powerup_and_target_detect;        //waits for keypress to continue  Holds target in reset state

newline();sendString("ATMEGA");
switch (target){
case 168: sendString ("168");  PageSZ = 0x40; PAmask = 0x1FC0; FlashSZ=0x2000; EE_top = 0x200; text_start = 0x5; break;		//no cal bits
case 328: sendString ("328"); PageSZ = 0x40; PAmask = 0x3FC0; FlashSZ=0x4000; EE_top = 0x200; text_start = 0x5; break;		//For normal strings
default: wdt_enable(WDTO_1S); while(1);break;}
Text_Press_P_R_or_H;

while(1){
op_code = waitforkeypress();
if ((op_code == 'P')||(op_code == 'p'))  {Atmel_powerup; Atmel_config(Prog_enable_h, 0); break;}}

Text_SendEorAOK

while(1){
op_code = waitforkeypress();
switch (op_code){
case 'e':	if(target==328){EE_top = 0x400; text_start = 0x200;}Prog_Target_EEPROM(); break;		//Place backup strings between 0x200 and 0x400
case 'E':  Prog_Target_EEPROM(); break;//No return from here

case 'D':
sendString("Reset target EEPROM! D or AOK");newline();
if(waitforkeypress() == 'D'){
sendString("10 sec wait");
if(target == 168){for (int m = 0; m <= 0x1FF;m++)
{Read_write_mem('I', m, 0xFF);}}
if(target == 328){for (int m = 0; m <= 0x3FD;m++)			//Do not delete Cal data
{Read_write_mem('I', m, 0xFF);}}
sendString(" Done\r\n");}wdt_enable(WDTO_60MS); while(1);break;

case 'p': 
case 'P': sendString ("8MHz");break;
case 'x':
case 'X': wdt_enable(WDTO_60MS); while(1);break;
default: break;} 
if ((op_code == 'P') || (op_code == 'p')) break;}		

temp_char = (Atmel_config(Chip_erase_h, 0)); 
Atmel_config(write_fuse_bits_H_h,fuse_H );
if(fuse_H == 0xD0){Atmel_config(write_extended_fuse_bits_h,0xFD );
Atmel_config(write_fuse_bits_h,0xC2 );}
if(fuse_H == 0xD7){Atmel_config(write_extended_fuse_bits_h,0xFF );
Atmel_config(write_fuse_bits_h,0xE2 );}

Atmel_config(write_lock_bits_h,0xEB );

prog_counter=0; record_length_old=0;  cmd_counter = 0; 
Flash_flag = 0;  HW_address = 0;  section_break = 0; orphan = 0; 

newline();Text_Send_Hex

UCSR0B |= (1<<RXCIE0); sei();

Program_Flash();
Verify_Flash();  
Read_write_mem('I', 0x3F9, 1); 				//EXTRA LINE: See notes above

newline();
sendString (Version);newline();
Text_ATMEGA168_configBits; newline();  
sendHex(16, Atmel_config(read_extended_fuse_bits_h, 0));
sendHex(16, Atmel_config(read_fuse_bits_H_h,0));	//Target config bits
sendHex(16, Atmel_config(read_fuse_bits_h, 0));
sendHex(16, Atmel_config(read_lock_bits_h, 0));

Text_on_chip_cal_bit
if (cal_factor==1) sendHex(16, OSCCAL);
sendHex(16, OSCCAL);

newline();
Text_Hex_file_size
sendHex(10,cmd_counter); sendSpace();sendString("d'loaded:"); sendSpace();
sendHex(10,prog_counter); sendString(" in:"); sendSpace();
sendHex(10,read_ops); sendString(" out");newline();

sendString("Power cycle then press -r- to auto cal the target device.\r\n");

wdt_enable(WDTO_60MS); while(1);
return 1;  }


/***************************************************************************************************************************************************/
ISR(USART_RX_vect){
	unsigned char Rx_Hex_char=0;
	unsigned char Rx_askii_char;
	int local_pointer;
		
Rx_askii_char = receiveChar();
switch (Rx_askii_char){
case '0':  Rx_Hex_char = 0x00; break;						//Convert askii chars received from hex file to binary digits
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
case 0x0:  	break;											//Detect -:- at start of new line
case 0x1: 	tempInt1 = Rx_Hex_char<<4;  break;				//Acquire first digit 
case 0x2: 	tempInt1 += Rx_Hex_char;  						//Acquire second digit and combine with first to obtain number of commands in line
			char_count = 9 + ((tempInt1) *2); 				//Calculate line length in terms of individual characters
			local_pointer = w_pointer++; 					//Update pointer to array "store"
			store[local_pointer] = tempInt1; break;			//Save the number of commands in the line to the array  
case 0x3: 	tempInt1 = Rx_Hex_char<<4;  break;				//Next 4 digits give the address of the first command in the line
case 0x4:	tempInt1 += Rx_Hex_char; 
			tempInt1=tempInt1<<8; break;					//Acquire second digit and combine it with first 
case 0x5:	tempInt1 += Rx_Hex_char<<4;  break;			//Continue for third digit
case 0x6: 	tempInt1 += Rx_Hex_char; 						//Acquire final digit and caculate address of next command 
			local_pointer = w_pointer++; 					//Update pointers to array "store"
			store[local_pointer] = tempInt1; break;			//Save address of next command to array "store"
case 0x7: 	break;											//chars 7 and 8 are not used
case 0x8: 	break;
default: 	break;}

if ((counter > 8)&&(counter < char_count)){				//Continue to acquire, decode and store commands
if ((counter & 0x03) == 0x01){tempInt1 = Rx_Hex_char<<4;}	//Note: Final two chars at the end of every line are ignored
if ((counter & 0x03) == 0x02)  {tempInt1 += Rx_Hex_char;}
if ((counter & 0x03) == 0x03)  {tempInt2 = Rx_Hex_char<<4;}
if ((counter & 0x03) == 0x0)  	{tempInt2+= Rx_Hex_char; 
								tempInt2=tempInt2<<8;tempInt1+=tempInt2;
local_pointer = w_pointer++; 	
store[local_pointer] = tempInt1; cmd_counter++;}}

counter++;
w_pointer = w_pointer & 0b00011111;	}  						//Overwrites array after 32 entries



/***************************************************************************************************************************************************/
void Program_Flash (void){

new_record();  												//Start reading first record which is being downloaded to array "store" 
start_new_code_block(); 									//Initialise new programming block (usually starts at address zero but not exclusivle so)
Program_record();											//Coppy commands from array "store" to the page_buffer														
			
		
while(1){		
new_record();												//Continue reading subsequent records
if (record_length==0)break; 								//Escape when end of hex file is reached


if (Hex_address == HW_address){								//Normal code: Address read from hex file equals HW address and lines contains 8 commands
switch(short_record){
case 0: if (space_on_page == (PageSZ - line_offset))		//If starting new page
			{page_address = (Hex_address & PAmask);}		//get new page address
			break;

case 1:	start_new_code_block();								//Short line with no break in file (often found in WinAVR hex files).
		short_record=0;break;}}
		
		
if(Hex_address != HW_address){								//Break in file
	if (section_break){										//Section break: always found when two hex files are combined into one 										
		if((Flash_flag) && (!(orphan)))
		{write_page_SUB(page_address);}						//Burn contents of the partially full page buffer to flash
		if(orphan) 
		write_page_SUB(page_address + PageSZ);} 			//Burn outstanding commands to the next page in flash			
		
	if(page_break)											//In practice page breaks and short jumps are rarely if ever found											
		{if((Flash_flag) && (!(orphan)))					//Burn contents of the partially filled page buffer to flash
		{write_page_SUB(page_address);}														
		orphan = 0;}
		
	start_new_code_block();									//A new code block is always required where there is a break in the hex file.
	short_record=0;}
		
Program_record();}											//Continue filling page_buffer
		

cli();	
UCSR0B &= (~(1<<RXCIE0));									//download complete, disable UART Rx interrupt
LEDs_off;				
while(1){if (isCharavailable(1)==1)receiveChar();
		else break;}										//Clear last few characters of hex file
	
if((Flash_flag) && (!(orphan)))
{write_page_SUB(page_address);}								//Burn final contents of page_buffer to flash
if(orphan) {write_page_SUB(page_address + PageSZ);}}




/***************************************************************************************************************************************************/

void Verify_Flash (void){			//short version
int   star_counter;
signed int phys_address;
char offset=0;
read_ops = 0;
Hex_cmd = 0;
star_counter = 0;
phys_address = 0;
while(1){
Hex_cmd = Read_write_mem('L',phys_address, 0x0);							//H for test mode
Hex_cmd = (Hex_cmd<<8) + (Read_write_mem('H',phys_address, 0x0)); 
phys_address++; 
star_counter++;       
if (phys_address == FlashSZ)break;
if (Hex_cmd != 0xFFFF){
read_ops++; if(read_ops >= prog_counter) offset = 4;}
if (!( star_counter - 200)){sendChar('*' + offset);star_counter = 0;}}}




/***************************************************************************************************************************************************/
void Prog_on_chip_EEPROM(void){
char User_response, next_char, text;
int EEP_read_address=0,EEP_write_address = 0;

sendString("Set baud rate to 2.4k then AK\r\n");	
USART_init(1,160);
User_prompt;
//waitforkeypress();
sendString("Send message file\r\n");



/***************Strings in the .txt file are saved to EEPROM**************************/

while(1){next_char = waitforkeypress();								//wait for first character from file
if ((next_char != '\r') && (next_char != '\n'))						//Ignore leading carriage returns
break;}							
Text_to_EEPROM(&EEP_write_address, next_char);								//save first letter to EEPROM

while(EEP_write_address < 0x1F6) 											//Exits before cal bytes can be overwritten
	{if(isCharavailable(2)) 											//returns 1 if a new letter is available (0 at the end of the file) 
	{text = receiveChar(); 												//Temporary storage

	switch (text){														//Test the new character	
		case '\r':														//If it is '\r' and or '\n' 
		case '\n':														//ignore it or replace it with with a single '\0'
		if(next_char == '\0')break; 
		else 	{next_char = '\0'; 
				Text_to_EEPROM(&EEP_write_address, next_char);}break;
		default : 	next_char = text; 									//save the letter
					Text_to_EEPROM(&EEP_write_address, next_char);			//increments the write address
					break;}

	}else break; }														//End of file reached

if(EEP_write_address == 0x1F6)												//If text file was too long
{Text_to_EEPROM(&EEP_write_address, '\0');									//Place '\0' in 0x1F6 to terminate the string
binUnwantedChars_dot();}												//Send dots to pc to indicate lost characters


/****************Echo text file to screen with the address of each string**********************/

sendHex(16, EEP_read_address); sendChar('\t');							//Send address of first line of text
do{																		//Read back text one line at a time
while(1){text = Text_from_EEPROM(&EEP_read_address);						//Increments the read address
if(text)sendChar(text); else break;} 								//When '\0' is detected start next line
newline();sendHex(16,EEP_read_address); 									//Send address of next line
sendChar('\t');}
while(EEP_read_address < EEP_write_address);									//Exit when read address equals write address


sendString("\r\nBAUD RATE 57.6k!!\r\npress any key to continue\r\n");

USART_init(0,16);

waitforkeypress();
wdt_enable(WDTO_60MS); while(1);}




/****************************************************/
void Text_to_EEPROM(int*w_a, char byte){					//on-chip EEPROM
eeprom_write_byte((uint8_t*)(*w_a),byte); 					//macro provided by winavr
*w_a = *w_a + 1;}



/****************************************************/
char Text_from_EEPROM(int*r_a){
return eeprom_read_byte((uint8_t*)((*r_a)++));}			//macro provided by winavr



/*********************************************************************/
void binUnwantedChars_dot (void){char bin_char;
while(1){if (isCharavailable(5)==1){bin_char = receiveChar();sendChar('.');}else break;}newline();}




/*

void Verify_Flash (void){


int  line_counter = 0, print_line = 0;						//Controls printing of hex file													
int line_no;												//Refers to the .hex file
signed int phys_address;									//Address in flash memory
signed int prog_counter_mem;								//Initialised with size of .hex file used for programming

phys_address = 0;  read_ops=0; 
line_no = 0; prog_counter_mem = prog_counter; 


while(1){ if(!(prog_counter_mem))break;					//print out loop starts here, exit when finished
		
while(1) {													//Start reading the flash memory searching for the next hex command

Hex_cmd = Read_write_mem('L',phys_address, 0x0);							
Hex_cmd = (Hex_cmd<<8) + (Read_write_mem('H',phys_address, 0x0)); 
phys_address++;        
if (phys_address == FlashSZ)break;							//No more memory? Quit if yes
if (Hex_cmd != 0xFFFF) break;								//If the hex command is 0xFFFF remain in this loop otherwise exit.
LEDs_on;}

LEDs_off;
if (phys_address == FlashSZ)break;							//Exit when there is no more flash to read

if ((print_line == 0)  && (!(line_no%10)))
sendChar('*');												//Print out of hex file not required

read_ops++;													//Value to be sent to PC for comparison with the hex filer size
prog_counter_mem--;											//"prog_counter_mem" decrements to zero when the end of the file is reached


for(int m=0; m<7; m++){    								//Read the next seven locations in the flash memory   

Hex_cmd = Read_write_mem('L',phys_address, 0x0);				
Hex_cmd = (Hex_cmd<<8) + (Read_write_mem('H',phys_address, 0x0)); 
phys_address++;
if(Hex_cmd == 0xFFFF)break;									//Read 0xFFFF: return to start of print out loop
prog_counter_mem--;

if ((print_line) &&  (!(line_counter%2))) {LEDs_on;} else {LEDs_off;} 			       

read_ops++;

if(phys_address==FlashSZ)break;}

line_no++;
if (phys_address == FlashSZ)break;}

LEDs_off;
newline();newline(); }

*/




