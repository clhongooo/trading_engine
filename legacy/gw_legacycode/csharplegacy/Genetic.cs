using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;


class Scenario
{
    double dProbability = double.NaN;
    public double Probability { get { return dProbability; } set { dProbability = value; } }

    DateTime dtEndingDate;
    public DateTime EndingDate { get { return dtEndingDate; } set { dtEndingDate = value; } }

    double dPrice1 = double.NaN;
    public double Price1 { set { dPrice1 = value; } get { return dPrice1; } }
    double dPrice2 = double.NaN;
    public double Price2 { set { dPrice2 = value; } get { return dPrice2; } }
    double dVolatility = double.NaN;
    public double Volatility { set { dVolatility = value; } get { return dVolatility;  } }

    public void GetEndingDatePriceVolatility(ref double dPrb, ref DateTime dtED, ref double dP1, ref double dP2, ref double dV)
    {
        dPrb = dProbability;
        dtED = dtEndingDate;
        dP1 = dPrice1;
        dP2 = dPrice2;
        dV = dVolatility;
    }
}

//--------------------------------------------------------------
// Holding the collection of scenario objects
//--------------------------------------------------------------
public class ScenarioProvider
{
    HistoricalDataHelper histdh = HistoricalDataHelper.Instance;

    double dStartingPrice1 = double.NaN;
    double dStartingPrice2 = double.NaN;
    public double StartingPrice1 { get { return dStartingPrice1; } }
    public double StartingPrice2 { get { return dStartingPrice2; } }
    double dStartingVolatility = double.NaN;
    public double StartingVolatility { get { return dStartingVolatility; } }
    private DateTime dtStartingDate;
    public DateTime StartingDate { get { return dtStartingDate; } }

    private string sFutCode = "";
    private string sFutCode2 = "";
    public string FutCode { get { return sFutCode; } }
    public string FutCode2 { get { return sFutCode2; } }


    List<Scenario> lScenario = new List<Scenario>();


    //List of 10 Tranches, each has a hashtable of P0/P-1/E(IV)*20(key) and probability (value)
    List<Hashtable> lTranchPDF = new List<Hashtable>(); //0-9 (remember to convert from 1-10)

    //Hashtable of Hashtable: Key in binary strings e.g. "10010",
    //then the item is another Hashtable of the PDFs, whose keys are -6 -5 -4 -3 ... 5 6, same as TranchPDF
    Hashtable hFSM1_PDF = new Hashtable();
    Hashtable hFSM2_PDF = new Hashtable();

    Hashtable htDeltaTranchMap = new Hashtable();
    Hashtable htGammaTranchMap = new Hashtable();
    Hashtable htVolSmileImplPDF1 = new Hashtable();
    Hashtable htVolSmileImplPDF2 = new Hashtable();
    StatHelper shVolSmileImplPDF1 = new StatHelper((IParam.VOLSMILEREGENEND-IParam.VOLSMILEREGENSTART)/IParam.VOLSMILEINTERVAL-1);
    StatHelper shScaledVolSmileImplPDF1 = new StatHelper((IParam.VOLSMILEREGENEND-IParam.VOLSMILEREGENSTART)/IParam.VOLSMILEINTERVAL-1);
    StatHelper shVolSmileImplPDF2 = new StatHelper((IParam.VOLSMILEREGENEND - IParam.VOLSMILEREGENSTART) / IParam.VOLSMILEINTERVAL-1);
    double dATMFut1 = double.NaN;
    double dATMFut2 = double.NaN;
    int iCurDeltaTranch = 0; //1-5
    int iCurGammaTranch = 0; //1-3
    int iCurTranch0 = 0; //0-14

    //------------------------------------------------------------
    public int GetNoOfScenario()
    {
        return lScenario.Count;
    }

