/*
 * Programmer: Sunny Yan
 * DateTime: 23:19 11/05/2012
 */

using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;


public class RandomEngine
{
    private Random rand = null;

    public RandomEngine()
    {
        rand = new Random(unchecked((int)(DateTime.Now.Ticks)));
    }
    //iL inclusive
    //iU exclusive
    public int Next(int iL, int iU)
    {
        return rand.Next(iL, iU);
    }
}

//n1+n2+n3+...+n13 = 13n1 + (1+12)(12)/2 h
public class CalcAvgIntervalHeight
{
    int iCount = 0;
    double dSmallest = 0;
    double dSumScPx = 0;

    public void Add(double dA)
    {
        if (dSmallest == 0) dSmallest = dA;
        if (dA < dSmallest) dSmallest = dA;
        dSumScPx += dA;
        iCount++;
    }
    public double GetAvgIntervalHeight()
    {
        return (dSumScPx - iCount * dSmallest) * 2 / (double)iCount / (double)(iCount - 1);
    }
}


public class SmaAlt
{
    private bool bStable = false;
    private double[] daX = null;
    private double[] daSma = null;
    private int iPtr = 0;
    private double dRunningTotal = 0;

    public void Reset(int iS)
    {
        iPtr = 0;
        dRunningTotal = 0;
        bStable = false;
        daX = new double[iS];
        daSma = new double[iS];
        for (int i = 0; i < daX.Length; i++)
        {
            daX[i] = 0;
            daSma[i] = 0;
        }
    }
    public SmaAlt(int iS)
    {
        Reset(iS);
    }
    private int BChk(int i)
    {
        if (i >= daX.Length) return BChk(i - daX.Length);
        else if (i < 0) return BChk(i + daX.Length);
        else return i;
    }
    public void Add(double dP)
    {
        if (double.IsNaN(dP) || dP < 0.00001) return;
        dRunningTotal -= daX[iPtr];
        dRunningTotal += dP;
        daX[iPtr] = dP;
        iPtr = BChk(iPtr + 1);
        if (iPtr == 0) bStable = true;
    }
    public bool Stable { get { return bStable; } }
    public double Previous
    {
        get
        {
            double dPrev = daSma[BChk(iPtr - 2)];
            if (Math.Abs(dPrev) > 0.00001) return dPrev;
            else return double.NaN;
        }
    }
    public double SMA
    {
        get
        {
            double dSma = double.NaN;
            if (Stable) dSma = dRunningTotal / daX.Length;
            else dSma = double.NaN;
            daSma[BChk(iPtr - 1)] = dSma;
            return dSma;
        }
    }
    public static void UnitTest()
    {
        double dSMA = 0;
        SmaAlt s = new SmaAlt(5);
        s.Add(5); dSMA = s.SMA;
        s.Add(2); dSMA = s.SMA;
        s.Add(9); dSMA = s.SMA;
        s.Add(12); dSMA = s.SMA;
        s.Add(13); dSMA = s.SMA;
        s.Add(4); dSMA = s.SMA;
        s.Add(8); dSMA = s.SMA;
        s.Add(7); dSMA = s.SMA;
        s.Add(6); dSMA = s.SMA;
        s.Add(19); dSMA = s.SMA;
        s.Add(16); dSMA = s.SMA;
        s.Add(1); dSMA = s.SMA;
        s.Add(14); dSMA = s.SMA;
    }
}


public class WrAlt
{
    private double[] daWr = null;
    private HighLowHelper hlh = null;
    private double dClose = double.NaN;
    private int iPtr = 0;

    public void Reset(int iS)
    {
        daWr = new double[iS];
        hlh = new HighLowHelper(iS);
        dClose = double.NaN;
        iPtr = 0;
        for (int i = 0; i < iS; i++) daWr[i] = 0;
    }
    public WrAlt(int iS)
    {
        Reset(iS);
    }
    private int BChk(int i)
    {
        if (i >= daWr.Length) return BChk(i - daWr.Length);
        else if (i < 0) return BChk(i + daWr.Length);
        else return i;
    }
    public void Add(double dO, double dH, double dL, double dC, double dV)
    {
        hlh.Add(dH, dL);
        dClose = dC;
        iPtr = BChk(iPtr + 1);
    }
    public bool Stable { get { return hlh.Stable; } }
    public double Previous
    {
        get
        {
            double dPrev = daWr[BChk(iPtr - 2)];
            if (Math.Abs(dPrev) > 0.00001) return dPrev;
            else return double.NaN;
        }
    }
    public double WR
    {
        get
        {
            double dWr = double.NaN;
            if (Stable) dWr = (hlh.HighestHigh - dClose) / (hlh.HighestHigh - hlh.LowestLow) * -100;
            else dWr = double.NaN;
            daWr[BChk(iPtr - 1)] = dWr;
            return dWr;
        }
    }
    public static void UnitTest()
    {
        //0.666666667
        //0.916666667
        //0.545454545
        //0.727272727
        //0.727272727
        //0.117647059
        //0.222222222
        //0.833333333
        //0.722222222

        double dWr = 0;
        WrAlt w = new WrAlt(5);
        w.Add(0, 5, 3, 4, 0); dWr = w.WR;
        w.Add(0, 2, 1, 2, 0); dWr = w.WR;
        w.Add(0, 9, 3, 6, 0); dWr = w.WR;
        w.Add(0, 12, 4, 12, 0); dWr = w.WR;
        w.Add(0, 13, 3, 5, 0); dWr = w.WR;
        w.Add(0, 4, 2, 2, 0); dWr = w.WR;
        w.Add(0, 8, 5, 7, 0); dWr = w.WR;
        w.Add(0, 7, 5, 5, 0); dWr = w.WR;
        w.Add(0, 6, 2, 5, 0); dWr = w.WR;
        w.Add(0, 19, 8, 17, 0); dWr = w.WR;
        w.Add(0, 16, 1, 15, 0); dWr = w.WR;
        w.Add(0, 5, 3, 4, 0); dWr = w.WR;
        w.Add(0, 14, 5, 6, 0); dWr = w.WR;
    }
}

