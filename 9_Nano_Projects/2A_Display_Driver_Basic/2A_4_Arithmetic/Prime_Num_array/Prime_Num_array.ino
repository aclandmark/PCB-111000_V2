
#include "Prime_numbers_header.h"

char counter=0;

int main (void){
int I=0;                                        //Zero for first array of 200 numbers, one for second array etc.
int j = 0; 
int array_size = 200;
unsigned int array[200], mask;  
                                                                                                                                    
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
}j++;} 


while(1){if (waitforkeypress_Basic() != 'x');
else break;}
I += 1;
counter = 10;
String_to_PC_Basic("\r\n\r\n");
   }}


/***************************************************************************************************************************/
void prime_no_generator                        
(int I,int n,  unsigned int reg[]){            //n the array size is 200, I is the array number (zero for the first one)
                                                
int i=2;                                       //We compute the product i * m
int m;                                         //i always starts at 2 then increments to 3, 4, etc. 
while (i*i < n*(I+1)){                         //Maximum value i can take
wdr();

if (I)m = (n*I/i)-i +1;
else m = 0;

while(i*(i+m) <= n*(1+I))                       //2*101, 2*102, 2*103,... etc   3*67,3*68, 3*69,...etc    5*41, 5*42,...etc
{reg[i*(i+m) -1 - n*I] = 0;                   //set appropriate registers to zero (Note the offset of 1 place).
while((!(reg[i*(i+m) -1 - n*I]))
&& ((i*(i+m) -1 - n*I) < n))
{m+=1;}}

i+=1;
}}



/****************************************************************************************************************************/
