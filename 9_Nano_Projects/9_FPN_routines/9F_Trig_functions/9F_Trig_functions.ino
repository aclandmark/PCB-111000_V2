
/*See
https://proofwiki.org/wiki/Power_Series_Expansion_for_Real_Arccosine_Function
https://proofwiki.org/wiki/Power_Series_Expansion_for_Real_Arcsine_Function
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
char sign, type;
float Cosine;
float Angle, SC_num;

setup_HW_with_reset_analysis;
wdt_enable(WDTO_30MS);
while(switch_1_down)wdr();
Serial.write("Angle in degrees? = ");
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
}


Result = Sin_Cos(Angle, type);
//Serial.write ("Cos = ");
FPN_to_String(Result, 1, 5, '\t',Num_string);
Serial.write (Num_string);

display_FPN_short(Result, Num_string);
while(switch_1_up)wdr();
reset_status = 0;

//Serial.write("Arcos = ");
switch(type){
  case 'c':Serial.write ("Arccos = ");break;
  case 's':Serial.write ("Arcsin = ");break;
}


Angle = Arc_cos(Result, type, 1);
display_FPN_short(Angle, Num_string);
FPN_to_String(Angle, 2, 3, ' ',Num_string);
Serial.write (Num_string);
Serial.write(" Degrees (");
Angle = Arc_cos(Result, type, 2);
FPN_to_String(Angle, 2, 3, ')',Num_string);
Serial.write (Num_string);

newline_A();newline_A();


while(switch_1_down)wdr();



while(1);

}

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
float Arc_cos(float Cos, char type, char lib){

float Angle;
float term;
float Q = 1.0;      //term counter
float Cos_bkp;
float difference;


Cos_bkp = Cos;
if (lib == 2){Angle = PIE/2.0 - Cos;
Cos = Cos * Cos * Cos;}

if (lib == 1){Angle = FPN_sub(FPN_div(PIE,2), Cos);
Cos = FPN_mult(FPN_mult(Cos, Cos), Cos);}

term = FPN_div(0.5,3.0);                 //term counter = 1

Angle = FPN_sub (Angle, FPN_mult(term, Cos));
difference = Angle;

while(1){wdr();
Q += 1.0;

if (lib == 1){
term = term * ((2.0*Q) - 1.0)*((2.0*Q) - 1.0)/(2.0*Q)/((2.0*Q)+1.0);
Cos = Cos * Cos_bkp * Cos_bkp;}

if (lib == 2){
term = FPN_mult(term,FPN_mult(FPN_sub (FPN_mult(2.0,Q), 1.0), FPN_sub (FPN_mult(2.0,Q), 1.0)));
term = FPN_div (term, (FPN_mult(FPN_mult(2.0,Q),FPN_add (FPN_mult(2.0,Q), 1.0))));
Cos = FPN_mult(FPN_mult(Cos, Cos_bkp), Cos_bkp);}

//if(lib == 2)Cos = FPN_mult(FPN_mult(Cos, Cos_bkp), Cos_bkp);
//if(lib == 1)Cos = Cos * Cos_bkp * Cos_bkp;

 
Angle = FPN_sub (Angle, FPN_mult(term, Cos));
difference = FPN_sub(difference, Angle);
if (FPN_LT(difference, 1e-6) && FPN_GT(difference, -1e-6))break;
difference = Angle;}

Angle = FPN_mult(Angle, 57.2958);

if (type == 'c');
if (type == 's'){Angle = Angle - 90.0; Angle = Angle * -1.0;}

return Angle;



if(lib == 3){
  
Cos_bkp = Cos;
Angle = PIE/2.0 - Cos;
Cos = Cos * Cos * Cos;

term = FPN_div(0.5,3.0);                 //term counter = 1

Angle = FPN_sub (Angle, FPN_mult(term, Cos));
difference = Angle;

while(1){wdr();
Q += 1.0;
term = term * ((2.0*Q) - 1.0)*((2.0*Q) - 1.0)/(2.0*Q)/((2.0*Q)+1.0);
//term = FPN_mult(term,FPN_mult(FPN_sub (FPN_mult(2.0,Q), 1.0), FPN_sub (FPN_mult(2.0,Q), 1.0)));
//term = FPN_div (term, (FPN_mult(FPN_mult(2.0,Q),FPN_add (FPN_mult(2.0,Q), 1.0))));

Cos = Cos * Cos_bkp * Cos_bkp;
//Cos = FPN_mult(FPN_mult(Cos, Cos_bkp), Cos_bkp);
 
Angle = FPN_sub (Angle, FPN_mult(term, Cos));
difference = FPN_sub(difference, Angle);
if (FPN_LT(difference, 1e-6) && FPN_GT(difference, -1e-6))break;
//if ((difference <= 1E-6) && (difference >= -1E-6))break;
difference = Angle;}

Angle = FPN_mult(Angle, 57.2958);

if (type == 'c');
if (type == 's'){Angle = Angle - 90.0; Angle = Angle * -1.0;}

return Angle;}


}



/**********************************************************************************************************************/
/*
cos x = 1-(x^2)/2! + (x^4)/4! - (x^6)/6! + (x^8)/8!...................

sin x =  x - (x^3)/3! + (x^5)/5! - (x^7)/7!................


 
 * ***************************************************/
