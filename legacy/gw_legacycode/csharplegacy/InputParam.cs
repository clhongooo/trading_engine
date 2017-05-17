using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

public class IParam
{
    //unstable parameters
    //Genetic Optimization
    public static bool DRAWCHROMWITHREPLACEMENT = true; //allows more than 1 mutation to each
    public static int GALAXYMAX = 4;
    public static int CHROMOSOMEMAX = -1; //dummy, set elsewhere
    public static int WORLDMAX = -1; //dummy, set elsewhere
    public static int EVOLUTIONSTAGEMAX = -1; //dummy, set elsewhere

    public static int MAXDEVIATIONFRMMYPORT = Int16.MinValue; //dummy, set elsewhere
    public static double DEVIATIONFRMMYPORTFACTOR = double.NaN; //dummy, set elsewhere
    public static int MAXNUMCONTRACTCODE = Int16.MinValue; //dummy, set elsewhere
    public static int MAXNUMCONTRACT = Int16.MinValue; //dummy, set elsewhere
    public static int MAXNUMFUTCONTRACT = Int16.MinValue; //dummy, set elsewhere
    public static bool FORCEINCLUDEFUT = false;

    public static double CAPITAL = 70000; //in points, because our PnL is in points
    public static double HOWMUCHKELLY = 1/20d; //0.5 = half kelly
    public static double PSNSZLIM_MAXLOSSPCT = -3000/CAPITAL; //in points, mini contract
    public static double KELLYALPHAHARDCAP = CAPITAL / 2000; //2000 coz in points, mini contract
                                                         //e.g. CAPITAL=700K, 1 mini contract 20K initial margin

    public static int OPTNAKEDSHORTCAP = -10; //TWEAK //in terms of mini contracts
    public static int FRONTMTHNESS = 1; //TWEAK //ROLL
    //1 = 1st front month contracts only
    //2 = 2nd front month contracts only
    //3 = both 1st and 2nd front month contracts

    public static int RUNMODE = 1; //1 = full scale, 2 = adjusting MyPort, 3 = quick full scale
    public static void GeneticOptimizationRunMode()
    {
        if (IParam.RUNMODE == 1 || IParam.RUNMODE == 3)
        {
            IParam.BestPortFilePath = "bestportRM1.csv";
            IParam.MAXDEVIATIONFRMMYPORT = 1000; //TWEAK //allow as much freedom as possible
            IParam.DEVIATIONFRMMYPORTFACTOR = 0; //TWEAK
            IParam.MAXNUMCONTRACTCODE = 3; //TWEAK
            IParam.MAXNUMCONTRACT = IParam.MAXDEVIATIONFRMMYPORT; //TWEAK
            IParam.MAXNUMFUTCONTRACT = 2; //TWEAK
        }
        else if (IParam.RUNMODE == 2)
        {
            IParam.BestPortFilePath = "bestportRM2.csv";
            IParam.MAXDEVIATIONFRMMYPORT = 30; //TWEAK //around 6 HSI options
            IParam.DEVIATIONFRMMYPORTFACTOR = 0.5; //TWEAK //a tiny bit of resistance
            IParam.MAXNUMCONTRACTCODE = 50; //TWEAK
            IParam.MAXNUMCONTRACT = 1000; //TWEAK
            IParam.MAXNUMFUTCONTRACT = 2; //TWEAK
        }
    }
    public static void SmartUnvsWrldStage(int iGalaxy)
    {
        if (IParam.RUNMODE == 3) { WORLDMAX = 2; EVOLUTIONSTAGEMAX = 5; CHROMOSOMEMAX = 30; }
        else
        {
            if (iGalaxy == 0) { WORLDMAX = 15; EVOLUTIONSTAGEMAX = 2; CHROMOSOMEMAX = 60; }
            else if (iGalaxy == 1) { WORLDMAX = 2; EVOLUTIONSTAGEMAX = 10; CHROMOSOMEMAX = 60; }
            else if (iGalaxy == 2) { WORLDMAX = 1; EVOLUTIONSTAGEMAX = 50; CHROMOSOMEMAX = 60; }

            //preliminary scan, if both 1st and 2nd front mth, wider range!
            if (FRONTMTHNESS == 3) WORLDMAX *= 2;
        }
    }

