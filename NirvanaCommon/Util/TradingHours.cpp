#include <TradingHours.h>

TradingHours::TradingHours() : m_CfgFileLoaded(false), m_DefaultBehaviour(DFT_DISALLOW)
{
}


TradingHours::TradingHours(const TradingHours::DefaultBehaviour db) : m_CfgFileLoaded(false), m_DefaultBehaviour(db)
{
}

TradingHours::~TradingHours()
{
}

void TradingHours::ReadConfig(const string & configfilename)
{

  deque<string> dqConfig;
  STool::ReadFile(configfilename,dqConfig);

  for (deque<string>::iterator itCfg = dqConfig.begin(); itCfg != dqConfig.end(); ++itCfg)
  {
    const string & sConfigLine = *itCfg;

    vector<string> vs;
    boost::split(vs, sConfigLine, boost::is_any_of(","));

    //--------------------------------------------------
    // Not a valid line
    //--------------------------------------------------
    if (vs.size() < 2)
    {
      cerr << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") Wrong file format." << endl << flush;
      usleep(10000);
      exit(1);
    }

    const string & symbol    = vs[0];
    const string & sYYYYMMDD = vs[1];

    if (STool::IsStringAllLetters(sYYYYMMDD)) continue;

    const YYYYMMDD ymd(sYYYYMMDD);

    map<string,Date_TradingHours>::iterator it = m_map_Sym_DateTrdgHour.find(symbol);
    if (it == m_map_Sym_DateTrdgHour.end())
    {
      m_map_Sym_DateTrdgHour[symbol] = Date_TradingHours();
      it = m_map_Sym_DateTrdgHour.find(symbol);
    }

    Date_TradingHours & dth = it->second;

    dth.m_vYYYYMMDD.push_back(ymd);
    if (vs.size() == 2)
    {
      //--------------------------------------------------
      // Non-trading day
      //--------------------------------------------------
      Time_Range tr;
      dth.m_vTimeRange.push_back(tr);
    }
    else
    {
      Time_Range tr;

      unsigned int iIdx = 2;
      while (iIdx < vs.size())
      {
        //--------------------------------------------------
        // Market close time
        //--------------------------------------------------
        if (iIdx == vs.size()-1)
        {
          tr.m_CloseTime = HHMMSS(boost::lexical_cast<int>(vs[iIdx])); iIdx++;
        }
        else
        {
          const int iHMS_Start= boost::lexical_cast<int>(vs[iIdx]); iIdx++;
          const int iHMS_End  = boost::lexical_cast<int>(vs[iIdx]); iIdx++;

          tr.m_Time.push_back(HHMMSS(iHMS_Start));
          tr.m_Time.push_back(HHMMSS(iHMS_End));
          tr.m_Prop.push_back('S');
          tr.m_Prop.push_back('E');
        }
      }
      dth.m_vTimeRange.push_back(tr);
    }
  }
  m_CfgFileLoaded = true;

  return;
}

bool TradingHours::IsCfgFileLoaded() const
{
  return m_CfgFileLoaded;
}

//--------------------------------------------------
// All convenience wrappers
//--------------------------------------------------
bool TradingHours::IsTradingHour(const string & symbol, const YYYYMMDDHHMMSS & ymdhms)
{
  return IsTradingHourPrivate(symbol,ymdhms.GetYYYYMMDD(),ymdhms.GetHHMMSS(), 0, 0, 0);
}
bool TradingHours::IsTradingHour(const string & symbol, const YYYYMMDD & ymd, const HHMMSS & hms)
{
  return IsTradingHourPrivate(symbol,ymd,hms, 0, 0, 0);
}

bool TradingHours::IsTradingHourStartEndBuffer(const string & symbol, const YYYYMMDDHHMMSS & ymdhms, const int iStartBufferInSec, const int iEndBufferInSec)
{
  return IsTradingHourPrivate(symbol,ymdhms.GetYYYYMMDD(),ymdhms.GetHHMMSS(), iStartBufferInSec, iEndBufferInSec, 0);
}

bool TradingHours::IsTradingHourBreakBuffer(const string & symbol, const YYYYMMDDHHMMSS & ymdhms, const int iBreakBufferInSec)
{
  return IsTradingHourPrivate(symbol,ymdhms.GetYYYYMMDD(),ymdhms.GetHHMMSS(), 0, 0, iBreakBufferInSec);
}

