

/**********************************************************************************************************/
void new_record(void){int Local_r_pointer;								//Start of new record from the hex file

while (r_pointer == w_pointer);											//Wait for new entry in array "store"
Local_r_pointer = r_pointer;
record_length =  store[Local_r_pointer]; 								//Obtain the length of the new record
inc_r_pointer;  														//Increment the read pointer
Count_down = record_length/2; 											//Initialised to record lenghth (in commands) 
if (record_length_old < 0x10)short_record=1;							//Check for "record length" (<8 commands)
record_length_old = record_length;										//Save record length

while (r_pointer == w_pointer);											//Wait for next entry in array "store"
Local_r_pointer = r_pointer;
Hex_address  =  (store[Local_r_pointer]); 								//Get the address of the first command in the new record
inc_r_pointer;  														//Increment the read pointer
Hex_address  = Hex_address/2;											//Convert address from byte to word value
prog_led_control++;

if (Hex_address == HW_address)orphan = 0;								//New record follows on immediately from the old
else{if (Hex_address == 0);											//Start of hex file: address is zero
		else{if ((Hex_address & PAmask) > (page_address + PageSZ))		//Long jump: At least one page is unused
				{section_break=1;page_break=0;}
			if((Hex_address & PAmask) == (page_address + PageSZ))		//Page jump: Jump to next page without filling current one
				{section_break=0;page_break=1; }
			if ((Hex_address & PAmask) == page_address)				//Short jump: Jump within page
				{section_break=0;page_break=0; orphan = 0;}}}

if(prog_led_control & 0b00001000) {LEDs_on;}	
else {LEDs_off;}}  



/**********************************************************************************************************/
void start_new_code_block(void){
HW_address = Hex_address;												//Initialise HW_address
page_address = (Hex_address & PAmask);									//Obtain page address
write_address = Hex_address - page_address;								//Initialise write_address: zero to Page size -1
page_offset = write_address;											//"page_offset" is the initial value of "write_address"											
line_offset = Hex_address & 0x0007;										//Not all lines start with addresses that are multiples of 8
space_on_page = (PageSZ - page_offset);}								//Initialise "space_on_page"






/**********************************************************************************************************/
void Program_record(void){

while(Count_down){ 														//Initially contains the number of commands in the line
Count_down--;															//Decrement "count_down" each time a command is written to the page buffer
Flash_flag = 1;  														//Indicates that page buffer containes commands that will need burning to flash 
copy_cmd_to_page_buffer();  												 
write_address++;
HW_address++;
space_on_page--;
prog_counter++;       
if (write_address == PageSZ){											//If page_buffrer is now full:
Read_write_mem('W', page_address, 0x0);									//Burn contents of page buffer to flash
Flash_flag = 0;															//Buffer now contains no data to burn to flash
write_address = 0;														//"while loop" continues if there is a line offset
space_on_page = PageSZ;
page_offset = line_offset;
if (line_offset) orphan = 1;}}}										//One or more commands in current record will be on next page


/**********************************************************************************************************/
void copy_cmd_to_page_buffer(void){
get_next_hex_cmd();
Load_page('L', write_address, Hex_cmd & 0x00FF); 
Load_page('H', write_address, (Hex_cmd >> 8));}


/**********************************************************************************************************/
void get_next_hex_cmd(void){int Local_r_pointer;
while (r_pointer == w_pointer);
Local_r_pointer = r_pointer;
Hex_cmd = store[Local_r_pointer];
inc_r_pointer;}



/**********************************************************************************************************/
void write_page_SUB(signed int page_address){
Read_write_mem('W',  page_address, 0x0);
Flash_flag = 0;}



/***********************************************************/
char receive_byte_with_Ack(void){
char byte;
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);		//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));						//Wait for interrupt
byte = TWDR;
return byte;}

/***********************************************************/
char receive_byte_with_Nack(void){
char byte;
TWCR = (1 << TWEN) | (1 << TWINT);		//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));						//Wait for interrupt
byte = TWDR;
return byte;}





