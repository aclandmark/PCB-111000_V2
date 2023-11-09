void Prog_on_chip_EEPROM(void){
char next_char, text,char_counter;
int EEP_read_address=0,EEP_write_address = 0;

newline();sendString ("BR 2k4 then AK");newline();            //Text_Baud_Rate_L;

USART_init(1,160);
waitforkeypress();
sendString ("Text file?");newline();                  //Text_message_file;

if ((text = waitforkeypress()) == '"');
else
{while(1){if(isCharavailable(6))
text = receiveChar(); 
if (text == '"') break;}} 


/***************Strings in the .txt file are saved to on-chip EEPROM**************************/

while(1){next_char = waitforkeypress();                           //wait for first character from file
if ((next_char != '\r') && (next_char != '\n'))                   //Ignore leading carriage returns
break;}             
Text_to_EEPROM(&EEP_write_address, next_char);                    //save first letter to EEPROM

while(EEP_write_address < 0x1FA)                                  //Exits before cal bytes can be overwritten
  {if(isCharavailable(6))                                         //returns 1 if a new letter is available (0 at the end of the file) 
  {text = receiveChar();                                          //Temporary storage

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

Num_to_PC(16, EEP_read_address); sendChar('\t');                  //Send address of first line of text

do{char_counter = 0;                                              //Read back text one line at a time
while(char_counter < 150)
{text = Text_from_EEPROM(&EEP_read_address);                      //Increments the read address
char_counter += 1;
if(text)sendChar(text); else break;}                              //When '\0' is detected start next line
newline();
Num_to_PC(16,EEP_read_address);                                   //Send address of next line
sendChar('\t');}
while(EEP_read_address < EEP_write_address);                      //Exit when read address equals write address

Text_Baud_Rate_H;
USART_init(0,16);
waitforkeypress();
wdt_enable(WDTO_60MS); while(1);}
