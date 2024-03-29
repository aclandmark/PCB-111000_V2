
/*
Three principal subroutines are provided:
	Cal_at_Power_on_Reset()
	manual_cal_mini-OS_device
	cal_plot_328()
	
The remaining subroutines are called by one or more of these three.
Operation also depends on the operation of the T1 and T2 ISR's

Cal_at_Power_on_Reset() should ensure that calibration is always adequate to enable
hex files to be downloaded to the user device.


Operation of Cal_at_Power_on_Reset():
To make this run switch the DPDT to the right and power cycle the board.
If it is switched left the user project will start immediately.


It also runs after the Atmega 328 has been programmed using "Project_pcb_168_V2.30C":
Switch the DPDT to the left operate SW1 and then power cycle.
Press R in response to the user prompt;

Note:
Subroutine start_T2_for_ATMEGA_168_cal() is also included.
This sends an I2C interrupt to the user device every 7_8125mS 
during which time an 8MHz clock counts to 62,500
*/








void initialise_timers_for_cal_error(void);
void start_timers_for_cal_error(void);
long compute_error(char, char, char);
void Minimise_error(int, char*, char*, long*, char*, char);
void start_T2_for_ATMEGA_168_cal(char);
void Cal_at_Power_on_Reset (void);
void manual_cal_PCB_A_device(void);
void cal_plot_328(void);
void cal_spot_check(void);	

void I2C_master_transmit(char);
char I2C_master_receive(char);


#define Get_ready_to_calibrate \
TIMSK2 |= (1 << TOIE2);\
TIMSK1 |= (1 << TOIE1);\
initialise_timers_for_cal_error();\
start_timers_for_cal_error();

#define close_calibration \
initialise_timers_for_cal_error();\
TIMSK2 &= (~(1 << TOIE2));\
TIMSK1 &= (~(1 << TOIE1));


#define calibrate_without_sign_plus_warm_up_time \
cal_mode = 5;\
cal_error = compute_error(0,cal_mode,0);\
cal_error = compute_error(0,cal_mode,0);\
cal_error = compute_error(0,cal_mode,0);




/************************************************************************************************************/
void manual_cal_PCB_A_device(void){
long cal_error;
char OSCCAL_UV;		

TIMSK0 &= (~(1 << TOIE0));								//display not required
EA_buff_ptr = 0;
cal_mode = 5;		
Get_ready_to_calibrate;
OSCCAL -=20;											//Compute cal error for 41 values of OSCCAL
for(int m = 0; m <= 40; m++)
{cal_error = compute_error(1,cal_mode,1);OSCCAL++;}
OSCCAL = OSCCAL_WV;
close_calibration;
		
Initialise_I2C_master_write;							//Transmit error values to user
I2C_master_transmit(OSCCAL_DV);
I2C_master_transmit(OSCCAL_WV - 20);						
for(int m = 0; m <= 40; m++){							
I2C_master_transmit(buffer[m] >> 8);
I2C_master_transmit(buffer[m]);}
I2C_master_transmit	(OSCCAL_WV);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	
	
Initialise_I2C_master_read;								//Read OSCCAL_UV (user value)
OSCCAL_UV = I2C_master_receive(0);							
TWCR = (1 << TWINT ) | (1 << TWEN ) | (1 << TWSTO );
	
/*********************************************/
Get_ready_to_calibrate;									//Test value of OSCCAL entered by user
if(OSCCAL_UV == 0xFF)OSCCAL = OSCCAL_WV;				//If 0xFF reinstate working value
else OSCCAL = OSCCAL_UV;								//OSCCAL test value
calibrate_without_sign_plus_warm_up_time;								
close_calibration;
	
Initialise_I2C_master_write;							//Report results to user
if(cal_error > 1750)			//5550					//Error resulting from User OSCCAL exceeds 1750
{I2C_master_transmit('X');								//Reject result
I2C_master_transmit(cal_error >> 8);						
I2C_master_transmit(cal_error);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
eeprom_write_byte((uint8_t*)0x3FE, 0xFF); 				//Reset OSCCAL values storred in EEPROM
eeprom_write_byte((uint8_t*)0x3FF, 0xFF);
OSCCAL = OSCCAL_WV;										//Reinstate default value
return;}			
	
/*************************************************/
else{I2C_master_transmit('Y');							//Error resulting from User OSCCAL is less than 1750
	
eeprom_write_byte((uint8_t*)0x3FE, OSCCAL_UV); 		//save user OSCCAL to EEPROM
eeprom_write_byte((uint8_t*)0x3FF, OSCCAL_UV); 
if(OSCCAL_UV == 0xFF) OSCCAL = OSCCAL_WV;				//Reinstate working value
else {OSCCAL = OSCCAL_UV;
OSCCAL_WV = OSCCAL;}	
	
TWDR = eeprom_read_byte((uint8_t*)0x3FE);				//Echo values saved to EEPROM to user
TWCR = (1 << TWINT) | (1 << TWEN);
while (!(TWCR & (1 << TWINT)));
TWDR = eeprom_read_byte((uint8_t*)0x3FF);
TWCR = (1 << TWINT) | (1 << TWEN);
while (!(TWCR & (1 << TWINT)));
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}
TIMSK0 |= (1 << TOIE0);									//Restore multplexer interrupt
}




