/*
 * Deltix
 */
//using QuantOffice.FinancialAnalysis.TA;
//using QuantOffice.FinancialAnalysis.Base;
//using FinMath.Derivatives;

/*
 * Programmer: Sunny Yan
 * DateTime: 17:30 17/05/2012
 */

using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.Serialization.Formatters.Binary;






// Option Code: e.g. C.19000.F12 (using futures code at the end)
public class SymbolCodeHelper


[Serializable]
public class VolatilitySurface
{
    private double dVHSILatest = double.NaN; //the very latest VHSI
    public double VHSILatest { set { dVHSILatest = value; } get { return dVHSILatest; } }
    private DateTime dtVolSmileDate;
    public DateTime VolSmileDate { set { dtVolSmileDate = value; } get { return dtVolSmileDate; } }

    //Historical Volatility:
    private const int iTRSIZE = 20;
    private double dVolECalpha = 1 / 50d;
    private double[] daDO = new double[iTRSIZE];
    private double[] daDH = new double[iTRSIZE];
    private double[] daDL = new double[iTRSIZE];
    private double[] daDC = new double[iTRSIZE];
    private int iDHLCPtr = 0;

    //Historical Volatility: Method 0 (Original ATR / Px > Quadratic Mapping)
    private double[] daVol0QM = new double[iTRSIZE];
    //Historical Volatility: Method 0a (Original ATR / Px > Cubic Least Square)
    private double[] daVol0aCM = new double[iTRSIZE];

    //Historical Volatility: Method 1 (ATR / Px > Quadratic Mapping)
    private double[] daVol1QM = new double[iTRSIZE];
    private double[] daVol1QMEC = new double[iTRSIZE];
    private double[] daVol1EC = new double[iTRSIZE]; //error correction
    private double[] daTR = new double[iTRSIZE];
    private double[] daATR = new double[iTRSIZE];
    public double ATR { get { return daATR[DHLC_BChk(iDHLCPtr - 1)]; } }
    private const double dM1alpha = 1 / 13d;
    //Historical Volatility: Method 2 (Zhang Qiang's paper)
    //Historical Volatility: Method 3 (Garman & Klass)
    private const double dM3alpha = 1 / 15d;
    private double[] daVol3 = new double[iTRSIZE];
    private double[] daVol3EMA = new double[iTRSIZE];
    private double[] daVol3EMALM = new double[iTRSIZE];
    private double[] daVol3EMALMEC = new double[iTRSIZE];
    private double[] daVol3EC = new double[iTRSIZE]; //error correction
    //Historical Volatility: Method 1 + 3
    private double[] daVol13 = new double[iTRSIZE];


    //current volatility smile imported from file
    private Hashtable htRuggedVolSmile1 = new Hashtable(); //the volatility smile 1st front mth
    private Hashtable htRuggedVolSmile2 = new Hashtable(); //the volatility smile 2nd front mth
    private Hashtable htVolSmile1 = new Hashtable(); //the volatility smile 1st front mth
    private Hashtable htVolSmile2 = new Hashtable(); //the volatility smile 2nd front mth
    private double dATMVolSmile1 = double.NaN; //the futures price at the time of volatility smile data
    private double dATMVolSmile2 = double.NaN; //the futures price at the time of volatility smile data
    private double dVHSIVolSmile1 = double.NaN; //VHSI at the time of volatility smile data
    private double dVHSIVolSmile2 = double.NaN; //VHSI at the time of volatility smile data
    private double dMthStartVolSmileTTM = 0;
    private DateTime dtMthStartDate;
    private double dMthStartATMVolSmile1 = 0;
    private Hashtable htMthStartVolSmile1 = new Hashtable();
    private SortedList lhtScVolSmile1 = new SortedList();

