
#include "Prime_numbers_header.h"



int main (void){

char counter=0;
unsigned int Number=0;  
                                                                                                                                    
 setup_HW;

String_to_PC_Basic("New method");
 
 while (1){
for(int m = 1; m <= 100; m++){

if(prime_num_check(Number + m))
{ Int_to_PC_Basic(Number + m);I2C_Tx_long(Number+m);_delay_ms(100);
counter += 1;
if (counter==10){counter = 0;newline_Basic();}}} 

while(1){if (waitforkeypress_Basic() != 'x');
else break;}
Number += 100;
String_to_PC_Basic("\r\n\r\n");counter = 0;}

}

/***************************************************************************************************************************/
char prime_num_check(int Num)
{            
int i = 2;                        
int m;                            

while(1){
  if (i*i > Num)break;
  m = 1;
while(1){
  if (i*m == Num)return 0;
  if (i*m > Num) break;
  m += 1;
}
i += 1;}
return 1;}





/****************************************************************************************************************************/
