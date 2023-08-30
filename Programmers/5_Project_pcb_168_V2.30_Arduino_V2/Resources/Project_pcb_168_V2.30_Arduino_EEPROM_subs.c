

/*
Because of the time required to burn data to EEPROM a file cannot simply be downloaded at 57.6kB
The speed can be reduced considerably or the file can be downloaded several times.
It was felt that the latter approach generated less hassle for the user and has been adopted here.
Each time the file is downloaded in full but only 256 chars or less are stored to an array in SRAM.
When this array is full its contents are copied to EEPROM.

The text/data file contains several sections.  Explanatory text if required appears first.
Strings are then placed between a pair of -"- characters.
After this data can be apended and all text such as data names for example is ignored.

The following variables are used to keep a track of this process:


"EEPROM_buffer"	Array into which file text and data is downloaded before being saved to EEPROM
"EEP_pointer"	and 	"*ptr_EEP_pointer"		Address of next free location in the EEPROM
"file_pointer"	and		"*ptr_file_pointer"		Address in text/data file being downloaded from PC
"array_pointer	and		"*ptr_array_pointer"	Address of next free location in array (temp storrage for file)
"data_counter"	and		"*ptr_data_counter"		Counts data items as they are downloaded	
"DL_flag"		and		"*ptr_DL_flag"			Status of download
"DL_status"		and		"ptr_DL_status"			Status of download

"reservation"	Used to enter the amount of EEPROM required by the application which should not be used for text/data
"op_code_1"		Read, write or delete
"op_code_2"		Specifies format used for reading back numbers from the EEPROM


Note the following bug:		It appears that text strings are always downloaded consistently.
							If numeric data is also stored there seem to be up to 3 consecutive addresses for the first byte.
							The middle address appears to be used most of the time
							But every now and then one of the other adresses is used.
							Obviously this recks the operation of the client program used to read the EEPROM unless it is noticed
							and an appropriate offset of plus or minus 1 added to the data addresses.

*/


void Upload_text(int);
char Download_text(int*,int*,char*,int*, unsigned char*);
char next_char_from_PC(void);
char Download_data(int*,int*, char*, int*,int*, unsigned char*);



/*******************************************************************************************/
void Read_on_chip_EEPROM(int EEPROM_address){
char temp_char, read_counter = 0;
while(1){
temp_char =  eeprom_read_byte((uint8_t*)(EEPROM_address++));
if ((temp_char != '\0') && (++read_counter <= 80)) sendChar(temp_char);
else break;}}






/********************************************************************************************************************************************/
void Prog_Target_EEPROM(void){	
unsigned char  EEPROM_buffer[256];				//265	//130			
int EEP_pointer = 0,   file_pointer = 0,  array_pointer = 0,  data_counter = 0;
char key_press, DL_flag = 0, DL_status, op_code_1, op_code_2;	    
char reservation[4];
int App_reservation;


EEP_pointer = text_start;												//Start saving user strings/data at address 0x05

Text__Press_W_or_R;
while(1){sendString("?  ");	
if (isCharavailable(255)) break;} 										//User prompt  
op_code_1 = receiveChar();								



/********************************************************************************************************************************************/
switch (op_code_1){														//Read the contents of the EEPROM


case 'r':
case 'R':
newline();

if(((byte)Read_write_mem('O', text_start, 0)) ==0xFF){sendString("No Data!\r\n");break;}

Upload_text(EE_top);	

break;		


/********************************************************************************************************************************************/
case 'W':
case 'w':

sendString("\r\nSend text file.");

do 																		//Remain in do-loop untill text all downloaded
	{DL_status = (Download_text(&EEP_pointer,\
	&file_pointer, 	&DL_flag, &array_pointer,\
	EEPROM_buffer));													//Text saved to EEPROM buffer, then copied to EEPROM
	if (!(DL_status))													//If buffer full and space available in EEPROM
		{DL_flag = 0;  													//reset "DL_flag"
		LEDs_off;														//and request subsequent download
		sendString("  Again!");																		
		}
	}while (!(DL_status));												//Exit at end of text (-"- detected) 
LEDs_off;	


if (DL_flag == 2)														//Download data (if any) untill EEPROM buffer is full
	{while (!(Download_data(&EEP_pointer, &file_pointer,\
	&DL_flag, &data_counter,&array_pointer,\
	EEPROM_buffer)))													//Save text and data to EEPROM			
		{LEDs_off;														//If EEPROM buffer fills						
		sendString("  Again!");											//request additional download untill EEPROM is full
		}
	}
binUnwantedChars ();
LEDs_off;

sendString("   AK?\r\n");	binUnwantedChars();		

Read_write_mem('I', 0x0, (EEP_pointer >> 8));  	
Read_write_mem('I', 0x1, (EEP_pointer));

Num_to_PC(16,((byte)Read_write_mem('O', 0x0, 0) << 8) + (byte)Read_write_mem('O', 0x1, 0));

waitforkeypress();
Upload_text(EEP_pointer);   
 
break;}

wdt_enable(WDTO_60MS); while(1);}



