

#define port_reg   digits[0]
#define cpu_reg_2   digits[1]
#define cpu_reg_1   digits[2]
#define Display_registers   I2C_Tx_BWops(digits)

#define clear_bits \
String_to_PC_Basic("\r\n\r\nClear bits");\
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
String_to_PC_Basic("\r\nAgain\r\n");


#define set_bits \
String_to_PC_Basic("\r\n\r\nSet bits");\
String_to_PC_Basic("\r\nStep 2  OR with PORT register");\
waitforkeypress_Basic();\
port_reg = port_reg | cpu_reg_1;\
Display_registers;\
cpu_reg_1 = 0;\
_delay_ms(500);\
Display_registers;\
String_to_PC_Basic("\r\nAgain\r\n");
