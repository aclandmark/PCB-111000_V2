
//Prototypes required for "Project_pcb_168_V2.30B.c"
void Read_on_chip_EEPROM(int);
void Prog_Target_EEPROM(void);
void Prog_on_chip_EEPROM(void);
void Program_Flash (void);
void Verify_Flash (void);

//Prototypes required for "Project_pcb_168_V2_30B_EEPROM_subs.c"
char Download_text(int*,int*,char*,int*, unsigned char*);
char Download_data(int*,int*, char*, int*,int*, unsigned char*);
int decimal_conversion (char, int*);
int Hex_conversion (char, int*);
void Upload_text(int);
void Upload_data(int, int);
void Upload_data_1(int, int);
void Upload_data_2(int, int);
char next_char_from_PC(void);
int askiX4_to_hex_V2 ( char*);

//Prototypes for "Project_pcb_168_V2.30B_SW_subs"
void new_record(void); 
void start_new_code_block(void);
void Program_record(void);
void copy_cmd_to_page_buffer(void);
void get_next_hex_cmd(void);
void write_page_SUB(int);
char receive_byte_with_Ack(void);
char receive_byte_with_Nack(void);

//Prototypes for "1_Basic_Timer_IO_subs"
void newline(void);
void sendSpace(void);
void Timer_T0_10mS_delay_x_m(int);
void timer_T0_sub(char, unsigned char);
void timer_T0_sub_with_interrupt(char, unsigned char);
void timer_T1_sub(char, unsigned int);
void delay_with_interrupt(char, unsigned char);
char waitforkeypress(void);
char receiveChar(void);
char isCharavailable (int);
void binUnwantedChars (void);
void sendChar(char);
void sendString(char*);
void sendHex(char, unsigned int);
void sendCharasASKI(char, unsigned char);
void USART_init (unsigned char, unsigned char);
void my_utoa(char, unsigned int, char*, char);
void my_chartoa(char, unsigned char, char*);
int askiX2_to_hex (char*);
char non_numeric_char (char);
void sendsignedHex (int);
char Rx_data(void);
int askiX4_to_hex (char*);
int my_atoi_with_sign (char*);

//Prototypes for "1_ATMEGA_Programmer_V2.25_HW_subs"
char Atmel_config ( unsigned int, unsigned int );
char Read_write_mem(char, int, char);
void Load_page(char, int, unsigned char);

//Prototypes for "Project_pcb_168_V2.30B.h"
void Clock_period(void);
void Clock_period(void){for(int p = 0; p<= 3; p++){asm("nop");}}



#define two_msec_delay;		timer_T0_sub(delay_2ms);
#define five_msec_delay;	timer_T0_sub(delay_5ms);
#define ten_msec_delay;		timer_T0_sub(delay_10ms);
#define twenty_msec_delay;	timer_T0_sub(delay_20ms);
#define T0_delay_10ms 5,178
#define T1_delay_500mS 5,0xF0BE

#define PGClock_L  
#define PGClock_H  Clock_period();
#define delay_20ms 5,100
#define delay_10ms 5,183
#define delay_5ms 5,220
#define delay_2ms 4,195

#define RBL 127

#define inc_r_pointer;\
r_pointer++;\
r_pointer = r_pointer & 0b00011111;


int  cmd_counter;											//Counts commands as they are downloaded from the PC
int prog_counter;											//Counts commands burned to flash
signed int  read_ops=0;										//Total number of commands read from flash
volatile int counter;										//Counts characters in a record as they are downloded from the PC
volatile int char_count;									//The number of askii character in a single record
volatile unsigned char Count_down;							//Counts commands as record is programmed
volatile int   tempInt1, tempInt2;							//Used to assemble commands and addresses as the are downloaded
int store[35];												//Used to store commands and address ready for the programmer
volatile unsigned char w_pointer,r_pointer;					//Read/write pointers to "store" to which hex file is saved
unsigned int Hex_cmd;										//Command read from flash during verification

unsigned char cmd_pin, resp_pin, clock_pin, reset_pin;		//Used to define the programming pins

unsigned int target;
int Hex_address;											//Address read from the hex file
int HW_address;												//Hard ware address (usually tracks Hex_address)
signed int page_address;									//Address of first location on a page of flash 
volatile int write_address;									//Address on page_buffer to which next command will be written
signed int FlashSZ;											//Amount of flash memory supplied on target device
signed int PAmask;											//Used to obtain the flash page address from the hex address
signed int PageSZ;											//Size of a page of flash

signed char short_record;									//Record  containing less that eight 16 bit commands
signed char page_offset;									//Address of first location on page buffer to be used
signed char space_on_page;									//Keeps a track of the space remaining on a page buffer
unsigned char Flash_flag;									//Indicates that the page buffer contains commands

