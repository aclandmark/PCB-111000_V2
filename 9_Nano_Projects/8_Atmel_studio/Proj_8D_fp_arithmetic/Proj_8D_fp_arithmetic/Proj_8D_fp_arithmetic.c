 

#define BL 30

#include "fp_arithmetic_header.h"

int main(void)
{
	float fpn_1, fpn_2, result=1.0;
	char FPN_as_string[BL];
	char op;
		
	setup_HW;
	String_to_PC_Basic("\r\nEnter fpn (cr) then +,-,*,/,^,% or ~ then  fpn (cr)\r\n");
	
	FPN_string_KBD_to_display(FPN_as_string);
	sscanf (FPN_as_string, "%g", &fpn_1);
	display_8_digit_num_string(FPN_as_string, fpn_1);
	
	op = waitforkeypress_Basic(); Char_to_PC_Basic(op);
	
	while(1){
		if (!(arithmetic_op(op))) {op = '?'; result = fpn_1;}
		if((op != '~') && (op != '?'))
		{FPN_string_KBD_to_display(FPN_as_string);
			sscanf (FPN_as_string, "%g", &fpn_2);
			newline_Basic();
			display_8_digit_num_string(FPN_as_string, fpn_2);}
		
		switch(op){
			case '+':result = fpn_1 + fpn_2; break;
			case '-':result = fpn_1 - fpn_2; break;
			case '*':result = fpn_1 * fpn_2;; break;
			case '/':result = fpn_1 / fpn_2; break;
			case '^':result = pow(fpn_1, fpn_2); break;
			case '%':result = pow(fpn_1,(1/fpn_2)); break;
			case '~':result = 1/fpn_1; break;}
					
		if (op == '?')sprintf(FPN_as_string, "\r\n%g    %c", fpn_1, op);
		else if(op != '~')sprintf(FPN_as_string, "%g    %c    %g  =  %g", fpn_1, op, fpn_2, result);
		else sprintf(FPN_as_string, "\r\n1/%g  =  %g", fpn_1, result);
		Num_String_to_PC(FPN_as_string);
		Timer_T0_10mS_delay_x_m(50);
		display_8_digit_num_string(FPN_as_string, result);
		
		fpn_1 = result;
	op = waitforkeypress_Basic(); Char_to_PC_Basic(op);
	if(op == 'x')SW_reset;}}
	
	

char arithmetic_op(char op){
switch (op)
{case '+': case '-': case '*': case '/':	
case '^': case '%': case '~': return 1;break;}
return 0;}