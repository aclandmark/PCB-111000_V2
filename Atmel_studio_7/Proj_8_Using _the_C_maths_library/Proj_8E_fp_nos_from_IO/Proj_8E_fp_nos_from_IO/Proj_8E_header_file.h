

#include "../../../Resources_V15/Header_files/Basic_HW_setup.h"
#include "../../../Resources_V15/Subroutines/Basic_HW_plus_Timer.c"
#include "../../../Resources_V15/Subroutines/Basic_PC_comms.c"
#include "../../../Resources_V15/I2C_Subroutines/I2C_subroutines_1.c"
#include "../../../Resources_V15/I2C_Subroutines/I2C_slave_Rx_Tx.c"
#include "../../../Resources_V15/Header_files/I_O_from_switches.h"
#include "../../../Resources_V15/Subroutines/scanf_and_printf_projects.c"




int Double_from_IO(FILE *mystr_input);
FILE uart_input_Double_from_IO = FDEV_SETUP_STREAM(NULL, Double_from_IO, _FDEV_SETUP_READ);
FILE  mystdout = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);