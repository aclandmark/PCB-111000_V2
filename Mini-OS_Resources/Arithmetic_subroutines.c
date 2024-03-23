
//void Fraction_to_Binary_D(unsigned long,unsigned long, unsigned int*,unsigned int*);
unsigned long Binary_points_to_Decimal_L (unsigned int, unsigned int);
long Binary_points_to_Decimal_Signed (long);


/**************************************************************************************************/
void Fraction_to_Binary_D(unsigned long rem,unsigned long Den, unsigned int* Res_LB,unsigned int*  Res_HB){		//Positive fraction: Askii to Binary
(*Res_LB) = 0; (*Res_HB) = 0;
for(int n = 0; n <= 15; n++){
if ((2*(rem))/Den) {(*Res_HB) |= (1 << (15-n));}	//sendChar(1 + '0');}else sendChar('0');
(rem) = (2*(rem))%Den;}
for(int n = 0; n <= 15; n++){
if ((2*(rem))/Den) {(*Res_LB) |= (1 << (15-n));} 	//sendChar(1 + '0');}else sendChar('0');
(rem) = (2*(rem))%Den;}}


/**************************************************************************************************/
unsigned long Binary_points_to_Decimal_L (unsigned int RHSofBP_LB, unsigned int RHSofBP_HB)
{unsigned long  RHSofDP;
RHSofDP = 0;
for(int n = 0; n <= 15; n++){switch(n){
case 0: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 500000000; break;
case 1: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 250000000; break;
case 2: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 125000000; break;
case 3: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 62500000; break;
case 4: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 31250000; break;
case 5: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 15625000; break;
case 6: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 7812500; break;
case 7: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 3906250; break;
case 8: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 1953125; break;
case 9: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 976563; break;
case 10: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 488281; break;
case 11: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 244141; break;
case 12: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 122070; break;
case 13: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 61035; break;
case 14: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 30518; break;
case 15: if (RHSofBP_HB & (1 << (15-n)))RHSofDP += 15259; break;}}
for(int n = 0; n <= 15; n++){switch (n){
case 0: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 7629; break;
case 1: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 3815; break;
case 2: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 1907; break;
case 3: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 954; break;
case 4: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 477; break;
case 5: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 238; break;
case 6: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 119; break;
case 7: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 60; break;
case 8: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 30; break;
case 9: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 15; break;
case 10: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 7; break;
case 11: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 4; break;
case 12: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 2; break;
case 13: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 1; break;
case 14: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 0; break;
case 15: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 0; break;}}
RHSofDP += 0x00000008; 	RHSofDP = RHSofDP & 0xFFFFFFF0;  //Round to 8 decimal places	
return RHSofDP;}



/**************************************************************************************************/
long Binary_points_to_Decimal_Signed (long RHSofBP)
{long  RHSofDP=0;
unsigned int RHSofBP_LB;
RHSofBP_LB = RHSofBP;

for(int n = 0; n <= 15; n++){switch(n){						//bit zero is sign bit
case 0: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP -= 1000000000; break;
case 1: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 500000000; break;
case 2: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 250000000; break;
case 3: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 125000000; break;
case 4: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 62500000; break;
case 5: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 31250000; break;
case 6: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 15625000; break;
case 7: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 7812500; break;
case 8: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 3906250; break;
case 9: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 1953125; break;
case 10: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 976563; break;
case 11: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 488281; break;
case 12: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 244141; break;
case 13: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 122070; break;
case 14: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 61035; break;
case 15: if (RHSofBP>>16 & (1 << (15-n)))RHSofDP += 30518; break;}}
for(int n = 0; n <= 15; n++){switch (n){
case 0: if (RHSofBP_LB & (1 << (15-n)))RHSofDP +=  15259; break;
case 1: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 7629; break;
case 2: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 3815; break;
case 3: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 1907; break;
case 4: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 954; break;
case 5: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 477; break;
case 6: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 238; break;
case 7: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 119; break;
case 8: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 60; break;
case 9: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 30; break;
case 10: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 15; break;
case 11: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 7; break;
case 12: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 4; break;
case 13: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 2; break;
case 14: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 1; break;
case 15: if (RHSofBP_LB & (1 << (15-n)))RHSofDP += 0; break;}}	
return RHSofDP;}


