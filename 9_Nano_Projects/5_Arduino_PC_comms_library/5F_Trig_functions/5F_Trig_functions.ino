
/*
 Sines and cosines are calculated using series that have the same terms as those used by e^x. 

To recover the angles see
      https://proofwiki.org/wiki/Power_Series_Expansion_for_Real_Arccosine_Function
      https://proofwiki.org/wiki/Power_Series_Expansion_for_Real_Arcsine_Function
and   https://proofwiki.org/wiki/Power_Series_Expansion_for_Real_Arctangent_Function
 
*/

 
#include "Trig_function_header.h"

#define message_1 "\r\nTrig function: Enter angle in degrees\r\n"
#define message_2 "\r\nAnother angle?\r\n"

#define BL 30                                     //Buffer length

float Pie = 3.1415926;


int main (void) 

{char Num_string[BL + 2];
float Num;                               //Scientfic number pus its backup
float Result;
char  Fn;

setup_HW_Arduino;

switch(reset_status){
  case POR_reset:                 User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:                 Serial.write(message_2);break;
  case External_reset:            Serial.write(message_1);break;}

Num = Sc_Num_from_PC_A(Num_string, BL);           //User enters the scientific number
if(!(Num))Num = 0.0001;
else{
Num = Num *2.0 * Pie /360.0;}
Sc_Num_to_PC_A(Num, 1,5,'\r');

Serial.write("Keypress c, s or t?\r\n");
Fn = waitforkeypress_A();

switch (Fn){
case 'c': Result = Sin_cos_power_series(Num, 'c'); Serial.write("Cos x =");break;
case 's': Result = Sin_cos_power_series(Num, 's');Serial.write("Sin x =");break;
case 't': Result = Sin_cos_power_series(Num, 's')/Sin_cos_power_series(Num, 'c');Serial.write("Tan x =");break;}

Sc_Num_to_PC_A(Result,1,8,'\r');
I2C_FPN_to_display(Result);

switch (Fn){
case 'c': Result = Arc_sin_cos(Result, 'c'); Serial.write("ArcCos x =");break;
case 's': Result = Arc_sin_cos(Result, 's');Serial.write("ArcSin x =");break;
case 't': Result = Arc_Tan(Result);Serial.write("ArcTan x =");break;}

Sc_Num_to_PC_A(Result,1,8,'\r');
while(switch_1_down);
display_FPN_short(Result, Num_string);

while(switch_1_up);

SW_reset;
return 1;}



/************************************************************************************************************************/
float Sin_cos_power_series(float Num, char type)
{float term;                                                 //Power series terms
float difference;                                            //difference berween consequtive terms
float Result;       
int m;                                                      //Use power series to calculate the natural logarithm
int term_counter = 0;


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
term = term * Num/(float)m;
term = term * Num/(float)(m+1);

m+=2;
if ((++term_counter)%2)Result = Result - term; 
else Result = Result + term; 

difference = difference - Result;
if ((difference <= 1E-5) && (difference >= -1E-5))break;
difference = Result;}

return Result;}



/******************************************************************************************************************************/
float Arc_sin_cos(float Num, char type){

float Angle;
float term;
float Q = 1.0;      //term counter
float Num_bkp;
float difference;
int counter = 0;

Num_bkp = Num;
Angle = Pie/2.0 - Num;
Num = Num * Num * Num;

term = 0.5/3.0;
Angle = Angle - (term * Num);
difference = Angle;

while(1){wdr(); 
Q += 1.0;
term = term * ((2.0*Q) - 1.0)*((2.0*Q) - 1.0)/(2.0*Q)/((2.0*Q)+1.0);
Num = Num * Num_bkp * Num_bkp;

 Angle = Angle - (term * Num);
 difference = difference - Angle;
 if((difference < 1.0e-6) && (difference > -1.0e-6))break;
difference = Angle;}

Angle = Angle * 57.2958;
if (type == 'c');
if (type == 's'){Angle = Angle - 90.0; Angle = Angle * -1.0;}

return Angle;}



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

Result = Pie/2.0 - Result;
if(Num <= 1.0) Result -= Pie;}

return Result * 57.2958;}





/************************************************************************************************************************/
