// FINITE STATE MACHINE TRAINING
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <deque>
#include <map>
#include <string>
#include "StateMach.h"
#include "StateMach1.h"
#include "StateMach2.h"
#include "STool.h"
#include "BarProvider.h"
#include "HKFE.h"
#include "GKYZ.h"
#include "STool.h"

using namespace std;

int main(int argc, char **argv)
{
  //------------------------------------------
  cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << argc << endl << flush;
  for (unsigned int i = 0; i < argc; ++i)
    cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << argv[i] << endl << flush;
  //--------------------------------------------------
  
  if (argc != 13)
  {
    cout << "Usage: FSMA_Training [StockListFile] [InputDataPath] [InputDataFileSuffix] [VHSI File / NOT_USED] [OutputPath] [YYYYMMDD Start Date] [YYYYMMDD End Date] [HKFE_Calendar.csv] [Discretization Interval] [Max Rtn / V_R] [GKYZ Window] [Exp Weighting Alpha]" << endl;
    return 0;
  }

  //------------------------------------------
  YYYYMMDD ymdTrngStartDate;
  YYYYMMDD ymdTrngEndDate;
  string sStockListFile       = string(argv[1]);
  string sInputDataPath       = string(argv[2]);
  string sInputDataFileSuffix = string(argv[3]);
  string sVHSIPath            = string(argv[4]);
  bool bUseVHSI               = (sVHSIPath == "NOT_USED") ? false : true;
  string sOutputPath          = string(argv[5]);
  int iTrngStartDate          = boost::lexical_cast<int>(string(argv[6]));
  int iTrngEndDate            = boost::lexical_cast<int>(string(argv[7]));
  string sHKFECalendar        = string(argv[8]);
  double dDiscretizationIntvl = boost::lexical_cast<double>(string(argv[9]));
  double dMaxRtnOverVR        = boost::lexical_cast<double>(string(argv[10]));
  int iGKYZWindow             = boost::lexical_cast<int>(string(argv[11]));
  double dExpWeightAlpha      = boost::lexical_cast<double>(string(argv[12]));
  ymdTrngStartDate.Set(iTrngStartDate);
  ymdTrngEndDate.Set(iTrngEndDate);
  //------------------------------------------


  //------------------------------------------
  // Read stock file list
  deque<string> dqStockListFile;
  STool::ReadFile(sStockListFile.c_str(), dqStockListFile);
  if (dqStockListFile.empty()) return 0;
  //------------------------------------------

  //------------------------------------------
  shared_ptr<HKFE> m_HKFE = HKFE::Instance();
  m_HKFE->LoadCalendar(sHKFECalendar);


  //------------------------------------------
  BarProvider bpv(sVHSIPath.c_str(), "DOHLCV", 2, 'F', 'N', 'P');

  //------------------------------------------
  // Collect Stat for each Stock
  //------------------------------------------
  for (int i = 0; i < dqStockListFile.size(); i++)
  {
    //--------------------------------------------------
    // Reset
    //--------------------------------------------------
    StateMach2 sm_stk;
    GKYZ gkyz_stk(iGKYZWindow);
    cout << "Using FSM: [" << sm_stk.Name() << "].\t";

    deque<map<const double,double> > dqmStatePMF;
    for (int k = 0; k < sm_stk.TotalStates(); k++)
      dqmStatePMF.push_back(map<const double,double>());


    deque<double> dqdCloseStk;
    deque<int> dqiStateStk;

    //--------------------------------------------------
    vector<string> vs;
    boost::split(vs, dqStockListFile[i], boost::is_any_of(","));
    string sCode = STool::PadLeftS(vs[0],'0',5);

    string sPath(sInputDataPath);
    sPath += "/";
    sPath += sCode;
    sPath += sInputDataFileSuffix;
    //--------------------------------------------------

    cout << "[" << iTrngStartDate << " - " << iTrngEndDate << "] " << sPath << endl;


    //--------------------------------------------------
    {
      BarProvider bp(sPath.c_str(), "DOHLCV", 2, 'F', 'N', 'P');
      bpv.ResetPsn();

      bp.MoveNext();
      bpv.MoveNext();

      YMD ymdTrdgDate(bpv.GetYYYYMMDD());

      while (ymdTrdgDate != YMD::BAD_DATE)
      {
        while (bp.GetYYYYMMDD() < ymdTrdgDate && bp.HasNextBar()) bp.MoveNext();
        while (bpv.GetYYYYMMDD() < ymdTrdgDate && bpv.HasNextBar()) bpv.MoveNext();

        if (bp.GetYYYYMMDD() == ymdTrdgDate &&
            bpv.GetYYYYMMDD() == ymdTrdgDate &&
            ymdTrdgDate >= ymdTrngStartDate &&
            ymdTrdgDate <= ymdTrngEndDate)
        {
          gkyz_stk.Add(bp.Open(), bp.High(), bp.Low(), bp.Close());
          if (bUseVHSI)
          {
            sm_stk.Add(bp.High(), bp.Low(), bp.Close(), bpv.Close());
          }
          else if (gkyz_stk.Ready())
          {
            sm_stk.Add(bp.High(), bp.Low(), bp.Close(), gkyz_stk.Value());
          }

          dqdCloseStk.push_back(bp.Close());
          if (sm_stk.Ready() && gkyz_stk.Ready())
          {
            dqiStateStk.push_back(sm_stk.State());

            if (dqiStateStk.size() >= 2)
            {
              int iStateStk = dqiStateStk[dqiStateStk.size()-2];
              double dRndRtnOvVR =
                STool::RoundIntvl(
                    (dqdCloseStk[dqdCloseStk.size()-1]/dqdCloseStk[dqdCloseStk.size()-2]-1) / (gkyz_stk.Value()),
                    dDiscretizationIntvl);

              //--------------------------------------------------
              // Cap the max and min
              //--------------------------------------------------
              if (dRndRtnOvVR > dMaxRtnOverVR)
                dRndRtnOvVR = dMaxRtnOverVR;
              else if (dRndRtnOvVR < -dMaxRtnOverVR)
                dRndRtnOvVR = -dMaxRtnOverVR;

              //--------------------------------------------------
              // Counting the number of occurrence for that state
              //--------------------------------------------------
              double dIncrement = dExpWeightAlpha + exp(dExpWeightAlpha * dqdCloseStk.size());

              if (dqmStatePMF[iStateStk].find(dRndRtnOvVR) != dqmStatePMF[iStateStk].end())
                dqmStatePMF[iStateStk][dRndRtnOvVR] += dIncrement;
              else
                dqmStatePMF[iStateStk][dRndRtnOvVR] = dIncrement;

            }
          }
        }
        m_HKFE->NextTrdgDate(ymdTrdgDate,1);
      }
    }

    {
      //--------------------------------------------------
      // Output FSM stat
      //--------------------------------------------------
      sPath = sOutputPath;
      sPath += "/";
      sPath += sCode;
      sPath += "_";
      sPath += ymdTrngStartDate.ToStr();
      sPath += "_";
      sPath += ymdTrngEndDate.ToStr();
      deque<double> dq_dStateMean;
      deque<double> dq_dStateVariance;
      deque<double> dq_dStateSkewness;
      deque<double> dq_dStateKurtosis;

      ofstream ofs;
      ofs.open(sPath.c_str(), ios::out);
      if (ofs.is_open())
      {
        for (int k = 0; k < sm_stk.TotalStates(); k++)
        {

          double dStateMean = 0;
          double dStateVariance = 0;
          double dStateSkewness = 0;
          double dStateKurtosis = 0;

          int iTotCnt = 0;

          for (map<const double,double>::iterator it=dqmStatePMF[k].begin(); it!=dqmStatePMF[k].end(); ++it) iTotCnt += it->second;

          for (map<const double,double>::iterator it=dqmStatePMF[k].begin(); it!=dqmStatePMF[k].end(); ++it)
          {
            // ofs << "State\t" << k << "\tRtnOverVR\t" << it->first << "\tProbMass\t" << (double)(it->second)/(double)(iTotCnt) << "\tFreq\t" << (it->second) << "\tBinary\t" << STool::DecToBin(k) << endl;
            ofs << k << "," << it->first << "," << (double)(it->second)/(double)(iTotCnt) << "," << (it->second) << "," << STool::DecToBin(k) << endl;
            //sm_stk.StateDesc(k)
            dStateMean     += (double)(it->second)/(double)(iTotCnt) * (it->first);
            dStateVariance += (double)(it->second)/(double)(iTotCnt) * (it->first) * (it->first);
            dStateSkewness += (double)(it->second)/(double)(iTotCnt) * (it->first) * (it->first) * (it->first);
            dStateKurtosis += (double)(it->second)/(double)(iTotCnt) * (it->first) * (it->first) * (it->first) * (it->first);
          }

          dq_dStateMean.push_back    (dStateMean    );
          dq_dStateVariance.push_back(dStateVariance);
          dq_dStateSkewness.push_back(dStateSkewness);
          dq_dStateKurtosis.push_back(dStateKurtosis);

        }

        // //--------------------------------------------------
        // // Output state stat
        // //--------------------------------------------------
        // for (int k = 0; k < sm_stk.TotalStates(); k++)
        //   ofs << "Stat\tState\t" << k << "\tMean\t" << dq_dStateMean[k] << "\tVariance\t" << dq_dStateVariance[k] << "\tSkewness\t" << dq_dStateSkewness[k] << "\tKurtosis\t" << dq_dStateKurtosis[k] << endl;

      }

      ofs.close();

      //------------------------------------------
    }

  } // for

  return 0;
}