    public static double CalcTgtRtn(double dScore)
    {
        double dRtn = 138;
        //empirical
        //TWEAK
        //if (dScore > 0.3) dRtn = 200;
        //else if (dScore > 0.2) dRtn = 150;
        //else if (dScore > 0.1) dRtn = 100;
        //else if (dScore > 0.04634) dRtn = 71;
        //else if (dScore > 0.035) dRtn = 66;
        //else if (dScore > 0.0175) dRtn = 54;
        //else if (dScore > 0.0083) dRtn = 49;
        //else if (dScore > 0.006225) dRtn = 43;
        //else if (dScore > 0.00415) dRtn = 35;
        //else if (dScore > 0.002075) dRtn = 30;
        //else if (dScore > 0.001038) dRtn = 20;

        return dRtn;
    }

    //stable parameters
    //-----------------------------------------------------------------------------
    //input files
    public static string BestPortFilePath = null;
    public static string DataNewFilePath = "data-new.csv";
    public static string DataOldFilePath = "data-old.csv";
    public static string MyPortFilePath = "myport.csv";
    public static string DefaultPortFilePath = "defaultport.csv";
    public static string PDFFilePath = "INPUTDATA/pdf.csv";
    public static string FiniteStateMachine1PDFFilePath = "INPUTDATA/fsm1pdf.csv";
    public static string FiniteStateMachine2PDFFilePath = "INPUTDATA/fsm2pdf.csv";
    public static string TranchDeltaFilePath = "INPUTDATA/tranches-delta.csv";
    public static string TranchGammaFilePath = "INPUTDATA/tranches-gamma.csv";
    public static string RawDataOpt1FilePath = "INPUTDATA/rawdata-opt-1.csv";
    public static string RawDataFut1FilePath = "INPUTDATA/rawdata-fut-1.csv";
    public static string RawDataOpt2FilePath = "INPUTDATA/rawdata-opt-2.csv";
    public static string RawDataFut2FilePath = "INPUTDATA/rawdata-fut-2.csv";
    public static string VolSmileMthStartFilePath = "OUTPUTDATA/volatilitysmile-MthStart.csv";


    //output files
    public static string DetailFilePath = "detail.txt";
    public static string HSCIEXHSIFilePath = "HSCIEXHSI.csv";
    public static string GeneticResultFilePath = "genetic.txt";
    public static string ScenarioFilePath = "OUTPUTDATA/scenario.csv";
    public static string ScenarioPDFSummaryFilePath = "OUTPUTDATA/scenario-PDFsummary.csv";
    public static string AssessFilePath = "assess.txt";
    public static string ImpliedRNPDFStatFilePath = "OUTPUTDATA/impliedPDF-stat.txt";
    public static string VolSmileMethFilePath1 = "OUTPUTDATA/volsmileMeth1.txt";
    public static string VolSmileMethFilePath2 = "OUTPUTDATA/volsmileMeth2.txt";
    public static string VSImpliedPDF1FilePath = "OUTPUTDATA/impliedPDF-1.csv";
    public static string VSImpliedPDF2FilePath = "OUTPUTDATA/impliedPDF-2.csv";
    public static string VolSmile1FilePath = "OUTPUTDATA/volatilitysmile-1.csv";
    public static string VolSmile2FilePath = "OUTPUTDATA/volatilitysmile-2.csv";
    public static string VolSmileScenarioPart1FilePath = "OUTPUTDATA/volatilitysmile-sc-";
    public static string VolSmileScenarioPart2FilePath = ".csv";
    public static string VolSmile1RuggedFilePath = "OUTPUTDATA/volatilitysmile-rugged-1.csv";
    public static string VolSmile2RuggedFilePath = "OUTPUTDATA/volatilitysmile-rugged-2.csv";



    //genetic
    public static int SCENARIOMAX = 13;

