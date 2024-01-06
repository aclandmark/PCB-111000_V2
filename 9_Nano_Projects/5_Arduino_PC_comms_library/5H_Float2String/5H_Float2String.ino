
/**************Proj_6E_Acquire_and process_FPN_Data_from_the KBD********************/



#include "Float2String_header.h"

#define Buff_Length  20
#define PIE 3.14159


int main (void){
float Num_1;
float power = 0.5;
char digits[Buff_Length + 2];
float angle;
long ipart;
float fpart;
char counter = 0;
char ndp;
int array_ptr;
int zero_counter;
int Round;
long decimal_part;
long Int_num;

setup_HW_Arduino_IO;


Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n\
Press SW2 to pause the display.\r\n\
SW1 to scroll through the results.\r\n");

Num_1 = FPN_KBD_to_display_A(digits, Buff_Length); 
I2C_FPN_to_display(Num_1);
while(switch_1_down);

Serial.print(Num_1,5);Serial.write("\t\t");
//ipart = (long)Num_1;
ipart = Float2Long(Num_1);
//fpart = Num_1 - (float)ipart;
fpart = Num_1 - Int2Float(ipart);


Serial.print (ipart);Serial.write("\t");
Serial.print (fpart,4);Serial.write ("\t");

Serial.write ("\r\nNumber of decimal places is: ");
ndp = waitforkeypress_A() - '0';
Serial.write(ndp + '0');newline_A();

if(fpart < 0.1){zero_counter = 1;}
else zero_counter = 0;
for(int m = 0; m <= ndp; m++){if((fpart = fpart*10.0)<0.1){zero_counter += 1;}}


decimal_part = (long)fpart; 
decimal_part += 5;
decimal_part/=10;

array_ptr = Int_to_string(ipart,digits,0); 
digits[array_ptr] = '.'; digits[array_ptr + 1] = 0;
if (zero_counter){while(zero_counter>0){digits[array_ptr + 1] = '0'; 
zero_counter -= 1;array_ptr +=1;}}digits[array_ptr + 1] = 0;

Int_to_string(decimal_part, digits, array_ptr+1);
Serial.write(digits);
 
SW_reset;}



/***********************************************************************************************************************/
float Int2Float(long num){
int m;
long twos_expt;

for(m = 0; m <= 31; m++){if(num & ((unsigned long)0x80000000 >> m))break;}
m = 31 - m;
if (m < 23)num = num << (23 - m); 
if (m > 23)num = num >> (m - 23); 
twos_expt = m - 1 + 127;
twos_expt = twos_expt << 23;
num = num + twos_expt;
return (*(float*)(&num));}



/***********************************************************************************************************************/
long Float2Long(float num){
long twos_expt;
long L_num;

twos_expt = (*(long*)&num);
twos_expt = twos_expt << 1;
twos_expt = (unsigned long)twos_expt >> 24;
twos_expt -= 127;

L_num = (*(long*)(&num));
L_num =  ((unsigned long)0x80000000 >> 8) | L_num;
L_num = L_num << 8;
L_num = (unsigned long)L_num >> (31 - twos_expt);
return L_num;}



/***********************************************************************************************************************/
int Int_to_string (long number, char* s, int i)
{ char dig;
int start;

start = i;
  
 while(1){
 dig = number % 10 + '0'; s[start] = dig; i+= 1;
  if(!(number = number / 10)>0)break;
  for(int m = i; m>start; m--)s[m] = s[m-1];}
  s[i] = '\0';
  return i;}



/***********************************************************************************************************************/
void FPN_to_PC(float FPN){
long fpn_as_int;

fpn_as_int = *(long*)(&FPN);
for(int m = 0; m <= 31; m++){if(fpn_as_int & ((unsigned long)0x80000000 >> m)) Serial.write('1'); else Serial.write('0');}}



/***********************************************************************************************************************/
void Long_to_PC(long num){
for(int m = 0; m <= 31; m++){if(num & ((unsigned long)0x80000000 >> m)) Serial.write('1'); else Serial.write('0');}}



/***********************************************************************************************************************/
long Int_Num_from_PC_A_Local(char * num_as_string, char bufferlen)    //Project version implements the backspace key
{char strln;
Serial.flush();   
strln = Serial.readBytesUntil('\r',num_as_string, bufferlen);
num_as_string[strln] = 0;
return atol(num_as_string);}



/****************************************************************************************************************/
