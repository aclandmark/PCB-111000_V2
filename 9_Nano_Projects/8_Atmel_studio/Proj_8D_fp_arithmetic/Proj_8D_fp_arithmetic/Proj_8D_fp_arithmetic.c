/*
 * Proj_8D_fp_arithmetic.c
 *
 * Created: 25/01/2024 17:54:56
 * Author : Mark
 */ 

#define BL 20
#include "fp_arithmetic_header.h"

int main(void)
{
	float FPN_2;
	char FPN_as_string[BL];
	char keypress;
	
	setup_HW;
	String_to_PC_Basic("Enter FPN\r\n");
	
	while(1){
		FPN_string_KBD_to_display(FPN_as_string);
		sscanf (FPN_as_string, "%g", &FPN_2);
		display_8_digit_num_string(FPN_as_string, FPN_2);
		
		while(1){
			keypress = waitforkeypress_Basic();
			if (keypress == 'd'){FPN_2 /= 7.0; sprintf(FPN_as_string, "\t/7=\t%g", FPN_2);}
			else if (keypress == 'm'){FPN_2 *= 7.0; sprintf(FPN_as_string, "\t*7=\t%g", FPN_2);}
			else break;
			
			Num_String_to_PC(FPN_as_string);
			sprintf(FPN_as_string, "\r\n%g", FPN_2);
			Num_String_to_PC(FPN_as_string);
			
		display_8_digit_num_string(FPN_as_string, FPN_2);}
	String_to_PC_Basic("\r\nNew_FPN?\r\n");}}

