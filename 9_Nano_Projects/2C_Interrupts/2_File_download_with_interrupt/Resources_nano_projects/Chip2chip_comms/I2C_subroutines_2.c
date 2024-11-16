



/************************************************************************/
void I2C_Tx_OS_timer(char mode, char s[]){								
char num_bytes=8;
I2C_Tx(num_bytes,mode, s);}

/************************************************************************/
void I2C_Tx_Clock_command(char timer_mode, char command){			
char s[2]; char num_bytes=1;
s[0] = command;
I2C_Tx(num_bytes, timer_mode, s);}


/************************************************************************/
void I2C_initiate_10mS_ref(void){
char num_bytes=0;
char mode = 'F';

waiting_for_I2C_master;
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);

TWCR = (1 << TWINT) | (1 << TWEN);		//clear interrupt and leave I2C slave active
TWCR |= (1 << TWEA) | (1 << TWIE);} 	//Slave mode with interrupt and Enable Acknowledge


/**********************************************************************************************/
void I2C_initiate_7_8125mS_ref(void){
char num_bytes=0;
char mode = 'U';
waiting_for_I2C_master;
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
clear_I2C_interrupt;} 



/************************************************************************/
void scan_328_cal_factors(void){					
char num_bytes=0;		 
char mode = 'M';

waiting_for_I2C_master;		
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
TWCR = (1 << TWINT);}



/************************************************************************/
void PCB_test (char test_num, char test_digit){
char s[2]; char num_bytes=2; char mode = 'O';
s[0] = test_num;
s[1] = test_digit;
I2C_Tx(num_bytes, mode, s);}



/************************************************************************/
void shred_328_cal(void){

I2C_Tx_initiate_mode('Y');}




