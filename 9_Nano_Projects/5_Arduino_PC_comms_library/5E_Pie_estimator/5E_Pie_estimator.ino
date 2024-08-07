
#include "Pie_estimator_header.h"


/****************************PIE calculator*********************************************************************
A circle is drawn over a fine grid of squares of side 1 unit in length
The number of squares within the circle is counted to give its area 
from which pie is calculated.

Best estimate for pie is 3.141592 for a radius of 25,000
The Code blocks C complier running on a PC using 64 bit and floating point doubles gives 3.141592653589 for
a radius of 370,000,000 in about 10 seconds
********************************************************************************/

/*
EEPROM usage
0x1FF, 0x1FE and 0x1FD     OSCCAL
0x1FC and 0x1FB            PRN generator
0x1FA                      Reset source
*/



#define message_1 "\r\nEstimate value for PIE. Enter radius (50000 max)?\r\n\
Push and hold SW1 to repeat. SW2 to pause display\r\n"

#define message_2 "\t?\r\n"



int main (void) 

{long Area;                    //The area of a 90 degree segment of circle which has its centre at location 0,0.
unsigned long R;               //The radius of the circle
unsigned long X;               //The x coordinate of a column of squares
unsigned long Y;               //The y coordinate at which the column intersects the circle
char Num_string[15];
float pie;


setup_HW_Arduino;

switch(reset_status){
  case POR_reset:                 User_prompt_A;    Serial.write(message_1);break;
  case WDT_reset:                 Serial.write(message_2);break;
  case External_reset:            Serial.write(message_1);break;}


R = Unsigned_Int_from_PC_A(Num_string, 0);                              //DIY subroutine uses Arduino functions
Unsigned_Int_to_PC_A(R, Num_string, '\t');                             //DIY subroutine uses Arduino functions
Y = 1;
Area = 0;

for(unsigned int m = 1; m <= R; m++){wdr();                           //Count number of squares contained in the circle
X = R - m;  
while (Y*Y <= ((R*R) - (X*X))) Y += 1; Y -= 1;
Area += Y;}



/************************Calculate 4*Area/R^2****************************/

if ((pie = (float)Area / (float)R / (float)R * 4) < 0.0)              //Convert integer numbers to FPN and calculate pie
Serial.write ("Overflows\r\n");
else {Serial.print (pie,6);                                           //Arduino converts FPN to string and sends this to the PC

I2C_FPN_to_display(pie);}

SW_reset;
return 1; }








/***************************************************************************************************************************/