/*
Subroutine Download_text() returns	"DL_status" and sets "DL_flag"

Download_text() returns 0	"DL_flag" = 1	Text partially downloaded and saved to EEPROM. Request additional download.
Download_text() returns 1	"DL_flag" = 2	End of text encountered.  Text saved to EEPROM_buffer but not to EEPROM. Call "download_data()"
Download_text() returns 1	"DL_flag" = 3	Text download halted: EEPROM is full.

if "DL_flag" == 2  subroutine "Download_data()" is called to finish saving text to EEPROM and start saving data (if any)

download_data() returns	1	"DL_flag" = 2			File download complete with or without data
download_data() returns	1	"DL_flag" = 3			Data download stopped EEPROM allocation all used up
download_data() returns	0	"DL_flag" = 0			Request additional download
download_data() returns	1	"DL_flag" = 4			Data download stopped EEPROM allocation all used up
*/




/********************************************************************************************************************************************/
char Download_text(int *ptr_EEP_pointer,\
int *ptr_file_pointer,\
char *ptr_DL_flag,\
int *ptr_array_pointer, unsigned char EEPROM_buffer[]){	
int UART_counter = 0;
char text_char;


//Ignore short preliminary text section until the first -"- is encounter which signals the start of the first string to be saved to EEPROM

if ((text_char = waitforkeypress()) == '"');
else
{text_char = waitforkeypress();												//Count characters as they are downloaded from the PC									
if (text_char !=  '\0')UART_counter++;										//Arduino seems to down load several spurious nulls during download.
while(1){text_char = next_char_from_PC(); 
if ((text_char !=  '\0'))UART_counter++; 									//Ignore characters untill a -"- is detected, then place them
if (text_char == '"') break;}}												//in an EEPROM buffer if being downloaded for the first time	


//Save text to EEPROM_buffer untill it is full or second -"- is encountered 
//signalling the end of the text strings and start of data (if any)

	
while(!(*ptr_DL_flag))													//EEPROM buffer full? No: Enter loop 1. Yes: skip Loop 1. 
{if((text_char = next_char_from_PC()) == '\0')continue; 				//Ignore null chars loop back to top of the while loop
else UART_counter++; 													//Loop 1: Acquire text characters and increment "UART_counter"

if(text_char != '"')													//Check for -"-? No: Enter Loop 2: Yes: skip loop 2.
	{if (UART_counter > *ptr_file_pointer)								//Loop 2: Text downloaded for first time? Yes: Enter Loop 3. No: skip loop 3
		{switch (text_char)												//Loop 3: Detect carraige return and new line:
			{case '\n':													//combine them if necessary and replace with '\0'
			case '\r':
				if (EEPROM_buffer[((*ptr_array_pointer)-1)] != '\0')
					{text_char = '\0';}
				else {((*ptr_array_pointer)--); text_char = '\0';}
			default: break;
			}
		EEPROM_buffer[((*ptr_array_pointer)++)] = text_char;			//Place characters as they are received in the EEPROM buffer

		if((*ptr_array_pointer >= RBL) && (text_char != '\r') &&\
				 (text_char != '\0'))									//If EEPROM bufffer fills up part way through a string 
			*ptr_DL_flag = 1;											//set "*ptr_DL_flag" to force exit from Loop 1
		}																//Loop 3																														
	}																	//Loop 2
	else break;															//Second -"- received: Exit Loop 1.	 																					
}																		//Loop 1: Exit when EEPROM buffer is full 

//EEPROM buffer full: This usually means that the text download 
//is not complete

if (*ptr_DL_flag == 1)													//If EEPROM buffer is full: Text download is not complete													
	{binUnwantedChars(); 												//Ignore remaining characters in the EEPROM text/data file														
	*ptr_file_pointer = UART_counter;									//Save the "UART_counter" in "file_pointer"
	EEPROM_buffer[(*ptr_array_pointer)++] = '\0';						//Terminate the EEPROM_buffer with a '\0'.

	LEDs_on;
	{int n=0;															//Copy characters from "EEPROM_buffer" to the EEPROM
	while (((*ptr_EEP_pointer) +n) < EE_top)							//Stop copying when space rerserved for the application is reached
		{Read_write_mem('I', ((*ptr_EEP_pointer) + n),\
		EEPROM_buffer[n]); 
		n++;
		if(n==(*ptr_array_pointer))break;}								//Exit when the end of the "EEPROM_buffer" is reached

	if (((*ptr_EEP_pointer)+n) == EE_top)								//EEPROM allocation all used? Yes:
		{*ptr_DL_flag = 3; 												//Set DL_flag to 3.																	
		Read_write_mem('I', (EE_top-1), '\0'); 						//Terminate EEPROM in '\0'
		*ptr_EEP_pointer = EE_top; 										//Set EEP_pointer to EEPROM 
		return 1;}														//Return setting the "DL_status" to 1

	(*ptr_EEP_pointer) += n-1; 										//Set "EEP_pointer" to next available address in the EEPROM
	*ptr_array_pointer = 0;}  											//Get ready to start filling the EEPROM buffer again.
	return 0;}															//Return setting the "DL_status" to 0


//End of text section of the file encountered: Any data is appended.


else 																	//-"- char encountered to terminate text input
	LEDs_on;							
	EEPROM_buffer[((*ptr_array_pointer)++)] = '\0';					//Terminate final string in a '\0'.
	*ptr_file_pointer = UART_counter;									//Save the "UART_counter" in "file_pointer"
	(*ptr_EEP_pointer) += (*ptr_array_pointer); 						//Set "EEP_pointer" to the address available for the first number 
	if((*ptr_EEP_pointer) >= EE_top)									//If saving the entire string would cause the EEPROM allocation to be exceeded
		{*ptr_DL_flag = 3; 												//set the download flag to 3
			{int n=0;
			while (((*ptr_EEP_pointer) - (*ptr_array_pointer) +n)\
			< (EE_top-1))
				{Read_write_mem('I', ((*ptr_EEP_pointer) -\
				(*ptr_array_pointer) + n),EEPROM_buffer[n]);			//Copy as much as possible of the EEPROM buffer to the EPPROM
				n++;
				}
			}
		Read_write_mem('I', (EE_top-1), '\0'); 						//Terminate the EEPROM in '\0'
		*ptr_EEP_pointer = EE_top; 
		return 1;														//Return setting the "DL_status" to 1
		}
	*ptr_DL_flag = 2; 													//If overflow is not a problem set "DL_flag" to 2
	return 1;}															//Return setting the "DL_status" to 1






