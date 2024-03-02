
#include "Timer_interrupt_header.h"

#define I2C_initiate_10mS_ref \
TCCR2B = 2;\
TIMSK2 |= ((1 << OCIE2A) | (1 << TOV2));\
wdt_enable(WDTO_120MS);


volatile int TCNT2_BKP; 

int main (void){
setup_HW_Arduino;

TCNT2 = 0;
TCCR2A = 0;
OCR2B = 0;

Serial.write("\r\nT2 tests: -R- for polling, -r- for ISR then press AK \r\n");
User_prompt_A;

TCNT2=0;
OCR2A = 250;
TIFR2 = 0xFF; 
TCCR2B = 7; 

if(User_response == 'R'){while(1){if(TIFR2 & 0x02)break;}TIMER_2_TEST();TCCR2B = 0;}

else{
while(1){if(TIFR2 & 0x02){TCNT2_BKP = TCNT2;break;} }
while(TCNT2 < 254);TCCR2B = 0;Serial.print((int)TCNT2);
Serial.print("\r\nT2 interrupt flag set\r\nAK to run ISR\r\n");
waitforkeypress_A();TIMSK2 |= (1 << OCIE2A);}

SW_reset;}



ISR(TIMER2_COMPA_vect){Serial.print((int)TCNT2_BKP);Serial.write(": Runs ISR\r\n");}

void TIMER_2_TEST(void)
{Serial.print((int)TCNT2);TIFR2 |= 2;Serial.write(": Runs Subroutine\r\n");}





/************************************************************************/