    public void ReadInputFiles()
    {
        try
        {

            using (StreamReader sr = new StreamReader(IParam.PDFFilePath))
            {
                for (int i = 0; i < 15; i++) lTranchPDF.Add(new Hashtable());

                string sLine;
                while ((sLine = sr.ReadLine()) != null)
                {
                    string[] sParts = sLine.Split('\t');
                    for (int i = 0; i < 15; i++)
                        ((Hashtable)lTranchPDF[i]).Add(Double.Parse(sParts[0]), Double.Parse(sParts[i + 1]));
                }
                sr.Close;
            }

            using (StreamReader sr = new StreamReader(IParam.FiniteStateMachine1PDFFilePath))
            {
                string sLine;
                while ((sLine = sr.ReadLine()) != null)
                {
                    string[] sParts = sLine.Split('\t');
                    for (int m = 0; m < 31; m++) //31 rows
                    {
                        Hashtable htTmp = new Hashtable();

                        for (int n = -6; n < 7; n++) //14 columns
                            htTmp.Add(n/100d,Double.Parse(sParts[n+7]));

                        if (!hFSM1_PDF.ContainsKey(sParts[0]))
                            hFSM1_PDF.Add(sParts[0],htTmp);
                    }
                }
                sr.Close;
            }
            using (StreamReader sr = new StreamReader(IParam.FiniteStateMachine2PDFFilePath))
            {
                string sLine;
                while ((sLine = sr.ReadLine()) != null)
                {
                    string[] sParts = sLine.Split('\t');
                    for (int m = 0; m < 62; m++) //62 rows
                    {
                        Hashtable htTmp = new Hashtable();

                        for (int n = -6; n < 7; n++) //14 columns
                            htTmp.Add(n / 100d, Double.Parse(sParts[n + 7]));

                        if (!hFSM2_PDF.ContainsKey(sParts[0]))
                            hFSM2_PDF.Add(sParts[0], htTmp);
                    }
                }
                sr.Close;
            }

            using (StreamReader sr = new StreamReader(IParam.RawDataFut1FilePath))
            {
                string sLine;
                sLine = sr.ReadLine();
                dATMFut1 = double.Parse(sLine);
                sr.Close;
            }
            using (StreamReader sr = new StreamReader(IParam.RawDataFut2FilePath))
            {
                string sLine;
                sLine = sr.ReadLine();
                dATMFut2 = double.Parse(sLine);
                sr.Close;
            }

            using (StreamReader sr = new StreamReader(IParam.VSImpliedPDF1FilePath))
            {
                string sLine;
                while ((sLine = sr.ReadLine()) != null)
                {
                    string[] sParts = sLine.Split('\t');
                    htVolSmileImplPDF1.Add(int.Parse(sParts[0]), Double.Parse(sParts[1]));
                    shVolSmileImplPDF1.Add(int.Parse(sParts[0]), (long)(Double.Parse(sParts[1])*10000));
                    shScaledVolSmileImplPDF1.Add((double.Parse(sParts[0]) / IParam.KURTOSISSCALINGFACTOR),
                                                 (long)(Double.Parse(sParts[1]) * 1000000));
                }
                sr.Close;
            }
            using (StreamReader sr = new StreamReader(IParam.VSImpliedPDF2FilePath))
            {
                string sLine;
                while ((sLine = sr.ReadLine()) != null)
                {
                    string[] sParts = sLine.Split('\t');
                    htVolSmileImplPDF2.Add(int.Parse(sParts[0]), Double.Parse(sParts[1]));
                    shVolSmileImplPDF2.Add(int.Parse(sParts[0]), (long)(Double.Parse(sParts[1])*10000));
                }
                sr.Close;
            }
            using (StreamReader sr = new StreamReader(IParam.TranchDeltaFilePath))
            {
                string sLine;
                while ((sLine = sr.ReadLine()) != null)
                {
                    string[] sParts = sLine.Split('\t');
                    htDeltaTranchMap.Add(int.Parse(sParts[0]), int.Parse(sParts[1]));
                }
                sr.Close;
            }
            using (StreamReader sr = new StreamReader(IParam.TranchGammaFilePath))
            {
                string sLine;
                while ((sLine = sr.ReadLine()) != null)
                {
                    string[] sParts = sLine.Split('\t');
                    htGammaTranchMap.Add(double.Parse(sParts[0]), int.Parse(sParts[1]));
                }
                sr.Close;
            }
            dStartingPrice1 = dATMFut1;
            dStartingPrice2 = dATMFut2;



        }
        catch (Exception e)
        {
            Console.WriteLine("Misc input files could not be read:");
            Console.WriteLine(e.Message);
            Environment.Exit(0);
        }
    }
    public void GenerateScenarioFile_ScVolSmile(RuleBasedDayTradeSys rbdts)
    {
        try
        {
            ReadInputFiles();



            dtStartingDate = rbdts.LatestTime;
            sFutCode = histdh.GetFutCodeHK_WthMinDTM(dtStartingDate, 5 / 1440d);
            sFutCode2 = histdh.GetFutCodeHK_WthMinDTM(dtStartingDate, 30);

            dStartingVolatility = rbdts.LatestVHSI;

            //Option Medley subsystems
            //which delta tranch?
            object[] oaDT = new object[htDeltaTranchMap.Count];
            htDeltaTranchMap.Keys.CopyTo(oaDT, 0);
            Array.Sort(oaDT);

            for (int i = 0; i < oaDT.Length; i++)
            {
                if ((int)(oaDT[i]) > rbdts.LatestPortDelta)
                {
                    iCurDeltaTranch = (int)htDeltaTranchMap[(int)(oaDT[i-1])];
                    break;
                }
            }

            //which gamma tranch?
            object[] oaGT = new object[htGammaTranchMap.Count];
            htGammaTranchMap.Keys.CopyTo(oaGT, 0);
            Array.Sort(oaGT);

            for (int i = 0; i < oaGT.Length; i++)
            {
                if ((double)(oaGT[i]) > rbdts.LatestPortGamma)
                {
                    iCurGammaTranch = (int)htGammaTranchMap[(double)(oaGT[i - 1])];
                    break;
                }
            }

            iCurTranch0 = (iCurDeltaTranch - 1) * 3 + (iCurGammaTranch - 1);


            //for displaying the original historical PDFs (without variance adjustments)
            double[] daPDFPrice1 = new double[lTranchPDF[iCurTranch0].Count];
            double[] daPDFPrice2 = new double[lTranchPDF[iCurTranch0].Count];
            double[] daPDFModelPortf = new double[lTranchPDF[iCurTranch0].Count];
            double[] daPDFFSM1 = new double[lTranchPDF[iCurTranch0].Count];
            double[] daPDFFSM2 = new double[lTranchPDF[iCurTranch0].Count];

            //find the applicable model PDF
            object[] keys = new object[lTranchPDF[iCurTranch0].Count];
            lTranchPDF[iCurTranch0].Keys.CopyTo(keys,0);
            Array.Sort(keys);
            Array.Reverse(keys);


            //we start building our scenarios from the historical PDFs
            int iSc = 0;
            foreach (double d in keys) //13 scenarios: +6 -6 0
            {
                Scenario newScen = new Scenario();
                newScen.EndingDate = dtStartingDate.AddDays(IParam.SCENARIOENDDAY);
                newScen.Price1 = Math.Floor((d / 20 * rbdts.LatestEIV + 1) * dStartingPrice1);
                newScen.Price2 = Math.Floor((d / 20 * rbdts.LatestEIV + 1) * dStartingPrice2);

                //model portfolio and FSM historical PDFs
                daPDFModelPortf[iSc] = (double)lTranchPDF[iCurTranch0][d]; //store it up
                daPDFFSM1[iSc] = (double)((Hashtable)hFSM1_PDF[rbdts.CurrentFSM1State])[d]; //store it up
                daPDFFSM2[iSc] = (double)((Hashtable)hFSM2_PDF[rbdts.CurrentFSM2State])[d]; //store it up
                daPDFPrice1[iSc] = newScen.Price1;
                daPDFPrice2[iSc] = newScen.Price2;

                //combine the historical probabilities
                newScen.Probability =   daPDFModelPortf[iSc] * IParam.MODELPDFWEIGHTING +
                                        daPDFFSM1[iSc] * IParam.FSM1PDFWEIGHTING +
                                        daPDFFSM2[iSc] * IParam.FSM2PDFWEIGHTING;
                //--------------------------------------------------------

                lScenario.Add(newScen);
                iSc++;
            }


            //before outputing our scenario PMF1,
            //let's do the following adjustments
            //- in the light of the RNPDF1, adjust variance of historical PDFs
            //- shift the RNPDF1 by the mean of the historical PDFs,
            //- use insight from kurtosis of RNPDF, adjust that shift,
            //- merge the PDFs into our PDFs,
            double dSDRNPDF1 = shVolSmileImplPDF1.Stdev;

            StatHelper shHistPMF = new StatHelper(lTranchPDF[iCurTranch0].Count);
            foreach (Scenario sc in lScenario)
                shHistPMF.Add(sc.Price1, (long)(sc.Probability * 10000));

            //adjust variance of our historical portfolio according to the RNPDF
            HistoricalDataHelper hdh = HistoricalDataHelper.Instance;
            double dStartDateTTM = hdh.GetCalendarTTMHK(StartingDate, sFutCode);
            if (dStartDateTTM > 10d / 365)
            {
                foreach (Scenario sc in lScenario)
                {
                    sc.Price1 = (((sc.Price1 - shHistPMF.AMean) / shHistPMF.Stdev) * shVolSmileImplPDF1.Stdev)
                                        + shHistPMF.AMean;
                    sc.Price1 = Math.Floor(sc.Price1);
                }
            }

            //the destination VHSI should be obtained according to the scenario price change
            int iMidLoc = (daPDFPrice1.Length - 1) / 2;
            foreach (Scenario sc in lScenario)
            {
                double dPxChg = sc.Price1 / dATMFut1 - 1;

                VolatilitySurface vsTmp = (VolatilitySurface)(rbdts.vs.DeepClone());
                double dEIVold = vsTmp.EIV0a;

                vsTmp.AddHSIDailyOHLC(sc.Price1, sc.Price1, sc.Price1, sc.Price1, 0);
                double dEIVnew = vsTmp.EIV0a;
                double dEIVChg = dEIVnew / dEIVold - 1;

                double dEVHSIChg = 0;
                if (rbdts.LastCloseHSI_Abv_SMA10)
                {
                    double a = -680.0616916962;
                    double b = 120.9014015905;
                    double c = -4.1560316532;
                    double d = -0.0339419105;

                    dEVHSIChg = a * Math.Pow(dPxChg, 3) +
                                b * Math.Pow(dPxChg, 2) +
                                c * Math.Pow(dPxChg, 1) +
                                d;
                }
                else
                {
                    double a = 27.7506038752;
                    double b = 0.2695412305;
                    double c = -1.6992334670;
                    double d = 0.0268843388;

                    dEVHSIChg = a * Math.Pow(dPxChg, 3) +
                                b * Math.Pow(dPxChg, 2) +
                                c * Math.Pow(dPxChg, 1) +
                                d;
                }

                double[] p = new double[16];
                if (rbdts.LatestVHSI < 14)
                {
                    p[9] = 0.6912071442;
                    p[10] = 0.6375765615;
                    p[11] = 0.7735175707;
                    p[12] = 1.3218202064;
                    p[13] = 0.7767423406;
                    p[14] = 0.6772932696;
                    p[15] = -0.6673259547;
                }
                else if (rbdts.LatestVHSI < 35)
                {
                    p[9] = 0.4021487361;
                    p[10] = 0.3763276913;
                    p[11] = 0.5136209996;
                    p[12] = 1.1248662838;
                    p[13] = 0.4082610540;
                    p[14] = 0.4586002421;
                    p[15] = -0.3930596642;
                }
                else
                {
                    p[9] = 0.1252541918;
                    p[10] = 0.1244680225;
                    p[11] = 0.7102435342;
                    p[12] = 1.4334545138;
                    p[13] = 0.1821070106;
                    p[14] = 0.4012996682;
                    p[15] = -0.1319323072;
                }

                double dEVHSIPctgChg = 0;

                if (rbdts.LastCloseHSI_Abv_SMA10) dEVHSIPctgChg += p[9] * (1 + dEVHSIChg);
                else dEVHSIPctgChg += p[10] * (1 + dEVHSIChg);

                if (rbdts.LastCloseHSI_Abv_SMA10) dEVHSIPctgChg += p[11] * (dEIVChg);
                else dEVHSIPctgChg += p[12] * (dEIVChg);

                if (rbdts.LastCloseHSI_Abv_SMA10) dEVHSIPctgChg += p[13] * (dEIVold / rbdts.LatestVHSI - 1);
                else dEVHSIPctgChg += p[14] * (dEIVold / rbdts.LatestVHSI - 1);

                sc.Volatility = dStartingVolatility * (1 + dEVHSIPctgChg) + p[15] * dEIVnew;
            }


            //finding average distance between 2 scenario prices
            CalcAvgIntervalHeight caih1 = new CalcAvgIntervalHeight();
            foreach (Scenario sc in lScenario) caih1.Add(sc.Price1);
            double dScenPxHeight = caih1.GetAvgIntervalHeight();


            //now shift the RNPDF by the mean of our historical PDFs
            double dShiftAmount = shHistPMF.AMean - shVolSmileImplPDF1.AMean;


            //if more than 10 calendar days, adjust this shift using the insight from the kurtosis of the RNPDF
            //if (dStartDateTTM > 10d / 365)
            //{
            //    double dKurtRatio = shScaledVolSmileImplPDF1.KurtosisPve /
            //                        shScaledVolSmileImplPDF1.KurtosisNve * 100;

            //     //confirming bullishness
            //    if (dKurtRatio > 29.5d && shHistPMF.AMean > dATMFut1)
            //    {
            //        dShiftAmount += shVolSmileImplPDF1.Stdev / 20;
            //    }
            //    //doubting bullishness, try to counter the historical PDF
            //    else if (dKurtRatio > 29.5d && shHistPMF.AMean < dATMFut1)
            //    {
            //        dShiftAmount = -dShiftAmount;
            //    }
            //    //confirming bearishness
            //    else if (dKurtRatio < 27d && shHistPMF.AMean < dATMFut1)
            //    {
            //        dShiftAmount -= shVolSmileImplPDF1.Stdev / 20;
            //    }
            //    //doubting bearishness, try to counter the historical PDF
            //    else if (dKurtRatio < 27d && shHistPMF.AMean > dATMFut1)
            //    {
            //        dShiftAmount = -dShiftAmount;
            //    }
            //}


            //merge the shifted RNPDF with our historical PDFs
            foreach (Scenario sc in lScenario)
            {
                double dEpsilon1M = sc.Price1 - dScenPxHeight/2;
                double dEpsilon1P = sc.Price1 + dScenPxHeight/2;

                double dImplProb1 = 0;
                foreach (DictionaryEntry de in htVolSmileImplPDF1)
                {
                    if (((int)(de.Key) + dShiftAmount) > dEpsilon1M &&
                        ((int)(de.Key) + dShiftAmount) < dEpsilon1P)
                    {
                        dImplProb1 += (double)(de.Value);
                    }
                }
                sc.Probability = sc.Probability * IParam.HISTPDFWEIGHTING +
                                 dImplProb1 * IParam.IMPL1stPDFWEIGHTING;
            }




            //output our final results for scenario PMF
            string dDelimitor = "\t";

            using (StreamWriter fout = new StreamWriter(IParam.ScenarioPDFSummaryFilePath, false))
            {
                fout.WriteLine("Fut1\tOM\tFSM1\tFSM2\tFut2\t");


                for (int i = 0; i < daPDFModelPortf.Length; i++)
                {
                    fout.Write(Math.Round(daPDFPrice1[i],0));
                    fout.Write("\t");
                    fout.Write(Math.Round((double)daPDFModelPortf[i] * 100, 2));
                    fout.Write("%\t");
                    fout.Write(Math.Round((double)daPDFFSM1[i] * 100, 2));
                    fout.Write("%\t");
                    fout.Write(Math.Round((double)daPDFFSM2[i] * 100, 2));
                    fout.Write("%\t");
                    fout.Write(Math.Round(daPDFPrice2[i], 0));

                    if (i == (daPDFModelPortf.Length - 1) / 2) fout.WriteLine(" <<");
                    else fout.WriteLine();

                }

                fout.WriteLine("Finite State Machine 1: State = " + rbdts.CurrentFSM1State);
                fout.WriteLine("Finite State Machine 2: State = " + rbdts.CurrentFSM2State);
                fout.WriteLine("E[IV] = " + rbdts.LatestEIV);

                fout.Close;
            }

            using (StreamWriter fout = new StreamWriter(IParam.ScenarioFilePath, false))
            {
                string sLine;

                //output
                sLine = "";

                sLine += "1.00000";
                sLine += dDelimitor;
                sLine += dtStartingDate.Year.ToString() + "-" +
                            dtStartingDate.Month.ToString() + "-" +
                            dtStartingDate.Day.ToString();
                sLine += dDelimitor;
                sLine += dStartingPrice1.ToString();
                sLine += dDelimitor;
                sLine += dStartingPrice2.ToString();
                sLine += dDelimitor;
                sLine += dStartingVolatility.ToString();

                fout.WriteLine(sLine);

                foreach (Scenario sc in lScenario)
                {
                    sLine = "";
                    sLine += sc.Probability.ToString();
                    sLine += dDelimitor;
                    sLine += sc.EndingDate.Year.ToString() + "-" +
                                sc.EndingDate.Month.ToString() + "-" +
                                sc.EndingDate.Day.ToString();
                    sLine += dDelimitor;
                    sLine += Math.Round(sc.Price1,2).ToString();
                    sLine += dDelimitor;
                    sLine += Math.Round(sc.Price2, 2).ToString();
                    sLine += dDelimitor;
                    sLine += sc.Volatility.ToString();

                    fout.WriteLine(sLine);
                }



                fout.Close;
            }

            //output the expected new volatility smile under each scenario
            int iIdx = 0;
            foreach (Scenario sc in lScenario)
            {
                string sFileName = IParam.VolSmileScenarioPart1FilePath + iIdx + IParam.VolSmileScenarioPart2FilePath;
                using (StreamWriter fout = new StreamWriter(sFileName, false))
                {
                    DateTime dtScEndDate = dtStartingDate.AddDays(IParam.SCENARIOENDDAY);

                    fout.Write("ATM\t");
                    fout.WriteLine(sc.Price1);
                    fout.Write("VHSI\t");
                    fout.WriteLine(sc.Volatility);

                    for (int k = IParam.VOLSMILEREGENSTART; k < IParam.VOLSMILEREGENEND; k += IParam.VOLSMILEINTERVAL)
                    {
                        rbdts.vs.VHSILatest = sc.Volatility;
                        double dVol = rbdts.vs.GetScenarioVolatilityFromModel(sc.Price1, sc.Price2, (double)k, dtScEndDate, 1);
                        fout.Write(k);
                        fout.Write("\t");
                        fout.WriteLine(dVol*100);
                    }

                    fout.Close;
                }
                iIdx++;
            }



            //previously tried to obtain the vol smile from RNPDF, failed
            //int iIdx = 0;
            //HistoricalDataHelper hdh = HistoricalDataHelper.Instance;
            //DateTime dtScEndDate = dtStartingDate.AddDays(IParam.SCENARIOENDDAY);
            //double dScTTM = hdh.GetCalendarTTMHK(dtScEndDate, sFutCode);
            //foreach (Scenario sc in lScenario)
            //{
            //    //adjust variance of RNPDF according to our expected VHSI level, then move to the scenario price
            //    Hashtable htNewRNPDF = new Hashtable();
            //    foreach (DictionaryEntry de in htVolSmileImplPDF1)
            //    {
            //        double dNewKey =
            //               ((double)(int)(de.Key) - shVolSmileImplPDF1.AMean) * (sc.Volatility / StartingVolatility) +
            //               shVolSmileImplPDF1.AMean +
            //               (sc.Price1 - StartingPrice1);
            //        htNewRNPDF.Add(dNewKey, (double)htVolSmileImplPDF1[de.Key]);
            //    }

            //    Hashtable htScVolSmile = new Hashtable();
            //    BlackScholesAlt.GetVolSmile_fromRNPDF(htNewRNPDF, dScTTM, sc.Price1, sc.Volatility, ref htScVolSmile);

            //    //upto.... got stuck after GetVolSmile_fromRNPDF can't return a decent vol smile

            //    string sFileName = IParam.VolSmileScenarioPart1FilePath + iIdx + IParam.VolSmileScenarioPart2FilePath;
            //    using (StreamWriter fout = new StreamWriter(sFileName, false))
            //    {
            //        fout.WriteLine("test");
            //        fout.Close;
            //    }
            //    iIdx++;
            //}


        }
        catch (Exception e)
        {
            Console.WriteLine("Scenario files could not be written:");
            Console.WriteLine(e.Message);
        }
    }

