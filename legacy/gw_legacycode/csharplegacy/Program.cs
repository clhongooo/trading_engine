using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace OptionMedley
{

    class Program
    {
        public static void RunOptionMedley(RuleBasedDayTradeSys rbdts, string sDataFile)
        {
            RunOptionMedley(rbdts, sDataFile, null);
        }

        public static void RunOptionMedley(RuleBasedDayTradeSys rbdts, string sDataFile, List<double> ldPnL)
        {
            DayBarProvider dbp = new DayBarProvider();
            dbp.ReadFile(sDataFile);

            Bar barHSIDayBar = null;
            Bar barHSCIDayBar = null;
            Console.WriteLine("The program has read:");
            while (dbp.GetNext(ref barHSIDayBar, ref barHSCIDayBar))
            {
                if (barHSIDayBar.dtDate > IParam.dtDateConcerned) break;

                Console.Write(  barHSIDayBar.Date + ", " +
                                barHSIDayBar.Open + ", " +
                                barHSIDayBar.High + ", " +
                                barHSIDayBar.Low + ", " +
                                barHSIDayBar.Close + ", " +
                                barHSIDayBar.Volatility + " | " +
                                barHSCIDayBar.Close);
                Console.WriteLine();

                rbdts.OnDayOpen(barHSIDayBar.dtDate,
                    barHSIDayBar.Open, barHSIDayBar.High, barHSIDayBar.Low, barHSIDayBar.Close,
                    barHSCIDayBar.Open, barHSCIDayBar.High, barHSCIDayBar.Low, barHSCIDayBar.Close);
                rbdts.OnDayClose(barHSIDayBar.dtDate,
                    barHSIDayBar.Open, barHSIDayBar.High, barHSIDayBar.Low, barHSIDayBar.Close,
                    barHSIDayBar.Volatility,
                    barHSCIDayBar.Open, barHSCIDayBar.High, barHSCIDayBar.Low, barHSCIDayBar.Close);


            }
        }

        static void Main(string[] args)
        {
            //LstSquCubicRegr ls = new LstSquCubicRegr();
            //ls.AddPoints(0.145, -0.1444050245);

            //double a = ls.aTerm;
            //double b = ls.bTerm;
            //double c = ls.cTerm;
            //double d = ls.dTerm;

            //int i = 0;

            string sInput = "";
            if (args.Length == 0)
            {
                Console.WriteLine("Basic:");
                Console.WriteLine("'1' Run the optimal set (data-new).");
                Console.WriteLine("'2' Run the optimal set (data-old).");
                Console.WriteLine("'3' Loop through all systems with standard size.");
                Console.WriteLine("'4' Run a specific system.");
                Console.WriteLine("With genetic optimization:");
                Console.WriteLine("'f' Calculate all PDFs and scenarios only.");
                Console.WriteLine("'a' Get the latest genetically optimal portfolio only.");
                Console.WriteLine("'c' Assess (default) portfolio impact under scenarios.");
                Console.WriteLine("'d' Assess (bestportRM1) portfolio impact under scenarios.");
                Console.WriteLine("'e' Assess (bestportRM2) portfolio impact under scenarios.");
                Console.Write("Choice: ");
                sInput = Console.ReadLine();
            }
            else
            {
                sInput = args[0];
                if (sInput[0] != '1' && sInput[0] != '2' &&
                    sInput[0] != '3' && sInput[0] != '4' &&
                    sInput[0] != 'a' &&
                    sInput[0] != 'c' && sInput[0] != 'd' &&
                    sInput[0] != 'e' && sInput[0] != 'f')
                {
                    Console.WriteLine("Invalid choice.");
                    return;
                }
            }

            if (sInput[0] == '1' || sInput[0] == '2')
            {
                IParam.LoadOptimal();

                RuleBasedDayTradeSys rbdts = new RuleBasedDayTradeSys(IParam.SYSNAME.OPTIMAL,
                    IParam.DetailFilePath, "OM/pnl-OPTIMAL.txt", "OM/mr-OPTIMAL.txt", IParam.HSCIEXHSIFilePath, true);

                RunOptionMedley(rbdts,
                                sInput[0] == '1' ? IParam.DataNewFilePath : IParam.DataOldFilePath);
            }
            else if (sInput[0] == '3')
            {
                foreach (IParam.SYSNAME sn in Enum.GetValues(typeof(IParam.SYSNAME)))
                {
                    IParam.ResetPosSize();
                    IParam.PosSize[(int)sn] = 10000000;

                    RuleBasedDayTradeSys rbdts = new RuleBasedDayTradeSys(sn,
                        IParam.DetailFilePath, "OM/pnl-" + sn.ToString() + ".txt", "OM/mr-" + sn.ToString() + ".txt",
                        IParam.HSCIEXHSIFilePath, false);

                    RunOptionMedley(rbdts, IParam.DataOldFilePath);

                }
            }
            else if (sInput[0] == '4')
            {
                IParam.ResetPosSize();
                IParam.PosSize[17] = 10000000;

                RuleBasedDayTradeSys rbdts = new RuleBasedDayTradeSys(IParam.SYSNAME.OPTIMAL,
                    IParam.DetailFilePath, "OM/pnl-OPTIMAL.txt", "OM/mr-OPTIMAL.txt", IParam.HSCIEXHSIFilePath, true);

                RunOptionMedley(rbdts, IParam.DataOldFilePath);

            }
            else if (sInput[0] == 'f')
            {
                IParam.LoadParamSetA();

                //run basic systems to get delta
                IParam.LoadOptimal();
                RuleBasedDayTradeSys rbdts = new RuleBasedDayTradeSys(IParam.SYSNAME.OPTIMAL,
                    IParam.DetailFilePath, "OM/pnl-OPTIMAL.txt", "OM/mr-OPTIMAL.txt", IParam.HSCIEXHSIFilePath, true);
                RunOptionMedley(rbdts, IParam.DataNewFilePath);

                //generate PDFs
                VolatilitySurface.GenVolSmile_ImplPDF(rbdts);
                rbdts.vs.ReadVolatilitySmileFromFile(true);
                ScenarioProvider sp = new ScenarioProvider();
                sp.GenerateScenarioFile_ScVolSmile(rbdts);
            }
            else if (sInput[0] == 'a')
            {
                IParam.LoadParamSetA();

                bool bLoadBestPortFrmFile = true;
                IParam.RUNMODE = 1;
                string sInput6 = "";
                string sInput7 = "";
                if (args.Length >= 2)
                {
                    sInput6 = args[1];
                    sInput7 = args[2];
                }
                else
                {
                    Console.Write("Load best portfolio");
                    Console.Write(" [Y]/N: ");
                    sInput6 = Console.ReadLine();
                    Console.Write("Genetic Optimization Run Mode [1 = Full Scale / 2 = Adjust MyPort / 3 = Quick Full Scale]");
                    Console.Write(" [1]/2/3: ");
                    sInput7 = Console.ReadLine();
                }
                if (sInput6 != null && sInput6.Length != 0 && sInput6[0] == 'N') bLoadBestPortFrmFile = false;
                if (sInput7 != null && sInput7.Length != 0 && sInput7[0] == '2') IParam.RUNMODE = 2;
                if (sInput7 != null && sInput7.Length != 0 && sInput7[0] == '3') IParam.RUNMODE = 3;
                IParam.GeneticOptimizationRunMode();

                //apply genetic optimization on that result
                ScenarioProvider sp = new ScenarioProvider();
                sp.ReadScenarioFile();
                Genetic g = new Genetic();
                PositionController pcMyPort = new PositionController(true);
                pcMyPort.LoadPortfolioFrmFile(IParam.MyPortFilePath);
                g.RunGeneticOptimizationGalaxy(sp, bLoadBestPortFrmFile, pcMyPort);

            }
            else if (sInput[0] == 'c' || sInput[0] == 'd' || sInput[0] == 'e')
            {
                StatHelper sh = new StatHelper(IParam.SCENARIOMAX);
                SortedList sl = new SortedList();

                PositionController pcMyPort = new PositionController(true);
                pcMyPort.LoadPortfolioFrmFile(IParam.MyPortFilePath);

                ScenarioProvider sp = new ScenarioProvider();
                sp.ReadScenarioFile();

                PositionController pc = new PositionController(true);
                pc.SetPathsAndResetFile(IParam.AssessFilePath, IParam.AssessFilePath,
                    IParam.AssessFilePath, IParam.AssessFilePath, true);

                RandomEngine re = new RandomEngine();

                //load our default portfolio
                IParam.RUNMODE = 1;
                if (sInput[0] == 'd') IParam.RUNMODE = 1;
                else if (sInput[0] == 'e') IParam.RUNMODE = 2;
                IParam.GeneticOptimizationRunMode();

                if (sInput[0] == 'c') pc.LoadPortfolioFrmFile(IParam.DefaultPortFilePath);
                else if (sInput[0] == 'd' || sInput[0] == 'e') pc.LoadPortfolioFrmFile(IParam.BestPortFilePath);


                //we output internal figures to make sure things are calculated right
                pc.vs.VHSILatest = sp.StartingVolatility;
                pc.MarkToMarket(sp.StartingPrice1, sp.StartingPrice2, sp.StartingDate, 0);
                //pc.OutputDetails(sp.StartingDate.ToString());
                //pc.OutputPositionInfoClose(sp.StartingDate, sp.StartingPrice1, sp.StartingPrice2, 3);
                //pc.OutputDetails("---------------------------------------------------------------");
                pc.OutputDetails("KYIERJ7");

                string sBufferredScenarioDetailes = "";

                for (int iSc = 0; iSc < sp.GetNoOfScenario(); iSc++)
                {
                    //reset to starting condition
                    pc.vs.VHSILatest = sp.StartingVolatility;
                    pc.MarkToMarket(sp.StartingPrice1, sp.StartingPrice2, sp.StartingDate, 0);
                    pc.ResetCPnL();
                    pc.PenalizeCPnLPctgFrict(sp.StartingPrice1, sp.StartingPrice2, sp.StartingDate, 1);

                    //load scenario
                    double dScenarioProbability = 0;
                    DateTime dtScenarioEndingDate = sp.StartingDate; //just set it to sth
                    double dScenarioPrice1 = 0;
                    double dScenarioPrice2 = 0;
                    double dScenarioVolatility = 0;
                    sp.GetProbEndingDatePriceVolatility(iSc, ref dScenarioProbability, ref dtScenarioEndingDate, ref dScenarioPrice1, ref dScenarioPrice2, ref dScenarioVolatility);

                    //load scenario into PositionController
                    pc.vs.VHSILatest = dScenarioVolatility;
                    pc.MarkToMarket(dScenarioPrice1, dScenarioPrice2, dtScenarioEndingDate, 0);
                    pc.OutputPositionInfoClose(dtScenarioEndingDate, dScenarioPrice1, dScenarioPrice2, 3);
                    pc.PenalizeCPnLPctgFrict(dScenarioPrice1, dScenarioPrice2, dtScenarioEndingDate, 1);


                    //store it for calculation
                    sh.Add(pc.CPnL, (int)(dScenarioProbability * 1000));
                    if (!sl.ContainsKey(pc.CPnL))
                        sl.Add(pc.CPnL, dScenarioProbability);
                    else
                        sl[pc.CPnL] = (double)sl[pc.CPnL] + dScenarioProbability;

                    //buffer up intermediate results
                    sBufferredScenarioDetailes += "Scenario: " + iSc.ToString() + "\tCPnL: " +
                        Math.Round(pc.CPnL,3).ToString() + "\t" +
                        (pc.CPnL == 0 ? "\t" : "") +
                        dtScenarioEndingDate.Year.ToString() + "-" +
                        dtScenarioEndingDate.Month.ToString() + "-" +
                        dtScenarioEndingDate.Day.ToString() + "\t" +
                        Math.Round(dScenarioProbability * 100, 1).ToString() + "%\t" +
                        Math.Round(dScenarioPrice1, 0).ToString() + "\t" +
                        Math.Round(dScenarioPrice2, 0).ToString() + "\t" +
                        Math.Round(dScenarioVolatility, 2).ToString() + "\n";
                }

                //output results
                pc.OutputDetails(sBufferredScenarioDetailes);

                pc.OutputDetails("------------------------------------------");
                pc.OutputDetails("Expected Return: " + Math.Round(sh.AMean, 2).ToString());
                string sTmp = "";
                double dUtility =
                    Math.Round(Genetic.CalcUtilityScore(sh.AMean, sh.SemiVar, sh.SemiVar0,
                    //(double)sl.GetKey(0), (double)sl.GetByIndex(0),
                                                        (int)Math.Min(pc.AggCallPos, pc.AggPutPos),
                                                        pc.CompareOptCtrtDiff(pcMyPort),
                                                        true), 5);
                sTmp += "Utility : " + dUtility.ToString();
                sTmp += "\t";
                double dScore =
                    Math.Round(Genetic.CalcUtilityScore(sh.AMean, sh.SemiVar, sh.SemiVar0,
                    //(double)sl.GetKey(0), (double)sl.GetByIndex(0),
                                                        (int)Math.Min(pc.AggCallPos, pc.AggPutPos),
                                                        pc.CompareOptCtrtDiff(pcMyPort),
                                                        false), 5);
                sTmp += "Score : " + dScore.ToString();
                pc.OutputDetails(sTmp);

                double dAlpha = Math.Round(Genetic.GetPositionSizeAlpha(sh.X, sh.V), 2);
                pc.OutputDetails("Kelly = " + Math.Round(IParam.HOWMUCHKELLY, 2) +
                                    " , Alpha = " + dAlpha);


                //reset to starting condition and print once
                pc.vs.VHSILatest = sp.StartingVolatility;
                pc.MarkToMarket(sp.StartingPrice1, sp.StartingPrice2, sp.StartingDate, 0);

                pc.OutputDetails("-------------------------------------------------------");
                pc.OutputDetails("Position size (Satellite futures):");
                pc.OutputPositionInfoClose(sp.StartingDate, sp.StartingPrice1, sp.StartingPrice2, 3);
                pc.OutputDetails("Portfolio Delta: " + (Math.Round(pc.GetPortDelta(sp.StartingDate, sp.StartingPrice1, sp.StartingPrice2), 2)).ToString());


                //ok, now we calculate mess up the PositionController to calculate the equivalent regular contract size
                //reset to starting condition
                pc.MultiplyFutPsnSz(sp.StartingPrice1, sp.StartingDate, dAlpha, "ASS");
                pc.MultiplyOptPsnSz(sp.StartingPrice1, sp.StartingDate, dAlpha / 5d, "ASS");
                pc.OutputDetails("-------------------------------------------------------");
                pc.OutputDetails("Position size after multiplying alpha:");
                pc.OutputPositionInfoClose(sp.StartingDate, sp.StartingPrice1, sp.StartingPrice2, 4);


                ExtensionMethods.PrintFileToConsole(IParam.AssessFilePath);
            }
        }
    }
}