    private static int NOOFSYS = 1000; //max num of sub-systems

    public static int[] PosSize = new int[NOOFSYS];
    public static void ResetPosSize()
    {
        for (int i = 0; i < NOOFSYS; i++) PosSize[i] = 0;
    }


    //-----------------------------------------------------------------------------
    //subsystems
    //-----------------------------------------------------------------------------
    public enum SYSNAME
    {
        TR = 0, CT, FMD, GUT, SE, FC, CS, DR,
        EV, SOSC, SOSP, DMKL, DMKS,
        B3, B5, B10, BZ, B1DHL,
        GSSU, GSSD, GSWU, GSWD, GWSU, GWSD, GWWU, GWWD,
        OPTIMAL
    };
    public static void LoadOptimal()
    {
        ResetPosSize();

        int i = 0;
        PosSize[i] = 229091; i++;//TR
        PosSize[i] = 363277; i++;//CT
        PosSize[i] = 791051; i++;//FMD
        PosSize[i] = 871049; i++;//GUT
        PosSize[i] = 179411; i++;//SE
        PosSize[i] = 10448; i++;//FC
        PosSize[i] = 1003149; i++;//CS
        PosSize[i] = 669085; i++;//DR
        PosSize[i] = 123245; i++;//EV
        PosSize[i] = 287747; i++;//SOSC
        PosSize[i] = 285318; i++;//SOSP
        PosSize[i] = 179403; i++;//DMKL
        PosSize[i] = 140197; i++;//DMKS
        PosSize[i] = 115783; i++;//B3
        PosSize[i] = 305029; i++;//B5
        PosSize[i] = 388725; i++;//B10
        PosSize[i] = 0; i++;//BZ
        PosSize[i] = 0; i++;//B1DHL
        PosSize[i] = 0; i++;//GSSU
        PosSize[i] = 0; i++;//GSSD
        PosSize[i] = 0; i++;//GSWU
        PosSize[i] = 0; i++;//GSWD
        PosSize[i] = 0; i++;//GWSU
        PosSize[i] = 0; i++;//GWSD
        PosSize[i] = 0; i++;//GWWU
        PosSize[i] = 0; i++;//GWWD
        PosSize[i] = 0; i++;//OPTIMAL
    }


    public static int CLOSEBELOW = 0;

    //-----------------------------------------------------------------------------
    //TWEAK
    //-----general
    public static int NOFRICTION = 0; //specially for the basic systems which only aim at getting the pdf
    //-----for futures friction
    //public static int FRICTION = 30; //30 = assuming 29 pt slippage + 1 pt commission
    public static int FRICTION = 20; //20 = assuming 19 pt slippage + 1 pt commission
    //public static int FRICTION = 10; //10 = assuming 9 pt slippage + 1 pt commission
    //public static int FRICTION = 0; //fictional!!!
    //-----for option smart friction (start)
    //public static double FRICTIONPCTG = 0.05;
    //public static double FRICTIONPCTG = 0.038;
    public static double FRICTIONPCTG = 0.02;
    //public static double FRICTIONPCTG = 0.015;
    //public static double FRICTIONPCTG = 0.00; //fictional!!!
    public static int FRICTIONTRXCOST = 1;
    //public static int FRICTIONTRXCOST = 0; //fictional!!!
    //-----for smart friction (end)


    //--------------------------------------------------------------------------------
    //parameters related to the period of accuracy of the market implied PDFs
    //TWEAK all
    //--------------------------------------------------------------------------------
    //set to double.NaN to make sure it crashes out if LoadParamSetA is not called first.
    public static double HISTPDFWEIGHTING = double.NaN;
    public static double MODELPDFWEIGHTING = double.NaN;
    public static double FSM1PDFWEIGHTING = double.NaN;
    public static double FSM2PDFWEIGHTING = double.NaN;

    public static double IMPL1stPDFWEIGHTING = double.NaN;
    public static double IMPL2ndPDFWEIGHTING = double.NaN;
    public static int SCENARIOENDDAY = 1; //TWEAK
    public static int FARTHESTSTRIKE = -1;