/*
void Verify_Flash_99 (void){
int line_no_mem;
int  print_out = 0, print_counter, star_counter = 0;
unsigned int Verify_address;
signed int phys_address, FF_cmd_counter;
signed int prog_counter_mem;
int data_counter;
char print_out_mode = 0; //temp_char;
char skip_lines[4];

FF_cmd_counter = 0;

Text_int_0_FF; sendString("   ");	
print_out = 0; data_counter = 0;
skip_lines[0] = '0';		//MSB
skip_lines[1] = waitforkeypress();
skip_lines[2] = '\0';

if (isCharavailable(100)){skip_lines[0] = skip_lines[1];
skip_lines[1] = receiveChar();}	binUnwantedChars();		//NEW CMD
print_out = askiX2_to_hex(skip_lines);
sendHex (16,print_out); sendString("   ");

if (print_out == 0); else {
sendString("1/2?\r\n");	
print_out_mode =  waitforkeypress(); binUnwantedChars();		//NEW CMD
newline();}

Verify_address=0;  phys_address = 0;  read_ops=0; line_no_mem = line_no-2;
line_no = 0; prog_counter_mem = prog_counter; print_counter = 0;

while(1){ if(!(prog_counter_mem))break;		//print out loop starts
FF_cmd_counter = 0;
while(1) {
Hex_cmd = 0;
Hex_cmd = Read_write_mem('L',phys_address, 0x0);							//H for test mode
Hex_cmd = (Hex_cmd<<8) + (Read_write_mem('H',phys_address, 0x0)); 
phys_address++;        
if (phys_address == FlashSZ)break;
if (Hex_cmd != 0xFFFF) break;
LEDs_on;
Verify_address++;FF_cmd_counter++; 
//if(FF_cmd_counter >= FF_cmd_counter_limit){Verify_address = 0x3800; phys_address = 0x3800; FF_cmd_counter = 0; }
}
LEDs_off;
if (phys_address == FlashSZ)break;

if ((print_out == 0)  && (!( star_counter - 10))){sendChar('*');star_counter = 0;}

if(print_out && ((!(line_no))||(print_counter == print_out)))			
{newline(); sendHex (16, Verify_address*2);
sendString("   "); if(print_out_mode == '1'){Print_hex_file;} else sendHex (16, Hex_cmd);
}
read_ops++;
prog_counter_mem--;

{char m; for (m=0; m<7; m++){       
Hex_cmd = Read_write_mem('L',phys_address, 0x0);								//H for test mode
Hex_cmd = (Hex_cmd<<8) + (Read_write_mem('H',phys_address, 0x0)); 
if (Hex_cmd != 0xFFFF){
Verify_address++; prog_counter_mem--;
if(Verify_address & 0x0040) {
LEDs_on;	}				 
else {LEDs_off;} 			
phys_address++;         
if(print_out && ((!(line_no))||(print_counter == print_out))){if(print_out_mode == '1'){Print_hex_file;} else sendHex (16, Hex_cmd);
{five_msec_delay;}}
read_ops++;}
if(phys_address==FlashSZ)break;}}
if(print_counter == print_out)print_counter = 0;
line_no++;
print_counter++; star_counter++;
if (phys_address == FlashSZ)break;
Verify_address++; }//print out loop ends
LEDs_off;
newline();}*/








/*
void Program_Flash (void){
char temp_char=0;
UCSR0B |= (1<<RXCIE0);sei();



new_record();  short_line=0;
start_new_code_block();
Program_record();			
		
while(1){		//loop1
new_record();
if (line_length==0)break; 	//ISR variable

if ((Hex_address == PIC_address) && (!(short_line))){	//normal ongoing code block
if (space_on_page == (PageSZ - offset)){page_address = (Hex_address & PAmask);}
Program_record();}	

else{if(Hex_address != PIC_address)//loop 2
	{//normal break	loop 3 
	if (section_break){//PAGE Adress increases by at least 0x40
	if((Flash_flag) && (!(orphan)))write_page_SUB(page_address);    //+0x20 for offset pages
	if(orphan) write_page_SUB(page_address + PageSZ);}   //0x20??
		
	else{//loop 4
		if(page_break){if((Flash_flag) && (!(orphan))) write_page_SUB(page_address);
						orphan = 0; }}//  break within page loop 4
	}//loop 3
	start_new_code_block(); Program_record();  if(short_line)short_line=0;//short_line no break
	}	//loop 2
	}	//loop 1
	
cli();  	
UCSR0B &= (~(1<<RXCIE0));	// clear UART interrupt flag
LEDs_off;				
while(1){if (isCharavailable(2)==1)temp_char = receiveChar();else break;}	
	
if((Flash_flag) && (!(orphan))){write_page_SUB(page_address);}
if(orphan) {write_page_SUB(page_address + PageSZ);}}	
*/

