float logE_power_series(float Num)
{float logE, logE_old;                                  //logs are calculated iteratively
float term;                                             //Power series terms
float difference;                                       //difference berween consequtive terms

if (FPN_GT(Num, 1.99))return 0.693;
Num = FPN_sub (Num, 1.0);
if (Num < 0.00001){logE = 1E-5;}
else                                                    //Use power series to calculate the natural logarithm
{
int m = 1;
term = 1.0;
while(1){wdr();
term = FPN_mult(term, FPN_div(Num, (float)m));

if (m == 1){logE = term; difference= logE;}
else{
  if (m%2)logE = FPN_add (logE, term);
  else logE = FPN_sub(logE, term); 
  difference = FPN_sub(logE, logE_old);
  }

if((FPN_GT(FPN_div(difference, logE), -1e-5)) 
&& (FPN_LT(FPN_div(difference, logE), 1e-5)))break;
 if (!(m%20))Serial.write('.'); 
 
logE_old = logE;
term = FPN_mult(term, (float)m);
if ((m += 1) > 500)break; }}
return logE;}
