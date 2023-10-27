

#include "Proj_9E_header_file.h"

volatile char T1_OVF;
volatile long error_SUM;
unsigned char OSCCAL_WV, OSCCAL_DV;
volatile int EA_counter;

char Num_string[12];



int main (void){

long error;

setup_HW_Arduino_IO_Extra;                              //"setup_HW"initially saves default OSCCAL in 0x1FD and
OSCCAL_DV = eeprom_read_byte((uint8_t*)0x1FD);          //then checks for "user-cal" and copies it to OSCCAL if present
OSCCAL_WV = OSCCAL;                                     //Save actual cal value: could be "user-cal" or the default OSCCAL

Serial.write("Cal factor working value   \t");                      //Print the actual (working) value of OSCCAL
Int_Num_to_PC_A(OSCCAL_WV, Num_string, '\r' );
Serial.write("Cal factor default value   \t");        //Print OSCCAL_default_Value
Int_Num_to_PC_A(OSCCAL_DV , Num_string, '\r' ); 

Serial.write("Cal error tabulated against OSCCAL\r\n");

_delay_ms(10);
  
for(int m = 0x10; m <= 0xF0; m++){ 
error = compute_single_error(m);
Int_Num_to_PC_A(m, Num_string, '\t');
Int_Num_to_PC_A(error, Num_string, ' '); 
Serial.write("   \t");
Int_Num_to_PC_A(error*100/62500, Num_string,'%');
newline_A();_delay_ms(25);}

Serial.write("AK to repeat"); 
waitforkeypress_A();
SW_reset;
}


/**********************************************************************************************/
long compute_error(char error_mode) 
    {long error;
    waiting_for_I2C_master_with_ISR;                  //TWI generates interrupt every 7.8125mS 
    EA_counter = 0;                                   //Compute error for each value of OSCCAL 10 times
    error_SUM = 0;
    while(EA_counter < 15);EA_counter = 0;TWCR = 0;   //wait here for 15 TWI interrupts
    error = error_SUM;
    return error/10;}                                 //return average error values
  



/**********************************************************************************************/
ISR(TIMER1_OVF_vect){T1_OVF++; }                  //T1 should count to 62500 in 7.8125ms.

ISR(TWI_vect){                                   //T2 (mini_OS) overflows: giving a calibrated tick rate
long TCNT1_BKP;
TWDR;                                             //Read TWDR the data register
send_byte_with_Nack(1);                           //Master responds by remaining in mode P
clear_I2C_interrupt;

TCCR1B = 0;                                       //Halt T1
TCNT1_BKP = TCNT1;                                //Copy the value of TCNT1
TCNT1 = 0;                                        //Clear TCNT1
TCCR1B = 1;                                       //Get T1 running again ASAP (Note T2 (mini_OS) has not stopped running)
if(EA_counter < 5)T1_OVF = 0;                     //Ignore first 5 results
else                                              //Sum error results for the next 20 TWI interrupts

{if(T1_OVF)                                       //compute error when T1 does and does not overflow
{T1_OVF = 0;
error_SUM = error_SUM + TCNT1_BKP + 3033;}    
else {error_SUM = error_SUM + TCNT1_BKP - 62500;}}
EA_counter++;                                     //counter increments up to 25.

waiting_for_I2C_master_with_ISR;}                 //Be ready for next TWI interrupt 



/**********************************************************************************************/
void I2C_initiate_7_8125mS_ref(void){
char num_bytes=0;
char mode = 'U';
waiting_for_I2C_master;
send_byte_with_Ack(num_bytes);
send_byte_with_Nack(mode);
clear_I2C_interrupt;} 



/**********************************************************************************************/
long compute_single_error(char OSCCAL_TV){         //Trial value
long error;
char SREG_bkp;

SREG_bkp = SREG;
OSCCAL = OSCCAL_TV;                             //Set OSCCAL equal to the trial value           
TIMSK1 |= (1 << TOIE1);                         //Enable T1 interrupt
I2C_initiate_7_8125mS_ref();                    //Request Mode P: 7.8125mS ref signal from master
waiting_for_I2C_master;                         //First 7.8125mS tick
send_byte_with_Nack(1);                         //Master responds by staying in mode P
clear_I2C_interrupt;
TCNT1=0;TCCR1B = 1;                             //Start T1 with no prescalling (at 8MHz)
sei();
error = compute_error(0);
waiting_for_I2C_master;
send_byte_with_Nack(0);                         //Master responds by exiting mode P
clear_I2C_interrupt;
TIMSK1 &= (~(1 << TOIE1));                      //Disable T1 interrupt
SREG = SREG_bkp;
OSCCAL = OSCCAL_WV;                             //Restore safe value of OSCCAL
return error;} 



/*****************************************************************************************************************/