public class BlackScholesAlt
{
    public static double GetCallValue(double Spot, double Strike, double rf, double TTM, double Volatility)
    {
        double a = Math.Log(Spot / Strike);
        double b_call = (rf + 0.5 * Math.Pow(Volatility, 2)) * TTM;
        double b_put = (rf - 0.5 * Math.Pow(Volatility, 2)) * TTM;
        double c = Volatility * Math.Sqrt(TTM);
        double d1 = (a + b_call) / c;
        double d2 = (a + b_put) / c;
        return Spot * NORMDIST(d1, 0, 1, true) - Strike * Math.Exp(-rf * TTM) * NORMDIST(d2, 0, 1, true);
    }
    public static double GetPutValue(double Spot, double Strike, double rf, double TTM, double Volatility)
    {
        double a = Math.Log(Spot / Strike);
        double b_call = (rf + 0.5 * Math.Pow(Volatility, 2)) * TTM;
        double b_put = (rf - 0.5 * Math.Pow(Volatility, 2)) * TTM;
        double c = Volatility * Math.Sqrt(TTM);
        double d1 = (a + b_call) / c;
        double d2 = (a + b_put) / c;
        return Strike * Math.Exp(-rf * TTM) * NORMDIST(-d2, 0, 1, true) - Spot * NORMDIST(-d1, 0, 1, true);
    }
    public static double CalcImpliedVolatility(bool bIsCall, double Spot, double Strike, double rf, double TTM, double Price)
    {
        double dIVUB = 1.2; //upper-bound
        double dIVLB = 0.1; //lower-bound
        double dPriceUB = double.NaN;
        double dPriceLB = double.NaN;

        if (bIsCall)
        {
            dPriceUB = GetCallValue(Spot, Strike, rf, TTM, dIVUB);
            dPriceLB = GetCallValue(Spot, Strike, rf, TTM, dIVLB);
        }
        else
        {
            dPriceUB = GetPutValue(Spot, Strike, rf, TTM, dIVUB);
            dPriceLB = GetPutValue(Spot, Strike, rf, TTM, dIVLB);
        }

        for (int i = 0; i < 10; i++)
        {
            double dIVM = (dIVUB + dIVLB) / 2;
            double dPriceM = (bIsCall ? GetCallValue(Spot, Strike, rf, TTM, dIVM) :
                                        GetPutValue(Spot, Strike, rf, TTM, dIVM));

            if (Price > dPriceLB && Price < dPriceM)
            {
                dPriceUB = dPriceM;
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
    public static double NORMDIST(double x, double mean, double std, bool cumulative)
    {
        if (cumulative)
        {
            return Phi(x, mean, std);
        }
        else
        {
            double tmp = 1 / ((Math.Sqrt(2 * Math.PI) * std));
            return tmp * Math.Exp(-.5 * Math.Pow((x - mean) / std, 2));
        }
    }
    //from h ttp://www.cs.princeton.edu/introcs/...Math.java.html
    // fractional error less than 1.2 * 10 ^ -7.
    static double erf(double z)
    {
        double t = 1.0 / (1.0 + 0.5 * Math.Abs(z));

        // use Horner's method
        double ans = 1 - t * Math.Exp(-z * z - 1.26551223 +
        t * (1.00002368 +
        t * (0.37409196 +
        t * (0.09678418 +
        t * (-0.18628806 +
        t * (0.27886807 +
        t * (-1.13520398 +
        t * (1.48851587 +
        t * (-0.82215223 +
        t * (0.17087277))))))))));
        if (z >= 0) return ans;
        else return -ans;
    }

    // cumulative normal distribution
    static double Phi(double z)
    {
        return 0.5 * (1.0 + erf(z / (Math.Sqrt(2.0))));
    }

    // cumulative normal distribution with mean mu and std deviation sigma
    static double Phi(double z, double mu, double sigma)
    {
        return Phi((z - mu) / sigma);
    }

    public static double GetCallDelta(double Spot, double Strike, double rf, double TTM, double Volatility)
    {
        //N(d1)
        double d1 = (Math.Log(Spot / Strike) + (rf + Volatility / 2) * TTM) / Math.Sqrt(Volatility) / Math.Sqrt(TTM);
        return Phi(d1);
    }
    public static double GetPutDelta(double Spot, double Strike, double rf, double TTM, double Volatility)
    {
        //N(d1)-1
        double d1 = (Math.Log(Spot / Strike) + (rf + Volatility / 2) * TTM) / Math.Sqrt(Volatility) / Math.Sqrt(TTM);
        return Phi(d1) - 1;
    }
    public static double GetGamma(double Spot, double Strike, double rf, double TTM, double Volatility)
    {
        double d1 = (Math.Log(Spot / Strike) + (rf + Volatility / 2) * TTM) / Math.Sqrt(Volatility) / Math.Sqrt(TTM);
        return Phi(d1) / Spot / Volatility / Math.Sqrt(TTM);
    }

    //returns if the conversion is successful
    //and puts the RNPDF into htPDFd
    public static bool GetRNPDF_fromVolSmile(Hashtable htUnifiedIV, double dATMPx, double dTTM_VS, ref Hashtable htPDFd)
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
                    (IParam.rf - (double)htUnifiedIV[keyUIV[i]] * (double)htUnifiedIV[keyUIV[i]] / 2d) * dTTM_VS)
                    /
                    ((double)htUnifiedIV[keyUIV[i]] * Math.Sqrt(dTTM_VS));
                //binary call
                double cum_prob = BlackScholesAlt.NORMDIST(d2,0,1,true); // *
                //Math.Pow(Math.E, -IParam.rf * dTTM_VS1); //20121112 - need to compute future value, don't discount

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
                              Math.Sqrt(hdh.GetCalendarNumDays(dTTM_VS) / IParam.SCENARIOENDDAY) +
                              dATMPx +
                              (shPDFatMat.AMean - dATMPx) / (hdh.GetCalendarNumDays(dTTM_VS) / IParam.SCENARIOENDDAY),
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
    public static bool GetVolSmile_fromRNPDF(Hashtable htRNPDF, double dTTM, double dATM, double dEstATMVol, ref Hashtable htScVolSmile)
    {
        //we don't know the location of the ATM price in the RNPDF yet, it's still off by an unknown amount
        //we want to fix the volatility level at the ATM price as well, so that the resulting smile hsa the desired shape
        double dATM_d2 = (IParam.rf / (dEstATMVol/100) - (dEstATMVol/100) / 2) * Math.Sqrt(dTTM);
        double dATM_Nd2 = NORMDIST(dATM_d2, 0, 1, true);

        object[] keyPDF = new object[htRNPDF.Keys.Count];
        htRNPDF.Keys.CopyTo(keyPDF, 0);
        Array.Sort(keyPDF);

        //now find the ATM location in the RNPDF
        double dCumProbAbvK = 0;
        int iATMStrikeLocRNPDF = 0;
        double dATMStrikeRNPDF = 0;
        for (int i = keyPDF.Length - 1; i >= 0; i--)
        {
            dCumProbAbvK += (double)htRNPDF[keyPDF[i]];
            if (dCumProbAbvK >= dATM_Nd2)
            {
                iATMStrikeLocRNPDF = i;
                dATMStrikeRNPDF = (double)keyPDF[i];
                break;
            }
        }


        dCumProbAbvK = 0;

        for (int i = keyPDF.Length - 1; i >= 0 ; i--)
        {
            dCumProbAbvK += (double)htRNPDF[keyPDF[i]];
            double dAdjRNPDFStrike = (double)keyPDF[i] + (dATM - dATMStrikeRNPDF);

            double dLower = -10;
            double dUpper = 10;
            double dTriald2 = 0;
            double dTrialCumProb = NORMDIST(dTriald2,0,1,true);
            while (Math.Abs(dCumProbAbvK - dTrialCumProb) > 0.0000001)
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
            double dTmpTerm = d2*d2/dTTM + 2/dTTM*(Math.Log(dATM/dAdjRNPDFStrike)+IParam.rf*dTTM);
            double dSigma1 = -d2 / Math.Sqrt(dTTM) + Math.Sqrt(dTmpTerm);
            double dSigma2 = -d2 / Math.Sqrt(dTTM) - Math.Sqrt(dTmpTerm);
            double dSigma = 0;

            if (dSigma1 < 0 && dSigma2 < 0) continue;
            if (dSigma1 > 2 && dSigma2 > 2) continue;
            if (dSigma1 > 0 && dSigma1 < 2 && (dSigma2 < 0 || dSigma2 > 2)) dSigma = dSigma1;
            if (dSigma2 > 0 && dSigma2 < 2 && (dSigma1 < 0 || dSigma1 > 2)) dSigma = dSigma2;

            htScVolSmile.Add(dAdjRNPDFStrike, dSigma);


        }
        return true;
    }
}


public class DelayFunction
{
    double[] dX = null;
    int iPtr = 0;
    int iDelay = 0;
    int iSize = 0;

    public DelayFunction(int iD)
    {
        iDelay = iD;
        iSize = iDelay + 1;
        dX = new double[iSize];
    }
    public void Add(double dA)
    {
        dX[iPtr] = dA;
        iPtr = (iPtr + 1 + iSize) % iSize;
    }
    public double Value
    {
        get
        {
            return dX[iPtr];
        }
    }
}


//public class MarkovModel
//{
//    int iBarSizeMultiple; //No. of times between the logical bar size and the actual bar size
//    int iPredictNLogicBars; //Predict the price of the N'th logical bars
//    int iHistSize; //iPredictNLogicBars*iBarSizeMultiple+1
//    int iPredictSize; //iPredictNLogicBars*iBarSizeMultiple
//    double dPMFDeltaAtr; //The size of 1 step in the unit of ATR on the x-axis of the PMF
//    int iNumOfRegions; //No. of steps on the x-axis of the PMF

//    Sma sma1 = null;
//    Sma sma2 = null;
//    Sma sma3 = null;
//    Sma sma4 = null;
//    StatHelper sh = null;
//    Wr wr1 = null;
//    Wr wr2 = null;
//    Atr atr = null;

//    double[] daPxHist = null;
//    int[] iaStateHist = null;
//    int[,] iaStatePMFCnt = null;
//    int iPxHistStatePtr = 0;

//    double[,] daOfficialStateCnt = null;
//    double[,] daOfficialStatePMF = null;
//    double[,] daOfficialStateCMF = null;
//    StatHelper[] shStateStat = null;

//    public StatHelper[] StateStat
//    {
//        get
//        {
//            return shStateStat;
//        }
//    }
//    public int TimeSpanMul
//    {
//        get
//        {
//            return iPredictNLogicBars * iBarSizeMultiple;
//        }
//    }
//    public MarkovModel(int iBSM, int iPNLB, double dPMFD, int iNR)
//    {
//        iBarSizeMultiple = iBSM;
//        iPredictNLogicBars = iPNLB;
//        iHistSize = iPredictNLogicBars * iBarSizeMultiple + 1;
//        iPredictSize = iPredictNLogicBars * iBarSizeMultiple;
//        dPMFDeltaAtr = dPMFD;
//        iNumOfRegions = iNR;

//        sma1 = new Sma(iBarSizeMultiple * 5); //1 bit
//        sma2 = new Sma(iBarSizeMultiple * 10); //1 bit
//        sma3 = new Sma(iBarSizeMultiple * 20); //1 bit
//        sma4 = new Sma(iBarSizeMultiple * 50); //1 bit
//        sh = new StatHelper(iBarSizeMultiple * 5); // 3 bit
//        wr1 = new Wr(iBarSizeMultiple * 5); // 2 bit
//        wr2 = new Wr(iBarSizeMultiple * 10); // 2 bit
//        atr = new Atr(iBarSizeMultiple * 20);

//        daPxHist = new double[iHistSize];
//        iaStateHist = new int[iHistSize];
//        iaStatePMFCnt = new int[2048, iNumOfRegions]; //2^11
//        iPxHistStatePtr = 0;


//        daOfficialStateCnt = new double[2048, iNumOfRegions];
//        daOfficialStatePMF = new double[2048, iNumOfRegions];
//        daOfficialStateCMF = new double[2048, iNumOfRegions];
//        shStateStat = new StatHelper[2048];
//        for (int i = 0; i < 2048; i++)
//            shStateStat[i] = new StatHelper(iNumOfRegions);


//    }
//    public void Add(double dO, double dH, double dL, double dC)
//    {
//        sma1.Add((dH + dL + dC) / 3);
//        sma2.Add((dH + dL + dC) / 3);
//        sma3.Add((dH + dL + dC) / 3);
//        sma4.Add((dH + dL + dC) / 3);
//        sh.Add((dH + dL + dC) / 3);
//        wr1.Add(dO, dH, dL, dC, 0);
//        wr2.Add(dO, dH, dL, dC, 0);
//        atr.Add(dO, dH, dL, dC, 0);

//        daPxHist[iPxHistStatePtr] = (dH + dL + dC) / 3;
//        if (Stable) iaStateHist[iPxHistStatePtr] = GetPresentState();
//        iPxHistStatePtr = (iPxHistStatePtr + 1) % iHistSize;
//    }
//    public bool Stable
//    {
//        get
//        {
//            if (!sma1.Stable || !sma2.Stable || !sma3.Stable || !sma4.Stable ||
//                !sh.Stable || !wr1.Stable || !wr2.Stable || !atr.Stable) return false;
//            else return true;
//        }
//    }
//    public int StateId
//    {
//        get
//        {
//            if (Stable)
//                return iaStateHist[(iPxHistStatePtr - 1 + iHistSize) % iHistSize];
//            else return -1;
//        }
//    }
//    public int GetPresentState()
//    {
//        int iStateId = 0;
//        if (!Stable) return -1;
//        double dPx = daPxHist[(iPxHistStatePtr - 1 + iHistSize) % iHistSize];

//        //2^11 = 2048 states
//        if (dPx > sma1.SMA) iStateId += Convert.ToInt32("00000000001", 2);
//        if (dPx > sma2.SMA) iStateId += Convert.ToInt32("00000000010", 2);
//        if (dPx > sma3.SMA) iStateId += Convert.ToInt32("00000000100", 2);
//        if (dPx > sma4.SMA) iStateId += Convert.ToInt32("00000001000", 2);

//        if (dPx > sh.AMean + 2 * sh.Stdev) iStateId += Convert.ToInt32("00000010000", 2);
//        else if (dPx > sh.AMean + 1 * sh.Stdev) iStateId += Convert.ToInt32("00000100000", 2);
//        else if (dPx > sh.AMean + 0 * sh.Stdev) iStateId += Convert.ToInt32("00000110000", 2);
//        else if (dPx > sh.AMean - 1 * sh.Stdev) iStateId += Convert.ToInt32("00001000000", 2);
//        else if (dPx > sh.AMean - 2 * sh.Stdev) iStateId += Convert.ToInt32("00001010000", 2);
//        else iStateId += Convert.ToInt32("00001100000", 2);

//        if (wr1.WR > -0.25) iStateId += Convert.ToInt32("00000000000", 2);
//        else if (wr1.WR > -0.5) iStateId += Convert.ToInt32("00010000000", 2);
//        else if (wr1.WR > -0.75) iStateId += Convert.ToInt32("00100000000", 2);
//        else iStateId += Convert.ToInt32("00110000000", 2);
//        if (wr2.WR > -0.25) iStateId += Convert.ToInt32("00000000000", 2);
//        else if (wr2.WR > -0.5) iStateId += Convert.ToInt32("01000000000", 2);
//        else if (wr2.WR > -0.75) iStateId += Convert.ToInt32("10000000000", 2);
//        else iStateId += Convert.ToInt32("11000000000", 2);

//        return iStateId;
//    }
//    public void CollectStat()
//    {
//        //Check if we have sufficiently warmed up
//        if (!Stable) return;
//        int iOriPtr = (iPxHistStatePtr - iPredictSize + iHistSize) % iHistSize;
//        if (iaStateHist[iOriPtr] == 0 || daPxHist[iOriPtr] == 0) return;

//        int iCurPtr = (iPxHistStatePtr - 1 + iHistSize) % iHistSize;
//        double dPxChg = daPxHist[iCurPtr] - daPxHist[iOriPtr];
//        double dAtrMvm = dPxChg / atr.ATR;

//        bool bBucketFilled = false;
//        for (int i = 0; i < iNumOfRegions; i++)
//        {
//            if (dAtrMvm < (-dPMFDeltaAtr * (iNumOfRegions / 2 - 1) + dPMFDeltaAtr * i) && !bBucketFilled)
//            {
//                iaStatePMFCnt[iaStateHist[iOriPtr], i]++;
//                bBucketFilled = true;
//            }
//        }
//        if (!bBucketFilled) iaStatePMFCnt[iaStateHist[iOriPtr], iNumOfRegions - 1]++;

//        return;
//    }
//    public void OutputStat(string sFileName)
//    {
//        StreamWriter sw = new StreamWriter(sFileName);
//        for (int i = 0; i < 2048; i++)
//        {
//            string s;
//            s = i.ToString();
//            s += "\t";
//            for (int j = 0; j < iNumOfRegions; j++)
//            {
//                s += iaStatePMFCnt[i, j];
//                if (j != iNumOfRegions - 1) s += "\t";
//            }
//            sw.WriteLine(s);
//        }
//        sw.Close;
//    }
//    public void InputStat(string sFileName)
//    {
//        using (StreamReader sr = new StreamReader(sFileName))
//        {
//            string sLine;
//            while ((sLine = sr.ReadLine()) != null)
//            {
//                string[] sFields = sLine.Split('\t');
//                for (int i = 1; i < sFields.Length; i++)
//                {
//                    daOfficialStateCnt[Convert.ToInt32(sFields[0]), i - 1] = Convert.ToDouble(sFields[i]);
//                }
//            }
//        }
//        for (int i = 0; i < 2048; i++)
//        {
//            daOfficialStatePMF[i, 0] = daOfficialStateCnt[i, 0];
//            daOfficialStateCMF[i, 0] = daOfficialStateCnt[i, 0];
//            double dTotalCnt = daOfficialStateCnt[i, 0];
//            for (int j = 1; j < iNumOfRegions; j++)
//            {
//                daOfficialStateCMF[i, j] = daOfficialStateCMF[i, j - 1] + daOfficialStateCnt[i, j];
//                daOfficialStatePMF[i, j] = daOfficialStateCnt[i, j];
//                dTotalCnt += daOfficialStateCnt[i, j];
//            }
//            for (int j = 0; j < iNumOfRegions; j++)
//            {
//                daOfficialStatePMF[i, j] /= dTotalCnt;
//                daOfficialStateCMF[i, j] /= dTotalCnt;
//            }
//        }
//        for (int i = 0; i < 2048; i++)
//        {
//            for (int j = 0; j < iNumOfRegions; j++)
//                shStateStat[i].Add(-dPMFDeltaAtr * (iNumOfRegions / 2 - 0.5 - j),
//                                    (long)daOfficialStateCnt[i, j]);
//        }

//        return;
//    }
//    //	public double CalcOptimalBetSize(int iStateId)
//    //	{
//    //		if (iStateId == -1) return double.NaN;
//    //		
//    //		double[] daDMDA = new double[20]; //d m / d alpha
//    //		double a_optimal = 0;
//    //
//    //		for (int a=-10; a<10; a++)
//    //		{
//    //			double dTotalCnt = 0;
//    //			double dmda = 0;
//    //			for (int j=0; j<iNumOfRegions; j++)
//    //			{
//    //				dTotalCnt += daOfficialStateCnt[iStateId,j];
//    //				double delta = dPMFDeltaAtr * atr.ATR / daPxHist[(iPxHistStatePtr-1+iHistSize)%iHistSize];
//    //				dmda += daOfficialStateCnt[iStateId,j] * (j+1-iNumOfRegions/2) * delta / (1 + a*10000*(j+1-iNumOfRegions/2)*delta);
//    //			}
//    //			dmda = dmda / dTotalCnt; //to change from count to probability
//    //			daDMDA[a+10] = dmda;
//    //		}
//    //		double dSmallestAbs = Math.Abs(daDMDA[0]);
//    //        a_optimal = -10;
//    //		for (int a=-9; a<10; a++)
//    //		{
//    //			if (dSmallestAbs > Math.Abs(daDMDA[0]))
//    //			{
//    //				dSmallestAbs = Math.Abs(daDMDA[0]);
//    //				a_optimal = a+10;
//    //			}
//    //		}
//    //		return a_optimal;
//    //	}
//}




public class DateTimeHelper
{
    //output format
    public static string convertDateTimeToYYYYMMDD(DateTime dt, string sDelim)
    {
        string sY = dt.Year.ToString();
        string sM = dt.Month.ToString();
        if (sM.Trim().Length == 1) sM = "0" + sM;
        string sD = dt.Date.ToString();
        if (sD.Trim().Length == 1) sD = "0" + sD;
        return (sY + sDelim + sM + sDelim + sD);

    }

    public DateTimeHelper()
    {
    }
}

public class MarketFlow
{
    int iFlow = 0; //1=Up, 0=Neutral, -1=Down
    public int Flow { get { return iFlow; } }

    //latest synthetic bar
    HighLowHelper hlhLatestSynBar = null;
    int iRemainSynBarLife = 0;
    int iBarAggregation = 0; //if BA 240, then M1 to H4

    //last 5 synthetic bars
    double[] daSynBarHighs = new double[5];
    double[] daSynBarLows = new double[5];
    int iSynBarPtr = 0;
    bool bRoundTrip = false;

    //fractals
    ArrayList alUpFractalPx = new ArrayList();
    ArrayList alDownFractalPx = new ArrayList();

    public double UpFractal
    {
        get
        {
            if (alUpFractalPx.Count > 0)
                return (double)alUpFractalPx[alUpFractalPx.Count - 1];
            else return double.NaN;
        }
    }
    public double DownFractal
    {
        get
        {
            if (alDownFractalPx.Count > 0)
                return (double)alDownFractalPx[alDownFractalPx.Count - 1];
            else return double.NaN;
        }
    }

    public MarketFlow(int iBA)
    {
        iBarAggregation = iBA;
        hlhLatestSynBar = new HighLowHelper(iBarAggregation);
        iRemainSynBarLife = iBarAggregation;
        iSynBarPtr = 0;
        iFlow = 0;
    }
    public void Add(double dH, double dL)
    {
        if (iRemainSynBarLife > 0)
        {
            hlhLatestSynBar.Add(dH, dL);
            iRemainSynBarLife--;

            if (alUpFractalPx.Count > 0 && alDownFractalPx.Count > 0)
            {
                if (iFlow != 1 && dH > (double)alUpFractalPx[alUpFractalPx.Count - 1])
                {
                    iFlow = 1;
                }
                else if (iFlow != -1 && dL < (double)alDownFractalPx[alDownFractalPx.Count - 1])
                {
                    iFlow = -1;
                }
            }
        }
        else
        {
            daSynBarHighs[iSynBarPtr] = hlhLatestSynBar.HighestHigh;
            daSynBarLows[iSynBarPtr] = hlhLatestSynBar.LowestLow;

            if (bRoundTrip &&
                daSynBarHighs[(iSynBarPtr - 2 + 5) % 5] > daSynBarHighs[(iSynBarPtr - 4 + 5) % 5] &&
                daSynBarHighs[(iSynBarPtr - 2 + 5) % 5] > daSynBarHighs[(iSynBarPtr - 3 + 5) % 5] &&
                daSynBarHighs[(iSynBarPtr - 2 + 5) % 5] > daSynBarHighs[(iSynBarPtr - 1 + 5) % 5] &&
                daSynBarHighs[(iSynBarPtr - 2 + 5) % 5] > daSynBarHighs[(iSynBarPtr - 0 + 5) % 5])
            {
                alUpFractalPx.Add(daSynBarHighs[(iSynBarPtr - 2 + 5) % 5]);
            }
            else if (bRoundTrip &&
                        daSynBarLows[(iSynBarPtr - 2 + 5) % 5] < daSynBarLows[(iSynBarPtr - 4 + 5) % 5] &&
                        daSynBarLows[(iSynBarPtr - 2 + 5) % 5] < daSynBarLows[(iSynBarPtr - 3 + 5) % 5] &&
                        daSynBarLows[(iSynBarPtr - 2 + 5) % 5] < daSynBarLows[(iSynBarPtr - 1 + 5) % 5] &&
                        daSynBarLows[(iSynBarPtr - 2 + 5) % 5] < daSynBarLows[(iSynBarPtr - 0 + 5) % 5])
            {
                alDownFractalPx.Add(daSynBarLows[(iSynBarPtr - 2 + 5) % 5]);
            }

            if (iSynBarPtr == 4) bRoundTrip = true;
            iSynBarPtr = (iSynBarPtr + 1) % 5;
            iRemainSynBarLife = iBarAggregation;
        }
    }
}
public enum FibDir { FROMHIGH, FROMLOW };
public enum FibLevel { F0, F236, F382, F50, F618, F764, F100 };
public class FibonacciHelper
{
    private static double[] daFibLevel = { 0d, 0.236068d, 0.381966d, 0.5d, 0.618034d, 0.763932d, 1d };

    HighLowHelper hlh = null;
    int iCapacity = 0;
    double dThreshold = double.NaN;

    public FibonacciHelper(int iC, double dT)
    {
        iCapacity = iC;
        dThreshold = dT;
        hlh = new HighLowHelper(iCapacity);
    }
    public void Add(double dH, double dL)
    {
        hlh.Add(dH, dL);
    }
    public bool AtFibRetracement(double dP, FibLevel fiblev, FibDir dir)
    {
        double dFibRetrace = double.NaN;
        if (dir == FibDir.FROMHIGH) dFibRetrace = hlh.HighestHigh - hlh.HLRange * daFibLevel[(int)fiblev];
        else dFibRetrace = hlh.LowestLow + hlh.HLRange * daFibLevel[(int)fiblev];

        if (dP > dFibRetrace - dThreshold && dP < dFibRetrace + dThreshold) return true;
        else return false;
    }
    public double GetFibRetracement(FibLevel fiblev, FibDir dir)
    {
        if (dir == FibDir.FROMHIGH) return hlh.HighestHigh - hlh.HLRange * daFibLevel[(int)fiblev];
        else return hlh.LowestLow + hlh.HLRange * daFibLevel[(int)fiblev];
    }
    public static double GetFibRetracement(double dH, double dL, FibLevel fiblev, FibDir dir)
    {
        if (double.IsNaN(dH) || double.IsNaN(dL)) return double.NaN;
        if (dL > dH) return double.NaN;
        if (dir == FibDir.FROMHIGH) return dH - (dH - dL) * daFibLevel[(int)fiblev];
        else return dL + (dH - dL) * daFibLevel[(int)fiblev];
    }

}

public enum PivotPoint { R4, R3, R2, R1, PP, S1, S2, S3, S4 };
public class PivotPointHelper
{
    public static double GetPivotPoint(double dH, double dL, double dC, PivotPoint pp)
    {
        double dReturn = double.NaN;

        if (double.IsNaN(dH) || double.IsNaN(dL) || double.IsNaN(dC)) return double.NaN;
        if (dL > dH || dC > dH || dC < dL) return double.NaN;

        double dPP = (dH + dL + dC) / 3;

        switch (pp)
        {
            case PivotPoint.R4: dReturn = dPP + (dH - dL) * 3; break;
            case PivotPoint.R3: dReturn = dPP + (dH - dL) * 2; break;
            case PivotPoint.R2: dReturn = dPP + (dH - dL); break;
            case PivotPoint.R1: dReturn = (dPP * 2) - dL; break;
            case PivotPoint.PP: dReturn = dPP; break;
            case PivotPoint.S1: dReturn = (dPP * 2) - dH; break;
            case PivotPoint.S2: dReturn = dPP - (dH - dL); break;
            case PivotPoint.S3: dReturn = dPP - (dH - dL) * 2; break;
            case PivotPoint.S4: dReturn = dPP - (dH - dL) * 3; break;
        }
        return dReturn;
    }
}

public class HighLowHelper
{
    private int iSize = 0;
    private ArrayList alHValRaw = null;
    private ArrayList alHValSorted = null;
    private ArrayList alLValRaw = null;
    private ArrayList alLValSorted = null;

    public HighLowHelper(int iS)
    {
        iSize = iS;
        alHValRaw = new ArrayList();
        alHValSorted = new ArrayList();
        alLValRaw = new ArrayList();
        alLValSorted = new ArrayList();
    }
    public int Count
    {
        get
        {
            if (alHValRaw.Count == alLValRaw.Count) return alHValRaw.Count;
            else return 0;
        }
    }
    public bool Stable
    {
        get
        {
            if (Count < iSize) return false;
            else return true;
        }
    }
    public void Reset()
    {
        alHValRaw.Clear();
        alHValSorted.Clear();
        alLValRaw.Clear();
        alLValSorted.Clear();
    }
    public void Add(Double dH, Double dL)
    {
        int iLoc = 0;
        alHValRaw.Add(dH);
        alLValRaw.Add(dL);
        if (alHValSorted.Count == 0) alHValSorted.Add(dH);
        else
        {
            iLoc = alHValSorted.BinarySearch(dH);
            if (iLoc < 0) alHValSorted.Insert(~iLoc, dH);
            else alHValSorted.Insert(iLoc, dH);
        }
        if (alLValSorted.Count == 0) alLValSorted.Add(dL);
        else
        {
            iLoc = alLValSorted.BinarySearch(dL);
            if (iLoc < 0) alLValSorted.Insert(~iLoc, dL);
            else alLValSorted.Insert(iLoc, dL);
        }

        if (alHValRaw.Count > iSize)
        {
            iLoc = alHValSorted.BinarySearch(alHValRaw[0]);
            alHValSorted.RemoveAt(iLoc);
            alHValRaw.RemoveAt(0);
        }
        if (alLValRaw.Count > iSize)
        {
            iLoc = alLValSorted.BinarySearch(alLValRaw[0]);
            alLValSorted.RemoveAt(iLoc);
            alLValRaw.RemoveAt(0);
        }
    }

    public double HighestHigh
    {
        get
        {
            if (alHValSorted.Count > 0)
                return (double)alHValSorted[alHValSorted.Count - 1];
            else
                return double.NaN;
        }
    }
    public double LowestLow
    {
        get
        {
            if (alLValSorted.Count > 0)
                return (double)alLValSorted[0];
            else
                return double.NaN;
        }
    }
    public double ChannelHeight
    {
        get
        {
            return HighestHigh - LowestLow;
        }
    }
    public double HLRange
    {
        get
        {
            return ChannelHeight;
        }
    }
}
public class HighLowHelperOld
{
    private int iSize = 0;
    private ArrayList alHighs = null;
    private ArrayList alLows = null;

    public HighLowHelperOld(int iS)
    {
        alHighs = new ArrayList();
        alLows = new ArrayList();
        iSize = iS;
    }
    public int Count
    {
        get
        {
            return alHighs.Count;
        }
    }
    public bool Stable
    {
        get
        {
            if (alHighs.Count == iSize && alLows.Count == iSize) return true;
            else return false;
        }
    }
    public void Reset()
    {
        alHighs = new ArrayList();
        alLows = new ArrayList();
    }
    public void Add(double dH, double dL)
    {
        alHighs.Add(dH);
        alLows.Add(dL);
        if (alHighs.Count > iSize) alHighs.RemoveAt(0);
        if (alLows.Count > iSize) alLows.RemoveAt(0);
    }
    public double HighestHigh
    {
        get
        {
            double dH = (double)alHighs[0];
            for (int i = 0; i < alHighs.Count; i++)
                if ((double)alHighs[i] > dH) dH = (double)alHighs[i];
            return dH;
        }
    }
    public double LowestLow
    {
        get
        {
            double dL = (double)alLows[0];
            for (int i = 0; i < alLows.Count; i++)
                if ((double)alLows[i] < dL) dL = (double)alLows[i];
            return dL;
        }
    }
    public double ChannelHeight
    {
        get
        {
            if (!this.Stable) return double.NaN;
            else return HighestHigh - LowestLow;
        }
    }
}

public class IsCrossHelper
{
    private double[] value1 = new double[2];
    private double[] value2 = new double[2];

    public bool Stable
    {
        get
        {
            if (!double.IsNaN(value1[0]) &&
                !double.IsNaN(value1[1]) &&
                !double.IsNaN(value2[0]) &&
                !double.IsNaN(value2[1]))
            {
                return true;
            }
            else return false;
        }
    }
    public IsCrossHelper()
    {
    }
    public void Reset()
    {
        value1[0] = double.NaN;
        value1[1] = double.NaN;
        value2[0] = double.NaN;
        value2[1] = double.NaN;
    }
    public void Add(double d1, double d2)
    {
        if (double.IsNaN(d1) || double.IsNaN(d2)) return;

        value1[0] = value1[1];
        value2[0] = value2[1];

        value1[1] = d1;
        value2[1] = d2;

        return;
    }
    //value1 crossing above value2
    public bool IsCrossUp()
    {
        if (value1[1] > value2[1] && value1[0] < value2[0]) return true;
        else return false;
    }
    //value1 crossing below value2
    public bool IsCrossDown()
    {
        if (value1[1] < value2[1] && value1[0] > value2[0]) return true;
        else return false;
    }
}

public class SimpleLinRegrnHelper
{
    private bool bStableY = false;
    private int iSize = 0;

    private double[] daY = null;
    private double dFullSumY = 0;
    private int iPointerY = 0;

    private double[] daCovXY = null;
    private double[] daVarX = null;
    private bool bNeedUpdateCovXY = true;
    private bool bNeedUpdateVarX = true;

    public bool Stable { get { return bStableY; } }

    public SimpleLinRegrnHelper(int iS)
    {
        Reset(iS);
    }

    public void Reset(int iS)
    {
        bStableY = false;
        iSize = iS;

        dFullSumY = 0;
        iPointerY = 0;

        daY = new double[iSize];
        daCovXY = new double[iSize];
        daVarX = new double[iSize];
        bNeedUpdateCovXY = true;
        bNeedUpdateVarX = true;
    }

    private int BChk(int idx)
    {
        if (idx >= iSize) return BChk(idx - iSize);
        else if (idx < 0) return BChk(idx + iSize);
        else return idx;
    }
    public void AddY(double dY)
    {
        daY[iPointerY] = dY;

        if (bStableY) dFullSumY -= daY[iPointerY];
        dFullSumY += dY;
        iPointerY++;

        if (iPointerY >= iSize)
        {
            iPointerY = 0;
            bStableY = true;
        }
        bNeedUpdateCovXY = true;
        bNeedUpdateVarX = true;

        return;
    }
    public double CovXY(int iS)
    {
        if (!bNeedUpdateCovXY) return daCovXY[BChk(iPointerY - 1)];
        if (iS > iSize) return double.NaN;

        if (bStableY)
        {
            double dAvgY = 0;
            double dAvgX = 0;

            if (iS == iSize)
            {
                dAvgY = dFullSumY / (double)iSize;
                dAvgX = ((double)iSize - 1) / 2d;
            }
            else
            {
                for (int i = 0; i < iS; i++)
                {
                    dAvgY += daY[BChk(iPointerY - 1 - i)];
                    dAvgX += iSize - 1 - i; //the X at iPointer = iSize; X-1 = iSize-1
                }
                dAvgY /= (double)iS;
                dAvgX /= (double)iS;
            }

            double dCovXY = 0;

            //the X at iPointer = iSize; X-1 = iSize-1
            for (int i = 0; i < iS; i++)
                dCovXY += (double)(BChk(iSize - 1 - i) - dAvgX) * (daY[BChk(iPointerY - 1 - i)] - dAvgY) / (double)iS;

            daCovXY[BChk(iPointerY - 1)] = dCovXY;
            return dCovXY;
        }
        else
        {
            return double.NaN;
        }
    }
    public double VarX(int iS)
    {
        if (!bNeedUpdateVarX) return daVarX[BChk(iPointerY - 1)];
        if (iS > iSize) return double.NaN;

        double dVX = 0;
        double dAvgX = 0;

        if (iS == iSize) dAvgX = ((double)iSize - 1) / 2d;
        else
        {
            for (int i = 0; i < iS; i++) dAvgX += iSize - 1 - i; //the X at iPointer = iSize; X-1 = iSize-1
            dAvgX /= (double)iS;
        }

        for (int i = 0; i < iS; i++)
            dVX += Math.Pow((double)iSize - 1 - i - dAvgX, 2) / (double)iS;

        daVarX[BChk(iPointerY - 1)] = dVX;

        return dVX;
    }

    public double Slope()
    {
        return Slope(iSize);
    }
    public double Slope(int iS)
    {
        if (bStableY)
        {
            return CovXY(Math.Min(iS, iSize)) / VarX(Math.Min(iS, iSize));
        }
        else
        {
            return double.NaN;
        }
    }
    public double Slope(int iPos, bool bHist)
    {
        if (bStableY)
        {
            return daCovXY[BChk(iPointerY - 1 + iPos)] / daVarX[BChk(iPointerY - 1 + iPos)];
        }
        else
        {
            return double.NaN;
        }
    }
}

public class SimpleLinRegrnHelperOld
{
    private bool bHasMemory = false;
    private bool bStableY = false;
    private double[] daY = null;
    private double dSumY = 0;
    private int iPointerY = 0;
    private int iSize = 0;

    private double dVarX = double.NaN;
    private double[] daCovXY = null;

    private bool bNeedUpdateCovXY = true;

    public bool Stable { get { return bStableY; } }

    public SimpleLinRegrnHelperOld(int iS, bool bHM)
    {
        bHasMemory = bHM;
        Reset(iS);
    }
    public void Reset(int iS)
    {
        iSize = iS;
        dSumY = 0;
        iPointerY = 0;
        dVarX = VarX(iSize); //calculate once and for all
        bStableY = false;

        daY = new double[iSize];
        daCovXY = new double[iSize];
        bNeedUpdateCovXY = true;
    }
    private int BChk(int idx)
    {
        if (idx >= iSize) return BChk(idx - iSize);
        else if (idx < 0) return BChk(idx + iSize);
        else return idx;
    }
    public void AddY(double dY)
    {
        if (bStableY) dSumY -= daY[iPointerY];
        daY[iPointerY] = dY;
        dSumY += dY;
        iPointerY++;

        if (iPointerY >= iSize)
        {
            iPointerY = 0;
            bStableY = true;
        }
        bNeedUpdateCovXY = true;

        if (bHasMemory) CovXY();

        return;
    }
    public double CovXY()
    {
        if (!bNeedUpdateCovXY) return daCovXY[BChk(iPointerY - 1)];

        if (bStableY)
        {
            double dAvgY = dSumY / (double)iSize;
            double dAvgX = ((double)iSize + 1) / 2d;
            double dCovXY = 0;
            for (int i = 0; i < iSize; i++)
                dCovXY += ((double)((iSize - iPointerY + i) % iSize) - dAvgX) * (daY[i] - dAvgY) / (double)iSize;
            daCovXY[BChk(iPointerY - 1)] = dCovXY;
            return dCovXY;
        }
        else
        {
            return double.NaN;
        }
    }
    public double VarX(int iS)
    {
        double dVX = 0;
        double dAvgX = ((double)iS + 1) / 2d;

        for (int i = 0; i < iS; i++)
            dVX += Math.Pow((double)i + 1 - dAvgX, 2) / (double)iS;

        return dVX;
    }
    public double Slope(int iPos)
    {
        if (bStableY && bHasMemory)
        {
            return daCovXY[BChk(iPointerY - 1 + iPos)] / dVarX;
        }
        else
        {
            return double.NaN;
        }
    }
    public double Slope()
    {
        if (bStableY)
        {
            return CovXY() / dVarX;
        }
        else
        {
            return double.NaN;
        }
    }
}


public class CovarCorrel
{
    private StatHelper shA = null;
    private StatHelper shB = null;

    public CovarCorrel(StatHelper a, StatHelper b)
    {
        shA = a;
        shB = b;
    }

    public double Covar
    {
        get
        {
            int i = 0;
            double dAAM = shA.AMean; if (double.IsNaN(dAAM)) return double.NaN;
            double dBAM = shB.AMean; if (double.IsNaN(dBAM)) return double.NaN;
            if (shA.Count != shB.Count) return double.NaN;

            double dSumABDevFrmMean = 0;
            while (i < shA.Capacity)
            {
                dSumABDevFrmMean += (shA.X[i] - shA.AMean) * (shB.X[i] - shB.AMean);
                i++;
                if (i == shA.Count) break;
            }
            return dSumABDevFrmMean / shA.Count;
        }
    }
}

public class StatHelper
{
    private long lIterationCnt = 0;
    private double[] daX = null;
    private long[] laV = null;

    private int iCapacity = 0;
    private int iPtr = 0;
    private double dXVRunningTotal = 0;
    private long lVRunningTotal = 0;
    private bool bRoundTrip = false;

    private double dAMLatest = double.NaN;
    private double dVARLatest = double.NaN;
    private double dSVARLatest = double.NaN;
    private double dSVAR0Latest = double.NaN;
    private double dSDLatest = double.NaN;
    private double dSKEWLatest = double.NaN;
    private double dKURTLatest = double.NaN;
    private double dKURTPveLatest = double.NaN;
    private double dKURTNveLatest = double.NaN;

    private double dAMPrevious = double.NaN;
    private double dVARPrevious = double.NaN;
    private double dSVARPrevious = double.NaN;
    private double dSVAR0Previous = double.NaN;
    private double dSDPrevious = double.NaN;
    private double dSKEWPrevious = double.NaN;
    private double dKURTPrevious = double.NaN;
    private double dKURTPvePrevious = double.NaN;
    private double dKURTNvePrevious = double.NaN;

    private bool bAMNeedsUpdate = true;
    private bool bVARNeedsUpdate = true;
    private bool bSVARNeedsUpdate = true;
    private bool bSVAR0NeedsUpdate = true;
    private bool bSDNeedsUpdate = true;
    private bool bSkewnessNeedsUpdate = true;
    private bool bKurtosisNeedsUpdate = true;
    private bool bKurtosisPveNeedsUpdate = true;
    private bool bKurtosisNveNeedsUpdate = true;

    public double[] X { get { return daX; } }
    public long[] V { get { return laV; } }

    public StatHelper(int i)
    {
        Reset(i);
    }
    public void Reset(int i)
    {
        lIterationCnt = 0;
        iCapacity = i;
        iPtr = 0;
        dXVRunningTotal = 0;
        lVRunningTotal = 0;
        bRoundTrip = false;

        dAMLatest = double.NaN;
        dVARLatest = double.NaN;
        dSVARLatest = double.NaN;
        dSVAR0Latest = double.NaN;
        dSDLatest = double.NaN;
        dSKEWLatest = double.NaN;
        dKURTLatest = double.NaN;
        dKURTPveLatest = double.NaN;
        dKURTNveLatest = double.NaN;

        dAMPrevious = double.NaN;
        dVARPrevious = double.NaN;
        dSVARPrevious = double.NaN;
        dSVAR0Previous = double.NaN;
        dSDPrevious = double.NaN;
        dSKEWPrevious = double.NaN;
        dKURTPrevious = double.NaN;
        dKURTPvePrevious = double.NaN;
        dKURTNvePrevious = double.NaN;

        bAMNeedsUpdate = true;
        bVARNeedsUpdate = true;
        bSVARNeedsUpdate = true;
        bSVAR0NeedsUpdate = true;
        bSDNeedsUpdate = true;
        bSkewnessNeedsUpdate = true;
        bKurtosisNeedsUpdate = true;
        bKurtosisPveNeedsUpdate = true;
        bKurtosisNveNeedsUpdate = true;
        
        daX = new double[iCapacity];
        laV = new long[iCapacity];
        return;
    }
    public void Add(double d)
    {
        Add(d, 1);
        return;
    }
    public void Add(double d, long v)
    {
        if (!double.IsNaN(d) && d != 0 && v >= 0)
        {
            lIterationCnt++; if (lIterationCnt > 1000000) lIterationCnt = 0;
            dXVRunningTotal = dXVRunningTotal - daX[iPtr] * laV[iPtr] + d * v;
            lVRunningTotal = lVRunningTotal - laV[iPtr] + v;
            daX[iPtr] = d;
            laV[iPtr] = v;

            iPtr = (iPtr + 1) % iCapacity;
            if (iPtr == 0) bRoundTrip = true;

            bAMNeedsUpdate = true;
            bVARNeedsUpdate = true;
            bSVARNeedsUpdate = true;
            bSVAR0NeedsUpdate = true;
            bSDNeedsUpdate = true;
            bSkewnessNeedsUpdate = true;
            bKurtosisNeedsUpdate = true;
            bKurtosisPveNeedsUpdate = true;
            bKurtosisNveNeedsUpdate = true;
        }
        return;
    }
    public bool Stable
    {
        get
        {
            if (bRoundTrip) return true;
            else return false;
        }
    }
    public int Capacity { get { return iCapacity; } }

    public int Count
    {
        get
        {
            if (bRoundTrip) return iCapacity;
            else return iPtr;
        }
    }
    public double AMean
    {
        get
        {
            if (bAMNeedsUpdate && lVRunningTotal > 0)
            {
                dAMPrevious = dAMLatest;
                dAMLatest = dXVRunningTotal / lVRunningTotal;
                bAMNeedsUpdate = false;
            }
            return dAMLatest;
        }
    }
    public double AMeanPrev
    {
        get
        {
            return dAMPrevious;
        }
    }
    public double Var
    {
        get
        {
            if (bVARNeedsUpdate)
            {
                int i = 0;
                double dAM = AMean; if (double.IsNaN(dAM)) return double.NaN;

                double dSumVTerm = 0;
                double dSumV = 0;
                while (i < iCapacity)
                {
                    dSumVTerm += laV[i] * (daX[i] - dAM) * (daX[i] - dAM);
                    dSumV += laV[i];
                    i++;
                    if (!bRoundTrip && i == iPtr) break;
                }
                dVARPrevious = dVARLatest;
                dVARLatest = dSumVTerm / dSumV;
            }
            return dVARLatest;
        }
    }

    //from AM, not from 0
    public double SemiVar
    {
        get
        {
            if (bSVARNeedsUpdate)
            {
                int i = 0;
                double dAM = AMean; if (double.IsNaN(dAM)) return double.NaN;

                double dSumVTerm = 0;
                double dSumV = 0;
                while (i < iCapacity)
                {
                    if (daX[i] < dAM)
                    {
                        dSumVTerm += laV[i] * (daX[i] - dAM) * (daX[i] - dAM);
                        dSumV += laV[i];
                    }
                    i++;
                    if (!bRoundTrip && i == iPtr) break;
                }
                dSVARPrevious = dSVARLatest;
                dSVARLatest = dSumVTerm / dSumV;
            }
            return dSVARLatest;
        }
    }
    //calculating deviation of daX[i] from 0, not from the mean
    //with a lower bound of 1
    public double SemiVar0
    {
        get
        {
            if (bSVAR0NeedsUpdate)
            {
                int i = 0;

                double dSumVTerm = 0;
                double dSumV = 0;
                while (i < iCapacity)
                {
                    if (daX[i] < 0)
                    {
                        dSumVTerm += laV[i] * (daX[i] * daX[i]);
                        dSumV += laV[i];
                    }
                    i++;
                    if (!bRoundTrip && i == iPtr) break;
                }
                dSVAR0Previous = dSVAR0Latest;
                dSVAR0Latest = dSumVTerm / dSumV;

                if (dSVAR0Latest < 1) dSVAR0Latest = 1;
            }
            return dSVAR0Latest;
        }
    }

    public double VarPrev
    {
        get
        {
            return dVARPrevious;
        }
    }
    public double Stdev
    {
        get
        {
            if (bSDNeedsUpdate)
            {
                dSDPrevious = dSDLatest;
                dSDLatest = Math.Sqrt(Var);
            }
            return dSDLatest;
        }
    }
    public double StdevPrev
    {
        get
        {
            return dSDPrevious;
        }
    }

    public double Skewness
    {
        get
        {
            if (bSkewnessNeedsUpdate)
            {
                int i = 0;
                double dAM = AMean; if (double.IsNaN(dAM)) return double.NaN;

                double dSumVTerm = 0;
                double dSumV = 0;
                while (i < iCapacity)
                {
                    dSumVTerm += laV[i] * (daX[i] - dAM) * (daX[i] - dAM) * (daX[i] - dAM);
                    dSumV += laV[i];
                    i++;
                    if (!bRoundTrip && i == iPtr) break;
                }
                dSKEWPrevious = dSKEWLatest;
                dSKEWLatest = dSumVTerm / dSumV;
            }
            return dSKEWLatest;
        }
    }

    public double Kurtosis
    {
        get
        {
            if (bKurtosisNeedsUpdate)
            {
                int i = 0;
                double dAM = AMean; if (double.IsNaN(dAM)) return double.NaN;

                double dSumVTerm = 0;
                double dSumV = 0;
                while (i < iCapacity)
                {
                    dSumVTerm += laV[i] * (daX[i] - dAM) * (daX[i] - dAM)
                                        * (daX[i] - dAM) * (daX[i] - dAM);
                    dSumV += laV[i];
                    i++;
                    if (!bRoundTrip && i == iPtr) break;
                }
                dKURTPrevious = dKURTLatest;
                dKURTLatest = dSumVTerm / dSumV;
            }
            return dKURTLatest;
        }
    }

    public double KurtosisPve
    {
        get
        {
            if (bKurtosisPveNeedsUpdate)
            {
                int i = 0;
                double dAM = AMean; if (double.IsNaN(dAM)) return double.NaN;

                double dSumVTerm = 0;
                double dSumV = 0;
                while (i < iCapacity)
                {
                    if (daX[i] > dAM)
                    {
                        dSumVTerm += laV[i] * (daX[i] - dAM) * (daX[i] - dAM)
                                            * (daX[i] - dAM) * (daX[i] - dAM);
                        dSumV += laV[i];
                    }
                    i++;
                    if (!bRoundTrip && i == iPtr) break;
                }
                dKURTPvePrevious = dKURTPveLatest;
                dKURTPveLatest = dSumVTerm / dSumV;
            }
            return dKURTPveLatest;
        }
    }
    public double KurtosisNve
    {
        get
        {
            if (bKurtosisNveNeedsUpdate)
            {
                int i = 0;
                double dAM = AMean; if (double.IsNaN(dAM)) return double.NaN;

                double dSumVTerm = 0;
                double dSumV = 0;
                while (i < iCapacity)
                {
                    if (daX[i] < dAM)
                    {
                        dSumVTerm += laV[i] * (daX[i] - dAM) * (daX[i] - dAM)
                                            * (daX[i] - dAM) * (daX[i] - dAM);
                        dSumV += laV[i];
                    }
                    i++;
                    if (!bRoundTrip && i == iPtr) break;
                }
                dKURTNvePrevious = dKURTNveLatest;
                dKURTNveLatest = dSumVTerm / dSumV;
            }
            return dKURTNveLatest;
        }
    }
}

public class TrendStrength
{
    private int iNumOfMAs = 0;
    private int iStartingMA = 0;
    private int iStepSize = 0;
    private SmaAlt[] smaTrendStren = null;

    public bool Stable
    {
        get
        {
            bool bStable = true;
            for (int i = 0; i < iNumOfMAs; i++)
                if (!smaTrendStren[i].Stable) bStable = false;
            return bStable;
        }
    }
    public void GetSMAValues(out double[] daSMAV)
    {
        daSMAV = new double[iNumOfMAs];
        for (int i = 0; i < iNumOfMAs; i++)
            daSMAV[i] = smaTrendStren[i].SMA;
    }
    public TrendStrength(int iNMA, int iST, int iSS)
    {
        iNumOfMAs = iNMA;
        iStartingMA = iST;
        iStepSize = iSS;

        Reset();
    }
    public void Reset()
    {
        smaTrendStren = new SmaAlt[iNumOfMAs];

        int i = iStartingMA;

        for (int j = 0; j < iNumOfMAs; j++)
        {
            smaTrendStren[j] = new SmaAlt(i);
            i += iStepSize;
        }
    }
    public void Add(double dPx)
    {
        for (int i = 0; i < iNumOfMAs; i++) smaTrendStren[i].Add(dPx);
    }
    public double GetCurrentScore(double dPx)
    {
        double dScore = 0;
        for (int i = 0; i < iNumOfMAs; i++)
        {
            if (dPx > smaTrendStren[i].SMA) dScore += 100 / iNumOfMAs;
            else dScore -= 100 / iNumOfMAs;
        }
        return dScore;
    }
    public double GetPreviousScore(double dPx)
    {
        double dScore = 0;
        for (int i = 0; i < iNumOfMAs; i++)
        {
            if (dPx > smaTrendStren[i].Previous) dScore += 100 / iNumOfMAs;
            else dScore -= 100 / iNumOfMAs;
        }
        return dScore;
    }
}

//This class recognizes 2 types: A(1) and B(0).
public class BeanCounterAB
{
    long lCntA = 0;
    long lCntB = 0;
    int iSize = 0;
    ArrayList alCumCnt = null;
    long lLastCumCnt = 0;

    public BeanCounterAB(int iS)
    {
        Reset(iS);
    }
    public void Reset(int iS)
    {
        lCntA = 0;
        lCntB = 0;
        iSize = iS;
        alCumCnt = new ArrayList();
        lLastCumCnt = 0;
    }
    public bool Stable
    {
        get
        {
            return (alCumCnt.Count >= iSize);
        }
    }
    public void AddA(long lCnt)
    {
        for (long i = 0; i < lCnt; i++) alCumCnt.Add(alCumCnt.Count > 0 ? (long)alCumCnt[alCumCnt.Count - 1] + 1 : 1);
        lCntA += lCnt;
        AdjustCntAB();
    }
    public void AddB(long lCnt)
    {
        for (long i = 0; i < lCnt; i++) alCumCnt.Add(alCumCnt.Count > 0 ? (long)alCumCnt[alCumCnt.Count - 1] : 0);
        lCntB += lCnt;
        AdjustCntAB();
    }
    private void AdjustCntAB()
    {
        if (alCumCnt.Count > iSize)
        {
            long lNumOfAExcluded = (long)alCumCnt[alCumCnt.Count - iSize - 1] - lLastCumCnt;
            long lNumOfBExcluded = alCumCnt.Count - iSize - lNumOfAExcluded;
            lCntA -= lNumOfAExcluded;
            lCntB -= lNumOfBExcluded;
            lLastCumCnt = (long)alCumCnt[alCumCnt.Count - iSize - 1];
            alCumCnt.RemoveRange(0, alCumCnt.Count - iSize);

        }
        return;
    }
    public long CountA
    {
        get
        {
            return lCntA;
        }
    }
    public long CountB
    {
        get
        {
            return lCntB;
        }
    }
    public double RatioAB
    {
        get
        {
            if (lCntB > 0) return (double)lCntA / (double)lCntB;
            else return double.NaN;
        }
    }
    public double RatioBA
    {
        get
        {
            if (lCntA > 0) return (double)lCntB / (double)lCntA;
            else return double.NaN;
        }
    }
    public double RatioAT
    {
        get
        {
            if (lCntA + lCntB > 0) return (double)lCntA / (double)(lCntA + lCntB);
            else return double.NaN;
        }
    }
    public double RatioBT
    {
        get
        {
            if (lCntA + lCntB > 0) return (double)lCntB / (double)(lCntA + lCntB);
            else return double.NaN;
        }
    }
}

public class StdNmlRandVar
{
    Random rand = null;

    public StdNmlRandVar()
    {
        rand = new Random(unchecked((int)(DateTime.Now.Ticks)));
    }
    public double GetUniformFrmZeroToOne()
    {
        return ((double)(rand.Next() % 32767)) / 32767;
    }
    public double GetStdNml()
    {
        double dRtn = 0;
        for (int i = 0; i < 12; i++) dRtn += GetUniformFrmZeroToOne();
        return dRtn - 6;
    }
}

// rbdts = u S dt + s S dZ
public class DiffusionProcessGeoBrwn
{
    double d_mu;
    double d_sigma;
    double d_S;
    StdNmlRandVar snrv = new StdNmlRandVar();

    private double GetDifferentialNoMem(double dt)
    {
        return (d_mu * dt + d_sigma * snrv.GetStdNml() * Math.Sqrt(dt)) * d_S;
    }

    public DiffusionProcessGeoBrwn(double dm, double rbdts, double ds0)
    {
        d_mu = dm;
        d_sigma = rbdts;
        d_S = ds0;
    }
    // rbdts = uSdt + sSdB
    public double GetNext(double dt)
    {
        d_S += GetDifferentialNoMem(dt);
        return d_S;
    }
};

// rbdts = u (S' - S) dt + s S dZ
public class DiffusionProcessFixedIncm
{
    double d_mu;
    double d_sigma;
    double d_Sbar;
    double d_S;
    StdNmlRandVar snrv = new StdNmlRandVar();

    private double GetDifferentialNoMem(double dt)
    {
        return (d_mu * (d_Sbar - d_S) * dt + d_sigma * snrv.GetStdNml() * Math.Sqrt(dt)) * d_S;
    }

    public DiffusionProcessFixedIncm(double dm, double rbdts, double dsbar, double ds0)
    {
        d_mu = dm;
        d_sigma = rbdts;
        d_Sbar = dsbar;
        d_S = ds0;
    }
    // rbdts = uSdt + sSdB
    public double GetNext(double dt)
    {
        d_S += GetDifferentialNoMem(dt);
        return d_S;
    }
}



// X = A f ( w t ) where f is cos or sin
public class Sinusoidal
{
    bool bIsSine;
    double dA;
    double dT; //period
    double dTime;

    public Sinusoidal(char c, double da, double dt)
    {
        if (c == 'c' || c == 'C') bIsSine = false;
        else if (c == 's' || c == 'S') bIsSine = true;
        dA = da;
        dT = dt;
        dTime = 0;
    }
    public void UpdatePeriod(double dt)
    {
        dT = dt;
    }
    public double GetNext(double dt)
    {
        dTime += dt;
        if (!bIsSine) return dA * Math.Cos(2 * Math.PI / dT * dTime);
        else return dA * Math.Sin(2 * Math.PI / dT * dTime);
    }
}

public class JohnEhlers
{
    const int iCapacity = 1000;

    StatHelper shHistPx = null;

    double[] daHistH;
    double[] daHistL;
    double[] daHistC;
    double[] daHistM;
    int iCirArrIdx;
    bool bHistPxReady;

    double[] daRange;
    double[] daSmooth;
    double[] daDetrender;
    double[] daPeriodDD;
    double[] daSmoothPeriodDD;
    double[] daPeriodHD;
    double[] daSmoothPeriodHD;
    double[] daSmoothPeriodAvg;
    double dInitGuessPeriod;

    double[] daSignal;
    double[] daNoise;
    double[] daSNR;

    double[] daSineWaveI;
    double[] daSineWaveILead;
    double[] daSSineWave;

    double[] daStoPctS;
    double[] daStoPctK;
    double[] daStoPctD;

    double[] dajInPhase;
    double[] dajQuadrature;
    double[] daInPhase1;
    double[] daQuadrature1;
    double[] daInPhase2;
    double[] daQuadrature2;
    double[] daInPhase3;
    double[] daQuadrature3;
    double[] Re;
    double[] Im;

    bool bDetrenderReady;

    double[] daPASMA_11;
    double[] daPASMA_12;
    double[] daPASMA_13;
    double[] daPASMA_21;
    double[] daZLSMA;
    double[] daIPEMA;
    double[] daInstTrendl;

    private int BChk(int i)
    {
        if (i >= 0 && i < iCapacity) return i;
        else if (i < 0) return BChk(i + iCapacity);
        else if (i >= iCapacity) return BChk(i - iCapacity);
        else return i;
    }

    public JohnEhlers(double dG)
    {
        shHistPx = new StatHelper(3 * 22); //maybe that's the last 3 cycles? haha

        dInitGuessPeriod = dG;

        daSignal = new double[iCapacity];
        daNoise = new double[iCapacity];
        daSNR = new double[iCapacity];

        daSineWaveI = new double[iCapacity];
        daSineWaveILead = new double[iCapacity];
        daSSineWave = new double[iCapacity];

        daStoPctS = new double[iCapacity];
        daStoPctK = new double[iCapacity];
        daStoPctD = new double[iCapacity];

        daHistH = new double[iCapacity];
        daHistL = new double[iCapacity];
        daHistC = new double[iCapacity];
        daHistM = new double[iCapacity];
        iCirArrIdx = 0;
        bHistPxReady = false;
        daRange = new double[iCapacity];

        daSmooth = new double[iCapacity];
        daDetrender = new double[iCapacity];
        daPeriodDD = new double[iCapacity];
        daSmoothPeriodDD = new double[iCapacity];
        daPeriodHD = new double[iCapacity];
        daSmoothPeriodHD = new double[iCapacity];
        daSmoothPeriodAvg = new double[iCapacity];

        dajInPhase = new double[iCapacity];
        dajQuadrature = new double[iCapacity];
        daInPhase1 = new double[iCapacity];
        daQuadrature1 = new double[iCapacity];
        daInPhase2 = new double[iCapacity];
        daQuadrature2 = new double[iCapacity];
        daInPhase3 = new double[iCapacity];
        daQuadrature3 = new double[iCapacity];

        Re = new double[iCapacity];
        Im = new double[iCapacity];

        daPASMA_11 = new double[iCapacity];
        daPASMA_12 = new double[iCapacity];
        daPASMA_13 = new double[iCapacity];
        daPASMA_21 = new double[iCapacity];
        daZLSMA = new double[iCapacity];
        daIPEMA = new double[iCapacity];
        daInstTrendl = new double[iCapacity];

        bDetrenderReady = false;

        for (int j = 0; j < iCapacity; j++)
        {
            daRange[j] = 0;

            daSignal[j] = 0;
            daNoise[j] = 0;
            daSNR[j] = 0;

            daSineWaveI[j] = 0;
            daSineWaveILead[j] = 0;
            daSSineWave[j] = 0;

            daStoPctS[j] = 0;
            daStoPctK[j] = 0;
            daStoPctD[j] = 0;

            daSmooth[j] = 0;
            daDetrender[j] = 0;
            daPeriodDD[j] = dInitGuessPeriod;
            daSmoothPeriodDD[j] = dInitGuessPeriod;
            daPeriodHD[j] = dInitGuessPeriod;
            daSmoothPeriodHD[j] = dInitGuessPeriod;
            daSmoothPeriodAvg[j] = dInitGuessPeriod;

            dajInPhase[j] = 0;
            dajQuadrature[j] = 0;
            daInPhase1[j] = 0;
            daQuadrature1[j] = 0;
            daInPhase2[j] = 0;
            daQuadrature2[j] = 0;
            daInPhase3[j] = 0;
            daQuadrature3[j] = 0;

            Re[j] = 0;
            Im[j] = 0;

            daPASMA_11[j] = 0;
            daPASMA_12[j] = 0;
            daPASMA_13[j] = 0;
            daPASMA_21[j] = 0;
            daZLSMA[j] = 0;
            daIPEMA[j] = 0;
            daInstTrendl[j] = 0;

        }
    }

    public double GetHistPx()
    {
        return daHistM[BChk(iCirArrIdx - 1)];
    }
    public double GetPeriodDD()
    {
        return daSmoothPeriodDD[BChk(iCirArrIdx - 1)];
    }
    public double GetPeriodHD()
    {
        return daSmoothPeriodHD[BChk(iCirArrIdx - 1)];
    }
    public double GetSNR()
    {
        return daSNR[BChk(iCirArrIdx - 1)];
    }
    public double GetSmoothPeriodAvg()
    {
        return daSmoothPeriodAvg[BChk(iCirArrIdx - 1)];
    }
    public double GetSineWaveI()
    {
        return daSineWaveI[BChk(iCirArrIdx - 1)];
    }
    public double GetSineWaveILead()
    {
        return daSineWaveILead[BChk(iCirArrIdx - 1)];
    }
    public double GetSSineWave()
    {
        return daSSineWave[BChk(iCirArrIdx - 1)];
    }
    public double GetStoPctK()
    {
        return daStoPctK[BChk(iCirArrIdx - 1)];
    }
    public double GetStoPctD()
    {
        return daStoPctD[BChk(iCirArrIdx - 1)];
    }
    public double GetZeroLagSMA()
    {
        return daZLSMA[BChk(iCirArrIdx - 1)];
    }
    public double GetInvPeriodEMA()
    {
        return daIPEMA[BChk(iCirArrIdx - 1)];
    }
    public double GetInstTrendl()
    {
        return daInstTrendl[BChk(iCirArrIdx - 1)];
    }
    public void Add(double dH, double dL, double dC)
    {
        //trash those dH==dL==dC
        if (dH == dL && dL == dC) return;
        if (double.IsNaN(dH) || double.IsNaN(dL) || double.IsNaN(dC)) return;

        double dM = (dH + dL + dC) / 3;

        if (!bHistPxReady && iCirArrIdx >= 4) bHistPxReady = true;
        if (!bDetrenderReady && iCirArrIdx >= 7) bDetrenderReady = true;

        daHistH[iCirArrIdx] = dH;
        daHistL[iCirArrIdx] = dL;
        daHistC[iCirArrIdx] = dC;
        daHistM[iCirArrIdx] = dM;

        daRange[iCirArrIdx] = (dH + dL) / 2;
        daNoise[iCirArrIdx] = 0.1 * (dH - dL) * 0.25 + 0.9 * daNoise[BChk(iCirArrIdx - 1)];

        CalSDIQ();
        CalDualDifferentiator();
        CalHomodyneDiscriminator();
        CalAvgSmoothPeriod();
        CalSNR();
        CalPeriodAdjSMA_11();
        CalPeriodAdjSMA_12();
        CalPeriodAdjSMA_13();
        CalPeriodAdjSMA_21();
        CalInstTrendl();
        CalZeroLagSMA();
        CalInvPeriodEMA();
        CalPeriodAdjSto();
        CalSineWaveIndicator();
        CalSSineWaveIndicator();

        iCirArrIdx = BChk(iCirArrIdx + 1);
    }
    private void CalInvPeriodEMA()
    {
        double dAlpha = (Math.Pow(daSmoothPeriodAvg[iCirArrIdx], 2) + daSmoothPeriodAvg[iCirArrIdx]) / 5000;

        if (Math.Abs(daIPEMA[BChk(iCirArrIdx - 1)]) < 0.0001 || Math.Abs(daHistM[BChk(iCirArrIdx - 2)]) < 0.0001)
            daIPEMA[iCirArrIdx] = daHistM[iCirArrIdx];
        else daIPEMA[iCirArrIdx] = dAlpha * daHistM[iCirArrIdx] + (1 - dAlpha) * daIPEMA[BChk(iCirArrIdx - 1)];
    }
    //Period
    private void CalPeriodAdjSMA_11()
    {
        int iPeriod = (int)(daSmoothPeriodAvg[iCirArrIdx]);
        iPeriod = (iPeriod > 6 ? iPeriod : 6);
        int iActPeriod = 0;

        daPASMA_11[iCirArrIdx] = 0;
        for (int i = 0; i < iPeriod; i++)
        {
            if (Math.Abs(daHistM[BChk(iCirArrIdx - i)]) > 0.0001) iActPeriod++;
            daPASMA_11[iCirArrIdx] += daHistM[BChk(iCirArrIdx - i)];
        }

        if (iActPeriod < 6 || Math.Abs(daPASMA_11[iCirArrIdx]) < 0.0001) daPASMA_11[iCirArrIdx] = daHistM[iCirArrIdx];
        else daPASMA_11[iCirArrIdx] = daPASMA_11[iCirArrIdx] / iActPeriod;
    }
    //Period / 2
    private void CalPeriodAdjSMA_12()
    {
        int iPeriod = (int)(daSmoothPeriodAvg[iCirArrIdx] / 2);
        iPeriod = (iPeriod > 5 ? iPeriod : 5);
        int iActPeriod = 0;

        daPASMA_12[iCirArrIdx] = 0;
        for (int i = 0; i < iPeriod; i++)
        {
            if (Math.Abs(daHistM[BChk(iCirArrIdx - i)]) > 0.0001) iActPeriod++;
            daPASMA_12[iCirArrIdx] += daHistM[BChk(iCirArrIdx - i)];
        }

        if (iActPeriod < 5 || Math.Abs(daPASMA_12[iCirArrIdx]) < 0.0001) daPASMA_12[iCirArrIdx] = daHistM[iCirArrIdx];
        else daPASMA_12[iCirArrIdx] = daPASMA_12[iCirArrIdx] / iActPeriod;
    }
    //Period / 3
    private void CalPeriodAdjSMA_13()
    {
        int iPeriod = (int)(daSmoothPeriodAvg[iCirArrIdx] / 3);
        iPeriod = (iPeriod > 5 ? iPeriod : 5);
        int iActPeriod = 0;

        daPASMA_13[iCirArrIdx] = 0;
        for (int i = 0; i < iPeriod; i++)
        {
            if (Math.Abs(daHistM[BChk(iCirArrIdx - i)]) > 0.0001) iActPeriod++;
            daPASMA_13[iCirArrIdx] += daHistM[BChk(iCirArrIdx - i)];
        }

        if (iActPeriod < 5 || Math.Abs(daPASMA_13[iCirArrIdx]) < 0.0001) daPASMA_13[iCirArrIdx] = daHistM[iCirArrIdx];
        else daPASMA_13[iCirArrIdx] = daPASMA_13[iCirArrIdx] / iActPeriod;
    }
    //Period * 2
    private void CalPeriodAdjSMA_21()
    {
        int iPeriod = (int)(daSmoothPeriodAvg[iCirArrIdx] * 2);
        iPeriod = (iPeriod > 6 ? iPeriod : 6);
        int iActPeriod = 0;

        daPASMA_21[iCirArrIdx] = 0;
        for (int i = 0; i < iPeriod; i++)
        {
            if (Math.Abs(daHistM[BChk(iCirArrIdx - i)]) > 0.0001) iActPeriod++;
            daPASMA_21[iCirArrIdx] += daHistM[BChk(iCirArrIdx - i)];
        }

        if (iActPeriod < 6 || Math.Abs(daPASMA_21[iCirArrIdx]) < 0.0001) daPASMA_21[iCirArrIdx] = daHistM[iCirArrIdx];
        else daPASMA_21[iCirArrIdx] = daPASMA_21[iCirArrIdx] / iActPeriod;
    }
    private void CalInstTrendl()
    {
        //Instantaneous Trendline similar to John Ehlers'
        daInstTrendl[iCirArrIdx] = (4 * daPASMA_11[iCirArrIdx] +
                                    3 * daPASMA_11[iCirArrIdx] +
                                    2 * daPASMA_11[iCirArrIdx] +
                                    1 * daPASMA_11[iCirArrIdx]) / 10;
    }
    private void CalZeroLagSMA()
    {
        //Zero Lag SMA: using Taylor Expansion
        if (Math.Abs(daPASMA_13[BChk(iCirArrIdx - 2)]) < 0.0001) daZLSMA[iCirArrIdx] = daPASMA_13[iCirArrIdx];
        else
        {
            daZLSMA[iCirArrIdx] = daPASMA_13[iCirArrIdx] +
                                    1 * (daPASMA_13[iCirArrIdx] - daPASMA_13[BChk(iCirArrIdx - 1)]) +
                                    0.5 * (daPASMA_13[iCirArrIdx] - daPASMA_13[BChk(iCirArrIdx - 1)] -
                                    daPASMA_13[BChk(iCirArrIdx - 1)] + daPASMA_13[BChk(iCirArrIdx - 2)]);
            daZLSMA[iCirArrIdx] = 0.33 * daZLSMA[iCirArrIdx] + 0.67 * daZLSMA[BChk(iCirArrIdx - 1)];
        }
    }
    private void CalSDIQ()
    {
        if (!bHistPxReady) return;

        //Smooth historical prices using EMA
        daSmooth[iCirArrIdx] = 4 * daHistM[BChk(iCirArrIdx)] +
                                    3 * daHistM[BChk(iCirArrIdx - 1)] +
                                    2 * daHistM[BChk(iCirArrIdx - 2)] +
                                    1 * daHistM[BChk(iCirArrIdx - 3)];
        daSmooth[iCirArrIdx] = daSmooth[iCirArrIdx] / 10;

        //Why the same as Hilbert transform?
        daDetrender[iCirArrIdx] = (0.0962 * daSmooth[BChk(iCirArrIdx)] +
                                        0.5769 * daSmooth[BChk(iCirArrIdx - 2)] -
                                        0.5769 * daSmooth[BChk(iCirArrIdx - 4)] -
                                        0.0962 * daSmooth[BChk(iCirArrIdx - 6)]) *
                                    (0.075 * daSmoothPeriodAvg[BChk(iCirArrIdx - 1)] + 0.54);


        if (!bHistPxReady || !bDetrenderReady) return;

        //Compute InPhase and Quadrature (Hilbert Transform)
        daQuadrature1[iCirArrIdx] = (0.0962 * daDetrender[BChk(iCirArrIdx)] +
                                        0.5769 * daDetrender[BChk(iCirArrIdx - 2)] -
                                        0.5769 * daDetrender[BChk(iCirArrIdx - 4)] -
                                        0.0962 * daDetrender[BChk(iCirArrIdx - 6)]) *
                                        (0.075 * daSmoothPeriodAvg[BChk(iCirArrIdx - 1)] + 0.54);
        daInPhase1[iCirArrIdx] = daDetrender[BChk(iCirArrIdx - 3)];

        //Advance the phase of I1 and Q1 by 90 deg
        dajInPhase[iCirArrIdx] = (0.0962 * daInPhase1[BChk(iCirArrIdx)] +
                                        0.5769 * daInPhase1[BChk(iCirArrIdx - 2)] -
                                        0.5769 * daInPhase1[BChk(iCirArrIdx - 4)] -
                                        0.0962 * daInPhase1[BChk(iCirArrIdx - 6)]) *
                                    (0.075 * daSmoothPeriodAvg[BChk(iCirArrIdx - 1)] + 0.54);
        dajQuadrature[iCirArrIdx] = (0.0962 * daQuadrature1[BChk(iCirArrIdx)] +
                                        0.5769 * daQuadrature1[BChk(iCirArrIdx - 2)] -
                                        0.5769 * daQuadrature1[BChk(iCirArrIdx - 4)] -
                                        0.0962 * daQuadrature1[BChk(iCirArrIdx - 6)]) *
                                        (0.075 * daSmoothPeriodAvg[BChk(iCirArrIdx - 1)] + 0.54);


        //Phasor addition for 3 bar averaging
        daInPhase2[iCirArrIdx] = daInPhase1[iCirArrIdx] - dajQuadrature[iCirArrIdx];
        daQuadrature2[iCirArrIdx] = daQuadrature1[iCirArrIdx] + dajInPhase[iCirArrIdx];

        //Smooth I and Q before applying the discriminator
        daInPhase2[iCirArrIdx] = 0.2 * daInPhase2[iCirArrIdx] + 0.8 * daInPhase2[BChk(iCirArrIdx - 1)];
        daQuadrature2[iCirArrIdx] = 0.2 * daQuadrature2[iCirArrIdx] + 0.8 * daQuadrature2[BChk(iCirArrIdx - 1)];

    }
    private void CalDualDifferentiator()
    {
        //Dual Differential Discriminator
        double dValue1 = daQuadrature2[iCirArrIdx] * (daInPhase2[iCirArrIdx] - daInPhase2[BChk(iCirArrIdx - 1)])
                        - daInPhase2[iCirArrIdx] * (daQuadrature2[iCirArrIdx] - daQuadrature2[BChk(iCirArrIdx - 1)]);
        //if (dValue1 < -0.01) //good for 40 to 10
        //if (dValue1 > 0.01) //good for 10 to 40 (after introducing noise sigma '0.005' to the source T (period)
        if (Math.Abs(dValue1) > 0.01)
        {
            daPeriodDD[iCirArrIdx] = 2 * Math.PI *
                                        (daInPhase2[iCirArrIdx] * daInPhase2[iCirArrIdx] +
                                        daQuadrature2[iCirArrIdx] * daQuadrature2[iCirArrIdx]) / Math.Abs(dValue1);
        }
        if (daPeriodDD[iCirArrIdx] > 1.5 * daPeriodDD[BChk(iCirArrIdx - 1)])
            daPeriodDD[iCirArrIdx] = 1.5 * daPeriodDD[BChk(iCirArrIdx - 1)];
        else if (daPeriodDD[iCirArrIdx] < 0.67 * daPeriodDD[BChk(iCirArrIdx - 1)])
            daPeriodDD[iCirArrIdx] = 0.67 * daPeriodDD[BChk(iCirArrIdx - 1)];

        if (daPeriodDD[iCirArrIdx] < 6) daPeriodDD[iCirArrIdx] = 6;
        else if (daPeriodDD[iCirArrIdx] > 50) daPeriodDD[iCirArrIdx] = 50;

        daPeriodDD[iCirArrIdx] = 0.1 * daPeriodDD[iCirArrIdx] + 0.9 * daPeriodDD[BChk(iCirArrIdx - 1)];

        daSmoothPeriodDD[iCirArrIdx] = daPeriodDD[iCirArrIdx] +
                                        (daPeriodDD[iCirArrIdx] - daPeriodDD[BChk(iCirArrIdx - 1)]) +
                                        0.5 * (daPeriodDD[iCirArrIdx] - daPeriodDD[BChk(iCirArrIdx - 1)] -
                                        daPeriodDD[BChk(iCirArrIdx - 1)] + daPeriodDD[BChk(iCirArrIdx - 2)]);
        daSmoothPeriodDD[iCirArrIdx] = 0.2 * daSmoothPeriodDD[iCirArrIdx] + 0.8 * daSmoothPeriodDD[BChk(iCirArrIdx - 1)];
    }
    private void CalHomodyneDiscriminator()
    {
        Re[iCirArrIdx] = daInPhase2[iCirArrIdx] * daInPhase2[BChk(iCirArrIdx - 1)] +
                            daQuadrature2[iCirArrIdx] * daQuadrature2[BChk(iCirArrIdx - 1)];
        Im[iCirArrIdx] = daInPhase2[iCirArrIdx] * daQuadrature2[BChk(iCirArrIdx - 1)] -
                            daQuadrature2[iCirArrIdx] * daInPhase2[BChk(iCirArrIdx - 1)];
        Re[iCirArrIdx] = 0.2 * Re[iCirArrIdx] + 0.8 * Re[BChk(iCirArrIdx - 1)];
        Im[iCirArrIdx] = 0.2 * Im[iCirArrIdx] + 0.8 * Im[BChk(iCirArrIdx - 1)];

        if (Im[iCirArrIdx] != 0 && Re[iCirArrIdx] != 0)
        {
            daPeriodHD[iCirArrIdx] = 2 * Math.PI / (Math.Atan(Im[iCirArrIdx] / Re[iCirArrIdx]));
        }

        if (daPeriodHD[iCirArrIdx] > 1.5 * daPeriodHD[BChk(iCirArrIdx - 1)])
            daPeriodHD[iCirArrIdx] = 1.5 * daPeriodHD[BChk(iCirArrIdx - 1)];

        if (daPeriodHD[iCirArrIdx] < 0.67 * daPeriodHD[BChk(iCirArrIdx - 1)])
            daPeriodHD[iCirArrIdx] = 0.67 * daPeriodHD[BChk(iCirArrIdx - 1)];

        if (daPeriodHD[iCirArrIdx] < 6) daPeriodHD[iCirArrIdx] = 6;
        if (daPeriodHD[iCirArrIdx] > 50) daPeriodHD[iCirArrIdx] = 50;

        daPeriodHD[iCirArrIdx] = 0.1 * daPeriodHD[iCirArrIdx] + 0.9 * daPeriodHD[BChk(iCirArrIdx - 1)];

        daSmoothPeriodHD[iCirArrIdx] = daPeriodHD[iCirArrIdx] +
                                        (daPeriodHD[iCirArrIdx] - daPeriodHD[BChk(iCirArrIdx - 1)]) +
                                        0.5 * (daPeriodHD[iCirArrIdx] - daPeriodHD[BChk(iCirArrIdx - 1)] -
                                        daPeriodHD[BChk(iCirArrIdx - 1)] + daPeriodHD[BChk(iCirArrIdx - 2)]);
        daSmoothPeriodHD[iCirArrIdx] = 0.2 * daSmoothPeriodHD[iCirArrIdx] + 0.8 * daSmoothPeriodHD[BChk(iCirArrIdx - 1)];

    }
    private void CalAvgSmoothPeriod()
    {
        daSmoothPeriodAvg[iCirArrIdx] = (daSmoothPeriodDD[iCirArrIdx] * 3 + daSmoothPeriodHD[iCirArrIdx] * 2) / 5;
        daSmoothPeriodAvg[iCirArrIdx] = 0.33 * daSmoothPeriodAvg[iCirArrIdx] + 0.67 * daSmoothPeriodAvg[BChk(iCirArrIdx - 1)];
    }
    private void CalSNR()
    {
        daQuadrature3[iCirArrIdx] = 0.5
                            * (daSmooth[iCirArrIdx] - daSmooth[BChk(iCirArrIdx - 2)])
                            * (0.1759 * daSmoothPeriodAvg[iCirArrIdx] + 0.4607);
        daInPhase3[iCirArrIdx] = 0;

        for (int i = 0; i < (int)(daSmoothPeriodAvg[iCirArrIdx] / 2) - 1; i++)
        {
            daInPhase3[iCirArrIdx] = daInPhase3[iCirArrIdx] + daQuadrature3[BChk(iCirArrIdx - i)];
        }

        daInPhase3[iCirArrIdx] = 1.57 * daInPhase3[iCirArrIdx] / (daSmoothPeriodAvg[iCirArrIdx] / 2);

        daSignal[iCirArrIdx] = daInPhase3[iCirArrIdx] * daInPhase3[iCirArrIdx] +
                                daQuadrature3[iCirArrIdx] * daQuadrature3[iCirArrIdx];

        if (daNoise[iCirArrIdx] != 0 && daSignal[iCirArrIdx] != 0)
            daSNR[iCirArrIdx] = 0.33 * (10 * Math.Log10(daSignal[iCirArrIdx] / daNoise[iCirArrIdx])) + 0.67 * daSNR[BChk(iCirArrIdx - 1)];
        else
            daSNR[iCirArrIdx] = daSNR[BChk(iCirArrIdx - 1)];
    }
    private void CalSineWaveIndicator()
    {
        //Calculate Dominant Cycle Phase
        double dDCPeriod = daSmoothPeriodAvg[iCirArrIdx];
        double dRealPart = 0;
        double dImagPart = 0;
        double dDCPhase = 0;

        for (int i = 0; i < (int)dDCPeriod - 1; i++)
        {
            //dRealPart += cos(2 * Math.PI * (double)i / dDCPeriod) * daSmooth[BChk(iCirArrIdx-i)];
            //dImagPart += sin(2 * Math.PI * (double)i / dDCPeriod) * daSmooth[BChk(iCirArrIdx-i)];
            dRealPart += Math.Cos(2 * Math.PI * (double)i / dDCPeriod) * daStoPctS[BChk(iCirArrIdx - i)] - 50;
            dImagPart += Math.Sin(2 * Math.PI * (double)i / dDCPeriod) * daStoPctS[BChk(iCirArrIdx - i)] - 50;
        }

        if (Math.Abs(dRealPart) > 0.001) dDCPhase = Math.Atan2(dImagPart, dRealPart);
        else dDCPhase = Math.PI / (double)2 * (dImagPart > 0 ? 1 : -1);

        /*
        dDCPhase += Math.PI / (double)2;

        //Compensate for one bar lag of the Weighted Moving Average
        dDCPhase += 2 * Math.PI / daSmoothPeriodAvg[iCirArrIdx];

        if (dImagPart < 0) dDCPhase += Math.PI;
        if (dDCPhase > 315 * Math.PI / 180) dDCPhase -= 2 * Math.PI;
        */

        daSineWaveI[iCirArrIdx] = Math.Sin(dDCPhase);
        daSineWaveILead[iCirArrIdx] = Math.Sin(dDCPhase + (Math.PI / (double)4));
    }
    private void CalSSineWaveIndicator()
    {
        double dPeriod = daSmoothPeriodAvg[BChk(iCirArrIdx - 1)];

        double dTrendSlope = 0;
        if (daPASMA_21[BChk(iCirArrIdx - 1)] > 0)
            dTrendSlope = daPASMA_21[iCirArrIdx] - daPASMA_21[BChk(iCirArrIdx - 1)];
        double dX_M = daHistM[iCirArrIdx] - daPASMA_11[iCirArrIdx];
        double dASinWt = dX_M - dTrendSlope * dPeriod / 2;
        shHistPx.Add(dASinWt);

        double dA = shHistPx.Stdev * 2.5; // covering 98%
        double dSinWt = dASinWt / dA;
        double dAdjSinWt = 0;
        if (dSinWt > 1) dAdjSinWt = 1;
        else if (dSinWt < -1) dAdjSinWt = -1;
        else dAdjSinWt = dSinWt;

        daSSineWave[iCirArrIdx] = dAdjSinWt;

    }
    private void CalPeriodAdjSto()
    {
        double dPeriod = daSmoothPeriodAvg[iCirArrIdx];
        double dHH = daHistH[iCirArrIdx] - daPASMA_12[iCirArrIdx];
        double dLL = daHistL[iCirArrIdx] - daPASMA_12[iCirArrIdx];

        for (int i = 1; i < (int)dPeriod; i++)
        {
            if (daHistH[BChk(iCirArrIdx - i)] > 0 && daHistH[BChk(iCirArrIdx - i)] - daPASMA_12[BChk(iCirArrIdx - i)] > dHH) dHH = daHistH[BChk(iCirArrIdx - i)] - daPASMA_12[BChk(iCirArrIdx - i)];
            if (daHistL[BChk(iCirArrIdx - i)] > 0 && daHistL[BChk(iCirArrIdx - i)] - daPASMA_12[BChk(iCirArrIdx - i)] < dLL) dLL = daHistL[BChk(iCirArrIdx - i)] - daPASMA_12[BChk(iCirArrIdx - i)];
        }

        daStoPctS[iCirArrIdx] = (daHistM[iCirArrIdx] - daPASMA_12[iCirArrIdx] - dLL) / (dHH - dLL) * 100;

        if (Math.Abs(daStoPctK[BChk(iCirArrIdx - 1)]) < 0.0001) daStoPctK[iCirArrIdx] = daStoPctS[iCirArrIdx];
        else daStoPctK[iCirArrIdx] = 0.1 * daStoPctS[iCirArrIdx] + 0.9 * daStoPctK[BChk(iCirArrIdx - 1)];

        daStoPctK[iCirArrIdx] = daStoPctK[iCirArrIdx] +
                                daStoPctK[iCirArrIdx] - daStoPctK[BChk(iCirArrIdx - 1)] +
                                0.5 * (daStoPctK[iCirArrIdx] - daStoPctK[BChk(iCirArrIdx - 1)] -
                                daStoPctK[BChk(iCirArrIdx - 1)] + daStoPctK[BChk(iCirArrIdx - 2)]);

        daStoPctK[iCirArrIdx] = 0.1 * daStoPctK[iCirArrIdx] + 0.9 * daStoPctK[BChk(iCirArrIdx - 1)];

        if (Math.Abs(daStoPctD[BChk(iCirArrIdx - 1)]) < 0.0001) daStoPctD[iCirArrIdx] = daStoPctK[iCirArrIdx];
        else daStoPctD[iCirArrIdx] = 0.33 * daStoPctK[iCirArrIdx] + 0.67 * daStoPctD[BChk(iCirArrIdx - 1)];

    }
}


