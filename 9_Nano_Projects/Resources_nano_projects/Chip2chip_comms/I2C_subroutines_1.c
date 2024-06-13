

void I2C_Tx(char, char, char*);
unsigned int PRN_16bit_GEN(unsigned int, unsigned char*);



/************************************************************************/
void I2C_Tx_2_integers(unsigned int s1, unsigned int s2){			
char num_bytes=4; char mode = 1; char s[4];
for (int m = 0;  m < 4; m++){
switch (m){
case 0: s[m] = s1; break; 											//Send S1 lower byte
case 1: s[m] = s1 >> 8; break; 									//Send S1 higher byte									
case 2: s[m] = s2; 	break;											//Send S1 lower byte
case 3: s[m] = s2 >> 8; break;}}									//Send S1 higher byte
I2C_Tx(num_bytes,mode, s);}



/************************************************************************/
void I2C_Tx_any_segment(char segment, char digit_num){
char s[2]; char num_bytes=2; char mode = 3;
s[0] = segment;
s[1] = digit_num; 
I2C_Tx(num_bytes,mode, s);}


/************************************************************************/
void I2C_Tx_any_segment_clear_all(void){
char segment = 'a'; char digit_num = 0;
char s[2]; char num_bytes=2; char mode = 2;
s[0] = segment;
s[1] = digit_num; 
I2C_Tx(num_bytes,mode, s);}



/************************************************************************/
void I2C_Tx_long(long L_number){
char s[4];
char num_bytes=4; char mode=6;
for(int m=0; m<=3; m++){s[m] = (L_number >> (8*(3-m)));}
I2C_Tx(num_bytes,mode, s);}



/************************************************************************/
void I2C_Tx_BWops(char s[]){										//Bit wise operations
char num_bytes=8;
char mode = 'G';
I2C_Tx(num_bytes,mode, s);}




/************************************************************************/
void I2C_Tx_8_byte_array(char s[]){									//was I2C_Tx_1
char num_bytes=8; char mode=4;
I2C_Tx(num_bytes,mode, s);}



/************************************************************************/
void I2C_Tx_display_char (char single_char, char sign_bit){
char s[2]; char num_bytes=2; char mode = 5;
s[0] = single_char;
s[1] = sign_bit;
I2C_Tx(num_bytes, mode, s);}



/************************************************************************/
void I2C_Tx_display(void){

int PRN = 0;
unsigned char PRN_counter = 0;

if ((!(eeprom_read_byte((uint8_t*)0x1FC))) && (!(eeprom_read_byte((uint8_t*)0x1FB))))
eeprom_write_byte((uint8_t*)(0x1FC), 0xFF);


while(1){
PRN = PRN_16bit_GEN (PRN, &PRN_counter);									//Generate a new PRN (0) tells subroutine to use the EEPROM
I2C_Tx_2_integers(PRN, (PRN<<1));							//Display two "pseudo random numbers"
Timer_T1_sub(T1_delay_100ms);}}



/************************************************************************/
void I2C_Tx_LED_dimmer(void){
char Dimmer_control;

Dimmer_control = 0;

if ((PIND & 0x04) && (PINB & 0x40) && (PIND & 0x80)) Dimmer_control = 0;				//No user switches pressed

if(((PIND & 0x04)^0x04) && ((PINB & 0x40)^0x40) && (PIND & 0x80))Dimmer_control = 1;		//sw1 and sw2 pressed

if(((PIND & 0x04)^0x04) && (PINB & 0x40) && ((PIND & 0x80)^0x80))Dimmer_control = 2;		//sw1 and sw3 pressed

if ((PIND & 0x04) && ((PINB & 0x40)^0x40) && ((PIND & 0x80)^0x80))Dimmer_control = 3;	//sw2 and sw3 pressed

/*
if((PINB & 0x40)^0x40)Dimmer_control = 1;			//if Switch_2 down
else Dimmer_control = 0;							//Normal operation

}*/	
//Serial.print((int) Dimmer_control);
I2C_Tx(1, 'Q', &Dimmer_control);}



/************************************************************************/
/*void I2C_Rx_get_version(char str_type){
char num_bytes=1; char mode='P';
char s[2];

s[0]= str_type; s[1]=0;
I2C_Tx(num_bytes,mode, s);
waiting_for_I2C_master;
num_bytes = (receive_byte_with_Ack() - '0') * 10;
num_bytes += (receive_byte_with_Ack() - '0');
for (int m = 0; m < num_bytes; m++){
if (m ==(num_bytes-1)){Char_to_PC_Basic(receive_byte_with_Nack());}
else {Char_to_PC_Basic(receive_byte_with_Ack());}}
TWCR = (1 << TWINT);}
*/


/************************************************************************/
/*void I2C_Rx_get_version_A(char str_type){			//NEEDED
char num_bytes=1; char mode='P';
char s[2];

s[0]= str_type; s[1]=0;
I2C_Tx(num_bytes,mode, s);
waiting_for_I2C_master;
num_bytes = (receive_byte_with_Ack() - '0') * 10;
num_bytes += (receive_byte_with_Ack() - '0');
for (int m = 0; m < num_bytes; m++){
if (m ==(num_bytes-1)){Serial.write(receive_byte_with_Nack());}
else {Serial.write(receive_byte_with_Ack());}}
TWCR = (1 << TWINT);}
*/


/************************************************************************/
void I2C_TX_328_check(void){			//NEEDED:	Tests to see whether or not user wants to chech version info
char num_bytes=0;						//or test clock calibration 
char mode = 'V';

waiting_for_I2C_master;		
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
TWCR = (1 << TWINT) | (1 << TWEN);		//clear interrupt and leave I2C slave active
TWCR |= (1 << TWEA) | (1 << TWIE);} 	//Slave mode with interrupt and Enable Acknowledge



/************************************************************************/
long I2C_displayToNum(void){		
long L_number = 0;
unsigned char receive_byte;
char num_bytes=0;
char mode = 'I';

waiting_for_I2C_master;		
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
TWCR = (1 << TWINT) | (1 << TWEN);			//clear interrupt and leave I2C active
waiting_for_I2C_master;

for (int m = 0; m<=3; m++){				//Receive 4 chars and assemble into unsigned long result
if (m ==3){receive_byte = receive_byte_with_Nack();}
else {receive_byte = receive_byte_with_Ack();}
switch(m){
case 0: case 1: case 2:L_number =  L_number + receive_byte; 
L_number = L_number << 8; break;
case 3: L_number =  L_number + receive_byte; break;}}
clear_I2C_interrupt;
return L_number;}


