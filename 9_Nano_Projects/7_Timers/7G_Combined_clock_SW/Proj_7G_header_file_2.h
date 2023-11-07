

#define User_instructions \
Serial.write(message_1);\
Serial.write(message_2);\
Serial.write(message_3);\
Serial.write(message_4);\
Serial.write(message_5);\
Serial.write(message_6);



# define message_1 \
"\r\nCOMBINED CLOCK/SW:\r\n\
Conect to pc, press sw1 and enter time (or reset and repeat)\r\n\
Power cycle to 5V supply if wanted and press sw1 then sw2 to start\r\n"


# define message_2 \
"Toggle sw2 to switch between clock and new SW and press sw 3 to select old SW\r\n\
Press sw1 to pause, blank or resume display\r\n"


# define message_3 \
"\r\n\
Start by pressing sw3 in place of sw1 \
to enable 2 stop watches and no clock\r\n\r\n\
To adjust time: Press SW2 to restore clock time "


# define message_4 \
"then immediately press SW1 to induce a WDTout\r\n\
Release sw1 to increment time or sw2 to decrement it then both switches to restore normal operation.\r\n"


# define message_5 \
"\r\n\
Bug: Having pressed SW3 when no timer is running \
it is neccessary to press SW2 twice to restore normal operation\r\n"

# define message_6 \
"\r\n\
To force reset avoiding user prompt press sw3 and then \
press reset switch.\r\n"





#define Update_sec_counter  if(sec_counter_save){cli(); sec_counter = sec_counter_V;\
sei();sec_counter_save=0;}

#define setup_64ms_WDT_with_interrupt \
wdr();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = (1<< WDE) | (1 << WDIE) | (1 << WDP1);




void set_time(void);                                      //If mode is 255:   Enables user to enter start; converts it to seconds and saves it in seconds in EEPROM 
                                                          //If mode is zero:  Reads time back out from EEPROM locations
                                                          //If mode is 0:   Reads time back from EEPROM locations 3 - 6; saves resut in "sec_counter"
                                                          //Stores mode in EEPROM locarion 2
void Calculate_time(void);                                //Converts seconds ("sec_counter") to hours, minutes and seconds 
void timer_utoa(char);                                    //Converts hours, minutes and seconds to strings

void Timer(void);                                         //Calls "update_7_seg_display", to close one I2C transaction and sets payload to initiate the next one          
void update_7_seg_display(void);                          //I2C controler: supervises transmission of timer data to the mini OS.
long clockTOstop_watch(void);                             //Saves time before selecting stop watch display (i.e. reseting time to zero)
long restore_Clock(void);                                 //Restores time by adding "clock_time" to the current SW time
long restore_stop_watch(void);                            //Restores SW time by subtracting the curent time from the value stored in "clock_time"
void restore_display(void);                               //Re-activate Stop watch/clock display
void clear_display(void);                                 //Blanks the display
char sw_de_bounce(void);                                  //User Timer with interrupt to control SW bounce
void print_out_bkp(void);                                 //For test only: prints out contents of BKP store EEPROM 7 - 18



/************Define clock memory locations********************/
#define HoursH digits[7]
#define HoursL digits[6]
#define MinsH digits[5]
#define MinsL digits[4]
#define SecsH digits[3]
#define SecsL digits[2]
#define msecsH digits[1]
#define msecsL digits[0]


#define AT_clock_mode 'C'
#define AT_exit_stop_watch 10, '1'
#define AT_mode_1 1

#define display_clock 'D', '1'
#define hide_clock 'D', '2'
#define pause_clock 'D', '3'
#define increment_seconds 'D', '5'
#define decrement_minutes 'D', '6'
#define decrement_seconds 'D', '7'
#define increment_minutes 'D', '8'

#define display_current_time 'E', '1'
#define display_stored_times 'E', '4'
#define store_time 'E', '2'
#define one100ms_mode 'E', '1'
#define ten_ms_mode 'E', '2'


#define set_data_mode                   Char_to_EEPROM(0x02,0);
#define clear_data_mode                 Char_to_EEPROM(0x02,0xFF); 
#define data_mode_not_set               Char_from_EEPROM(0x02)


#define Arduino_non_WDTout              Char_from_EEPROM(0x3EE)
#define Arduino_WDTout                  !(Char_from_EEPROM(0x3EE))
#define set_Arduino_WDTout              Char_to_EEPROM( 0x3EE, 0);
#define clear_Arduino_WDT_flag          Char_to_EEPROM( 0x3EE, 0xFF);

void Char_to_EEPROM(int location, char text){eeprom_write_byte((uint8_t*)(location), text);}
char Char_from_EEPROM(int location){return eeprom_read_byte((uint8_t*) location);}
