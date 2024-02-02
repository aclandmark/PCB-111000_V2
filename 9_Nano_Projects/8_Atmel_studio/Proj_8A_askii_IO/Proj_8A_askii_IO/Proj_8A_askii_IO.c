


/*Proj_8A_askii_IO
********************************************/

/*

For the PCB111000 project Atmel studio was chosen to develope the mini_OS SW
It allows more complex applications to be split into several smaller ones and subsequently combined
It also allows use of assembly language programing
and gives easy access to the status register for reset analysis

It also supports a subsection of the standard C library for IO of floating point numbers.

It is possible that all these advantages are also availsable with Arduino however I believe that both
applications have their own special advantages.  I would not want to be without either and stronly recommend
that programmers become familiar with both.

The one trouble with Atmel studio is getting it to support the floating point library.

To do this click on the Project tab and then select properties
click on "miscelleneous" under "AVR/GNU linker"
Paste the following strings into the "Other Linker Flags" box

-Wl,-u,vfscanf -lscanf_flt -lm -Wl,-u,vfprintf,-lprintf_flt,-lm




This project introduces

1.	Use of the standard C-libraries to format input/output to and from the PC.

2.	Library subroutine "scanf" which converts keyboard input to a variety of different formats 
	including string and numeric.

3.	Library subroutine "printf" which converts binary data to a variety of different numeric formats 
	and combines it with text before sending it to the PC screen.

4.	Project subroutines uart_getHex(); uart_getDecimal(); and uart_getDouble(); all used to acquire 
	data and all designed to ignore any illegal key presses.

6.	Project subroutine "uart_putchar" used to send characters to the PC for display.

5.	The "stdin" and "stdout" functions that determine which of the "uart" functions will be active.

6.	Data streams into which characters are received from the PC and from which they are sent to the PC.

7.	Input & output of strings, hex numbers, decimal numbers, long numbers and floating point numbers.


Note: 
This program is based completely on the results of internet searches.

See for example:
https://www.tutorialspoint.com/c_standard_library/c_function_scanf.htm
 and
https://appelsiini.net/2011/simple-usart-with-avr-lib
A good text is probally essential to make head and tail of any of it 
*/

 

#include "Proj_8A_header.h"


/*******Define data streams*****************/
FILE  uart_output = FDEV_SETUP_STREAM (uart_putchar_local, NULL, _FDEV_SETUP_WRITE);

FILE uart_input_any_char = FDEV_SETUP_STREAM(NULL, uart_getchar_local, _FDEV_SETUP_READ);
FILE uart_input_Hex = FDEV_SETUP_STREAM(NULL, uart_getHex, _FDEV_SETUP_READ);
FILE uart_input_Integer = FDEV_SETUP_STREAM(NULL, uart_getDecimal_local, _FDEV_SETUP_READ);
FILE uart_input_Long = FDEV_SETUP_STREAM(NULL, uart_getDecimal, _FDEV_SETUP_READ);
FILE uart_input_Double = FDEV_SETUP_STREAM(NULL, uart_getDouble, _FDEV_SETUP_READ);

int uart_getchar_local(FILE *mystr_input);					//accepts any char
int uart_getDecimal_local(FILE *mystr_input);				//Ignores non-Decimal chars
int uart_putchar_local(char c, FILE *mystr_output);

#define BL 20