    private int DHLC_BChk(int iPtr)
    {
        if (iPtr >= iTRSIZE) return DHLC_BChk(iPtr - iTRSIZE);
        else if (iPtr < 0) return DHLC_BChk(iPtr + iTRSIZE);
        else return iPtr;
    }
    public VolatilitySurface(bool bForceReload)
    {
        ReadVolatilitySmileFromFile(bForceReload);
    }
    public void ReadVolatilitySmileFromFile(bool bForceReload)
    {
        if (IParam.DataDepotVolSmile.htVolSmile1.Count == 0 ||
            IParam.DataDepotVolSmile.htVolSmile2.Count == 0 ||
            IParam.DataDepotVolSmile.htRuggedVolSmile1.Count == 0 ||
            IParam.DataDepotVolSmile.htRuggedVolSmile2.Count == 0 ||
            bForceReload)
        {
            try
            {
                //reset internal data first
                dATMVolSmile1 = 0;
                dATMVolSmile2 = 0;
                dVHSIVolSmile1 = 0;
                dVHSIVolSmile2 = 0;
                htVolSmile1 = new Hashtable();
                htVolSmile2 = new Hashtable();
                htRuggedVolSmile1 = new Hashtable();
                htRuggedVolSmile2 = new Hashtable();
                dMthStartVolSmileTTM = 0;
                dMthStartATMVolSmile1 = 0;
                htMthStartVolSmile1 = new Hashtable();
                lhtScVolSmile1 = new SortedList();


                HistoricalDataHelper hdh = HistoricalDataHelper.Instance;
                using (StreamReader sr = new StreamReader(IParam.RawDataFut1FilePath))
                {
                    string sLine;
                    sLine = sr.ReadLine();
                    sLine = sr.ReadLine();
                    dtVolSmileDate = SymbolCodeHelper.ConvertYYYYMMDD(sLine);
                    sr.Close;
                }
                using (StreamReader sr = new StreamReader(IParam.VolSmileMthStartFilePath))
                {
                    string sLine;
                    while ((sLine = sr.ReadLine()) != null)
                    {
                        string[] sParts = sLine.Split('\t');
                        if (sParts.Length < 2) return;
                        if (sParts[0].CompareTo("ATM") == 0) dMthStartATMVolSmile1 = Math.Round(Double.Parse(sParts[1]) / IParam.OPTSTRIKEINTVL, 0) * IParam.OPTSTRIKEINTVL;
                        else if (sParts[0].CompareTo("DATE") == 0)
                        {
                            dtMthStartDate = SymbolCodeHelper.ConvertYYYYMMDD(sParts[1]);
                            dMthStartVolSmileTTM = hdh.GetCalendarTTMHK(SymbolCodeHelper.ConvertYYYYMMDD(sParts[1]), 1);
                        }
                        else if (sParts[0].CompareTo("VHSI") == 0) continue;
                        else
                        {
                            //ignore if the strike price is invalid
                            int iStrike = int.Parse(sParts[0]);
                            if (iStrike % IParam.VOLSMILEINTERVAL == 0)
                                htMthStartVolSmile1.Add(iStrike, Double.Parse(sParts[1]));
                        }
                    }
                    sr.Close;
                }
                using (StreamReader sr = new StreamReader(IParam.VolSmile1FilePath))
                {
                    string sLine;
                    while ((sLine = sr.ReadLine()) != null)
                    {
                        string[] sParts = sLine.Split('\t');
                        if (sParts.Length < 2) return;
                        if (sParts[0].CompareTo("ATM") == 0) dATMVolSmile1 = Math.Round(Double.Parse(sParts[1]) / IParam.OPTSTRIKEINTVL, 0) * IParam.OPTSTRIKEINTVL;
                        else if (sParts[0].CompareTo("VHSI") == 0) dVHSIVolSmile1 = Double.Parse(sParts[1]);
                        else
                        {
                            //ignore if the strike price is invalid
                            int iStrike = int.Parse(sParts[0]);
                            if (iStrike % IParam.VOLSMILEINTERVAL == 0)
                                htVolSmile1.Add(iStrike, Double.Parse(sParts[1]));
                        }
                    }
                    sr.Close;
                }
                using (StreamReader sr = new StreamReader(IParam.VolSmile2FilePath))
                {
                    string sLine;
                    while ((sLine = sr.ReadLine()) != null)
                    {
                        string[] sParts = sLine.Split('\t');
                        if (sParts.Length < 2) return;
                        if (sParts[0].CompareTo("ATM") == 0) dATMVolSmile2 = Math.Round(Double.Parse(sParts[1]) / IParam.OPTSTRIKEINTVL, 0) * IParam.OPTSTRIKEINTVL;
                        else if (sParts[0].CompareTo("VHSI") == 0) dVHSIVolSmile2 = Double.Parse(sParts[1]);
                        else
                        {
                            //ignore if the strike price is invalid
                            int iStrike = int.Parse(sParts[0]);
                            if (iStrike % IParam.VOLSMILEINTERVAL == 0)
                                htVolSmile2.Add(iStrike, Double.Parse(sParts[1]));
                        }
                    }
                    sr.Close;
                }
                for (int i = 0; i < IParam.SCENARIOMAX; i++)
                {
                    Hashtable htScVolSmile1 = new Hashtable();
                    int iScATMPrice = 0;
                    string sTmpFileNm = IParam.VolSmileScenarioPart1FilePath + i + IParam.VolSmileScenarioPart2FilePath;
                    using (StreamReader sr = new StreamReader(sTmpFileNm))
                    {
                        string sLine;
                        while ((sLine = sr.ReadLine()) != null)
                        {
                            string[] sParts = sLine.Split('\t');
                            if (sParts.Length < 2) return;
                            if (sParts[0].CompareTo("ATM") == 0)
                                iScATMPrice = (int)Math.Floor(Double.Parse(sParts[1]));
                            else if (sParts[0].CompareTo("VHSI") == 0) continue;
                            else
                            {
                                //ignore if the strike price is invalid
                                int iStrike = int.Parse(sParts[0]);
                                if (iStrike % IParam.VOLSMILEINTERVAL == 0)
                                    htScVolSmile1.Add(iStrike, Double.Parse(sParts[1]));
                            }
                        }
                        sr.Close;
                    }
                    lhtScVolSmile1.Add(iScATMPrice, htScVolSmile1);
                }
                using (StreamReader sr = new StreamReader(IParam.VolSmile1RuggedFilePath))
                {
                    string sLine;
                    while ((sLine = sr.ReadLine()) != null)
                    {
                        string[] sParts = sLine.Split('\t');
                        if (sParts.Length < 2) return;
                        {
                            //ignore if the strike price is invalid
                            int iStrike = int.Parse(sParts[0]);
                            if (iStrike % IParam.VOLSMILEINTERVAL == 0)
                                htRuggedVolSmile1.Add(iStrike, Double.Parse(sParts[1]));
                        }
                    }
                    sr.Close;
                }
                using (StreamReader sr = new StreamReader(IParam.VolSmile2RuggedFilePath))
                {
                    string sLine;
                    while ((sLine = sr.ReadLine()) != null)
                    {
                        string[] sParts = sLine.Split('\t');
                        if (sParts.Length < 2) return;
                        {
                            //ignore if the strike price is invalid
                            int iStrike = int.Parse(sParts[0]);
                            if (iStrike % IParam.VOLSMILEINTERVAL == 0)
                                htRuggedVolSmile2.Add(iStrike, Double.Parse(sParts[1]));
                        }
                    }
                    sr.Close;
                }

                //store it to memory
                IParam.DataDepotVolSmile.dtVolSmileDate = dtVolSmileDate;
                IParam.DataDepotVolSmile.dATMVolSmile1 = dATMVolSmile1;
                IParam.DataDepotVolSmile.dVHSIVolSmile1 = dVHSIVolSmile1;
                IParam.DataDepotVolSmile.htVolSmile1 = (Hashtable)htVolSmile1.Clone();
                IParam.DataDepotVolSmile.htRuggedVolSmile1 = (Hashtable)htRuggedVolSmile1.Clone();
                IParam.DataDepotVolSmile.dATMVolSmile2 = dATMVolSmile2;
                IParam.DataDepotVolSmile.dVHSIVolSmile2 = dVHSIVolSmile2;
                IParam.DataDepotVolSmile.htVolSmile2 = (Hashtable)htVolSmile2.Clone();
                IParam.DataDepotVolSmile.htRuggedVolSmile2 = (Hashtable)htRuggedVolSmile2.Clone();

                IParam.DataDepotVolSmile.dMthStartVolSmileTTM = dMthStartVolSmileTTM;
                IParam.DataDepotVolSmile.dtMthStartDate = dtMthStartDate;
                IParam.DataDepotVolSmile.dMthStartATMVolSmile1 = dMthStartATMVolSmile1;
                IParam.DataDepotVolSmile.htMthStartVolSmile1 = (Hashtable)htMthStartVolSmile1.Clone();
                IParam.DataDepotVolSmile.lhtScVolSmile1 = (SortedList)lhtScVolSmile1.Clone();

            }
            catch (Exception e)
            {
                Console.WriteLine("Volatility smile file could not be read:");
                Console.WriteLine(e.Message);
                Environment.Exit(0);
            }
        }
        else
        {
            dtVolSmileDate = IParam.DataDepotVolSmile.dtVolSmileDate;
            dATMVolSmile1 = IParam.DataDepotVolSmile.dATMVolSmile1;
            dVHSIVolSmile1 = IParam.DataDepotVolSmile.dVHSIVolSmile1;
            htVolSmile1 = (Hashtable)IParam.DataDepotVolSmile.htVolSmile1.Clone();
            htRuggedVolSmile1 = (Hashtable)IParam.DataDepotVolSmile.htRuggedVolSmile1.Clone();
            dATMVolSmile2 = IParam.DataDepotVolSmile.dATMVolSmile2;
            dVHSIVolSmile2 = IParam.DataDepotVolSmile.dVHSIVolSmile2;
            htVolSmile2 = (Hashtable)IParam.DataDepotVolSmile.htVolSmile2.Clone();
            htRuggedVolSmile2 = (Hashtable)IParam.DataDepotVolSmile.htRuggedVolSmile2.Clone();
            dMthStartVolSmileTTM = IParam.DataDepotVolSmile.dMthStartVolSmileTTM;
            dtMthStartDate = IParam.DataDepotVolSmile.dtMthStartDate;
            dMthStartATMVolSmile1 = IParam.DataDepotVolSmile.dMthStartATMVolSmile1;
            htMthStartVolSmile1 = (Hashtable)IParam.DataDepotVolSmile.htMthStartVolSmile1.Clone();
            lhtScVolSmile1 = (SortedList)IParam.DataDepotVolSmile.lhtScVolSmile1.Clone();
        }
    }
    public static void GenVolSmile_ImplPDF(RuleBasedDayTradeSys rbdts)
    {
        try
        {
            HistoricalDataHelper hdh = HistoricalDataHelper.Instance;

            double dATMPx1 = double.NaN;
            double dATMPx2 = double.NaN;
            double dTTM_VS1 = double.NaN; //TTM for the option prices in the volatility smile file
            double dTTM_VS2 = double.NaN; //TTM for the option prices in the volatility smile file
            Hashtable htCallPrice1 = new Hashtable();
            Hashtable htPutPrice1 = new Hashtable();
            Hashtable htCallIV1 = new Hashtable();
            Hashtable htPutIV1 = new Hashtable();
            Hashtable htUnifiedIV1 = new Hashtable(); //combining call and put
            Hashtable htCallPrice2 = new Hashtable();
            Hashtable htPutPrice2 = new Hashtable();
            Hashtable htCallIV2 = new Hashtable();
            Hashtable htPutIV2 = new Hashtable();
            Hashtable htUnifiedIV2 = new Hashtable(); //combining call and put

            using (StreamReader sr = new StreamReader(IParam.RawDataOpt1FilePath))
            {
                string sLine;
                while ((sLine = sr.ReadLine()) != null)
                {
                    string[] sParts = sLine.Split(',');
                    if (sParts.Length < 2) continue; //sth missing on this row, skip it
                    if (sParts[2].CompareTo("") == 0) continue; //no strike price

                    //C,Jun-12,8400,-,-,
                    //P,Jun-12,8400,1,2,
                    double dBidPx = double.NaN, dAskPx = double.NaN;
                    double dAvgPx = double.NaN;

                    if (!double.TryParse(sParts[3], out dBidPx) || !double.TryParse(sParts[4], out dAskPx))
                        continue;  //prices missing on this row, skip it

                    if (Math.Abs(dAskPx) < 1 || Math.Abs(dBidPx) < 1) continue;
                    if (dAskPx - dBidPx > IParam.MAXVOLSMILEBIDASKSPRD) continue; //mid-price not reliable when the spread is so large

                    dAvgPx = (dBidPx + dAskPx) / 2;

                    if (sParts[0][0] == 'C') htCallPrice1.Add(double.Parse(sParts[2]), dAvgPx);
                    else if (sParts[0][0] == 'P') htPutPrice1.Add(double.Parse(sParts[2]), dAvgPx);
                }
                sr.Close;
            }
            using (StreamReader sr = new StreamReader(IParam.RawDataOpt2FilePath))
            {
                string sLine;
                while ((sLine = sr.ReadLine()) != null)
                {
                    string[] sParts = sLine.Split(',');
                    if (sParts.Length < 2) continue; //sth missing on this row, skip it
                    if (sParts[2].CompareTo("") == 0) continue; //no strike price

                    //C,Jun-12,8400,-,-,
                    //P,Jun-12,8400,1,2,
                    double dBidPx = double.NaN , dAskPx = double.NaN;
                    double dAvgPx = double.NaN;

                    if (!double.TryParse(sParts[3], out dBidPx) || !double.TryParse(sParts[4], out dAskPx))
                        continue;  //prices missing on this row, skip it

                    if (dAskPx - dBidPx > IParam.MAXVOLSMILEBIDASKSPRD) continue; //mid-price not reliable when the spread is so large

                    dAvgPx = (dBidPx + dAskPx) / 2;

                    if (sParts[0][0] == 'C') htCallPrice2.Add(double.Parse(sParts[2]),dAvgPx);
                    else if (sParts[0][0] == 'P') htPutPrice2.Add(double.Parse(sParts[2]), dAvgPx);
                }
               sr.Close;
            }
            using (StreamReader sr = new StreamReader(IParam.RawDataFut1FilePath))
            {
                string sLine;
                sLine = sr.ReadLine();
                dATMPx1 = double.Parse(sLine);
                sLine = sr.ReadLine();
                DateTime dtDataDate = SymbolCodeHelper.ConvertYYYYMMDD(sLine);
                dTTM_VS1 = hdh.GetCalendarTTMHK(dtDataDate, 1);
                sr.Close;
            }
            using (StreamReader sr = new StreamReader(IParam.RawDataFut2FilePath))
            {
                string sLine;
                sLine = sr.ReadLine();
                dATMPx2 = double.Parse(sLine);
                sLine = sr.ReadLine();
                DateTime dtDataDate = SymbolCodeHelper.ConvertYYYYMMDD(sLine);
                dTTM_VS2 = hdh.GetCalendarTTMHK(dtDataDate, 2);
                sr.Close;
            }

            //ok, now calculate the implied volatilities
            foreach (DictionaryEntry de in htCallPrice1)
            {
                htCallIV1.Add(de.Key,
                BlackScholesAlt.CalcImpliedVolatility(true, dATMPx1, (double)(de.Key), IParam.rf, dTTM_VS1, (double)(de.Value)));
            }
            foreach (DictionaryEntry de in htCallPrice2)
            {
                htCallIV2.Add(de.Key,
                BlackScholesAlt.CalcImpliedVolatility(true, dATMPx2, (double)(de.Key), IParam.rf, dTTM_VS2, (double)(de.Value)));
            }
            foreach (DictionaryEntry de in htPutPrice1)
            {
                htPutIV1.Add(de.Key,
                BlackScholesAlt.CalcImpliedVolatility(false, dATMPx1, (double)(de.Key), IParam.rf, dTTM_VS1, (double)(de.Value)));
            }
            foreach (DictionaryEntry de in htPutPrice2)
            {
                htPutIV2.Add(de.Key,
                BlackScholesAlt.CalcImpliedVolatility(false, dATMPx2, (double)(de.Key), IParam.rf, dTTM_VS2, (double)(de.Value)));
            }

            //getting our unified volatility smile
            object[] oUnifiedStrikes1 = new object[htCallIV1.Keys.Count + htPutIV1.Keys.Count];
            htCallIV1.Keys.CopyTo(oUnifiedStrikes1, 0);
            htPutIV1.Keys.CopyTo(oUnifiedStrikes1, htCallIV1.Keys.Count);
            Array.Sort(oUnifiedStrikes1);

            object[] oUnifiedStrikes2 = new object[htCallIV2.Keys.Count + htPutIV2.Keys.Count];
            htCallIV2.Keys.CopyTo(oUnifiedStrikes2, 0);
            htPutIV2.Keys.CopyTo(oUnifiedStrikes2, htCallIV2.Keys.Count);
            Array.Sort(oUnifiedStrikes2);

            for (int i = 0; i < oUnifiedStrikes1.Length; i++)
            {
                double dCallIV = double.NaN;
                double dPutIV = double.NaN;

                if (htCallIV1.ContainsKey(oUnifiedStrikes1[i])) dCallIV = (double)htCallIV1[oUnifiedStrikes1[i]];
                if (htPutIV1.ContainsKey(oUnifiedStrikes1[i])) dPutIV = (double)htPutIV1[oUnifiedStrikes1[i]];

                if (!htUnifiedIV1.ContainsKey((int)(double)(oUnifiedStrikes1[i])))
                {
                    if (!double.IsNaN(dCallIV) && !double.IsNaN(dPutIV) && (int)(double)(oUnifiedStrikes1[i]) < dATMPx1)
                        htUnifiedIV1.Add((int)(double)(oUnifiedStrikes1[i]), dPutIV);
                    else if (!double.IsNaN(dCallIV) && !double.IsNaN(dPutIV) && (int)(double)(oUnifiedStrikes1[i]) > dATMPx1)
                        htUnifiedIV1.Add((int)(double)(oUnifiedStrikes1[i]), dCallIV);
                    else if (!double.IsNaN(dCallIV) && double.IsNaN(dPutIV))
                        htUnifiedIV1.Add((int)(double)(oUnifiedStrikes1[i]), dCallIV);
                    else if (double.IsNaN(dCallIV) && !double.IsNaN(dPutIV))
                        htUnifiedIV1.Add((int)(double)(oUnifiedStrikes1[i]), dPutIV);
                }
            }

            for (int i = 0; i < oUnifiedStrikes2.Length; i++)
            {
                double dCallIV = double.NaN;
                double dPutIV = double.NaN;

                if (htCallIV2.ContainsKey(oUnifiedStrikes2[i])) dCallIV = (double)htCallIV2[oUnifiedStrikes2[i]];
                if (htPutIV2.ContainsKey(oUnifiedStrikes2[i])) dPutIV = (double)htPutIV2[oUnifiedStrikes2[i]];

                if (!htUnifiedIV2.ContainsKey((int)(double)(oUnifiedStrikes2[i])))
                {
                    if (!double.IsNaN(dCallIV) && !double.IsNaN(dPutIV) && (int)(double)(oUnifiedStrikes2[i]) < dATMPx2)
                        htUnifiedIV2.Add((int)(double)(oUnifiedStrikes2[i]), dPutIV);
                    else if (!double.IsNaN(dCallIV) && !double.IsNaN(dPutIV) && (int)(double)(oUnifiedStrikes2[i]) > dATMPx2)
                        htUnifiedIV2.Add((int)(double)(oUnifiedStrikes2[i]), dCallIV);
                    else if (!double.IsNaN(dCallIV) && double.IsNaN(dPutIV))
                        htUnifiedIV2.Add((int)(double)(oUnifiedStrikes2[i]), dCallIV);
                    else if (double.IsNaN(dCallIV) && !double.IsNaN(dPutIV))
                        htUnifiedIV2.Add((int)(double)(oUnifiedStrikes2[i]), dPutIV);
                }
            }

            //before smoothing, output the rugged volatility smile
            using (StreamWriter fout = new StreamWriter(IParam.VolSmile1RuggedFilePath, false))
            {
                object[] keys = new object[htUnifiedIV1.Keys.Count];
                htUnifiedIV1.Keys.CopyTo(keys, 0);
                Array.Sort(keys);
                for (int i = 0; i < keys.Length; i++)
                {
                    fout.WriteLine(((int)(keys[i])).ToString() + "\t" +
                                   ((double)(htUnifiedIV1[keys[i]]) * 100).ToString());
                }
                fout.Close;
            }
            using (StreamWriter fout = new StreamWriter(IParam.VolSmile2RuggedFilePath, false))
            {
                object[] keys = new object[htUnifiedIV2.Keys.Count];
                htUnifiedIV2.Keys.CopyTo(keys, 0);
                Array.Sort(keys);
                for (int i = 0; i < keys.Length; i++)
                {
                    fout.WriteLine(((int)(keys[i])).ToString() + "\t" +
                                   ((double)(htUnifiedIV2[keys[i]]) * 100).ToString());
                }
                fout.Close;
            }



            //output results only if htUnifiedIV has enough elements,
            //otherwise the good old data will be overwritten by the new and incomplete ones, esp before 9:30
            if (htUnifiedIV1.Count > IParam.MINPTVOLSMILE && VolSmileSmoothing(1, ref htUnifiedIV1))
            {
                using (StreamWriter fout = new StreamWriter(IParam.VolSmile1FilePath, false))
                {
                    fout.WriteLine("ATM\t" + dATMPx1.ToString());
                    fout.WriteLine("VHSI\t" + rbdts.LatestVHSI.ToString());
                    object[] keys = new object[htUnifiedIV1.Keys.Count];
                    htUnifiedIV1.Keys.CopyTo(keys, 0);
                    Array.Sort(keys);
                    for (int i = 0; i < keys.Length; i++)
                    {
                        fout.WriteLine(((int)(keys[i])).ToString() + "\t" +
                                       ((double)(htUnifiedIV1[keys[i]]) * 100).ToString());
                    }
                    fout.Close;
                }
            }
            if (htUnifiedIV2.Count > IParam.MINPTVOLSMILE && VolSmileSmoothing(2, ref htUnifiedIV2))
            {
                using (StreamWriter fout = new StreamWriter(IParam.VolSmile2FilePath, false))
                {
                    fout.WriteLine("ATM\t" + dATMPx2.ToString());
                    fout.WriteLine("VHSI\t" + rbdts.LatestVHSI.ToString());
                    object[] keys = new object[htUnifiedIV2.Keys.Count];
                    htUnifiedIV2.Keys.CopyTo(keys, 0);
                    Array.Sort(keys);
                    for (int i = 0; i < keys.Length; i++)
                    {
                        fout.WriteLine(((int)(keys[i])).ToString() + "\t" +
                                       ((double)(htUnifiedIV2[keys[i]]) * 100).ToString());
                    }
                    fout.Close;
                }
            }

            Hashtable htPDF1d = new Hashtable();
            Hashtable htPDF2d = new Hashtable();

            //calculate the implied PDF prob density function for the 1st front month
            using (StreamWriter fout = new StreamWriter(IParam.VSImpliedPDF1FilePath, false))
            {
                BlackScholesAlt.GetRNPDF_fromVolSmile(htUnifiedIV1, dATMPx1, dTTM_VS1, ref htPDF1d);

                object[] keyPDF1d = new object[htPDF1d.Keys.Count];
                htPDF1d.Keys.CopyTo(keyPDF1d, 0);
                Array.Sort(keyPDF1d);

                for (int i = 0; i < keyPDF1d.Length; i++) //all items
                {
                    fout.Write(Math.Round((double)keyPDF1d[i],0));
                    fout.Write("\t");
                    fout.WriteLine(string.Format("{0:0.00000000000000000000}", (double)htPDF1d[keyPDF1d[i]]));
                }

                fout.Close;
            }
            //calculate the implied PDF prob density function for the 2nd front month
            using (StreamWriter fout = new StreamWriter(IParam.VSImpliedPDF2FilePath, false))
            {
                BlackScholesAlt.GetRNPDF_fromVolSmile(htUnifiedIV2, dATMPx2, dTTM_VS2, ref htPDF2d);

                object[] keyPDF2d = new object[htPDF2d.Keys.Count];
                htPDF2d.Keys.CopyTo(keyPDF2d, 0);
                Array.Sort(keyPDF2d);

                for (int i = 0; i < keyPDF2d.Length; i++) //all items
                {
                    fout.Write(Math.Round((double)keyPDF2d[i],0));
                    fout.Write("\t");
                    fout.WriteLine(string.Format("{0:0.00000000000000000000}", (double)htPDF2d[keyPDF2d[i]]));
                }

                fout.Close;
            }


            //collect statistics about the RNPDF
            using (StreamWriter fout = new StreamWriter(IParam.ImpliedRNPDFStatFilePath, false))
            {
                StatHelper sh1 = new StatHelper(htPDF1d.Count);
                StatHelper sh1PxChgScaled = new StatHelper(htPDF1d.Count);
                StatHelper sh2 = new StatHelper(htPDF2d.Count);
                StatHelper sh2PxChgScaled = new StatHelper(htPDF2d.Count);

                if (htPDF1d.Count > 0)
                {
                    foreach (DictionaryEntry de in htPDF1d)
                    {
                        sh1.Add(           (double)de.Key,
                                           (long)((double)de.Value * 1000000));
                        sh1PxChgScaled.Add(Math.Round((double)de.Key,0) / IParam.KURTOSISSCALINGFACTOR,
                                           (long)((double)de.Value * 1000000));
                    }
                }
                if (htPDF2d.Count > 0)
                {
                    foreach (DictionaryEntry de in htPDF2d)
                    {
                        sh2.Add(           (double)de.Key,
                                           (long)((double)de.Value * 1000000));
                        sh2PxChgScaled.Add(Math.Round((double)de.Key, 0) / IParam.KURTOSISSCALINGFACTOR,
                                           (long)((double)de.Value * 1000000));
                    }
                }

                fout.Write("FM1\t");
                fout.Write("SD\t");
                fout.Write((int)sh1.Stdev);
                fout.Write("\t");
                fout.Write("SKW\t");
                fout.Write(Math.Round(sh1PxChgScaled.Skewness,2));
                fout.Write("\tK+/K-\t");
                fout.Write(Math.Round(sh1PxChgScaled.KurtosisPve/sh1PxChgScaled.KurtosisNve*100, 2));
                fout.Write("\tK\t");
                fout.Write(Math.Round(sh1PxChgScaled.Kurtosis, 2));
                fout.Write("\tK+\t");
                fout.Write(Math.Round(sh1PxChgScaled.KurtosisPve, 2));
                fout.Write("\tK-\t");
                fout.Write(Math.Round(sh1PxChgScaled.KurtosisNve, 2));
                fout.WriteLine("");

                fout.Write("FM2\t");
                fout.Write("SD\t");
                fout.Write((int)sh2.Stdev);
                fout.Write("\t");
                fout.Write("SKW\t");
                fout.Write(Math.Round(sh2PxChgScaled.Skewness, 2));
                fout.Write("\tK+/K-\t");
                fout.Write(Math.Round(sh2PxChgScaled.KurtosisPve / sh2PxChgScaled.KurtosisNve * 100, 2));
                fout.Write("\tK\t");
                fout.Write(Math.Round(sh2PxChgScaled.Kurtosis, 2));
                fout.Write("\tK+\t");
                fout.Write(Math.Round(sh2PxChgScaled.KurtosisPve, 2));
                fout.Write("\tK-\t");
                fout.Write(Math.Round(sh2PxChgScaled.KurtosisNve, 2));
                fout.WriteLine("");

                //--------------------------------------------------------

                fout.Close;
            }

            
        }
        catch (Exception e)
        {
            Console.WriteLine("Volatility smile file could not be generated:");
            Console.WriteLine(e.Message);
            Environment.Exit(0);
        }
    }

