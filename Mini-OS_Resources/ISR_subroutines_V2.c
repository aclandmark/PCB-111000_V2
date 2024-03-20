
char I2C_master_receive(char);



/*****************************************************************************************/
ISR(TIMER0_OVF_vect) {
	
	if(!(T0_interupt_cnt)){									//Set to zero by Proj subroutine I2C_Tx_LED_dimmer() subroutine mode Q
		if(mode == 'F'){									//Initiates I2C_initiate_10mS_ref
			TIMSK2 &= (!((1 << OCIE2A) | (1 << TOV2)));
			sei();
			Display_driver();
		TIMSK2 |= ((1 << OCIE2A) | (1 << TOV2));}
		else {Display_driver(); }}
	
	
if(eeprom_read_byte((uint8_t*)0x3FB) == 0x01){				//High level brightness
TCNT0 = 0;}


if(eeprom_read_byte((uint8_t*)0x3FB) == 0x02){				//Medium level brightness
	
TCNT0 = 212;		//224												//Initialise Timer 0 for 250uS pulse
	switch(T0_interupt_cnt){
		case 0: T0_interupt_cnt = 1;break;
		case 1: {clear_display;} T0_interupt_cnt = 0; TCNT0 = 50; break;}}		//32


if(eeprom_read_byte((uint8_t*)0x3FB) == 0x03){				//Low level brightness and multiplexer demo

if (!(MUX_cntl))											//Default setting Zero Set in "main" routine Low intensity.
{TCNT0 = 240;												//Initialise Timer 0 for 125uS pulse
	switch(T0_interupt_cnt){
		case 0: T0_interupt_cnt = 1;break;
		case 1: {clear_display;} 
			T0_interupt_cnt = 0; TCNT0 = 16; break;}}		//Set T2 to interrupt after 1875uS

else{														//Demonstrate the operation of the multiplexer
	
	switch (MUX_cntl){										//Set by project Multiplexer_demo mode "L"
		case 1: TCNT0 = 248; break;		//250
		case 2: TCNT0 = 244; break;		//375
		case 3: TCNT0 = 240; break;		//500
		case 4: TCNT0 = 251; break;		//625
		case 5: TCNT0 = 250; break;		//750
		case 6: TCNT0 = 246; break;		//1250
		case 7: TCNT0 = 240; break;}	//2000

	switch(T0_interupt_cnt){
		case 0: T0_interupt_cnt = 1;break;
		case 1: {clear_display;}
			switch (MUX_cntl){
				case 1: TCNT0 = 108; break;		//3750
				case 2: TCNT0 = 76; break;		//5625
				case 3: TCNT0 = 16; break;		//7500
				case 4: TCNT0 = 181; break;		//9375
				case 5: TCNT0 = 166; break;		//11250
				case 6: TCNT0 = 106; break;		//18750
				case 7:	TCNT0 = 16; break;}		//30000
		T0_interupt_cnt = 0;  break;}
}}}




/*****************************************************************************************/
ISR(TIMER1_OVF_vect) {
switch (mode){
case 9:  
{if(!(mode_C_ptr)){for(int m = 0; m<=7; m++)display_buf[m]=0;}
display_buf[7-mode_C_ptr] = '_';
mode_C_ptr++; mode_C_ptr=mode_C_ptr%8;
timer_T1_sub_with_interrupt(T1_delay_500ms);} break;

case 'M':   case 'N':	case 'T': case 'X':  T1_OVF += 1; break;

default: T1_ovf_flag = 1; TCCR1B = 0; break;}}




/*****************************************************************************************/
ISR(TIMER2_OVF_vect) {
long TCNT1_BKP;
char cal_168_mode;
switch (mode){
case 'K':   timer_2_counter++; if(timer_2_counter == 3)						//Scrolls scientific number across the display
{timer_2_counter=0; display_float(Sc_Num_string);} break;

case 'M':     case 'N':  case 'T': case 'X':   								//Atmega 328 calibration subroutines 
TCCR1B = 0;																	//Halt T1
TCNT1_BKP = TCNT1;															//Copy the value of TCNT1
TCNT1 = 0;																	//Clear TCNT1
TCCR1B = 1;																	//Get T1 running again ASAP (Note T2 has not stopped running)
if(EA_counter < cal_mode)T1_OVF = 0;										//Ignore first 5 results
else
{switch(T1_OVF){
case 0: error_SUM = error_SUM + (TCNT1_BKP - 62500); break;
case 1: error_SUM = error_SUM + (TCNT1_BKP - 62500 + 65536); break;
case 2: error_SUM = error_SUM + (TCNT1_BKP - 62500 + 131072); break;}
T1_OVF = 0;}EA_counter++;break;

case 'U': 																	//start_T2_for_ATMEGA_168_cal
Initialise_I2C_master_read;
cal_168_mode = (I2C_master_receive(0));	
TWCR = (1 << TWINT ) | (1 << TWEN ) | (1 << TWSTO );
if(cal_168_mode); else start_T2_for_ATMEGA_168_cal(0);				
break;

default:
timer_2_counter++; if(timer_2_counter == 16){timer_2_counter=0; 			//Enter this loop every second
if(mode != 'F'){update_timer (); Ten_mS_tick = 41;}							//Internal clock modes 7,8 and 9 (counts to 41 (40.96) in 10ms)
if(mode == 'F') { OCR2A = 41;Ext_tick();}}break;}}							//10mS external ref signal
																			//Ext_tck: //send a TWI start condition with interrupt



/*****************************************************************************************/
ISR(TIMER2_COMPA_vect){OCR2A = OCR2A + 41;  
	OCR2A = OCR2A%256; Ext_tick();}											//call every 10mS mode F only




/*****************************************************************************************/
ISR(TWI_vect){
switch(entry_point){																		//Address of slave (master read operation)  SLA + R
case 'A': TWDR = 0x03; TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE); 					//Send slave address + 'R'
			entry_point='B'; break;
case 'B': if (TWSR == 0x40){TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);				//Ack received wait for payload size
			entry_point='C';} 
			else {TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE); 
			entry_point='A';} break;														//Repeated Start: Nack received
case 'C': payload_size = TWDR; 
			if(!(payload_size))entry_point = 'Z'; 											//No payload: Exit
			else {TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);			//Send a repeated start condition
			entry_point='E';} break;
case 'E': TWDR = 0x03; TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE); 					//Send slave address + 'R'
			entry_point='F'; break;
case 'F': if (TWSR != 0x40){TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE);		
			entry_point='E';} 																//Nack received: Send repeated start
			else {TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE); 			//Ack received: Continue
			entry_point='G';} break;
case 'G': display_buf[disp_ptr] = TWDR; disp_ptr++; 										//Save first char in display buff
			if (disp_ptr < (payload_size-1))
			{TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
			entry_point = 'G';}																//Save subsequent chars in display buff
			else{TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);entry_point = 'H';}break;
case 'H': display_buf[disp_ptr] = TWDR;														//Last char
			entry_point = 'Z';
			break;}																			//Data transfer complete: Exit
if(entry_point=='Z'){TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
wdr();}}

		
