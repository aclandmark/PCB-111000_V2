

#define port_reg   digits[0]
#define cpu_reg_2   digits[1]
#define cpu_reg_1   digits[2]
#define Display_registers   I2C_Tx_BWops(digits)

#define clear_bits \
for(int m = 0; m <=7; m++)reg_bkp[m] = 0;\
String_to_PC_Basic("\r\nStep 2: Invert CPU register");\
waitforkeypress_Basic();\
cpu_reg_1 = ~cpu_reg_1;\
Display_registers;\
\
String_to_PC_Basic("\r\nStep 3  AND with PORT register");\
waitforkeypress_Basic();\
port_reg = port_reg & cpu_reg_1;\
Display_registers;\
cpu_reg_1 = 0;\
_delay_ms(500);\
Display_registers;\
String_to_PC_Basic("\r\n\r\nAgain");


#define set_bits \
for(int m = 0; m <=7; m++)reg_bkp[m] = 0;\
String_to_PC_Basic("\r\nStep 2  OR with PORT register");\
waitforkeypress_Basic();\
port_reg = port_reg | cpu_reg_1;\
Display_registers;\
cpu_reg_1 = 0;\
_delay_ms(500);\
Display_registers;\
String_to_PC_Basic("\r\n\r\nAgain");


#define change_bits \
for(int m = 0; m <=7; m++)reg_bkp[m] = 0;\
String_to_PC_Basic("\r\nStep 2  EOR with PORT register");\
waitforkeypress_Basic();\
port_reg = port_reg ^ cpu_reg_1;\
Display_registers;\
cpu_reg_1 = 0;\
_delay_ms(500);\
Display_registers;\
String_to_PC_Basic("\r\n\r\nAgain");


#define test_one_bit \
for(int m = 0; m <=7; m++)reg_bkp[m] = 0;\
String_to_PC_Basic("\r\nStep 2  AND with PORT register");\
waitforkeypress_Basic();\
cpu_reg_2 = port_reg & cpu_reg_1;\
Display_registers;\
cpu_reg_1 = 0;\
String_to_PC_Basic("ak");waitforkeypress_Basic();\
Display_registers;\
cpu_reg_2 = 0;\
String_to_PC_Basic("ak");waitforkeypress_Basic();\
Display_registers;\
String_to_PC_Basic("\r\n\r\nAgain");







/*************************************************************************/
