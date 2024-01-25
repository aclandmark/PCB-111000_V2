
/*
Uses DIY data entry into string with sscanf to decode the string and sprintf to replenish it
DIY coding required to match the string to 8 digit display but not the PC

*/



#include "Basic_IO_header.h"

#define BL 15

char fmt_1[8] = "%1.1e";
char fmt_2[8] = "%1.2e";
char fmt_3[8] = "%1.3e"	;
char fmt_4[8] = "%1.4e"	;
char fmt_5[8] = "%1.5e"	;
char fmt_6[8] = "%1.6e"	;
char fmt_7[8] = "%g";


int main(void)
{
	float FPN_2;
	char FPN_as_string[BL];
	char keypress;
	
	setup_HW;
	String_to_PC_Basic("Enter FPN\r\n");
	
	while(1){
		FPN_string_KBD_to_display_local(FPN_as_string);
		sscanf (FPN_as_string, "%g", &FPN_2);
		display_8_digit_num_string_local(FPN_as_string, FPN_2);
		
		while(1){
			keypress = waitforkeypress_Basic();
			if (keypress == 'd'){FPN_2 /= 7.0; sprintf(FPN_as_string, "\t/7=\t%g", FPN_2);}
			else if (keypress == 'm'){FPN_2 *= 7.0; sprintf(FPN_as_string, "\t*7=\t%g", FPN_2);}
			else break;
			
			Num_String_to_PC(FPN_as_string);
			sprintf(FPN_as_string, "\r\n%g", FPN_2);
			Num_String_to_PC(FPN_as_string);
			
		display_8_digit_num_string_local(FPN_as_string, FPN_2);}
	String_to_PC_Basic("\r\nNew_FPN?\r\n");}}
	
	
	
	/********************************************************************************************************************************************/
	void display_8_digit_num_string_local(char* display_string,  float FPN_2){
		
		int p;
		char format[8];
		
		char num_format = '1';
		
		do{
			switch(num_format)
			{	case '1':strcpy(format, fmt_1);break;
				case '2':strcpy(format, fmt_2);break;
				case '3':strcpy(format, fmt_3);break;
				case '4':strcpy(format, fmt_4);break;
				case '5':strcpy(format, fmt_5);break;
				case '6':strcpy(format, fmt_6);break;
			case '7':strcpy(format, fmt_7);break;}
			
			for(int m = 0; m <=14; m++)display_string[m] = 0;
			sprintf(display_string, format, FPN_2);
			
			reverse(display_string);
			
			for (int q = 0; q < 2; q++)
			{p=0;
				while ((display_string[p] != 'E') && (display_string[p] != 'e'))									//Locate the E/e symbol
				{p += 1; if (p==8)break;}																			//No E/e is present
				if((display_string[p-1] == '+') || (display_string[p-1] == '0'))									//Remove "+" sign from exponent
				{for (int m = p-1; m < 14; m++)display_string[m]=display_string[m+1];}								//Remove leading zero from positive exponent
				
				if((display_string[p-1] == '-') && (display_string[p-2] == '0'))
			for (int m = p-2; m < 14; m++)display_string[m]=display_string[m+1];}								//Remove leading zero from negative exponent
			
			if ((display_string[0]== '0') && ((display_string[1]== 'e')||(display_string[1]== 'E')))			//Remove the redundant "E0" string
			for (int q = 0; q < 2; q++)
			for (int m = 0; m < 14; m++)display_string[m]=display_string[m+1];
			
			num_format += 1;
			
		}while((!(display_string[7])) && (num_format <= '7'));													//Exit when all format strings tested or display is full
		
		p=0;
		while ((display_string[p] != 'E') && (display_string[p] != 'e'))
		{p += 1; if (p==8)break;}
		if (p !=8)																								//E/e present
		{p += 1; while(display_string[p] == '0')
			{for(int m = p; m < 14; m++ )display_string[m] =display_string[m+1];}									//Remove redundant trailing zeros
			if(display_string[p] == '.'){for(int m = 0; m < 14; m++ )
			display_string[14-m] =display_string[13-m];display_string[0]='0';}}										//Re-insert zero following a decimal point
			
			
			if (p==8){																								//No exponent term
				while (display_string[0] == '0')
				{for(int m = 0; m < 14; m++ )display_string[m] =display_string[m+1];}									//Remove redundant trailing zeros
				if(display_string[0]=='.') {for(int m = 0; m < 14; m++ )
				display_string[14-m] =display_string[13-m];display_string[0]='0';}}										//Re-insert zero following a decimal point
				
			I2C_Tx_8_byte_array(display_string);}
			


			/********************************************************************************************************************************************/
			void FPN_string_KBD_to_display_local(char display_buffer[]){
				char keypress;
				
				char Str_lngth=0;

				for(int n = 0; n < BL; n++) display_buffer[n] = 0;

				while(1){                                                          			//Remain in loop until a valid character is received
					keypress = waitforkeypress_Basic();
					if ((!(decimal_digit_Basic(keypress)))
					&& (keypress != '-')
					&& (keypress != '.'))continue;                                     		//Ignore keypress if it is not OK
					display_buffer[0] = keypress;
				break;}
				
				Increment_display_local(display_buffer, keypress, &Str_lngth);
				
				while(1){
					if ((keypress = wait_for_return_key_Basic())  =='\r')break;						//Stay in loop until return key press is detected

					if (!(decimal_digit_Basic(keypress)) && (keypress != '.')						//Check for valid keypresses
					&& (keypress != '-') && (keypress != 'E')
					&& (keypress != 'e') &&  (keypress != '\b'))continue;

					switch (keypress){

						case '\b':  for (int n = 0; n < BL - 1; n++)								//Backspace keypress
						display_buffer[n] = display_buffer[n + 1];
						display_buffer[BL - 1] = 0;
						Increment_display_local(display_buffer, keypress, &Str_lngth);break;

						default:
						for(int n = BL - 1; n>=1; n--)												//Shift display for each new keypress except '.'
						display_buffer[n] = display_buffer[n-1];
						display_buffer[0] = keypress;												//Add new keypress to display
					Increment_display_local( display_buffer, keypress, &Str_lngth);break;}}
					
					I2C_Tx_any_segment_clear_all();													//Flash display
					Timer_T0_10mS_delay_x_m(10);
					I2C_Tx_8_byte_array(display_buffer);
				reverse (display_buffer);}



				/**********************************************************************************************************/
				void Increment_display_local(char*display_buffer, char keypress, char* Str_lngth){
					
					reverse (display_buffer);

					if(*Str_lngth)for(int m = 0; m < *Str_lngth; m++)Char_to_PC_Basic('\b');
					if(( *Str_lngth==1) &&(keypress == '\b'))String_to_PC_Basic("\b ");
					*Str_lngth=Num_String_to_PC(display_buffer);
					reverse (display_buffer);
				I2C_Tx_8_byte_array(display_buffer);}





				/***********************************************************************************************************************/

				
