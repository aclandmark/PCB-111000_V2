



int main (void){

	signed char digit_num=0;
	int letter=0;
	char payload_size, sign;
	long L_number;
	signed char expnt;
	char sign_bit;
	char test_num, test_digit;

	
	/****Watchdog initiated for mode F only (user clock/stop watch with
	external 10mS crystal interrupt).*********/

	/***********Brown-out:  This is set (using config bits only) for 2.9V*************/

	if(MCUSR & (1 << BORF)){									//Detect brown-out
	MCUSR &= (~(1 << BORF));}									//Reset brown-out flag

	ADMUX |= (1 << REFS0);										//select internal ADC ref and remove external supply on AREF pin
	setup_watchdog;
	initialise_IO;												//Ensures that all IO is initially set to WPU
	set_digit_drivers;
	clear_digits;
	clear_display;
	initialise_Arithmetic_variables;

	
	TWBR = 32;													//gives 100KHz I2C clock for TWSR
	ASSR = (1 << AS2); 											//initialise T2 for crystal
	timer_2_counter=0;											//Initialise timer_2_counter to zero


	OSCCAL_DV = eeprom_read_byte((uint8_t*)0x3FD);				//Save OSCALL working and default values


	/****************************************************/
	sei();

	if(eeprom_read_byte((uint8_t*)0x3F9) == 1)					//Post programming //and POR
	Cal_at_Power_on_Reset ();									//call cal routine
	/****************************************************/

	if ((PIND & (1 << PD1)) && (MCUSR & (1 << PORF)))
	Cal_at_Power_on_Reset ();

	MCUSR &= (~(1 << PORF));

	OSCCAL_WV = OSCCAL;

switch (eeprom_read_byte((uint8_t*)0x3FB))
{
	case 1: timer_T0_sub_with_interrupt(T0_delay_2ms);break;
	case 3: timer_T0_sub_with_interrupt(T0_delay_125us);break;
	default: eeprom_write_byte((uint8_t*)0x3FB, 0x02);
	timer_T0_sub_with_interrupt(T0_delay_350us);break;
}
T0_interupt_cnt = 0;											//Start multiplexer
TIMSK0 |= (1 << TOIE0);											//T0 interrupt enabled
MUX_cntl = 0;													//Only used in multiplexer demo project 

	while(1){													//main loop in which the program resides

		while((mode == 'F')\
		||(mode == 'K')\
		|| (mode == 'U'));



		while(1){													//Repetitively poll the PIC until it responds
			TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);		//send a start condition  (this identical to a "repeated start")
			while (!(TWCR & (1 << TWINT)));							//Wait for TWINT flag
			TWDR = 0x03;											//Address of slave (master read operation)  SLA + R
			TWCR = (1 << TWINT) | (1 << TWEN);						//Clear TWINT bit to start transmission of address
			while (!(TWCR & (1 << TWINT)));							//Wait for TWINT flag
			if (TWSR == 0x40)break;									//SLA + W successfully transmitted ACK received
			if(clock_flag==1){refresh_clock_display;}}

			clock_flag=0;

			payload_size = I2C_master_receive(1);
			if(payload_size)mode = I2C_master_receive(1);
			else mode = I2C_master_receive(0);

			if (mode != 8) display_mask=0;								//disable flashing digits

			if(payload_size){											//payload is zero for mode 'G'

				for (int m = 0; m < payload_size; m++){					//payload_size = Num digits to be downloaded
					if (m == (payload_size-1))
					I2C_data[m] = I2C_master_receive(0);
				else I2C_data[m] = I2C_master_receive(1);}
			TWCR = (1 << TWINT ) | (1 << TWEN ) | (1 << TWSTO );}		//Send stop bit

			switch(mode){
				case 3: 
				case 'F':  case 'I':  case 'Q': break;
				default: {clear_display_buffer;}break;}

				switch (mode){

					case 1:		I2C_Tx_2_integers; break;				//Uses the 32 vertical segments to display two binary numbers

					case 2:												//Illuminates/clears any of the 56 segments
					case 3: 	I2C_Tx_any_segment; break;

					case 4: 	I2C_Tx_8_byte_array;break;				//Display a string of 8 digits

					case 5: 	I2C_Tx_display_char; break;				//Displays +/- char as number and binary

					case 6: 	I2C_Tx_long; break;						//Displays 8 digit number

					case 'C':	basic_clock(); break;					//I2C_Tx_OS_timer(AT_clock_mode, start_time): Starts the timer/clock

					case 'D':	multi_mode_clock(); break;				//Used with subroutine I2C_Tx_Clock_command(char timer_mode, char command): Controls the timer/clock
					
					case 'E':	stop_watch(); break;					//Used by I2C_Tx_Clock_command(one100ms_mode) and I2C_Tx_Clock_command(ten_ms_mode)

					case 'F':	I2C_initiate_10mS_ref; break;			//Ten_mS_interrupt for combined clock and stopwatch;
					
					case 'G':	I2C_Tx_BWops;	break;					//Used to illustrate bit wise logic operations

					case 'H':	Message_from_the_OS(); break;			//Messages compiled by user and saved to pcb_A Atmega 328 EEPROM

					case 'I':	I2C_displayToLong; break;				//Converts display to long number and transmits it to the Atmega 168

					case 'J':	I2C_Tx_real_num; break;					//Displays a real number

					case 'K':	I2C_Tx_float_num; break;				//Scrolls scientific number across the display

					case 'L':	Multiplexer_demo; break;

					case 'M':	cal_plot_328(); break;					//Scans 328 cal-factor from 0x10 to 0xF0

					case 'N':	manual_cal_PCB_A_device(); break;		//The Atmega328
					
					case 'O':	PCB_test; break;						//For manufacturing test: disables the multiplexer
					
					case 'P':	I2C_Rx_get_version; break;

					case 'Q':	I2C_Tx_LED_dimmer; break;

					/**********Mode'T' is used by Cal_at_Power_on_Reset()***************************/

					case 'U':	start_T2_for_ATMEGA_168_cal(1); break;

					case 'V':	set_diagnostic_mode; break;				//0x3FC is set to 1 when the user enters 'x' at the p/r prompt

					case 'X':	cal_spot_check();break;

					case 'Z':	Initialise_I2C_master_write; I2C_master_transmit(OSCCAL);
					I2C_master_transmit(eeprom_read_byte((uint8_t*)0x3FE));
					I2C_master_transmit(eeprom_read_byte((uint8_t*)0x3FF));
					I2C_master_transmit(OSCCAL_WV);TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
					break;
				}}}



				void I2C_master_transmit(char data){
					TWDR = data;
					TWCR = (1 << TWINT) | (1 << TWEN);
				while (!(TWCR & (1 << TWINT)));}


				char I2C_master_receive(char mode){
					char data;
					if(!(mode))TWCR = (1 << TWINT) | (1 << TWEN);
					else TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
					while (!(TWCR & (1 << TWINT)));
					data =  TWDR;
				return data;}



