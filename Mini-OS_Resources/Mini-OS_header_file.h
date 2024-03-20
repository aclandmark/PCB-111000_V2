


/****************************************************************************************************/
void I2C_master_transmit(char);
char I2C_master_receive(char);
void basic_clock (void);
void multi_mode_clock(void);
void stop_watch(void);
void Message_from_the_OS(void);
void cal_plot_328(void);
void manual_cal_PCB_A_device(void);
void spot_check_328_cal(void);
void start_T2_for_ATMEGA_168_cal(char);



/****************************************************************************************************/
volatile char payload_size;							
volatile int disp_ptr;
volatile char Char_received, exit_loop = 0, display_mask;
volatile char mode=1, timer_mode, inc_secs, inc_mins;
volatile char display_buf[8], clock_buf[8], stop_watch_buf[8],strobe[8];
volatile int buf_ptr, mode_C_ptr;
volatile char T1_ovf_flag=0, timer_2_counter;
volatile signed char output_2;
volatile int Ten_mS_tick = 41;
unsigned long number[6];
signed char exponent[3];


volatile char entry_point;
char Sc_Num_string[16];
int Sc_Num_string_length, Sc_Num_string_pointer, display_char_skip_counter;

volatile int EA_counter, EA_buff_ptr;
int buffer[41];	
volatile char T1_OVF;
volatile long error_SUM;
char OSCCAL_test;
volatile char MUX_cntl, T0_interupt_cnt;

char I2C_data[10];

char  display_backup[9];
signed char exponent_BKP[2];
int RN;
char keypres_counter_old;
char overflow;

char Op;
unsigned long RHSofDP;
signed char expnt_result;
char result[4];
int accumlator;
int interim_result;

char OSCCAL_WV;
char OSCCAL_DV;
volatile char cal_mode; 			//Defines number of averages used when measuring osccal_error	

char clock_flag=0, stop_watch_flag, PIC_cmd, Ten_mS_tick_counter = 0, stop_watch_mode;



/****************************************************************************************************/
#define setup_watchdog \
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;

#define wdr()  __asm__ __volatile__("wdr")




#define initialise_Arithmetic_variables \
exponent_BKP[0]=0; exponent_BKP[1]=0;\
expnt=0;\
Op = 0;\
accumlator=0;\
RN=0;keypres_counter_old=0; overflow=0;\
RHSofDP = 0;expnt_result = 0;



/****************************************************************************************************/
#define two_fifty_mS_delay timer_T1_sub_with_interrupt(T1_delay_250ms);while (T1_ovf_flag == 0);T1_ovf_flag = 0;
#define T1_delay_250ms 5,0xF861
#define two_fifty_mS_delay_with_interrupts TIMSK1 |= (1 << TOIE1);two_fifty_mS_delay; TIMSK1 &= (~(1 << TOIE1));

#define one_sec_delay timer_T1_sub_with_interrupt(T1_delay_1sec);while (T1_ovf_flag == 0);T1_ovf_flag = 0;
#define T1_delay_1sec 5,0xE17B
#define one_sec_delay_with_interrupts TIMSK1 |= (1 << TOIE1);one_sec_delay; TIMSK1 &= (~(1 << TOIE1));



/**********************************************************************************************************************/
#define T0_delay_33ms 5,0
#define T0_delay_20ms 5,100
#define T0_delay_16ms 5,131
#define T0_delay_12ms 5,162
#define T0_delay_10ms 5,178
#define T0_delay_8ms 5,194
#define T0_delay_6ms 4,70
#define T0_delay_5ms 4,100
#define T0_delay_4ms 4,130
#define T0_delay_3ms 4,162
#define T0_delay_2500us 4,178
#define T0_delay_2ms 3,0
#define T0_delay_1500us 3,69
#define T0_delay_1ms 3,128
#define T0_delay_667us 3,173


#define T0_delay_875us 3,144


#define T0_delay_125us 3,240
#define T0_delay_1875us 3,16

#define T0_delay_250us 3,224
#define T0_delay_S_250us 4,248
#define T0_delay_3750us 4,136

#define T0_delay_350us	3, 212
#define T0_delay_1650us	3, 50