bool TradingHours::IsTradingHourStartEndBreakBuffer(const string & symbol, const YYYYMMDDHHMMSS & ymdhms, const int iStartBufferInSec, const int iEndBufferInSec, const int iBreakBufferInSec)
{
  return IsTradingHourPrivate(symbol,ymdhms.GetYYYYMMDD(),ymdhms.GetHHMMSS(), iStartBufferInSec, iEndBufferInSec, iBreakBufferInSec);
}

bool TradingHours::IsTradingHourStartEndBreakBuffer(const string & symbol, const YYYYMMDD & ymd, const HHMMSS & hms, const int iStartBufferInSec, const int iEndBufferInSec, const int iBreakBufferInSec)
{
  return IsTradingHourPrivate(symbol,ymd,hms, iStartBufferInSec, iEndBufferInSec, iBreakBufferInSec);
}
//--------------------------------------------------

bool TradingHours::IsTradingHourPrivate(const string & symbol, const YYYYMMDD & ymd, const HHMMSS & hms, const int iStartBufferInSec, const int iEndBufferInSec, const int iBreakBufferInSec)
{
  map<string,Date_TradingHours>::const_iterator it = m_map_Sym_DateTrdgHour.find(symbol);
  if (it == m_map_Sym_DateTrdgHour.end())
  {
    if (m_DefaultBehaviour == DFT_DISALLOW) return false;
    else                                    return true;
  }

  const vector<YYYYMMDD>   & vYYYYMMDD    = it->second.m_vYYYYMMDD;
  const vector<Time_Range> & vTimeRange   = it->second.m_vTimeRange;

  if (ymd < vYYYYMMDD[0])
  {
    if (m_DefaultBehaviour == DFT_DISALLOW) return false;
    else                                    return true;
  }

  //--------------------------------------------------
  // Mutable
  //--------------------------------------------------
  vector<YYYYMMDD>::const_iterator it_vYYYYMMDD;
  int iDateIdx = 0;

  //--------------------------------------------------
  // Check last memoized index before binary search
  //--------------------------------------------------
  map<string,int>::iterator itMem = m_Memoized_DateIdx.find(symbol);
  if (itMem == m_Memoized_DateIdx.end())
  {
    it_vYYYYMMDD = lower_bound(vYYYYMMDD.begin(),vYYYYMMDD.end(),ymd);
    iDateIdx = distance(vYYYYMMDD.begin(),it_vYYYYMMDD);
  }
  else
  {
    const int iIdx = itMem->second;
    if (vYYYYMMDD[iIdx  ] >= ymd &&
        vYYYYMMDD[iIdx-1] <  ymd)
    {
      //--------------------------------------------------
      // Still usable, so no need to search again
      //--------------------------------------------------
      it_vYYYYMMDD = vYYYYMMDD.begin() + iIdx;
      iDateIdx     = iIdx;
    }
    else
    {
      it_vYYYYMMDD = lower_bound(vYYYYMMDD.begin(),vYYYYMMDD.end(),ymd);
      iDateIdx = distance(vYYYYMMDD.begin(),it_vYYYYMMDD);
    }
  }

  //--------------------------------------------------
  // Memoize
  //--------------------------------------------------
  m_Memoized_DateIdx[symbol] = iDateIdx;

  //--------------------------------------------------
  // Immutable
  //--------------------------------------------------
  vector<YYYYMMDD>::const_iterator cit_vYYYYMMDD = it_vYYYYMMDD;
  const int ciDateIdx = iDateIdx;

  if (*cit_vYYYYMMDD == ymd)
  {
    //--------------------------------------------------
    // Spot on : date
    //--------------------------------------------------
    const vector<char>  & vProp  = vTimeRange[ciDateIdx].m_Prop;
    const vector<HHMMSS> & vHHMMSS = vTimeRange[ciDateIdx].m_Time;

    if (vHHMMSS.empty() || hms < vHHMMSS[0]) return false;

    vector<HMS> vHMS;
    FForEach (vHHMMSS,[&](const HHMMSS & hhmmss) { vHMS.push_back(HMS(hhmmss)); });

    {
      int iCnt = 0;
      FForEach (vProp,[&](const char P)
                {
                  if      (iCnt == 0              && P == 'S') vHMS[iCnt].AddSecond     (max(iStartBufferInSec,iBreakBufferInSec));
                  else if (iCnt == vProp.size()-1 && P == 'E') vHMS[iCnt].SubtractSecond(max(  iEndBufferInSec,iBreakBufferInSec));
                  else if (                          P == 'S') vHMS[iCnt].AddSecond     (                      iBreakBufferInSec);
                  else if (                          P == 'E') vHMS[iCnt].SubtractSecond(                      iBreakBufferInSec);
                  iCnt++;
                });
    }

    //--------------------------------------------------
    // Not expecting a lot of trading sessions, so no need to memoize
    //--------------------------------------------------
    vector<HMS>::iterator it_vHMS = lower_bound(vHMS.begin(),vHMS.end(),hms);
    const int iTimeIdx = distance(vHMS.begin(),it_vHMS);

    if (*it_vHMS == hms)
    {
      //--------------------------------------------------
      // Spot on : time
      //--------------------------------------------------
      return true;
    }
    else
    {
      const char propAft = vProp[iTimeIdx];
      const char propBef = vProp[iTimeIdx-1];

      if      (propBef == 'S' && propAft == 'E') return true;
      else if (propBef == 'E' && propAft == 'S') return false;
      else                                       return false;
    }
  }
  else
  {
    //--------------------------------------------------
    // Follow the trading hour of the date immediately before
    //--------------------------------------------------
    if (cit_vYYYYMMDD != vYYYYMMDD.begin())
    {
      cit_vYYYYMMDD--;
      return IsTradingHourPrivate(symbol, *cit_vYYYYMMDD, hms, iStartBufferInSec, iEndBufferInSec, iBreakBufferInSec);
    }
  }

  return false;
}


