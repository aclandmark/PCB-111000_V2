

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



/************************************************************************/
void PCB_test(void){					
char num_bytes=0;		 
char mode = 'O';

waiting_for_I2C_master;		
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
TWCR = (1 << TWINT);}



/************************************************************************/
void scan_328_cal_factors(void){					
char num_bytes=0;		 
char mode = 'M';

waiting_for_I2C_master;		
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
TWCR = (1 << TWINT);}



/************************************************************************/
void shred_328_cal(void){

I2C_Tx_initiate_mode('Y');}