    public static DateTime dtDateConcerned;
    public static void LoadParamSetA()
    {
        try
        {
            using (StreamReader sr = new StreamReader(IParam.RawDataFut1FilePath))
            {
                string sLine;
                sLine = sr.ReadLine();
                sLine = sr.ReadLine();

                dtDateConcerned = SymbolCodeHelper.ConvertYYYYMMDD(sLine);
            }


            MODELPDFWEIGHTING = 0.60; FSM1PDFWEIGHTING = 0.15; FSM2PDFWEIGHTING = 0.25; //adding up to 100%
            HISTPDFWEIGHTING = 0.50;
            IMPL1stPDFWEIGHTING = 0.50; IMPL2ndPDFWEIGHTING = 0;

            if (dtDateConcerned.Day > 25) FARTHESTSTRIKE = 800;
            else if (dtDateConcerned.Day > 20) FARTHESTSTRIKE = 1100;
            else if (dtDateConcerned.Day > 15) FARTHESTSTRIKE = 1400;
            else if (dtDateConcerned.Day > 10) FARTHESTSTRIKE = 1700;
            else FARTHESTSTRIKE = 2000;


        }
        catch (Exception e)
        {
            Console.WriteLine("Raw Data file could not be read:");
            Console.WriteLine(e.Message);
            Environment.Exit(0);
        }

    }



    //--------------------------------------------------------------------------------
    //volatility surface calculations
    //--------------------------------------------------------------------------------
    public static bool ENABLEMETH0 = true;
    public static bool ENABLEMETH1 = false;
    public static bool ENABLEMETH3 = false;

    //option
    public static double rf = 0.0021; //0.21%, Oct 2012
    //public static double dy = 0.0336; //mostly not using div yield in calculation because we are using futures
                                        //HSI, 3.36%, Oct 2012

    public static int OPTSTRIKEINTVL = 200;
    //minimum number of data points to expect in the volatility smile data
    public static int MINPTVOLSMILE = 5;  //TWEAK //don't want too few points, but as maturity approaches there won't be too many points
    public static int MAXVOLSMILEBIDASKSPRD = 10; //TWEAK //if the spread is too large don't use it for smile calculation
    //maximum percentage deviation from the linearly interpolation when deciding if the data pt is good

    public static int IMPLRNPDFFATTAILSUBSIDE = 1000;
    public static int VOLSMILEREGENSTART = 16000; //ROLL
    public static int VOLSMILEREGENEND = 26000; //ROLL
    public static int VOLSMILEINTERVAL = 10; //must be factor of 200, e.g. 30 not ok, but 10 or 20 or 50 ok

    public static double UGLYHARDCODEUNDPX = 20000;
    public static DateTime UGLYHARDCODEDATE = new DateTime(2000,1,1);
    public static double KURTOSISSCALINGFACTOR = 100;

    //------------------------------------------------------------------------------------------
    // Depot files:
    // 1. As a general place to store data.
    // 2. Hope to reduce file I/O.
    //------------------------------------------------------------------------------------------
    public class DataDepotVolSmile
    {
        public static DateTime dtVolSmileDate;
        public static double dATMVolSmile1 = 0;
        public static double dATMVolSmile2 = 0;
        public static double dVHSIVolSmile1 = 0;
        public static double dVHSIVolSmile2 = 0;
        public static Hashtable htVolSmile1 = new Hashtable();
        public static Hashtable htVolSmile2 = new Hashtable();
        public static Hashtable htRuggedVolSmile1 = new Hashtable();
        public static Hashtable htRuggedVolSmile2 = new Hashtable();
        public static double dMthStartVolSmileTTM = 0;
        public static DateTime dtMthStartDate;
        public static double dMthStartATMVolSmile1 = 0;
        public static Hashtable htMthStartVolSmile1 = new Hashtable();
        public static SortedList lhtScVolSmile1 = new SortedList();

    }
    public class DataDepotLoadPort
    {
        public static Hashtable htFileNameContent = new Hashtable(); //string filename, List<string> filecontent
    }




}
