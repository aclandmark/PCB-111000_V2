
long FPN_to_Significand(float, long *, char *);
long Fraction_to_Binary_Signed(long, long);
float Assemble_FPN(unsigned long, int, char);
long unpack_FPN(float, int *, char*);
void Print_long_as_binary(long);
float Scientifc_num_to_FPN(float, char);

long Get_fpn_from_KBD(char *, int *, char *, long * , char *, int);   
void Check_num_for_to_big_or_small(float);

int strLength (char*);

char PCMSK0_backup, PCMSK2_backup, float_display_mode;



/*****************************************************************************/
#define pci_on_sw1_and_sw3_enabled (PCMSK2 & 0x84) == 0x84
#define pci_on_sw2_enabled (PCMSK0 & 0x40) == 0x40
#define PCIenabled ((pci_on_sw1_and_sw3_enabled) || (pci_on_sw2_enabled))
#define disable_pci_on_sw1_and_sw3  PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));
#define disable_pci_on_sw2  PCMSK0 &= (~(1 << PCINT6));



/*****************************************************************************/
#define I2C_Tx_float_display_control \
{\
PCMSK0_backup= PCMSK0;\
PCMSK2_backup= PCMSK2;\
float_display_mode = '0';\
if (PCIenabled){disable_pci_on_sw2;disable_pci_on_sw1_and_sw3;}\
while(1){wdr();\
if(switch_2_down)float_display_mode = '1'; else float_display_mode = '0';\
if(switch_1_down)float_display_mode = '2';\
waiting_for_I2C_master;\
send_byte_with_Nack(float_display_mode);\
clear_I2C_interrupt;\
if(float_display_mode == '2')break;}\
PCMSK0 = PCMSK0_backup;\
PCMSK2 = PCMSK2_backup;}



float Scientific_number_from_KBD(char * digits, char *sign, int BL){

long Significand;
long  twos_denominator;
char   tens_expnt;
int twos_expnt;
long FPN_digits;
float FPN;
char sign_local;


Significand = Get_fpn_from_KBD(digits, &twos_expnt, &tens_expnt, &twos_denominator, &sign_local, BL);              //Can be positive or negative
FPN_digits = Fraction_to_Binary_Signed(Significand, twos_denominator);                            					//0.1011000011.... for example
if (!(FPN_digits)) return 0;
else
{FPN = Assemble_FPN(FPN_digits, twos_expnt, sign_local);
FPN = Scientifc_num_to_FPN(FPN, tens_expnt);
*sign = sign_local;
Check_num_for_to_big_or_small(FPN);
return FPN;}}




/******************************************************************************/
long Fraction_to_Binary_Signed(long rem, long Den){			//Converts rem/Den to 0.1xxxxxxxxxx... where x is either 0 or 1
long Result = 0; 						
unsigned int res_LB=0; 
char sign = '+';

if((rem >= -0x40000000) && (rem < 0x40000000) &&
(Den >= -0x40000000) && (Den < 0x40000000));
else{rem /= 2; Den /= 2;}

if ((rem < 0) || (Den < 0)) sign = '-';
if ((rem < 0) && (Den < 0)) sign = '+';

for(int n = 1; n <= 15; n++){								//bit 0 is reserved for sign bit
if ((2*(rem))/Den)(Result) |= (1 << (15-n));				//Uses 32 integer divisions to obtain the bicimal points
rem = (2*(rem))%Den; }
Result = Result << 16;
for(int n = 0; n <= 15; n++){
if ((2*(rem))/Den)(res_LB) |= (1 << (15-n));
rem = (2*(rem))%Den;}
Result += res_LB;											//Result is not affected by the signs of rem and Den
if(sign == '-')Result = Result * (-1);						//Inverting the sign bit is equivalent to adding -1
return Result;}



/******************************************************************************/
/*
10 is (10/16) x 2^4 = 0.101 0000 0000 0000 00000 0000 0000 0000 = 0x50000000 x 2^4 in a form suitable for arithmetic
0.1 is (8/10) / 2^3 = 0.110 0110 0110 0110 0110 0110 0110 0110 = 0x66666666 / 2^3 in a form suitable for arithmetic
*/

