// FINITE STATE MACHINE TRAINING
#include <boost/scoped_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <deque>
#include <map>
#include <string>
#include "SNYStat.h"
#include "STool.h"
#include "BarProvider.h"
#include "HKFE.h"

using namespace std;

int main(int argc, char **argv)
{
  //------------------------------------------
  if (argc != 8)
  {
    cout << "Usage: CorrelationCalculator [StockListFile] [DataDir] [OutputDir] [YYYYMMDD Start Date] [YYYYMMDD End Date] [HKFE_Calendar.csv] [Lookback Window]" << endl;
    return 0;
  }

  //------------------------------------------
  YYYYMMDD ymdTrngStartDate;
  YYYYMMDD ymdTrngEndDate;
  string sStockListFile = string(argv[1]);
  string sDataDir       = string(argv[2]);
  string sOutputDir     = string(argv[3]);
  int iTrngStartDate    = boost::lexical_cast<int>(string(argv[4]));
  int iTrngEndDate      = boost::lexical_cast<int>(string(argv[5]));
  string sHKFECalendar  = string(argv[6]);
  int iLookbkWinLen     = boost::lexical_cast<int>(string(argv[7]));
  ymdTrngStartDate.Set(iTrngStartDate);
  ymdTrngEndDate.Set(iTrngEndDate);
  //------------------------------------------

  //------------------------------------------
  // Read stock file list
  deque<string> dqStockListFile;
  cout << "Reading: " << sStockListFile << endl;
  dqStockListFile = STool::ReadFile(sStockListFile.c_str());
  if (dqStockListFile.empty()) return 0;
  //------------------------------------------

  //------------------------------------------
  shared_ptr<HKFE> m_HKFE = HKFE::Instance();
  m_HKFE->LoadCalendar(sHKFECalendar);

  //------------------------------------------
  // Calculate Correl
  //--------------------------------------------------
  vector<int> vDelay(dqStockListFile.size(),0);
  SNYStat snys(dqStockListFile.size(),vDelay,iLookbkWinLen,iLookbkWinLen*100,true);

  ptr_vector<BarProvider> bp;
  vector<double> vLastPx(dqStockListFile.size(),0);

  //--------------------------------------------------
  for (int i = 0; i < dqStockListFile.size(); i++)
  {
    vector<string> vs;
    boost::split(vs, dqStockListFile[i], boost::is_any_of(","));
    string sPath(sDataDir);
    sPath += vs[0];
    sPath += ".csv";
    cout << "Reading: " << sPath << endl;
    bp.push_back(new BarProvider(sPath.c_str(), "DOHLCV", 2, 'F', 'N', 'P'));
  }
  //--------------------------------------------------

  YMD ymdSysDate(ymdTrngStartDate);

  while
    (ymdSysDate != YMD::BAD_DATE &&
     ymdSysDate <= ymdTrngEndDate)
    {
      for (int i = 0; i < dqStockListFile.size(); i++)
      {
        while (bp[i].GetYYYYMMDD() < ymdSysDate && bp[i].HasNextBar()) bp[i].MoveNext();
      }

      for (int i = 0; i < dqStockListFile.size(); i++)
      {
        if (bp[i].GetYYYYMMDD() == ymdSysDate)
          snys.Add(i,bp[i].Return());
        else
          snys.Add(i,vLastPx[i]);
      }

      // cout << "Processing date: " << ymdSysDate.GetYMD().ToYMDi() << endl << flush;
      //--------------------------------------------------
      // Output Correl
      //--------------------------------------------------
      string sPath = sOutputDir + "/" + boost::lexical_cast<string>(ymdSysDate.ToYMDi());

      ofstream ofs;
      ofs.open(sPath.c_str(), ios::out);
      if (ofs.is_open())
      {
        ofs << "Stock Code";
        for (unsigned int i = 0; i < dqStockListFile.size(); ++i)
        {
          ofs << "," << dqStockListFile[i];
        }
        ofs << endl;
        for (unsigned int i = 0; i < dqStockListFile.size(); ++i)
        {
          ofs << dqStockListFile[i];
          for (unsigned int j = 0; j < dqStockListFile.size(); ++j)
          {
            bool   bRtn = false;
            double dCrl = 999;

            if (snys.Correl(i,j,dCrl))
              ofs << "," << dCrl;
            else
              ofs << ",999";
          }
          ofs << endl;
        }
      }
      ofs.close();

      m_HKFE->NextTrdgDate(ymdSysDate,1);
    }


  //------------------------------------------

  return 0;
}
