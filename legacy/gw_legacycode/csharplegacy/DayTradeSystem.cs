using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;
using System.Reflection;
using System.IO;

public class RuleBasedDayTradeSys
{
    IParam.SYSNAME sysname;

    HistoricalDataHelper histdh = HistoricalDataHelper.Instance;
    PositionController pc = null;
    public VolatilitySurface vs { get { return pc.vs; } }

    List<double> ldHSI_H = null;
    List<double> ldHSI_L = null;
    List<double> ldHSI_C = null;
    List<double> ldHSCI_C = null;
    List<double> ldHSCI_CEXHSI = null;
    List<double> ldHSCI_CEXHSI_HSI = null;

    //Technical Indicators
    SmaAlt sma3d = null;
    SmaAlt sma5d = null;
    SmaAlt sma10d = null;
    public bool LastCloseHSI_Abv_SMA10 { get { return (ldHSI_C[ldHSI_C.Count - 1] > sma10d.SMA); } }
    SmaAlt sma20d = null;
    SmaAlt sma50d = null;
    SmaAlt sma250d = null;
    WrAlt wr3 = null;
    WrAlt wr20 = null;
    JohnEhlers je = null;

    SmaAlt sma20d_HSCIEXHSI_HSI = null;

    SmaAlt smaATR_P3 = null;

    HighLowHelper hlh2 = null;
    HighLowHelper hlh5 = null;
    HighLowHelper hlh6 = null;
    HighLowHelper hlh10 = null;
    HighLowHelper hlh13 = null;
    HighLowHelper hlh20 = null;
    HighLowHelper hlh50 = null;


    //short term memory of sub-systems
    int iSysCTLatestDecisn = 0;

    bool bSysDMKLSetup = false;
    int iSysDMKLCountDw = 0;
    double dSysDMKL1stCntDwClose = double.NaN;

    bool bSysDMKSSetup = false;
    int iSysDMKSCountDw = 0;
    double dSysDMKS1stCntDwClose = double.NaN;

    int iLastB1DHLPos = 0;

    bool bDetailed;

    //for external information retrieval
    double _LatestPortDelta = 0;
    double _LatestPortGamma = 0;
    double _LatestPrice = 0;
    DateTime _LatestTime;
    double _LatestVHSI = 0;
    double _LatestATR = 0;
    double _LatestEIV = 0;
    string _State_FSM1 = "";
    string _State_FSM2 = "";
    public double LatestPortDelta { get { return _LatestPortDelta; } }
    public double LatestPortGamma { get { return _LatestPortGamma; } }
    public double LatestPrice { get { return _LatestPrice; } }
    public DateTime LatestTime { get { return _LatestTime; } }
    public double LatestVHSI { get { return _LatestVHSI; } }
    public double LatestATR { get { return _LatestATR; } }
    public double LatestEIV { get { return _LatestEIV; } }
    public string CurrentFSM1State { get { return _State_FSM1; } }
    public string CurrentFSM2State { get { return _State_FSM2; } }

    public RuleBasedDayTradeSys(IParam.SYSNAME sn, string dDetailedOutputPath, string dCPnLOutputPath, string dMROutputPath, string dHSCIEXHSIOutputPath, bool bD)
    {
        bDetailed = bD;

        sysname = sn;
        pc = new PositionController(true);
        pc.SetPathsAndResetFile(dDetailedOutputPath, dCPnLOutputPath, dMROutputPath, dHSCIEXHSIOutputPath, true);

        ldHSI_H = new List<double>();
        ldHSI_L = new List<double>();
        ldHSI_C = new List<double>();
        ldHSCI_C = new List<double>();
        ldHSCI_CEXHSI = new List<double>();
        ldHSCI_CEXHSI_HSI = new List<double>();

        sma3d = new SmaAlt(3);
        sma5d = new SmaAlt(5);
        sma10d = new SmaAlt(10);
        sma20d = new SmaAlt(20);
        sma50d = new SmaAlt(50);
        sma250d = new SmaAlt(250);
        wr3 = new WrAlt(3);
        wr20 = new WrAlt(20);
        je = new JohnEhlers(22);
        sma20d_HSCIEXHSI_HSI = new SmaAlt(20);


        smaATR_P3 = new SmaAlt(3);

        hlh2 = new HighLowHelper(2);
        hlh5 = new HighLowHelper(5);
        hlh6 = new HighLowHelper(6);
        hlh10 = new HighLowHelper(10);
        hlh13 = new HighLowHelper(13);
        hlh20 = new HighLowHelper(20);
        hlh50 = new HighLowHelper(50);
    }


    public double GetHSCIEXHSI(double dHSCI, double dHSCIlast, double dHSI, double dHSIlast)
    {
        double dHSCIEXHSI = double.NaN;

        if (ldHSCI_CEXHSI.Count == 0)
            dHSCIEXHSI = dHSI; //initialize to the same level as the current HSI to make them most comparable!
        else dHSCIEXHSI = ldHSCI_CEXHSI[ldHSCI_CEXHSI.Count - 1];

        if (!double.IsNaN(dHSCI) && dHSCI > 0 &&
            !double.IsNaN(dHSCIlast) && dHSCIlast > 0 &&
            !double.IsNaN(dHSI) && dHSI > 0 &&
            !double.IsNaN(dHSIlast) && dHSIlast > 0)
            dHSCIEXHSI *= ((dHSCI / dHSCIlast) - (0.63 / 0.97) * (dHSI / dHSIlast)) * (0.97 / 0.34);
        return dHSCIEXHSI;
    }


