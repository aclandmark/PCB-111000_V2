

#include "../../Resources_V12/Header_files/Basic_HW_setup.h"
#include "../../Resources_V12/Subroutines/Basic_HW_plus_Timer.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../Resources_V12/Subroutines/Basic_PC_comms.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_slave_Rx_Tx.c"

#include "../../Resources_V12/Subroutines/Data_to_from_EEPROMs.c"
#include "../../Resources_V12/Subroutines/Random_and_prime_nos.c"
#include "../../Resources_V12/I2C_Subroutines/I2C_diagnostic.c"



void String_to_PC_local(char*);
void String_to_PC_alt(char* s);