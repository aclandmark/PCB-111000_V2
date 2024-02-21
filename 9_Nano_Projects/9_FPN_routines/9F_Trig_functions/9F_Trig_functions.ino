
/*
Standard series for arcsin/cos were very slow to converge especially with the DIY FPN functions
Sines and cosines were therefore converted to tangents. Arctan was relatively fast to converge
Similarly it was found simpler to calculate tangets using the formulat Tan = Sin/Cos 
*/

#include  "9F_header.h" 
#define Buff_Length  20

#define Get_and_echo_Sc_num \
SC_num = Scientific_number_from_KBD(Num_string, &sign, Buff_Length);\
FPN_to_String(SC_num, 2, 3, '=', Num_string);\
Serial.write(Num_string);

#define PIE 3.14159


int main (void) 

{char Num_string[Buff_Length + 2];
float Result;
char sign, type;
float Angle, SC_num;

setup_HW_with_reset_analysis;

wdt_enable(WDTO_30MS);
while(switch_1_down)wdr();
Serial.write("Angle in degrees(Not Zero)? = ");
Get_and_echo_Sc_num;
Angle = FPN_div(SC_num, 57.2958);

FPN_to_String(Angle, 1, 5, ' ',Num_string);
Serial.write (Num_string);
Serial.write(" Radians\t\t");
Serial.write("s, c or t?\t");
type = waitforkeypress_A();

switch(type){
  case 'c':Serial.write ("Cos = ");break;
  case 's':Serial.write ("Sin = ");break;
  case 't':Serial.write ("Tan = ");break;}
  
if (type == 't')Result = Tan(Angle);
else Result = Sin_Cos(Angle, type);
FPN_to_String(Result, 1, 5, '\t',Num_string);
Serial.write (Num_string);

display_FPN_short(Result, Num_string);
while(switch_1_up)wdr();
reset_status = 0;

switch(type){
  case 'c':Serial.write ("Arccos = ");break;
  case 's':Serial.write ("Arcsin = ");break;
  case 't':Serial.write ("Arctan = ");break;}

if (type == 't')Angle = Arc_Tan(Result);
else 
Angle = Arc_sin_cos(Result, type);
display_FPN_short(Angle, Num_string);
FPN_to_String(Angle, 2, 3, ' ',Num_string);
Serial.write (Num_string);
Serial.write(" Degrees ");

newline_A();newline_A();
while(switch_1_down)wdr();
while(1);}



/**************************************************************************************************************************/
float Sin_Cos(float Num, char type)
{float term;                                                 //Power series terms
float difference;                                            //difference berween consequtive terms
float Result;       
int m;                                                      //Use power series to calculate the natural logarithm
int term_counter = 0;

char Num_string_2[15];

if(type == 'c'){
term = 1.0;
Result = 1.0;
difference = 1.0;
m = 1;}

if(type == 's'){
term = Num;
Result = term;  
difference = Num;                                       //difference berween consequtive terms
m = 2;}  

while(1){wdr();
term = FPN_mult(term, FPN_div(Num, Int2Float(m)));
term = FPN_mult(term, FPN_div(Num, Int2Float(m+1)));
m+=2;
if ((++term_counter)%2)Result = FPN_sub(Result, term); 
else Result = FPN_add(Result, term); 

difference = FPN_sub(difference, Result);
if(FPN_LT(difference, 1E-5) && (FPN_GT(difference, -1E-5)))break;
difference = Result;}

return Result;}



/******************************************************************************************************************/
float Tan(float Angle){
return FPN_div (Sin_Cos (Angle, 's'), Sin_Cos(Angle, 'c'));}



/*********************************************************************************************************************************/
float Arc_sin_cos(float Num, char type){
float tan_x;
float angle;
char sign = '+';
if (Num < 0){sign = '-'; }

if(type =='c'){
if (sign == '+')
return Arc_Tan(root(1.0-(Num*Num))/Num);
else return (180.0 + Arc_Tan(root(1.0-(Num*Num))/Num));}
if(type =='s'){return Arc_Tan(Num/root(1.0-(Num*Num)));}}