    public void ReadScenarioFile()
    {
        try
        {
            ReadInputFiles();

            using (StreamReader sr = new StreamReader(IParam.ScenarioFilePath))
            {
                string sLine;
                sLine = sr.ReadLine();
                string[] sParts = sLine.Split('\t');

                dtStartingDate = SymbolCodeHelper.ConvertYYYYMMDD(sParts[1]);
                sFutCode = histdh.GetFutCodeHK_WthMinDTM(dtStartingDate, 5 / 1440d);
                sFutCode2 = histdh.GetFutCodeHK_WthMinDTM(dtStartingDate, 30);


                dStartingPrice1 = Double.Parse(sParts[2]);
                dStartingPrice2 = Double.Parse(sParts[3]);
                dStartingVolatility = Double.Parse(sParts[4]);

                while ((sLine = sr.ReadLine()) != null)
                {
                    if (sLine.Length < 3) continue;
                    sParts = sLine.Split('\t');
                    Scenario newScen = new Scenario();
                    newScen.Probability = Double.Parse(sParts[0]);
                    newScen.EndingDate = SymbolCodeHelper.ConvertYYYYMMDD(sParts[1]);
                    newScen.Price1 = Double.Parse(sParts[2]);
                    newScen.Price2 = Double.Parse(sParts[3]);
                    newScen.Volatility = Double.Parse(sParts[4]);
                    lScenario.Add(newScen);
                }
            }


        }
        catch (Exception e)
        {
            Console.WriteLine("Scenario file could not be read:");
            Console.WriteLine(e.Message);
        }
    }


