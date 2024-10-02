
#include "Prime_factors_header.h"

#define message_1 "\r\nEnter integer number:  PCB returns all prime factors of that number.(Press sw1 to cancel.)\r\n"
#define message_2 "New number?\r\n"
#define message_3 "\r\nCalculation cancelled. New number?\r\n"


#define Search_array_length   125


/************************************************************************************************************/
int main (void)  
  { 
    char num_string[12];
    long  number;
    long factor;
    char factor_counter;
 
 setup_HW;


  String_to_PC_Basic(message_1);
   number = Int_from_PC_Basic(num_string);
   newline_Basic();
  Int_to_PC_Basic(number);  _delay_ms(5);

 Char_to_PC_Basic('\t');
  factor_counter = 0;
  do{_delay_ms(5);                                                  //Required by Serial.print
  factor = Product_search(number);                                 //Get lowest factor
  number = number/factor;                                          //Next number to factorise
  Int_to_PC_Basic(factor); //
  factor_counter += 1;} 
  while (number != 1);  

  if(factor_counter == 1){String_to_PC_Basic("Prime\r");}
  SW_reset;
  return 1;}



/************************************************************************************************************/
long Product_search  (long number)  {
long search_array[Search_array_length]; 
int j=0;
long n=Search_array_length;
long L=0;

while(1){
for(long k =0; k < n; k++)                              //Populate array (i.e. with 401, 402, 403, ..etc) 
{search_array[k] = k+1+n*L;}
prime_no_generator(L,n,search_array);                   //Set all non primes in array to zero
j=0; 
while(search_array[j] == 0)j += 1;                     //Select first prime number
while (j<=Search_array_length)
{if ((search_array[j] >= 2) &&
(number%search_array[j] == 0))                         //Scan array until a factor is found
{return search_array[j];}                              //and return it
j += 1;} 
if(L*Search_array_length > number)return 0;                            //No factors found at all: Return zero for prime number
L += 1; Char_to_PC_Basic('.');}}                           //No factor found in array: increment L and populate new array



/************************************************************************************************************/
void prime_no_generator
(long L, long n,  long reg[]){                  //n the array size is 200, I is the array number (zero for the first one)
  
long i;                                         //We compute the product i * m
long m;                                        //i always starts at 2 then increments to 3, 4, etc.
i=2;                                           //m is set to take us to the beginning of the array 
while (i*i < n*(L+1)){                         //Maximum value i can take
wdr();
m=0;
while(i*(i+m) <= n*L)m++;                      //Go to start of the new array for example
while(i*(i+m) <= n*(1+L))                      //2*101, 2*102, 2*103,... etc   3*67,3*68, 3*69,...etc    5*41, 5*42,...etc
{reg[i*(i+m) -1 - n*L] = 0; m++;wdr(); }      //set appropriate registers to zero (Note the offset of 1 place) 
i++;}}


/************************************************************************************************************/




/************************************************************************************************************/




/************************************************************************************************************/
