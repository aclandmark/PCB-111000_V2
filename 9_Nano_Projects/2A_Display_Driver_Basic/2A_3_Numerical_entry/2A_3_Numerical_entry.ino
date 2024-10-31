
/*
 Similar to 6A and 6B but uses define statements to specify which segments are required to produce each 
 digit.
 Users simply press digits 0 to 9 at the keyboard and the number is displayed.
 Note however that it is displayed in the reverse order (a common problem with number processing).
 */
/*
EEPROM usage
0x1FF, 0x1FE and 0x1FD     OSCCAL
0x1FC and 0x1FB            PRN generator
0x1FA                      Reset source
*/


#include "Segment_driver_header.h"



#define zero "abcdef"                   //chars a,b,c,d,e and f are stored in an array named "zero"
#define one "bc"                        //chars b and c are stored in an array named "one"
#define two "abdeg"                     //Note: the compiler terminates each string in zero
#define three "abcdg"
#define four "bcfg"
#define five "acdfg"
#define six "gcdef"
#define seven "bca"
#define eight "abcdefg"
#define nine "gabcf"





int main (void){

char   digit;
int digit_num=0;                                                    //defines number of next digit on display           
const char* string_ptr = 0;                                         //pointer: will be loaded with the address of a segment string 
setup_HW;                                                //(i.e. the address of string "zero", "one", "two" etc....) 


if(watch_dog_reset != 1){

Char_to_PC_Basic('0'); Char_to_PC_Basic('\t');Int_to_PC_Basic((int)zero);newline_Basic();
Char_to_PC_Basic('1'); Char_to_PC_Basic('\t');Int_to_PC_Basic((long)one);newline_Basic();
Char_to_PC_Basic('2'); Char_to_PC_Basic('\t');Int_to_PC_Basic((long)two);newline_Basic();
Char_to_PC_Basic('3'); Char_to_PC_Basic('\t');Int_to_PC_Basic((long)three);newline_Basic();
Char_to_PC_Basic('4'); Char_to_PC_Basic('\t');Int_to_PC_Basic((long)four);newline_Basic();
Char_to_PC_Basic('5'); Char_to_PC_Basic('\t');Int_to_PC_Basic((int)five);newline_Basic();
Char_to_PC_Basic('6'); Char_to_PC_Basic('\t');Int_to_PC_Basic((long)six);newline_Basic();
Char_to_PC_Basic('7'); Char_to_PC_Basic('\t');Int_to_PC_Basic((long)seven);newline_Basic();
Char_to_PC_Basic('8'); Char_to_PC_Basic('\t');Int_to_PC_Basic((long)eight);newline_Basic();
Char_to_PC_Basic('9'); Char_to_PC_Basic('\t');Int_to_PC_Basic((long)nine);newline_Basic();
String_to_PC_Basic("Send digits?");}

else String_to_PC_Basic("\r\nAgain");

I2C_Tx_any_segment_clear_all();

//while(1){

digit_num = 0;                                                      //First digit on display

do{                                                                 //start of "do{}while();" loop
while(!(isCharavailable_Basic(1)))wdr(); digit = Char_from_PC_Basic();           //user enters digit (0 to 9) at the PC keyboard


switch(digit){                                                      //The appropriate address is loaded into location "string_pointer"
case '0': string_ptr = zero; break;                                 //The address of array zero is loaded into location "string_ptr"
case '1': string_ptr = one; break;
case '2': string_ptr = two; break;
case '3': string_ptr = three; break;
case '4': string_ptr = four; break;
case '5': string_ptr = five; break;
case '6': string_ptr = six; break;
case '7': string_ptr = seven; break;
case '8': string_ptr = eight; break;
case '9': string_ptr = nine; break;
default: continue; break;}                                          //Illegal key press: Go immediately to the start of the do loop

                                                                    //Send the address of the required string to subroutine "display_num_string();"
display_num_string(string_ptr, digit_num);digit_num++;} 
while (digit_num < 8);                                              //return to the top of the "do" loop until all digits have been illuminated

while(!(isCharavailable_Basic(1)))wdr(); Char_from_PC_Basic();
I2C_Tx_any_segment_clear_all();//}                                                    //clear display and repeat
SW_reset;}



/************************************************************************************************************************/
void display_num_string (const char* s, int digit_num){             //Subroutine requires a pointer to the string   
int char_ptr=0;                                                     //containing segments used to define a digit
char letter;

while(1){
letter = *(s + char_ptr);// (s[char_ptr]);                          //Note these two expressions are equivalent
switch(letter){                                                     //Work through the segments contained in the 
case 'a':                                                           //string until '\0' is encountered
case 'b': 
case 'c': 
case 'd': 
case 'e': 
case 'f': 
case 'g': I2C_Tx_any_segment(letter, digit_num);
break;                                                              //update display one segment at a time
case 0:  return; break;                                             //zero indicates the end of the string
default: break;}char_ptr++;}}                                       //incrementing "char_ptr" steps through the string
                                                                    //Selecting segment letters in turn



/************************************************************************************************************************/                                                                 
