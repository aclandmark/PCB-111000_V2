




#define wdr()  __asm__ __volatile__("wdr")


signed int  read_ops=0;
char dummy_byte;

int main (void){

	char cal_factor=0;
	char target_detected = 0;
	//char keypress;

	/*This program is loaded into the boot section starting at SW location 0x7000*/
	/*Config bit selection ensures that all resets send the program counter to 0x7000*/

	wdr();
	MCUSR &= ~(1<<WDRF);
	WDTCSR |= (1 <<WDCE) | (1<< WDE);
	WDTCSR = 0;

	/*For every reset the default oscillator cal word is automatically loaded by the microprocessor HW*/
	/*Placing the cal read macro here will apply user calibration*/
	/*EEPROM upper address is 0x400* (i.e.1024 1kB) User Cal word must be stored in EEPROM locations
	0x3FE and 0x3FF*/
	
	eeprom_write_byte((uint8_t*)0x3FD, OSCCAL); 		//New line Save OSCAAL

	if ((eeprom_read_byte((uint8_t*)0x3FE) > 0x0F)\
	&&  (eeprom_read_byte((uint8_t*)0x3FE) < 0xF0) && (eeprom_read_byte((uint8_t*)0x3FE)\
	== eeprom_read_byte((uint8_t*)0x3FF))) {OSCCAL = eeprom_read_byte((uint8_t*)0x3FE);
	cal_factor=1;}
	else{cal_factor=0;}


	/*************This program works with 8MHz clock and EEPROM preserved*********************************/
	/*POR or watchdog timer resets are detected and cause the program counter to switch to location 0x0000
	where the application program resides*/

	if (!(MCUSR & (1 << EXTRF)) )						//POR, BOR or watchdog timeout but not the reset switch
	{MCUCR = (1<<IVCE);MCUCR = 0x0;						//select interrupt vector table starting at 0x000
	asm("jmp 0x0000");}
	
	/************************mini_OS reset code*************************************/
	//Reset mini-OS if PORTB4 (MISO) pin set low by user project otherwise continue to programmer
	 
	MCUCR &= (~(1 << PUD));								//Pull up disable set to zero: week pull-up enabled
	DDRB &= (~(1 << DDB4));								//PORTB4 set to input
	PORTB |= (1 << PORTB4);								//PORTB4 set for week pull up.
	
	//USART_init(0,16);
	//_delay_ms(10);
	two_msec_delay;
	
	//for(int m = 0; m < 200; m++){twenty_msec_delay;sendChar('.');}
	
	if ((PINB & 0x10)^0x10)								//If PORTB4 pulled low by user project reset jump to 0x0000
	{MCUCR = (1<<IVCE);MCUCR = 0x0;						//select interrupt vector table starting at 0x000
	asm("jmp 0x0000");}
	
	PORTB &= (~(1 << PORTB4));							//Restore default value	
	
	/**********************************************************************************/
	
	MCUCR = (1<<IVCE);  								//use interrupt vector table starting at start of boot section
	MCUCR = (1<<IVSEL);
	MCUSR &= (~(1 << EXTRF));  							//Reset the external reset flag

	setup_leds;
	ADMUX |= (1 << REFS0);								//select internal ADC ref and remove external supply on AREF pin
	USART_init(0,16);
	//for(int m = 0; m < 200; m++){twenty_msec_delay;sendChar(':');}
	while(1){
		boot_target;
		Atmel_powerup_and_target_detect;
		
		/*****************Power-up and make contact with target****************************/
		if(target_detected == 1){

			while(1){
				do{sendString("p/g  ");}	 while((isCharavailable(300) == 0));

				dummy_byte = receiveChar();
				if ((dummy_byte == 'p'))break;
				if((dummy_byte == 'g') || (dummy_byte == 'x')){
					if (dummy_byte == 'x') {eeprom_write_byte((uint8_t*)0x3FC, 0x1);}

				sendString ("Sw!\r\n");wdt_enable(WDTO_60MS); while(1);}}
				sendString ("\r\nSend file (ATMEGA168):\r\n");
				Start_LED_Activity;

				PageSZ = 0x40; PAmask = 0x1FC0; FlashSZ=0x2000;

				counter = 1;

				prog_counter=0; line_length_old=0;
				Flash_flag = 0;  PIC_address = 0;  section_break = 0; orphan = 0;
				w_pointer = 0; r_pointer = 0;line_counter = 0;


				Atmel_config(Prog_enable_h, 0);

				//while ((keypress = waitforkeypress()) != ':')						//Ignore characters before the first ':'
				//{if (keypress == 'x'){sendString("Reset!\r\n");}}					//x pressed to escape

				/***Erase target flash and program target config space***/
				Atmel_config(Chip_erase_h, 0);
				

				UCSR0B |= (1<<RXCIE0); 	sei();												//UART interrupts now active

				Program_Flash();
				
				Atmel_config(write_extended_fuse_bits_h,0xFF);
				Atmel_config(write_fuse_bits_H_h,0xD5);										//BOD 2.9V
				Atmel_config(write_fuse_bits_h,0xC2);										//0mS SUT 8MHz RC clock
				Atmel_config(write_lock_bits_h,0xEB);
				
				Halt_LED_Activity;
				Verify_Flash();

				Reset_H;																	//Extra line Exit programming mode

				if(prog_counter == read_ops) sendString(" OK"); else sendString("!!??");
				} else{sendChar('!');}									//target not detected during power up and target detect phase

			if(cal_factor==1) sendString("  UC\r\n"); else sendString("  DC\r\n");

		}return 1;}



		ISR(USART_RX_vect){
			unsigned char tempChar=0;
			unsigned char tempChar1;
			int local_pointer;
			
			tempChar1 = receiveChar();
			if (tempChar1 == ':')counter = 0;
			else {if (tempChar1 <= '9') tempChar = tempChar1 - '0'; else tempChar = tempChar1 - '7';}

			switch (counter){
				case 0x0:  break;
				case 0x1: tempInt1 = tempChar<<4;  break;
				case 0x2: tempInt1 += tempChar;  char_count = 9 + ((tempInt1) *2);
				local_pointer = w_pointer++; store[local_pointer] = tempInt1; break;
				case 0x3: tempInt1 = tempChar<<4;  break;
				case 0x4: tempInt1 += tempChar; tempInt1=tempInt1<<8; break;
				case 0x5: tempInt1 += tempChar<<4;  break;
				case 0x6: tempInt1 += tempChar;  local_pointer = w_pointer++; store[local_pointer] = tempInt1; break;
				case 0x7: break;
				case 0x8:  break;
			default: break;}

			if ((counter > 8)&&(counter < char_count)){
				if ((counter & 0x03) == 0x01){tempInt1 = tempChar<<4;}
				if ((counter & 0x03) == 0x02)  {tempInt1 += tempChar;}
				if ((counter & 0x03) == 0x03)  {tempInt2 = tempChar<<4;}
				if ((counter & 0x03) == 0x0)  {tempInt2+= tempChar; tempInt2 = tempInt2<<8;tempInt1+=tempInt2;
				local_pointer = w_pointer++; store[local_pointer] = tempInt1; }}

				counter++;
			w_pointer = w_pointer & 0b00111111;	}


			void Program_Flash (void){
				new_record();  short_line=0;
				start_new_code_block();
				Program_record();
				
				while(1){																					//loop1
					new_record();
					if (line_length==0)break; 																//ISR variable


					if ((Hex_address == PIC_address) && (!(short_line))){									//normal ongoing code block
						if (space_on_page == (PageSZ - offset)){page_address = (Hex_address & PAmask);}
					Program_record();	}

					else{if(Hex_address != PIC_address)														//loop 2
						{																					//normal break	loop 3
							if (section_break){																//PAGE address increases by at least 0x40
								if((Flash_flag) && (!(orphan)))write_page_SUB(page_address);    			//+0x20 for offset pages
							if(orphan) write_page_SUB(page_address + PageSZ);}   							//0x20??
							
							else{																			//loop 4
								if(page_break){if((Flash_flag) && (!(orphan))) write_page_SUB(page_address);
								orphan = 0; }}																//  break within page loop 4
							}																				//loop 3
							start_new_code_block(); Program_record();  if(short_line)short_line=0;			//short_line no break
						}																					//loop 2
					}																						//loop 1
					
					UCSR0B &= (~(1<<RXCIE0));
					while(1){if (isCharavailable(2)==1)receiveChar();else break;}

					if((Flash_flag) && (!(orphan))){write_page_SUB(page_address);}
					if(orphan) {write_page_SUB(page_address + PageSZ);}
				}




				
				void Verify_Flash (void){																	//short version
					int   star_counter;
					signed int phys_address;
					char offset=0;
					read_ops = 0;
					Hex_cmd = 0;
					star_counter = 0;
					phys_address = 0;
					while(1){
						Hex_cmd = Read_write_mem('L',phys_address, 0x0);
						Hex_cmd = (Hex_cmd<<8) + (Read_write_mem('H',phys_address, 0x0));
						phys_address++;
						star_counter++;
						if (phys_address == FlashSZ)break;
						if (Hex_cmd != 0xFFFF){
						read_ops++; if(read_ops >= prog_counter) offset = 4;}

						if (!( star_counter - 200)){sendChar('*' + offset);star_counter = 0;}}}


						void timer_T0_sub(char Counter_speed, unsigned char Start_point){
							TCNT0 = Start_point;
							TCCR0B = Counter_speed;
							while(!(TIFR0 && (1<<TOV0)));
						TIFR0 |= (1<<TOV0); TCCR0B = 0;}

						

						