int main(void)
{	char input, input_string[BL+2];
	int integer_number;
	long long_number;
	double Floating_point_num;						//double is not supported. Compiler substitutes float for double
	
	

	setup_HW;
	stdout = &uart_output;
	stdin  = &uart_input_any_char;

	if ((MCUSR & (1 << PORF)) || MCUSR & (1 << EXTRF)){MCUSR = 0;

	puts("Type c, s, h, d, l, e");SW_reset;}				//char, string, hex, decimal, long floating (note: Long float is not supported)

	do{printf("?    ");}while((isCharavailable_Basic(100) == 0));
	input = getchar();
	
	printf("\t");
	
	switch (input){
		case 'c':
		{printf("\nEnter chars. Return to escape\nEcho:\t"); }
		while(1){input = getchar();
			printf("%c  ", input);
		if((input == '\r') || (input == '\n'))break;}break;
		
		case 's':
		printf("\rEnter string:\t");
		scanf("%[^\r]", input_string);
		printf("\t%s  ", input_string);
		break;
		
		case 'h':
		stdin  = &uart_input_Hex;
		printf("\r\rEnter hex number (zero to escape)");
		while(1){scanf("%x", &integer_number);
			if(!(integer_number))break;
			printf("\n%x  ", integer_number);
			printf("%o  ", integer_number);
			printf("%d  ", integer_number);
		printf("%u  ", integer_number);}
		break;
		
		case 'd':															//Allows use of the Delete key
		stdin  = &uart_input_Integer;
		printf("\r\rEnter decimal number (zero to escape, backspace supported)\n");
		while(1){putchar(' '); scanf("%s", input_string);
		del_key_press_remover(input_string);
		integer_number = atoi(input_string);
		if(!(integer_number))break;
		printf("\t%d  ", integer_number);
		printf("\t%x\r", integer_number);}
		break;
				
		case 'l':
		stdin  = &uart_input_Long;
		printf("\r\rEnter long number (zero to escape)\n");
		while(1){scanf("\n%ld", &long_number);
			if(!(long_number))break;
			printf("\n%ld  ", long_number);
		printf("%lx  ", long_number);}
		break;
				
		case 'e':							
		stdin  = &uart_input_Double;
		printf("\r\rEnter scientific number (zero to escape, backspace supported)\n");
		while(1){putchar(' ');scanf("%s", input_string);
			del_key_press_remover(input_string);
			Floating_point_num =  atof(input_string);
			if(!(Floating_point_num))break;
		printf("\t\t%g  ", Floating_point_num);
		printf("\t%1.5e\r", Floating_point_num);}
		break;
		
	default: break;}
	
	SW_reset;
return 0;}



/*************************************************************************************/
int uart_putchar_local(char c, FILE *mystr_output)
{if (c == '\n')
	uart_putchar_local('\r', mystr_output);
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
return 0;}



/*************************************************************************************/
int uart_getchar_local(FILE *mystr_input)
{char x;
while (!(UCSR0A & (1 << RXC0)));
	x = UDR0;
	if ((x!='\r') && (x!='\n'))putchar(x);
	if (x == '\b')return'#';
	return x;}



/*************************************************************************************/
int uart_getDecimal_local(FILE *mystr_input)
{char keypress;
	{while (!(UCSR0A & (1 << RXC0)));
		keypress = UDR0;
		while(!(decimal_digit_Basic (keypress)) && (keypress != '\r') && (keypress != '\n')&& (keypress != '-')&&(keypress != '\b'))
		{while((isCharavailable_Basic(100) == 0));keypress = UDR0;}
			
	if ((keypress !='\r') && (keypress !='\n'))putchar(keypress);		
	if (keypress == '\b')return'#';		
	return keypress;}}



	/*************************************************************************************/
void del_key_press_remover(char * num_as_string)
{int strln;
	int trailing_bs_counter = 0;

	Timer_T0_10mS_delay_x_m(1);	
	strln = strLen(num_as_string);
		
	//Remove trailing delete chars******************************************************************************************
	for(int m = strln; m; m--){if(num_as_string[m-1] == '#')trailing_bs_counter += 1;else break;}
	for(int m = 0; m < (trailing_bs_counter * 2); m++){if(strln == m)break; else num_as_string[strln - m-1] = '\0'; }

	//Remove leading delete chars******************************************************************************************
	for(int m = 0; m < strln; m++){
		while(num_as_string[0] == '#')
		{for(int p = 0; p < strln-1; p++){num_as_string[p] = num_as_string[p+1];num_as_string[p+1] = 0;m = 0;}}
		
		//Remove remaining del chars*********************************************************************************************
		if(num_as_string[m] != '#');
		else for(int p = m; p < strln-1; p++){num_as_string[p-1] = num_as_string[p+1]; num_as_string[p+1] = 0;num_as_string[p] = 0;m = 0;}}

		num_as_string[strln] = 0;}



/*************************************************************************************/
int strLen(char s[]){
	int i;
	i=0;
	while(s[i] != '\0') ++i;
	return i;}