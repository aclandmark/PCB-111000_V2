
#include "Prime_factors_header.h"

/************************************************************************************************************/
int main (void)  
  { 
    char num_string[12];
    long  number = 6076;
    long factor=0;
    char factor_counter;
  long prime_num = 1, next_prime;
  
 setup_HW;
if(watch_dog_reset == 1)String_to_PC_Basic("\r\n?");
else String_to_PC_Basic("Factors: Enter number\r\n");

number = Int_from_PC_Basic( num_string);
while(1){
next_prime =  next_prime_number(prime_num);
while (!(number%next_prime))
{Int_to_PC_Basic(next_prime); Char_to_PC_Basic(' '); number /= next_prime;} 
prime_num = next_prime;
factor += 1;
if(!(factor%50))
Char_to_PC_Basic('.');
if (number==1)break;}
SW_reset;}



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


/*************************************************************************************/