    public void GetProbEndingDatePriceVolatility(int iScenario, ref double dProb, ref DateTime dEndingDate, ref double dPrice1, ref double dPrice2, ref double dVolatility)
    {
        if (iScenario < lScenario.Count)
            lScenario[iScenario].GetEndingDatePriceVolatility(ref dProb, ref dEndingDate, ref dPrice1, ref dPrice2, ref dVolatility);
    }
}

class Genetic
{
    public int DrawNextChrom(RandomEngine re, bool[] baDrawHistory)
    {
        int iDraw = 0;
        iDraw = re.Next(0, IParam.CHROMOSOMEMAX);

        if (!IParam.DRAWCHROMWITHREPLACEMENT)
        {
            while (baDrawHistory[iDraw])
                iDraw = re.Next(0, IParam.CHROMOSOMEMAX);
        }

        baDrawHistory[iDraw] = true;
        return iDraw;
    }

    //returns true if a code can be obtained; false if not
    //any price for dPx will do
    bool GenerateStrikeFutCode(RandomEngine re, PositionController pc, ScenarioProvider sp,
                              double dPx, bool bIsCall, ref int iStrike, ref string sFutCode)
    {
        if (pc.HasReachMaxTrdgCap)
        {
            //oops, max num of contracts reached, can't random, try to find an option with the same bIsCall
            string[] saOptionCode = pc.GetHTOptionCode();
            List<string> lMatchingOptionCode = new List<string>();

            foreach (string s in saOptionCode)
            {
                bool bCallPut = false;
                if (SymbolCodeHelper.IsCallFromOptCode(s)) bCallPut = true;
                else if (SymbolCodeHelper.IsPutFromOptCode(s)) bCallPut = false;

                if (bIsCall == bCallPut) lMatchingOptionCode.Add(s);
            }

            if (lMatchingOptionCode.Count == 0) return false; //no match
            else
            {
                int iTheLuckyOne = re.Next(0, lMatchingOptionCode.Count);
                iStrike = SymbolCodeHelper.GetStrikeFromOptCode((string)lMatchingOptionCode[iTheLuckyOne]);
                sFutCode = SymbolCodeHelper.GetFutCodeFromOptCode((string)lMatchingOptionCode[iTheLuckyOne]);
                return true; //found match!
            }
        }

        //no restriction, hurray, completely random now
        //but we prefer not getting near ATM strikes because their prices are too sensitive
        //and also not too far away,
        //using the formula for approx ATM call options
        int iFM = 0;
        if (IParam.FRONTMTHNESS == 1)
        {
            sFutCode = sp.FutCode;
            iFM = 1;
        }
        else if (IParam.FRONTMTHNESS == 2)
        {
            sFutCode = sp.FutCode2;
            iFM = 2;
        }
        else if (IParam.FRONTMTHNESS == 3)
        {
            if (re.Next(0, 10) < 5)
            {
                sFutCode = sp.FutCode;
                iFM = 1;
            }
            else
            {
                sFutCode = sp.FutCode2;
                iFM = 2;
            }
        }

        HistoricalDataHelper hdh = HistoricalDataHelper.Instance;
        double dEstATMCall = 0.4 *
                             sp.StartingVolatility / 100 *
                             sp.StartingPrice1 *
                             Math.Sqrt(hdh.GetCalendarTTMHK(sp.StartingDate, iFM));
        int iFarthest = (int)((Math.Max(dEstATMCall - 170, 0) * 4 + IParam.FARTHESTSTRIKE) / 200) + 1;

        if (bIsCall)
            iStrike = (int)Math.Round(dPx / 200, 0) * 200 + re.Next(2, iFarthest) * 200;
        else
            iStrike = (int)Math.Round(dPx / 200, 0) * 200 - re.Next(2, iFarthest) * 200;


        return true;
    }

