void newline(void){sendString ("\r\n");}
void sendSpace(void){sendString ("  ");}

	
/*******************************************************************************************/
void Timer_T0_10mS_delay_x_m(int m)
{for (int n = 0; n < m; n++){timer_T0_sub(T0_delay_10ms);}}


/*******************************************************************************************/
void timer_T0_sub(char Counter_speed, unsigned char Start_point){ 
TCNT0 = Start_point;
TCCR0B = Counter_speed;
while(!(TIFR0 & (1<<TOV0)));
TIFR0 |= (1<<TOV0); TCCR0B = 0;}


/*******************************************************************************************/
void timer_T0_sub_with_interrupt(char Counter_speed, unsigned char St_point){ 
TCNT0 = St_point;
TCCR0B = Counter_speed;}
ISR(TIMER0_OVF_vect) {T0_ovf_flag = 1; TCCR0B = 0; }


/*******************************************************************************************/
void Timer_T1_sub(char Counter_speed, unsigned int Start_point){ 
TCNT1H = (Start_point >> 8);
TCNT1L = Start_point & 0x00FF;
TIFR1 = 0xFF;
TCCR1B = Counter_speed;
while(!(TIFR1 & (1<<TOV1)));
TIFR1 |= (1<<TOV1); 
TCCR1B = 0;}


/*******************************************************************************************/
void delay_with_interrupt(char Counter_speed, unsigned char St_point){
TIMSK0 |= (1 << TOIE0); 
timer_T0_sub_with_interrupt(Counter_speed, St_point);
while(!(T0_ovf_flag));T0_ovf_flag = 0;
TIMSK0 &= (~(1 << TOIE0));}



/*******************************************************************************************/
char waitforkeypress(void){
while (!(UCSR0A & (1 << RXC0)));
return UDR0;}


/*******************************************************************************************/
char receiveChar(void)
{return UDR0;}


/*******************************************************************************************/
char isCharavailable (int m){int n = 0;		
while (!(UCSR0A & (1 << RXC0))){n++;
if (n>4000) {m--;n = 0;}if (m == 0)return 0;}
return 1;}


/*******************************************************************************************/
void binUnwantedChars (void){char bin_char;
while(1){if (isCharavailable(2)==1)bin_char = receiveChar();else break;}}



/*******************************************************************************************/
void sendChar(char data){
while (!(UCSR0A & (1 << UDRE0)));
UDR0 = data;}


/*******************************************************************************************/
void sendString(char s[]){
int i = 0;
while(i < 200){
if(s[i] == '\0') break;
sendChar(s[i++]);} }


/*******************************************************************************************/
void sendHex(char radix, unsigned int Hex){ char print_out_string[6];
my_utoa(radix, Hex, print_out_string, 'Z'); sendString(print_out_string);sendString(" ");}//include leading zeros


/*******************************************************************************************/
void sendCharasASKI(char radix, unsigned char CHR){ char print_out_string[4];
my_chartoa(radix, CHR, print_out_string); sendString(print_out_string);sendString(" ");}


/*******************************************************************************************/
void Read_on_chip_EEPROM(int EEPROM_address){
char temp_char, read_counter = 0;
while(1){
temp_char =  eeprom_read_byte((uint8_t*)(EEPROM_address++));
if ((temp_char != '\0') && (++read_counter <= 70)) sendChar(temp_char);
else break;}}



/*******************************************************************************************/
void USART_init (unsigned char UBRROH_N, unsigned char UBRR0L_N ){
{twenty_msec_delay;}	
UCSR0B = 0;
UBRR0H = UBRROH_N;  // for 9.6 and 38.4kB/s;
UBRR0L = UBRR0L_N;  //25 for 38.4kHz baud rate for 8MHz osc. freq.
UCSR0A = (1 << U2X0);
UCSR0B = (1 << RXEN0) | (1<< TXEN0);
UCSR0C =  (1 << UCSZ00)| (1 << UCSZ01);} 


/*******************************************************************************************/
void my_utoa(char radix, unsigned int n, char s[], char mode){
unsigned char i,sl,j,c;
i=0; sl=0;
do{sl++;
s[i] = n % radix;
if (s[i] < 10) s[i]+= '0';
else s[i] += '7';
i++;
}while((n/=radix)>0);
if ((i < 4) && (mode == 'Z')){while(i < 4) {s[i++] = '0';sl++;}}	//new line
s[i] = '\0';
for(i=0, j=sl-1; i<j; i++,j--){
c=s[i];  s[i] = s[j];  s[j] = c;}}



/*******************************************************************************************/
void my_chartoa(char radix, unsigned char n, char s[]){
unsigned char i,sl,j,c;
i=0; sl=0;
do{sl++;
s[i] = n % radix;
if (s[i] < 10) s[i]+= '0';
else s[i] += '7';
i++;
}while((n/=radix)>0);
s[i] = '\0';
for(i=0, j=sl-1; i<j; i++,j--){
c=s[i];  s[i] = s[j];  s[j] = c;}}



/*******************************************************************************************/
int askiX2_to_hex (char a[]) {
int hex;
hex=0;
if (non_numeric_char(a[0])) return 0;
if (non_numeric_char(a[1])) return 0;

a[0] = a[0] - '0'; if (a[0] >=17)a[0] = a[0]-7;
hex = hex + (a[0]<<4);
a[1] = a[1] - '0'; if (a[1] >=17)a[1] = a[1]-7;
hex = hex + a[1];
return hex;}



/*******************************************************************************************/
char non_numeric_char (char data){
if (((data > '9') && (data < 'A')) || (data < '0') || (data > 'F'))return 1;
else return 0;}


/*******************************************************************************************/
void sendsignedHex (int Hex){
char print_out_string[6];
char sign_char = '+';
if (Hex & 0x8000) {Hex = ~Hex + 1; sign_char = '-';}
my_utoa(10, Hex, print_out_string, 'N'); 		//No leading zeros
if (sign_char == '-')
{sendChar('-'); sendString(print_out_string);sendString("  ");}
else {sendChar(' '); sendString(print_out_string);sendString("  ");}}



/*******************************************************************************************/
char Rx_data(void){
unsigned int n = 0;
while (!(UCSR0A & (1 << RXC0))){n++;
if (n > 15000) return 0;}			//was 15000 for BR of 600
return UDR0;}



/*******************************************************************************************/
int askiX4_to_hex ( char a[])
{int m, n;  int hex;
hex=0;n=3;
for(m=0; m<=3; m++){ 
if (a[m]=='\0') {  hex = hex >> (4*(n+1)); return hex;}
if(non_numeric_char(a[m])) return 0;
a[m] = a[m] - '0'; if (a[m] >=17)a[m] = a[m]-7;
hex = hex + (a[m] << (4*n)); n--;}
return hex;}


/*******************************************************************************************/
int my_atoi_with_sign (char s[]){ int i,n;
n=0;
switch (s[0]){
case '-' : 
for(i=1; s[i] >= '0' && s[i] <= '9'; ++i)
n= 10*n + (s[i] - '0'); n = 0-n; break;

case '+' : 
for(i=1; s[i] >= '0' && s[i] <= '9'; ++i)
n= 10*n + (s[i] - '0'); break;

default:
for(i=0; s[i] >= '0' && s[i] <= '9'; ++i)
n= 10*n + (s[i] - '0'); break;}
return n;}

