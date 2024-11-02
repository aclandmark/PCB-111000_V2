
#include "Prime_numbers_header.h"

int main (void){
long array_num=0;                                        //Zero for first array of 200 numbers, one for second array etc. 
long array_size = 200, array_pointer=1;
long array_of_nums[200], prime_num;  
int counter=0;
long number;
char num_string[12];

                                                                                                                                    
 setup_HW;

if(watch_dog_reset == 1)
String_to_PC_Basic("\r\n?");
else 
String_to_PC_Basic("\r\nEnter number  ");

number = Int_from_PC_Basic( num_string);


//counter = 10;
while(1){
array_num = prime_no_generator(array_num, array_size,  array_of_nums);
while(array_pointer < array_size)
{prime_num = next_prime_number(&array_pointer, array_of_nums, array_size); 

if(prime_num){
counter += 1; if (!(counter%50)){counter = 0;Char_to_PC_Basic('.');}
while (!(number%prime_num))
{String_to_PC_Basic("A ");Int_to_PC_Basic(prime_num); 
Char_to_PC_Basic(' '); 
number /= prime_num;
}
if (number ==1)SW_reset;
}}

array_num += 1;
array_pointer=0;
//Int_to_PC_Basic(array_num);String_to_PC_Basic("\r\n");
//counter = 10;
}}
   
   




/******************************************************************************************************************/
long next_prime_number(long *pointer, long *reg, int array_size){/////////////////////////////////
  
  while(!(*(reg + *pointer)))*pointer = *pointer +1;

  if (*pointer == array_size)return 0;
  return ((long)*(reg + (*pointer)++));
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
