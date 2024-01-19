/*
 * Proj_8A_askii_IO.c
 *
 * Created: 19/01/2024 15:24:36
 * Author : Mark
 */ 

#include "Proj_8A_header.h"





/*******Define data streams*****************/
FILE  uart_output = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

FILE uart_input_any_char = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);
FILE uart_input_Hex = FDEV_SETUP_STREAM(NULL, uart_getHex, _FDEV_SETUP_READ);
FILE uart_input_Integer = FDEV_SETUP_STREAM(NULL, uart_getDecimal, _FDEV_SETUP_READ);
FILE uart_input_Long = FDEV_SETUP_STREAM(NULL, uart_getDecimal, _FDEV_SETUP_READ);
FILE uart_input_Double = FDEV_SETUP_STREAM(NULL, uart_getDouble, _FDEV_SETUP_READ);

int uart_getchar_local(FILE *mystr_input);					//accepts any char
int uart_getDecimal_local(FILE *mystr_input);				//Ignores non-Decimal chars
int uart_putchar_local(char c, FILE *mystr_output);


int main(void)
{	char input;
	int integer_number;
	long long_number;
	double Floating_point_num;

	setup_HW;
	stdout = &uart_output;
	stdin  = &uart_input_any_char;

	if ((MCUSR & (1 << PORF)) || MCUSR & (1 << EXTRF)){MCUSR = 0;

	puts("Type c, s, h, d, l, e\n");SW_reset;}				//char, string, hex, decimal, long floating (note: Long float is not supported)

	do{printf("?    ");}while((isCharavailable_Basic(100) == 0));
	input = getchar();
	
	printf("\n");
	
	switch (input){
		case 'c':
		{printf("Enter chars. Return to escape\nEcho:\n"); }
		while(1){input = getchar();
			printf("%c  ", input);
		if((input == '\r') || (input == '\n'))break;}break;
		
		case 's':
		printf("Enter string (space to escape)\nEcho:\n");
		scanf("%s", &input);
		printf("%s  ", &input);
		break;
		
		case 'h':
		stdin  = &uart_input_Hex;
		printf("Enter hex number (zero to escape)\nEcho:");
		while(1){scanf("%x", &integer_number);
			if(!(integer_number))break;
			printf("\n%x  ", integer_number);
			printf("%o  ", integer_number);
			printf("%d  ", integer_number);
		printf("%u  ", integer_number);}
		break;
		
		case 'd':
		stdin  = &uart_input_Integer;
		printf("Enter decimal number <0 or >0 (zero to escape)\nEcho:");
		while(1){scanf("\n%d", &integer_number);
			if(!(integer_number))break;
			printf("\n%d  ", integer_number);
		printf("%x  ", integer_number);}
		break;
		
		
		case 'l':
		stdin  = &uart_input_Long;
		printf("Enter long number <0 or >0 (zero to escape)\nEcho:");
		while(1){scanf("\n%ld", &long_number);
			if(!(long_number))break;
			printf("\n%ld  ", long_number);
		printf("%lx  ", long_number);}
		break;
		
		
		case 'e':																//4.5E3 for example
		stdin  = &uart_input_Double;
		printf("Enter scientific number <0 or >0 (zero to escape)\nEcho:");
		while(1){scanf("%lf", &Floating_point_num);
			if(!(Floating_point_num))break;
		printf("\n%g  ", Floating_point_num);}
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
{while (!(UCSR0A & (1 << RXC0)));
return UDR0;}


/*************************************************************************************/
int uart_getDecimal_local(FILE *mystr_input)
{char keypress;
	{while (!(UCSR0A & (1 << RXC0)));
		keypress = UDR0;
		while(!(decimal_digit_Basic (keypress)) && (keypress != '\r') && (keypress != '\n')&& (keypress != '-'))
		{while((isCharavailable_Basic(100) == 0));keypress = UDR0;}
	return keypress;}}

	/*************************************************************************************/