    private void UpdateHSIOHLC(double dDO, double dDH, double dDL, double dDC, double dTTDE)
    {
        if (double.IsNaN(dDO) || Math.Abs(dDO) < 0.00001 ||
            double.IsNaN(dDH) || Math.Abs(dDH) < 0.00001 ||
            double.IsNaN(dDL) || Math.Abs(dDL) < 0.00001 ||
            double.IsNaN(dDC) || Math.Abs(dDC) < 0.00001) return;

        daDO[iDHLCPtr] = dDO;
        daDH[iDHLCPtr] = dDH;
        daDL[iDHLCPtr] = dDL;
        daDC[iDHLCPtr] = dDC;

        //Method 0 / 1
        if (IParam.ENABLEMETH0 || IParam.ENABLEMETH1)
        {
            if (Math.Abs(daDC[DHLC_BChk(iDHLCPtr - 1)]) > 0.00001)
            {
                daTR[iDHLCPtr] = Math.Max((dDH - dDL), Math.Abs(daDC[DHLC_BChk(iDHLCPtr - 1)] - dDL));
                daTR[iDHLCPtr] = Math.Max(Math.Abs(daDC[DHLC_BChk(iDHLCPtr - 1)] - dDH), daTR[iDHLCPtr]);
            }
            else daTR[iDHLCPtr] = dDH - dDL;

            if (Math.Abs(daATR[DHLC_BChk(iDHLCPtr - 1)]) > 0.00001)
                daATR[iDHLCPtr] = (1 - dM1alpha + dM1alpha * dTTDE) * daATR[DHLC_BChk(iDHLCPtr - 1)] + dM1alpha * (1 - dTTDE) * daTR[iDHLCPtr];
            else
                daATR[iDHLCPtr] = daTR[iDHLCPtr];

            double dATRPct = daATR[iDHLCPtr] / daDC[iDHLCPtr];

            //Method 0
            if (IParam.ENABLEMETH0)
            {
                daVol0QM[iDHLCPtr] = (24 * dATRPct * dATRPct + 9 * dATRPct + 0.08) * 100;

                {
                    double a = 0.0000397815;
                    double b = -0.0103688087;
                    double c = 1.6101008055;
                    double d = -0.0358991661;
                    double dATRPct1000 = dATRPct * 1000;

                    daVol0aCM[iDHLCPtr] = (a * dATRPct1000 * dATRPct1000 * dATRPct1000 +
                                           b * dATRPct1000 * dATRPct1000 +
                                           c * dATRPct1000 +
                                           d);
                }
            }

            //Method 1
            if (IParam.ENABLEMETH1)
            {
                if (!double.IsNaN(dATRPct)) daVol1QM[iDHLCPtr] = (18 * dATRPct * dATRPct + 9 * dATRPct + 0.1) * 100;
                else daVol1QM[iDHLCPtr] = dVHSILatest;

                if (Math.Abs(daVol1QM[DHLC_BChk(iDHLCPtr - 1)]) > 0.00001 && !double.IsNaN(dVHSILatest))
                    daVol1EC[iDHLCPtr] = dVolECalpha * (dVHSILatest - daVol1QM[DHLC_BChk(iDHLCPtr - 1)]) + (1 - dVolECalpha) * daVol1EC[DHLC_BChk(iDHLCPtr - 1)];
                else daVol1EC[iDHLCPtr] = 0;

                daVol1QMEC[iDHLCPtr] = daVol1QM[iDHLCPtr] + daVol1EC[iDHLCPtr];

            }
        }

        //Method 3
        if (IParam.ENABLEMETH3)
        {
            double dDt = Math.Log(daDH[iDHLCPtr]) - Math.Log(daDL[iDHLCPtr]);
            double dDt2 = Math.Log(daDC[iDHLCPtr]) - Math.Log(daDO[iDHLCPtr]);

            daVol3[iDHLCPtr] = 0.5 * dDt * dDt - (2 * Math.Log(2) - 1) * dDt2 * dDt2;
            if (daVol3EMA[DHLC_BChk(iDHLCPtr - 1)] > 0.00001)
                daVol3EMA[iDHLCPtr] = dM3alpha * (1 - dTTDE) * daVol3[iDHLCPtr] + (1 - dM3alpha + dM3alpha * dTTDE) * daVol3EMA[DHLC_BChk(iDHLCPtr - 1)];
            else
                daVol3EMA[iDHLCPtr] = daVol3[iDHLCPtr];

            daVol3EMALM[iDHLCPtr] = (Math.Sqrt(daVol3EMA[iDHLCPtr]) * 100 * 25 - 25) * 2.2 / 3 + 26;

            if (Math.Abs(daVol3EMALM[DHLC_BChk(iDHLCPtr - 1)]) > 0.00001 && !double.IsNaN(dVHSILatest))
                daVol3EC[iDHLCPtr] = dVolECalpha * (dVHSILatest - daVol3EMALM[DHLC_BChk(iDHLCPtr - 1)]) + (1 - dVolECalpha) * daVol3EC[DHLC_BChk(iDHLCPtr - 1)];
            else daVol3EC[iDHLCPtr] = 0;

            daVol3EMALMEC[iDHLCPtr] = daVol3EMALM[iDHLCPtr] + daVol3EC[iDHLCPtr];
        }
    }

    public void AddHSIDailyOHLC(double dDO, double dDH, double dDL, double dDC, double dTTDE)
    {
        UpdateHSIOHLC(dDO, dDH, dDL, dDC, dTTDE);
        iDHLCPtr = DHLC_BChk(iDHLCPtr + 1);

        if (IParam.ENABLEMETH1 && IParam.ENABLEMETH3)
            daVol13[DHLC_BChk(iDHLCPtr - 1)] = HistVolPct_13;
    }
    public void AddHSIIntradayOHLC(double dDO, double dDH, double dDL, double dCP, double dTTDE)
    {
        UpdateHSIOHLC(dDO, dDH, dDL, dCP, dTTDE);
    }

    public double EIV0 { get { return daVol0QM[DHLC_BChk(iDHLCPtr - 1)]; } }
    public double EIV0_est { get { return daVol0QM[iDHLCPtr]; } }
    public double EIV0a { get { return daVol0aCM[DHLC_BChk(iDHLCPtr - 1)]; } }
    public double HistVolPct_1 { get { return daVol1QMEC[DHLC_BChk(iDHLCPtr - 1)]; } }
    public double HistVolPct_1_est { get { return daVol1QMEC[iDHLCPtr]; } }
    public double HistVolPct_3 { get { return daVol3EMALMEC[DHLC_BChk(iDHLCPtr - 1)]; } }
    public double HistVolPct_3_est { get { return daVol3EMALMEC[iDHLCPtr]; } }
    public double HistVolPct_13 { get {
        return (daVol1QMEC[DHLC_BChk(iDHLCPtr - 1)] + daVol3EMALMEC[DHLC_BChk(iDHLCPtr - 1)]) / 2; } }
    public double HistVolPct_13_est { get {
            return (daVol1QMEC[iDHLCPtr] + daVol3EMALMEC[iDHLCPtr]) / 2; } }
    public double HistVolPct_13_UB_est
    {
        get
        {
            double dVol13_0 = HistVolPct_13_est;
            double dVol13_1 = HistVolPct_13_est;
            double dVol13_2 = HistVolPct_13_est;

            if (Math.Abs(daVol13[DHLC_BChk(iDHLCPtr - 1)]) > 0.00001)
                dVol13_1 = daVol13[DHLC_BChk(iDHLCPtr - 1)];
            if (Math.Abs(daVol13[DHLC_BChk(iDHLCPtr - 2)]) > 0.00001)
                dVol13_2 = daVol13[DHLC_BChk(iDHLCPtr - 2)];

            return dVol13_0 * 1.05 + Math.Max(dVol13_0 - dVol13_1, 0) + Math.Max(0.5 * (dVol13_0 + dVol13_2 - 2 * dVol13_1), 0);
        }
    }
    public double HistVolPct_13_LB_est
    {
        get
        {
            double dVol13_0 = HistVolPct_13_est;
            double dVol13_1 = HistVolPct_13_est;
            double dVol13_2 = HistVolPct_13_est;

            if (Math.Abs(daVol13[DHLC_BChk(iDHLCPtr - 1)]) > 0.00001)
                dVol13_1 = daVol13[DHLC_BChk(iDHLCPtr - 1)];
            if (Math.Abs(daVol13[DHLC_BChk(iDHLCPtr - 2)]) > 0.00001)
                dVol13_2 = daVol13[DHLC_BChk(iDHLCPtr - 2)];

            return dVol13_0 * 0.95 + Math.Min(dVol13_0 - dVol13_1, 0) + Math.Min(0.5 * (dVol13_0 + dVol13_2 - 2 * dVol13_1), 0);
        }
    }


    public double GetScenarioVolatilityFromModel(double dFutPx1, double dFutPx2, double dStrike, DateTime dtCT, int iFrontMth)
    {
        double dRtnVol = double.NaN;

        HistoricalDataHelper hdh = HistoricalDataHelper.Instance;

        //get the relevant figures according to front-monthness
        double dFutPrice = dFutPx1;
        if (iFrontMth == 1) dFutPrice = dFutPx1;
        else if (iFrontMth == 2) dFutPrice = dFutPx2;

        double dVHSIVolSmile = double.NaN;
        double dATMVolSmile = double.NaN;
        Hashtable htVolSmile = null;
        Hashtable htRuggedVolSmile = null;
        if (iFrontMth == 1)
        {
            dVHSIVolSmile = dVHSIVolSmile1;
            dATMVolSmile = dATMVolSmile1;
            htVolSmile = htVolSmile1;
            htRuggedVolSmile = htRuggedVolSmile1;
        }
        else if (iFrontMth == 2)
        {
            dVHSIVolSmile = dVHSIVolSmile2;
            dATMVolSmile = dATMVolSmile2;
            htVolSmile = htVolSmile2;
            htRuggedVolSmile = htRuggedVolSmile2;
        }

        //new method: accounting for the curling / rolling up of the volatility smile
        if (htVolSmile.ContainsKey((int)dStrike))
        {
            //the rolling up rate 3000 points below or 4000 above ATM
            double a = 64.10308681;
            double b = -22.54235833;
            double c = -52.98359007;
            double d = -31.51438583;
            double e = -7.768533802;
            double f = 52.01569809;
            double g = 0.971764854;
            double h = 0.340564233;
            double i = 0.838342455;
            double j = 0.031324491;

                
            string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(dtCT, 5 / 1440d);
            double dNewTTM = hdh.GetCalendarTTMHK(dtCT, s1FrontMthFutCode);
            if (dNewTTM > 4d / 365d) dNewTTM = dNewTTM * 5 / 7; //using trading days
            double dNewTIME = 1 - dNewTTM;

            double dEstTailChg1 = (g * Math.Pow((dVHSILatest/dVHSIVolSmile)-1,2) +
                                    h * Math.Pow((dVHSILatest/dVHSIVolSmile)-1,1) +
                                    i) * dNewTTM * 365 * j;
            double dEstCumTailChg2 = a * Math.Pow(dNewTIME, 6) +
                                        b * Math.Pow(dNewTIME, 4) +
                                        c * Math.Pow(dNewTIME, 3) +
                                        d * Math.Pow(dNewTIME, 2) +
                                        e * Math.Pow(dNewTIME, 1) +
                                        f;
            double dEstCumTailChgRefPt = dEstTailChg1 + dEstCumTailChg2;


            int iTmp1 = (int)Math.Max(Math.Round((dATMVolSmile-3000)/IParam.VOLSMILEINTERVAL,0)*IParam.VOLSMILEINTERVAL,IParam.VOLSMILEREGENSTART);
            int iTmp2 = (int)Math.Max(Math.Round((dMthStartATMVolSmile1-3000)/IParam.VOLSMILEINTERVAL,0)*IParam.VOLSMILEINTERVAL,IParam.VOLSMILEREGENSTART);
            int iTmp3 = (int)Math.Max(Math.Round(dATMVolSmile / IParam.VOLSMILEINTERVAL, 0) * IParam.VOLSMILEINTERVAL, IParam.VOLSMILEREGENSTART);
            int iTmp4 = (int)Math.Max(Math.Round(dMthStartATMVolSmile1 / IParam.VOLSMILEINTERVAL, 0) * IParam.VOLSMILEINTERVAL, IParam.VOLSMILEREGENSTART);
            double dLatestTailChgFrmMthStart = ((double)htVolSmile[iTmp1] * (double)htMthStartVolSmile1[iTmp4] / (double)htVolSmile[iTmp3])
                                                / (double)htMthStartVolSmile1[iTmp2] - 1;
            double dEstTailChgRefPt = dEstCumTailChgRefPt - dLatestTailChgFrmMthStart;

                
            //the expected IV change at the reference point
            double dEstCurl = 0;
            if (dStrike > dFutPrice)
                dEstCurl = dEstTailChgRefPt * Math.Min((dStrike - dFutPrice) / 4000, 1d);
            else if (dStrike < dFutPrice)
                dEstCurl = dEstTailChgRefPt * Math.Min((dFutPrice - dStrike) / 3000, 1d);


            //adjust from the latest vol smile
            //dATMVolSmile1 = our old ATM
            //dFutPrice = our new ATM
            int iHorShiftedStrike = (int)Math.Max(Math.Min(Math.Round((dStrike - dFutPrice + dATMVolSmile) /
                                                IParam.VOLSMILEINTERVAL, 0) * IParam.VOLSMILEINTERVAL,
                                                IParam.VOLSMILEREGENEND), IParam.VOLSMILEREGENSTART);
            dRtnVol =
                (double)htVolSmile[iHorShiftedStrike] * //horizontal shift from month start
                (dVHSILatest / dVHSIVolSmile) * //then predict the new volatility level and adjust the whole curve
                (1 + dEstCurl); //linear interpolation for curling / rolling up


        }

        return dRtnVol / 100;
    }

    //dStrike: in HSI points
    //iFrontMth: 1 = 1st Front Month etc
    //dTTM: in years
    public double GetLocalVolatility(double dFutPx1, double dFutPx2, double dStrike, DateTime dtCT, int iFrontMth)
    {
        double dRtnVol = double.NaN;

        double dVHSIVolSmile = double.NaN;
        double dATMVolSmile = double.NaN;
        Hashtable htVolSmile = null;
        Hashtable htRuggedVolSmile = null;
        if (iFrontMth == 1)
        {
            dVHSIVolSmile = dVHSIVolSmile1;
            dATMVolSmile = dATMVolSmile1;
            htVolSmile = htVolSmile1;
            htRuggedVolSmile = htRuggedVolSmile1;
        }
        else if (iFrontMth == 2)
        {
            dVHSIVolSmile = dVHSIVolSmile2;
            dATMVolSmile = dATMVolSmile2;
            htVolSmile = htVolSmile2;
            htRuggedVolSmile = htRuggedVolSmile2;
        }


        if (dtCT.CompareTo(dtVolSmileDate) == 0 && htRuggedVolSmile.ContainsKey((int)dStrike))
        {
            dRtnVol = (double)htRuggedVolSmile[(int)dStrike];
        }
        else if (dtCT.CompareTo(dtVolSmileDate) > 0 && lhtScVolSmile1.ContainsKey((int)Math.Floor(dFutPx1)))
        {
            dRtnVol = (double)(((Hashtable)(lhtScVolSmile1[(int)Math.Floor(dFutPx1)]))[(int)dStrike]);
        }
        else if (dtCT.CompareTo(dtVolSmileDate) <= 0 && htVolSmile.ContainsKey((int)dStrike))
        {
            //the latest VHSI is equal to the VHSI in the options data file
            //we must be getting the current volatility smile
            //therefore just directly read off the implied volatility
            dRtnVol = (double)htVolSmile[(int)dStrike];
        }
        else
        {
            return double.NaN;
        }

        return dRtnVol / 100;
    }

    public static void OutputVolSmileMeth(int iFrontMth, string s, bool bAppend)
    {
        using (StreamWriter fout = new StreamWriter(iFrontMth == 1 ?
                                                    IParam.VolSmileMethFilePath1 : IParam.VolSmileMethFilePath2,
                                                    bAppend))
        {
            fout.WriteLine("Front Mth: " + iFrontMth.ToString() + ": " + s);
            fout.Close;
        }
    }

    //returns false if error unacceptable
    //returns true otherwise
    public static bool CheckVolSmileSmoothgEffective(Hashtable htRuggedVolSmile, Hashtable htSmoothVolSmile)
    {
        double dSumErrSq = 0;

        foreach (DictionaryEntry de in htRuggedVolSmile)
        {
            if (htSmoothVolSmile.ContainsKey(de.Key))
                dSumErrSq += Math.Pow((double)(de.Value) * 100 - (double)(htSmoothVolSmile[de.Key]) * 100, 2);
        }

        if (dSumErrSq > htRuggedVolSmile.Count / 3d ) return false;
        else return true;


    }
    public static bool VolSmileSmoothing(int iFrontMth, ref Hashtable htVolSmile)
    {
        OutputVolSmileMeth(iFrontMth, "", false);


        Hashtable htVolSmileLeastSqCubic = VolSmileLeastSqCubic(ref htVolSmile);


        Hashtable htVolSmileAugmented = (Hashtable)htVolSmile.DeepClone();
        //to guide the curve at the high ends
        if (htVolSmileAugmented.ContainsKey(IParam.VOLSMILEREGENEND))
            htVolSmileAugmented[IParam.VOLSMILEREGENEND] = htVolSmileLeastSqCubic[IParam.VOLSMILEREGENEND];
        else
            htVolSmileAugmented.Add(IParam.VOLSMILEREGENEND, htVolSmileLeastSqCubic[IParam.VOLSMILEREGENEND]);



        Hashtable htVolSmileLeastSqQuartic = VolSmileLeastSqQuartic(ref htVolSmileAugmented);
        Hashtable htVolSmileLeastSqQuintic = VolSmileLeastSqQuintic(ref htVolSmileAugmented);
        Hashtable htVolSmileLeastSqSextic = VolSmileLeastSqSextic(ref htVolSmileAugmented);



        if (htVolSmileLeastSqSextic != null && CheckVolSmileSmoothgEffective(htVolSmile, htVolSmileLeastSqSextic))
        {
            OutputVolSmileMeth(iFrontMth, "Least Square (Sextic) used.", true);
            OutputVolSmileMeth(iFrontMth, "Error between rugged and smoothed vol smile is small.", true);
            htVolSmile = (Hashtable)htVolSmileLeastSqSextic.DeepClone();
            return true;
        }
        else if (htVolSmileLeastSqQuintic != null && CheckVolSmileSmoothgEffective(htVolSmile, htVolSmileLeastSqQuintic))
        {
            OutputVolSmileMeth(iFrontMth, "Least Square (Quintic) used.", true);
            OutputVolSmileMeth(iFrontMth, "Error between rugged and smoothed vol smile is small.", true);
            htVolSmile = (Hashtable)htVolSmileLeastSqQuintic.DeepClone();
            return true;
        }
        else if (htVolSmileLeastSqQuartic != null && CheckVolSmileSmoothgEffective(htVolSmile, htVolSmileLeastSqQuartic))
        {
            OutputVolSmileMeth(iFrontMth, "Least Square (Quartic) used.", true);
            OutputVolSmileMeth(iFrontMth, "Error between rugged and smoothed vol smile is small.", true);
            htVolSmile = (Hashtable)htVolSmileLeastSqQuartic.DeepClone();
            return true;
        }
        else if (htVolSmileLeastSqCubic != null && CheckVolSmileSmoothgEffective(htVolSmile, htVolSmileLeastSqCubic))
        {
            OutputVolSmileMeth(iFrontMth, "Least Square (Cubic) used.", true);
            OutputVolSmileMeth(iFrontMth, "Error between rugged and smoothed vol smile is small.", true);
            htVolSmile = (Hashtable)htVolSmileLeastSqCubic.DeepClone();
            return true;
        }
        else
        {
            //try to get a quintic polynomial fit
            if (VolSmileLagrangePolyQuintic(iFrontMth, ref htVolSmileAugmented) && CheckVolSmileSmoothgEffective(htVolSmile, htVolSmileAugmented))
            {
                OutputVolSmileMeth(iFrontMth, "Lagrange Polynomial (Quintic) used.", true);
                OutputVolSmileMeth(iFrontMth, "Error between rugged and smoothed vol smile is small.", true);
                htVolSmile = (Hashtable)htVolSmileAugmented.DeepClone();
                return true;
            }
            else
            {
                //what shape is that??? no choice but to use (linear) interpolation
                Hashtable htVolSmileInterp = null;

                if (htVolSmileLeastSqQuintic != null) htVolSmileInterp = VolSmileInterpolation(htVolSmile, htVolSmileLeastSqQuintic);
                else if (htVolSmileLeastSqQuartic != null) htVolSmileInterp = VolSmileInterpolation(htVolSmile, htVolSmileLeastSqQuartic);
                else if (htVolSmileLeastSqCubic != null) htVolSmileInterp = VolSmileInterpolation(htVolSmile, htVolSmileLeastSqCubic);

                OutputVolSmileMeth(iFrontMth, "Linear Interpolation used.", true);

                if (CheckVolSmileSmoothgEffective(htVolSmile, htVolSmileInterp))
                    OutputVolSmileMeth(iFrontMth, "Error between rugged and smoothed vol smile is small.", true);
                else
                    OutputVolSmileMeth(iFrontMth, "Error between rugged and smoothed vol smile is large.", true);

                htVolSmile = (Hashtable)htVolSmileInterp.DeepClone();
                return true;
            }
        }


    }

