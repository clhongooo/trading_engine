#include "BlackScholes.h"


double BlackScholes::d1(double S, double K, double r, double q, double tau, double V)
{
  return ( (log(S/K) + (r - q + (V*V/2)) * tau) /V /sqrt(tau) );
}
double BlackScholes::d2(double S, double K, double r, double q, double tau, double V)
{
  return ( d1(S,K,r,q,tau,V) - V * sqrt(tau) );
}
double BlackScholes::EuropeanCallPrice(double S, double K, double r, double q, double tau, double V)
{
  double dd1 = d1(S,K,r,q,tau,V);
  double dd2 = d2(S,K,r,q,tau,V);
  return S * pow(M_E, -q*tau) * gsl_cdf_ugaussian_P(dd1) - K * pow(M_E, -r*tau) * gsl_cdf_ugaussian_P(dd2);
}
double BlackScholes::EuropeanPutPrice(double S, double K, double r, double q, double tau, double V)
{
  double dd1 = d1(S,K,r,q,tau,V);
  double dd2 = d2(S,K,r,q,tau,V);
  return K * pow(M_E, -r*tau) * gsl_cdf_ugaussian_P(-dd2) - S * pow(M_E, -q*tau) * gsl_cdf_ugaussian_P(-dd1);
}

double BlackScholes::EuropeanCallPriceFut(double F, double K, double r, double tau, double V)
{
  return EuropeanCallPrice(F*pow(M_E, -r*tau), K, r, 0, tau, V);
}
double BlackScholes::EuropeanPutPriceFut(double F, double K, double r, double tau, double V)
{
  return EuropeanPutPrice(F*pow(M_E, -r*tau), K, r, 0, tau, V);
}

double BlackScholes::ImpldVolatility(bool bIsCall, double S, double K, double r, double q, double tau, double OPrice)
{
  return ImpldVolatility(bIsCall, S, K, r, q, tau, OPrice, 0.5);
}

double BlackScholes::ImpldVolatility(bool bIsCall, double S, double K, double r, double q, double tau, double OPrice, double dIVGuess)
{
  double dIVUB = 2; //upper-bound
  double dIVLB = 0.01; //lower-bound
  // double dPriceUB;
  double dPriceLB;
  double dPriceGuess;

  int iBias = 0;
  bool bFirstTime = true;

  if (bIsCall) dPriceGuess = EuropeanCallPrice(S, K, r, q, tau, dIVGuess);
  else         dPriceGuess = EuropeanPutPrice(S, K, r, q, tau, dIVGuess);

  if (dPriceGuess > OPrice)
  {
    iBias = 1;
    dIVUB = dIVGuess;
    if (bIsCall) dPriceLB = EuropeanCallPrice(S, K, r, q, tau, dIVLB);
    else         dPriceLB = EuropeanPutPrice(S, K, r, q, tau, dIVLB);
  }
  else
  {
    iBias = -1;
    dPriceLB = dPriceGuess;
    dIVLB = dIVGuess;
  }

  while (abs(dIVLB - dIVUB) > NIR_EPSILON)
  {
    double dIVM;
    if (bFirstTime)
    {
      if      (iBias ==  1) dIVM = dIVLB + (dIVUB - dIVLB) * 5 / 6;
      else if (iBias == -1) dIVM = dIVLB + (dIVUB - dIVLB) * 1 / 6;
      else                  dIVM = (dIVLB + dIVUB) / 2;
      bFirstTime = false;
    }
    else
    {
      dIVM = (dIVLB + dIVUB) / 2;
    }

    double dPriceM = (bIsCall ? EuropeanCallPrice(S, K, r, q, tau, dIVM) : EuropeanPutPrice(S, K, r, q, tau, dIVM));

    if (OPrice > dPriceLB && OPrice < dPriceM)
    {
      dIVUB = dIVM;
    }
    else
    {
      dPriceLB = dPriceM;
      dIVLB = dIVM;
    }
  }

  return (dIVUB + dIVLB) / 2;
}


double BlackScholes::ImpldVolatilityFut(bool bIsCall, double F, double K, double r, double tau, double OPrice)
{
  return ImpldVolatilityFut(bIsCall, F, K, r, tau, OPrice, 0.5);
}
double BlackScholes::ImpldVolatilityFut(bool bIsCall, double F, double K, double r, double tau, double OPrice, double dIVGuess)
{
  return ImpldVolatility(bIsCall, F*pow(M_E, -r*tau), K, r, 0, tau, OPrice, dIVGuess);
}

