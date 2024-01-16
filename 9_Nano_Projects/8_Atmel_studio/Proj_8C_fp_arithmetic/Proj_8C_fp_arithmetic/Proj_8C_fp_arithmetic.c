/*
https://startingelectronics.org/articles/atmel-AVR-8-bit/print-float-atmel-studio-7/
 */ 



#include "FP_arithmetic_header.h"



#define BL 15


char fmt_1[8] = "%1.2g";
char fmt_2[8] = "%1.3g";
char fmt_3[8] = "%1.4g"	;
char fmt_4[8] = "%1.5g"	;
char fmt_5[8] = "%1.6g";
char fmt_6[8] = "%1.7g";
char fmt_7[8] = "%1.8g";
char fmt_8[8] = "%g";


char Num_String_to_PC(char s[]){								//Transmits a sequence (string) of characters and requires the address in program memory of the first one
	int i = 0;																//"i" gives the relative address of the next character to be transmitted
	while(i < 200){															//Transmits up to 200 characters using "Char_to_PC()" or untill the null (\0) character is detected
		if(s[i] == '\0') break;
	Char_to_PC_Basic(s[i++]);
	}return i;}												//Transmit character and increment "i" so that it addresses (points to) the next one.


int main(void)
{
	float FPN_2;
	char source_str[BL];											//Loaded with key presses
	char display_string[BL];
		
    setup_HW;
	
	while(1){
		FPN_string_KBD_to_display(source_str);
		sscanf (source_str, "%g", &FPN_2);
		display_8_digit_num_string(source_str, FPN_2);
		
		waitforkeypress_Basic();
		
		FPN_2 /= 7.0;
		
		sprintf(display_string, "\t/7=\t%g", FPN_2);
		Num_String_to_PC(display_string);
		
		String_to_PC_Basic("\tDisplay format?\r\n");
		display_8_digit_num_string(display_string, FPN_2);
	String_to_PC_Basic("New_FPN?\r\n");}}
	
	
	
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
		case '5':strcpy(format, fmt_5);break;
		case '6':strcpy(format, fmt_6);break;
		//case '7':strcpy(format, fmt_7);break;
		case '8':strcpy(format, fmt_8);return;break;}
	
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
		//waitforkeypress_Basic();	
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
void reverse (char s[]){											//See Joe Pardue's book p 83
	int c,i,j;
	for (i=0, j = strlen(s) - 1; i < j; i++, j--){
		c = s[i];
		s[i] = s[j];
	s[j] = c;}}





/***********************************************************************************************************************/

	