float Scientifc_num_to_FPN(float FPN, char tens_expnt )
{
int twos_expnt, twos_expnt_old;
long FPN_digits;
int FPN_shift;
char sign;


FPN_digits = unpack_FPN(FPN, &twos_expnt, &sign);						//Returns positive number with sign

if (tens_expnt > 0 ){
for(int m = 0; m < tens_expnt; m++){wdr();

while (FPN_digits >= 0x66666666)										//Multiply by 10: (Divide by 0.1) Use denominator of 0x50000000 with a tws_exponent of 3
{FPN_digits /= 2; twos_expnt += 1;}

FPN_digits = 
Fraction_to_Binary_Signed(FPN_digits, 0x66666666);

for(int m = 0; m <= 2; m++){
if (twos_expnt < 128)twos_expnt += 1;
else {twos_expnt = 129;FPN_digits = 0;}}}}


if (tens_expnt < 0 ){
for(int m = 0; m < tens_expnt * -1; m++){wdr();

while (FPN_digits >= 0x50000000)										//Divide by 10 Use denominator of 0x50000000 with a twos_exponent of 4
{FPN_digits /= 2; twos_expnt += 1; }

FPN_digits = 
Fraction_to_Binary_Signed((long)FPN_digits, 0x50000000 );

for(int m = 0; m <= 3; m++){
if (twos_expnt > -160)twos_expnt -= 1;}}}

FPN = Assemble_FPN((unsigned long)FPN_digits, twos_expnt, sign);		//Returns signed number

return FPN;}



/*********************************************************************/
void I2C_Tx_float_num(long L_number, char expnt){
char s[5];
char num_bytes=5; char mode='K';
for(int m=0; m<=3; m++){s[m] = (L_number >> (8*(3-m)));}
s[4] = expnt;
I2C_Tx(num_bytes,mode, s);}



/*********************************************************************/
void I2C_FPN_to_display(float num){
  long Denominator;
  long Significand;
  char expnt;

Significand = FPN_to_Significand(num, &Denominator, &expnt);
Significand = Fraction_to_Binary_Signed(Significand, Denominator);
I2C_Tx_float_num(Significand, expnt);
I2C_Tx_float_display_control;}



/************************************************************************/
float Assemble_FPN(unsigned long FPN_digits, int twos_expnt, char sign)			//The FPN to be assembled is provided as a long numer (FPN_digits) which has been shifted left until 
{int shift = 0;																	//bit 30 is a 1.  There is also a twos_expnt which can be multiplied with the long number to give     
int twos_expnt_BKP;																//the required FPN assuming that a binary point is placed immediately to the left of the digits. 

twos_expnt_BKP = twos_expnt;													//The 2's exponent is equal to the numner of bits to the left of the binary point

if(twos_expnt_BKP >= -125)														//FNP > ~1.18E-38 (Shift FPN_digits 7 places to the right and clear the 1 at the MSB (bit 23)
{FPN_digits = FPN_digits >> 6;													//Step 1:  Shift FPN_digits 6 places to the right
FPN_digits += 1;																//Step 2:  Round the LSB. 						
FPN_digits = FPN_digits >> 1;													//Step 3:  Shift the number one more place so that the MSB is located at bit 23 (ie the FPN has 24 bits)
twos_expnt += 126;									
FPN_digits = FPN_digits  &  (~((unsigned long)0x80000000 >> 8));				//Clear bit 23 (which is always 1)
FPN_digits = FPN_digits | ((long)twos_expnt << 23);}							//Exponent can now occupy bits 23 to 30 (bit 31 reserved for sign)

if(twos_expnt_BKP <= -126){														//For 2's exponent of -126 Shift FPN_digits 8 places to the right (i.e. 2's exponent -118) 						
shift = -(118 + twos_expnt);
if (twos_expnt_BKP >= -146)														//Provided 2's exponent >= 146 the LSB can be rounded 
{FPN_digits = FPN_digits >> (shift -1);											//First shift
FPN_digits += 1;																//Rounding						
FPN_digits = FPN_digits >> 1;}													//Shift rounded bit out
else FPN_digits = FPN_digits >> shift;											//Rounding not implemented. The FPN appraoches 1.4E-45
twos_expnt = 0;}																//Bits 23 to 30 left as zero (line probably not needed)

if (sign == '-')FPN_digits = FPN_digits | (unsigned long)0x80000000;			//Rienstate sign bit
return *(float*)&FPN_digits;}													//Return FPN_digits using a float pointer



