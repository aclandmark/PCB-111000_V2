
#include "Prime_numbers_header.h"



int main (void){

char counter=9, group = 1;
unsigned int prime_num = 1;  
                                                                                                                                    
 setup_HW;

String_to_PC_Basic("New method_2\r\n1  ");
 while(1){
prime_num = next_prime_number(prime_num);

if(prime_num > 200*group)
{newline_Basic(); 
counter = 10; 
group += 1;
waitforkeypress_Basic();}

if (counter)                                   //Print them out 10 at a time
{if (counter == 10)
String_to_PC_Basic("\r\n");
Int_to_PC_Basic(prime_num); 
Char_to_PC_Basic(' ');
counter -= 1;

if(!(counter)){counter = 10;}}

}}

/***************************************************************************************************************************/
unsigned long next_prime_number(unsigned long num){
 
 while (!(prime_num_check(++num)));
 return num;}


/***************************************************************************************************************************/
char prime_num_check(unsigned int Num)                        
{   
int i = 2;   
int m;  
 while(1){  
if (i*i > Num)break;  
m=0;
while(1){
if(i*(i+m) == Num)return 0;
if(i*(i+m) > Num)break;
m += 1;}  
i += 1;}
return 1;}

/****************************************************************************************************************************/
