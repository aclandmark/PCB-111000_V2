

#include "../../../Resources_V15/Header_files/Basic_HW_setup.h"
#include "../../../Resources_V15/Subroutines/Basic_HW_plus_Timer.c"
#include "../../../Resources_V15/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../../Resources_V15/I2C_Subroutines/I2C_subroutines_2.c"
#include "../../../Resources_V15/Subroutines/Basic_PC_comms.c"
#include "../../../Resources_V15/Header_files/I_O_from_switches.h"	
#include "../../../Resources_V15/Subroutines/Integers_to_from_PC.c"
#include "../../../Resources_V15/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../../Resources_V15/I2C_Subroutines/I2C_diagnostic.c"

#include "../../../Resources_V15/Subroutines/Data_to_from_EEPROMs.c"
#include "../../../Resources_V15/Subroutines/Random_and_prime_nos.c"



void Minimise_error(int, char *, char *, long *, char *, char  );
void I2C_initiate_7_8125mS_ref(void);
long compute_error(char, char, char);



long buffer[1];												//Not actually used here
volatile char T1_OVF;
volatile long error_SUM;
char OSCCAL_WV, OSCCAL_DV, New_UC_value;
volatile int EA_counter, EA_buff_ptr;
volatile char cal_mode;




#define waiting_for_I2C_master_with_ISR \
TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWIE);


#define initialise_and_start_timers \
TIMSK1 |= (1 << TOIE1);\
I2C_initiate_7_8125mS_ref();\
waiting_for_I2C_master;\
send_byte_with_Nack(1);\
clear_I2C_interrupt;\
TCNT1=0;TCCR1B = 1;		


#define close_timers \
waiting_for_I2C_master;\
send_byte_with_Nack(0);\
clear_I2C_interrupt;
