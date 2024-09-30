
#include "Prime_numbers_header.h"

char counter=0; //del

int main (void){
int I=0;                                        //Zero for first array of 200 numbers, one for second array etc.
int j = 0; 
int array_size = 100;
unsigned int array[100], mask;  
                                                                                                                                    
 setup_HW;
 
String_to_PC_Basic("OLD method");

counter = 10;

while(1){

for(int j =0; j < array_size; j++)              //Populate an array with a sequence of numbers 
{array[j] = j+1+array_size*I;}                  //for example 201, 202, 203, .....400
prime_no_generator(I,array_size,array);         //Sets all non-primes in the array to zero

j = 0;
while(j<array_size){if (array[j] != 0)         //Scan the array for prime numbers
{
if (counter)                                   //Print them out 10 at a time
{if (counter == 10)
String_to_PC_Basic("\r\n");
Int_to_PC_Basic(array[j]); 
Char_to_PC_Basic(' ');
counter -= 1;

if(!(counter)){counter = 10;}    } 



/***************************Control display*********************************************************************************/
mask = 0;
{int m = 15; while (!(array [j] & (1 << m)))
{mask |= (1 << m); m -= 1;}}
array[j] = (~array[j]) & ~mask;
//pause_pci_on_sw2;
//////_delay_ms(100);
//Timer_T0_10mS_delay_x_m(10);

//I2C_Tx_2_integers(array[j],reverse (array[j]));
//resume_PCI_on_sw2;
}j++;} 


while(1){if (waitforkeypress_Basic() != 'x');
else break;}
I += 1;
counter = 10;
String_to_PC_Basic("\r\n\r\n");
}


/*
char counter=0;
unsigned int Number=0;  
                                                                                                                                    
 setup_HW;

String_to_PC_Basic("New method");
 
 while (1){
for(int m = 1; m <= 100; m++){

if(prime_num_check(Number + m))
{ Int_to_PC_Basic(Number + m);
counter += 1;
if (counter==10){counter = 0;newline_Basic();}}} 

while(1){if (waitforkeypress_Basic() != 'x');
else break;}
Number += 100;
String_to_PC_Basic("\r\n\r\n");counter = 0;}
*/





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



/***************************************************************************************************************************/
void prime_no_generator                        
(int I,int n,  unsigned int reg[]){            //n the array size is 200, I is the array number (zero for the first one)
                                                
int i;                                         //We compute the product i * m
int m;                                         //i always starts at 2 then increments to 3, 4, etc. 
i=2;                                           //m is set to take us to the beginning of the array  
while (i*i < n*(I+1)){                         //Maximum value i can take
wdr();
m=0;
while(i*(i+m) <= n*I)m++;                       //Go to start of the new array for example
while(i*(i+m) <= n*(1+I))                       //2*101, 2*102, 2*103,... etc   3*67,3*68, 3*69,...etc    5*41, 5*42,...etc
{reg[i*(i+m) -1 - n*I] = 0; m++;}               //set appropriate registers to zero (Note the offset of 1 place).
i++;}}
/****************************************************************************************************************************/