/*******************************************************************************************/
void cal_plot_328(void){							//Called by Proj_9F (mode M)
long cal_error;

TIMSK0 &= (~(1 << TOIE0));								//display not required
cal_mode = 2;

for(int m = 0x10; m <= 0xF0; m++){
Get_ready_to_calibrate;
OSCCAL = m;
cal_error = compute_error(0,cal_mode,1);
OSCCAL = OSCCAL_WV;
close_calibration;

Initialise_I2C_master_write;
I2C_master_transmit(cal_error >> 8);
I2C_master_transmit(cal_error);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}
TIMSK0 |= (1 << TOIE0);									//Restore multplexer interrupt
}





/************************************************************************************/
void initialise_timers_for_cal_error(void){
TCNT1=0;TCCR1B = 0;									//Reset and halt T1
TCCR2B =  0x0;	while(ASSR & (1 << TCR2BUB));		//Halt T2
TCCR2A = 0; while(ASSR & (1 << TCR2AUB));			//Reset T2 
TCNT2=0; while(ASSR & (1 << TCN2UB));	}			//Reset TCNT2




/************************************************************************************/
void start_timers_for_cal_error(void)
{TCCR2B = 1; 	
while(ASSR & (1 << TCR2BUB));
TCCR1B = 1;}
	
	


/************************************************************************************/
void start_T2_for_ATMEGA_168_cal(char calibration_mode){					
TCCR2B =  0x0;	while(ASSR & (1 << TCR2BUB));		//Halt T2
TCCR2A = 0; while(ASSR & (1 << TCR2AUB));			//Reset T2 
TCNT2=0; while(ASSR & (1 << TCN2UB));
if(calibration_mode){
TIMSK2 |= (1 << TOIE2);							//Set Timer 2: interrupt on overflow
TCCR2B = 1; 										//Start T2 with a clock of 32.768Hz
while(ASSR & (1 << TCR2BUB));}
else{TIMSK2 &= (~(1 << TOIE2));mode = 0;}}




/*************************************************************/
void Minimise_error(int limit, char *counter_1, char *counter_2, long *error_mag, char *OSCCAL_mem, char local_cal_mode )
{while(*counter_2 < 20){ OSCCAL = *counter_1; *error_mag = compute_error(0,local_cal_mode,0); 
		if(*error_mag < limit)break;
		*counter_1 -= 1;
		*counter_2 +=1;}
		if (*counter_2 < 20)*OSCCAL_mem = OSCCAL;else OSCCAL = *OSCCAL_mem;}




/************************************************************************************************/		
long compute_error(char local_error_mode, char local_cal_mode, char sign)						//char OSCCAL_test_value)		
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
		if (local_error_mode)
		{buffer[EA_buff_ptr] = error/Num_2; EA_buff_ptr++;}
		return error/Num_2;}




/*****************************************************************************************************/
void Cal_at_Power_on_Reset (void){
char counter_1, counter_2;		
char OSCCAL_WV, OSCCAL_mem = 0;
long  error_mag; 
int limit;

ONE; digit_0;												//Initialise display

Timer_T1_sub(T1_delay_1sec);								//Crystal warm up time

cal_mode = 1;
mode = 'T';													//Required by T0 and T1 ISR	
Get_ready_to_calibrate;
		
counter_1 = 0xF1;
while(1){if (!(counter_1%6))toggle_digit_0;
counter_1 -= 1;
OSCCAL = counter_1; error_mag = compute_error(0,cal_mode,0); 
if(counter_1 > 0xE0)continue; 
if(error_mag < 1000)break;}
		
OSCCAL_mem = OSCCAL;
counter_2 = 0;
toggle_digit_0;

cal_mode = 5;		
limit = 1000;
for(int m = 1; m <= 9; m++){if (!(m%3))toggle_digit_0;
limit -= 100;
Minimise_error(limit, &counter_1, &counter_2, &error_mag, &OSCCAL_mem, cal_mode);}
		
error_mag = compute_error(0,cal_mode,0);
OSCCAL_WV = OSCCAL;	
close_calibration;
eeprom_write_byte((uint8_t*)0x3FE, OSCCAL_WV); 
eeprom_write_byte((uint8_t*)0x3FF, OSCCAL_WV);
	
if(eeprom_read_byte((uint8_t*)0x3F9) == 1)
{eeprom_write_byte((uint8_t*)0x3F9, 0xFF); 

Initialise_I2C_master_write;
I2C_master_transmit(OSCCAL);
I2C_master_transmit(error_mag >> 8);
I2C_master_transmit(error_mag);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);}

clear_digits;
clear_display;}							

	
		
/*************************************************/	
void cal_spot_check(void)								//Called by diagnostic mode

{long cal_error;	
								
Get_ready_to_calibrate;
calibrate_without_sign_plus_warm_up_time;							
close_calibration;

Initialise_I2C_master_write;
I2C_master_transmit(OSCCAL);
I2C_master_transmit(cal_error >> 8);
I2C_master_transmit(cal_error);
TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	}