    public static double CalcUtilityScore(double dAM, double dSV, double dSV0, /*double dLL, double dProbLL,*/ int iMinAggCallPut, int iDiffFrmMyPort, bool bUtility)
    {
        if (double.IsNaN(dAM)) return 0;
        else
        return Math.Sign(dAM) *
               (Math.Abs(dAM) / Math.Sqrt(dSV)) *
               (Math.Abs(dAM) / Math.Sqrt(dSV0)) *
               (iMinAggCallPut < IParam.OPTNAKEDSHORTCAP ? 0 : 1) *
               (bUtility ?
                (
                (iDiffFrmMyPort > IParam.MAXDEVIATIONFRMMYPORT ? 0 : 1) *
                (1 / Math.Pow(1d + iDiffFrmMyPort, IParam.DEVIATIONFRMMYPORTFACTOR))
                )
                : 1); //scale the score for easy reading
                //favour increase in position if under profit target
                //but favour decrease in position if above profit target
    }

    public static double GetPositionSizeAlpha(double[] daPnL, long[] laCount)
    {
        double dFullKellyAlpha = 0;
        double[] daProb = new double[laCount.Length];

        double[] daA = new double[laCount.Length];

        long lTotCnt = 0; for (int i = 0; i < laCount.Length; i++) lTotCnt += laCount[i];
        for (int i = 0; i < laCount.Length; i++) daProb[i] = ((double)laCount[i])/((double)lTotCnt);

        for (int i = 0; i < daPnL.Length; i++) daPnL[i] /= IParam.CAPITAL;

        double dMaxLossPctg = 0;
        for (int i = 0; i < daPnL.Length; i++)
            if (dMaxLossPctg > daPnL[i]) dMaxLossPctg = daPnL[i];

        while (true)
        {
            double dTotA = 0;
            for (int i = 0; i < daPnL.Length; i++)
            {
                daA[i] = daProb[i] * daPnL[i] / (1 + dFullKellyAlpha * daPnL[i]);
                dTotA += daA[i];
            }

            if (dTotA < 0) break;
            else dFullKellyAlpha += 1;

            if (dFullKellyAlpha * IParam.HOWMUCHKELLY * dMaxLossPctg < IParam.PSNSZLIM_MAXLOSSPCT ||
                dFullKellyAlpha * IParam.HOWMUCHKELLY > IParam.KELLYALPHAHARDCAP)
            {
                break;
            }
        }

        return dFullKellyAlpha * IParam.HOWMUCHKELLY;
    }

    string GetEllapsedTime(DateTime dtExperimentStart)
    {
        string sRtn = Math.Round(((TimeSpan)(DateTime.Now - dtExperimentStart)).TotalSeconds, 0).ToString();
        if (sRtn.Length < 6) while (sRtn.Length < 6) sRtn = " " + sRtn;

        return sRtn;
    }

    public PositionController RunGeneticOptimizationGalaxy(ScenarioProvider sp, bool bLoadBestPortFrmFile, PositionController pcMyPort)
    {
        DateTime dtExptStart = DateTime.Now;

        //creating many many galaxies and worlds
        PositionController pcBestOfAllWorlds = null;
        SortedList slBestUtilAllWorlds = new SortedList();
        List<double> ldBestScoreAllWorlds = new List<double>();

        for (int iGalaxy = 0; iGalaxy < IParam.GALAXYMAX; iGalaxy++)
        {
            //smart way of optimization
            IParam.SmartUnvsWrldStage(iGalaxy);

            Console.WriteLine(GetEllapsedTime(dtExptStart) + " sec | Enter Galaxy " + iGalaxy.ToString() + "...");
            Console.WriteLine("             Number of Worlds: " + IParam.WORLDMAX.ToString());
            Console.WriteLine("             Number of Evolution Stages: " + IParam.EVOLUTIONSTAGEMAX.ToString());

            PositionController pcFittest1World = null;
            double dBestUtilFit1World = double.NaN;
            double dBestScoreFit1World = double.NaN;

            for (int iWorld = 0; iWorld < IParam.WORLDMAX; iWorld++)
            {
                RunGeneticOptimizationWorld(sp, bLoadBestPortFrmFile, pcBestOfAllWorlds,
                                    ref pcFittest1World, ref dBestUtilFit1World, ref dBestScoreFit1World,
                                    pcMyPort);
                if (!slBestUtilAllWorlds.ContainsKey(dBestUtilFit1World))
                    slBestUtilAllWorlds.Add(dBestUtilFit1World, pcFittest1World);
                ldBestScoreAllWorlds.Add(dBestScoreFit1World);
                //which world are we in right now?
                if (iWorld % (int)(Math.Max(IParam.WORLDMAX / 2, 1)) == 0)
                    Console.WriteLine(GetEllapsedTime(dtExptStart) + " sec | World% = " +
                        Math.Round((double)iWorld / IParam.WORLDMAX * 100, 0) + "%:" +
                        "\tsup(U)=" +
                        Math.Round((double)slBestUtilAllWorlds.GetKey(slBestUtilAllWorlds.Count - 1),8));
            }

            if (slBestUtilAllWorlds.Count > 0)
            {
                ldBestScoreAllWorlds.Sort();

                //for the next loop
                pcBestOfAllWorlds
                    = ((PositionController)slBestUtilAllWorlds.GetByIndex(slBestUtilAllWorlds.Count - 1)).FastClone();
                Console.Write("             Best of all Worlds:\t");
                Console.Write("sup(U) = " + Math.Round((double)slBestUtilAllWorlds.GetKey(slBestUtilAllWorlds.Count - 1), 8));
                Console.WriteLine("\tsup(S) = " + Math.Round(ldBestScoreAllWorlds[ldBestScoreAllWorlds.Count-1], 8));
            }
        }

        //output the best of all galaxies to file
        if (slBestUtilAllWorlds.Count > 0 && pcBestOfAllWorlds != null)
            pcBestOfAllWorlds.OutputPositionGenetic((double)slBestUtilAllWorlds.GetKey(slBestUtilAllWorlds.Count - 1));

        return pcBestOfAllWorlds;
    }


