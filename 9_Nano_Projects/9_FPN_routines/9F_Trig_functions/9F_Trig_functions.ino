
/*

*/

#include  "9F_header.h" 
#define Buff_Length  20

#define Get_and_echo_Sc_num \
SC_num = Scientific_number_from_KBD(Num_string, &sign, Buff_Length);\
FPN_to_String(SC_num, 2, 3, '=', Num_string);\
Serial.write(Num_string);

#define PIE 3.14159


int main (void) 
{

char Num_string[Buff_Length + 2];
                                 
float Result;
char sign, type, type_old;
float Cosine;
float Angle, SC_num;

setup_HW_with_reset_analysis;

while(switch_1_up);
while(switch_1_down);

float test = 0.8725;
Serial.print(root(test),5);


wdt_enable(WDTO_30MS);
while(switch_1_down)wdr();
Serial.write("Angle in degrees(Not Zero)? = ");
Get_and_echo_Sc_num;
Angle = SC_num/57.2958;

FPN_to_String(Angle, 1, 5, ' ',Num_string);
Serial.write (Num_string);
Serial.write(" Radians\t\t");
Serial.write("s, c or t?\t");
type = waitforkeypress_A();

switch(type){
  case 'c':Serial.write ("Cos = ");break;
  case 's':Serial.write ("Sin = ");break;
  case 't':Serial.write ("Tan = ");break;
}
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
Angle = Arc_sin_cos(Result, type, 1);
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
term = FPN_mult(term, FPN_div(Num, (float)m));
term = FPN_mult(term, FPN_div(Num, (float)(m+1)));
m+=2;
if ((++term_counter)%2)Result = FPN_sub(Result, term); 
else Result = FPN_add(Result, term); 

difference = FPN_sub(difference, Result);
if ((difference <= 1E-5) && (difference >= -1E-5))break;
difference = Result;}

return Result;}



/******************************************************************************************************************/
float Tan(float Angle){
return Sin_Cos (Angle, 's')/Sin_Cos(Angle, 'c');}




/*********************************************************************************************************************************/
float Arc_sin_cos(float Num, char type, char lib){
float tan_x;
float angle;
char sign = '+';
if (Num < 0){sign = '-'; }

if(type =='c'){
/*tan_x = pow(1.0-(Num*Num), 0.5)/Num;
angle = Arc_Tan(tan_x); 
return angle;}
*/
if (sign == '+')
return Arc_Tan(pow(1.0-(Num*Num), 0.5)/Num);
else return (180.0 + Arc_Tan(pow(1.0-(Num*Num), 0.5)/Num));
}

if(type =='s'){return Arc_Tan(Num/pow(1.0-(Num*Num), 0.5));}


}



/*******************************************************************************************************************/
float Arc_Tan(float Num){
float term;
float Result;

if ((Num > -1.0) && (Num < 1.0)){
term = Num;
Result = Num;
  for (int m = 3; m <50; m+=2){
term = term *Num*Num/(float)m;
  
 if ((m+1)%4)Result = Result + term;
 else Result = Result - term; 
 term = term*(float)m; }}
else{
 term = 1.0/Num; 
Result = term;
for (int m = 3; m <20; m+=2){
term = term/Num/Num/(float)m;
if ((m+1)%4)Result = Result + term;
 else   Result = Result - term; 
 term = term *(float)m;}

Result = PIE/2.0 - Result;
if(Num <= 1.0) Result -= PIE;
}

return Result * 57.2958;}





/****************************************Root to best integer value. Start iteration at 2***************************************/
float root(float Num){

float inc = 1.0;
float product_mem, product = 1.0;
float start_value = 2.0;
float result = 1;
float Tens_multiplier = 0.1;

int Tens_expt = 2;
int root = 2;


Num *= 100.0;

while(1){
  for(int m = 0; m< root; m++)product = product * start_value;
  
if (product <= Num){result = start_value; product_mem = product;}
else break;
start_value += inc;
product = 1.0;}

if (product_mem > Num)result = 1.0;                           //Initial value for numbers less than 4 


/*************************Improve iteration starting with one decimal place then 2, 3......up to 6 in all******************************/
for(int p = 1; p <= 6; p++)
{inc /= 10.0;
product = 1.0;
start_value = result + inc;
while(1){
  for(int m = 0; m< root; m++)product = product * start_value;
if (product <= Num){result = start_value; product_mem = product;}
else break;
start_value += inc;
product = 1.0;}}


return result * Tens_multiplier;}        




/**********************************************************************************************************************/
/*
cos x = 1-(x^2)/2! + (x^4)/4! - (x^6)/6! + (x^8)/8!...................

sin x =  x - (x^3)/3! + (x^5)/5! - (x^7)/7!................


 
 * ***************************************************/
