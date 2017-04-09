#include "HKFE.h"

YYYYMMDD HKFE::ymd20130408(20130408);
YYYYMMDD HKFE::ymd20120305(20120305);
YYYYMMDD HKFE::ymd20110307(20110307);
YYYYMMDD HKFE::ymd20000000(20000000);

HHMMSS HKFE::hms91500(91500);
HHMMSS HKFE::hms94500(94500);
HHMMSS HKFE::hms120000(120000);
HHMMSS HKFE::hms123000(123000);
HHMMSS HKFE::hms130000(130000);
HHMMSS HKFE::hms133000(133000);
HHMMSS HKFE::hms143000(143000);
HHMMSS HKFE::hms161500(161500);

const double HKFE::_TRDG_DAY_PER_YR = 252;



// vector<YYYYMMDD> HKFE::_vTrdgD;
// vector<int>      HKFE::_vExpyDIdx;
// map<int,int>     HKFE::_mCtrt_TrdgDIdx;
// map<int,int>     HKFE::_mCtrt_1FMthNum;
// map<int,int>     HKFE::_mCtrt_2FMthNum;
// map<int,char>    HKFE::_mCtrt_1FMthFutCode;
// map<int,char>    HKFE::_mCtrt_2FMthFutCode;
// map<int,char>    HKFE::_mCtrt_1FMthCallCode;
// map<int,char>    HKFE::_mCtrt_2FMthCallCode;
// map<int,char>    HKFE::_mCtrt_1FMthPutCode;
// map<int,char>    HKFE::_mCtrt_2FMthPutCode;

boost::weak_ptr<HKFE> HKFE::m_pInstance;

boost::shared_ptr<HKFE> HKFE::Instance() {
  boost::shared_ptr<HKFE> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new HKFE());
    m_pInstance = instance;
  }
  return instance;
}

HKFE::HKFE()
{
}


HKFE::~HKFE()
{
  //	for (unsigned i = 0; i < _vTrdgD.size(); ++i)
  //		delete _vTrdgD[i];
}

void HKFE::LoadCalendar(const string & sCalendarPath)
{
  deque<string> dqRawFile;
  STool::ReadFile(sCalendarPath,dqRawFile);
  if (dqRawFile.empty()) return;
  for (unsigned int i = 1; i < dqRawFile.size(); ++i) //skip header
  {
    string sTD, sE="", sH="";
    string s1FMthNum    , s2FMthNum;
    string s1FMthFutCode , s2FMthFutCode;
    string s1FMthCallCode, s2FMthCallCode;
    string s1FMthPutCode , s2FMthPutCode;

    Tokenizer tz(dqRawFile[i],",");
    if (tz.NextToken()) sTD = tz.GetToken();
    if (tz.NextToken()) sE = tz.GetToken();
    if (tz.NextToken()) sH = tz.GetToken();

    YYYYMMDD ymd(sTD);
    _vTrdgD.push_back(ymd);
    if (sE.at(0)=='E' || sE.at(0)=='Y')
    {
      YMD ymd2(ymd);
      _vExpyDIdx.push_back(_vTrdgD.size()-1);
      _mCtrt_TrdgDIdx[ymd2.ToIntYM()] = _vTrdgD.size()-1;
    }
    if (sH.at(0)=='Y')
    {
      _vHalfDIdx.push_back(_vTrdgD.size()-1);
    }
    if (tz.NextToken()) _mCtrt_1FMthNum     [ymd.ToInt()] = boost::lexical_cast<int>(tz.GetToken());
    if (tz.NextToken()) _mCtrt_2FMthNum     [ymd.ToInt()] = boost::lexical_cast<int>(tz.GetToken());
    if (tz.NextToken()) _mCtrt_1FMthFutCode [ymd.ToInt()] = tz.GetToken().at(0);
    if (tz.NextToken()) _mCtrt_2FMthFutCode [ymd.ToInt()] = tz.GetToken().at(0);
    if (tz.NextToken()) _mCtrt_1FMthCallCode[ymd.ToInt()] = tz.GetToken().at(0);
    if (tz.NextToken()) _mCtrt_2FMthCallCode[ymd.ToInt()] = tz.GetToken().at(0);
    if (tz.NextToken()) _mCtrt_1FMthPutCode [ymd.ToInt()] = tz.GetToken().at(0);
    if (tz.NextToken()) _mCtrt_2FMthPutCode [ymd.ToInt()] = tz.GetToken().at(0);
  }
}

