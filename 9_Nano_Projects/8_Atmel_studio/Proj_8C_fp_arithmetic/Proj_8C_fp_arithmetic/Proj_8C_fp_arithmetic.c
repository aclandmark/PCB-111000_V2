


#include "FP_arithmetic_header.h"

#define BL 15

char fmt_1[8] = "%1.2g";
char fmt_2[8] = "%1.3g";
char fmt_3[8] = "%1.4g"	;
char fmt_4[8] = "%1.5g"	;
char fmt_5[8] = "%g";


int main(void)
{
	float FPN_2;
	char FPN_as_string[BL];
			
    setup_HW;
	String_to_PC_Basic("Enter FPN\r\n");
	
	while(1){
		FPN_string_KBD_to_display(FPN_as_string);
		sscanf (FPN_as_string, "%g", &FPN_2);
		display_8_digit_num_string(FPN_as_string, FPN_2);
				
		waitforkeypress_Basic();
		
		FPN_2 /= 7.0;
		
		sprintf(FPN_as_string, "\t/7=\t%g", FPN_2);
		Num_String_to_PC(FPN_as_string);
				
		display_8_digit_num_string(FPN_as_string, FPN_2);
	String_to_PC_Basic("\tNew_FPN?\r\n");}}
	
	
	
/********************************************************************************************************************************************/
void display_8_digit_num_string(char* display_string,  float FPN_2){
	
	int p;
	char format[8];
	
	char num_format = '1';
	
	do{	
	switch(num_format)
	{	case '1':strcpy(format, fmt_1);break;
		case '2':strcpy(format, fmt_2);break;
		case '3':strcpy(format, fmt_3);break;
		case '4':strcpy(format, fmt_4);break;
		case '5':strcpy(format, fmt_5);return;break;}
			
	for(int m = 0; m <=14; m++)display_string[m] = 0;
	sprintf(display_string, format, FPN_2);
	
	reverse(display_string);
	
		for (int q = 0; q < 2; q++)
		{p=0;
			while ((display_string[p] != 'E') && (display_string[p] != 'e')) {p += 1; if (p==8)break;}
			if((display_string[p-1] == '+') || (display_string[p-1] == '0'))
			{for (int m = p-1; m < 14; m++)display_string[m]=display_string[m+1];}
				
				if((display_string[p-1] == '-') && (display_string[p-2] == '0'))
			for (int m = p-2; m < 14; m++)display_string[m]=display_string[m+1];}
			I2C_Tx_8_byte_array(display_string);num_format += 1;
		}while(!(display_string[7]));}
		


/********************************************************************************************************************************************/
void FPN_string_KBD_to_display(char display_buffer[]){
	char keypress;
	
	char Str_lngth=0;

	for(int n = 0; n < BL; n++) display_buffer[n] = 0;

	while(1){                                                          	//Remain in loop until a valid character is received
		keypress = waitforkeypress_Basic();
		if ((!(decimal_digit_Basic(keypress)))
		&& (keypress != '-')
		&& (keypress != '.'))continue;                                     	//Ignore keypress if it is not OK
		display_buffer[0] = keypress;
	break;}
	
	Increment_display(display_buffer, keypress, &Str_lngth);
	
	while(1){
		if ((keypress = wait_for_return_key_Basic())  =='\r')break;               //Stay in loop until return key press is detected

		if (!(decimal_digit_Basic(keypress)) && (keypress != '.')                 //Check for valid keypresses
		&& (keypress != '-') && (keypress != 'E')
		&& (keypress != 'e') &&  (keypress != '\b'))continue;

		switch (keypress){

			case '\b':  for (int n = 0; n < BL - 1; n++)                             //Backspace keypress
			display_buffer[n] = display_buffer[n + 1];
			display_buffer[BL - 1] = 0;
			Increment_display(display_buffer, keypress, &Str_lngth);break;

			default:
			for(int n = BL - 1; n>=1; n--)                                            //Shift display for each new keypress except '.'
			display_buffer[n] = display_buffer[n-1];
			display_buffer[0] = keypress;                                         //Add new keypress to display
		Increment_display( display_buffer, keypress, &Str_lngth);break;}}
		
		I2C_Tx_any_segment_clear_all();											//Flash display
		Timer_T0_10mS_delay_x_m(10);
		I2C_Tx_8_byte_array(display_buffer);
	reverse (display_buffer);}



/**********************************************************************************************************/
void Increment_display(char*display_buffer, char keypress, char* Str_lngth){
	
	reverse (display_buffer);

	if(*Str_lngth)for(int m = 0; m < *Str_lngth; m++)Char_to_PC_Basic('\b');
	if(( *Str_lngth==1) &&(keypress == '\b'))String_to_PC_Basic("\b ");
	*Str_lngth=Num_String_to_PC(display_buffer);
	reverse (display_buffer);
I2C_Tx_8_byte_array(display_buffer);}





/***********************************************************************************************************************/

	
