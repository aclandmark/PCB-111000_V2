
/*
Program synthesizes a pulse waveform on the PC screen and also does some floating point arithmetic.
Floating point results of zero or infinity cause the program to crash and trigger a WDTout with interrupt.
Switch presses are used to start the waveform adjust its parameters and trigger the FP arithmetic.
A SW_reset is triggered after 25 uninterrupted waveform cycles.

Enter duty cycle x 10 (i.e. 1 to 9)
Enter the number of time slots or harmonics as ~5 to 256
Use switch presses in place of the return key press


See https://en.wikipedia.org/wiki/Pulse_wave for details of the pulse train
*/


#include "8B_header_file_1.h"

#define Buff_Length  20
#define T1_delay_250ms 5,0xF85F

float Num_1, Num_2;
char digits_12[12];   ///////////////////////                                //Array used to drive the display
char Num_as_string[12];


int main (void){

setup_HW_Arduino_IO;



if (watch_dog_reset){Serial.write ("?\t");}else {User_prompt_A;Serial.write("Enter FPN then AK.");}

Num_1 = Sc_Num_from_PC_A( Num_as_string, Buff_Length );
if (watch_dog_reset)(watch_dog_reset) = 0; else Serial.write(" You entered");  Serial.print(Num_1,4);
display_FPN_short(Num_1, digits_12);
waitforkeypress_A();
Num_1 *= 11.0;
Serial.write("Result post arithmetic (AK to repeat)\r\n");
display_FPN_short(Num_1, digits_12);
waitforkeypress_A();
SW_reset;
}