    public static Hashtable VolSmileInterpolation(Hashtable htVolSmile, Hashtable htVolSmileLinRgrn)
    {
        Hashtable htVolSmileTmp = new Hashtable();

        object[] oK = new object[htVolSmile.Count];
        htVolSmile.Keys.CopyTo(oK, 0);
        Array.Sort(oK);

        int iJustAbv = 0;

        for (int i = IParam.VOLSMILEREGENSTART; i <= IParam.VOLSMILEREGENEND; i += IParam.VOLSMILEINTERVAL)
        {
            while (i > (int)oK[iJustAbv] && iJustAbv < oK.Length-1) iJustAbv++;

            if (htVolSmile.ContainsKey(i))
            {
                htVolSmileTmp.Add(i, htVolSmile[i]);
            }
            else
            {
                if (iJustAbv == 0 || iJustAbv >= oK.Length - 1)
                {
                    htVolSmileTmp.Add(i, (double)htVolSmileLinRgrn[i]);
                }
                else
                {
                    double dTmp2 =
                        (double)htVolSmile[(int)oK[iJustAbv-1]] * ((double)((int)oK[iJustAbv] - i) / ((int)oK[iJustAbv] - (int)oK[iJustAbv-1])) +
                        (double)htVolSmile[(int)oK[iJustAbv]] * ((double)(i - (int)oK[iJustAbv-1]) / ((int)oK[iJustAbv] - (int)oK[iJustAbv-1]));

                    htVolSmileTmp.Add(i, dTmp2);
                }

            }


        }

        return htVolSmileTmp;
    }


    public static Hashtable VolSmileLeastSqCubic(ref Hashtable htVolSmile)
    {
        LstSquCubicRegr solvr = new LstSquCubicRegr();
        foreach (DictionaryEntry de in htVolSmile) solvr.AddPoints((double)(int)(de.Key), (double)(de.Value));

        double a = solvr.aTerm;
        double b = solvr.bTerm;
        double c = solvr.cTerm;
        double d = solvr.dTerm;

        Hashtable htVolSmileTmp = new Hashtable();

        for (int i = IParam.VOLSMILEREGENSTART; i <= IParam.VOLSMILEREGENEND; i += IParam.VOLSMILEINTERVAL)
        {
            double dTmp = i;
            if (!double.IsNaN(a) && !double.IsNaN(b) && !double.IsNaN(c) && !double.IsNaN(d) &&
                a != 0 && b != 0 && c != 0 && d != 0)
                htVolSmileTmp[i] = a * dTmp * dTmp * dTmp +
                                   b * dTmp * dTmp +
                                   c * dTmp +
                                   d;
        }

        foreach (DictionaryEntry de in htVolSmileTmp)
        {
            if ((double)de.Value < 0) return null;
        }

        if (htVolSmileTmp.Count == 0) return null;

        return htVolSmileTmp;
    }

    public static Hashtable VolSmileLeastSqQuartic(ref Hashtable htVolSmile)
    {
        LstSquQuarticRegr solvr = new LstSquQuarticRegr();
        foreach (DictionaryEntry de in htVolSmile) solvr.AddPoints((double)((int)(de.Key)) / 1000, (double)(de.Value)); //divide 1000 to prevent overflow in the regression solver

        double a = solvr.aTerm;
        double b = solvr.bTerm;
        double c = solvr.cTerm;
        double d = solvr.dTerm;
        double e = solvr.eTerm;

        Hashtable htVolSmileTmp = new Hashtable();

        for (int i = IParam.VOLSMILEREGENSTART; i <= IParam.VOLSMILEREGENEND; i += IParam.VOLSMILEINTERVAL)
        {
            double dTmp = i / 1000d;
            if (!double.IsNaN(a) && !double.IsNaN(b) && !double.IsNaN(c) && !double.IsNaN(d) && !double.IsNaN(e) &&
                a != 0 && b != 0 && c != 0 && d != 0 && e != 0)
                htVolSmileTmp[i] = a * dTmp * dTmp * dTmp * dTmp +
                                   b * dTmp * dTmp * dTmp +
                                   c * dTmp * dTmp +
                                   d * dTmp +
                                   e;
        }

        foreach (DictionaryEntry de in htVolSmileTmp)
        {
            if ((double)de.Value < 0) return null;
        }

        if (htVolSmileTmp.Count == 0) return null;

        return htVolSmileTmp;
    }
    public static Hashtable VolSmileLeastSqQuintic(ref Hashtable htVolSmile)
    {
        LstSquQuinticRegr solvr = new LstSquQuinticRegr();
        foreach (DictionaryEntry de in htVolSmile) solvr.AddPoints((double)((int)(de.Key))/10000, (double)(de.Value)); //divide 1000 to prevent overflow in the regression solver

        double a = solvr.aTerm;
        double b = solvr.bTerm;
        double c = solvr.cTerm;
        double d = solvr.dTerm;
        double e = solvr.eTerm;
        double f = solvr.fTerm;

        Hashtable htVolSmileTmp = new Hashtable();

        for (int i = IParam.VOLSMILEREGENSTART; i <= IParam.VOLSMILEREGENEND; i += IParam.VOLSMILEINTERVAL)
        {
            double dTmp = i / 10000d;
            if (!double.IsNaN(a) && !double.IsNaN(b) && !double.IsNaN(c) && !double.IsNaN(d) && !double.IsNaN(e) && !double.IsNaN(f) &&
                a != 0 && b != 0 && c != 0 && d != 0 && e != 0 && f != 0)
                htVolSmileTmp[i] = a * dTmp * dTmp * dTmp * dTmp * dTmp +
                                   b * dTmp * dTmp * dTmp * dTmp +
                                   c * dTmp * dTmp * dTmp +
                                   d * dTmp * dTmp +
                                   e * dTmp +
                                   f;
        }

        foreach (DictionaryEntry de in htVolSmileTmp)
        {
            if ((double)de.Value < 0) return null;
        }

        if (htVolSmileTmp.Count == 0) return null;

        return htVolSmileTmp;
    }
    public static Hashtable VolSmileLeastSqSextic(ref Hashtable htVolSmile)
    {
        LstSquSexticRegr solvr = new LstSquSexticRegr();
        foreach (DictionaryEntry de in htVolSmile) solvr.AddPoints((double)((int)(de.Key)) / 100000, (double)(de.Value)); //divide 1000 to prevent overflow in the regression solver

        double a = solvr.aTerm;
        double b = solvr.bTerm;
        double c = solvr.cTerm;
        double d = solvr.dTerm;
        double e = solvr.eTerm;
        double f = solvr.fTerm;
        double g = solvr.gTerm;

        Hashtable htVolSmileTmp = new Hashtable();

        for (int i = IParam.VOLSMILEREGENSTART; i <= IParam.VOLSMILEREGENEND; i += IParam.VOLSMILEINTERVAL)
        {
            double dTmp = i / 100000d;
            if (!double.IsNaN(a) && !double.IsNaN(b) && !double.IsNaN(c) && !double.IsNaN(d) && !double.IsNaN(e) && !double.IsNaN(f) && !double.IsNaN(g) &&
                a != 0 && b != 0 && c != 0 && d != 0 && e != 0 && f != 0 && g != 0)
                htVolSmileTmp[i] = a * dTmp * dTmp * dTmp * dTmp * dTmp * dTmp +
                                   b * dTmp * dTmp * dTmp * dTmp * dTmp +
                                   c * dTmp * dTmp * dTmp * dTmp +
                                   d * dTmp * dTmp * dTmp +
                                   e * dTmp * dTmp +
                                   f * dTmp +
                                   g;
        }

        foreach (DictionaryEntry de in htVolSmileTmp)
        {
            if ((double)de.Value < 0) return null;
        }

        if (htVolSmileTmp.Count == 0) return null;

        return htVolSmileTmp;
    }
    public static bool VolSmileLagrangePolyCubic(ref Hashtable htVolSmile)
    {
        //(k1,v1) left point
        //(k2,v2) trough in the smile
        //(k3,v3) right point
        double v1 = 0, v2 = 0, v3 = 0, v4 = 0;
        int k1 = 0, k2 = 0, k3 = 0, k4 = 0;
        int iTroughIdx = 0;

        if (htVolSmile.Count < IParam.MINPTVOLSMILE)
        {
            return false;
        }
        else
        {
            object[] oK = new object[htVolSmile.Count];
            htVolSmile.Keys.CopyTo(oK, 0);
            Array.Sort(oK);

            //locate the trough, (k2,v2)
            for (int i = 0; i < oK.Length; i++)
            {
                if (double.IsNaN(v2) || v2 == 0 || (double)(htVolSmile[oK[i]]) < v2)
                {
                    v2 = (double)(htVolSmile[oK[i]]);
                    k2 = (int)(oK[i]);
                    iTroughIdx = i;
                }
            }

            //locate the left data point, (k1,v1)
            k1 = (int)Math.Round(((int)(oK[0]) + k2) / 2d / IParam.OPTSTRIKEINTVL, 0) * IParam.OPTSTRIKEINTVL;
            while (!htVolSmile.ContainsKey(k1)
                    || k1 < k2) k1 += IParam.OPTSTRIKEINTVL;
            if (k1 == k2) return false;
            v1 = (double)htVolSmile[k1];

            //locate the second left data point, (k4,v4)
            k4 = (int)(Math.Round((double)(k2 + k1) / 2d / IParam.OPTSTRIKEINTVL, 0) * IParam.OPTSTRIKEINTVL);
            while (!htVolSmile.ContainsKey(k4)
                    || k4 < k2 || k4 <= k1) k4 += IParam.OPTSTRIKEINTVL;
            if (k4 == k2) return false;
            v4 = (double)htVolSmile[k4];

            //locate the right data point, (k3,v3)
            k3 = (int)Math.Round(((int)(oK[oK.Length - 1]) + k2) / 2d / IParam.OPTSTRIKEINTVL, 0) * IParam.OPTSTRIKEINTVL;
            while (!htVolSmile.ContainsKey(k3)
                    || k3 > k2) k3 -= IParam.OPTSTRIKEINTVL;
            if (k3 == k2) return false;
            v3 = (double)htVolSmile[k3];


            //---------------------------------------------------------------
            // Lagrange polynomial: Cubic: x^3
            //---------------------------------------------------------------
            double A = v1 / (k1 - k2) / (k1 - k3) / (k1 - k4);
            double B = v2 / (k2 - k1) / (k2 - k3) / (k2 - k4);
            double C = v3 / (k3 - k1) / (k3 - k2) / (k3 - k4);
            double D = v4 / (k4 - k1) / (k4 - k2) / (k4 - k3);

            double a = A + B + C + D;
            double b = -(A + B + C) * k4 - (A + B + D) * k3 - (A + C + D) * k2 - (B + C + D) * k1;
            double c = (A + B) * k3 * k4 + (A + C) * k2 * k4 + (A + D) * k2 * k3 +
                       (B + C) * k1 * k4 + (B + D) * k1 * k3 + (C + D) * k1 * k2;
            double d = -A * k2 * k3 * k4 - B * k1 * k3 * k4 - C * k1 * k2 * k4 - D * k1 * k2 * k3;

            Hashtable htVolSmileTmp = new Hashtable();

            for (int i = IParam.VOLSMILEREGENSTART; i <= IParam.VOLSMILEREGENEND; i += IParam.VOLSMILEINTERVAL)
            {
                double IV = a * Math.Pow((double)i, 3) +
                            b * Math.Pow((double)i, 2) +
                            c * Math.Pow((double)i, 1) +
                            d;
                htVolSmileTmp[i] = IV;
            }

            foreach (DictionaryEntry de in htVolSmileTmp)
                if ((double)de.Value < 0)
                    return false;

            //check convexity
            for (int i = IParam.VOLSMILEREGENSTART;
                     i <= IParam.VOLSMILEREGENEND - 2 * IParam.VOLSMILEINTERVAL;
                     i += IParam.VOLSMILEINTERVAL)
            {
                if ((double)htVolSmileTmp[i+2*IParam.VOLSMILEINTERVAL] - (double)htVolSmileTmp[i+1*IParam.VOLSMILEINTERVAL]
                    <
                    (double)htVolSmileTmp[i+1*IParam.VOLSMILEINTERVAL] - (double)htVolSmileTmp[i])
                    return false;
            }

            //ok, no problem
            htVolSmile = (Hashtable)htVolSmileTmp.DeepClone();


            return true;
        }
    }
    
