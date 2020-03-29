void Prog_EEPROM(void){					
int EEP_address_counter = 0,   Saved_UART_counter_value = 0,   EEP_buffer_pointer, EEP_data_counter = 0;
char temp_char, Down_Load_Flag = 0, down_load_status, op_code;	
unsigned char  EEPROM_buffer[265];
char reservation[4];
int App_reservation;

EEP_address_counter = text_start;
Text_Press_W_or_R;
while(1){if (!(isCharavailable(150))) sendString("?");	else break;}
switch(receiveChar()){

case 'R':
if(((Read_write_mem('O', 0x0, 0)) ==0xFF)  &&  ((Read_write_mem('O', 0x1, 0)) ==0xFF))
{sendString("No Data!\r\n");break;}

EEPROM = ( ( (Read_write_mem('O', 0x3, 0))<<8 ) +  (Read_write_mem('O', 0x4, 0))) ;
Upload_text((Read_write_mem('O', 0x0, 0x0) << 8) + Read_write_mem('O', 0x1, 0x0));	
if(Read_write_mem('O', 0x2, 0x0) && (Read_write_mem('O', 0x2, 0x0)!= 0xFF)){sendString("1,2 or 3");	
op_code =  waitforkeypress();	binUnwantedChars();		
switch (op_code){
case '1':  Upload_data_1 (((Read_write_mem('O', 0x0, 0x0) << 8) + Read_write_mem('O', 0x1, 0x0))  ,Read_write_mem('O', 0x2, 0x0)); break;
case '2':	 Upload_data_2 (((Read_write_mem('O', 0x0, 0x0) << 8) + Read_write_mem('O', 0x1, 0x0))  ,Read_write_mem('O', 0x2, 0x0)); break;	
default:	Upload_data (((Read_write_mem('O', 0x0, 0x0) << 8) + Read_write_mem('O', 0x1, 0x0))  ,Read_write_mem('O', 0x2, 0x0)); break;}}


if ((((Read_write_mem('O', 0x3, 0)) <<8 ) +  (Read_write_mem('O', 0x4, 0)))  == 0xFFFF); else	
{ if((((Read_write_mem('O', 0x3, 0)) <<8 ) +  (Read_write_mem('O', 0x4, 0))) < 0x200)
{sendString("\r\nApp: Start addr's  ");sendHex(16,(((Read_write_mem('O', 0x3, 0)) <<8 ) +  (Read_write_mem('O', 0x4, 0)))) ;}}
break;		

case 'r':
text_start_mem = text_start;
text_start = 0x200;
Upload_text(0x3F7);
text_start = text_start_mem;
break;

case 'W':
case 'w':
Text_int_0_FF; 		
reservation[0] = '0';		//MSB
reservation[2] = '\0';
while(1){
reservation[1] = waitforkeypress();
if (non_numeric_char (reservation[1]))sendChar ('?'); else break;}

if (isCharavailable(150)){while(1){
temp_char = receiveChar();
if (non_numeric_char (temp_char)) {sendChar ('?'); while (!(UCSR0A & (1 << RXC0)));} else break;}  binUnwantedChars();		//NEW LINE
reservation[0] = reservation[1]; 
reservation[1] = temp_char;}
	
App_reservation =  askiX2_to_hex(reservation);

if(App_reservation > 0){
sendString("\r\nApp: Start addr's  ");sendHex(16, (EEPROM - App_reservation));newline();
sendString ("X to escape or AOK\r\n");
if(waitforkeypress() == 'X') {binUnwantedChars();	wdt_enable(WDTO_1S); while(1);}}
binUnwantedChars();		
EEPROM = EEPROM - App_reservation;
temp_char = Read_write_mem('I', 0x3, (EEPROM >> 8)); 	
temp_char = Read_write_mem('I', 0x4, (EEPROM & 0x00FF));

Text_Send_Text_File;
EEP_data_counter = 0;
do {
down_load_status = (Download_text(&EEP_address_counter, &Saved_UART_counter_value,\
&Down_Load_Flag, &EEP_buffer_pointer,  EEPROM_buffer));
if (!(down_load_status)){Down_Load_Flag = 0;  
LED_1_off;	
Text_Send_file_again}}
while (!(down_load_status));
LED_1_off;	

if (Down_Load_Flag == 2){
while (!(Download_data(&EEP_address_counter, &Saved_UART_counter_value,\
&Down_Load_Flag, &EEP_data_counter,  &EEP_buffer_pointer, EEPROM_buffer)))
{LED_1_off;							
Text_Send_file_again;}}
binUnwantedChars ();
LED_1_off;

sendString("AK?");	
binUnwantedChars();		
temp_char = Read_write_mem('I', 0x0, (EEP_address_counter >> 8));  	
temp_char = Read_write_mem('I', 0x1, (EEP_address_counter & 0x00FF));
temp_char = Read_write_mem('I', 0x2, EEP_data_counter);	temp_char = waitforkeypress();
Upload_text(EEP_address_counter);  
if (EEP_data_counter > 0) Upload_data (EEP_address_counter, EEP_data_counter); 
break;  //End of Read case
default: break;}
wdt_enable(WDTO_60MS); while(1);}