/*ISR(USART_RX_vect){
	unsigned char tempChar=0;
	unsigned char tempChar1;
	int local_pointer;
		
tempChar1 = receiveChar();
switch (tempChar1){
case '0':  tempChar = 0x00; break;
case '1':  tempChar = 0x01; break;
case '2':  tempChar = 0x02; break;
case '3':  tempChar = 0x03; break;
case '4':  tempChar = 0x04; break;
case '5':  tempChar = 0x05; break;
case '6':  tempChar = 0x06; break;
case '7':  tempChar = 0x07; break;
case '8':  tempChar = 0x08; break;
case '9':  tempChar = 0x09; break;
case 'A':  tempChar = 0x0A; break;
case 'B':  tempChar = 0x0B; break;
case 'C':  tempChar = 0x0C; break;
case 'D':  tempChar = 0x0D; break;
case 'E':  tempChar = 0x0E; break;
case 'F':  tempChar = 0x0F; break;
case ':':  counter = 0;  break;
default: break;}

switch (counter){
case 0x0:  line_no++;  break;
case 0x1: tempInt1 = tempChar<<4;  break;
case 0x2: tempInt1 += tempChar;  char_count = 9 + ((tempInt1) *2);  
			local_pointer = w_pointer++; store[local_pointer] = tempInt1; break;
case 0x3: tempInt1 = tempChar<<4;  break;
case 0x4: tempInt1 += tempChar; tempInt1=tempInt1<<8; break;
case 0x5: tempInt1 += tempChar<<4;  break;
case 0x6: tempInt1 += tempChar;  local_pointer = w_pointer++; store[local_pointer] = tempInt1; break;
case 0x7: break;
case 0x8:  break;
default: break;}

if ((counter > 8)&&(counter < char_count)){
if ((counter & 0x03) == 0x01){tempInt1 = tempChar<<4;}
if ((counter & 0x03) == 0x02)  {tempInt1 += tempChar;}
if ((counter & 0x03) == 0x03)  {tempInt2 = tempChar<<4;}
if ((counter & 0x03) == 0x0)  {tempInt2+= tempChar; tempInt2 = tempInt2<<8;tempInt1+=tempInt2;
local_pointer = w_pointer++; store[local_pointer] = tempInt1; cmd_counter++;}}

counter++;
w_pointer = w_pointer & 0b00111111;	}*/