    public static bool VolSmileLagrangePolyQuintic(int iFrontMth, ref Hashtable htVolSmile)
    {
        //(k1,v1) left point 1
        //(k4,v4) left point 2
        //(k6,v6) left point 3
        //(k2,v2) trough in the smile
        //(k3,v3) right point 1
        //(k5,v5) right point 2
        double v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0, v6 = 0;
        int k1 = 0, k2 = 0, k3 = 0, k4 = 0, k5 = 0, k6 = 0;
        int iTroughIdx = 0;

        object[] oK = new object[htVolSmile.Count];
        htVolSmile.Keys.CopyTo(oK, 0);
        Array.Sort(oK);

        //locate the trough, (k2,v2)
        for (int i = 1; i < oK.Length - 1; i++)
        {
            if (double.IsNaN(v2) || v2 == 0 || (double)(htVolSmile[oK[i]]) < v2)
            {
                v2 = (double)(htVolSmile[oK[i]]);
                k2 = (int)(oK[i]);
                iTroughIdx = i;
            }
        }
            
        //the leftmost data point is used to fix the overall shape of the curve
        k1 = (int)(oK[0]);
        v1 = (double)htVolSmile[k1];

        //the rightmost data point is used to fix the overall shape of the curve
        k5 = (int)(oK[oK.Length - 1]);
        v5 = (double)htVolSmile[k5];

        //locate somewhere a bit higher than mid-point
        double d1 = (double)(int)(oK[1]);
        double d2 = (double)(int)(oK[oK.Length - 2]);
        k4 = (int)(Math.Round((d1 + d2) / 2d / IParam.OPTSTRIKEINTVL, 0) * IParam.OPTSTRIKEINTVL + IParam.OPTSTRIKEINTVL);
        while (!htVolSmile.ContainsKey(k4)
                && k4 < (int)oK[oK.Length - 2]) k4 += IParam.OPTSTRIKEINTVL;
        v4 = (double)htVolSmile[k4];

        //locate a point to the left of k4, (k6,v6)
        k6 = k4 - IParam.OPTSTRIKEINTVL;
        while (!htVolSmile.ContainsKey(k6)
                && k6 > (int)oK[1]) k6 -= IParam.OPTSTRIKEINTVL;
        v6 = (double)htVolSmile[k6];

        //locate a point to the right of k4, (k3,v3)
        k3 = k4 + IParam.OPTSTRIKEINTVL;
        while (!htVolSmile.ContainsKey(k3)
                && k3 < (int)oK[oK.Length - 2]) k3 += IParam.OPTSTRIKEINTVL;
        v3 = (double)htVolSmile[k3];

        if (k2 == k3 || k2 == k4 || k2 == k6 ||
            k3 == k4 || k3 == k6 ||
            k4 == k6)
        {
            if (htVolSmile.Count - 2 == 6) //only 6 pts excluding the boundaries, actual IV from 1-6
            {
                //keep k1, k5
                k2 = (int)oK[2]; v2 = (double)htVolSmile[k2];
                k3 = (int)oK[3]; v3 = (double)htVolSmile[k3];
                k4 = (int)oK[4]; v4 = (double)htVolSmile[k4];
                k6 = (int)oK[5]; v6 = (double)htVolSmile[k6];
            }
            else if (htVolSmile.Count - 2 == 7) //actual IV from 1-7
            {
                //keep k1, k5
                k2 = (int)oK[3]; v2 = (double)htVolSmile[k2];
                k3 = (int)oK[4]; v3 = (double)htVolSmile[k3];
                k4 = (int)oK[5]; v4 = (double)htVolSmile[k4];
                k6 = (int)oK[6]; v6 = (double)htVolSmile[k6];
            }
            else if (htVolSmile.Count - 2 == 8) //actual IV from 1-8
            {
                //keep k1, k5
                k2 = (int)oK[4]; v2 = (double)htVolSmile[k2];
                k3 = (int)oK[5]; v3 = (double)htVolSmile[k3];
                k4 = (int)oK[6]; v4 = (double)htVolSmile[k4];
                k6 = (int)oK[7]; v6 = (double)htVolSmile[k6];
            }
            else if (htVolSmile.Count - 2 == 9) //actual IV from 1-9
            {
                //keep k1, k5
                k2 = (int)oK[5]; v2 = (double)htVolSmile[k2];
                k3 = (int)oK[6]; v3 = (double)htVolSmile[k3];
                k4 = (int)oK[7]; v4 = (double)htVolSmile[k4];
                k6 = (int)oK[8]; v6 = (double)htVolSmile[k6];
            }
            else if (htVolSmile.Count - 2 == 10) //actual IV from 1-10
            {
                //keep k1, k5
                k2 = (int)oK[6]; v2 = (double)htVolSmile[k2];
                k3 = (int)oK[7]; v3 = (double)htVolSmile[k3];
                k4 = (int)oK[8]; v4 = (double)htVolSmile[k4];
                k6 = (int)oK[9]; v6 = (double)htVolSmile[k6];
            }
            else if (htVolSmile.Count - 2 == 11) //actual IV from 1-11
            {
                //keep k1, k5
                k2 = (int)oK[6]; v2 = (double)htVolSmile[k2];
                k3 = (int)oK[8]; v3 = (double)htVolSmile[k3];
                k4 = (int)oK[9]; v4 = (double)htVolSmile[k4];
                k6 = (int)oK[10]; v6 = (double)htVolSmile[k6];
            }
            else if (htVolSmile.Count - 2 > 11) //plenty of points
            {
                //keep k1, k5

                //move k2, keep k3, k4, k6
                bool bNoConflict = false;
                while (!bNoConflict)
                {
                    if (k2 == k3) { k2 = k3 - IParam.OPTSTRIKEINTVL; v2 = (double)htVolSmile[k2]; }
                    if (k2 == k4) { k2 = k4 - IParam.OPTSTRIKEINTVL; v2 = (double)htVolSmile[k2]; }
                    if (k2 == k6) { k2 = k6 - IParam.OPTSTRIKEINTVL; v2 = (double)htVolSmile[k2]; }

                    if (k2 == k3 || k2 == k4 || k2 == k6 ||
                        k3 == k4 || k3 == k6 ||
                        k4 == k6) bNoConflict = false;
                    else bNoConflict = true;
                }
            }
            else
            {
                return false;
            }
        }

        //---------------------------------------------------------------
        // Lagrange polynomial: Quintic: deg(P(x)) == 5
        //---------------------------------------------------------------
        //using this curve to regenerate the whole volatility smile
        //in a much higher resolution, to facilitate implied PDF calculation
        //dk's = just to make the numbers smaller
        double dk1 = k1 / IParam.OPTSTRIKEINTVL;
        double dk2 = k2 / IParam.OPTSTRIKEINTVL;
        double dk3 = k3 / IParam.OPTSTRIKEINTVL;
        double dk4 = k4 / IParam.OPTSTRIKEINTVL;
        double dk5 = k5 / IParam.OPTSTRIKEINTVL;
        double dk6 = k6 / IParam.OPTSTRIKEINTVL;

        double p7 = (dk1 - dk2) * (dk1 - dk3) * (dk1 - dk4) * (dk1 - dk5) * (dk1 - dk6);
        double p8 = (dk2 - dk1) * (dk2 - dk3) * (dk2 - dk4) * (dk2 - dk5) * (dk2 - dk6);
        double p9 = (dk3 - dk1) * (dk3 - dk2) * (dk3 - dk4) * (dk3 - dk5) * (dk3 - dk6);
        double p10 = (dk4 - dk1) * (dk4 - dk2) * (dk4 - dk3) * (dk4 - dk5) * (dk4 - dk6);
        double p11 = (dk5 - dk1) * (dk5 - dk2) * (dk5 - dk3) * (dk5 - dk4) * (dk5 - dk6);
        double p12 = (dk6 - dk1) * (dk6 - dk2) * (dk6 - dk3) * (dk6 - dk4) * (dk6 - dk5);

        Hashtable htVolSmileTmp = new Hashtable();

        for (int i = IParam.VOLSMILEREGENSTART; i <= IParam.VOLSMILEREGENEND; i += IParam.VOLSMILEINTERVAL)
        {
            double j = (double)i / IParam.OPTSTRIKEINTVL;
            double p1 = (j-dk2)*(j-dk3)*(j-dk4)*(j-dk5)*(j-dk6);
            double p2 = (j-dk1)*(j-dk3)*(j-dk4)*(j-dk5)*(j-dk6);
            double p3 = (j-dk1)*(j-dk2)*(j-dk4)*(j-dk5)*(j-dk6);
            double p4 = (j-dk1)*(j-dk2)*(j-dk3)*(j-dk5)*(j-dk6);
            double p5 = (j-dk1)*(j-dk2)*(j-dk3)*(j-dk4)*(j-dk6);
            double p6 = (j-dk1)*(j-dk2)*(j-dk3)*(j-dk4)*(j-dk5);

            double IV = v1 * p1 / p7 +
                        v2 * p2 / p8 +
                        v3 * p3 / p9 +
                        v4 * p4 / p10 +
                        v5 * p5 / p11 +
                        v6 * p6 / p12;
            htVolSmileTmp[i] = IV;
        }

        foreach (DictionaryEntry de in htVolSmileTmp)
            if ((double)de.Value < 0)
            {
                OutputVolSmileMeth(iFrontMth, "Lagrange Polynomial (Quintic) failed - Negative values appear in the fitted curve", true);
                return false;
            }

        //to check convexity, or not to check convexity, that is the question
        //for (int i = IParam.VOLSMILEREGENSTART;
        //            i <= IParam.VOLSMILEREGENEND - 2 * IParam.VOLSMILEINTERVAL;
        //            i += IParam.VOLSMILEINTERVAL)
        //{
        //    if ((double)htVolSmileTmp[i + 2 * IParam.VOLSMILEINTERVAL] - (double)htVolSmileTmp[i + 1 * IParam.VOLSMILEINTERVAL]
        //        <
        //        (double)htVolSmileTmp[i + 1 * IParam.VOLSMILEINTERVAL] - (double)htVolSmileTmp[i])
        //    {
        //        OutputVolSmileMeth(iFrontMth, "Lagrange Polynomial (Quintic) failed - Convexity test failed", true);
        //        return false;
        //    }
        //}

        //ok, no problem
        htVolSmile = (Hashtable)htVolSmileTmp.DeepClone();


        return true;
    }


}



public class PositionController
{
    private string sDetailedOutputPath;
    private string sCPnLOutputPath;
    private string sMROutputPath;
    private string sHSCIEXHSIOutputPath;

    public PositionController(bool bForceReload)
    {
        vs = new VolatilitySurface(bForceReload);
    }

    public void SetPathsAndResetFile(string sD, string sC, string sM, string sH, bool bClean)
    {
        sDetailedOutputPath = sD;
        sCPnLOutputPath = sC;
        sMROutputPath = sM;
        sHSCIEXHSIOutputPath = sH;
        if (bClean) ResetOutputFile();
    }

    private double dCPnL = 0; public double CPnL { get { return dCPnL; } } public void ResetCPnL() { dCPnL = 0; }

    private double dAggFutPx = 0;
    private double dAggOptPx = 0;

    private long lFutPos = 0;
    private Hashtable htOptPos = new Hashtable(); //<Key>: Strike+"."+[M][YY] e.g. 20000,F12; <Value>
    public Hashtable HTOPTION { get { return htOptPos; } }
    private Hashtable htOptPosPrev = new Hashtable();
    private Hashtable htIdenOptPos = new Hashtable(); //Strategy Identifier e.g. FMD, Hashtable like htOptPos

    private HistoricalDataHelper hdh = HistoricalDataHelper.Instance;
    public VolatilitySurface vs = null;

    public long FutPos { get { return lFutPos; } }
    public long AbsCallPos
    {
        get
        {
            long lCall = 0;
            foreach (DictionaryEntry de in htOptPos)
                if (SymbolCodeHelper.IsCallFromOptCode((string)de.Key))
                    lCall += Math.Abs((long)de.Value);
            return lCall;
        }
    }
    public long AbsPutPos
    {
        get
        {
            long lPut = 0;
            foreach (DictionaryEntry de in htOptPos)
                if (SymbolCodeHelper.IsPutFromOptCode((string)de.Key))
                    lPut += Math.Abs((long)de.Value);
            return lPut;
        }
    }
    public long AggCallPos
    {
        get
        {
            long lCall = 0;
            foreach (DictionaryEntry de in htOptPos)
                if (SymbolCodeHelper.IsCallFromOptCode((string)de.Key))
                    lCall += (long)de.Value;
            return lCall;
        }
    }
    public long AggPutPos
    {
        get
        {
            long lPut = 0;
            foreach (DictionaryEntry de in htOptPos)
                if (SymbolCodeHelper.IsPutFromOptCode((string)de.Key))
                    lPut += (long)de.Value;
            return lPut;
        }
    }
    public double AggFutPx { get { return dAggFutPx; } }

    //only cloning the file paths and the positions, not even cloning the pnl
    public PositionController FastClone()
    {
        PositionController pcRtn = new PositionController(false);

        pcRtn.SetPathsAndResetFile(sDetailedOutputPath,
                                    sCPnLOutputPath,
                                    sMROutputPath,
                                    sHSCIEXHSIOutputPath,
                                    false);

        pcRtn.htOptPos = (Hashtable)htOptPos.Clone();
        pcRtn.htIdenOptPos = (Hashtable)htIdenOptPos.DeepClone();
        pcRtn.lFutPos = lFutPos;

        return pcRtn;
    }

    public int OptionContractNum
    {
        get
        {
            int iNum = 0;
            foreach (DictionaryEntry de in htOptPos)
            {
                if ((long)Math.Abs((long)(de.Value)) != 0) iNum++;
            }
            return iNum;
        }
        //strangely found to slower the optimization down...
        //get
        //{
        //    return htOptPos.Count;
        //}
    }

    public void PenalizeCPnLPctgFrict(double dUndPrice1, double dUndPrice2, DateTime dtCT, int iRnd)
    {
        object[] keys = new object[htOptPos.Count];
        htOptPos.Keys.CopyTo(keys, 0);

        foreach (object key in keys)
        {
            double dOptPx = 0;
            double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key));

            double dUndPrice = dUndPrice1;
            int iFrontMth = 1;
            string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(dtCT, 5 / 1440d);
            if (SymbolCodeHelper.GetFutCodeFromOptCode((string)key).CompareTo(s1FrontMthFutCode) == 0)
            {
                iFrontMth = 1;
                dUndPrice = dUndPrice1;
            }
            else
            {
                iFrontMth = 2;
                dUndPrice = dUndPrice2;
            }