char Download_text(int *ptr_EEP_address_counter,\
int *ptr_Saved_UART_counter_value,\
char *ptr_Down_Load_Flag,\
int *ptr_EEP_buffer_pointer, unsigned char EEPROM_buffer[]){	
int UART_counter = 0;
char temp_char, text_char;

*ptr_EEP_buffer_pointer = 0;
text_char = waitforkeypress();	UART_counter++;											
while(1){text_char = Rx_data(); UART_counter++; if (text_char == '"') break;}	
	
while(!(*ptr_Down_Load_Flag)){//loop1
text_char = Rx_data(); UART_counter++; 
if (text_char != '"')	{//loop2
if (UART_counter > *ptr_Saved_UART_counter_value){//loop 3  Only save text if downloaded for the first time

switch (text_char){	
case '\n':
case '\r':
if (   EEPROM_buffer[ ((*ptr_EEP_buffer_pointer)-1)] != '\0')
{text_char = '\0';} else {((*ptr_EEP_buffer_pointer)--); text_char = '\0';}
default: break;}

EEPROM_buffer[((*ptr_EEP_buffer_pointer)++)] = text_char;
if 	((*ptr_EEP_buffer_pointer >= RBL) && (text_char != '\r') && (text_char != '\0')) *ptr_Down_Load_Flag = 1;	}//loop 3																														
}//loop2
else break;}   	//loop1	Exit Loop 1 when the second " is received

if (*ptr_Down_Load_Flag == 1){binUnwantedChars();	 
*ptr_Saved_UART_counter_value = UART_counter; 
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = '\0';

LED_1_on;
{int n=0;
while (((*ptr_EEP_address_counter) +n) < EEPROM){
temp_char = Read_write_mem('I', ((*ptr_EEP_address_counter) + n),EEPROM_buffer[n]); 
n++;
if (n==(*ptr_EEP_buffer_pointer))break;}
if (((*ptr_EEP_address_counter) +n) == EEPROM){*ptr_Down_Load_Flag = 3; // EEPROM overflow occurs: Save text and exit
temp_char = Read_write_mem('I', (EEPROM-1), '\0'); 
*ptr_EEP_address_counter = EEPROM;  
return 1;}
(*ptr_EEP_address_counter) += n-1; 
*ptr_EEP_buffer_pointer = 0;}  
return 0;}

else // " char encountered to terminate text input
LED_1_on;							
EEPROM_buffer[((*ptr_EEP_buffer_pointer)++)] = '\0';
*ptr_Saved_UART_counter_value = UART_counter;
(*ptr_EEP_address_counter) += (*ptr_EEP_buffer_pointer); //addres of first number 
if((*ptr_EEP_address_counter) >= EEPROM){*ptr_Down_Load_Flag = 3; //EEPROM overflow occurs: Save text and exit
{int n=0;\
while (((*ptr_EEP_address_counter) - (*ptr_EEP_buffer_pointer) +n) < (EEPROM-1)){\
temp_char = Read_write_mem('I', ((*ptr_EEP_address_counter) - (*ptr_EEP_buffer_pointer) + n),EEPROM_buffer[n]);
n++;}}
temp_char = Read_write_mem('I', (EEPROM-1), '\0'); 
*ptr_EEP_address_counter = EEPROM; return 1;}
*ptr_Down_Load_Flag = 2; 
return 1;}



