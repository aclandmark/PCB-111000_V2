

void Int_to_PC_Basic(long);
void Char_to_PC_Basic(char);
char waitforkeypress_Basic(void);
char Int_to_String_Basic (long, char *);





void root_computation(long Num, char * result_as_string){
long result, result_old;
int root = 2;
long start_value, product;
char str_ptr;

  
product = 1.0;
start_value = 2.0;
result = 1;

if((product = start_value * start_value) > Num)result = 1; 
else{
while(1){
product = start_value * start_value; 
if (product <= Num){result = start_value;} 
else break;
start_value += 1;
product = 1.0;}}
str_ptr = Int_to_String_Basic(result, result_as_string); //Char_to_PC_Basic('.');



/*************************Improve iteration starting with one decimal place then 2, 3......up to 6 in all******************************/
for(int p = 1; p <= 6; p++)
{Num *= 100;
  result *= 10;
  result_old = result;
product = 1.0;
start_value = result + 1;
while(1){
 product = start_value * start_value;
if (product <= Num){result = start_value;}
else break;
start_value += 1;
product = 1.0;}

//Char_to_PC_Basic(start_value-result_old -1 + '0');

*(result_as_string + str_ptr) = (start_value-result_old -1 + '0');str_ptr += 1;
}
*(result_as_string + str_ptr) = '\0';
}




char Int_to_String_Basic (long number, char * s)
{ int i = 11;
  //char s[12];
for(int m = 0; m <= 11; m++)s[m] = 0;
  
   do
  { *(s+i) = number % 10 + '0'; i-=1;
  }
  while ((number = number / 10) > 0);
  *(s-i) = '.';

  return ++i;}
 //load string rom reverse end
