
#include "Prime_numbers_header.h"

char counter=0;

int main (void){
long array_num=0;                                        //Zero for first array of 200 numbers, one for second array etc.
int array_size = 200, array_pointer=1;
long num_array[200], prime_num;  
char counter=9;
char num_string[12];

                                                                                                                                    
 setup_HW;
 
String_to_PC_Basic("Prime number array\r\n1  ");

while(1){
array_num = prime_no_generator(array_num,array_size,  num_array);
while(array_pointer < array_size)
{prime_num = next_prime_number(&array_pointer, num_array, array_size); 
if (counter)                                   //Print them out 10 at a time
{if (counter == 10)
String_to_PC_Basic("\r\n");
if(prime_num)Int_to_PC_Basic(prime_num); 
Char_to_PC_Basic(' ');
counter -= 1;

if(!(counter)){counter = 10;}}
}

String_to_PC_Basic("\r\n\r\nZero or next array or AOK  ");
if(waitforkeypress_Basic()=='0')array_num += 1;
else {String_to_PC_Basic("\t Array number?  ");
  array_num=Int_from_PC_Basic( num_string);}

array_pointer=0;
counter = 10;
newline_Basic();}}
   
   




/******************************************************************************************************************/
long next_prime_number(int *pointer, long *reg, int array_size){
  
  while(!(*(reg + *pointer)))*pointer = *pointer +1;

  if (*pointer == array_size)return 0;
  return (*(reg + (*pointer)++));
  }

/***************************************************************************************************************************/
long prime_no_generator                        
(long I,long n,  long reg[]){            //n the array size is 200, I is the array number (zero for the first one)

long i=2;                                       //We compute the product i * m
long m;                                         //i always starts at 2 then increments to 3, 4, etc. 

for(int j =0; j < n; j++)              //Populate an array with a sequence of numbers 
{reg[j] = j+1+(n*I);}                  //for example 201, 202, 203, .....400
                                                

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
}return I;}



/****************************************************************************************************************************/
