/*
EEPROM usage
0x1FF, 0x1FE and 0x1FD		OSCCAL
0x1FC and 0x1FB				PRN generator
*/


#include "First_project_header.h"

int main (void)                          //Example 1
  { unsigned int PORT_1;

  setup_HW;
    PORT_1 = 1;
  for (int m = 0; m <= 15; m++)
  {  I2C_Tx_2_integers(PORT_1, PORT_1);
    Timer_T0_10mS_delay_x_m(12);
   PORT_1 = PORT_1 << 1;
   }
  SW_reset;
    }
  
 
/**************************************************************************************************

  Use this area for saving the examples when they have been got working and finished with
  Use the space above for the active program (the one being worked on)


***************Example 1: One bar sweep right to left*********************************************
int main (void)                          //Example 1
  { unsigned int PORT_1;

  setup_HW;
    PORT_1 = 1;
  for (int m = 0; m <= 15; m++)
  {  I2C_Tx_2_integers(PORT_1, PORT_1);
    Timer_T0_10mS_delay_x_m(12);
   PORT_1 = PORT_1 << 1;
   }
  SW_reset;
    }


  ***********************************************************************************************
int main (void)                          //Example 1A
  { unsigned int PORT_1;

  setup_HW;
  wdt_enable(WDTO_30MS);
    PORT_1 = 1;
  for (int m = 0; m <= 15; m++)
  {  I2C_Tx_2_integers(PORT_1, PORT_1);
    Timer_T0_10mS_delay_x_m(12);
   PORT_1 = PORT_1 << 1;
   }
  while(1);
    }

***************Example 2: One bar sweep right to left and back to right again*********************
int main (void)                          //Example 2
  { unsigned int PORT_1;

  setup_HW;
  wdt_enable(WDTO_30MS);
  PORT_1 = 1;
  for (int m = 0; m <= 15; m++)
  {  I2C_Tx_2_integers(PORT_1 << m, PORT_1 << m);
    Timer_T0_10mS_delay_x_m(12);
   }
for (int m = 14; m; m--)
{  I2C_Tx_2_integers(PORT_1 << m, PORT_1 << m);
    Timer_T0_10mS_delay_x_m(12);
   }
  while(1);
  }


  
  ***********************************************************************************************
#include "Local_subroutines.c"

int main (void)                          //Example 2A
  { unsigned int PORT_1;
    int T1_period = 15;

  setup_HW;
  while(1){
  PORT_1 = 1;
  for (int m = 0; m <= 15; m++)
  {  I2C_Tx_2_integers(PORT_1 << m, PORT_1 << m);
    Timer_T0_10mS_delay_x_m(T1_period);}
for (int m = 14; m; m--)
{  I2C_Tx_2_integers(PORT_1 << m, PORT_1 << m);
    Timer_T0_10mS_delay_x_m(T1_period); }
   if(switch_1_down)T1_period = Adjust_T1_period(T1_period);}}




 ***************Example 3: Both half bars running back and forth together**************************
 int main (void)                          //Example 3
  { unsigned int PORT_1, PORT_2;

  setup_HW;
  while (1)
  { PORT_1 = 0b0000000000000001;      //1
    PORT_2 = 0b1000000000000000;     //0x8000;
    for (int m = 0; m <= 15; m++)
    { I2C_Tx_2_integers(PORT_1, PORT_2);
      _delay_ms(60);
      PORT_1 = PORT_1 << 1;
      PORT_2 = PORT_2 >> 1;
    }
  }
  return 1;
  }



***************Example 4: Single half bar runs back and forth**************************************
  int main (void)                          //Example 4
  { unsigned long PORT_1, PORT_2;

  setup_HW;
  while (1)
  { PORT_1 = 1;
    PORT_2 = 0x80000000;
    for (int m = 0; m <= 31; m++)
    {  I2C_Tx_2_integers(PORT_1, PORT_2);
      _delay_ms(60);
      PORT_1 = PORT_1 << 1;
      PORT_2 = PORT_2 >> 1;
    }
  }
  return 1;
  }


*****************Example 5: Four half bars execute two ellipses************************************
 int main (void)                          //Example 5
  { unsigned int PORT_1, PORT_2;

 setup_HW;
  while (1)
  { PORT_1 = 1;
    PORT_2 = 0x8000;
    for (int m = 0; m <= 7; m++)
    { I2C_Tx_2_integers(PORT_1 | PORT_2,
      (PORT_1 << 8)  | (PORT_2 >> 8));
      _delay_ms(60);
      PORT_1 = PORT_1 << 1;
      PORT_2 = PORT_2 >> 1;
    }
  }
  return 1;
  } 






******************Example 6: Two half bars execute see-saw motion *********************************
 int main (void)                          //Example 6
  { unsigned int PORT_1, PORT_2, m = 0, n = 0;

  setup_HW;
  PORT_1 = 1;
  PORT_2 = 0x8000;
  while (1)
  { I2C_Tx_2_integers(PORT_1 << m, PORT_2 >> m);
    _delay_ms(60);
    if (!(n)) m += 1;
    if (m == 16)n = 1;
    if (n == 1)m -= 1;
    if (m == 0)n = 0;
  }
  return 1;
  }




******************Example 7: Multi bar display****************************************************
int main (void)                          //Example 7
  { unsigned int PORT_1 = 1;
  char m = 1;
  char overflow = 0;

  setup_HW;
  while (1)
  { I2C_Tx_2_integers(PORT_1, ~PORT_1);
    
    _delay_ms(80);
    
    if (m <= 5)
    {
      PORT_1 |= (PORT_1 << 1);
      m += 1;
    wdr();}
    else PORT_1 = PORT_1 << 1;
    if (overflow)PORT_1 |= 1;
    if (PORT_1 & 0x8000) overflow = 1;
    else overflow = 0;
  }
  }  

int main (void)                          //Example 7A
  { unsigned int PORT_1 = 1;
  char m = 1;
  char overflow = 0;

  setup_HW;

  wdt_enable(WDTO_30MS);
  while(switch_1_down)wdr();
  
  while (1)
  { I2C_Tx_2_integers(PORT_1, ~PORT_1);
    
    Timer_T0_10mS_delay_x_m(8);
    
    if (m <= 5)
    {
      PORT_1 |= (PORT_1 << 1);
      m += 1;
    wdr();}
    else PORT_1 = PORT_1 << 1;
    if (overflow)PORT_1 |= 1;
    if (PORT_1 & 0x8000) overflow = 1;
    else overflow = 0;
    while(switch_1_down);
  }
  }






*****************Example 8: Leds sweep left to right with random motion***************************
int main (void)                          //Example 8
  { unsigned int random_num;
  unsigned char PRN_counter;
  long PORT_1 = 1, PORT_2 = 1;

  setup_HW;
  wdt_enable(WDTO_30MS);
  PRN_counter = 0;
  random_num = PRN_16bit_GEN (0, &PRN_counter);
  while (1)
  { for (int m = 0; m < random_num % 3; m++)
    { if (!(PORT_1 = ((PORT_1 * 2) % 0x10000)))
        PORT_1 = 1;
    }
    if (!(PORT_2 = ((PORT_2 * 2) % 0x10000)))PORT_2 = 1;
    I2C_Tx_2_integers(PORT_1, PORT_2);
    Timer_T0_10mS_delay_x_m(8);
    random_num = PRN_16bit_GEN (random_num, &PRN_counter);
  }
  }  



*****************Example 9: The default program****************************************************
 
#include "Local_subroutines.c"

unsigned int PRN;
unsigned char PRN_counter;

 int main (void)                             //Example 9
  {int T1_period = 15;
  char direction;
  char seg_counter;
  setup_HW;
  
  I2C_Tx_any_segment_clear_all();
  PRN_counter = 0;
  PRN = PRN_16bit_GEN (0, &PRN_counter);
   direction=1;
  while(1){
     direction ^= 1;
   seg_counter=0;
 PRN = update_display(T1_period, direction, seg_counter, PRN);
 if(switch_1_down); else T1_period = Adjust_T1_period(T1_period);}}



*****************Example 10: Another Random number display*****************************************
int main (void)                          //Example 10
{ unsigned int PRN;
  unsigned char PRN_counter;

  setup_HW;
  wdt_enable(WDTO_30MS);
  PRN_counter = 0;
  PRN = PRN_16bit_GEN (0, &PRN_counter);
  while (1)
  { PRN = PRN_16bit_GEN (PRN, &PRN_counter);
    I2C_Tx_2_integers(PRN, (PRN << ((PRN % 2) + 1)));
    Timer_T0_10mS_delay_x_m(10);
  }
}



  
 **************************************************************************************************/