/*******************************************************************************************************************/
float Arc_Tan(float Num){
float term;
float Result;
float difference = 1.0;


if (Num > 1e4)return 90.0;
if (Num < -1e4)return -90.0;

if ((Num > -1.0) && (Num < 1.0)){
term = Num;
Result = Num;
  for (int m = 3; m <500; m+=2){
    
term = FPN_mult(term, Num);
term = FPN_div(term, (float)m);
term = FPN_mult(term, Num);
  
 if ((m+1)%4)Result = FPN_add(Result, term);
 else Result = FPN_sub (Result, term); 
 term = FPN_mult (term, Int2Float(m));
 
 difference = FPN_sub(difference, Result);
if(FPN_LT(difference, 1E-4) && (FPN_GT(difference, -1E-4)))break;

difference = Result;}}
else
{difference = 1.0;
 term = FPN_div(1.0,Num); 
Result = term;
for (int m = 3; m <500; m+=2){ 
term = FPN_div(term, Num);
term = FPN_div(term,Int2Float(m));

term = FPN_div(term, Num);

if ((m+1)%4)Result = Result + term;
 else   Result = FPN_sub (Result, term); 
 term = FPN_mult(term, Int2Float(m));
 
  difference = FPN_sub(difference, Result);
if(FPN_LT(difference, 1E-4) && (FPN_GT(difference, -1E-4)))break; 
difference = Result; }

Result = FPN_sub(FPN_div(PIE,2.0), Result);
if(Num <= 1.0) Result = FPN_sub (Result, PIE);}

return FPN_mult (Result, 57.2958);}




/*******************************************************************************************************************/
float root(float Num){

float inc = 1.0;
float product_mem, product = 1.0;
float start_value = 2.0;
float result = 1;
float Tens_multiplier = 1.0;

int Tens_expt = 0;
int root = 2;

while(Num < 1.0){Num = FPN_mult (Num, 100.0); Tens_expt += root;} 
for(int m = 0; m < Tens_expt; m += root)Tens_multiplier = FPN_div (Tens_multiplier, 10.0);
while(1){
  for(int m = 0; m< root; m++)product = FPN_mult(product, start_value);
 if(FPN_GT_or_EQ(Num, product)){result = start_value; product_mem = product;}
else break;
start_value = FPN_add(start_value, inc);
product = 1.0;}

if (product_mem > Num)result = 1.0;                           //Initial value for numbers less than 4 
/*************************Improve iteration starting with one decimal place then 2, 3......up to 6 in all******************************/


for(int p = 1; p <= 6; p++)
{
  inc = FPN_div(inc, 10);
product = 1.0;
start_value = result + inc;
while(1){
  for(int m = 0; m< root; m++)product = FPN_mult (product, start_value);
if (product <= Num){result = start_value; product_mem = product;}
else break;
start_value = FPN_add(start_value, inc);
product = 1.0;}}

return FPN_mult(result, Tens_multiplier);}       



float Int2Float(long num){
int m;
long twos_expt;

for(m = 0; m <= 31; m++)                                        //Scan long number from bit 31 stoping at the most significant '1' (the MSB)
{if(num & ((unsigned long)0x80000000 >> m))break;}
m = 31 - m;
if (m < 23)num = num << (23 - m);                               //Shift the long number so that the MSB is at location 23
if (m > 23)num = num >> (m - 23); 
twos_expt = m - 1 + 127;                                       //The twos exponent is the number of significant bits plus 127
twos_expt = twos_expt << 23;                                    //shift the twos exponent so that it occupies bits 23 to 30.
num = num + twos_expt;                                          //Add the exponent into long number. Note bit 23 which is always '1' is overwritten
return (*(float*)(&num));}                                      //Tell the compiler that the long number is now to be treated as a floating point number


/**********************************************************************************************************************/
/*
cos x = 1-(x^2)/2! + (x^4)/4! - (x^6)/6! + (x^8)/8!...................

sin x =  x - (x^3)/3! + (x^5)/5! - (x^7)/7!................


 
 * ***************************************************/
