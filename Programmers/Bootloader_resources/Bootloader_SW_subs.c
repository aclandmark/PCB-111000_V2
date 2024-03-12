
void USART_init (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
{twenty_msec_delay;}	
UCSR0B = 0;
UBRR0H = UBRROH_N; 
UBRR0L = UBRR0L_N;
UCSR0A = (1 << U2X0);
UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);} 


void sendString(char s[]){
int i = 0;
while(i < 200){
if(s[i] == '\0') break;
sendChar(s[i++]);} }

char isCharavailable (int m){int n = 0;		
while (!(UCSR0A & (1 << RXC0))){n++;
if (n>4000) {m--;n = 0;}if (m == 0)return 0;}
return 1;}

char receiveChar(void)
{return UDR0;}


void sendChar(char data){
while (!(UCSR0A & (1 << UDRE0)));
UDR0 = data;}

void waitforchar(void){unsigned char n=225; 																																			
while (!(UCSR0A & (1 << RXC0))) {n--; if(n==0) break;}}
	
char waitforkeypress (void){										//Wait here indefinitely but prevent WDTime-out
	while (!(UCSR0A & (1 << RXC0)));
return UDR0;}	

void newline(void){sendString ("\r\n");}


void new_record(void){int B_int_r_pointer;			
next_char;  
B_int_r_pointer = r_pointer;
line_length =  store[B_int_r_pointer]; inc_r_pointer; 
Count_down = line_length/2; 
if (line_length_old < 0x10)short_line=1;
line_length_old = line_length;
next_char;
B_int_r_pointer = r_pointer;
Hex_address  =  (store[B_int_r_pointer]); inc_r_pointer; 
Hex_address  = Hex_address/2;
line_counter++;
if (Hex_address == PIC_address) orphan = 0;
else{if (Hex_address == 0);
		else{if ((Hex_address & PAmask) > (page_address + PageSZ)){section_break=1;page_break=0;}
			if((Hex_address & PAmask) == (page_address + PageSZ)){section_break=0;page_break=1; }
			if ((Hex_address & PAmask) == page_address){section_break=0;page_break=0; orphan = 0;}}}
if(line_counter & 0b00001000) {LED_2_on;}	 
else {LED_2_off;}
} 



void start_new_code_block(void){signed char page_offset;
PIC_address = Hex_address;
page_address = (Hex_address & PAmask);
write_address = Hex_address - page_address;
page_offset = write_address;
line_offset = Hex_address & 0x0007;
offset = page_offset;
space_on_page = (PageSZ - page_offset);}


void Program_record(void){
while(Count_down){ 
Count_down--;
Flash_flag = 1;   
decode_cmd_write_to_flash_SUB();   
write_address++;
PIC_address++;
space_on_page--;
prog_counter++;       
if (write_address == PageSZ){
Read_write_mem('W', page_address, 0x0);
Flash_flag = 0;
write_address = 0;
space_on_page = PageSZ;
offset = line_offset; 
if (offset) orphan = 1;}}}


void decode_cmd_write_to_flash_SUB(void){
decode_HEX_cmd_SUB();
Load_page('L', write_address, Hex_cmd & 0x00FF); 
Load_page('H', write_address, (Hex_cmd >> 8));}

void decode_HEX_cmd_SUB(void){int A_int_r_pointer;
next_char;
A_int_r_pointer = r_pointer;
Hex_cmd = store[A_int_r_pointer];inc_r_pointer;
}

void write_page_SUB(signed int page_address){
Read_write_mem('W',  page_address, 0x0);
Flash_flag = 0;}


