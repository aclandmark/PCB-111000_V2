
void binUnwantedChars_dot (void);
void Text_to_EEPROM(int*, char);
char Text_from_EEPROM(int*);
long compute_error(char, char);




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
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);						//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));										//Wait for interrupt
byte = TWDR;
return byte;}



/***********************************************************/
char receive_byte_with_Nack(void){
char byte;
TWCR = (1 << TWEN) | (1 << TWINT);										//Set Ack enable and clear interrupt
while (!(TWCR & (1 << TWINT)));										//Wait for interrupt
byte = TWDR;
return byte;}



/***************************************************************************************************************************************************/
void Prog_on_chip_EEPROM(void){
char next_char, text,char_counter;
int EEP_read_address=0,EEP_write_address = 0;

newline();sendString ("BR 2k4 then AK");newline();						//Text_Baud_Rate_L;
USART_init(1,160);
waitforkeypress();
sendString ("Text file?");newline();									//Text_message_file;

if ((text = waitforkeypress()) == '"');
else
{while(1){if(isCharavailable(2))
text = receiveChar(); 
if (text == '"') break;}}	


/***************Strings in the .txt file are saved to on-chip EEPROM**************************/

while(1){next_char = waitforkeypress();								//wait for first character from file
if ((next_char != '\r') && (next_char != '\n'))						//Ignore leading carriage returns
break;}							
Text_to_EEPROM(&EEP_write_address, next_char);							//save first letter to EEPROM

while(EEP_write_address < 0x1FA) 										//Exits before cal bytes can be overwritten
	{if(isCharavailable(2)) 											//returns 1 if a new letter is available (0 at the end of the file) 
	{text = receiveChar(); 												//Temporary storage

	switch (text){														//Test the new character	
		case '\r':														//If it is '\r' and or '\n' 
		case '\n':														//ignore it or replace it with with a single '\0'
		if(next_char == '\0')break; 
		else 	{next_char = '\0'; 
				Text_to_EEPROM(&EEP_write_address, next_char);}break;
		case '"': break;												//Ignore the second " symbol
		default : 	next_char = text; 									//save the letter
					Text_to_EEPROM(&EEP_write_address, next_char);		//increments the write address
					break;}

	}else break; }														//End of file reached

if(EEP_write_address == 0x1FA)											//If text file was too long
{Text_to_EEPROM(&EEP_write_address, '\0');								//Place '\0' in 0x1F6 to terminate the string
binUnwantedChars_dot();}												//Send dots to pc to indicate lost characters


/****************Echo text file to screen with the address of each string**********************/

sendHex(16, EEP_read_address); sendChar('\t');							//Send address of first line of text
do{char_counter = 0;													//Read back text one line at a time
while(char_counter < 150)
{text = Text_from_EEPROM(&EEP_read_address);							//Increments the read address
char_counter += 1;
if(text)sendChar(text); else break;} 									//When '\0' is detected start next line
newline();sendHex(16,EEP_read_address); 								//Send address of next line
sendChar('\t');}
while(EEP_read_address < EEP_write_address);							//Exit when read address equals write address

Text_Baud_Rate_H;
USART_init(0,16);
waitforkeypress();
wdt_enable(WDTO_60MS); while(1);}




/****************************************************/
void Text_to_EEPROM(int*w_a, char byte){								//on-chip EEPROM
eeprom_write_byte((uint8_t*)(*w_a),byte); 								
*w_a = *w_a + 1;}



/****************************************************/
char Text_from_EEPROM(int*r_a){
return eeprom_read_byte((uint8_t*)((*r_a)++));}						



/*********************************************************************/
void binUnwantedChars_dot (void){char bin_char;
while(1){if (isCharavailable(5)==1)
{bin_char = receiveChar();sendChar('.');}else break;}newline();}




/*************************************************************/
void Minimise_error(int limit, char *counter_1, char *counter_2, long *error_mag, char *OSCCAL_mem)
{while(*counter_2 < 20){ OSCCAL = *counter_1; *error_mag = compute_error(2,0); 
		if(*error_mag < limit)break;
		*counter_1 -= 1;
		*counter_2 +=1;}
		if (*counter_2 < 20)*OSCCAL_mem = OSCCAL;else OSCCAL = *OSCCAL_mem;}



/************************************************************************************************/		
void initialise_timers_for_cal_error(void){
		TCNT1=0;TCCR1B = 0;												//Reset and halt T1
		TCCR2B =  0x0;	while(ASSR & (1 << TCR2BUB));					//Halt T2
		TCCR2A = 0; while(ASSR & (1 << TCR2AUB));						//Reset T2 
		TCNT2=0; while(ASSR & (1 << TCN2UB));	}						//Reset TCNT2

/************************************************************************************************/		
		
		void start_timers_for_cal_error(void)
		{TCCR2B = 1; 	
		while(ASSR & (1 << TCR2BUB));
		TCCR1B = 1;}	



/************************************************************************************************/		
long compute_error(char local_cal_mode, char sign)						//char OSCCAL_test_value)		
		{long error;
		char Num_1, Num_2;
		
		if (local_cal_mode == 1){Num_1 = 2; Num_2 = 1;}
		if (local_cal_mode == 2){Num_1 = 6; Num_2 = 4;}
		if (local_cal_mode == 5){Num_1 = 15; Num_2 = 10;}
		
		EA_counter = 0;													//Compute error for each value of OSCCAL 10 times
		error_SUM = 0;
		while(EA_counter < Num_1);EA_counter = 0;
		error = error_SUM;
		if ((!sign) && (error < 0)) error *= (-1);
		return error/Num_2;}
		
		

/*****************************************************************************************************/
void auto_cal_168 (void){
char dot_counter=0, counter_1, counter_2;		
char OSCCAL_WV, OSCCAL_mem = 0;
long  error_mag; 
int limit;

DDRC |= (1 << PC5);
PORTC ^= (1 << PC5);	

ASSR = (1 << AS2); 		
TCNT2 = 0;				
Timer_T0_10mS_delay_x_m(100);

		sei();
		
		cal_mode = 2;
				
		Get_ready_to_calibrate;
		
		counter_1 = 0xF1;
		while(1){
		counter_1 -= 1;
		OSCCAL = counter_1; error_mag = compute_error(2,0); 
		if(counter_1 > 0xE8)continue; 
		dot_counter += 1; dot_counter = dot_counter%5; 
		if(!(dot_counter)){sendChar('.');PORTC ^= (1 << PC5);}
		if(error_mag < 1000)break;}
		
		OSCCAL_mem = OSCCAL;
		counter_2 = 0;
		
		limit = 1000;
		for(int m = 1; m <= 9; m++){
		limit -= 100;
		
		Minimise_error(limit, &counter_1, &counter_2, &error_mag, &OSCCAL_mem);}
		
		error_mag = compute_error(2,0);
		OSCCAL_WV = OSCCAL;	
		close_calibration;
		newline();
				
	eeprom_write_byte((uint8_t*)0x1FE, OSCCAL_WV); 
	eeprom_write_byte((uint8_t*)0x1FF, OSCCAL_WV);
	
	newline();sendString("UC\t");				
	sendHex(10,eeprom_read_byte((uint8_t*)0x1FE));
	sendString ("    ");
	sendHex(10,eeprom_read_byte((uint8_t*)0x1FF));
	sendString("\tError  ");	
	sendHex(10,error_mag);
	
	if(error_mag < 750)sendString("\tOK");newline();}	





