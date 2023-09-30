

void I2C_Tx(char, char, char*);




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
void I2C_Tx_8_byte_array(char s[]){									//was I2C_Tx_1
char num_bytes=8; char mode=4;
I2C_Tx(num_bytes,mode, s);}





/************************************************************************/
void I2C_Tx_LED_dimmer(void){
char Dimmer_control;

if((PINB & 0x40)^0x40)Dimmer_control = 1;			//if Switch_2 down
else Dimmer_control = 0;							//Normal operation

I2C_Tx(1, 'Q', &Dimmer_control);}	






/************************************************************************/
void I2C_Rx_get_version_A(char str_type){			//NEEDED
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



/************************************************************************/
void I2C_TX_328_check(void){			//NEEDED:	Tests to see whether or not user wants to chech version info
char num_bytes=0;						//or test clock calibration 
char mode = 'V';

waiting_for_I2C_master;		
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
TWCR = (1 << TWINT) | (1 << TWEN);		//clear interrupt and leave I2C slave active
TWCR |= (1 << TWEA) | (1 << TWIE);} 	//Slave mode with interrupt and Enable Acknowledge