            //first calculate option price for that contract
            if (SymbolCodeHelper.IsCallFromOptCode((string)key))
            {
                dOptPx = BlackScholesAlt.GetCallValue(
                         dUndPrice,
                         SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                         IParam.rf,
                         dTTM,
                         vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dtCT, iFrontMth));
            }
            else if (SymbolCodeHelper.IsPutFromOptCode((string)key))
            {
                dOptPx = BlackScholesAlt.GetPutValue(
                         dUndPrice,
                         SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                         IParam.rf,
                         dTTM,
                         vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dtCT, iFrontMth));
            }

            dCPnL -=  Math.Abs((long)htOptPos[key]) *
                      (Math.Ceiling(IParam.FRICTIONPCTG * dOptPx) + IParam.FRICTIONTRXCOST) *
                      iRnd; //1 round or 2 rounds

            dCPnL -= Math.Abs((long)lFutPos) * IParam.FRICTION * iRnd;
        }
    }

    public void LoadPortfolioFrmFile(string sFilePath)
    {
        if (!IParam.DataDepotLoadPort.htFileNameContent.ContainsKey(sFilePath))
        {
            using (StreamReader sr = new StreamReader(sFilePath))
            {
                try
                {
                    string sLine;
                    List<string> lContent = new List<string>();

                    while ((sLine = sr.ReadLine()) != null) lContent.Add(sLine);

                    IParam.DataDepotLoadPort.htFileNameContent.Add(sFilePath, lContent);

                    sr.Close;
                }
                catch (Exception e)
                {
                    Console.WriteLine("Default portfolio file could not be read:");
                    Console.WriteLine(e.Message);
                    Environment.Exit(0);
                }
            }
        }


        List<string> lStrings = (List<string>)IParam.DataDepotLoadPort.htFileNameContent[sFilePath];


        for (int i = 0; i < lStrings.Count; i++)
        {
            string sLine = lStrings[i];
            string[] sParts = sLine.Split(',');

            if (SymbolCodeHelper.IsCallFromOptCode(sParts[0]) ||
                SymbolCodeHelper.IsPutFromOptCode(sParts[0]))
            {
                TradeOpt(Double.Parse(sParts[1]) < 0 ? false : true,
                    SymbolCodeHelper.IsCallFromOptCode(sParts[0]),
                    SymbolCodeHelper.GetStrikeFromOptCode(sParts[0]),
                    IParam.UGLYHARDCODEDATE,
                    SymbolCodeHelper.GetFutCodeFromOptCode(sParts[0]),
                    (int)Math.Abs(Double.Parse(sParts[1])),
                    IParam.UGLYHARDCODEUNDPX, IParam.UGLYHARDCODEUNDPX,
                    false, IParam.NOFRICTION, "ASS");
            }
            else if (sParts[0].CompareTo("FUT") == 0)
            {
                TradeFut(Double.Parse(sParts[1]) < 0 ? false : true,
                    (int)Math.Abs(Double.Parse(sParts[1])),
                    IParam.UGLYHARDCODEUNDPX,
                    IParam.FRICTION);
            }
        }

    }

    //any price dPx will do, because after knocking out the randome ctrt, we'll reset pnl
    public void KnockOut1RandCtrt(RandomEngine re, double dPx, DateTime dtCT, string sIden)
    {
        int iFP = (FutPos == 0 ? 0 : 1);
        int iOP = htOptPos.Count; //deliberately not use OptionContractNum

        object[] keys = new object[htOptPos.Count];
        htOptPos.Keys.CopyTo(keys, 0);

        
        if (iFP + iOP <= 0) return;
        else
        {
            int iTheLuckyOne = re.Next(0, iFP + iOP);

            if (iTheLuckyOne == 0 && iFP == 1) //get rid of futures
            {
                TradeFut((FutPos > 0 ? false : true), Math.Abs(FutPos), dPx, IParam.FRICTION);
            }
            else //get rid of options
            {
                string sOptCode = (string)keys[iTheLuckyOne - iFP];

                TradeOpt((long)htOptPos[keys[iTheLuckyOne - iFP]] > 0 ? false : true,
                    SymbolCodeHelper.IsCallFromOptCode(sOptCode),
                    SymbolCodeHelper.GetStrikeFromOptCode(sOptCode),
                    dtCT,
                    SymbolCodeHelper.GetFutCodeFromOptCode(sOptCode),
                    Math.Abs((long)htOptPos[sOptCode]),
                    dPx,dPx,
                    false, IParam.NOFRICTION,
                    sIden);
            }
        }
    }

    //any price for dPx will do since we'll reset pnl afterwards
    public void SlashPosByHalf(double dPx, DateTime dtCT, string sIden)
    {
        //futures
        if (FutPos != 0)
        {
            TradeFut(FutPos > 0 ? false : true,
                     (long)Math.Floor(Math.Abs(FutPos) / (double)2),
                     dPx, IParam.FRICTION);
        }

        //options
        object[] keys = new object[htOptPos.Count];
        htOptPos.Keys.CopyTo(keys, 0);

        foreach (object key in keys)
        {
            long lPos = (long)(htOptPos[key]);

            bool bIsCall = SymbolCodeHelper.IsCallFromOptCode((string)key);
            bool bIsPut = SymbolCodeHelper.IsPutFromOptCode((string)key);
            
            bool bCallPut = true;
            if (bIsCall && !bIsPut) bCallPut = true;
            else if (!bIsCall && bIsPut) bCallPut = false;

            if (lPos != 0)
                TradeOpt(lPos > 0 ? false : true,
                         bCallPut, SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                         dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key),
                         (long)Math.Floor(Math.Abs(lPos) / (double)2), dPx, dPx,
                         false, IParam.NOFRICTION, sIden);
        }
    }

    //any price for dPx will do since we'll reset pnl afterwards
    public void DoublePosSize(double dPx, DateTime dtCT, string sIden)
    {
        //futures
        if (FutPos != 0)
        {
            TradeFut(FutPos > 0 ? true : false,
                     (long)Math.Max(IParam.MAXNUMFUTCONTRACT-Math.Abs(FutPos),0),
                     dPx, IParam.FRICTION);
        }

        //options
        object[] keys = new object[htOptPos.Count];
        htOptPos.Keys.CopyTo(keys, 0);

        foreach (object key in keys)
        {
            long lPos = (long)(htOptPos[key]);

            bool bIsCall = SymbolCodeHelper.IsCallFromOptCode((string)key);
            bool bIsPut = SymbolCodeHelper.IsPutFromOptCode((string)key);

            bool bCallPut = true;
            if (bIsCall && !bIsPut) bCallPut = true;
            else if (!bIsCall && bIsPut) bCallPut = false;

            if (lPos != 0)
                TradeOpt(lPos > 0 ? true : false,
                         bCallPut, SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                         dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key),
                         (long)Math.Abs(lPos), dPx, dPx,
                         false, IParam.NOFRICTION, sIden);
        }
    }

    //any price for dPx will do since we just want to get the position size
    public void MultiplyFutPsnSz(double dPx, DateTime dtCT, double dMul, string sIden)
    {
        //futures
        if (FutPos != 0)
        {
            bool bLong = (FutPos > 0);
            long lAPos = (long)Math.Abs(FutPos);

            //first unwind
            TradeFut(!bLong,
                     lAPos,
                     dPx, IParam.FRICTION);
            //then rebuild
            TradeFut(bLong,
                     (long)Math.Round((double)lAPos * dMul,0),
                     dPx, IParam.FRICTION);
        }
    }

    //any price for dPx will do since we just want to get the position size
    public void MultiplyOptPsnSz(double dPx, DateTime dtCT, double dMul, string sIden)
    {
        //options
        object[] keys = new object[htOptPos.Count];
        htOptPos.Keys.CopyTo(keys, 0);

        foreach (object key in keys)
        {
            long lPos = (long)(htOptPos[key]);
            long lAPos = (long)Math.Abs(lPos);
            bool bLong = (lPos > 0);

            bool bIsCall = SymbolCodeHelper.IsCallFromOptCode((string)key);
            bool bIsPut = SymbolCodeHelper.IsPutFromOptCode((string)key);

            bool bCallPut = true;
            if (bIsCall && !bIsPut) bCallPut = true;
            else if (!bIsCall && bIsPut) bCallPut = false;

            if (lPos != 0)
            {
                //first unwind
                TradeOpt(!bLong,
                         bCallPut, SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                         dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key),
                         lAPos,
                         dPx, dPx,
                         false, IParam.NOFRICTION, sIden);
                //then rebuild
                TradeOpt(bLong,
                         bCallPut, SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                         dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key),
                         (long)Math.Round((double)lAPos * dMul, 0),
                         dPx, dPx,
                         false, IParam.NOFRICTION, sIden);
            }
        }
    }



    public string[] GetHTOptionCode()
    {
        string[] saRtn = new string[htOptPos.Count]; ;

        htOptPos.Keys.CopyTo(saRtn, 0);

        return saRtn;
    }

    public int CompareOptCtrtDiff(PositionController pcA)
    {
        int iDiff = 0;

        if (IParam.RUNMODE == 1 || IParam.RUNMODE == 3) return iDiff;

        Hashtable htA = pcA.HTOPTION;

        //starting from this HT
        foreach (DictionaryEntry de in htOptPos)
        {
            if (!htA.ContainsKey(de.Key)) //uniq in this HT
            {
                iDiff += (int)Math.Abs((long)de.Value);
            }
            else //common entries
            {
                iDiff += (int)Math.Abs((long)de.Value - (long)(htA[de.Key]));
            }
        }

        //then the other way round from htA
        foreach (DictionaryEntry de in htA)
        {
            if (!htOptPos.ContainsKey(de.Key)) //uniq in htA
            {
                iDiff += (int)Math.Abs((long)de.Value);
            }
        }

        return iDiff;
    }



    public bool HasReachMaxTrdgCap
    {
        get
        {
            if ((OptionContractNum + (FutPos == 0 ? 0 : 1)) >= IParam.MAXNUMCONTRACTCODE) return true;
            else return false;
        }
    }

    public void TradeFut(bool bIsLong, long lAbsPos, double dPrice, double dFriction)
    {
        if (lAbsPos == 0) return;

        lFutPos += (bIsLong ? 1 : -1) * Math.Abs(lAbsPos);
        dAggFutPx += (bIsLong ? 1 : -1) * Math.Abs(lAbsPos) * dPrice;
        dAggFutPx += Math.Abs(lAbsPos * dFriction);
    }
    public void TradeOpt(bool bIsLong, bool bIsCall, double dStrike, DateTime dtCT, string sFutCode, long lAbsPos, double dUndPrice1, double dUndPrice2, bool bUsePctgFrict, double dFriction, string sIden)
    {
        if (lAbsPos == 0) return;

        HistoricalDataHelper hdh = HistoricalDataHelper.Instance;

        int iMatMonth = SymbolCodeHelper.GetMonthFromFutCode(sFutCode);
        int iMatYear = SymbolCodeHelper.GetYearFromFutCode(sFutCode);

        double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutureCode(iMatMonth, iMatYear));


        double dUndPrice = dUndPrice1;
        int iFrontMth = 1;
        string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(dtCT, 5 / 1440d);
        if (sFutCode.CompareTo(s1FrontMthFutCode) == 0)
        {
            iFrontMth = 1;
            dUndPrice = dUndPrice1;
        }
        else
        {
            iFrontMth = 2;
            dUndPrice = dUndPrice2;
        }



        double dOptPrice = 0;
        if (bIsCall)
            dOptPrice = BlackScholesAlt.GetCallValue(dUndPrice, dStrike, IParam.rf, dTTM, vs.GetLocalVolatility(dUndPrice1, dUndPrice2, dStrike, dtCT, iFrontMth));
        else
            dOptPrice = BlackScholesAlt.GetPutValue(dUndPrice, dStrike, IParam.rf, dTTM, vs.GetLocalVolatility(dUndPrice1, dUndPrice2, dStrike, dtCT, iFrontMth));

        string sKey = SymbolCodeHelper.GetOptionCode(bIsCall, dStrike, iMatMonth, iMatYear);

        //master hashtable
        if (htOptPos.ContainsKey(sKey))
        {
            htOptPos[sKey] = (long)htOptPos[sKey] + (bIsLong ? 1 : -1) * Math.Abs(lAbsPos);

            //clean up
            if ((long)(htOptPos[sKey]) == 0) htOptPos.Remove(sKey);
        }
        else
        {
            htOptPos.Add(sKey, (bIsLong ? 1 : -1) * Math.Abs(lAbsPos));
        }

        dAggOptPx += (bIsLong ? 1 : -1) * Math.Abs(lAbsPos) * dOptPrice;

        //friction
        if (bUsePctgFrict && !Double.IsNaN(dOptPrice))
        {
            dAggOptPx += Math.Abs(lAbsPos) *
                         (Math.Ceiling(IParam.FRICTIONPCTG * dOptPrice) + IParam.FRICTIONTRXCOST);
        }
        else
        {
            dAggOptPx += Math.Abs(lAbsPos * dFriction);
        }

        //sub hashtable
        if (htIdenOptPos.ContainsKey(sIden))
        {
            if (((Hashtable)htIdenOptPos[sIden]).ContainsKey(sKey))
            {
                ((Hashtable)htIdenOptPos[sIden])[sKey] =
                    (long)((Hashtable)htIdenOptPos[sIden])[sKey] + (bIsLong ? 1 : -1) * Math.Abs(lAbsPos);

                //clean up
                if ((long)(((Hashtable)htIdenOptPos[sIden])[sKey]) == 0)
                {
                    ((Hashtable)htIdenOptPos[sIden]).Remove(sKey);
                }
            }
            else
            {
                ((Hashtable)htIdenOptPos[sIden]).Add(sKey, (bIsLong ? 1 : -1) * Math.Abs(lAbsPos));
            }
        }
        else
        {
            Hashtable htTmp = new Hashtable();
            htTmp.Add(sKey, (bIsLong ? 1 : -1) * Math.Abs(lAbsPos));
            htIdenOptPos.Add(sIden, htTmp);
        }
    }

    public double GetPortOptDelta(DateTime dtCT, double dUndPrice1, double dUndPrice2)
    {
        double dPortOptDelta = 0;

        foreach (DictionaryEntry de in htOptPos)
        {
            double dOptDelta = 0;
            double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)de.Key));


            double dUndPrice = dUndPrice1;
            int iFrontMth = 1;
            string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(dtCT, 5 / 1440d);
            if (SymbolCodeHelper.GetFutCodeFromOptCode((string)de.Key).CompareTo(s1FrontMthFutCode) == 0)
            {
                iFrontMth = 1;
                dUndPrice = dUndPrice1;
            }
            else
            {
                iFrontMth = 2;
                dUndPrice = dUndPrice2;
            }



            if (SymbolCodeHelper.IsCallFromOptCode((string)de.Key))
            {
                dOptDelta = BlackScholesAlt.GetCallDelta(dUndPrice,
                            SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key),
                            IParam.rf,
                            dTTM,
                            vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key), dtCT, iFrontMth));
            }
            else if (SymbolCodeHelper.IsPutFromOptCode((string)de.Key))
            {
                dOptDelta = BlackScholesAlt.GetPutDelta(dUndPrice,
                            SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key),
                            IParam.rf,
                            dTTM,
                            vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key), dtCT, iFrontMth));
            }
            else continue;

            dPortOptDelta += (long)de.Value * dOptDelta;
        }
        return dPortOptDelta;
    }
    public double GetPortDelta(DateTime dtCT, double dUndPrice1, double dUndPrice2)
    {
        double dPortDelta = 0;
        dPortDelta = GetPortOptDelta(dtCT, dUndPrice1, dUndPrice2);
        dPortDelta += lFutPos;
        return dPortDelta;
    }
    public double GetPortOptGamma(DateTime dtCT, double dUndPrice1, double dUndPrice2)
    {
        double dPortOptGamma = 0;

        foreach (DictionaryEntry de in htOptPos)
        {
            double dOptGamma = 0;
            double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)de.Key));

            double dUndPrice = dUndPrice1;
            int iFrontMth = 1;
            string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(dtCT, 5 / 1440d);
            if (SymbolCodeHelper.GetFutCodeFromOptCode((string)de.Key).CompareTo(s1FrontMthFutCode) == 0)
            {
                iFrontMth = 1;
                dUndPrice = dUndPrice1;
            }
            else
            {
                iFrontMth = 2;
                dUndPrice = dUndPrice2;
            }


            dOptGamma = BlackScholesAlt.GetGamma(dUndPrice,
                        SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key),
                        IParam.rf,
                        dTTM,
                        vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key), dtCT, iFrontMth));

            dPortOptGamma += (long)de.Value * dOptGamma;
        }
        return dPortOptGamma;
    }

    //1. Update our CPnL
    //2. Perform clean up of expired options
    public void MarkToMarket(double dUndPrice1, double dUndPrice2, DateTime dtCT, double dCloseShortOptBelowPx)
    {
        //futures
        dAggFutPx -= dUndPrice1 * lFutPos;
        dCPnL -= dAggFutPx;
        dAggFutPx = 0;
        dAggFutPx = dUndPrice1 * lFutPos;

        //options
        double dAggOptPx_New = 0;

        //
        object[] keys2 = new object[htIdenOptPos.Count];
        htIdenOptPos.Keys.CopyTo(keys2, 0);

        foreach (object key2 in keys2)
        {
            object[] keys3 = new object[((Hashtable)htIdenOptPos[key2]).Count];
            ((Hashtable)htIdenOptPos[key2]).Keys.CopyTo(keys3, 0);

            foreach (object key3 in keys3)
            {
                double dOptPx = 0;
                double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key3));


                double dUndPrice = dUndPrice1;
                int iFrontMth = 1;
                string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(dtCT, 5 / 1440d);
                if (SymbolCodeHelper.GetFutCodeFromOptCode((string)key3).CompareTo(s1FrontMthFutCode) == 0)
                {
                    iFrontMth = 1;
                    dUndPrice = dUndPrice1;
                }
                else
                {
                    iFrontMth = 2;
                    dUndPrice = dUndPrice2;
                }


                //first calculate option price for that contract
                if (SymbolCodeHelper.IsCallFromOptCode((string)key3))
                {
                    dOptPx = BlackScholesAlt.GetCallValue(
                             dUndPrice,
                             SymbolCodeHelper.GetStrikeFromOptCode((string)key3),
                             IParam.rf,
                             dTTM,
                             vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key3), dtCT, iFrontMth));
                }
                else if (SymbolCodeHelper.IsPutFromOptCode((string)key3))
                {
                    dOptPx = BlackScholesAlt.GetPutValue(
                             dUndPrice,
                             SymbolCodeHelper.GetStrikeFromOptCode((string)key3),
                             IParam.rf,
                             dTTM,
                             vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key3), dtCT, iFrontMth));
                }
                // remove those erroneous records
                else
                {
                    ((Hashtable)htIdenOptPos[key2]).Remove((string)key3);
                    //if (htOptPos.ContainsKey((string)key3)) htOptPos.Remove((string)key3);
                    continue;
                }

                //close that position virtually first
                dAggOptPx -= dOptPx * (long)((Hashtable)htIdenOptPos[key2])[key3];

                //clean-up if TTM < 1 minute, and
                if (dTTM < (double)1 / (365 * 24 * 60))
                {
                    ((Hashtable)htIdenOptPos[key2]).Remove((string)key3);
                    if (htOptPos.ContainsKey((string)key3)) htOptPos.Remove((string)key3);
                }
                //close those shorted options that are below this price
                else if (dOptPx < dCloseShortOptBelowPx && (long)((Hashtable)htIdenOptPos[key2])[key3] < 0)
                {
                    if (htOptPos.ContainsKey(key3))
                    {
                        htOptPos[key3] = (long)htOptPos[key3] - (long)((Hashtable)htIdenOptPos[key2])[key3];
                    }
                    else
                    {
                        htOptPos.Add(key3,-(long)((Hashtable)htIdenOptPos[key2])[key3]);
                    }
                    ((Hashtable)htIdenOptPos[key2]).Remove((string)key3);
                }
                //otherwise continue holding that position
                else dAggOptPx_New += dOptPx * (long)((Hashtable)htIdenOptPos[key2])[key3];

            }
        }
        dCPnL -= dAggOptPx;
        dAggOptPx = dAggOptPx_New;

        //forced to loop once more to clear empty contracts in the aggregate position
        object[] keys = new object[htOptPos.Count];
        htOptPos.Keys.CopyTo(keys, 0);

        foreach (object key in keys)
        {
            double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key));
            if (dTTM < (double)1 / (365 * 24 * 60)) htOptPos.Remove((string)key);
        }

    }

    public void CloseFutPos(double dPrice, double dFriction)
    {
        TradeFut(lFutPos > 0 ? false : true, Math.Abs(lFutPos), dPrice, dFriction);
    }
    public void CloseOptPos(double dUndPrice1, double dUndPrice2, DateTime dtCT, bool bUsePctgFrict, double dFriction, string sIden)
    {
        if (!htIdenOptPos.ContainsKey(sIden)) return;

        object[] keys = new object[((Hashtable)htIdenOptPos[sIden]).Count];
        ((Hashtable)htIdenOptPos[sIden]).Keys.CopyTo(keys, 0);

        foreach (object key in keys)
        {
            double dOptPx = 0;
            double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key));

            double dUndPrice = dUndPrice1;
            int iFrontMth = 1;
            string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(dtCT, 5 / 1440d);
            if (SymbolCodeHelper.GetFutCodeFromOptCode((string)key).CompareTo(s1FrontMthFutCode) == 0)
            {
                iFrontMth = 1;
                dUndPrice = dUndPrice1;
            }
            else
            {
                iFrontMth = 2;
                dUndPrice = dUndPrice2;
            }


            //first calculate option price for that contract
            if (SymbolCodeHelper.IsCallFromOptCode((string)key))
            {
                dOptPx = BlackScholesAlt.GetCallValue(
                         dUndPrice,
                         SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                         IParam.rf,
                         dTTM,
                         vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dtCT, iFrontMth));
            }
            else if (SymbolCodeHelper.IsPutFromOptCode((string)key))
            {
                dOptPx = BlackScholesAlt.GetPutValue(
                         dUndPrice,
                         SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                         IParam.rf,
                         dTTM,
                         vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dtCT, iFrontMth));
            }
            // remove those erroneous records
            else
            {
                ((Hashtable)htIdenOptPos[sIden]).Remove((string)key);
                continue;
            }

            //close position virtually
            dAggOptPx -= dOptPx * (long)((Hashtable)htIdenOptPos[sIden])[key];

            //friction
            if (bUsePctgFrict && !Double.IsNaN(dOptPx))
            {
                dAggOptPx += Math.Abs((long)((Hashtable)htIdenOptPos[sIden])[key]) *
                            (Math.Ceiling(dOptPx * IParam.FRICTIONPCTG) + IParam.FRICTIONTRXCOST);
            }
            else
            {
                dAggOptPx += Math.Abs(dFriction * (long)((Hashtable)htIdenOptPos[sIden])[key]);
            }

            //clean-up!!!
            if (htOptPos.ContainsKey(key))
            {
                htOptPos[key] = (long)htOptPos[key] - (long)((Hashtable)htIdenOptPos[sIden])[key];
            }
            else
            {
                htOptPos.Add(key, -(long)((Hashtable)htIdenOptPos[sIden])[key]);
            }
            ((Hashtable)htIdenOptPos[sIden]).Remove((string)key);
        }

    }
    //public void CloseAllOptPos(VolatilitySurface.VSymbol double dUndPrice1, double dUndPrice2, DateTime dtCT, double dFriction)
    //{
    //    //need to store the set of keys first coz we have to remove items from htOptPos
    //    object[] keys = new object[htOptPos.Count];
    //    htOptPos.Keys.CopyTo(keys, 0);

    //    foreach (object key in keys)
    //    {
    //        double dOptPx = 0;
    //        double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key));


    //        double dUndPrice = dUndPrice1;
    //        int iFrontMth = 1;
    //        string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(dtCT, 5 / 1440d);
    //        if (SymbolCodeHelper.GetFutCodeFromOptCode((string)key).CompareTo(s1FrontMthFutCode) == 0)
    //        {
    //            iFrontMth = 1;
    //            dUndPrice = dUndPrice1;
    //        }
    //        else
    //        {
    //            iFrontMth = 2;
    //            dUndPrice = dUndPrice2;
    //        }

    //        //first calculate option price for that contract
    //        if (SymbolCodeHelper.IsCallFromOptCode((string)key))
    //        {
    //            dOptPx = BlackScholesAlt.GetCallValue(
    //                     dUndPrice,
    //                     SymbolCodeHelper.GetStrikeFromOptCode((string)key),
    //                     IParam.rf,
    //                     dTTM,
    //                     vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dTTM, iFrontMth));
    //        }
    //        else if (SymbolCodeHelper.IsPutFromOptCode((string)key))
    //        {
    //            dOptPx = BlackScholesAlt.GetPutValue(
    //                     dUndPrice,
    //                     SymbolCodeHelper.GetStrikeFromOptCode((string)key),
    //                     IParam.rf,
    //                     dTTM,
    //                     vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dTTM, iFrontMth));
    //        }
    //        // remove those erroneous records
    //        else
    //        {
    //            htOptPos.Remove((string)key);
    //            continue;
    //        }

    //        //close position virtually
    //        dAggOptPx -= dOptPx * (long)htOptPos[key];
    //        dAggOptPx += Math.Abs(dFriction * (long)htOptPos[key]);

    //        //clean-up!!!
    //        htOptPos.Remove((string)key);
    //    }

    //    dCPnL -= dAggOptPx;
    //    dAggOptPx = 0;

    //    //sub hashtable
    //    object[] keys2 = new object[htIdenOptPos.Count];
    //    htIdenOptPos.Keys.CopyTo(keys2, 0);
    //    foreach (object key2 in keys2) htIdenOptPos.Remove((string)key2);
    //}

    //SPAN
    //Eleven	Futures up 3/3 range	Up	        Reduced by 1 day
    //Twelve	Futures up 3/3 range	Down	    Reduced by 1 day
    //Thirteen	Futures down 3/3 range	Up	        Reduced by 1 day
    //Fourteen	Futures down 3/3 range	Down	    Reduced by 1 day
    //Fifteen	Futures up extreme move	Unchanged	Reduced by 1 day Cover % of loss
    //Sixteen	Futures down extreme move	Unchanged	Reduced by 1 day Cover % of loss
    /*
    public double CalcMarginReq(VolatilitySurface.VSymbol double dFutPrice, double dUndPrice, DateTime dtCT)
    {
        double dMR = 0;

        bool bCallIsLongOnly = true;
        bool bPutIsLongOnly = true;

        double[] dLongCallPrem = new double[2];
        double[] dLongPutPrem = new double[2];
        double dShortCallAbsExcPos = 0;
        double dShortPutAbsExcPos = 0;

        double[] dPortRisk = new double[2];
        double dMaxPortRisk = 0;

        double dRange = dUndPrice / 12.5;

        //only need to run once
        object[] keys2 = new object[htOptPos.Count];
        htOptPos.Keys.CopyTo(keys2, 0);

        foreach (object key2 in keys2)
        {
            if (SymbolCodeHelper.IsCallFromOptCode((string)key2))
            {
                if ((long)htOptPos[key2] < 0)
                {
                    bCallIsLongOnly = false;
                    dShortCallAbsExcPos += Math.Abs((long)htOptPos[key2]);
                }
                else if ((long)htOptPos[key2] > 0)
                {
                    dShortCallAbsExcPos -= Math.Abs((long)htOptPos[key2]);
                }
            }
            else if (SymbolCodeHelper.IsPutFromOptCode((string)key2))
            {
                if ((long)htOptPos[key2] < 0)
                {
                    bPutIsLongOnly = false;
                    dShortPutAbsExcPos += Math.Abs((long)htOptPos[key2]);
                }
                else if ((long)htOptPos[key2] > 0)
                {
                    dShortPutAbsExcPos -= Math.Abs((long)htOptPos[key2]);
                }
            }
        }


        //run twice, one for up range and one for down range
        for (int i = 0; i < 2; i++)
        {
            double dFutProfit = 0;
            double dOptProfit = 0;

            double dAggOptPx_New = 0;

            dRange = Math.Abs(dRange) * (i == 0 ? 1 : -1);

            //futures
            dFutProfit = lFutPos * dRange;

            //options
            double dUndPxShifted = dUndPrice + dRange;

            object[] keys = new object[htOptPos.Count];
            htOptPos.Keys.CopyTo(keys, 0);

            foreach (object key in keys)
            {
                double dOptPx = 0;
                double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key));

                //first calculate option price for that contract
                if (SymbolCodeHelper.IsCallFromOptCode((string)key))
                {
                    dOptPx = BlackScholesAlt.GetCallValue(
                             dUndPxShifted,
                             SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                             IParam.rf,
                             dTTM,
                             vs.GetLocalVolatility(dUndPxShifted, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dtCT));

                    if ((long)htOptPos[key] > 0)
                        dLongCallPrem[i] += dOptPx * Math.Abs((long)htOptPos[key]);
                }
                else if (SymbolCodeHelper.IsPutFromOptCode((string)key))
                {
                    dOptPx = BlackScholesAlt.GetPutValue(
                             dUndPxShifted,
                             SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                             IParam.rf,
                             dTTM,
                             vs.GetLocalVolatility(dUndPxShifted, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dtCT));

                    if ((long)htOptPos[key] > 0)
                        dLongPutPrem[i] += dOptPx * Math.Abs((long)htOptPos[key]);
                }

                dAggOptPx_New += dOptPx * (long)htOptPos[key];
            }

            dOptProfit = dAggOptPx_New - dAggOptPx;

            dPortRisk[i] = dFutProfit + dOptProfit;
        }

        dMaxPortRisk = Math.Abs(Math.Min(dPortRisk[0], dPortRisk[1]));

        //Margin requirement
        //If long option only >>>> margin = min(risk, long opt val) (or premium)
        //Else margin = max(risk, short opt charge)
        double dMaxLongPrem0 = Math.Max(dLongCallPrem[0], dLongPutPrem[0]);
        double dMaxLongPrem1 = Math.Max(dLongCallPrem[1], dLongPutPrem[1]);

        if (bCallIsLongOnly && bPutIsLongOnly) dMR = Math.Min(dMaxPortRisk, Math.Max(dMaxLongPrem0, dMaxLongPrem1));
        else dMR = dMaxPortRisk + Math.Max(0, Math.Max(dShortCallAbsExcPos, dShortPutAbsExcPos)) * Math.Abs(dRange) / 3;


        return dMR;
    }
    */

    public void ResetOutputFile()
    {
        using (StreamWriter fout1 = new StreamWriter(sCPnLOutputPath, false))
        {
            fout1.WriteLine();
            fout1.Close;
        }
        using (StreamWriter fout2 = new StreamWriter(sMROutputPath, false))
        {
            fout2.WriteLine();
            fout2.Close;
        }
        using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, false))
        {
            fout.WriteLine();
            fout.Close;
        }
        using (StreamWriter fout3 = new StreamWriter(sHSCIEXHSIOutputPath, false))
        {
            fout3.WriteLine();
            fout3.Close;
        }
    }

    public void OutputDetails(string sTmp)
    {
        using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, true))
        {
            fout.WriteLine(sTmp);
            fout.Close;
        }
    }

    public void OutputHSCIEXHSI(DateTime dCT, double dHSCIEXHSI, double dHSI, double dHSCIEXHSI_HSI)
    {
        using (StreamWriter fout = new StreamWriter(sHSCIEXHSIOutputPath, true))
        {
            fout.WriteLine(dCT.Year.ToString() + "-" + dCT.Month.ToString() + "-"
                + dCT.Day.ToString()
                + "\t" + dHSI.ToString()
                + "\t" + dHSCIEXHSI.ToString()
                + "\t" + dHSCIEXHSI_HSI.ToString());
            fout.Close;
        }
    }

    public void OutputPositionInfoOpen(DateTime dtCT, double dUndPrice)
    {
        using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, true))
        {
            fout.WriteLine("=============================================================");
            fout.WriteLine("| " + dtCT.ToString() + " | Open |");
            fout.WriteLine("=============================================================");
            fout.WriteLine("Underlying: Open: " + dUndPrice.ToString());
            fout.WriteLine("CPnL (pts)= " + Math.Round(dCPnL, 2).ToString());
            fout.WriteLine("Futures Position = " + FutPos.ToString());
            fout.Close;
        }
    }

    public void OutputPositionGenetic(double dUtil)
    {
        using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, true))
        {
            fout.WriteLine("-------------------------------------------------------------");
            fout.WriteLine("Util: " + dUtil.ToString());
            fout.WriteLine("FUT," + FutPos.ToString());
            
            object[] keys = new object[htOptPos.Count];
            htOptPos.Keys.CopyTo(keys, 0);

            Array.Sort(keys);
            Array.Reverse(keys);

            foreach (object key in keys)
            {
                if ((long)htOptPos[key] != 0)
                {
                    fout.Write(key.ToString());
                    fout.WriteLine("," + ((long)htOptPos[key]).ToString());
                }
            }

            fout.Close;
        }
    }



    //iMode:
    //       2 detailed: basic option systems
    //       3 assess.txt: position and option price only
    //       4 position size only
    public void OutputPositionInfoClose(DateTime dtCT, double dUndPrice1, double dUndPrice2, int iMode)
    {
        if (iMode != 3 && iMode != 4)
        using (StreamWriter fout1 = new StreamWriter(sCPnLOutputPath, true))
        {
            fout1.Write(dtCT.ToString());
            fout1.Write("\t");
            fout1.WriteLine(Math.Round(dCPnL, 2).ToString());
            fout1.Close;
        }

        if (iMode != 3 && iMode != 4)
        using (StreamWriter fout2 = new StreamWriter(sMROutputPath, true))
        {
            fout2.Write(dtCT.ToString());
            fout2.Write("\t");
            //fout2.WriteLine(Math.Round(CalcMarginReq(dUndPrice, dUndPrice, dtCT)).ToString());
            fout2.Close;
        }

        using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, true))
        {
            if (iMode != 3 && iMode != 4)
            {
                fout.WriteLine("=============================================================");
                fout.WriteLine("| " + dtCT.ToString() + " | Close |");
                fout.WriteLine("=============================================================");
                fout.WriteLine("Underlying1: Close: " + dUndPrice1.ToString());
                fout.WriteLine("Underlying2: Close: " + dUndPrice2.ToString());
                fout.WriteLine("VHSI: " + vs.VHSILatest.ToString());
                fout.WriteLine("ATR= " + Math.Round(vs.ATR).ToString());
                //fout.WriteLine("AbsCallPos= " + AbsCallPos.ToString());
                //fout.WriteLine("AbsPutPos= " + AbsPutPos.ToString());
                //fout.WriteLine("AggCallPos= " + AggCallPos.ToString());
                //fout.WriteLine("AggPutPos= " + AggPutPos.ToString());
                //fout.WriteLine("AggFutPx= " + dAggFutPx.ToString());
                //fout.WriteLine("AggOptPx= " + dAggOptPx.ToString());
                fout.WriteLine("Delta= " + (GetPortDelta(dtCT, dUndPrice1, dUndPrice2)).ToString());
                fout.WriteLine("Gamma= " + (GetPortOptGamma(dtCT, dUndPrice1, dUndPrice2)).ToString());
                fout.WriteLine("CPnL (pts)= " + Math.Round(dCPnL, 2).ToString());
                //fout.WriteLine("Margin req (Pt)= " + Math.Round(CalcMarginReq(dUndPrice, dUndPrice, dtCT)).ToString());
                //fout.WriteLine("Margin req (Ptx10)= " + Math.Round(CalcMarginReq(dUndPrice, dUndPrice, dtCT) * 10).ToString());
                fout.WriteLine("-------------------------------------------------------------");
            }

            if (iMode == 4)
            {
                fout.WriteLine("Mini Futures =\t\t[" + FutPos.ToString() + "]");
                fout.WriteLine("Regular Options:");
            }
            else
            {
                fout.WriteLine("Futures Posn =\t\t[" + FutPos.ToString() + "]");
            }

            if (iMode != 3  && iMode != 4)
                fout.WriteLine("Options Aggregate Position:");

            object[] keys = new object[htOptPos.Count];
            htOptPos.Keys.CopyTo(keys, 0);

            Array.Sort(keys);
            Array.Reverse(keys);

            foreach (object key in keys)
            {
                double dOptPx = 0;
                double dTTM = hdh.GetCalendarTTMHK(dtCT, SymbolCodeHelper.GetFutCodeFromOptCode((string)key));

                double dUndPrice = dUndPrice1;
                int iFrontMth = 1;
                string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(dtCT, 5 / 1440d);
                if (SymbolCodeHelper.GetFutCodeFromOptCode((string)key).CompareTo(s1FrontMthFutCode) == 0)
                {
                    iFrontMth = 1;
                    dUndPrice = dUndPrice1;
                }
                else
                {
                    iFrontMth = 2;
                    dUndPrice = dUndPrice2;
                }

                //first calculate option price for that contract
                if (SymbolCodeHelper.IsCallFromOptCode((string)key))
                {
                    dOptPx = BlackScholesAlt.GetCallValue(
                             dUndPrice,
                             SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                             IParam.rf,
                             dTTM,
                             vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dtCT, iFrontMth));
                }
                else if (SymbolCodeHelper.IsPutFromOptCode((string)key))
                {
                    dOptPx = BlackScholesAlt.GetPutValue(
                             dUndPrice,
                             SymbolCodeHelper.GetStrikeFromOptCode((string)key),
                             IParam.rf,
                             dTTM,
                             vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), dtCT, iFrontMth));
                }

                fout.Write("Opt: " + key.ToString());
                fout.Write("\t[" + ((long)htOptPos[key]).ToString());
                fout.WriteLine("]\t" + Math.Round(dOptPx, 2).ToString());
            }


            if (iMode != 3 && iMode != 4)
            {
                fout.WriteLine("-------------------------------------------------------------");
                object[] keys2 = new object[htIdenOptPos.Count];
                htIdenOptPos.Keys.CopyTo(keys2, 0);

                Array.Sort(keys2);

                foreach (object key2 in keys2)
                {
                    object[] keys3 = new object[((Hashtable)htIdenOptPos[key2]).Count];
                    ((Hashtable)htIdenOptPos[key2]).Keys.CopyTo(keys3, 0);

                    Array.Sort(keys3);
                    Array.Reverse(keys3);

                    fout.WriteLine("Identifier: " + key2.ToString());

                    foreach (object key3 in keys3)
                    {
                        fout.Write("Opt: " + key3.ToString());
                        fout.WriteLine("\t[" + ((Hashtable)htIdenOptPos[key2])[key3].ToString() + "]");
                    }
                }

                fout.WriteLine("-------------------------------------------------------------");
                fout.WriteLine("Change in Options Aggregate Position:");

                //now compare the new position hashtable with the old hashtable
                if (htOptPosPrev.Count > 0)
                {
                    List<string> sKeysTotal = new List<string>();
                    List<string> sKeysTotalUniq = new List<string>();

                    foreach (string s in htOptPos.Keys)
                        if ((long)htOptPos[s] != 0) sKeysTotal.Add((string)s);
                    foreach (string s in htOptPosPrev.Keys)
                        if ((long)htOptPosPrev[s] != 0) sKeysTotal.Add((string)s);

                    sKeysTotal.Sort();

                    string sPrev = "";
                    foreach (string s in sKeysTotal)
                    {
                        if (s.CompareTo(sPrev) != 0) sKeysTotalUniq.Add(s);
                        sPrev = s;
                    }


                    foreach (string s in sKeysTotalUniq)
                    {
                        bool bOld = htOptPosPrev.ContainsKey(s);
                        bool bNew = htOptPos.ContainsKey(s);

                        if (bOld && (long)htOptPosPrev[s] == 0) bOld = false;
                        if (bNew && (long)htOptPos[s] == 0) bNew = false;

                        //new
                        if (!bOld && bNew)
                        {
                            fout.Write("Opt: " + s.ToString());
                            fout.WriteLine("\t[" + htOptPos[s].ToString() + "]");
                        }
                        //old closed
                        else if (bOld && !bNew)
                        {
                            fout.Write("Opt: " + s.ToString());
                            fout.WriteLine("\t[" + (-(long)htOptPosPrev[s]).ToString() + "]");
                        }
                        //pos changed
                        else if (bOld && bNew && ((long)htOptPos[s] - (long)htOptPosPrev[s] != 0))
                        {
                            fout.Write("Opt: " + s.ToString());
                            fout.WriteLine("\t[" + ((long)htOptPos[s] - (long)htOptPosPrev[s]).ToString() + "]");
                        }
                    }

                }

                htOptPosPrev = (Hashtable)htOptPos.Clone();
            }

            fout.Close;
        }
    }
}




