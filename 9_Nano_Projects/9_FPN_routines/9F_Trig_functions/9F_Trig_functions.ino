#include  "9F_header.h" 
#define Buff_Length  20

#define Get_and_echo_Sc_num \
SC_num = Scientific_number_from_KBD(Num_string, &sign, Buff_Length);\
FPN_to_String(SC_num, 1, 5, '\t', Num_string);\
Serial.write(Num_string);

#define PIE 3.14159


int main (void) 
{

char Num_string[Buff_Length + 2];
//float Num, Num_bkp;                               //Scientfic number pus its backup
//float Pow;                                        //Power to which the number is to be raised
//int twos_exp;                                     //Power to which 2 is raised 
//float SC_num;
//float logN;                                       //The log of Num
//float Log_result;                                 
float Result;
char sign;
float Cosine;
float Angle, SC_num;

setup_HW_with_reset_analysis;
wdt_enable(WDTO_30MS);
while(switch_1_down)wdr();
Serial.write("Angle in degrees?\t");
Get_and_echo_Sc_num;
Angle = SC_num/57.2958;
Serial.write("\r\nAngle in radians\t");
FPN_to_String(Angle, 1, 5, '\r',Num_string);
Serial.write (Num_string);

Cosine = Cos(Angle);
Serial.write ("Cosine:\t\t");
FPN_to_String(Cosine, 1, 5, '\r',Num_string);
Serial.write (Num_string);
//while(1)wdr();
I2C_FPN_to_display(Cosine);//}
display_FPN_short(Cosine, Num_string);
while(switch_1_down)wdr();
reset_status = 0;
//while(switch_3_down);
Serial.write("Arcos:\t\t");
Angle = Arc_cos(Cosine);
FPN_to_String(Angle, 1, 5, '\r',Num_string);
Serial.write (Num_string);
newline_A();

I2C_FPN_to_display(Angle);//}
display_FPN_short(Angle, Num_string);
while(switch_1_down)wdr();



while(1);

}


float Cos(float Num)
{float logE, logE_old;                                  //logs are calculated iteratively
float term = 1.0;                                             //Power series terms
float difference = 1.0;                                       //difference berween consequtive terms
float Result = 1.0;
int m = 1;                                              //Use power series to calculate the natural logarithm
int term_counter = 0;

char Num_string_2[15];

while(1){wdr();
term = FPN_mult(term, FPN_div(Num, (float)m));
term = FPN_mult(term, FPN_div(Num, (float)(m+1)));
m+=2;
if ((++term_counter)%2)Result = FPN_sub(Result, term); 
else Result = FPN_add(Result, term); 

difference = FPN_sub(difference, Result);
if ((difference <= 1E-7) && (difference >= -1E-7))break;
difference = Result;}

return Result;}


float Arc_cos(float Cos){

float Angle;
float term;
float Q = 1.0;      //term counter
float Cos_bkp;
float difference;


Cos_bkp = Cos;
Angle = PIE/2.0 - Cos;
Cos = Cos * Cos * Cos;

term = 0.5/3.0;                 //term counter = 1

Angle = Angle - (term * Cos);
difference = Angle;

while(1){wdr();
Q += 1.0;
term = term * ((2.0*Q) - 1.0)*((2.0*Q) - 1.0)/(2.0*Q)/((2.0*Q)+1.0);
Cos = Cos * Cos_bkp * Cos_bkp;
Angle = Angle - (term * Cos);

difference = FPN_sub(difference, Angle);
if ((difference <= 1E-7) && (difference >= -1E-7))break;
difference = Angle;}


return Angle * 57.2958 ;

}



/**********************************************

cos x = 1-(x^2)/2! + (x^4)/4! - (x^6)/6! + (x^8)/8!...................

sin x =  x - (x^3)/3! + (x^5)/5! - (x^7)/7!................


 
 * ***************************************************/