signed char record_length;
signed char record_length_old;
signed char orphan;											//Indicates that the contents of a record span two flash pages
signed char section_break;									//Set to 1 if at least one page of flash memory will be unused.
signed char page_break;										//Page only partialy filled before programming next one starts
volatile signed char line_offset;
unsigned int prog_led_control;


volatile char T0_ovf_flag=0;								//Used by "timer_sub_with_interrupt"
signed int EE_top;											//Max address available for user srings
char  cal_factor=0;											//1: Use call factor	0: default cal factor
int text_start, text_start_mem;								//Controls writing user strings to rarget EEPROM
char watchdog_reset;										//Set to 1 when watchdog timeout occurs



/*****************************************************************************/
#define setup_HW \
setup_watchdog;\
OSC_CAL;\
set_up_I2C;\
ADMUX |= (1 << REFS0);\
set_up_I_O;\
eeprom_write_byte((uint8_t*)(0x1FD),OSCCAL);\
while (!(PIND & (1 << PD1)));\
Timer_T0_10mS_delay_x_m(5);\
USART_init(0,16);\
Set_LED_ports;\
LEDs_off;


/*****************************************************************************/
#define setup_watchdog; \
if (MCUSR & (1<<WDRF)) watchdog_reset = 1;\
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;\


/*****************************************************************************/
#define set_up_I2C; TWAR = 0x02;


/*****************************************************************************/
#define OSC_CAL; \
if ((eeprom_read_byte((uint8_t*)0x1FF) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x1FF) < 0xF0) && (eeprom_read_byte((uint8_t*)0x1FF)\
== eeprom_read_byte((uint8_t*)0x1FE))) {OSCCAL = eeprom_read_byte((uint8_t*)0x1FF);cal_factor=1;}



/*****************************************************************************/
#define set_up_I_O;\
MCUCR &= (~(1 << PUD));\
DDRB = 0;\
DDRC = 0;\
DDRD = 0;\
PORTB = 0xFF;\
PORTC = 0xFF;\
PORTD = 0xFF;\
PORTC &= (~(1 << PC3));	



/************************************************************************************************************************************/
#define Initialise_variables_for_programming_flash \
prog_counter=0;  prog_led_control = 0; cmd_counter = 0; record_length_old=0;\
Flash_flag = 0;  HW_address = 0;  section_break = 0; orphan = 0;\
w_pointer = 0; r_pointer = 0; short_record=0;\
counter = 1;



/*****************************************************************************/
#define Prog_enable 0xAC530000
#define Prog_enable_h 0xAC53

#define signature_bit_1 0x30000000
#define signature_bit_1_h 0x3000

#define signature_bit_2 0x30000100
#define signature_bit_2_h 0x3000
#define signature_bit_2_l 0x0100

#define signature_bit_3 0x30000200
#define signature_bit_3_h 0x3000
#define signature_bit_3_l 0x0200

#define read_fuse_bits 0x50000000
#define read_fuse_bits_h 0x5000

#define read_fuse_bits_H 0x58080000
#define read_fuse_bits_H_h 0x5808

#define read_lock_bits 0x58000000
#define read_lock_bits_h 0x5800

#define read_extended_fuse_bits 0x50080000
#define read_extended_fuse_bits_h 0x5008

#define write_lock_bits 0xACE00000
#define write_lock_bits_h 0xACE0

#define write_fuse_bits_H 0xACA80000
#define write_fuse_bits_H_h 0xACA8

#define write_fuse_bits 0xACA00000
#define write_fuse_bits_h 0xACA0

#define write_extended_fuse_bits 0xACA40000
#define write_extended_fuse_bits_h 0xACA4

#define Chip_erase 0xAC800000
#define Chip_erase_h 0xAC80


/*****************************************************************************/
#define PGD_cmd_H PORTB |= cmd_pin
#define PGD_cmd_L PORTB &= ~(cmd_pin)
#define PGD_resp_H PINB & resp_pin
#define PGC_L PORTB &= ~(clock_pin)
#define PGC_H PORTB |= clock_pin
#define Reset_L PORTC &= ~(reset_pin)
#define Reset_H PORTC |= reset_pin

#define boot_target; cmd_pin =  0x08; resp_pin = 0x10; clock_pin =  0x20; reset_pin = 0x08; DDRB |= 0x28; DDRC |= 0x08;


