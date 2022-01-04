
There are three versions of PCB111000:

PCB_111000_New using two Atmega devices and the FTDI FT230XS USB bridge
PCB_111000_UNO (Also known as PCB111000_1) A simplified plug in version for use with the UNO
PCB_111000_CP2102 Using one Atmega one ATtiny and a CP2102 module for USB connectivity.


The new version is based on
PCB111000		The original development
PCB111000_V2		A tidied up version of the original

Original development used the winAVR compiler and programer/bootloader code 
developed especially for the project
V2 used the USBasp programmer
Remaining versions used Arduino and Studio 7 compilers. Programming was carried out
using the "UNO as AVR programmer" and "UNO as UPDI programmer" Arduino projects.