int HKFE::LocateTrdgD(YYYYMMDD ymd)
{
  // vector<YYYYMMDD>::iterator it;
  // if (!SLowerBound<vector<YYYYMMDD>::iterator,YYYYMMDD>::JustLarger(_vTrdgD.begin(),_vTrdgD.end(),ymd,it)) return -1;
  // int iPsn = distance(_vTrdgD.begin(),it);

  int iLowerBound = 0;
  int iUpperBound = _vTrdgD.size();
  int iPsn = iUpperBound / 2;
  while (_vTrdgD[iPsn] != ymd)
  {
    if (ymd > _vTrdgD[iPsn])
    {
      iLowerBound = iPsn;
    }
    else if (ymd < _vTrdgD[iPsn])
    {
      iUpperBound = iPsn + 1;
    }

    iPsn = (iUpperBound + iLowerBound) / 2;

    if (iUpperBound - iLowerBound == 2)
    {
      if (_vTrdgD[iPsn] == ymd)
      {
        return iPsn;
      }
      else
      {
        if (_vTrdgD[iPsn] > ymd) return iPsn;
        else return iUpperBound;
      }
    }
  }

  return iPsn;
}


//Shorthand for Futures
string HKFE::GetSymbol(char cInstr, int iY, int iM)
{
  return GetSymbol(cInstr,100,iY,iM);
}
//return format:
//Futures: F-2011-08
//Options: C-21400-2012-04
//Options: P-19800-2012-11
string HKFE::GetSymbol(char cInstr, int iK, int iY, int iM)
{
  if (!ValidityChk::Year(iY) ||
      !ValidityChk::Month(iM) ||
      !ValidityChk::HKFEDeriType(cInstr) ||
      !ValidityChk::HKFEOptStrike(iK)) return "";

  if (cInstr == 'F')
  {
    string s[3];
    s[0] = STool::CharToStr('F');
    s[1] = STool::ToStr(iY);
    s[2] = STool::PadLeft(STool::ToStr(iM),'0',2);
    return STool::ConstructStr(s,3,"-");
  }
  else
  {
    string s[4];
    s[0] = STool::CharToStr(cInstr);
    s[1] = STool::PadLeft(STool::ToStr(iK),'0',5);
    s[2] = STool::ToStr(iY);
    s[3] = STool::PadLeft(STool::ToStr(iM),'0',2);
    return STool::ConstructStr(s,4,"-");
  }
}