//--------------------------------------------------
// TODO code is duplicating IsTradingHourPrivate
//--------------------------------------------------
HHMMSS TradingHours::GetTimeNSecBefClose(const string & symbol, const YYYYMMDD & ymd, const int iSecBefClose)
{
  map<string,Date_TradingHours>::const_iterator it = m_map_Sym_DateTrdgHour.find(symbol);
  if (it == m_map_Sym_DateTrdgHour.end()) return HHMMSS();

  const vector<YYYYMMDD>   & vYYYYMMDD    = it->second.m_vYYYYMMDD;
  const vector<Time_Range> & vTimeRange   = it->second.m_vTimeRange;

  if (ymd < vYYYYMMDD[0]) return HHMMSS();

  //--------------------------------------------------
  // Mutable
  //--------------------------------------------------
  vector<YYYYMMDD>::const_iterator it_vYYYYMMDD;
  int iDateIdx = 0;

  //--------------------------------------------------
  // Check last memoized index before binary search
  //--------------------------------------------------
  map<string,int>::iterator itMem = m_Memoized_DateIdx.find(symbol);
  if (itMem == m_Memoized_DateIdx.end())
  {
    it_vYYYYMMDD = lower_bound(vYYYYMMDD.begin(),vYYYYMMDD.end(),ymd);
    iDateIdx = distance(vYYYYMMDD.begin(),it_vYYYYMMDD);
  }
  else
  {
    const int iIdx = itMem->second;
    if (vYYYYMMDD[iIdx  ] >= ymd &&
        vYYYYMMDD[iIdx-1] <  ymd)
    {
      //--------------------------------------------------
      // Still usable, so no need to search again
      //--------------------------------------------------
      it_vYYYYMMDD = vYYYYMMDD.begin() + iIdx;
      iDateIdx     = iIdx;
    }
    else
    {
      it_vYYYYMMDD = lower_bound(vYYYYMMDD.begin(),vYYYYMMDD.end(),ymd);
      iDateIdx = distance(vYYYYMMDD.begin(),it_vYYYYMMDD);
    }
  }

  //--------------------------------------------------
  // Memoize
  //--------------------------------------------------
  m_Memoized_DateIdx[symbol] = iDateIdx;

  //--------------------------------------------------
  // Immutable
  //--------------------------------------------------
  vector<YYYYMMDD>::const_iterator cit_vYYYYMMDD = it_vYYYYMMDD;
  const int ciDateIdx = iDateIdx;

  if (*cit_vYYYYMMDD == ymd)
  {
    //--------------------------------------------------
    // Spot on : date
    //--------------------------------------------------
    HMS hms(vTimeRange[ciDateIdx].m_CloseTime);
    hms.SubtractSecond(iSecBefClose);
    return hms; 
  }
  else
  {
    //--------------------------------------------------
    // Follow the trading hour of the date immediately before
    //--------------------------------------------------
    if (cit_vYYYYMMDD != vYYYYMMDD.begin())
    {
      cit_vYYYYMMDD--;
      return GetTimeNSecBefClose(symbol, *cit_vYYYYMMDD, iSecBefClose);
    }
  }

  return false;
}