/*****************************************************************************/
#define Set_LED_ports;	DDRB = (1 << DDB0) | (1 << DDB1);
#define LEDs_on; PORTB |= (1 << PB0)|(1 << PB1);
#define LEDs_off; PORTB &= (~((1 << PB0)|(1 << PB1)));
#define LED_1_on; PORTB |= (1 << PB1);
#define LED_1_off; PORTB &= (~( 1<< PB1));	
#define LED_2_off; PORTB &= (~(1 << PB0));
#define LED_2_on; PORTB |= (1 << PB0);



/**********HW V 1.3 Define target Pin & CA LED definitions STOP******************************/

#define Atmel_powerup;\
{two_msec_delay;}\
Reset_L;\
PGC_L;\
{two_msec_delay;}\
Reset_H;\
{two_msec_delay;}\
Reset_L;\
{twenty_msec_delay;}

#define Atmel_powerup_and_target_detect;\
Atmel_powerup;\
while(1){if((Atmel_config(Prog_enable_h, 0)==0x53) && (Atmel_config(signature_bit_1_h, 0) == 0x1E))break;\
else {sendString("TTND"); wdt_enable(WDTO_120MS);while(1);}}\
target_type_M = Atmel_config(signature_bit_2_h, signature_bit_2_l);\
target_type = Atmel_config(signature_bit_3_h, signature_bit_3_l);\
switch(target_type) {\
case 0x07: target = 8; break;\
case 0x08: target = 35; break;\
case 0x05:\
case 0x0A:\
if (target_type_M == 0x92) target = 48; else target = 88; break;\
case 0x14:\
case 0x0F: if(target_type_M == 0x95) target = 328; else target = 88; break;\
case 0x06:\
case 0x0B: if(target_type_M == 0x94)target = 168; if(target_type_M == 0x93)target = 15;break;\
default: newline(); sendString("TTND"); newline(); wdt_enable(WDTO_120MS);while(1);break;}



/*****************************************************************************/
#define Print_hex_file;\
if((((Hex_cmd>>8)<0x20)||((Hex_cmd>>8)>0x7E))&&(((Hex_cmd & 0x00FF)<0x20)||((Hex_cmd & 0x00FF)>0x7E)))sendHex(16, Hex_cmd);\
else{if(((Hex_cmd>>8)>=0x20) && ((Hex_cmd>>8)<=0x7E) && ((Hex_cmd & 0x00FF)>=0x20) && ((Hex_cmd & 0x00FF)<=0x7E))\
{sendChar(Hex_cmd>>8); sendChar(Hex_cmd & 0x00FF);}\
else { if(((Hex_cmd>>8)>=0x20) && ((Hex_cmd>>8)<=0x7E)){sendChar(Hex_cmd>>8); sendCharasASKI(16, (Hex_cmd & 0x00FF));}\
if(((Hex_cmd & 0x00FF)>=0x20) && ((Hex_cmd & 0x00FF)<=0x7E)){sendCharasASKI(16, (Hex_cmd>>8)); sendChar(Hex_cmd & 0x00FF);}}}



/*****************************************************************************/
#define Text_Atmega; newline(); Read_on_chip_EEPROM(0x0);
#define Text_detected; Read_on_chip_EEPROM(0x8); newline();
#define Text_Press_P_or_E; Read_on_chip_EEPROM(0x12);newline();
#define Text_Send_HexF; newline(); Read_on_chip_EEPROM(0x5D); newline();
#define Text_EEP_reset; Read_on_chip_EEPROM(0x7E); newline();
#define Text_10_sec_wait; Read_on_chip_EEPROM(0x98);

#define Text_Config; Read_on_chip_EEPROM(0xA4); newline();
#define Text_on_chip_cal; Read_on_chip_EEPROM(0xD5);
#define Text_File_size; newline(); Read_on_chip_EEPROM(0xE6);
#define Text_Auto_cal; Read_on_chip_EEPROM(0xF6); newline();
#define Text_target_cal; newline();Read_on_chip_EEPROM(0x130);
#define Text__Press_W_or_R; newline(); Read_on_chip_EEPROM(0x149);

#define Text_0-0xFF; Read_on_chip_EEPROM(0x17A);
#define Text_Send_TF; Read_on_chip_EEPROM(0x189);
#define Text_Baud_Rate_L; newline(); Read_on_chip_EEPROM(0x199); newline();
#define Text_message_file; Read_on_chip_EEPROM(0x1BD); newline();
#define Text_Baud_Rate_H; newline(); Read_on_chip_EEPROM(0x1CF); newline();




/*****************************************************************************/
#define User_prompt \
while(1){\
do{sendString("R?    ");}	 while((isCharavailable(250) == 0));\
User_response = receiveChar();\
if((User_response == 'R') || (User_response == 'r'))break;} sendString("\r\n");



/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;


#define clear_I2C_interrupt \
TWCR = (1 << TWINT);