double BlackScholes::CallDelta(double S, double K, double r, double q, double tau, double V)
{
  double dd1 = d1(S,K,r,q,tau,V);
  return pow(M_E, -q*tau) * gsl_cdf_ugaussian_P(dd1);
}
double BlackScholes::PutDelta(double S, double K, double r, double q, double tau, double V)
{
  double dd1 = d1(S,K,r,q,tau,V);
  return -pow(M_E, -q*tau) * gsl_cdf_ugaussian_P(-dd1);
}
double BlackScholes::Gamma(double S, double K, double r, double q, double tau, double V)
{
  double dd1 = d1(S,K,r,q,tau,V);
  return pow(M_E, -q*tau) /S /V /sqrt(tau) /sqrt(2*M_PI) * pow(M_E, -dd1*dd1/2);
}

//returns if the conversion is successful
//and puts the RNPDF into htPDFd
/*
   public static bool GetRNPDF_fromVolSmile(Hashtable htUnifiedIV, double dATMPx, double dT_VS, ref Hashtable htPDFd)
   {
   try
   {
   HistoricalDataHelper hdh = HistoricalDataHelper.Instance;

   Hashtable htPDFatMat1 = new Hashtable();
   Hashtable htCDFatMat = new Hashtable();

   object[] keyUIV = new object[htUnifiedIV.Keys.Count];
   htUnifiedIV.Keys.CopyTo(keyUIV, 0);
   Array.Sort(keyUIV);

   bool bCDFHasReached0 = false;
   for (int i = 0; i < keyUIV.Length - 1; i++) //1 item fewer
   {
   double d2 =
   (Math.Log(dATMPx / (double)(int)keyUIV[i]) +
   (IParam.r - (double)htUnifiedIV[keyUIV[i]] * (double)htUnifiedIV[keyUIV[i]] / 2d) * dT_VS)
   /
   ((double)htUnifiedIV[keyUIV[i]] * Math.Sqrt(dT_VS));
//binary call
double cum_prob = BlackScholes.NORMDIST(d2,0,1,true); // *
//Math.Pow(Math.E, -IParam.r * dT_VS1); //20121112 - need to compute future value, don't discount

if (cum_prob < 0) bCDFHasReached0 = true;

if (bCDFHasReached0) htCDFatMat.Add((int)keyUIV[i], 0d);
else htCDFatMat.Add((int)keyUIV[i], cum_prob);
}

//we assume Pr(S_T > VOLSMILEREGENSTART) = 1
bool bStartToDecrease = false;
double dLastVal = 0;
for (int i = IParam.VOLSMILEREGENSTART; i < IParam.VOLSMILEREGENEND; i++)
{
if (htCDFatMat.ContainsKey(i))
{
if ((double)htCDFatMat[i] < dLastVal) bStartToDecrease = true;
dLastVal = (double)htCDFatMat[i];
}
if (bStartToDecrease)
{
for (int j = i; j >= i-IParam.IMPLRNPDFFATTAILSUBSIDE; j--)
{
if (htCDFatMat.ContainsKey(j))
htCDFatMat[j] = dLastVal +
Math.Min((1-dLastVal)*(double)(i-j)/IParam.IMPLRNPDFFATTAILSUBSIDE, 1);
}
break;
}
}

object[] keyCDF = new object[htCDFatMat.Keys.Count];
htCDFatMat.Keys.CopyTo(keyCDF, 0);
Array.Sort(keyCDF);

for (int i = 0; i < keyCDF.Length - 1; i++) //1 item fewer
{
double dPDFtmp = (double)htCDFatMat[(int)keyCDF[i]] - (double)htCDFatMat[(int)keyCDF[i + 1]];

htPDFatMat1.Add(
(int)(((int)keyCDF[i + 1] + (int)keyCDF[i]) / 2d),
Math.Max(dPDFtmp, 0)
);
}

StatHelper shPDFatMat = new StatHelper(htPDFatMat1.Count);
double dMulConst = 10000;

object[] keyPDF = new object[htPDFatMat1.Keys.Count];
htPDFatMat1.Keys.CopyTo(keyPDF, 0);
Array.Sort(keyPDF);

for (int i = 0; i < keyPDF.Length; i++) //all items
shPDFatMat.Add((int)keyPDF[i],
    (int)((double)htPDFatMat1[(int)keyPDF[i]] * dMulConst));

for (int i = 0; i < keyPDF.Length; i++) //all items
{
  htPDFd.Add(((int)keyPDF[i] - shPDFatMat.AMean) /
      Math.Sqrt(hdh.GetCalendarNumDays(dT_VS) / IParam.SCENARIOENDDAY) +
      dATMPx +
      (shPDFatMat.AMean - dATMPx) / (hdh.GetCalendarNumDays(dT_VS) / IParam.SCENARIOENDDAY),
      (double)htPDFatMat1[(int)keyPDF[i]]
      );
}
}
catch (Exception e)
{
  Console.WriteLine("RNPDF could not be calculated from volatility smile.");
  Console.WriteLine(e.Message);
  return false;
}
return true;
}

//returns if the conversion is successful
//and puts the volatility smile into htScVolSmile
public static bool GetVolSmile_fromRNPDF(Hashtable htRNPDF, double dT, double dATM, double dEstATMVol, ref Hashtable htScVolSmile)
{
  //we don't know the location of the ATM price in the RNPDF yet, it's still off by an unknown amount
  //we want to fix the volatility level at the ATM price as well, so that the resulting smile hsa the desired shape
  double dATM_d2 = (IParam.r / (dEstATMVol/100) - (dEstATMVol/100) / 2) * Math.Sqrt(dT);
  double dATM_Nd2 = NORMDIST(dATM_d2, 0, 1, true);

  object[] keyPDF = new object[htRNPDF.Keys.Count];
  htRNPDF.Keys.CopyTo(keyPDF, 0);
  Array.Sort(keyPDF);

  //now find the ATM location in the RNPDF
  double dCumProbAbvK = 0;
  int iATMKLocRNPDF = 0;
  double dATMKRNPDF = 0;
  for (int i = keyPDF.Length - 1; i >= 0; i--)
  {
    dCumProbAbvK += (double)htRNPDF[keyPDF[i]];
    if (dCumProbAbvK >= dATM_Nd2)
    {
      iATMKLocRNPDF = i;
      dATMKRNPDF = (double)keyPDF[i];
      break;
    }
  }


  dCumProbAbvK = 0;

  for (int i = keyPDF.Length - 1; i >= 0 ; i--)
  {
    dCumProbAbvK += (double)htRNPDF[keyPDF[i]];
    double dAdjRNPDFK = (double)keyPDF[i] + (dATM - dATMKRNPDF);

    double dLower = -10;
    double dUpper = 10;
    double dTriald2 = 0;
    double dTrialCumProb = NORMDIST(dTriald2,0,1,true);
    while (Math.Abs(dCumProbAbvK - dTrialCumProb) > NIR_EPSILON)
    {
      if (dTrialCumProb > dCumProbAbvK)
      {
        dUpper = dTriald2;
        dTriald2 = (dUpper + dLower) / 2;
      }
      else if (dTrialCumProb < dCumProbAbvK)
      {
        dLower = dTriald2;
        dTriald2 = (dUpper + dLower) / 2;
      }
      dTrialCumProb = NORMDIST(dTriald2,0,1,true);
    }

    //now we have d2, must get the corresponding sigma
    double d2 = dTriald2;
    double dTmpTerm = d2*d2/dT + 2/dT*(Math.Log(dATM/dAdjRNPDFK)+IParam.r*dT);
    double dSigma1 = -d2 / Math.Sqrt(dT) + Math.Sqrt(dTmpTerm);
    double dSigma2 = -d2 / Math.Sqrt(dT) - Math.Sqrt(dTmpTerm);
    double dSigma = 0;

    if (dSigma1 < 0 && dSigma2 < 0) continue;
    if (dSigma1 > 2 && dSigma2 > 2) continue;
    if (dSigma1 > 0 && dSigma1 < 2 && (dSigma2 < 0 || dSigma2 > 2)) dSigma = dSigma1;
    if (dSigma2 > 0 && dSigma2 < 2 && (dSigma1 < 0 || dSigma1 > 2)) dSigma = dSigma2;

    htScVolSmile.Add(dAdjRNPDFK, dSigma);


  }
  return true;
}
*/