/*

void Verify_Flash_99 (void){
int line_no_mem;
int  print_out = 0, print_counter, star_counter = 0;
unsigned int Verify_address;
signed int phys_address, FF_cmd_counter;
signed int prog_counter_mem;
int data_counter;
char print_out_mode = 0; //temp_char;
char skip_lines[4];

FF_cmd_counter = 0;

Text_int_0_FF; sendString("   ");	
print_out = 0; data_counter = 0;
skip_lines[0] = '0';		//MSB
skip_lines[1] = waitforkeypress();
skip_lines[2] = '\0';

if (isCharavailable(100)){skip_lines[0] = skip_lines[1];
skip_lines[1] = receiveChar();}	binUnwantedChars();		//NEW CMD
print_out = askiX2_to_hex(skip_lines);
sendHex (16,print_out); sendString("   ");

if (print_out == 0); else {
sendString("1/2?\r\n");	
print_out_mode =  waitforkeypress(); binUnwantedChars();		//NEW CMD
newline();}

Verify_address=0;  phys_address = 0;  read_ops=0; line_no_mem = line_no-2;
line_no = 0; prog_counter_mem = prog_counter; print_counter = 0;

while(1){ if(!(prog_counter_mem))break;		//print out loop starts
FF_cmd_counter = 0;
while(1) {
Hex_cmd = 0;
Hex_cmd = Read_write_mem('L',phys_address, 0x0);							//H for test mode
Hex_cmd = (Hex_cmd<<8) + (Read_write_mem('H',phys_address, 0x0)); 
phys_address++;        
if (phys_address == FlashSZ)break;
if (Hex_cmd != 0xFFFF) break;
LEDs_on;
Verify_address++;FF_cmd_counter++; 
if(FF_cmd_counter >= FF_cmd_counter_limit){Verify_address = 0x3800; phys_address = 0x3800; FF_cmd_counter = 0; }
}
LEDs_off;
if (phys_address == FlashSZ)break;

if ((print_out == 0)  && (!( star_counter - 10))){sendChar('*');star_counter = 0;}

if(print_out && ((!(line_no))||(print_counter == print_out)))			
{newline(); sendHex (16, Verify_address*2);
sendString("   "); if(print_out_mode == '1'){Print_hex_file;} else sendHex (16, Hex_cmd);
}
read_ops++;
prog_counter_mem--;

{char m; for (m=0; m<7; m++){       
Hex_cmd = Read_write_mem('L',phys_address, 0x0);								//H for test mode
Hex_cmd = (Hex_cmd<<8) + (Read_write_mem('H',phys_address, 0x0)); 
if (Hex_cmd != 0xFFFF){
Verify_address++; prog_counter_mem--;
if(Verify_address & 0x0040) {
LEDs_on;	}				 
else {LEDs_off;} 			
phys_address++;         
if(print_out && ((!(line_no))||(print_counter == print_out))){if(print_out_mode == '1'){Print_hex_file;} else sendHex (16, Hex_cmd);
{five_msec_delay;}}
read_ops++;}
if(phys_address==FlashSZ)break;}}
if(print_counter == print_out)print_counter = 0;
line_no++;
print_counter++; star_counter++;
if (phys_address == FlashSZ)break;
Verify_address++; }//print out loop ends
LEDs_off;
newline();}




void waitforchar(void){unsigned char n=255; 																																			
while (!(UCSR0A & (1 << RXC0))) {n--; if(n==0) break;}}



void new_record(void){int B_int_r_pointer;			
next_char; 
B_int_r_pointer = r_pointer;
line_length =  store[B_int_r_pointer]; inc_r_pointer; 
Count_down = line_length/2; 
if (line_length_old < 0x10)short_line=1;
line_length_old = line_length;
next_char; 
B_int_r_pointer = r_pointer;
Hex_address  =  (store[B_int_r_pointer]); inc_r_pointer; 
Hex_address  = Hex_address/2;
line_counter++;
if (Hex_address == PIC_address) orphan = 0;
else{if (Hex_address == 0);
		else{if ((Hex_address & PAmask) > (page_address + PageSZ)){section_break=1;page_break=0;}
			if((Hex_address & PAmask) == (page_address + PageSZ)){section_break=0;page_break=1; }
			if ((Hex_address & PAmask) == page_address){section_break=0;page_break=0; orphan = 0;}}}
if(
 & 0b00001000) {LED_2_on;}	 
else {LED_2_off;}} 



void start_new_code_block(void){signed char page_offset;
PIC_address = Hex_address;
page_address = (Hex_address & PAmask);		//0x0FF0????
write_address = Hex_address - page_address;
page_offset = write_address;
line_offset = Hex_address & 0x0007;
offset = page_offset;	//	Hex_address & 0x0007;
space_on_page = (PageSZ - page_offset);}



void Program_record(void){
unsigned char Dummy_byte;
while(Count_down){ 
Count_down--;
Flash_flag = 1;   
decode_cmd_write_to_flash_SUB();   
write_address++;
PIC_address++;
space_on_page--;
prog_counter++;       
if (write_address == PageSZ){
Dummy_byte = Read_write_mem('W', page_address, 0x0);
Flash_flag = 0;
write_address = 0;
space_on_page = PageSZ;
offset = line_offset; 
if (offset) orphan = 1;}}}



void decode_cmd_write_to_flash_SUB(void){
decode_HEX_cmd_SUB();
Load_page('L', write_address, Hex_cmd & 0x00FF); 
Load_page('H', write_address, (Hex_cmd >> 8));}



void copy_cmd_to_page_buffer(void){
get_next_hex_cmd();
Load_page('L', write_address, Hex_cmd & 0x00FF); 
Load_page('H', write_address, (Hex_cmd >> 8));}


void get_next_hex_cmd(void){int Local_r_pointer;/////////
while (r_pointer == w_pointer);
Local_r_pointer = r_pointer;
Hex_cmd = store[Local_r_pointer];
inc_r_pointer;}





void decode_HEX_cmd_SUB(void){int A_int_r_pointer;
next_char;
A_int_r_pointer = r_pointer;
Hex_cmd = store[A_int_r_pointer];inc_r_pointer;}



void write_page_SUB(signed int page_address){
char Dummy_byte;
Dummy_byte = Read_write_mem('W',  page_address, 0x0);
Flash_flag = 0;}



char receive_byte_with_Ack(void){
char byte;
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);		//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));						//Wait for interrupt
byte = TWDR;
return byte;}



char receive_byte_with_Nack(void){
char byte;
TWCR = (1 << TWEN) | (1 << TWINT);		//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));						//Wait for interrupt
byte = TWDR;
return byte;}*/
