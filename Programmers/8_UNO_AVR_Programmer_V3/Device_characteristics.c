
#include <avr/wdt.h>
#define signature_bit_2_h 0x3000
#define signature_bit_2_l 0x0100
#define signature_bit_3_h 0x3000
#define signature_bit_3_l 0x0200
#define SW_reset {wdt_enable(WDTO_30MS);while(1);}

int sig_byte_2, sig_byte_3;
signed int PageSZ;												        //Size of a page of flash in 16 bit words
signed int PAmask;												        //Used to obtain the flash page address from the hex address
unsigned int FlashSZ;											        //Amount of flash memory in 16 bit words supplied on target device
int EE_size;													            //EEPROM size
unsigned char Fuse_Ex, Fuse_H, Fuse_L, Lock;      //All fuse (configuration byte)setings can be modified by the user

char Twd_EEPROM;

void sendString(const char*);
void sendChar(char);
unsigned char Atmel_config ( unsigned int, unsigned int );
void set_up_target_parameters(void);
char waitforkeypress(void);
void newline(void);





/**************************************************************************************************************/

void set_up_target_parameters(void){


const char *Atmega = "\r\nAtmega ";            

const char *Unrecognised_device = "\r\nUnrecognised_device:  ";

Twd_EEPROM = 5;																            //default delay in mS

sig_byte_2 = Atmel_config(signature_bit_2_h, signature_bit_2_l);
sig_byte_3 = Atmel_config(signature_bit_3_h, signature_bit_3_l);


switch(sig_byte_2)
	{	case 0x94:  															            //16KB devices
	switch (sig_byte_3)
	{case 0x06: sendString(Atmega);sendString("168");break;
	case 0x0B: sendString(Atmega);sendString("168P");break;
	default: sendString(Unrecognised_device);sendString("16KB");
	newline();SW_reset;break;}
	
	PageSZ = 0x40;			
	PAmask = 0x1FC0;														            //=FlashSZ - PageSZ		
	FlashSZ = 0x2000;		
	EE_size = 0x200;		
	Fuse_Ex = 0xFF;
	Fuse_H = 0xD7;
	Fuse_L = 0xE2;
	Lock = 0xFF;
	break;
	
	default: sendString("\r\nProgramming restricted to the Atmega168 used for setting up PCB-111000_V2."); 
	sendString("\r\n"); wdt_enable(WDTO_60MS);while(1);break;}}




 /**********************************************************************************************************************************************/
