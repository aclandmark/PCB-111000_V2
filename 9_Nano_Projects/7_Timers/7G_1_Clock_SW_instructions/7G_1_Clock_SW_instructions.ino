









#include "Proj_7G_1_header_file.h"
char Num_as_String[12];


int main (void){

  
  setup_HW_Arduino_IO;
  Prog_on_chip_EEPROM();      
}




void Prog_on_chip_EEPROM(void){
char next_char, text,char_counter;
int EEP_read_address=0,EEP_write_address = 0;
char text_counter = 0;


newline_A();Serial.write ("BR 2k4 then AK");newline_A();              //Text_Baud_Rate_L;
_delay_ms(25);
Serial.end();
Serial.begin(4800);
while (!Serial);
waitforkeypress_A();
Serial.write ("Text file?");newline_A();                            //Text_message_file;

if ((text = waitforkeypress_A()) == '"');
else
{while(1){if(isCharavailable_A(6))
//text = receiveChar(); 
//while(!(Serial.available()))wdr();
text = Serial.read(); 
if (text == '"') break;}} 


/***************Strings in the .txt file are saved to on-chip EEPROM**************************/

while(1){next_char = waitforkeypress_A();                           //wait for first character from file
if ((next_char != '\r') && (next_char != '\n'))                   //Ignore leading carriage returns
break;}             
Text_to_EEPROM(&EEP_write_address, next_char);                    //save first letter to EEPROM

while(EEP_write_address < 0x1FA)                                  //Exits before cal bytes can be overwritten
  {if(isCharavailable_A(6))                                         //returns 1 if a new letter is available (0 at the end of the file) 
  {//text = receiveChar();                                          //Temporary storage
//while(!(Serial.available()))wdr();
text = Serial.read(); 
if (!((++text_counter)%10)) Toggle_LED_1;

  switch (text){                                                  //Test the new character  
    case '\r':                                                   //If it is '\r' and or '\n' 
    case '\n':                                                    //ignore it or replace it with with a single '\0'
    if(next_char == '\0')break; 
    else  {next_char = '\0'; 
        Text_to_EEPROM(&EEP_write_address, next_char);}break;
    case '"': break;                                              //Ignore the second " symbol
    default :   next_char = text;                                 //save the letter
          Text_to_EEPROM(&EEP_write_address, next_char);          //increments the write address
          break;}

  }else break; }                                                  //End of file reached

if(EEP_write_address == 0x1FA)                                    //If text file was too long
{Text_to_EEPROM(&EEP_write_address, '\0');                        //Place '\0' in 0x1F6 to terminate the string
binUnwantedChars_dot();}                                          //Send dots to pc to indicate lost characters


/****************Echo text file to screen with the address of each string**********************/


Hex_to_PC_A(EEP_read_address, Num_as_String, '\t');                 //Send address of first line of text


do{char_counter = 0;                                              //Read back text one line at a time
while(char_counter < 150)
{text = Text_from_EEPROM(&EEP_read_address);                      //Increments the read address
char_counter += 1;
if(text)Serial.write(text); else break;}                              //When '\0' is detected start next line
newline_A();
                                 
Hex_to_PC_A(EEP_read_address, Num_as_String, '\t');}                 //Send address of next line

while(EEP_read_address < EEP_write_address);                      //Exit when read address equals write address

Serial.write("\r\nRestore Baud rate of 57600");LED_2_on;

_delay_ms(25);
Serial.end();
Serial.begin(115200);LED_1_on;
while (!Serial);
waitforkeypress_A();LEDs_off
Serial.write("\r\nFinnished");
while(1);}








/****************************************************/
void Text_to_EEPROM(int*w_a, char byte){                //on-chip EEPROM
eeprom_write_byte((uint8_t*)(*w_a),byte);                 
*w_a = *w_a + 1;}



/****************************************************/
char Text_from_EEPROM(int*r_a){
return eeprom_read_byte((uint8_t*)((*r_a)++));}           



/*********************************************************************/
void binUnwantedChars_dot (void){char bin_char;
while(1){if (isCharavailable_A(5)==1)
{
  bin_char = Serial.read();
Serial.write('.');}else break;}newline_A();}










/********************************************************************************************************************/