/*********************************************************************/
long unpack_FPN(float FPN, int *twos_expnt, char * sign)
{long FPN_digits;
int shift;

FPN_digits = (*(long*)&FPN);													//Read the FPN using a long pointer
if( FPN_digits & (((unsigned long)0x80000000)))
{ FPN_digits = FPN_digits & (~((unsigned long)0x80000000));						//If negative remove sign bit
*sign = '-';}else *sign = '+';

*twos_expnt = (FPN_digits >> 23) - 127;											//Expponent occupies bits 0 to 7

if(*twos_expnt >=-126){				
FPN_digits = (FPN_digits & 0x7FFFFF);											//Isolate bits zero to 22
FPN_digits |= ((unsigned long)0x80000000 >> 8);									//Add the unsaved 1 to bit 23
*twos_expnt += 1;
FPN_digits = FPN_digits << 7;													//Shift left 7 places until bit 30 is a 1
return FPN_digits;}

if(*twos_expnt == -127){														//Bit 23 will not be set to 1, 2's exponent is unknown
if(!(FPN_digits))return 0;														//If exponent is -127 shift is 9 MSB is bit 21
else																			//If exponent is -128 shift is 10 MSB is bit 20
{shift = 0;
while (!(FPN_digits & 0x40000000)){FPN_digits <<= 1; shift += 1;}
*twos_expnt = -(118 + shift);
return FPN_digits;}}}




/*********************************************************************/

void FPN_to_String(float FPN, char pre_dp, char post_dp, char next_char, char * print_string){

char tens_expnt, print_expnt;
int twos_expnt;
char sign;
long FPN_as_long, FPN_as_long_bkp;
long Denominator;
long int_part_max;
int Num_digits;
float round_denom;
float FPN_bkp;



if ((*(long*)(&FPN) == 0x80000000) || (*(long*)(&FPN) == 0))
{print_string[0] = '0'; print_string[1] = '.';print_string[2] = '0';
print_string[3] ='\r'; print_string[4] ='\n';print_string[5] = '\0';return;}					//+/- zero case

Num_digits = pre_dp + post_dp; 

if (*(long*)&FPN & (unsigned long) 0x80000000)
{(*(long*)&FPN &= 0x7FFFFFFF);sign = '-';} else sign = '+';										//determine sign and convert -ve numbers to +ve
tens_expnt = 0;
int_part_max = 1;
for(int m = 0; m < pre_dp; m++) int_part_max *= 10.0;											//for 3 digits before the dp the max integer part is set to 1000

FPN_bkp = FPN;

if(FPN_bkp >= (float)int_part_max){																//For numbers above the max integer value
while  (FPN >= (float)int_part_max){FPN /= 10.0; tens_expnt += 1;}								//Divide FPN by 10 until its integer part is below the max allowed value
print_expnt = tens_expnt;}																		//Increment the tens exponent accordinglly and save the result for printing

if(FPN_bkp < (float)int_part_max){																//Repeat for numbers below the max integer valur
while  (FPN < (float)int_part_max){FPN *= 10.0; tens_expnt -= 1;}
print_expnt = tens_expnt+1;}

while (FPN >= 1.0){FPN /= 10.0; tens_expnt += 1;}												//Convert to the form 0.123456789  (>= 0.1 to < 1)



/*****************************************Build the number 0.000000005 used for rounding*********************************/  
round_denom = 1.0;
for(int m = 0; m <= Num_digits; m++)round_denom *= 10.0; 
FPN = FPN + (5.0/round_denom);

if(FPN >= 1.0){FPN /= 10.0;tens_expnt += 1; print_expnt = tens_expnt-pre_dp;}				//Only runs if rounding increases FPN to 1

//newline_A();Serial.print(FPN);Serial.write("  ");



/*****************************************Obtain the number 12345678 in binary form***************************************/
if (sign == '-')  *(long*)& FPN |= (unsigned long) 0x80000000;
FPN_as_long = unpack_FPN(FPN, &twos_expnt, &sign);											//for FPN = 0.1 2's exponent is -3 and for 0.999 it is 0
FPN_as_long = FPN_as_long >> 4 ; 															//Occupies bits 0 to 27
Denominator = 0x8000000 << (-twos_expnt);													//2^(27 - 2's exponent) 

//Serial.print(-twos_expnt); Serial.write("  ");
//Serial.print(FPN_as_long,HEX); Serial.write("  ");Serial.print(Denominator,HEX); newline_A();



/****************************************Convert 12345678 to string form***************************************************/
{int p = 0;
Denominator /= 10.0; 

if(sign == '-')																				//Start building string for -ve numbers and those with 0.
{print_string[0] = '-'; p += 1;}
if (!(pre_dp))print_string[p++] = '0'; 
for (int m = 0; m < Num_digits; m++){
FPN_as_long_bkp = FPN_as_long/Denominator;													//Dividing FPN_as_long by the denominator isolates the most significant digit

if(m == pre_dp)print_string[p++] = '.'; 													//Add a dp is encountered add it to the string
if(FPN_as_long_bkp){print_string[p] = (FPN_as_long_bkp + '0');								//Save digit as a char 
FPN_as_long = FPN_as_long%Denominator;} 													//Remainder of dividing FPN_as_long by the denominator
else print_string[p] = '0'; 
p += 1;
FPN_as_long *= 10;}																			//Prepare to isolate the next most significant digit

if(print_expnt) {print_string[p++] = 'E'; 													//If there is an exponent add 'E' to the string
itoa(print_expnt, print_string+p, 10);}														//Convert print_exponent to askii and add them to the string 
else print_string[p++] = '\0';

p = 0;																						//Locate the null char and replace it with next_char
while (print_string[p++]); p -= 1;
print_string[p++] = next_char;																//Reinstate the null char
print_string[p] = '\0';}}