public class HistoricalDataHelper
{
    private static HistoricalDataHelper hdh = null;

    List<DateTime> ldtHKFESettlement = new List<DateTime>();
    List<DateTime> ldtRDTH_HSIc1_DataError = new List<DateTime>();
    List<DateTime> ldtRDTH_CIFc1_DataError = new List<DateTime>();
    int iYear, iMonth, iDay;
    int iHKFESettleSize = 0;

    private HistoricalDataHelper() { }

    public static HistoricalDataHelper Instance
    {
        get
        {
            if (hdh == null)
            {
                hdh = new HistoricalDataHelper();
                hdh.Init();
            }
            return hdh;
        }
    }

    public void Init()
    {
        ldtHKFESettlement.Add(new DateTime(1996, 01, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 02, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 03, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 05, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 06, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 08, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 09, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 10, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 11, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1996, 12, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 01, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 02, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 03, 26, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 05, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 06, 26, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 08, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 09, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 10, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 11, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1997, 12, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 01, 26, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 02, 26, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 03, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 05, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 06, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 08, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 09, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 10, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 11, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1998, 12, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 01, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 02, 25, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 03, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 05, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 06, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 07, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 09, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 10, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 11, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(1999, 12, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 01, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 02, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 03, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 04, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 05, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 06, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 07, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 09, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 10, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 11, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2000, 12, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 01, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 02, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 03, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 04, 26, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 05, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 06, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 09, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 10, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 11, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2001, 12, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 01, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 02, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 03, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 05, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 06, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 08, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 09, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 10, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 11, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2002, 12, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 01, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 02, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 03, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 05, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 06, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 08, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 09, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 10, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 11, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2003, 12, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 01, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 02, 26, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 03, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 05, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 06, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 07, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 09, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 10, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 11, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2004, 12, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 01, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 02, 25, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 03, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 04, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 05, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 06, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 07, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 09, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 10, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 11, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2005, 12, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 01, 26, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 02, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 03, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 04, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 05, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 06, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 07, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 09, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 10, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 11, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2006, 12, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 01, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 02, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 03, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 04, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 05, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 06, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 09, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 10, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 11, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2007, 12, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 01, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 02, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 03, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 05, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 06, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 08, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 09, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 10, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 11, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2008, 12, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 01, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 02, 26, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 03, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 05, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 06, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 08, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 09, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 10, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 11, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2009, 12, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 01, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 02, 25, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 03, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 05, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 06, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 07, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 09, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 10, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 11, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2010, 12, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 01, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 02, 25, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 03, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 04, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 05, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 06, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 07, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 09, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 10, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 11, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2011, 12, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 01, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 02, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 03, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 04, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 05, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 06, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 08, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 09, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 10, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 11, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2012, 12, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 01, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 02, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 03, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 04, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 05, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 06, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 07, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 08, 29, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 09, 27, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 10, 30, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 11, 28, 16, 16, 0)); iHKFESettleSize++;
        ldtHKFESettlement.Add(new DateTime(2013, 12, 30, 16, 16, 0)); iHKFESettleSize++;

        ldtRDTH_HSIc1_DataError.Add(new DateTime(2002, 05, 28));
        ldtRDTH_HSIc1_DataError.Add(new DateTime(2004, 01, 12));
        ldtRDTH_HSIc1_DataError.Add(new DateTime(2004, 01, 13));
        ldtRDTH_HSIc1_DataError.Add(new DateTime(2004, 07, 16));
        ldtRDTH_HSIc1_DataError.Add(new DateTime(2004, 10, 17));
        ldtRDTH_HSIc1_DataError.Add(new DateTime(2005, 11, 11));
        ldtRDTH_HSIc1_DataError.Add(new DateTime(2008, 08, 06));
        ldtRDTH_HSIc1_DataError.Add(new DateTime(2008, 08, 22));
        ldtRDTH_HSIc1_DataError.Add(new DateTime(2009, 09, 15));

        ldtRDTH_CIFc1_DataError.Add(new DateTime(2010, 09, 19));
        ldtRDTH_CIFc1_DataError.Add(new DateTime(2011, 03, 29));

    }
    public bool ShouldSkipDateCIF(DateTime dtInput)
    {
        return IsRDTH_CIFc1_DataError(dtInput);
    }
    public bool ShouldSkipDateHK(DateTime dtInput)
    {
        return IsRDTH_HSIc1_DataError(dtInput) || Is_HKFE_Settlement(dtInput);
    }
    public bool IsRDTH_CIFc1_DataError(DateTime dtInput)
    {
        iYear = dtInput.Year;
        iMonth = dtInput.Month;
        iDay = dtInput.Day;

        return ldtRDTH_CIFc1_DataError.Exists(CompareTheirDate);
    }
    public bool IsRDTH_HSIc1_DataError(DateTime dtInput)
    {
        iYear = dtInput.Year;
        iMonth = dtInput.Month;
        iDay = dtInput.Day;

        return ldtRDTH_HSIc1_DataError.Exists(CompareTheirDate);
    }
    public bool Is_HKFE_Settlement(DateTime dtInput)
    {
        iYear = dtInput.Year;
        iMonth = dtInput.Month;
        iDay = dtInput.Day;

        return ldtHKFESettlement.Exists(CompareTheirDate);
    }
    private bool CompareTheirDate(DateTime dt)
    {
        if (DateTime.Compare(dt, new DateTime(iYear, iMonth, iDay)) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    public int LocateComingSettlementDate(DateTime dtCT)
    {
        int iIdx = 0;
        iIdx = ldtHKFESettlement.BinarySearch(dtCT);
        iIdx = iIdx < 0 ? ~iIdx : iIdx;
        TimeSpan ts = (ldtHKFESettlement[iIdx] - dtCT);
        if (ts.TotalHours < 24) iIdx++; //if on the settlement date, use the next one, not this one
        return iIdx;
    }
    public string GetFutCodeHK_WthMinDTM(DateTime dtCT, double dMinDaysToMat)
    {
        int iFM = 1;
        while (GetCalendarTTMHK(dtCT, iFM) < (double)dMinDaysToMat / 365)
        {
            iFM++;
            if (iFM > 12) //sth's wrong
            {
                iFM = 1;
                break;
            }
        }

        int iIdx = LocateComingSettlementDate(dtCT);
        DateTime dtTmp = (DateTime)ldtHKFESettlement[iIdx - 1 + iFM];

        return SymbolCodeHelper.GetFutureCode(dtTmp.Month, dtTmp.Year);
    }
    public int GetFrontMthHK_WthMinDTM(DateTime dtCT, int iMinDaysToMat)
    {
        int iFM = 1;
        while (GetCalendarTTMHK(dtCT, iFM) < (double)iMinDaysToMat / 365) iFM++;
        return iFM;
    }
    public double GetCalendarNumDays(double dTTM)
    {
        return 365 * dTTM;
    }
    public double GetCalendarTTMHK(DateTime dtCT, int iFrontMth)
    {
        double dTTM = Double.Epsilon;

        int iIdx = LocateComingSettlementDate(dtCT);

        if (ldtHKFESettlement[iIdx].CompareTo(dtCT) > 0)
        {
            TimeSpan ts = (ldtHKFESettlement[iIdx - 1 + iFrontMth] - dtCT);
            return (double)ts.TotalMinutes / (365 * 24 * 60); //convert to year unit
        }

        return dTTM;
    }
    public double GetCalendarTTMHK(DateTime dtCT, string sFutCode)
    {
        double dTTM = Double.Epsilon;
        DateTime dtTmp = new DateTime(SymbolCodeHelper.GetYearFromFutCode(sFutCode),
                             SymbolCodeHelper.GetMonthFromFutCode(sFutCode), 10); //hardcode Day 10 in the month, no harm!

        int iIdx = LocateComingSettlementDate(dtTmp);

        if (ldtHKFESettlement[iIdx].CompareTo(dtCT) > 0)
        {
            TimeSpan ts = (ldtHKFESettlement[iIdx] - dtCT);
            dTTM = (double)ts.TotalMinutes / (365 * 24 * 60);
        }

        return dTTM;
    }

    /*
    public bool IsTradingTime(PortfolioExecutor pe, RuleBasedDayTradeSys rbdts)
    {
        int iMinB4Close = pe.iMinutesB4Close;
        if (pe.iMinutesB4Close == -1) iMinB4Close = 0;

        if (pe.iSpecialHandle == 1)
        {
            DateTime dtLocalTime = pe.ToUniversalTime(pe.CurrentTime).AddHours(8);

            if (ShouldSkipDateHK(dtLocalTime)) return false;

            //safe guard
            if (((TimeSpan)rbdts.Exchange.CurrentSessionCloseTime.Subtract(rbdts.Exchange.CurrentSessionOpenTime)).TotalHours < 12 &&
                (pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8).Day != dtLocalTime.Day ||
                pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).Day != dtLocalTime.Day))
            {
                return false;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) < 0 &&
                DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 12, 29, 0).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) > 0 &&
                DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 11, 59, 0).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) < 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 14, 30, 0)) > 0 &&
                DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 13, 30, 0)) > 0 &&
                DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else return false;
        }
        else if (pe.iSpecialHandle == 2)
        {
            if (pe.ToUniversalTime(pe.CurrentTime).Hour >= pe.iFXInactiveStart1 &&
                pe.ToUniversalTime(pe.CurrentTime).Hour <= pe.iFXInactiveEnd1)
            {
                return false;
            }
            else if (pe.ToUniversalTime(pe.CurrentTime).Hour >= pe.iFXInactiveStart2 &&
                pe.ToUniversalTime(pe.CurrentTime).Hour <= pe.iFXInactiveEnd2)
            {
                return false;
            }
            else return true;

        }
        else if (pe.iSpecialHandle == 3)
        {
            DateTime dtLocalTime = pe.ToUniversalTime(pe.CurrentTime).AddHours(8);

            //safe guard
            if (((TimeSpan)rbdts.Exchange.CurrentSessionCloseTime.Subtract(rbdts.Exchange.CurrentSessionOpenTime)).TotalHours < 12 &&
                (pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8).Day != dtLocalTime.Day ||
                pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).Day != dtLocalTime.Day))
            {
                return false;
            }
            else if (DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8)) > 0 &&
                 DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 11, 29, 0).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 13, 00, 0)) > 0 &&
                    DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else return false;
        }
        else if (pe.iSpecialHandle == 4)
        {
            DateTime dtLocalTime = pe.ToUniversalTime(pe.CurrentTime).AddHours(8);

            //safe guard
            if (((TimeSpan)rbdts.Exchange.CurrentSessionCloseTime.Subtract(rbdts.Exchange.CurrentSessionOpenTime)).TotalHours < 12 &&
                (pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8).Day != dtLocalTime.Day ||
                pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).Day != dtLocalTime.Day))
            {
                return false;
            }
            else if (DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8)) > 0 &&
                DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else return false;
        }
        else if (pe.iSpecialHandle == 5)
        {
            DateTime dtLocalTime = pe.ToUniversalTime(pe.CurrentTime).AddHours(8);

            if (ShouldSkipDateHK(dtLocalTime)) return false;

            //safe guard
            if (((TimeSpan)rbdts.Exchange.CurrentSessionCloseTime.Subtract(rbdts.Exchange.CurrentSessionOpenTime)).TotalHours < 12 &&
                (pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8).Day != dtLocalTime.Day ||
                pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).Day != dtLocalTime.Day))
            {
                return false;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) < 0 &&
                DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 11, 0, 0)) <= 0)
            {
                return true;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) > 0 &&
                DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 10, 45, 0)) <= 0)
            {
                return true;
            }
            else return false;
        }
        else if (pe.iSpecialHandle == 6)
        {
            DateTime dtLocalTime = pe.ToUniversalTime(pe.CurrentTime).AddHours(8);

            if (ShouldSkipDateHK(dtLocalTime)) return false;

            //safe guard
            if (((TimeSpan)rbdts.Exchange.CurrentSessionCloseTime.Subtract(rbdts.Exchange.CurrentSessionOpenTime)).TotalHours < 12 &&
                (pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8).Day != dtLocalTime.Day ||
                pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).Day != dtLocalTime.Day))
            {
                return false;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) < 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 11, 0, 0)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 12, 29, 0).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 10, 45, 0)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 11, 59, 0).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else return false;
        }
        else if (pe.iSpecialHandle == 7)
        {
            DateTime dtLocalTime = pe.ToUniversalTime(pe.CurrentTime).AddHours(8);

            if (ShouldSkipDateHK(dtLocalTime)) return false;

            //safe guard
            if (((TimeSpan)rbdts.Exchange.CurrentSessionCloseTime.Subtract(rbdts.Exchange.CurrentSessionOpenTime)).TotalHours < 12 &&
                (pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8).Day != dtLocalTime.Day ||
                pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).Day != dtLocalTime.Day))
            {
                return false;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) < 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 14, 30, 0)) >= 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 14, 59, 0)) <= 0)
            {
                return true;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 13, 30, 0)) >= 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 14, 59, 0)) <= 0)
            {
                return true;
            }
            else return false;
        }
        else if (pe.iSpecialHandle == 8)
        {
            DateTime dtLocalTime = pe.ToUniversalTime(pe.CurrentTime).AddHours(8);

            if (ShouldSkipDateHK(dtLocalTime)) return false;

            //safe guard
            if (((TimeSpan)rbdts.Exchange.CurrentSessionCloseTime.Subtract(rbdts.Exchange.CurrentSessionOpenTime)).TotalHours < 12 &&
                (pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8).Day != dtLocalTime.Day ||
                pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).Day != dtLocalTime.Day))
            {
                return false;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) < 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 14, 59, 0)) > 0 &&
                DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(2011, 3, 6, 23, 59, 59)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 14, 59, 0)) > 0 &&
                DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else return false;
        }
        else if (pe.iSpecialHandle == 9)
        {
            DateTime dtLocalTime = pe.ToUniversalTime(pe.CurrentTime).AddHours(8);
            if (ShouldSkipDateCIF(dtLocalTime)) return false;

            //safe guard
            if (((TimeSpan)rbdts.Exchange.CurrentSessionCloseTime.Subtract(rbdts.Exchange.CurrentSessionOpenTime)).TotalHours < 12 &&
                (pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8).Day != dtLocalTime.Day ||
                pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).Day != dtLocalTime.Day))
            {
                return false;
            }
            else if (DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionOpenTime).AddHours(8)) > 0 &&
                DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 11, 29, 0).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else if (DateTime.Compare(dtLocalTime, new DateTime(dtLocalTime.Year, dtLocalTime.Month, dtLocalTime.Day, 13, 00, 0)) > 0 &&
                    DateTime.Compare(dtLocalTime, pe.ToUniversalTime(rbdts.Exchange.CurrentSessionCloseTime).AddHours(8).AddMinutes(-iMinB4Close)) < 0)
            {
                return true;
            }
            else return false;

        }
        else
        {
            return true;
        }
    }
    */
}
