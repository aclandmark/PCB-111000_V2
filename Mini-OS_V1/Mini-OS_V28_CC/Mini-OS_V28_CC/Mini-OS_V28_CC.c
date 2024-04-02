/*********************

COMPILE using OPTIMIZE level (-01) only (was -Os in winAVR)

The ATMEGA 328 is programmed with the mini-OS in the application partition
and the bootloader in the boot section using "5_Project_pcb_168_V2.30_Arduino_V2"

*EEPROM reservations
0x3FF	user cal if set
0x3FE	user cal if set
0x3FD	Default cal supplied by Atmel
0x3FC	If 'x' pressed at p/r prompt 0x3FC is set to 1 for diagnostic mode.
if 'r' is pressed, normal mode runs
0x3FB	Controls brightness
0x3FA	No longer used
0x3F9	Set to 1 by programmer to trigger auto calibration;
0x3F8	EEP_subs  PRN_8bit_GEN().
0x3F7	Spare for PRN_8bit_GEN() (in case pf burn-out)
0x3F6	Reserved??
Zero to 0x3F5: For user strings and data
***********************/


# include "I2C_V28_0_CC_header_file.h"
# include "../../../Mini-OS_Resources/Mini-OS_header_file.h"
# include "I2C_V28_0_CC_display_subroutines.c"

# include "../../../Mini-OS_Resources/IO_subroutines.c"
# include "../../../Mini-OS_Resources/Arithmetic_subroutines.c"
# include "../../../Mini-OS_Resources/ISR_subroutines_V2.c"
# include "../../../Mini-OS_Resources/osccal_subroutines.c"
# include "../../../Mini-OS_Resources/mode_definitions_V2.h"
# include "../../../Mini-OS_Resources/clock_stop_watch_subroutines.c"
# include "../../../Mini-OS_Resources/Eeprom_subroutines.c"


char *SW_Version = "System files: Mini-OS_V28_CC plus pcb_Bootloader_V4_30_CC \r\n";


char *SW_info = "SW information:\
Use Project_pcb_168_V2.30_Arduino_V2 to update system files\r\n\
See Nano projects directory for user work.\r\n";


# include "../../../Mini-OS_Resources/Mini-OS_Main.c"