char Download_data(int *ptr_EEP_address_counter,int *ptr_Saved_UART_counter_value,\
char *ptr_Down_Load_Flag, int *ptr_EEP_data_counter,\
int *ptr_EEP_buffer_pointer, unsigned char EEPROM_buffer[]){

int data_int=0, UART_counter=0, ADDR_last_string=0, next_data_address=0;
char data_text,  temp_char;

if(*ptr_EEP_buffer_pointer == 0) {next_data_address = *ptr_EEP_address_counter + *ptr_EEP_data_counter + *ptr_EEP_data_counter;
data_text = waitforkeypress();	UART_counter++;	
while 	(UART_counter < *ptr_Saved_UART_counter_value) {data_text = Rx_data(); UART_counter++; }}

else {UART_counter = *ptr_Saved_UART_counter_value; ADDR_last_string = *ptr_EEP_address_counter - *ptr_EEP_buffer_pointer;}
	
while((*ptr_Down_Load_Flag ==0) || (*ptr_Down_Load_Flag ==2) ){ //LOOP 1
data_text = Rx_data(); UART_counter++; 
if(!data_text)break;
switch (data_text){
case '-': case '+': case '0': case '1': case '2':  case '3': case '4': case '5':  case '6': case '7': case '8': case '9':  
data_int = decimal_conversion(data_text, &UART_counter); 
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = (data_int >> 8);
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = (data_int & 0x00FF);
(*ptr_EEP_data_counter)++;
if((*ptr_EEP_buffer_pointer >= RBL)){
if(*ptr_Down_Load_Flag == 2) *ptr_Down_Load_Flag = 3;
if(*ptr_Down_Load_Flag == 0) *ptr_Down_Load_Flag = 4;}break;

case '$':  data_int = Hex_conversion(data_text, &UART_counter);
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = (data_int >> 8);
EEPROM_buffer[(*ptr_EEP_buffer_pointer)++] = (data_int & 0x00FF);
(*ptr_EEP_data_counter)++;
if((*ptr_EEP_buffer_pointer >= RBL)){								
if(*ptr_Down_Load_Flag == 2) *ptr_Down_Load_Flag = 3;
if(*ptr_Down_Load_Flag == 0) *ptr_Down_Load_Flag = 4;}break;

default: break;}
} //LOOP 1

binUnwantedChars();	

switch (*ptr_Down_Load_Flag){
case 2:	//RAM buffer only partially full:  Text and numbers or just text
LED_1_on;
{int n=0;\
while ((ADDR_last_string +n) < (EEPROM)){\
temp_char = Read_write_mem('I', (ADDR_last_string + n),EEPROM_buffer[n]);
n++;\
if (n==(*ptr_EEP_buffer_pointer))break;}} 
return 1; break;

case 3: //RAM buffer completely full:  Text and numbers
LED_1_on;
{int n=0;\
while ((ADDR_last_string +n) < (EEPROM)){\
temp_char = Read_write_mem('I', (ADDR_last_string + n),EEPROM_buffer[n]); 
n++;\
if (n==(*ptr_EEP_buffer_pointer))break;}
if((ADDR_last_string +n) == (EEPROM)) {
LED_1_off;	
return 1;} } //EEPROM overflow occurs: Save text and exit
(*ptr_EEP_buffer_pointer) = 0;  *ptr_Down_Load_Flag = 0; *ptr_Saved_UART_counter_value = UART_counter;
return 0; break;

case 0:	//RAM buffer only partially full:  End of file Numbers only
LED_1_on;
{int n=0;\
while ((next_data_address +n) < (EEPROM)){\
temp_char = Read_write_mem('I', (next_data_address + n),EEPROM_buffer[n]); 
n++;\
if (n==(*ptr_EEP_buffer_pointer))break;}}
(*ptr_EEP_buffer_pointer) = 0;
return 1;

case 4: //RAM buffer completely full:  Numbers only
LED_1_on;
{int n=0;\
while ((next_data_address +n) < (EEPROM)){\
temp_char = Read_write_mem('I', (next_data_address + n),EEPROM_buffer[n]);  
n++;\
if (n==(*ptr_EEP_buffer_pointer))break;}
if((next_data_address +n) == (EEPROM)){   
return 1;}}  //EEPROM overflow occurs: Save text and exit
(*ptr_EEP_buffer_pointer) = 0; *ptr_Down_Load_Flag = 0; *ptr_Saved_UART_counter_value = UART_counter;
return 0;}
return 0;}