bool HKFE::IsCall(const string &sSym)
{
  if (sSym.length() <= 5) return false;
  char c = sSym.at(sSym.length()-2);

  if (c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' || c == 'G' || c == 'H' || c == 'I' || c == 'J' || c == 'K' || c == 'L') return true;
  else return false;
}

bool HKFE::IsPut(const string &sSym)
{
  if (sSym.length() <= 5) return false;
  char c = sSym.at(sSym.length()-2);

  if (c == 'M' || c == 'N' || c == 'O' || c == 'P' || c == 'Q' || c == 'R' || c == 'S' || c == 'T' || c == 'U' || c == 'V' || c == 'W' || c == 'X') return true;
  else return false;
}

bool HKFE::IsFut(const string &sSym)
{
  if (sSym.length() != 5) return false;
  char c = sSym.at(3);

  if (c == 'F' || c == 'G' || c == 'H' || c == 'J' || c == 'K' || c == 'M' || c == 'N' || c == 'Q' || c == 'U' || c == 'V' || c == 'X' || c == 'Z') return true;
  else return false;
}


double HKFE::GetStrike(const string &sSym)
{
  if (!IsCall(sSym) && !IsPut(sSym)) return -1;

  // string sK;
  // Tokenizer tz(sSym);
  // if (tz.NextToken()) tz.GetToken();
  // if (tz.NextToken())
  // {
  //   sK = tz.GetToken();
  //   return STool::ToDbl(sK);
  // }
  // else
  // {
  //   return -1;
  // }

  return STool::ToDbl(sSym.substr(3,sSym.length()-3-2));
}


int HKFE::VerbatimYMi(const string &sSym)
{
  if (!IsFut(sSym) && !IsCall(sSym) && !IsPut(sSym))
  {
    return -1;
  }
  else
  {
    string sY = "";
    string sM = "";
    Tokenizer tz(sSym);
    if (tz.NextToken()) tz.GetToken();
    if (!IsFut(sSym)) if (tz.NextToken()) tz.GetToken();
    if (tz.NextToken())
    {
      sY = tz.GetToken();
    }
    if (tz.NextToken())
    {
      sM = tz.GetToken();
    }
    if (sY == "" || sM == "")
    {
      return -1;
    }
    else return boost::lexical_cast<int>(sY)*100+boost::lexical_cast<int>(sM);
  }
}

YYYYMMDD HKFE::GetExpyFmtMxx(const ContractType ct, const string & sSym)
{
  string sSym2 = sSym.substr(sSym.length()-3,3);
  int iY = -1;
  int iM = -1;
  iY = 2000+boost::lexical_cast<int>(sSym2.substr(1,2));
  if (ct == FUTURES)
  {
    switch(sSym2.at(0))
    {
      case 'F': { iM =  1; break; }
      case 'G': { iM =  2; break; }
      case 'H': { iM =  3; break; }
      case 'J': { iM =  4; break; }
      case 'K': { iM =  5; break; }
      case 'M': { iM =  6; break; }
      case 'N': { iM =  7; break; }
      case 'Q': { iM =  8; break; }
      case 'U': { iM =  9; break; }
      case 'V': { iM = 10; break; }
      case 'X': { iM = 11; break; }
      case 'Z': { iM = 12; break; }
    }
  }
  else if (ct == CALL)
  {
    switch(sSym2.at(0))
    {
      case 'A': { iM =  1; break; }
      case 'B': { iM =  2; break; }
      case 'C': { iM =  3; break; }
      case 'D': { iM =  4; break; }
      case 'E': { iM =  5; break; }
      case 'F': { iM =  6; break; }
      case 'G': { iM =  7; break; }
      case 'H': { iM =  8; break; }
      case 'I': { iM =  9; break; }
      case 'J': { iM = 10; break; }
      case 'K': { iM = 11; break; }
      case 'L': { iM = 12; break; }
    }
  }
  else if (ct == PUT)
  {
    switch(sSym2.at(0))
    {
      case 'M': { iM =  1; break; }
      case 'N': { iM =  2; break; }
      case 'O': { iM =  3; break; }
      case 'P': { iM =  4; break; }
      case 'Q': { iM =  5; break; }
      case 'R': { iM =  6; break; }
      case 'S': { iM =  7; break; }
      case 'T': { iM =  8; break; }
      case 'U': { iM =  9; break; }
      case 'V': { iM = 10; break; }
      case 'W': { iM = 11; break; }
      case 'X': { iM = 12; break; }
    }
  }

  int iCtrtYM = iY * 100 + iM;
  if (!STool::ContainsKey(_mCtrt_TrdgDIdx,iCtrtYM))
  {
    return -1;
  }

  return (_vTrdgD[_mCtrt_TrdgDIdx[iCtrtYM]]);
}

YYYYMMDD HKFE::GetExpy(const string &sSym, int iFM)
{
  int iCtrtYM = VerbatimYMi(sSym);

  if (!STool::ContainsKey(_mCtrt_TrdgDIdx,iCtrtYM))
  {
    return -1;
  }

  return (_vTrdgD[_mCtrt_TrdgDIdx[iCtrtYM]+iFM-1]);
}

YYYYMMDD HKFE::GetExpy(YMD ymd, int iFM)
{
  if (_vTrdgD.empty() || _vExpyDIdx.empty()) return YYYYMMDD();

  YYYYMMDD ymd2;
  ymd.ToYYYYMMDD(ymd2);
  int iPsn = LocateTrdgD(ymd2);

  if (iPsn < 0) return YYYYMMDD(19700101);

  vector<int>::iterator it;
  if (!SLowerBound<vector<int>::iterator,int>::JustLarger(_vExpyDIdx.begin(),_vExpyDIdx.end(),iPsn,it))
  {
    return -1;
  }

  return (_vTrdgD[*(it+iFM-1)]);
}

//return: e.g. F12, G10
string HKFE::FutCode(int iY, int iM)
{
  string sR = "";

  switch (iM)
  {
    case 1: sR += "F"; break;
    case 2: sR += "G"; break;
    case 3: sR += "H"; break;
    case 4: sR += "J"; break;
    case 5: sR += "K"; break;
    case 6: sR += "M"; break;
    case 7: sR += "N"; break;
    case 8: sR += "Q"; break;
    case 9: sR += "U"; break;
    case 10: sR += "V"; break;
    case 11: sR += "X"; break;
    case 12: sR += "Z"; break;
  }
  string sY = "";
  if ((iY % 100) == 0) sY = "00";
  else if ((iY % 100) < 10) sY = "0" + STool::ToStr(iY % 100);
  else sY = STool::ToStr(iY % 100);

  sR += sY;
  return sR;
}

bool HKFE::WhichFrontMth(const int iCurYMD, const int iCtrtYM, int & iWhichFM)
{
  if (_vTrdgD.empty()) return false;
  if (_vExpyDIdx.empty()) return false;
  if (!STool::ContainsKey(_mCtrt_TrdgDIdx,iCtrtYM)) return false;

  int iPsnCtrt = _mCtrt_TrdgDIdx[iCtrtYM];
  int iPsnCur = LocateTrdgD(iCurYMD);
  int iTgtDTM = iPsnCtrt-iPsnCur;

  int iTgtFM = 1;
  int iTgtDiff = 0;

  do
  {
    iTgtDiff = GetDTM(iCurYMD,iTgtFM) - iTgtDTM - 1;
    if (iTgtDiff > 0) iTgtFM -= 1;
    else if (iTgtDiff < 0) iTgtFM += 1;
  }
  while (iTgtDiff != 0 && iTgtFM >= 1);

  if (iTgtFM >= 1)
  {
    iWhichFM = iTgtFM;
    return true;
  }
  else return false;
}

//========================================================
//Time to Maturity - Begin
//========================================================
//iFM = Front Month. e.g. 1 = 1st First Mth, 2 = 2nd Front Mth
long HKFE::GetDTM(YYYYMMDD ymd, int iFM)
{
  if (_vTrdgD.empty() || _vExpyDIdx.empty()) return -1;

  //Error case
  if (_vTrdgD.size() < 10 || iFM < 1)
  {
    return -1;
  }

  //Locate the specified date
  vector<int>::iterator it;
  int iPsn = LocateTrdgD(ymd);

  if (!SLowerBound<vector<int>::iterator,int>::JustLarger(_vExpyDIdx.begin(),_vExpyDIdx.end(),iPsn,it))
  {
    return -1;
  }
  else
  {
    return *(it+iFM-1) - iPsn + 1;
  }

}

double HKFE::GetTTM(YYYYMMDDHHMMSS ymdhms, int iFM)
{
  double dDTM = GetDTM(ymdhms.GetYYYYMMDD(),iFM);

  //--------------------------------------------------
  // Approximate for now
  //--------------------------------------------------
  return dDTM / _TRDG_DAY_PER_YR;

  // YMD ymd(ymdhms.GetYYYYMMDD());
  // HMS hms(ymdhms.GetHHMMSS());
  // GetAftnClose(ymd,hms);
}

//========================================================
//Time to Maturity - End
//========================================================



//returns -1 / BAD_DATE if no more
void HKFE::NextTrdgDate(YMD & ymd, unsigned iNumDay)
{
  if (_vTrdgD.empty() || _vExpyDIdx.empty())
  {
    ymd.Invalidate();
    return;
  }

  YYYYMMDD ymd2;
  ymd.ToYYYYMMDD(ymd2);
  int iPsn = LocateTrdgD(ymd2);

  if (iPsn+iNumDay >= _vTrdgD.size()-1) ymd.Invalidate();
  else ymd.FromYYYYMMDD(_vTrdgD[iPsn+iNumDay]);
}



//returns -1 if that's the last trading minute for the trading day
void HKFE::NextTrdgMin(YMD & ymd, HMS & hms)
{
  hms.AddMinute(1);
  HKFE::TRDGSESS ts = GetTrdgSess(ymd,hms);

  if (ts == MRNG_SESS || ts == AFTN_SESS)
  {
    return;
  }
  else
  {
    if      (ts == BEFORE_OPEN) GetMrngOpen(ymd,hms);
    else if (ts == LUNCH_BRK) GetAftnOpen(ymd,hms);
    else if (ts == AFT_CLOSE) { NextTrdgDate(ymd,1); GetMrngOpen(ymd,hms); }

    return;
  }
}


// returns 1 if morning session
// returns 2 if afternoon session
// returns 7 if before market open
// returns 8 if lunch break
// returns 9 if after market close
HKFE::TRDGSESS HKFE::GetTrdgSess(const YMD & ymd, const HMS & hms)
{
  //????????-20110304: 945-1230, 1430-1615
  //20110307-20120302: 915-1200, 1330-1615
  //20120305-20130405: 915-1200, 1300-1615
  //20130408-????????: 915-1200, 1300-1615, 1700-2300
  if      (ymd >= ymd20130408) { if (hms < hms91500) return BEFORE_OPEN; else if (hms <= hms120000) return MRNG_SESS; else if (hms < hms130000) return LUNCH_BRK; else if (hms <= hms161500) return AFTN_SESS; else return AFT_CLOSE; }
  else if (ymd >= ymd20120305) { if (hms < hms91500) return BEFORE_OPEN; else if (hms <= hms120000) return MRNG_SESS; else if (hms < hms130000) return LUNCH_BRK; else if (hms <= hms161500) return AFTN_SESS; else return AFT_CLOSE; }
  else if (ymd >= ymd20110307) { if (hms < hms91500) return BEFORE_OPEN; else if (hms <= hms120000) return MRNG_SESS; else if (hms < hms133000) return LUNCH_BRK; else if (hms <= hms161500) return AFTN_SESS; else return AFT_CLOSE; }
  else if (ymd >= ymd20000000) { if (hms < hms94500) return BEFORE_OPEN; else if (hms <= hms123000) return MRNG_SESS; else if (hms < hms143000) return LUNCH_BRK; else if (hms <= hms161500) return AFTN_SESS; else return AFT_CLOSE; }
  return UNDEFMKTSTS;
}

void HKFE::GetMrngOpen(const YMD & ymd, HMS & hms)
{
  if      (ymd >= ymd20130408) { hms.Set( 9,15,0); }
  else if (ymd >= ymd20120305) { hms.Set( 9,15,0); }
  else if (ymd >= ymd20110307) { hms.Set( 9,15,0); }
  else if (ymd >= ymd20000000) { hms.Set( 9,45,0); }
}

void HKFE::GetMrngClose(const YMD & ymd, HMS & hms)
{
  if      (ymd >= ymd20130408) { hms.Set(12, 0,0); }
  else if (ymd >= ymd20120305) { hms.Set(12, 0,0); }
  else if (ymd >= ymd20110307) { hms.Set(12, 0,0); }
  else if (ymd >= ymd20000000) { hms.Set(12,30,0); }
}

void HKFE::GetAftnOpen(const YMD & ymd, HMS & hms)
{
  if      (ymd >= ymd20130408) { hms.Set(13, 0,0); }
  else if (ymd >= ymd20120305) { hms.Set(13, 0,0); }
  else if (ymd >= ymd20110307) { hms.Set(13,30,0); }
  else if (ymd >= ymd20000000) { hms.Set(14,30,0); }
}

void HKFE::GetAftnClose(const YMD & ymd, HMS & hms)
{
  if      (ymd >= ymd20130408) { hms.Set(16,15,0); }
  else if (ymd >= ymd20120305) { hms.Set(16,15,0); }
  else if (ymd >= ymd20110307) { hms.Set(16,15,0); }
  else if (ymd >= ymd20000000) { hms.Set(16,15,0); }
}


// bool HKFE::IsTradingHour(const YYYYMMDDHHMMSS & ymdhms)
// {
//   return IsTradingHour(ymdhms.GetYYYYMMDD(),ymdhms.GetHHMMSS());
// }
//
// bool HKFE::IsTradingHour(const YYYYMMDD & ymdA, const HHMMSS & hmsA)
// {
//   YMD ymd(ymdA);
//   HMS hms(hmsA);
//   return IsTradingHour(ymd,hms);
// }
//
// bool HKFE::IsTradingHour(const YMD & ymd, const HMS & hms)
// {
//   HKFE::TRDGSESS ts = GetTrdgSess(ymd,hms);
//   return (ts == MRNG_SESS || ts == AFTN_SESS);
// }

bool HKFE::IsTrdgHour5m(const YYYYMMDD & ymdA, const HHMMSS & hmsA)
{
  YMD ymd(ymdA);
  HMS hms(hmsA);
  return IsTrdgHour5m(ymd,hms);
}

bool HKFE::IsTrdgHour5m(const YMD & ymd, const HMS & hms)
{
  HMS hmsAdd5m(hms);
  HMS hmsSub5m(hms);

  hmsAdd5m.AddMinute(5);
  hmsSub5m.SubtractMinute(5);

  HKFE::TRDGSESS ts5a = GetTrdgSess(ymd,hmsAdd5m);
  HKFE::TRDGSESS ts5b = GetTrdgSess(ymd,hmsSub5m);
  return (ts5a == MRNG_SESS || ts5a == AFTN_SESS) && (ts5b == MRNG_SESS || ts5b == AFTN_SESS);
}


bool HKFE::IsTrdgHour10m(const YYYYMMDD & ymdA, const HHMMSS & hmsA)
{
  YMD ymd(ymdA);
  HMS hms(hmsA);
  return IsTrdgHour10m(ymd,hms);
}

bool HKFE::IsTrdgHour10m(const YMD & ymd, const HMS & hms)
{
  HMS hmsAdd10m(hms);
  HMS hmsSub10m(hms);

  hmsAdd10m.AddMinute(10);
  hmsSub10m.SubtractMinute(10);

  HKFE::TRDGSESS ts10a = GetTrdgSess(ymd,hmsAdd10m);
  HKFE::TRDGSESS ts10b = GetTrdgSess(ymd,hmsSub10m);
  return (ts10a == MRNG_SESS || ts10a == AFTN_SESS) && (ts10b == MRNG_SESS || ts10b == AFTN_SESS);
}

bool HKFE::IsExpyDay(const YMD & ymd)
{
  YYYYMMDD ymd2(ymd);
  return IsExpyDay(ymd2);
}

bool HKFE::IsExpyDay(const YYYYMMDD & ymd)
{
  int iPsn = LocateTrdgD(ymd);
  vector<int>::iterator it = std::find(_vExpyDIdx.begin(),_vExpyDIdx.end(),iPsn);

  if      (it != _vExpyDIdx.end()) return true;
  else if (it == _vExpyDIdx.end()) return false;
  else return false;

}

bool HKFE::IsHalfDay(const YMD & ymd)
{
  YYYYMMDD ymd2(ymd);
  return IsHalfDay(ymd2);
}

bool HKFE::IsHalfDay(const YYYYMMDD & ymd)
{
  int iPsn = LocateTrdgD(ymd);
  vector<int>::iterator it = std::find(_vHalfDIdx.begin(),_vHalfDIdx.end(),iPsn);

  if      (it != _vHalfDIdx.end()) return true;
  else if (it == _vHalfDIdx.end()) return false;
  else return false;

}

bool HKFE::IsTrdgDay(const YMD & ymd)
{
  YYYYMMDD ymd2(ymd);
  return IsTrdgDay(ymd2);
}

bool HKFE::IsTrdgDay(const YYYYMMDD & ymd)
{
  vector<YYYYMMDD>::iterator it = std::find(_vTrdgD.begin(),_vTrdgD.end(),ymd);

  if      (it != _vTrdgD.end()) return true;
  else if (it == _vTrdgD.end()) return false;
  else return false;

}

string HKFE::GetCtrtCode1_WithDTM(const ContractType ct, const YYYYMMDD & ymd, const int atleastdaystomat)
{
  int iFrontMth = 1;
  long lDTM = GetDTM(ymd,iFrontMth);
  while (lDTM < atleastdaystomat)
  {
    lDTM = GetDTM(ymd,++iFrontMth);
  }
  return GetCtrtCode1(ct,ymd,iFrontMth);
}
string HKFE::GetCtrtCode1(const ContractType ct, const YYYYMMDD & ymd, const int fm)
{
  string sTmp = GetCtrtCode2(ct,ymd,fm);
  if (sTmp.length() != 3) return "";
  ostringstream oo;
  oo << sTmp.at(0);
  oo << sTmp.at(2);
  return oo.str();
}

//--------------------------------------------------
string HKFE::GetCtrtCode2(const ContractType ct, const YYYYMMDD & ymd, const int fm)
{
  ostringstream oo;
  int iYMD = ymd.ToInt();

  map<int,char>::iterator it;
  if (fm == 1)
  {
    if      (ct == FUTURES) it = _mCtrt_1FMthFutCode.find(iYMD);
    else if (ct == CALL)    it = _mCtrt_1FMthCallCode.find(iYMD);
    else if (ct == PUT)     it = _mCtrt_1FMthPutCode.find(iYMD);

    if ((ct == FUTURES && it != _mCtrt_1FMthFutCode.end()) ||
        (ct == CALL    && it != _mCtrt_1FMthCallCode.end()) ||
        (ct == PUT     && it != _mCtrt_1FMthPutCode.end()))
    {
      oo << it->second;
    }
    else return "";
  }
  else if (fm == 2)
  {
    if      (ct == FUTURES) it = _mCtrt_2FMthFutCode.find(iYMD);
    else if (ct == CALL)    it = _mCtrt_2FMthCallCode.find(iYMD);
    else if (ct == PUT)     it = _mCtrt_2FMthPutCode.find(iYMD);

    if ((ct == FUTURES && it != _mCtrt_2FMthFutCode.end()) ||
        (ct == CALL    && it != _mCtrt_2FMthCallCode.end()) ||
        (ct == PUT     && it != _mCtrt_2FMthPutCode.end()))
    {
      oo << it->second;
    }
    else return "";
  }


  //--------------------------------------------------
  // May need to move to next year if Nov / Dec
  //--------------------------------------------------
  if ((iYMD/100) %100 >= 11)
  {
    if (it->second == 'K' || it->second == 'L' || it->second == 'W' || it->second == 'X' || it->second == 'Z')
    {
      oo << std::setw(2) << std::setfill('0') << (iYMD/10000 %100);
    }
    else if (it->second == 'A' || it->second == 'F' || it->second == 'M' || it->second == 'B' || it->second == 'N' || it->second == 'G')
    {
      oo << std::setw(2) << std::setfill('0') << (iYMD/10000 %100)+1;
    }
    else
    {
      oo << std::setw(2) << std::setfill('0') << (iYMD/10000 %100);
    }
  }
  else
  {
    oo << std::setw(2) << std::setfill('0') << (iYMD/10000 %100);
  }
  return oo.str();
}


string HKFE::Get1FMFutCodeAll()
{
  ostringstream oo;
  map<int,char>::iterator it;

  for (it = _mCtrt_1FMthFutCode.begin(); it != _mCtrt_1FMthFutCode.end(); ++it)
  {
    oo << it->first;
    oo << ": ";
    oo << it->second;
    oo << endl;
  }

  return oo.str();
}

string HKFE::Get2FMFutCodeAll()
{
  ostringstream oo;
  map<int,char>::iterator it;

  for (it = _mCtrt_2FMthFutCode.begin(); it != _mCtrt_2FMthFutCode.end(); ++it)
  {
    oo << it->first;
    oo << ": ";
    oo << it->second;
    oo << endl;
  }

  return oo.str();
}



bool HKFE::IsAProductInThisMarket(const string & symbol)
{
  string sSubStr;
  if (symbol.length() >= 3) sSubStr = symbol.substr(0,3);

  if (
    (symbol.length() == 5 || symbol.length() == 9 || symbol.length() == 10)
    &&
    (
      sSubStr == UNDERLYING_HSI || sSubStr == UNDERLYING_HHI
      ||
      sSubStr == UNDERLYING_MHI || sSubStr == UNDERLYING_MCH
    )
    ) return true;

  return false;
}

double HKFE::GetContractMultiplier(const string & symbol)
{
  if (symbol.length() <= 3) return NAN;

  string sSubStr = symbol.substr(0,3);

  if      (sSubStr == UNDERLYING_HSI || sSubStr == UNDERLYING_HHI) return 50;
  else if (sSubStr == UNDERLYING_MHI || sSubStr == UNDERLYING_MCH) return 10;
  else                                                             return NAN;
}