/*******************************************************************************************************************/
void display_FPN_short(float FPN, char * num_string){ 
char digits[8],sign, range;

for(int m = 0; m <=7; m++)digits[m] = 0;
if (FPN < 0) {sign = '-'; FPN *= -1.0;} else sign = '+';

if (FPN < 1.0e-9)range = 2;
if((FPN >= 1.0e-9 ) && (FPN < 1.0))range = 3;
if ((FPN >= 1.0) && (FPN < 10.0))range = 6;
if ((FPN >= 10.0) && (FPN < 1.0e10))range = 4;
if (FPN >= 1.0e10)range = 3;

if (sign == '-') {range -= 1;FPN *= -1.0;FPN_to_String(FPN, 1, range,'\0',num_string); }

else FPN_to_String((FPN), 1, range,'\0',num_string);
{int m = 0; while(num_string[m]) {digits[7-m] = num_string[m]; m += 1;}} 


/*********************Remove unwanted trailing zeros**************************************************/
{int p = 0;
while (!(digits[0])){for(int m = 0; m < 7; m++)                                       //short string: remove trailing null terms 
digits[m] = digits[m+1];digits[7]=0;}
   
while(p < 8){if (digits[p] == 'E') 
{p+=1;
if (digits[p] == '0'){for(int m = p; m < 7; m++)digits[m] = digits[m+1];digits[7]=0;   //Remove up to two zeros proceeding 'E'
p+=1;if (digits[p] == '0'){for(int m = p; m < 7; m++)digits[m] = digits[m+1];}
p-=1;if (digits[p] == '.')
{for(int m = 7; m > p; m--)digits[m] = digits[m-1];digits[p] = '0';}}                    //Display .0E rather than .E

I2C_Tx_8_byte_array(digits);return;}

else p+=1;}

p=0;
while(p < 8){if (digits[p] == '.')break; else p+=1;}                                    //No E and no decimal point. Display the full array
if (p==8){I2C_Tx_8_byte_array(digits);return;} 

p=0;                                                                                    //No E but decimal point
while (digits[0] == '0'){for(int m = 0; m < 7; m++)
digits[m] = digits[m+1];digits[7] = 0;}                                                 //Remove trailing zeros 
if(digits[0] == '.'){for(int m = 0; m < 7; m++)
digits[7-m] = digits[6-m]; digits[0] = '0';}}                                           //Display .0E rather than .E

I2C_Tx_8_byte_array(digits);}







/*void display_FPN_short(float FPN, char * digits_2){OLD VERSION	
char digits[8],sign, range;

for(int m = 0; m <=7; m++)digits[m] = 0;
if (FPN < 0) {sign = '-'; FPN *= -1.0;} else sign = '+';

if (FPN < 1.0e-9)range = 2;
if((FPN >= 1.0e-9 ) && (FPN < 1.0))range = 3;
if ((FPN >= 1.0) && (FPN < 10.0))range = 6;
if ((FPN >= 10.0) && (FPN < 1.0e10))range = 4;
if (FPN >= 1.0e10)range = 3;

if (sign == '-') {range -= 1;FPN *= -1.0;FPN_to_String(FPN, 1, range,'\0',digits_2); }

else FPN_to_String((FPN), 1, range,'\0',digits_2);
{int m = 0; while(digits_2[m]) {digits[7-m] = digits_2[m]; m += 1;}} 

I2C_Tx_8_byte_array(digits);}*/




/********************************************************************************************************************/