/********************************************************************************************************************************************/
char Download_data(int *ptr_EEP_pointer,int *ptr_file_pointer,\
char *ptr_DL_flag, int *ptr_data_counter,\
int *ptr_array_pointer,\
unsigned char EEPROM_buffer[])

{int ADDR_last_string=0, UART_counter=0;

	{UART_counter = *ptr_file_pointer; 
	ADDR_last_string = *ptr_EEP_pointer - *ptr_array_pointer;			//Address of last text character in EEPROM
	}

binUnwantedChars();	

switch (*ptr_DL_flag)															
	{case 2:															//Text download complete without causing EEPROM overflow 
		LEDs_on;														//Subsequent data (if any) all fits into the EEPROM_buffer
		{int n=0;\
		while ((ADDR_last_string +n) < (EE_top))
			{Read_write_mem('I', (ADDR_last_string + n),\
			EEPROM_buffer[n]);											//Save remaining text and data to EEPROM untill the EEPROM is full
			n++;														//or untill the EEPROM_buffer is empty (i.e.the download is complete)
			if (n==(*ptr_array_pointer))break;
			}
		} 
		return 1; break;
	}
return 0;
}



/********************************************************************************************************************************************/
void Upload_text(int EEP_pointer)
{char string_char;
int EEP_mem_counter = 0;

EEP_mem_counter = text_start;
while(EEP_mem_counter < EEP_pointer)
	{newline();
		Num_to_PC(16, EEP_mem_counter); 
	sendString ("    ");
	while(1)
		{string_char = Read_write_mem('O',(EEP_mem_counter++),0);
		if(string_char == '\0') break;
		sendChar(string_char); five_msec_delay;
		}
	}newline();
}





/********************************************************************************************************************************************/


char next_char_from_PC(void){
unsigned int n = 0;
while (!(UCSR0A & (1 << RXC0))){n++;
if (n > 15000) return 0;}													
return UDR0;}