int decimal_conversion (char data_text, int *ptr_UART_counter){
int char_counter=0;
char data_string[7], num_char;
data_string[char_counter++] = data_text;
while(1){
num_char = Rx_data(); (*ptr_UART_counter)++;
if (num_char == ' ') break;
if (num_char == '\r') break;
if (num_char == '\n') break;
if (num_char == '\0') break;
if (num_char == '\t') break;
else data_string[char_counter++] = num_char;}
data_string[char_counter++] = '\0';
return my_atoi_with_sign (data_string);}

int Hex_conversion (char data_text, int *ptr_UART_counter){
int char_counter=0;
char data_string[7], num_char;
while(1){
num_char = Rx_data(); (*ptr_UART_counter)++;
if (num_char == ' ') break;
if (num_char == '\r') break;
if (num_char == '\n') break;
if (num_char == '\0') break;
if (num_char == '\t') break;
else data_string[char_counter++] = num_char;}
data_string[char_counter++] = '\0';
return askiX4_to_hex (data_string);}


void Upload_text(int EEP_address_counter){ 
char string_char;
int EEP_mem_counter = 0;
EEP_mem_counter = text_start;
while (EEP_mem_counter < EEP_address_counter){newline();sendHex (16, EEP_mem_counter); sendString ("    ");
while(1){string_char = Read_write_mem('O',(EEP_mem_counter++),0);
if ((string_char == '\0') || (EEP_mem_counter == EEP_address_counter))break;
sendChar (string_char);}}}

void Upload_data(int address_first_data_item, int EEP_data_counter){
char output_counter=0;
int data_item = 0 ;
if(!(EEP_data_counter))return;
newline();sendHex (16, (address_first_data_item) );
{int n=0; while((address_first_data_item+n+1) < EEPROM){
data_item = (  (Read_write_mem('O', address_first_data_item+n, 0x0) << 8) + Read_write_mem('O',(address_first_data_item+n+1), 0x0)  );
if  (output_counter == 4){newline();output_counter  = 0;sendHex (16, (address_first_data_item+n) );}
sendChar('\t'); sendHex (16, data_item ); sendChar ('\t'); sendsignedHex (data_item );output_counter++; n+=2; if(n>2*(EEP_data_counter-1)) break; }}}

void Upload_data_1(int address_first_data_item, int EEP_data_counter){
char output_counter=0;
if(!(EEP_data_counter))return;
int data_item = 0 ;
newline();sendHex (16, (address_first_data_item) );
{int n=0; while((address_first_data_item+n+1) < EEPROM){
data_item = (  (Read_write_mem('O', address_first_data_item+n, 0x0) << 8) + Read_write_mem('O',(address_first_data_item+n+1), 0x0)  );
if  (output_counter == 8){newline();output_counter  = 0;sendHex (16, (address_first_data_item+n) );}
sendChar('\t');  sendsignedHex (data_item );output_counter++; n+=2; if(n>2*(EEP_data_counter-1)) break; }}}

void Upload_data_2(int address_first_data_item, int EEP_data_counter){
char output_counter=0;
int data_item = 0 ;
if(!(EEP_data_counter))return;
newline();sendHex (16, (address_first_data_item) );
{int n=0; while((address_first_data_item+n+1) < EEPROM){
data_item = (  (Read_write_mem('O', address_first_data_item+n, 0x0) << 8) + Read_write_mem('O',(address_first_data_item+n+1), 0x0)  );
if  (output_counter == 8){newline();output_counter  = 0;sendHex (16, (address_first_data_item+n) );}
sendChar('\t'); sendHex (16, data_item ); output_counter++; n+=2; if(n>2*(EEP_data_counter-1)) break; }}}