    //IParam.FRONTMTHNESS:
    //1 = 1st front month contracts only
    //2 = 2nd front month contracts only
    //3 = both 1st and 2nd front month contracts
    public void RunGeneticOptimizationWorld(ScenarioProvider sp, bool bLoadBestPortFrmFile, PositionController pcDefaultPort, ref PositionController pcBestOfThisWorld, ref double dBestWorldUtil, ref double dBestWorldScore, PositionController pcMyPort)
    {
        string sIden = "GEN";
        RandomEngine re = new RandomEngine();

        //some general variables
        DateTime dtScenarioEndingDate = sp.StartingDate; //just init it with sth
        double dScenarioProbability = double.NaN;
        double dScenarioPrice1 = double.NaN;
        double dScenarioPrice2 = double.NaN;
        double dScenarioVolatility = double.NaN;
        int iStrike = 0;

        //no need to recalculate score if there's no change to their chromosome, so we need score memory
        double[] daUtil = new double[IParam.CHROMOSOMEMAX];
        //need to adjust TARGETEXPRETURN based on score
        double[] daScore = new double[IParam.CHROMOSOMEMAX];

        //------------------------------------------------------------
        //this world / earth has been born, ancient creatures are created
        //init chromosomes to nil position, or from a default portfolio
        PositionController[] pcChromosome = new PositionController[IParam.CHROMOSOMEMAX];
        for (int iChrom = 0; iChrom < IParam.CHROMOSOMEMAX; iChrom++)
        {
            pcChromosome[iChrom] = new PositionController(false);
            pcChromosome[iChrom].SetPathsAndResetFile(
                IParam.GeneticResultFilePath, IParam.GeneticResultFilePath,
                IParam.GeneticResultFilePath, IParam.GeneticResultFilePath, false);
            if (bLoadBestPortFrmFile) pcChromosome[iChrom].LoadPortfolioFrmFile(IParam.BestPortFilePath);
            if (pcDefaultPort != null) pcChromosome[iChrom] = pcDefaultPort.FastClone();
        }

        //now evolution starts
        //creatures and their chromosomes undergo different stages in the evolution process
        for (int iEvolutionStage = 0; iEvolutionStage < IParam.EVOLUTIONSTAGEMAX; iEvolutionStage++)
        {
            //mutation: initialization
            bool[] baDrawHistory = new bool[IParam.CHROMOSOMEMAX];
            for (int i = 0; i < IParam.CHROMOSOMEMAX; i++) baDrawHistory[i] = false;
            int iTheLuckyOne;
            StatHelper[] shUtil = new StatHelper[IParam.CHROMOSOMEMAX];
            //SortedList[] slUtil = new SortedList[IParam.CHROMOSOMEMAX]; //disabled 07Nov12
            for (int i = 0; i < IParam.CHROMOSOMEMAX; i++)
            {
                shUtil[i] = new StatHelper(IParam.SCENARIOMAX);
                //slUtil[i] = new SortedList();  //disabled 07Nov12
            }
            int[] iaRank = new int[IParam.CHROMOSOMEMAX];
            string sFutCode = "";
            int iOCN = 1;

            //start to mutate
            //------------------------------------------------------------------------
            //random LC SC LP SP LF SF
            //------------------------------------------------------------------------
            for (int i = 0; i < 6; i++)
            {
                iTheLuckyOne = DrawNextChrom(re, baDrawHistory);

                //1. add LCxn at random strike, after random clobbering
                if (re.Next(0, 10) < 5)
                {
                    iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
                    if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
                    if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, true, ref iStrike, ref sFutCode))
                    {
                        pcChromosome[iTheLuckyOne].TradeOpt(true, true, iStrike,
                            sp.StartingDate, sFutCode,
                            re.Next(1, Math.Max(iOCN, 2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
                    }
                }
                //2. add SCxn at random strike, after random clobbering
                if (re.Next(0, 10) < 5)
                {
                    iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
                    if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
                    if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, true, ref iStrike, ref sFutCode))
                    {
                        pcChromosome[iTheLuckyOne].TradeOpt(false, true, iStrike,
                            sp.StartingDate, sFutCode,
                            re.Next(1, Math.Max(iOCN, 2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
                    }
                }
                //3. add LPxn at random strike, after random clobbering
                if (re.Next(0, 10) < 5)
                {
                    iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
                    if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
                    if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, false, ref iStrike, ref sFutCode))
                    {
                        pcChromosome[iTheLuckyOne].TradeOpt(true, false, iStrike,
                            sp.StartingDate, sFutCode,
                            re.Next(1, Math.Max(iOCN, 2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
                    }
                }
                //4. add SPxn at random strike, after random clobbering
                if (re.Next(0, 10) < 5)
                {
                    iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
                    if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
                    if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, false, ref iStrike, ref sFutCode))
                    {
                        pcChromosome[iTheLuckyOne].TradeOpt(false, false, iStrike,
                            sp.StartingDate, sFutCode,
                            re.Next(1, Math.Max(iOCN, 2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
                    }
                }
                //5. add LFxn, after random clobbering
                if (re.Next(0, 10) < 5)
                {
                    iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
                    if (pcChromosome[iTheLuckyOne].OptionContractNum >= 2 &&
                        re.Next(0, 10) < 5)
                        pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
                    if (!pcChromosome[iTheLuckyOne].HasReachMaxTrdgCap)
                    {
                        if (pcChromosome[iTheLuckyOne].FutPos <= 0)
                            pcChromosome[iTheLuckyOne].TradeFut(true, 1, sp.StartingPrice1, IParam.NOFRICTION);
                    }
                    else
                    {
                        if (pcChromosome[iTheLuckyOne].FutPos < 0)
                            pcChromosome[iTheLuckyOne].TradeFut(true, 1, sp.StartingPrice1, IParam.NOFRICTION);
                    }
                }
                //6. add SFxn, after random clobbering
                if (re.Next(0, 10) < 5)
                {
                    iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
                    if (pcChromosome[iTheLuckyOne].OptionContractNum >= 2 &&
                        re.Next(0, 10) < 5)
                        pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
                    if (!pcChromosome[iTheLuckyOne].HasReachMaxTrdgCap)
                    {
                        if (pcChromosome[iTheLuckyOne].FutPos >= 0)
                            pcChromosome[iTheLuckyOne].TradeFut(false, 1, sp.StartingPrice1, IParam.NOFRICTION);
                    }
                    else
                    {
                        if (pcChromosome[iTheLuckyOne].FutPos > 0)
                            pcChromosome[iTheLuckyOne].TradeFut(false, 1, sp.StartingPrice1, IParam.NOFRICTION);
                    }
                }
            }

            
            //------------------------------------------------------------------------
            //trying out different option strategies
            //------------------------------------------------------------------------
            //7. add short strangle: SCxn SPxn (LCxn) (LPxn) at random strikes, after random clobbering
            iTheLuckyOne = DrawNextChrom(re, baDrawHistory);
            iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
            if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, true, ref iStrike, ref sFutCode))
            {
                pcChromosome[iTheLuckyOne].TradeOpt(false, true, iStrike,
                    sp.StartingDate, sFutCode,
                    re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            }
            if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, false, ref iStrike, ref sFutCode))
            {
                pcChromosome[iTheLuckyOne].TradeOpt(false, false, iStrike,
                    sp.StartingDate, sFutCode,
                    re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            }
            if (re.Next(0, 10) < 3)
            {
                if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, true, ref iStrike, ref sFutCode))
                {
                    pcChromosome[iTheLuckyOne].TradeOpt(true, true, iStrike,
                        sp.StartingDate, sFutCode,
                        re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
                }
            }
            if (re.Next(0, 10) < 3)
            {
                if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, false, ref iStrike, ref sFutCode))
                {
                    pcChromosome[iTheLuckyOne].TradeOpt(true, false, iStrike,
                        sp.StartingDate, sFutCode,
                        re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
                }
            }

            //8. add vertical spread: LCxn SCxn at random strikes, after random clobbering
            iTheLuckyOne = DrawNextChrom(re, baDrawHistory);
            iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
            if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, true, ref iStrike, ref sFutCode))
            {
                pcChromosome[iTheLuckyOne].TradeOpt(true, true, iStrike,
                    sp.StartingDate, sFutCode, re.Next(1, Math.Max(iOCN,2)),
                    sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            }
            if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, true, ref iStrike, ref sFutCode))
            {
                pcChromosome[iTheLuckyOne].TradeOpt(false, true, iStrike,
                    sp.StartingDate, sFutCode, re.Next(1, Math.Max(iOCN,2)),
                    sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            }

            //9. add vertical spread: LPxn SPxn at random strikes, after random clobbering
            iTheLuckyOne = DrawNextChrom(re, baDrawHistory);
            iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
            if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, false, ref iStrike, ref sFutCode))
            {
                pcChromosome[iTheLuckyOne].TradeOpt(true, false, iStrike,
                    sp.StartingDate, sFutCode, re.Next(1, Math.Max(iOCN,2)),
                    sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            }
            if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, false, ref iStrike, ref sFutCode))
            {
                pcChromosome[iTheLuckyOne].TradeOpt(false, false, iStrike,
                    sp.StartingDate, sFutCode, re.Next(1, Math.Max(iOCN,2)),
                    sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            }

            //10-12. add protective put: SFxn SPxn LCxn at random strike, after random clobbering
            //this is more likely to produce optimal portfolios from experience
            //for (int i = 0; i < 3; i++)
            //{
            //    iTheLuckyOne = DrawNextChrom(re, baDrawHistory);
            //    iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
            //    if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            //    if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            //    if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, false, ref iStrike, ref sFutCode))
            //    {
            //        pcChromosome[iTheLuckyOne].TradeOpt(false, false, iStrike,
            //            sp.StartingDate, sFutCode,
            //            re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            //    }
            //    if (!pcChromosome[iTheLuckyOne].HasReachMaxTrdgCap ||
            //        (pcChromosome[iTheLuckyOne].HasReachMaxTrdgCap && pcChromosome[iTheLuckyOne].FutPos != 0))
            //        pcChromosome[iTheLuckyOne].TradeFut(false, re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, false, IParam.NOFRICTION);
            //    if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, true, ref iStrike, ref sFutCode))
            //    {
            //        pcChromosome[iTheLuckyOne].TradeOpt(true, true, iStrike,
            //            sp.StartingDate, sFutCode,
            //            re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            //    }
            //}

            //13. add covered call: LFxn SCxn (LPxn) at random strike, after random clobbering
            //iTheLuckyOne = DrawNextChrom(re, baDrawHistory);
            //iOCN = pcChromosome[iTheLuckyOne].OptionContractNum;
            //if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            //if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            //if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, true, ref iStrike, ref sFutCode))
            //{
            //    pcChromosome[iTheLuckyOne].TradeOpt(false, true, iStrike,
            //        sp.StartingDate, sFutCode,
            //        re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            //}
            //if (!pcChromosome[iTheLuckyOne].HasReachMaxTrdgCap ||
            //    (pcChromosome[iTheLuckyOne].HasReachMaxTrdgCap && pcChromosome[iTheLuckyOne].FutPos != 0))
            //    pcChromosome[iTheLuckyOne].TradeFut(true, re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, false, IParam.NOFRICTION);
            //if (re.Next(0, 10) < 5)
            //{
            //    if (GenerateStrikeFutCode(re, pcChromosome[iTheLuckyOne], sp, sp.StartingPrice1, false, ref iStrike, ref sFutCode))
            //    {
            //        pcChromosome[iTheLuckyOne].TradeOpt(true, false, iStrike,
            //            sp.StartingDate, sFutCode,
            //            re.Next(1, Math.Max(iOCN,2)), sp.StartingPrice1, sp.StartingPrice2, false, IParam.NOFRICTION, sIden);
            //    }
            //}

            //14. double size
            iTheLuckyOne = DrawNextChrom(re, baDrawHistory);
            if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            pcChromosome[iTheLuckyOne].DoublePosSize(sp.StartingPrice1, sp.StartingDate, sIden);

            //15. slash by half
            iTheLuckyOne = DrawNextChrom(re, baDrawHistory);
            if (re.Next(0, 10) < 3) pcChromosome[iTheLuckyOne].KnockOut1RandCtrt(re, sp.StartingPrice1, sp.StartingDate, sIden);
            pcChromosome[iTheLuckyOne].SlashPosByHalf(sp.StartingPrice1, sp.StartingDate, sIden);

            //limit the number of futures contract
            foreach (PositionController pcFC in pcChromosome)
            {
                if (pcFC.FutPos > IParam.MAXNUMFUTCONTRACT)
                {
                    pcFC.TradeFut(false, Math.Abs(pcFC.FutPos) - IParam.MAXNUMFUTCONTRACT,
                                         sp.StartingPrice1, IParam.NOFRICTION);
                }
                else if (-pcFC.FutPos > IParam.MAXNUMFUTCONTRACT)
                {
                    pcFC.TradeFut(true, Math.Abs(pcFC.FutPos) - IParam.MAXNUMFUTCONTRACT,
                                        sp.StartingPrice1, IParam.NOFRICTION);
                }
            }

            //force futures?
            if (IParam.FORCEINCLUDEFUT)
            {
                foreach (PositionController pcFC in pcChromosome)
                {
                    if (Math.Abs(pcFC.FutPos) == 0)
                    {
                        if (re.Next(0, 9) <= 4)
                            pcFC.TradeFut(true, 1, sp.StartingPrice1, IParam.NOFRICTION);
                        else
                            pcFC.TradeFut(false, 1, sp.StartingPrice1, IParam.NOFRICTION);
                    }
                }
            }


            //start running scenarios
            for (int iSc = 0; iSc < sp.GetNoOfScenario(); iSc++)
            {
                //reset CPnL
                for (int iChrom = 0; iChrom < IParam.CHROMOSOMEMAX; iChrom++)
                {
                    if (iEvolutionStage > 0 && !baDrawHistory[iChrom]) continue; //skip unchanged chromosomes

                    //house-keeping:
                    //if the number of contracts is getting out of hand, slash them
                    if (pcChromosome[iChrom].AbsCallPos +
                        pcChromosome[iChrom].AbsPutPos +
                        Math.Abs(pcChromosome[iChrom].FutPos) > IParam.MAXNUMCONTRACT)
                    pcChromosome[iChrom].SlashPosByHalf(sp.StartingPrice1, sp.StartingDate, sIden);

                    //reset to initial market condition
                    pcChromosome[iChrom].vs.VHSILatest = sp.StartingVolatility;
                    pcChromosome[iChrom].MarkToMarket(sp.StartingPrice1, sp.StartingPrice2, sp.StartingDate, 0);
                    pcChromosome[iChrom].ResetCPnL();
                    pcChromosome[iChrom].PenalizeCPnLPctgFrict(sp.StartingPrice1, sp.StartingPrice2, sp.StartingDate, 1);
                }

                //for each chromosome, update price and volatility
                sp.GetProbEndingDatePriceVolatility(iSc, ref dScenarioProbability, ref dtScenarioEndingDate, ref dScenarioPrice1, ref dScenarioPrice2, ref dScenarioVolatility);

                for (int iChrom = 0; iChrom < IParam.CHROMOSOMEMAX; iChrom++)
                {
                    if (iEvolutionStage > 0 && !baDrawHistory[iChrom]) continue; //skip unchanged chromosomes

                    pcChromosome[iChrom].vs.VHSILatest = dScenarioVolatility;
                    pcChromosome[iChrom].MarkToMarket(dScenarioPrice1, dScenarioPrice2, dtScenarioEndingDate, 0);
                    pcChromosome[iChrom].PenalizeCPnLPctgFrict(sp.StartingPrice1, sp.StartingPrice2, sp.StartingDate, 1);

                    shUtil[iChrom].Add(pcChromosome[iChrom].CPnL, (int)(dScenarioProbability * 1000));
                    //disabled 07Nov12
                    //if (!slUtil[iChrom].ContainsKey(pcChromosome[iChrom].CPnL))
                    //    slUtil[iChrom].Add(pcChromosome[iChrom].CPnL, dScenarioProbability);
                    //else
                    //    slUtil[iChrom][pcChromosome[iChrom].CPnL] =
                    //        (double)slUtil[iChrom][pcChromosome[iChrom].CPnL] + dScenarioProbability;
                }
            }

            //end of this evoluation stage
            //calculate utility
            for (int iChrom = 0; iChrom < IParam.CHROMOSOMEMAX; iChrom++)
            {
                if (iEvolutionStage > 0 && !baDrawHistory[iChrom]) continue; //skip unchanged chromosomes

                if (!double.IsNaN(shUtil[iChrom].AMean))
                {
                    daUtil[iChrom] = Genetic.CalcUtilityScore(shUtil[iChrom].AMean,
                                                              shUtil[iChrom].SemiVar,
                                                              shUtil[iChrom].SemiVar0,
                                                              //(double)slUtil[iChrom].GetKey(0),  //disabled 07Nov12
                                                              //(double)slUtil[iChrom].GetByIndex(0),
                                                              (int)Math.Min(pcChromosome[iChrom].AggCallPos, pcChromosome[iChrom].AggPutPos),
                                                              pcChromosome[iChrom].CompareOptCtrtDiff(pcMyPort),
                                                              true);
                    daScore[iChrom] = Genetic.CalcUtilityScore(shUtil[iChrom].AMean,
                                                               shUtil[iChrom].SemiVar,
                                                               shUtil[iChrom].SemiVar0,
                                                               //(double)slUtil[iChrom].GetKey(0), //disabled 07Nov12
                                                               //(double)slUtil[iChrom].GetByIndex(0),
                                                               (int)Math.Min(pcChromosome[iChrom].AggCallPos, pcChromosome[iChrom].AggPutPos),
                                                               pcChromosome[iChrom].CompareOptCtrtDiff(pcMyPort),
                                                               false);
                }
                else
                {
                    daUtil[iChrom] = 0;
                    daScore[iChrom] = 0;
                }
            }

            //rank chromosomes by their score
            //SortedList<double, List<int>> slUtilChrom = new SortedList<double, List<int>>();
            SortedList slUtilChrom = new SortedList();
            for (int i = 0; i < IParam.CHROMOSOMEMAX; i++)
            {
                if (slUtilChrom.ContainsKey(daUtil[i]))
                    ((List<int>)slUtilChrom[daUtil[i]]).Add(i);
                else
                {
                    List<int> li = new List<int>();
                    li.Add(i);
                    slUtilChrom.Add(daUtil[i], li);
                }
            }

            int iTotalAdjUtil = 0; //just the reverse of its rank
            int[] iaNoOfOffspring = new int[slUtilChrom.Count];

            int iAdjScore = 1;
            foreach (double d in slUtilChrom.Keys)
            {
                if (slUtilChrom[d] != null) iTotalAdjUtil += iAdjScore * ((List<int>)slUtilChrom[d]).Count;
                iAdjScore++;
            }
            for (iAdjScore = 1; iAdjScore <= slUtilChrom.Count; iAdjScore++)
                iaNoOfOffspring[iAdjScore - 1] = (int)Math.Ceiling(iAdjScore * IParam.CHROMOSOMEMAX / (double)iTotalAdjUtil);

            //now that the total no of offsprings may exceed IParam.CHROMOSOMEMAX... (reason 1)


            PositionController[] pcChromosome2 = new PositionController[IParam.CHROMOSOMEMAX];
            int iPtrChrom2 = 0; //pointing where we are in the target pcChromosome2
            int iPtrNoOfOffspring = iaNoOfOffspring.Length - 1;
            double[] daReversedKeys = new double[slUtilChrom.Count];
            slUtilChrom.Keys.CopyTo(daReversedKeys, 0);
            Array.Reverse(daReversedKeys);
            foreach (double d in daReversedKeys)
            {
                if (slUtilChrom[d] != null)
                    for (int i = 0; i < ((List<int>)slUtilChrom[d]).Count; i++)
                    {
                        for (int j = 0; j < iaNoOfOffspring[iPtrNoOfOffspring]; j++)
                        {
                            pcChromosome2[iPtrChrom2] = pcChromosome[((List<int>)slUtilChrom[d])[i]].FastClone();
                            daUtil[iPtrChrom2] = d; //coz we have score memory and the chromosomes are shuffled now, we need to set the scores right!
                            iPtrChrom2++;
                            if (iPtrChrom2 >= IParam.CHROMOSOMEMAX) goto LABEL1; //coz of (reason 1)
                        }
                    }

                iPtrNoOfOffspring--;
            }

        LABEL1:
            //now copy from the tmp pcChromosome2 back to the real pcChromosome
            for (int i = 0; i < pcChromosome2.Length; i++)
                pcChromosome[i] = pcChromosome2[i].FastClone();
        }

        //that's the end of this evolution process, the fittest has survived
        //can output the best chromosome to file for more verbosity but can be safed for less I/O
        //pcChromosome[0].OutputPositionGenetic(daUtil[0]);

        //output the best utility and score of this world
        pcBestOfThisWorld = pcChromosome[0].FastClone();
        dBestWorldUtil = daUtil[0];
        Array.Sort(daScore);
        dBestWorldScore = daScore[daScore.Length - 1];

    }
}