#define T0_delay_375us 4, 244
#define T0_delay_5625us 4, 76


#define T0_delay_500us 4,240
#define T0_delay_7500us 4,16

#define T0_delay_625us 5,251
#define T0_delay_9375us 5,181

#define T0_delay_750us 5,250
#define T0_delay_11250us 5,166

#define T0_delay_1250us 5, 246
#define T0_delay_18750us 5, 106

#define T0_delay_2000us 5, 240
#define T0_delay_30000us 5,16



#define T1_delay_500ms 5, 0xF0C3
//#define T1_delay_20ms 3, 0xF63C


/****************************************************************************************************/
#define refresh_clock_display   for (int n = 0; n < 8; n++){display_buf[n] = clock_buf[n];}
#define refresh_stop_watch_display  for (int n = 0; n < 8; n++){display_buf[n] = stop_watch_buf[n];}
#define initiate_stop_watch_display  for (int n = 0; n < 8; n++){stop_watch_buf[n] = '0';}

#define increment_seconds	if (clock_buf[0] < '9') clock_buf[0]++; else {clock_buf[0] = '0';\
if (clock_buf[1] < '5') clock_buf[1]++;else {clock_buf[1] = '0';\
if (clock_buf[3] < '9') clock_buf[3]++; else {clock_buf[3] = '0';\
if (clock_buf[4] < '5') clock_buf[4]++;else {clock_buf[4] = '0';}}}}

#define decrement_seconds if (clock_buf[0] > '0') clock_buf[0]--; else {clock_buf[0] = '9';\
if (clock_buf[1] > '0') clock_buf[1]--; else {clock_buf[1] = '5';\
if (clock_buf[3] > '0') clock_buf[3]--; else {clock_buf[3] = '9';\
if (clock_buf[4] > '0') clock_buf[4]--;else {clock_buf[4] = '5';}}}}


#define increment_minutes if (clock_buf[3] < '9') clock_buf[3]++; else {clock_buf[3] = '0';\
if (clock_buf[4] < '5') clock_buf[4]++;else {clock_buf[4] = '0';\
switch (clock_buf[7]){\
case '0': case '1': if (clock_buf[6] < '9') clock_buf[6]++; else {clock_buf[6] = '0';clock_buf[7]++;}break;\
case '2': if (clock_buf[6] < '3') clock_buf[6]++; else {clock_buf[6] = '0';clock_buf[7] = '0';}}}}


#define decrement_minutes if (clock_buf[3] > '0') clock_buf[3]--; else {clock_buf[3] = '9';\
if (clock_buf[4] > '0') clock_buf[4]--;else {clock_buf[4] = '5';\
switch (clock_buf[7]){\
case '1': case '2': if (clock_buf[6] > '0') clock_buf[6]--; else {clock_buf[6] = '9';clock_buf[7]--;}break;\
case '0': if (clock_buf[6] > '0') clock_buf[6]--; else {clock_buf[6] = '3';clock_buf[7] = '2';}}}}


/****************************************************************************************************/
#define Initialise_I2C_master_write \
	while(1){\
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);\
	while (!(TWCR & (1 << TWINT)));\
	TWDR = 0x02;\
	TWCR = (1 << TWINT) | (1 << TWEN);\
	while (!(TWCR & (1 << TWINT)));\
	if (TWSR == 0x18)break;	}	
	
#define Initialise_I2C_master_read \
	while(1){\
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);\
	while (!(TWCR & (1 << TWINT)));\
	TWDR = 0x03;\
	TWCR = (1 << TWINT) | (1 << TWEN);\
	while (!(TWCR & (1 << TWINT)));\
	if (TWSR == 0x40)break;	}


#define long_to_slave_I2C \
	while(1){\
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);\
	while (!(TWCR & (1 << TWINT)));\
	TWDR = 0x02;\
	TWCR = (1 << TWINT) | (1 << TWEN);\
	while (!(TWCR & (1 << TWINT)));\
	if (TWSR == 0x18)break;	}\
	for(int m = 0; m<=3; m++)\
	{TWDR = result[m];\
	TWCR = (1 << TWINT) | (1 << TWEN);\
	while (!(TWCR & (1 << TWINT)));}\
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);	




		



