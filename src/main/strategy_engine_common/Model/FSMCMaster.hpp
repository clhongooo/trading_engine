#ifndef FSMCMASTER_HPP
#define FSMCMASTER_HPP

#include "ModelSys.hpp"
#include "../Math/Atr.hpp"
#include "../Util/SException.hpp"
#include "../Util/STool.h"
#include "../Math/FSMC2.hpp"
#include "../Util/BarProvider.hpp"
#include "../Util/SException.hpp"
#include <deque>
#include <map>
#include <string>
#include <sstream>

using namespace std;

class FSMCMaster : public ModelSys
{
  private:
    void Add(const BarProvider &) {}
    Atr<double> _atr;
    deque<string> dqsExtraInfo;

    deque<string> dqStkList;
    deque<double> dqStkListWeight;
    deque<string> dqHSIConstitFile;

    deque<StateMach2> dqSm2_stk;
    map<const double,double> mHSIPMF;

    BarProvider bpv;
    deque<BarProvider> dq_bp;

    deque<deque<map<const double,double> > > dqdqmStatePMF; //dq(stock) - dq(state) - m(PMF)
    deque<map<const int,int> > dqmStateCnt; //dq(stock) - m(state)

    //Input parameters
    double _dInitPosSize;

  public:
    const char * Name() const;

    FSMC(const double *);
    void ResetChild();
    void Add(const BarProvider &, bool);
    void LoadAllData();
    void PrintExtraInfo() const;
};

#endif

const char * FSMC::Name() const { return "FSMC"; }
FSMC::FSMC(const double *dParamVector)
{
  _dInitPosSize = dParamVector[0];

  LoadAllData();
  Reset();
}

void FSMC::ResetChild()
{
  _atr.Reset(0.1,true);
  bpv.ResetPsn();

  dqSm2_stk.clear();
  if (!dqStkList.empty())
    for (int i = 0; i < dqStkList.size(); ++i)
    {
      dqSm2_stk.push_back(StateMach2());
      dq_bp[i].ResetPsn();
    }
}

void FSMC::LoadAllData()
{
  //------------------------------------------
  // Read VHSI
  //------------------------------------------
  bpv.LoadDataFile(IParam::DATA_DAILY_VHSI, "DOHLCV", 2, 'F', 'N', 'P');

  //------------------------------------------
  // Read Input File List
  //------------------------------------------
  dqHSIConstitFile.clear();
  dqStkList.clear();
  dqStkListWeight.clear();

  STool::ReadFile(IParam::STOCKLIST_HSI,dqHSIConstitFile);

  if (dqHSIConstitFile.empty())
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
  for (int i = 0; i < dqHSIConstitFile.size(); ++i)
  {
    string sPart;
    Tokenizer tz(dqHSIConstitFile[i],",");
    if (tz.NextToken()) sPart = tz.GetToken(); else continue;
    if (sPart.length() > 2) dqStkList.push_back(sPart);
    if (tz.NextToken()) sPart = tz.GetToken(); else continue;
    if (sPart.length() > 1) dqStkListWeight.push_back(STool::ToDbl(sPart,2));
  }

  //------------------------------------------
  dqdqmStatePMF.clear();
  for (int j = 0; j < dqStkList.size(); ++j)
    dqdqmStatePMF.push_back(deque<map<const double,double> >());

  StateMach2 sm2; // Temp
  for (int j = 0; j < dqStkList.size(); ++j)
    for (int k = 0; k < sm2.TotalStates(); ++k)
      dqdqmStatePMF[j].push_back(map<const double,double>());

  //-------------------------------------------
  dqmStateCnt.clear();
  for (int j = 0; j < dqStkList.size(); ++j)
    dqmStateCnt.push_back(map<const int,int>());

  //-------------------------------------------
  // Read individual PMF
  for (int i=0; i<dqStkList.size(); ++i)
  {
    string sPath(IParam::FINSTATEMACH_PMF); sPath += dqStkList[i];
    ifstream ifs(sPath.c_str(), ios::in);
    if (ifs.is_open())
    {
      string sLine;
      while (ifs.good())
      {
        getline(ifs,sLine);
        string sState="";
        string sPxChg="";
        string sProb="";
        string sStateCnt="";
        Tokenizer tz(sLine,",");
        if (tz.NextToken()) sState = tz.GetToken(); else continue;
        if (tz.NextToken()) sPxChg = tz.GetToken(); else continue;
        if (tz.NextToken()) sProb = tz.GetToken(); else continue;
        if (tz.NextToken()) sStateCnt = tz.GetToken(); else continue;

        if (sState == "" || sPxChg == "" || sProb == "" || sStateCnt == "") continue;

        int iState = STool::ToInt(sState);
        dqdqmStatePMF[i][iState][STool::ToDbl(sPxChg,10)]
        = STool::ToDbl(sProb,10);

        if (STool::ContainsKey(dqmStateCnt[i],iState))
          dqmStateCnt[i][iState] += STool::ToInt(sStateCnt);
        else
          dqmStateCnt[i][iState] = STool::ToInt(sStateCnt);
      }
    }
    else
    {
      SException se;
      se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
      throw se;
    }
    ifs.close();
  }

  //-------------------------------------------
  dq_bp.clear();
  for (int i = 0; i < dqStkList.size(); ++i)
    dq_bp.push_back(BarProvider());

  for (int i = 0; i < dqStkList.size(); ++i)
  {
    string sPath = IParam::DATA_ALL_STOCKS; sPath += dqStkList[i];
    cout << "Reading: " << sPath << endl;
    //dq_bp[i].LoadDataFile(sPath.c_str(), "DOHLCV", 2, 'F', 'N', 'P');
    dq_bp[i].LoadDataFile(sPath.c_str(), "DOHLCV", 2, 'B', 'H', 'P');
  }


}