    public void DetPsnSzSystemB(DateTime dtCT, double dFutPrice, ref int iPosSize3, ref int iPosSize5, ref int iPosSize10, ref int iPosSizeZ, ref int iPosSize1DHL)
    {
        //Monthly weights
        int iSysB3MthW = 0;
        int iSysB5MthW = 0;
        int iSysB10MthW = 0;
        int iSysBZMthW = 0;
        int iSysB1DHLMthW = 0;

        //System B3: Monthly weight
        //According to probability of crossing
        switch (dtCT.Month)
        {
            case 1: if (dtCT.Day < 15) iSysB3MthW = 3; else iSysB3MthW = 2; break;
            case 2: if (dtCT.Day < 15) iSysB3MthW = 1; else iSysB3MthW = 2; break;
            case 3: if (dtCT.Day < 15) iSysB3MthW = 2; else iSysB3MthW = 2; break;
            case 4: if (dtCT.Day < 15) iSysB3MthW = 2; else iSysB3MthW = 2; break;
            case 5: if (dtCT.Day < 15) iSysB3MthW = 2; else iSysB3MthW = 3; break;
            case 6: if (dtCT.Day < 15) iSysB3MthW = 1; else iSysB3MthW = 2; break;
            case 7: if (dtCT.Day < 15) iSysB3MthW = 3; else iSysB3MthW = 3; break;
            case 8: if (dtCT.Day < 15) iSysB3MthW = 1; else iSysB3MthW = 2; break;
            case 9: if (dtCT.Day < 15) iSysB3MthW = 3; else iSysB3MthW = 2; break;
            case 10: if (dtCT.Day < 15) iSysB3MthW = 3; else iSysB3MthW = 1; break;
            case 11: if (dtCT.Day < 15) iSysB3MthW = 3; else iSysB3MthW = 2; break;
            case 12: if (dtCT.Day < 15) iSysB3MthW = 2; else iSysB3MthW = 2; break;
            default: iSysB3MthW = 0; break;
        }

        //System B5: Monthly weight
        //According to probability of crossing
        switch (dtCT.Month)
        {
            case 1: if (dtCT.Day < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
            case 2: if (dtCT.Day < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
            case 3: if (dtCT.Day < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
            case 4: if (dtCT.Day < 15) iSysB5MthW = 3; else iSysB5MthW = 2; break;
            case 5: if (dtCT.Day < 15) iSysB5MthW = 2; else iSysB5MthW = 3; break;
            case 6: if (dtCT.Day < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
            case 7: if (dtCT.Day < 15) iSysB5MthW = 2; else iSysB5MthW = 3; break;
            case 8: if (dtCT.Day < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
            case 9: if (dtCT.Day < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
            case 10: if (dtCT.Day < 15) iSysB5MthW = 3; else iSysB5MthW = 2; break;
            case 11: if (dtCT.Day < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
            case 12: if (dtCT.Day < 15) iSysB5MthW = 1; else iSysB5MthW = 2; break;
            default: iSysB5MthW = 0; break;
        }

        //System B10: Monthly weight
        //According to probability of crossing
        switch (dtCT.Month)
        {
            case 1: if (dtCT.Day < 15) iSysB10MthW = 2; else iSysB10MthW = 1; break;
            case 2: if (dtCT.Day < 15) iSysB10MthW = 1; else iSysB10MthW = 2; break;
            case 3: if (dtCT.Day < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
            case 4: if (dtCT.Day < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
            case 5: if (dtCT.Day < 15) iSysB10MthW = 2; else iSysB10MthW = 1; break;
            case 6: if (dtCT.Day < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
            case 7: if (dtCT.Day < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
            case 8: if (dtCT.Day < 15) iSysB10MthW = 1; else iSysB10MthW = 2; break;
            case 9: if (dtCT.Day < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
            case 10: if (dtCT.Day < 15) iSysB10MthW = 3; else iSysB10MthW = 2; break;
            case 11: if (dtCT.Day < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
            case 12: if (dtCT.Day < 15) iSysB10MthW = 1; else iSysB10MthW = 2; break;
            default: iSysB10MthW = 0; break;
        }

        //System BZ: Monthly weight
        //According to probability of crossing
        iSysBZMthW = 3;
        iSysB1DHLMthW = 3;


        //Volatility-adjusted position size
        double dAlpha = pc.vs.ATR / dFutPrice;
        double dVolAdjB3 = Math.Pow(1 / dAlpha, 1.3) / 250;
        double dVolAdjB5 = Math.Pow(1 / dAlpha, 1) / 250;
        double dVolAdjB10 = Math.Pow(1 / dAlpha, 1) / 250;
        double dVolAdjBZ = Math.Pow(1 / dAlpha, 1) / 250;
        double dVolAdjB1DHL = Math.Pow(1 / dAlpha, 1) / 250;

        //System B3, B5, B10, BZ, B1DHL: Position size
        double dPosSize3 = IParam.PosSize[(int)IParam.SYSNAME.B3] * (double)iSysB3MthW * dVolAdjB3 / dFutPrice;
        iPosSize3 = (int)Math.Round(dPosSize3, 0);
        double dPosSize5 = IParam.PosSize[(int)IParam.SYSNAME.B5] * (double)iSysB5MthW * dVolAdjB5 / dFutPrice;
        iPosSize5 = (int)Math.Round(dPosSize5, 0);
        double dPosSize10 = IParam.PosSize[(int)IParam.SYSNAME.B10] * (double)iSysB10MthW * dVolAdjB10 / dFutPrice;
        iPosSize10 = (int)Math.Round(dPosSize10, 0);
        double dPosSizeZ = IParam.PosSize[(int)IParam.SYSNAME.BZ] * (double)iSysBZMthW * dVolAdjBZ / dFutPrice;
        iPosSizeZ = (int)Math.Round(dPosSizeZ, 0);
        double dPosSize1DHL = IParam.PosSize[(int)IParam.SYSNAME.B1DHL] * (double)iSysB1DHLMthW * dVolAdjB1DHL / dFutPrice;
        iPosSize1DHL = (int)Math.Round(dPosSize1DHL, 0);

        return;   
    }

    public string GetFSM1State(double dHSI)
    {
        string sRtn = "";

        if (dHSI > sma3d.SMA) sRtn += "1"; else sRtn += "0";
        if (dHSI > sma5d.SMA) sRtn += "1"; else sRtn += "0";
        if (Math.Abs(wr20.WR + 50) > 35) sRtn += "1"; else sRtn += "0";
        //deviation from centre
        double dDev = Math.Abs(dHSI - sma20d.SMA) / pc.vs.ATR;
        if (dDev > 3.115610105) sRtn += "11";
        else if (dDev > 1.794500381) sRtn += "10";
        else if (dDev > 0.861608321) sRtn += "01";
        else sRtn += "00";

        return sRtn;
    }

    public string GetFSM2State(double dHSI)
    {
        string sRtn = "";

        if (dHSI > sma3d.SMA) sRtn += "1"; else sRtn += "0";
        if (dHSI > sma5d.SMA) sRtn += "1"; else sRtn += "0";
        if (dHSI > sma20d.SMA) sRtn += "1"; else sRtn += "0";
        if (Math.Abs(wr20.WR + 50) > 45) sRtn += "1"; else sRtn += "0";
        //deviation from centre
        double dDev = Math.Abs(dHSI - sma20d.SMA) / pc.vs.ATR;
        if (dDev > 3.554387261) sRtn += "11";
        else if (dDev > 1.794500381) sRtn += "10";
        else if (dDev > 0.861608321) sRtn += "01";
        else sRtn += "00";

        return sRtn;
    }



    public void RunSystemB(DateTime dtCT, double dFutPrice)
    {
        if (IParam.PosSize[(int)IParam.SYSNAME.B3] == 0 &&
            IParam.PosSize[(int)IParam.SYSNAME.B5] == 0 &&
            IParam.PosSize[(int)IParam.SYSNAME.B10] == 0 &&
            IParam.PosSize[(int)IParam.SYSNAME.BZ] == 0 &&
            IParam.PosSize[(int)IParam.SYSNAME.B1DHL] == 0)
            return;

        int iPosSize3 = 0;
        int iPosSize5 = 0;
        int iPosSize10 = 0;
        int iPosSizeZ = 0;
        int iPosSize1DHL = 0;
        DetPsnSzSystemB(dtCT, dFutPrice, ref iPosSize3, ref iPosSize5, ref iPosSize10, ref iPosSizeZ, ref iPosSize1DHL);

        int iAggPos = 0;

        if (dFutPrice > sma3d.SMA) iAggPos += iPosSize3;
        else if (dFutPrice < sma3d.SMA) iAggPos -= iPosSize3;
        if (dFutPrice > sma5d.SMA) iAggPos += iPosSize5;
        else if (dFutPrice < sma5d.SMA) iAggPos -= iPosSize5;
        if (dFutPrice > sma10d.SMA) iAggPos += iPosSize10;
        else if (dFutPrice < sma10d.SMA) iAggPos -= iPosSize10;
        if (!double.IsNaN(je.GetZeroLagSMA()) && je.GetZeroLagSMA() > 0)
        {
            if (dFutPrice > je.GetZeroLagSMA()) iAggPos += iPosSizeZ;
            else if (dFutPrice < je.GetZeroLagSMA()) iAggPos -= iPosSizeZ;
        }
        if (ldHSI_C.Count > 2)
        {
            if (dFutPrice > ldHSI_H[ldHSI_H.Count - 2])
                iLastB1DHLPos = iPosSize1DHL;
            else if (dFutPrice < ldHSI_L[ldHSI_L.Count - 2])
                iLastB1DHLPos = -iPosSize1DHL;
            
            iAggPos += iLastB1DHLPos;
        }


        if (iAggPos > pc.FutPos)
            pc.TradeFut(true, Math.Abs(iAggPos - pc.FutPos), dFutPrice, IParam.NOFRICTION);
        else if (iAggPos < pc.FutPos)
            pc.TradeFut(false, Math.Abs(iAggPos - pc.FutPos), dFutPrice, IParam.NOFRICTION);

        return;
    }


    public void RunSystemG(DateTime dtCT, double dFutPricePC, double dFutPriceO, double dFutPriceH, double dFutPriceL, double dFutPriceC)
    {
        //Relative position to 10d and 20d
        bool bStrenLast10d = (dFutPriceO > hlh10.LowestLow + (hlh10.HLRange / 2) ? true : false);
        bool bStrenLast20d = (dFutPriceO > hlh20.LowestLow + (hlh20.HLRange / 2) ? true : false);

        //Gap size
        double dGap = dFutPriceO / dFutPricePC - 1;
        bool bGapUpDw = dGap > 0 ? true : false;
        double dDownGap = (!bGapUpDw ? -dGap : 0);
        int iDownGap = (int)Math.Truncate(Math.Max(dDownGap - 0.001, 0) / 0.002) + 1;
        double dUpGap = (bGapUpDw ? dGap : 0);
        int iUpGap = (int)Math.Truncate(Math.Max(dDownGap - 0.001, 0) / 0.002) + 1;

        //Position size
        double dPosSizeG = 0;


        //Calculate the Expected highest rebound / lowest retracement
        double dEHHRbndPct = 0;
        double dELLRtrcPct = 0;

        if (bStrenLast10d && bStrenLast20d && !bGapUpDw)
        {
            dEHHRbndPct = 0.0000208578591200051 * Math.Pow(iDownGap, 3)
                          + -0.000533869433605896 * Math.Pow(iDownGap, 2)
                          + 0.00416167533364532 * iDownGap
                          - 0.00191786171904916;
            dPosSizeG = IParam.PosSize[(int)IParam.SYSNAME.GSSD] / dFutPriceO;
        }
        else if (bStrenLast10d && bStrenLast20d && bGapUpDw)
        {
            dELLRtrcPct = -0.0000003714914060953 * Math.Pow(iDownGap, 3)
                          + 0.0000303883398929313 * Math.Pow(iDownGap, 2)
                          + -0.000262046509068164 * iDownGap
                          + 0.00640255898890488;
            dPosSizeG = IParam.PosSize[(int)IParam.SYSNAME.GSSU] / dFutPriceO;
        }
        else if (bStrenLast10d && !bStrenLast20d && !bGapUpDw)
        {
            dEHHRbndPct = -8.02343040969102 * 0.000001 * Math.Pow(iDownGap, 3)
                          + 0.000365660313770849 * Math.Pow(iDownGap, 2)
                          - 0.00443972388587243 * iDownGap
                          + 0.0184658100767389;
            dPosSizeG = IParam.PosSize[(int)IParam.SYSNAME.GWSD] / dFutPriceO;
        }
        else if (bStrenLast10d && !bStrenLast20d && bGapUpDw)
        {
            dELLRtrcPct = 7.15410399939425 * 0.000001 * Math.Pow(iDownGap, 3)
                          + -0.000265999043250634 * Math.Pow(iDownGap, 2)
                          + 0.00278902171255197 * iDownGap
                          - 0.000989491108559136;
            dPosSizeG = IParam.PosSize[(int)IParam.SYSNAME.GWSU] / dFutPriceO;
        }
        else if (!bStrenLast10d && bStrenLast20d && !bGapUpDw)
        {
            dEHHRbndPct = -6.31829118419655 * 0.000001 * Math.Pow(iDownGap, 3)
                          + 0.00022759496452409 * Math.Pow(iDownGap, 2)
                          - 0.00162216147874239 * iDownGap
                          + 0.00694435011954448;
            dPosSizeG = IParam.PosSize[(int)IParam.SYSNAME.GSWD] / dFutPriceO;
        }
        else if (!bStrenLast10d && bStrenLast20d && bGapUpDw)
        {
            dELLRtrcPct = 0.000024374644446028 * Math.Pow(iDownGap, 3)
                          + -0.000707289729192606 * Math.Pow(iDownGap, 2)
                          + 0.00546678912573438 * iDownGap
                          - 0.00282809886524485;
            dPosSizeG = IParam.PosSize[(int)IParam.SYSNAME.GSWU] / dFutPriceO;
        }
        else if (!bStrenLast10d && !bStrenLast20d && !bGapUpDw)
        {
            dEHHRbndPct = -3.67220033797158 * 0.000001 * Math.Pow(iDownGap, 3)
                          + 0.000151182828879659 * Math.Pow(iDownGap, 2)
                          - 0.00109918685961058 * iDownGap
                          + 0.00735905046302385;
            dPosSizeG = IParam.PosSize[(int)IParam.SYSNAME.GWWD] / dFutPriceO;
        }
        else if (!bStrenLast10d && !bStrenLast20d && bGapUpDw)
        {
            dELLRtrcPct = 0.0000115245319366214 * Math.Pow(iDownGap, 3)
                          + -0.000416130604527095 * Math.Pow(iDownGap, 2)
                          + 0.0040305544745203 * iDownGap
                          - 0.00164002358717294;
            dPosSizeG = IParam.PosSize[(int)IParam.SYSNAME.GWWU] / dFutPriceO;
        }

        double dEHHRbnd = dFutPriceO * (1 + dEHHRbndPct);
        double dEHHRbndHalf = dFutPriceO * (1 + dEHHRbndPct / 2);
        double dELLRtrc = dFutPriceO * (1 - dELLRtrcPct);
        double dELLRtrcHalf = dFutPriceO * (1 - dELLRtrcPct / 2);

        //Position size again
        dPosSizeG *= 1 / (pc.vs.ATR / dFutPriceO);
        int iPosSizeG = (int)Math.Round(dPosSizeG, 0);

        for (int iLoop = 0; iLoop < 10; iLoop++) pc.OutputDetails("");

        //Logic
        if (bGapUpDw && !(bStrenLast10d && bStrenLast20d) && Math.Abs(dELLRtrcHalf / dFutPriceO - 1) > 0.0025 && iPosSizeG > 0)
        {
            pc.TradeFut(true, iPosSizeG, dFutPriceO, IParam.NOFRICTION);

            pc.OutputDetails("");
            pc.OutputDetails("------------------------------------------------------------------");
            pc.OutputDetails("System G: Up gap" +
                             " - 20dSW: " + bStrenLast20d.ToString() +
                             " - 10dSW: " + bStrenLast10d.ToString());
            pc.OutputDetails("          Longed [" + iPosSizeG.ToString() + "] at mkt open " +
                                         dFutPriceO.ToString());
            pc.OutputDetails("          Stop loss at " + Math.Round(dELLRtrcHalf, 0).ToString() +
                             " offset [" + Math.Abs(Math.Round(dELLRtrcHalf-dFutPriceO,0)).ToString() + "]");
            pc.OutputDetails("          Otherwise exit at mkt close");

            if (dFutPriceL < dELLRtrcHalf)
                pc.TradeFut(false, iPosSizeG, dELLRtrcHalf, IParam.NOFRICTION);
            else
                pc.TradeFut(false, iPosSizeG, dFutPriceC, IParam.NOFRICTION);
        }
        else if (!bGapUpDw && ((!bStrenLast10d && !bStrenLast20d) || (!bStrenLast10d && bStrenLast20d)) && Math.Abs(dEHHRbndHalf / dFutPriceO - 1) > 0.0025 && iPosSizeG > 0)
        {
            pc.TradeFut(false, iPosSizeG, dFutPriceO, IParam.NOFRICTION);

            pc.OutputDetails("");
            pc.OutputDetails("------------------------------------------------------------------");
            pc.OutputDetails("System G: Down gap" +
                             " - 20dSW: " + bStrenLast20d.ToString() +
                             " - 10dSW: " + bStrenLast10d.ToString());
            pc.OutputDetails("          Shorted [" + iPosSizeG.ToString() + "] at mkt open " +
                                         dFutPriceO.ToString());
            pc.OutputDetails("          Stop loss at " + Math.Round(dEHHRbndHalf, 0).ToString() +
                             " offset [" + Math.Abs(Math.Round(dEHHRbndHalf - dFutPriceO,0)).ToString() + "]");
            pc.OutputDetails("          Otherwise exit at mkt close");

            if (dFutPriceH > dEHHRbndHalf)
                pc.TradeFut(true, iPosSizeG, dEHHRbndHalf, IParam.NOFRICTION);
            else
                pc.TradeFut(true, iPosSizeG, dFutPriceC, IParam.NOFRICTION);
        }


        return;
    }



    public void OnDayOpen(DateTime CurrentTime,
        double HSIDayOpen, double HSIDayHigh, double HSIDayLow, double HSIDayClose,
        double HSCIDayOpen, double HSCIDayHigh, double HSCIDayLow, double HSCIDayClose)
    {
        //pc.MarkToMarket(HSIDayOpen, HSIDayOpen, CurrentTime, IParam.CLOSEBELOW);
        RunSystemB(CurrentTime, HSIDayOpen);
        if (ldHSI_C.Count > 0) RunSystemG(CurrentTime, (double)ldHSI_C[ldHSI_C.Count - 1], HSIDayOpen, HSIDayHigh, HSIDayLow, HSIDayClose);
        pc.OutputPositionInfoOpen(CurrentTime, HSIDayOpen);
    }

    public void OnDayClose(DateTime CurrentTime,
        double HSIDayOpen, double HSIDayHigh, double HSIDayLow, double HSIDayClose,
        double Volatility,
        double HSCIDayOpen, double HSCIDayHigh, double HSCIDayLow, double HSCIDayClose)
    {
        //Calculate and Output HSCIEXHSI
        double dHSCIEXHSI = double.NaN;
        double dHSCIEXHSI_HSI = double.NaN;

        if (ldHSCI_C.Count >= 1)
        {
            dHSCIEXHSI = GetHSCIEXHSI(HSCIDayClose, ldHSCI_C[ldHSCI_C.Count - 1], HSIDayClose, ldHSI_C[ldHSI_C.Count - 1]);
            dHSCIEXHSI_HSI = dHSCIEXHSI / HSIDayClose;
            if (bDetailed) pc.OutputHSCIEXHSI(CurrentTime, dHSCIEXHSI, HSIDayClose, dHSCIEXHSI_HSI);
        }

        //Price memory
        ldHSI_H.Add(HSIDayHigh);
        ldHSI_L.Add(HSIDayLow);
        ldHSI_C.Add(HSIDayClose);
        ldHSCI_C.Add(HSCIDayClose);
        if (!double.IsNaN(dHSCIEXHSI) && dHSCIEXHSI > 0) ldHSCI_CEXHSI.Add(dHSCIEXHSI);
        if (!double.IsNaN(dHSCIEXHSI_HSI) && dHSCIEXHSI_HSI > 0) ldHSCI_CEXHSI_HSI.Add(dHSCIEXHSI_HSI);

        //Volatility related
        pc.vs.VHSILatest = Volatility;
        pc.vs.AddHSIDailyOHLC(HSIDayOpen, HSIDayHigh, HSIDayLow, HSIDayClose, 0);

        //Technical Indicators
        hlh2.Add(HSIDayHigh, HSIDayLow);
        hlh5.Add(HSIDayHigh, HSIDayLow);
        hlh6.Add(HSIDayHigh, HSIDayLow);
        hlh10.Add(HSIDayHigh, HSIDayLow);
        hlh13.Add(HSIDayHigh, HSIDayLow);
        hlh20.Add(HSIDayHigh, HSIDayLow);
        hlh50.Add(HSIDayHigh, HSIDayLow);
        wr3.Add(HSIDayOpen, HSIDayHigh, HSIDayLow, HSIDayClose, 0);
        wr20.Add(HSIDayOpen, HSIDayHigh, HSIDayLow, HSIDayClose, 0);
        je.Add(HSIDayHigh, HSIDayLow, HSIDayClose);

        sma3d.Add(HSIDayClose);
        sma5d.Add(HSIDayClose);
        sma10d.Add(HSIDayClose);
        sma20d.Add(HSIDayClose);
        sma50d.Add(HSIDayClose);
        sma250d.Add(HSIDayClose);
        if (!double.IsNaN(dHSCIEXHSI_HSI) && dHSCIEXHSI_HSI > 0) sma20d_HSCIEXHSI_HSI.Add(dHSCIEXHSI_HSI);
        smaATR_P3.Add(pc.vs.ATR / HSIDayClose);

        //Mark to market
        pc.MarkToMarket(HSIDayClose, HSIDayClose, CurrentTime, IParam.CLOSEBELOW);

        //futures code
        string sFutCode = histdh.GetFutCodeHK_WthMinDTM(CurrentTime, 1);
        string sFutCode2 = histdh.GetFutCodeHK_WthMinDTM(CurrentTime, 31);

        //strikes
        int iATMStrike = (int)Math.Round(HSIDayClose / 200, 0) * 200;

        //System CS
        //Use ATM

        //System CT
        //Friction = 0
        //2.13%: 0, 1
        //3.26%: 0, 2
        //4.36%: 0, 3
        //4.71%: 1, 3
        //5.29% 12632911: 1, 4
        //6.1% 21417193: 0, 10

        //Friction = 2
        //2.74%: 0 2
        //4.32%: 1 3
        //5.05%: 1 4
        //6.27%: 1 10
        //6.67%: 1 -
        //7.13%: 2 -
        //7.17%: 3 -
        double dSysCTCallStrike = HSIDayClose + pc.vs.ATR * 1;
        int iSysCTCallStrike = (int)Math.Round(dSysCTCallStrike / 200, 0) * 200;
        double dSysCTCallStrike2 = HSIDayClose + pc.vs.ATR * 3;
        int iSysCTCallStrike2 = (int)Math.Round(dSysCTCallStrike2 / 200, 0) * 200;
        double dSysCTPutStrike = HSIDayClose - pc.vs.ATR * 1;
        int iSysCTPutStrike = (int)Math.Round(dSysCTPutStrike / 200, 0) * 200;
        double dSysCTPutStrike2 = HSIDayClose - pc.vs.ATR * 3;
        int iSysCTPutStrike2 = (int)Math.Round(dSysCTPutStrike2 / 200, 0) * 200;

        //System TR
        //2572235: 0, 1
        //1655980: 1, 2
        //2240298: 0.5, 1.5
        double dSysTRCallStrike = HSIDayClose + pc.vs.ATR * 0;
        int iSysTRCallStrike = (int)Math.Round(dSysTRCallStrike / 200, 0) * 200;
        double dSysTRCallStrike2 = HSIDayClose + pc.vs.ATR * 1;
        int iSysTRCallStrike2 = (int)Math.Round(dSysTRCallStrike2 / 200, 0) * 200;
        double dSysTRPutStrike = HSIDayClose - pc.vs.ATR * 0;
        int iSysTRPutStrike = (int)Math.Round(dSysTRPutStrike / 200, 0) * 200;
        double dSysTRPutStrike2 = HSIDayClose - pc.vs.ATR * 1;
        int iSysTRPutStrike2 = (int)Math.Round(dSysTRPutStrike2 / 200, 0) * 200;

        //System FMD
        //Friction = 0
        //5.45%: 1, 4
        //5.8%: 2, 4
        //5.94%: 0, 3
        //6.72%: 2, 5
        //6.79%: 3, 5

        //Friction = 2
        //4.20%: 0 2
        //4.21%: 1 3
        //4.22%: 2 4
        //4.32%: 2 (call), 2 4 (put)
        //4.32%: 3 (call), 2 4 (put)
        //4.32%: 4 (call), 2 4 (put)
        //4.53%: 3 5
        //4.53%: 0 3
        //5.21%: 2 5
        //5.27%: 3 6
        //5.41%: 2 10
        //5.57%: 2 6
        //5.69%: 2 7
        //8.09%: 2 -
        //9.07%: 4 -
        //9.57%: 3 -
        double dSysFMDCallStrike = hlh20.HighestHigh + pc.vs.ATR * 2;
        int iSysFMDCallStrike = (int)Math.Round(dSysFMDCallStrike / 200, 0) * 200;
        double dSysFMDCallStrike2 = hlh20.HighestHigh + pc.vs.ATR * 4;
        int iSysFMDCallStrike2 = (int)Math.Round(dSysFMDCallStrike2 / 200, 0) * 200;
        double dSysFMDPutStrike = hlh20.LowestLow - pc.vs.ATR * 2;
        int iSysFMDPutStrike = (int)Math.Round(dSysFMDPutStrike / 200, 0) * 200;
        double dSysFMDPutStrike2 = hlh20.LowestLow - pc.vs.ATR * 4;
        int iSysFMDPutStrike2 = (int)Math.Round(dSysFMDPutStrike2 / 200, 0) * 200;

        //System GUT
        //Friction = 0
        //4.39%: 2
        //5.28%: 3
        //5.92%: 4
        //6.49%: 5

        //Friction = 2
        //2.91%: 4
        //3.13%: 2
        //3.28%: 3
        double dSysGUTPutStrike = HSIDayClose - pc.vs.ATR * 3;
        int iSysGUTPutStrike = (int)Math.Round(dSysGUTPutStrike / 200, 0) * 200;

        //System A
        //Friction = 0
        //6.53%: 2
        //8.12%: 3
        //9.66%: 4
        //10.83: 5

        //Friction = 2
        //-ve: 4
        //-ve: 2
        double dSysACallStrike = HSIDayClose + pc.vs.ATR * 4;
        int iSysACallStrike = (int)Math.Round(dSysACallStrike / 200, 0) * 200;
        double dSysAPutStrike = HSIDayClose - pc.vs.ATR * 4;
        int iSysAPutStrike = (int)Math.Round(dSysAPutStrike / 200, 0) * 200;

        //System FC
        //1.06%: 1 3 2 5
        //1.04%: 1 2 2 5
        //1.12%: 1 4 2 5
        //1.15%: 0 4 1 5
        //1.16%: 0 3 2 4
        //1.2%: 0 4 2 5
        //1.2%: 0 4 2 4
        double dSysFCCallStrike = HSIDayClose + pc.vs.ATR * 0;
        int iSysFCCallStrike = (int)Math.Round(dSysFCCallStrike / 200, 0) * 200;
        double dSysFCCallStrike2 = HSIDayClose + pc.vs.ATR * 3;
        int iSysFCCallStrike2 = (int)Math.Round(dSysFCCallStrike2 / 200, 0) * 200;
        double dSysFCPutStrike = HSIDayClose - pc.vs.ATR * 2;
        int iSysFCPutStrike = (int)Math.Round(dSysFCPutStrike / 200, 0) * 200;
        double dSysFCPutStrike2 = HSIDayClose - pc.vs.ATR * 4;
        int iSysFCPutStrike2 = (int)Math.Round(dSysFCPutStrike2 / 200, 0) * 200;

        //System SE
        //Friction = 0
        //4.99%: 2 4 1 3
        //5.11%: 1 3 2 3
        //5.33%: 1.5 3 2 3
        //5.41%: 2 3 2 3
        //5.42%: 2 4 2 5
        //5.44%: 3 4 2 3
        //5.46%: 3 5 2 3
        //5.50%: 2 4 2 4
        //5.55%: 2 4 2 3
        //5.67%: 2 4 3 5
        //5.77%: 2 4 3 4
        //5.90%: 2 4 4 5

        //Friction = 2
        //5.77%: 2 4 4 5
        double dSysSECallStrike = HSIDayClose + pc.vs.ATR * 2;
        int iSysSECallStrike = (int)Math.Round(dSysSECallStrike / 200, 0) * 200;
        double dSysSECallStrike2 = hlh5.HighestHigh + pc.vs.ATR * 4;
        int iSysSECallStrike2 = (int)Math.Round(dSysSECallStrike2 / 200, 0) * 200;
        double dSysSEPutStrike = HSIDayClose - pc.vs.ATR * 4;
        int iSysSEPutStrike = (int)Math.Round(dSysSEPutStrike / 200, 0) * 200;
        double dSysSEPutStrike2 = hlh5.LowestLow - pc.vs.ATR * 5;
        int iSysSEPutStrike2 = (int)Math.Round(dSysSEPutStrike2 / 200, 0) * 200;

        //System BPS
        //4.44%
        double dSysBPSPutStrike = HSIDayClose - pc.vs.ATR * 1;
        int iSysBPSPutStrike = (int)Math.Round(dSysBPSPutStrike / 200, 0) * 200;
        if (iSysBPSPutStrike == iATMStrike) iSysBPSPutStrike -= 200;

        double dSysBPSPutStrike2 = HSIDayClose - pc.vs.ATR * 2;
        int iSysBPSPutStrike2 = (int)Math.Round(dSysBPSPutStrike2 / 200, 0) * 200;
        if (iSysBPSPutStrike2 == iSysBPSPutStrike) iSysBPSPutStrike2 -= 200;

        //System DR
        //Friction = 0
        //2.90%: 3 4
        //3.35%: 2 4
        //4.59%: 1 3
        //4.84%: 0 2
        //6.32%: 0 1
        double dsysDRCallStrike = HSIDayClose + pc.vs.ATR * 0;
        int isysDRCallStrike = (int)Math.Round(dsysDRCallStrike / 200, 0) * 200;
        if (isysDRCallStrike == iATMStrike) isysDRCallStrike += 200;
        double dsysDRCallStrike2 = HSIDayClose + pc.vs.ATR * 1;
        int isysDRCallStrike2 = (int)Math.Round(dsysDRCallStrike2 / 200, 0) * 200;
        if (isysDRCallStrike2 == isysDRCallStrike) isysDRCallStrike2 += 200;

        //System SOSC
        double dsysSOSCCallStrike = HSIDayClose + pc.vs.ATR * 1;
        int isysSOSCCallStrike = (int)Math.Round(dsysSOSCCallStrike / 200, 0) * 200;
        if (isysSOSCCallStrike == iATMStrike) isysSOSCCallStrike += 200;

        double dsysSOSCCallStrike2 = HSIDayClose + pc.vs.ATR * 3;
        int isysSOSCCallStrike2 = (int)Math.Round(dsysSOSCCallStrike2 / 200, 0) * 200;
        if (isysSOSCCallStrike2 == isysSOSCCallStrike) isysSOSCCallStrike2 += 200;

        //System SOSP
        double dsysSOSPPutStrike = HSIDayClose - pc.vs.ATR * 6;
        int isysSOSPPutStrike = (int)Math.Round(dsysSOSPPutStrike / 200, 0) * 200;
        double dsysSOSPPutStrike2 = HSIDayClose - pc.vs.ATR * 4;
        int isysSOSPPutStrike2 = (int)Math.Round(dsysSOSPPutStrike2 / 200, 0) * 200;

        //System DMKL
        double dsysDMKLCallStrike = HSIDayHigh + pc.vs.ATR * 1;
        int isysDMKLCallStrike = (int)Math.Round(dsysDMKLCallStrike / 200, 0) * 200;
        double dsysDMKLPutStrike = HSIDayLow - pc.vs.ATR * 1;
        int isysDMKLPutStrike = (int)Math.Round(dsysDMKLPutStrike / 200, 0) * 200;

        //System DMKS
        double dsysDMKSCallStrike = HSIDayHigh + pc.vs.ATR * 2;
        int isysDMKSCallStrike = (int)Math.Round(dsysDMKSCallStrike / 200, 0) * 200;
        double dsysDMKSCallStrike2 = HSIDayHigh + pc.vs.ATR * 2;
        int isysDMKSCallStrike2 = (int)Math.Round(dsysDMKSCallStrike2 / 200, 0) * 200;
        double dsysDMKSPutStrike = HSIDayLow - pc.vs.ATR * 1;
        int isysDMKSPutStrike = (int)Math.Round(dsysDMKSPutStrike / 200, 0) * 200;

        //System HLP
        double dsysHLPPutStrike = HSIDayClose - pc.vs.ATR * 3;
        int isysHLPPutStrike = (int)Math.Round(dsysSOSPPutStrike / 200, 0) * 200;
        double dsysHLPPutStrike2 = HSIDayClose - pc.vs.ATR * 6;
        int isysHLPPutStrike2 = (int)Math.Round(dsysSOSPPutStrike2 / 200, 0) * 200;


        //----------------------------------------------------------------------------------
        //System CT
        if (IParam.PosSize[(int)IParam.SYSNAME.CT] > 0)
        {
            string sIden = "CT";
            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.CT] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            bool bSysCTPreCon = false;
            if ((CurrentTime.Month == 1 && CurrentTime.Day > 15) ||
                (CurrentTime.Month == 2 && CurrentTime.Day > 15) ||
                (CurrentTime.Month == 3 && CurrentTime.Day > 15) ||
                (CurrentTime.Month == 6) ||
                (CurrentTime.Month == 7 && CurrentTime.Day > 15) ||
                (CurrentTime.Month == 9 && CurrentTime.Day > 15) ||
                (CurrentTime.Month == 10 && CurrentTime.Day > 15) ||
                (CurrentTime.Month == 12)) bSysCTPreCon = true;
            if (CurrentTime.Day >= 25) bSysCTPreCon = false;
            if (pc.vs.ATR / HSIDayClose > 0.025) bSysCTPreCon = false;

            if (HSIDayClose < sma5d.SMA && HSIDayClose < sma10d.SMA &&
                bSysCTPreCon && iSysCTLatestDecisn <= 0)
            {
                pc.TradeOpt(false, true, iSysCTCallStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(true, true, iSysCTCallStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                iSysCTLatestDecisn = -1;
            }
            else if (HSIDayClose > sma5d.SMA && HSIDayClose > sma10d.SMA &&
                bSysCTPreCon && iSysCTLatestDecisn >= 0)
            {
                pc.TradeOpt(false, false, iSysCTPutStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(true, false, iSysCTPutStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                iSysCTLatestDecisn = 1;
            }
            else iSysCTLatestDecisn = 0;
        }

        //System TR
        if (IParam.PosSize[(int)IParam.SYSNAME.TR] > 0)
        {
            string sIden = "TR";

            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.TR] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            bool bSysTRPreCon = false;
            if ((CurrentTime.Month == 4 && CurrentTime.Day > 15) ||
                (CurrentTime.Month == 5) ||
                (CurrentTime.Month == 10) ||
                (CurrentTime.Month == 11))
                bSysTRPreCon = true;
            if (pc.vs.ATR / HSIDayClose > 0.018) bSysTRPreCon = false;


            if (HSIDayClose > sma3d.SMA && sma20d.SMA < sma20d.Previous && sma50d.SMA < sma50d.Previous && bSysTRPreCon)
            {
                pc.TradeOpt(true, true, iSysTRCallStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(false, true, iSysTRCallStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
            else if (HSIDayClose < sma5d.SMA && sma20d.SMA > sma20d.Previous && sma50d.SMA > sma50d.Previous && bSysTRPreCon)
            {
                pc.TradeOpt(true, false, iSysTRPutStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(false, false, iSysTRPutStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
        }

        //System FMD
        if (IParam.PosSize[(int)IParam.SYSNAME.FMD] > 0)
        {
            string sIden = "FMD";

            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.FMD] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            bool bSysFMDPreCon = false;
            if (CurrentTime.Month == 2 || CurrentTime.Month == 3 || CurrentTime.Month == 12) bSysFMDPreCon = true;
            if (CurrentTime.Day >= 25) bSysFMDPreCon = false;

            if (HSIDayClose > sma5d.SMA && HSIDayClose > sma10d.SMA && bSysFMDPreCon)
            {
                pc.TradeOpt(false, false, iSysFMDPutStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(true, false, iSysFMDPutStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
            else if (HSIDayClose < sma5d.SMA && HSIDayClose < sma10d.SMA && bSysFMDPreCon)
            {
                pc.TradeOpt(false, true, iSysFMDCallStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(true, true, iSysFMDCallStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
        }

        //System GUT
        if (IParam.PosSize[(int)IParam.SYSNAME.GUT] > 0)
        {
            string sIden = "GUT";

            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.GUT] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            bool bSysGUTPreCon = false;
            if ((CurrentTime.Month == 1) ||
                (CurrentTime.Month == 5) ||
                (CurrentTime.Month == 9) ||
                (CurrentTime.Month == 10) ||
                (CurrentTime.Month == 11)) bSysGUTPreCon = true;
            if (CurrentTime.Day >= 25) bSysGUTPreCon = false;
            if (pc.vs.VHSILatest > pc.vs.EIV0_est) bSysGUTPreCon = false;
            if (wr3.WR <= -50) bSysGUTPreCon = false;

            if ((HSIDayClose > sma3d.SMA || HSIDayClose > sma5d.SMA) && (HSIDayClose > sma20d.SMA || HSIDayClose > sma50d.SMA) && bSysGUTPreCon)
            {
                pc.TradeOpt(false, false, iSysGUTPutStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
            else
            {
                pc.CloseOptPos(HSIDayClose, HSIDayClose, CurrentTime, false, IParam.NOFRICTION, sIden);
            }
        }



        //System CS
        if (IParam.PosSize[(int)IParam.SYSNAME.CS] > 0)
        {
            string sIden = "CS";

            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.CS] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            bool bSysCSPreCon = true;
            if (CurrentTime.Day >= 25) bSysCSPreCon = false;
            if (pc.vs.ATR / HSIDayClose > 0.012) bSysCSPreCon = false;
            if (smaATR_P3.SMA > smaATR_P3.Previous) bSysCSPreCon = false;

            if (CurrentTime.Day >= 25)
            {
                pc.CloseOptPos(HSIDayClose, HSIDayClose, CurrentTime, false, IParam.NOFRICTION, sIden);
            }
            else if (bSysCSPreCon)
            {
                pc.TradeOpt(false, true, iATMStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(true, true, iATMStrike, CurrentTime, sFutCode2, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
        }

        //System FC
        if (IParam.PosSize[(int)IParam.SYSNAME.FC] > 0)
        {
            string sIden = "FC";

            double dSize = 1;
            if (HSIDayLow < hlh13.HighestHigh * 0.92) dSize = 1.3;
            else if (HSIDayLow < hlh6.HighestHigh * 0.95) dSize = 1;
            else dSize = 0;

            double dPosSize = dSize * IParam.PosSize[(int)IParam.SYSNAME.FC] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            bool bSysFCPreCon = true;
            if (CurrentTime.Day >= 25) bSysFCPreCon = false;
            if (pc.vs.VHSILatest < pc.vs.EIV0_est * 1.12) bSysFCPreCon = false;

            if (bSysFCPreCon)
            {
                pc.TradeOpt(true, true, iSysFCCallStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(false, true, iSysFCCallStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(false, false, iSysFCPutStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
        }

        //System SE
        if (IParam.PosSize[(int)IParam.SYSNAME.SE] > 0)
        {
            string sIden = "SE";

            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.SE] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            bool bSysSEPreCon = true;
            if (CurrentTime.Month == 9 ||
                CurrentTime.Month == 10 ||
                CurrentTime.Month == 12)
                bSysSEPreCon = false;

            if (histdh.GetCalendarTTMHK(CurrentTime, 1) < (double)6 / 365) bSysSEPreCon = false;

            if (HSIDayClose > hlh5.LowestLow + 2 * pc.vs.ATR && hlh5.Stable && hlh5.HighestHigh > 0 && bSysSEPreCon)
            {
                pc.TradeOpt(false, true, iSysSECallStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(false, false, iSysSEPutStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
            else if (HSIDayClose < hlh5.HighestHigh - 2 * pc.vs.ATR && hlh5.Stable && hlh5.HighestHigh > 0 && bSysSEPreCon)
            {
                pc.TradeOpt(false, false, iSysSEPutStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                pc.TradeOpt(false, true, iSysSECallStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
        }


        //System DR
        if (IParam.PosSize[(int)IParam.SYSNAME.DR] > 0)
        {
            string sIden = "DR";
            int iBoundaryDay = 20;

            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.DR] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            bool bsysDRPreCon2 = false;
            if (ldHSI_C.Count > 50)
                if (CurrentTime.Day <= iBoundaryDay &&
                    HSIDayClose > sma5d.SMA &&
                    HSIDayClose < hlh50.LowestLow + hlh50.HLRange / 2
                    ) bsysDRPreCon2 = true;

            bool bsysDRPreCon3 = false;
            if (ldHSI_C.Count > 50)
                if (CurrentTime.Day <= iBoundaryDay &&
                    HSIDayClose > hlh10.LowestLow + hlh10.HLRange / 2 &&
                    HSIDayClose < hlh50.LowestLow + hlh50.HLRange / 2
                    ) bsysDRPreCon3 = true;


            bool bsysDRPreCon4 = false;
            if (ldHSI_C.Count > 50)
                if (CurrentTime.Day <= iBoundaryDay &&
                    HSIDayClose > hlh5.LowestLow + hlh5.HLRange / 2 &&
                    HSIDayClose < hlh50.LowestLow + hlh50.HLRange / 2
                    ) bsysDRPreCon4 = true;

            if (CurrentTime.Day > iBoundaryDay)
            {
                pc.CloseOptPos(HSIDayClose, HSIDayClose, CurrentTime, false, IParam.NOFRICTION, sIden);
            }
            else
            {
                //debit
                if (bsysDRPreCon2)
                {
                    pc.TradeOpt(true, true, isysDRCallStrike, CurrentTime, sFutCode, 2 * iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                    pc.TradeOpt(false, true, isysDRCallStrike2, CurrentTime, sFutCode, 2 * iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                }
                //debit
                if (bsysDRPreCon3)
                {
                    pc.TradeOpt(true, true, isysDRCallStrike, CurrentTime, sFutCode, 1 * iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                    pc.TradeOpt(false, true, isysDRCallStrike2, CurrentTime, sFutCode, 1 * iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                }
                //credit
                if (bsysDRPreCon4)
                {
                    pc.TradeOpt(false, true, isysDRCallStrike, CurrentTime, sFutCode, 4 * iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                    pc.TradeOpt(true, true, isysDRCallStrike2, CurrentTime, sFutCode, 4 * iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                }
            }
        }

        //System EV
        if (IParam.PosSize[(int)IParam.SYSNAME.EV] > 0)
        {
            //string sIden = "EV";

            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.EV] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);


            if (pc.vs.VHSILatest < pc.vs.EIV0 && pc.vs.VHSILatest > pc.vs.EIV0 * 0.98)
            {
                pc.TradeFut(true, iPosSize, HSIDayClose, IParam.NOFRICTION);
            }
            else if (pc.vs.VHSILatest > pc.vs.EIV0 && pc.vs.VHSILatest < pc.vs.EIV0 * 1.02)
            {
                pc.TradeFut(false, iPosSize, HSIDayClose, IParam.NOFRICTION);
            }
            else
            {
                pc.CloseFutPos(HSIDayClose, IParam.NOFRICTION);
            }
        }


        //System SOSC
        if (IParam.PosSize[(int)IParam.SYSNAME.SOSC] > 0)
        {
            string sIden = "SOSC";
            int iBoundaryDay = 23;

            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.SOSC] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            if (ldHSI_C.Count > 50 && CurrentTime.Day < iBoundaryDay && je.GetSSineWave() > 0.8)
            {
                pc.TradeOpt(false, true, isysSOSCCallStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
            else if (je.GetSSineWave() > 0 && je.GetSSineWave() < 0.6)
            {
                pc.CloseOptPos(HSIDayClose, HSIDayClose, CurrentTime, false, IParam.NOFRICTION, sIden);
            }
        }

        //System SOSP
        if (IParam.PosSize[(int)IParam.SYSNAME.SOSP] > 0)
        {
            string sIden = "SOSP";
            int iBoundaryDay = 23;

            double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.SOSP] / HSIDayClose;
            int iPosSize = (int)Math.Round(dPosSize, 0);

            if (ldHSI_C.Count > 50 && CurrentTime.Day < iBoundaryDay && je.GetSSineWave() < -0.6)
            {
                pc.TradeOpt(false, false, isysSOSPPutStrike2, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
            else if (ldHSI_C.Count > 50 && CurrentTime.Day < iBoundaryDay && je.GetSSineWave() < -0.8)
            {
                pc.TradeOpt(false, false, isysSOSPPutStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
            }
            else if (je.GetSSineWave() > 0 && je.GetSSineWave() < 0.6)
            {
                pc.CloseOptPos(HSIDayClose, HSIDayClose, CurrentTime, false, IParam.NOFRICTION, sIden);
            }
        }

        //System DMKL
        if (IParam.PosSize[(int)IParam.SYSNAME.DMKL] > 0)
        {
            string sIden = "DMKL";
            int iDMKLn1 = 4; //n1 bars ago
            int iDMKLn2 = 6; //consecutive lower closes
            int iDMKLcd = 5; //countdown

            if (ldHSI_C.Count > iDMKLn1 + iDMKLn2 + 2)
            {
                double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.DMKL] / HSIDayClose;
                int iPosSize = (int)Math.Round(dPosSize, 0);

                bool bN2ConseLowerClose = true;
                for (int i = 0; i < iDMKLn2; i++)
                    if (ldHSI_C[ldHSI_C.Count - 1 - i] > ldHSI_C[ldHSI_C.Count - 1 - i - iDMKLn1])
                        bN2ConseLowerClose = false;

                if (bN2ConseLowerClose)
                {
                    bSysDMKLSetup = true;
                    iSysDMKLCountDw = 1; //buy count down reset to 1, whether it's a new countdown or a nullified previous countdown
                    dSysDMKL1stCntDwClose = ldHSI_C[ldHSI_C.Count - 1];
                    pc.CloseOptPos(HSIDayClose, HSIDayClose, CurrentTime, false, IParam.NOFRICTION, sIden);
                }

                if (bSysDMKLSetup &&
                    (ldHSI_C[ldHSI_C.Count - 1] >= hlh2.HighestHigh ||
                    ldHSI_H[ldHSI_H.Count - 1] > ldHSI_H[ldHSI_H.Count - 2] ||
                    ldHSI_C[ldHSI_C.Count - 1] > dSysDMKL1stCntDwClose))
                {
                    iSysDMKLCountDw++;
                }

                if (iSysDMKLCountDw >= iDMKLcd)
                {
                    pc.TradeOpt(false, false, isysDMKLPutStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                    pc.TradeOpt(false, true, isysDMKLCallStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                    
                    bSysDMKLSetup = false;
                    iSysDMKLCountDw = 0;
                    dSysDMKL1stCntDwClose = double.NaN;
                }
            }
        }

        //System DMKS
        if (IParam.PosSize[(int)IParam.SYSNAME.DMKS] > 0)
        {
            string sIden = "DMKS";
            int iDMKSn1 = 4; //n1 bars ago
            int iDMKSn2 = 5; //consecutive higher closes
            int iDMKScd = 5; //countdown
            //4,5,5 not the highest E[R] combination, but the equity curve is the most straight

            if (ldHSI_C.Count > iDMKSn1 + iDMKSn2 + 2)
            {
                double dPosSize = IParam.PosSize[(int)IParam.SYSNAME.DMKS] / HSIDayClose;
                int iPosSize = (int)Math.Round(dPosSize, 0);

                bool bN2ConseHigherClose = true;
                for (int i = 0; i < iDMKSn2; i++)
                    if (ldHSI_C[ldHSI_C.Count - 1 - i] < ldHSI_C[ldHSI_C.Count - 1 - i - iDMKSn1])
                        bN2ConseHigherClose = false;

                if (bN2ConseHigherClose)
                {
                    bSysDMKSSetup = true;
                    iSysDMKSCountDw = 1; //buy count down reset to 1, whether it's a new countdown or a nullified previous countdown
                    dSysDMKS1stCntDwClose = ldHSI_C[ldHSI_C.Count - 1];
                    pc.CloseOptPos(HSIDayClose, HSIDayClose, CurrentTime, false, IParam.NOFRICTION, sIden);
                }

                if (bSysDMKSSetup &&
                    (ldHSI_C[ldHSI_C.Count - 1] <= hlh2.LowestLow ||
                    ldHSI_H[ldHSI_H.Count - 1] < ldHSI_H[ldHSI_H.Count - 2] ||
                    ldHSI_C[ldHSI_C.Count - 1] < dSysDMKS1stCntDwClose))
                {
                    iSysDMKSCountDw++;
                }

                if (iSysDMKSCountDw >= iDMKScd)
                {
                    pc.TradeOpt(false, false, isysDMKSPutStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);
                    pc.TradeOpt(true, true, isysDMKSCallStrike, CurrentTime, sFutCode, iPosSize, HSIDayClose, HSIDayClose, false, IParam.NOFRICTION, sIden);

                    bSysDMKSSetup = false;
                    iSysDMKSCountDw = 0;
                    dSysDMKS1stCntDwClose = double.NaN;
                }
            }
        }

        RunSystemB(CurrentTime, HSIDayClose);

        pc.OutputPositionInfoClose(CurrentTime, HSIDayClose, HSIDayClose, 2);

        //for information retrieval purpose
        _LatestPortDelta = pc.GetPortDelta(CurrentTime, HSIDayClose, HSIDayClose);
        _LatestPortGamma = pc.GetPortOptGamma(CurrentTime, HSIDayClose, HSIDayClose);
        _LatestPrice = HSIDayClose;
        _LatestTime = CurrentTime;
        _LatestVHSI = Volatility;
        _LatestATR = pc.vs.ATR;
        //_LatestEIV = pc.vs.EIV0;
        _LatestEIV = pc.vs.EIV0a;
        _State_FSM1 = GetFSM1State(HSIDayClose);
        _State_FSM2 = GetFSM2State(HSIDayClose);

    }


}