void FSMC::Add(const BarProvider &bA, bool bAtDayOpen)
{
  if (bAtDayOpen) return;

  _atr.Add(bA.High(),bA.Low(),bA.Close());

  dqsExtraInfo.clear();
  double dWAStateCnt = 0;

  bpv.MoveNext();
  // Locate the bar with the correct trading date
  while (bpv.YMDi() < bA.YMDi() && bpv.HasNextBar()) bpv.MoveNext();
  if (bpv.YMDi() > bA.YMDi()) // Not allow missing data for VHSI
  {
    SException se;
    string sErr("");
    sErr += "bpv.YMDi()=";
    sErr += STool::ToStr(bpv.YMDi());
    sErr += ", bA.YMDi()=";
    sErr += STool::ToStr(bA.YMDi());
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__, sErr.c_str());
    throw se;
  }

  mHSIPMF.clear();

  for (int i = 0; i < dqStkList.size(); ++i)
  {
    dq_bp[i].MoveNext();

    // Locate the bar with the correct trading date
    while (dq_bp[i].YMDi() < bA.YMDi() && dq_bp[i].HasNextBar()) dq_bp[i].MoveNext();
    if (dq_bp[i].YMDi() > bA.YMDi()) dq_bp[i].ResetPsn(); // Maybe the stock wasn't listed yet...

    // Only Add the correct data
    if (dq_bp[i].YMDi() == bA.YMDi() && bpv.YMDi() == bA.YMDi())
    {
      dqSm2_stk[i].Add(dq_bp[i].High(), dq_bp[i].Low(), dq_bp[i].Close(), bpv.Close());

      // Only getting Markov state if this is valid
      if (dqSm2_stk[i].Ready())
      {
        int iStateStk = dqSm2_stk[i].State();

        stringstream ss;
        ss << dq_bp[i].YMDi() << ": " << bA.YMDi() << ": " << bpv.YMDi() << " | " << dq_bp[i].Close() << " | "  << bpv.Close() << " | " << bA.Close() << " | " << dqStkList[i] << ": State: " << iStateStk << ": StateCnt: " << dqmStateCnt[i][iStateStk];
        dqsExtraInfo.push_back(ss.str());

        dWAStateCnt += dqStkListWeight[i] * dqmStateCnt[i][iStateStk];
        for (map<const double,double>::iterator it=dqdqmStatePMF[i][iStateStk].begin(); it!=dqdqmStatePMF[i][iStateStk].end(); ++it)
        {
          if (STool::ContainsKey((map<const double,double>)(mHSIPMF),(double)(it->first)))
            mHSIPMF[it->first] += dqStkListWeight[i] * it->second;
          else
            mHSIPMF[it->first] = dqStkListWeight[i] * it->second;
        }
      }
    }
  }


  double dAMean = 0;
  double dVar = 0;
  double dSD = 0;

  double dTotalProb = 0;
  for (map<const double,double>::iterator it=mHSIPMF.begin(); it!=mHSIPMF.end(); ++it)
    dTotalProb += it->second;

  for (map<const double,double>::iterator it=mHSIPMF.begin(); it!=mHSIPMF.end(); ++it)
    dAMean += it->second / dTotalProb * it->first;

  for (map<const double,double>::iterator it=mHSIPMF.begin(); it!=mHSIPMF.end(); ++it)
    dVar += it->second / dTotalProb * pow(it->first-dAMean,2);

  dSD = sqrt(dVar);

  stringstream ss;
  ss << "AMean =\t" << STool::Round(dAMean,4)*100 << "%\tSD =\t" << STool::Round(dSD,4)*100 << "%\tWA Cnt=\t" << STool::Round(dWAStateCnt,2) << endl;
  dqsExtraInfo.push_back(ss.str());


  // Trading decision
  if (_atr.Ready())
  {
    if (dAMean > 0) this->_iPos = _dInitPosSize / bA.Close() / _atr.Value() * 100;
    else if (dAMean < 0) this->_iPos = -_dInitPosSize / bA.Close() / _atr.Value() * 100;
  }

}


void FSMC::PrintExtraInfo() const
{
  cout << endl;
  cout << "-----------------------------------------------------" << endl;
  cout << "Finite State Markov Chain: Extra Info:" << endl;
  for (int i=0; i<dqsExtraInfo.size(); ++i)
    cout << dqsExtraInfo[i] << endl;
}


