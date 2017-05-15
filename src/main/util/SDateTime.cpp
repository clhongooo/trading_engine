#include "SDateTime.h"

int SDateTime::DAYSINMTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
scoped_ptr<boost::gregorian::date> SDateTime::EpochDate;
YYYYMMDD YYYYMMDD::BAD_DATE(-1);
YMD YMD::BAD_DATE(0,0,-1);
HHMMSS HHMMSS::BAD_TIME(-1);
HHMM HHMM::BAD_TIME(-1);
HMS HMS::BAD_TIME(0,0,-1);

bool YYYYMMDD::operator==(const YYYYMMDD& ymd) const { if (!this->IsValid() && !ymd.IsValid()) return true; else if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD == ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator!=(const YYYYMMDD& ymd) const { return !(*this == ymd); }
bool YYYYMMDD::operator> (const YYYYMMDD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD >  ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator>=(const YYYYMMDD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD >= ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator< (const YYYYMMDD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD <  ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator<=(const YYYYMMDD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD <= ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator==(const YMD& ymd) const { if (!this->IsValid() && !ymd.IsValid()) return true; else if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD == ymd.ToInt()) ? true : false; }
bool YYYYMMDD::operator!=(const YMD& ymd) const { return !(*this == ymd); }
bool YYYYMMDD::operator> (const YMD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD >  ymd.ToInt()) ? true : false; }
bool YYYYMMDD::operator>=(const YMD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD >= ymd.ToInt()) ? true : false; }
bool YYYYMMDD::operator< (const YMD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD <  ymd.ToInt()) ? true : false; }
bool YYYYMMDD::operator<=(const YMD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (_iYMD <= ymd.ToInt()) ? true : false; }
bool YMD::operator==(const YYYYMMDD& ymd) const { if (!this->IsValid() && !ymd.IsValid()) return true; else if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() == ymd._iYMD) ? true : false; }
bool YMD::operator!=(const YYYYMMDD& ymd) const { return !(*this == ymd); }
bool YMD::operator> (const YYYYMMDD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() >  ymd._iYMD) ? true : false; }
bool YMD::operator>=(const YYYYMMDD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() >= ymd._iYMD) ? true : false; }
bool YMD::operator< (const YYYYMMDD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() <  ymd._iYMD) ? true : false; }
bool YMD::operator<=(const YYYYMMDD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() <= ymd._iYMD) ? true : false; }
bool YMD::operator==(const YMD& ymd) const { if (!this->IsValid() && !ymd.IsValid()) return true; else if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() == ymd.ToInt()) ? true : false; }
bool YMD::operator!=(const YMD& ymd) const { return !(*this == ymd); }
bool YMD::operator> (const YMD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() >  ymd.ToInt()) ? true : false; }
bool YMD::operator>=(const YMD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() >= ymd.ToInt()) ? true : false; }
bool YMD::operator< (const YMD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() <  ymd.ToInt()) ? true : false; }
bool YMD::operator<=(const YMD& ymd) const { if (!this->IsValid() || !ymd.IsValid()) return false; return (this->ToInt() <= ymd.ToInt()) ? true : false; }


YYYYMMDD::YYYYMMDD(const YMD &ymd) { _iYMD = ymd.ToInt(); }

int YYYYMMDD::ToInt() const { return _iYMD; }

int YYYYMMDD::ToIntYM() const { return _iYMD / 100; }

const string YYYYMMDD::ToStrYM() const { if (IsValid()) { return boost::lexical_cast<string>(_iYMD/100); } else return "yyyymm"; }
const string YYYYMMDD::ToStr()   const { if (IsValid()) { return boost::lexical_cast<string>(_iYMD); } else return "yyyymmdd"; }
int YYYYMMDD::GetDecade() const { return (_iYMD / 100000) % 10; }

long YYYYMMDD::operator-(const YYYYMMDD& ymda) const
{
  return (long)abs(SDateTime::CalendarDayDiff(*this, ymda));
}

YYYYMMDD::YYYYMMDD() : _iYMD(-1) {}
YYYYMMDD::YYYYMMDD(const YYYYMMDD &ymd) { _iYMD = ymd._iYMD; }
YYYYMMDD::YYYYMMDD(int iA) : _iYMD(iA) {}
void YYYYMMDD::Set(int iYMD) { _iYMD = iYMD; }
void YYYYMMDD::Set(int iY,int iM,int iD) { _iYMD = iY*10000 + iM*100 + iD; }
void YYYYMMDD::Set(const YYYYMMDD & ymd) { _iYMD = ymd._iYMD; }
int YYYYMMDD::Value() const { return _iYMD; }
void YYYYMMDD::Invalidate() { _iYMD = -1; }
const bool YYYYMMDD::IsValid() const { return _iYMD > 0 ? true : false; }

YYYYMMDD::YYYYMMDD(const string &s)
{
  Set(s);
}
YYYYMMDD::YYYYMMDD(const int y,const int m,const int d)
{
  Set(y,m,d);
}

void YYYYMMDD::Set(const string &s)
{
  vector<string> vs;
  boost::split(vs, s, boost::is_any_of("-"));

  if (vs.size() == 1)
  {
    _iYMD = boost::lexical_cast<int>(s);

  }
  else if (vs.size() == 2)
  {
    throw std::exception();
  }
  else if (vs.size() == 3)
  {
    _iYMD = boost::lexical_cast<int>(vs[0])*10000 + boost::lexical_cast<int>(vs[1])*100 + boost::lexical_cast<int>(vs[2]);
  }
}

const int YYYYMMDD::Year() const
{
  if (IsValid())
    return _iYMD/10000;
  else
    return 0;
}

const int YYYYMMDD::Month() const
{
  if (IsValid())
    return (_iYMD/100)%100;
  else
    return 0;
}

const int YYYYMMDD::Day() const
{
  if (IsValid())
    return _iYMD%100;
  else
    return 0;
}

const string YYYYMMDD::ToStr_() const
{
  if (!IsValid()) return "yyyy-mm-dd";

  string sRtn = "";
  YMD ymdTmp(_iYMD);
  return STool::ToStr(ymdTmp.Year())+"-"+STool::PadLeft(ymdTmp.Month(),'0',2)+"-"+STool::PadLeft(ymdTmp.Day(),'0',2);
}

int YMD::ToInt() const { return (_iY*10000 + _iM*100 + _iD); }


YMD::YMD() { _iY = 0; _iM = 0; _iD = -1; }
YMD::YMD(const YMD & ymd) { _iY = ymd._iY; _iM = ymd._iM; _iD = ymd._iD; }
YMD::YMD(int iY,int iM,int iD) { _iY = iY; _iM = iM; _iD = iD; }
YMD::YMD(const YYYYMMDD & ymd) { FromYYYYMMDD(ymd); }
YMD::YMD(int iA) { _iY = iA/10000; _iM = (iA/100)%100; _iD = iA%100; }

// setters and getters
void  YMD::Set(const int iY, const int iM, const int iD)  { _iY = iY; _iM = iM; _iD = iD; }
void  YMD::Set(const YYYYMMDD & ymd)                      { _iY = ymd.Year(); _iM = ymd.Month(); _iD = ymd.Day(); }
void  YMD::SetYear(int iY)                                    { _iY = iY; }
void  YMD::SetMonth(int iM)                                   { _iM = iM; }
void  YMD::SetDay(int iD)                                     { _iD = iD; }
const int YMD::Year()  const   { return _iY; }
const int YMD::Month() const   { return _iM; }
const int YMD::Day()   const   { return _iD; }
const void YMD::Invalidate()    { _iY = 0; _iM = 0; _iD = -1; }
const bool YMD::IsValid() const { return (_iY > 0 && _iM > 0 && _iD > 0) ? true : false; }


void YMD::AddYear(int iY) { _iY += iY; }
void YMD::SubtractYear(int iY) { _iY -= iY; }
void YMD::AddMonth(int iM) { if (iM < 0) { SubtractMonth(-iM); return; } _iM += iM; _iY += ((_iM-1)/12); _iM = ((_iM-1)%12)+1; }
void YMD::SubtractMonth(int iM) { if (iM < 0) { AddMonth(-iM); return; } _iY -= (iM / 12); iM = (iM % 12); if (iM >= _iM) { --_iY; iM-=_iM; _iM=12; } _iM-=iM; }



void YMD::ToYYYYMMDD(YYYYMMDD & yyyymmdd) const { yyyymmdd.Set(_iY,_iM,_iD); }
const string YMD::ToStrYM()  const { if (IsValid()) { return boost::lexical_cast<string>(_iY*100 + _iM); } else return "yyyymm"; }
const string YMD::ToStrYM_() const { if (IsValid()) { return boost::lexical_cast<string>(_iY) + "-" + STool::PadLeft(_iM,'0',2); } else return "yyyy-mm"; }
const string YMD::ToStr()    const { if (IsValid()) { return boost::lexical_cast<string>(_iY*10000 + _iM*100 + _iD); } else return "yyyymmdd"; }
const string YMD::ToStr_()   const { if (IsValid()) { return boost::lexical_cast<string>(_iY) + "-" + STool::PadLeft(_iM,'0',2) + "-" + STool::PadLeft(_iD,'0',2); } else return "yyyy-mm-dd"; }
int YMD::ToIntYM() const { return _iY * 100 + _iM; }

void YMD::FromYYYYMMDD(const YYYYMMDD & yyyymmdd) { this->Set(yyyymmdd._iYMD/10000,(yyyymmdd._iYMD%10000)/100,yyyymmdd._iYMD%100); }




void YMD::AddDay(int iD)
{
  if (iD < 0) { SubtractDay(-iD); return; }

  int iDaysInMth = SDateTime::DaysInMonth(_iY,_iM);
  while (_iD + iD > iDaysInMth)
  {
    iD = iD - (iDaysInMth - _iD + 1);
    _iD = 1;
    AddMonth(1);
    iDaysInMth = SDateTime::DaysInMonth(_iY,_iM);
  }
  _iD += iD;
}
void YMD::SubtractDay(int iD)
{
  if (iD < 0) { AddDay(-iD); return; }

  while (_iD - iD <= 0)
  {
    SubtractMonth(1);
    iD -= _iD;
    _iD = SDateTime::DaysInMonth(_iY,_iM);
  }
  _iD -= iD;
}

HHMMSS::HHMMSS(const HMS &hms) { _iHMS = hms.ToInt(); }
HHMM::HHMM(const HMS &hms) { _iHM = hms.ToInt()/100; }
HHMM::HHMM(const HHMMSS &hms) { _iHM = hms._iHMS/100; }

HHMMSS::HHMMSS(const string &s) { Set(s); }
HHMM::HHMM(const string &s) { Set(s); }

HHMMSS::HHMMSS(int iA)
{
  if (iA < 0) _iHMS = -1;
  else _iHMS = iA;
}
HHMMSS::HHMMSS(const int h,const int m,const int s)
{
  Set(h,m,s);
}
HHMM::HHMM(int iA)
{
  if (iA < 0) _iHM = -1;
  _iHM = iA;
}

void HHMMSS::Set(int iHMS)             { _iHMS = iHMS; }
void HHMMSS::Set(int iH,int iM,int iS) { _iHMS = iH*10000 + iM*100 + iS; }
void HHMMSS::Set(int iH,int iM)        { _iHMS = iH*10000 + iM*100; }
void HHMMSS::Set(const HHMMSS & hms)   { _iHMS = hms._iHMS; }

void HHMM::Set(int iHM_)             { if (iHM_ > 9999) _iHM = iHM_/100; else _iHM = iHM_; }
void HHMM::Set(int iH,int iM)        { _iHM = iH*100 + iM; }
void HHMM::Set(const HHMM & hm)      { _iHM = hm._iHM; }

void HHMMSS::Set(const string &s)
{
  string sH,sM,sS;

  vector<string> vs;
  boost::split(vs, s, boost::is_any_of(":"));

  if (vs.size() < 2)
  {
    _iHMS = boost::lexical_cast<int>(s);
    return;
  }

  sH = vs[0];
  sM = vs[1];
  if (vs.size() >= 3) sS = vs[2];
  else sS = "00";

  _iHMS = 0;
  _iHMS += boost::lexical_cast<int>(sH)*10000;
  _iHMS += boost::lexical_cast<int>(sM)*100;
  _iHMS += boost::lexical_cast<int>(sS);
}

void HHMM::Set(const string &s)
{
  string sH,sM;

  Tokenizer tz(s,":");
  if (tz.NextToken()) sH = tz.GetToken();
  else throw std::exception();
  if (tz.NextToken()) sM = tz.GetToken();
  else throw std::exception();

  _iHM = 0;
  _iHM += boost::lexical_cast<int>(sH)*100;
  _iHM += boost::lexical_cast<int>(sM);
}

HHMMSS::HHMMSS() : _iHMS(-1) {}
HHMM::HHMM() : _iHM(-1) {}

HHMMSS::HHMMSS(const HHMMSS & hms) { _iHMS = hms._iHMS; }
HHMM::HHMM(const HHMM & hm) { _iHM = hm._iHM; }

void HHMMSS::Invalidate() { _iHMS = -1; }
void HHMM::Invalidate() { _iHM = -1; }

const int  HHMMSS::Value() const { return _iHMS; }
const int  HHMM::Value() const { return _iHM; }

const bool HHMMSS::IsValid() const { return _iHMS >= 0 ? true : false; }
const bool HHMM::IsValid() const { return _iHM >= 0 ? true : false; }

const int HHMMSS::Hour()   const { return _iHMS / 10000; }
const int HHMMSS::Minute() const { return (_iHMS / 100) % 100; }
const int HHMMSS::Second() const { return _iHMS % 100; }

const int HHMM::Hour()   const { return _iHM / 100; }
const int HHMM::Minute() const { return _iHM % 100; }

int HHMMSS::ToInt() const { return _iHMS; }
int HHMM::ToInt()   const { return _iHM; }

const string HHMMSS::ToStrHM() const { if (IsValid()) { std::ostringstream ss; ss << std::setw(4) << std::setfill('0') << _iHMS/100; return ss.str(); } else return "hhmm"; }
const string HHMM::ToStr()     const { if (IsValid()) { std::ostringstream ss; ss << std::setw(4) << std::setfill('0') << _iHM; return ss.str(); } else return "hhmm"; }

const string HHMMSS::ToStr() const { if (IsValid()) { return STool::PadLeft(_iHMS,'0',6); } else return "hhmmss"; }

const string HHMMSS::ToStrHM_() const {
  if (!IsValid()) return "hh:mm";

  std::ostringstream ss;
  if (_iHMS == -1)
  {
    ss << "--:--";
  }
  else
  {
    ss << std::setw(2) << std::setfill('0') << _iHMS/10000 << ":"
      << std::setw(2) << std::setfill('0') << (_iHMS%10000)/100;
  }
  return ss.str();
}
const string HHMM::ToStr_() const {
  if (!IsValid()) return "hh:mm";

  std::ostringstream ss;
  if (_iHM == -1)
  {
    ss << "--:--";
  }
  else
  {
    ss << std::setw(2) << std::setfill('0') << _iHM/100 << ":"
      << std::setw(2) << std::setfill('0') << _iHM%100;
  }
  return ss.str();
}
const string HHMMSS::ToStr_() const {
  if (!IsValid()) return "hh:mm:ss";

  std::ostringstream ss;
  if (_iHMS == -1)
  {
    ss << "--:--:--";
  }
  else
  {
    ss << std::setw(2) << std::setfill('0') << _iHMS/10000 << ":"
      << std::setw(2) << std::setfill('0') << (_iHMS%10000)/100 << ":"
      << std::setw(2) << std::setfill('0') << _iHMS%100;
  }
  return ss.str();
}


bool HHMMSS::operator==(const HHMMSS& hms) const { if (!this->IsValid() && !hms.IsValid()) return true; else if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS == hms._iHMS) ? true : false; }
bool HHMMSS::operator!=(const HHMMSS& hms) const { return !(*this == hms); }
bool HHMMSS::operator> (const HHMMSS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS >  hms._iHMS) ? true : false; }
bool HHMMSS::operator>=(const HHMMSS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS >= hms._iHMS) ? true : false; }
bool HHMMSS::operator< (const HHMMSS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS <  hms._iHMS) ? true : false; }
bool HHMMSS::operator<=(const HHMMSS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS <= hms._iHMS) ? true : false; }
bool HHMMSS::operator==(const HMS& hms) const { if (!this->IsValid() && !hms.IsValid()) return true; else if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS == hms.ToInt()) ? true : false; }
bool HHMMSS::operator!=(const HMS& hms) const { return !(*this == hms); }
bool HHMMSS::operator> (const HMS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS >  hms.ToInt()) ? true : false; }
bool HHMMSS::operator>=(const HMS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS >= hms.ToInt()) ? true : false; }
bool HHMMSS::operator< (const HMS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS <  hms.ToInt()) ? true : false; }
bool HHMMSS::operator<=(const HMS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (_iHMS <= hms.ToInt()) ? true : false; }
bool HMS::operator==(const HMS& hms) const { if (!this->IsValid() && !hms.IsValid()) return true; else if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() == hms.ToInt()) ? true : false; }
bool HMS::operator!=(const HMS& hms) const { return !(*this == hms); }
bool HMS::operator> (const HMS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() >  hms.ToInt()) ? true : false; }
bool HMS::operator>=(const HMS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() >= hms.ToInt()) ? true : false; }
bool HMS::operator< (const HMS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() <  hms.ToInt()) ? true : false; }
bool HMS::operator<=(const HMS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() <= hms.ToInt()) ? true : false; }
bool HMS::operator==(const HHMMSS& hms) const { if (!this->IsValid() && !hms.IsValid()) return true; else if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() == hms._iHMS) ? true : false; }
bool HMS::operator!=(const HHMMSS& hms) const { return !(*this == hms); }
bool HMS::operator> (const HHMMSS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() >  hms._iHMS) ? true : false; }
bool HMS::operator>=(const HHMMSS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() >= hms._iHMS) ? true : false; }
bool HMS::operator< (const HHMMSS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() <  hms._iHMS) ? true : false; }
bool HMS::operator<=(const HHMMSS& hms) const { if (!this->IsValid() || !hms.IsValid()) return false; return (this->ToInt() <= hms._iHMS) ? true : false; }

bool HHMM::operator==(const HHMM& hm) const { if (!this->IsValid() && !hm.IsValid()) return true; else if (!this->IsValid() || !hm.IsValid()) return false; return (_iHM == hm._iHM) ? true : false; }
bool HHMM::operator!=(const HHMM& hm) const { return !(*this == hm); }
bool HHMM::operator> (const HHMM& hm) const { if (!this->IsValid() || !hm.IsValid()) return false; return (_iHM >  hm._iHM) ? true : false; }
bool HHMM::operator>=(const HHMM& hm) const { if (!this->IsValid() || !hm.IsValid()) return false; return (_iHM >= hm._iHM) ? true : false; }
bool HHMM::operator< (const HHMM& hm) const { if (!this->IsValid() || !hm.IsValid()) return false; return (_iHM <  hm._iHM) ? true : false; }
bool HHMM::operator<=(const HHMM& hm) const { if (!this->IsValid() || !hm.IsValid()) return false; return (_iHM <= hm._iHM) ? true : false; }


long HHMMSS::operator- (const HHMMSS & hms2) const
{
  long hms1_sec = this->Hour()*60*60 + this->Minute()*60 + this->Second();
  long hms2_sec = hms2.Hour()*60*60 + hms2.Minute()*60 + hms2.Second();
  return (hms1_sec - hms2_sec);
}



int HMS::ToInt() const { return (_iH*10000+_iM*100+_iS); }

HMS::HMS()                     { _iH = 0; _iM = 0; _iS = -1; }
HMS::HMS(const int hms)        { _iH = hms/10000; _iM = (hms/100)%100; _iS = hms%100; }
HMS::HMS(const HMS & hms)      { _iH = hms._iH; _iM = hms._iM; _iS = hms._iS; }
HMS::HMS(const HHMMSS& hms)    { _iH = hms._iHMS/10000; _iM = (hms._iHMS/100)%100; _iS = hms._iHMS%100; }
HMS::HMS(int iH,int iM,int iS) { _iH = iH; _iM = iM; _iS = iS; }

// setters and getters
void HMS::SetHour(int iH) { _iH = iH; }
void HMS::SetMinute(int iM) { _iM = iM; }
void HMS::SetSecond(int iS) { _iS = iS; }
void HMS::Set(int iH,int iM,int iS) { _iH = iH; _iM = iM; _iS = iS; }
void HMS::Set(const HHMMSS & hms) { _iH = hms.Hour(); _iM = hms.Minute(); _iS = hms.Second(); }
int HMS::Hour() const { return _iH; }
int HMS::Minute() const { return _iM; }
int HMS::Second() const { return _iS; }
const bool HMS::IsValid() const { return(_iH >= 0 && _iM >= 0 && _iS >= 0) ? true : false; }
void HMS::Invalidate() { _iH = -1; _iM = -1; _iS = -1; }

void HMS::ToHHMMSS(HHMMSS & hhmmss) const { hhmmss.Set(_iH,_iM,_iS); }
HHMMSS HMS::ToHHMMSS()      const { return HHMMSS(ToInt()); }
const string HMS::ToStrHM() const { if (IsValid()) { std::ostringstream ss; ss << std::setw(4) << std::setfill('0') << _iH*100 + _iM; return ss.str(); } else return "hhmm"; }
const string HMS::ToStr()   const { if (IsValid()) { std::ostringstream ss; ss << std::setw(6) << std::setfill('0') << _iH*10000 + _iM*100 + _iS; return ss.str(); } else return "hhmmss"; }
const string HMS::ToStrHM_()const { if (IsValid()) { return STool::PadLeft(_iH,'0',2) + ":" + STool::PadLeft(_iM,'0',2); } else return "hh:mm"; }
const string HMS::ToStr_()  const { if (IsValid()) { return STool::PadLeft(_iH,'0',2) + ":" + STool::PadLeft(_iM,'0',2) + ":" + STool::PadLeft(_iS,'0',2); } else return "hh:mm:ss"; }

int HMS::AddMinute(int iM)
{
  if (iM < 0)
  {
    return SubtractMinute(-iM);
  }
  else
  {
    int iDays = 0;
    _iM += iM;
    _iH += (_iM / 60);
    iDays = _iH / 24;
    _iH = _iH % 24;
    _iM = _iM % 60;
    return iDays;
  }
}
int HMS::SubtractMinute(int iM)
{
  if (iM < 0)
  {
    return AddMinute(-iM);
  }
  else
  {
    int iDays = 0;
    int iHM = _iH * 60 + _iM;
    iHM -= iM;
    while (iHM < 0)
    {
      --iDays;
      iHM += 1440;
    }
    _iH = iHM / 60;
    _iM = iHM % 60;

    return iDays;
  }
}
int HMS::AddSecond(int iS)
{
  if (iS < 0)
  {
    return SubtractSecond(-iS);
  }
  else
  {
    int iDays = 0;
    _iS += iS;
    _iM += (_iS / 60);
    _iH += (_iM / 60);
    iDays = _iH / 24;
    _iH = _iH % 24;
    _iM = _iM % 60;
    _iS = _iS % 60;
    return iDays;
  }
}
int HMS::SubtractSecond(int iS)
{
  if (iS < 0)
  {
    return AddSecond(-iS);
  }
  else
  {
    int iDays = 0;
    int iHMS = _iH * 3600 + _iM * 60 + _iS;
    iHMS -= iS;
    while (iHMS < 0)
    {
      --iDays;
      iHMS += 86400;
    }
    _iH = iHMS / 3600;
    _iM = (iHMS % 3600) / 60;
    _iS = iHMS % 60;

    return iDays;
  }
}


YMDHMS::YMDHMS()
{
  _ymd.Set(0,0,-1);
  _hms.Set(0,0,-1);
}
YMDHMS::YMDHMS(const int iY,const int iM,const int iD, const int iH,const int iMin,const int iS)
{
  SetYMDHMS(iY,iM,iD,iH,iMin,iS);
}
YMDHMS::YMDHMS(const YYYYMMDDHHMMSS & ymdhms)
{
  Set(ymdhms.GetYYYYMMDD());
  Set(ymdhms.GetHHMMSS());
}
void YMDHMS::Set(const YMD& ymd, const HMS& hms)
{
  Set(ymd);
  Set(hms);
}
void YMDHMS::SetYMDHMS(const int iY,const int iM,const int iD, const int iH,const int iMin,const int iS)
{
  _ymd.Set(iY,iM,iD);
  _hms.Set(iH,iMin,iS);
}
void YMDHMS::SetYMD(const int iY,const int iM,const int iD)
{
  _ymd.Set(iY,iM,iD);
}

void YMDHMS::Set(const YMD& ymd)
{
  _ymd.Set(ymd.Year(),ymd.Month(),ymd.Day());
}

void YMDHMS::SetHMS(const int iH,const int iM,const int iS)
{
  _hms.Set(iH,iM,iS);
}

void YMDHMS::Set(const HMS& hms)
{
  _hms.Set(hms.Hour(),hms.Minute(),hms.Second());
}

void YMDHMS::Set(const YYYYMMDDHHMMSS & ymdhms)
{
  _ymd.Set(ymdhms.GetYYYYMMDD());
  _hms.Set(ymdhms.GetHHMMSS());
}

bool YMDHMS::operator==(const YMDHMS& ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;
  if (this->GetYMD() == ymdhms.GetYMD() && this->GetHMS() == ymdhms.GetHMS()) return true;
  else return false;
}

const YMD YMDHMS::GetYMD() const { return _ymd; }
const HMS YMDHMS::GetHMS() const { return _hms; }



void YMDHMS::AddMinute(int iM)
{
  int iCarryForward =_hms.AddMinute(iM);
  if (iCarryForward != 0)
    _ymd.AddDay(iCarryForward);
}

void YMDHMS::AddSecond(int iS)
{
  int iCarryForward =_hms.AddSecond(iS);
  if (iCarryForward != 0)
    _ymd.AddDay(iCarryForward);
}

YYYYMMDDHHMMSS::YYYYMMDDHHMMSS()
{
}
YYYYMMDDHHMMSS::YYYYMMDDHHMMSS(const YYYYMMDDHHMMSS &ymdhms)
{
  _ymd.Set(ymdhms._ymd);
  _hms.Set(ymdhms._hms);
}

YYYYMMDDHHMMSS::YYYYMMDDHHMMSS(const int iYYYYMMDD, const int iHHMMSS)
{
  _ymd.Set(iYYYYMMDD);
  _hms.Set(iHHMMSS);
}

YYYYMMDDHHMMSS::YYYYMMDDHHMMSS(const YYYYMMDD & ymd,const HHMMSS & hms)
{
  _ymd.Set(ymd);
  _hms.Set(hms);
}

YYYYMMDDHHMMSS::YYYYMMDDHHMMSS(const YMD & ymd,const HMS & hms)
{
  _ymd.Set(ymd);
  _hms.Set(hms);
}

YYYYMMDDHHMMSS::YYYYMMDDHHMMSS(const string & ymd,const string & hms)
{
  _ymd.Set(ymd);
  _hms.Set(hms);
}

YYYYMMDDHHMMSS::YYYYMMDDHHMMSS(const int y, const int m, const int d, const int h, const int min, const int s)
{
  _ymd.Set(y,m,d);
  _hms.Set(h,min,s);
}

YYYYMMDDHHMMSS::YYYYMMDDHHMMSS(const unsigned long ulUnixTime, const SDateTime::TIMEPRECISION tp, const char * c)
{
  boost::shared_ptr<boost::posix_time::ptime> p_pt = SDateTime::fromUnixTimeToPTime(ulUnixTime, tp, SDateTime::GMT, SDateTime::GMT);
  _ymd.Set(boost::lexical_cast<int>(SDateTime::FormatPTimeYYYYMMDD(*p_pt)));
  _hms.Set(boost::lexical_cast<int>(SDateTime::FormatPTimeHHMMSS(*p_pt)));
}

void YYYYMMDDHHMMSS::Set(const YYYYMMDD & ymd,const HHMMSS & hms)
{
  _ymd.Set(ymd);
  _hms.Set(hms);
}

void YYYYMMDDHHMMSS::Set(const string & ymd, const string & hms)
{
  _ymd.Set(ymd);
  _hms.Set(hms);
}

void YYYYMMDDHHMMSS::Set(const YYYYMMDDHHMMSS & ymdhms)
{
  _ymd.Set(ymdhms._ymd);
  _hms.Set(ymdhms._hms);
}

void YYYYMMDDHHMMSS::Set(const YMDHMS & ymdhms)
{
  _ymd.Set(ymdhms._ymd);
  _hms.Set(ymdhms._hms);
}

const YYYYMMDD YYYYMMDDHHMMSS::GetYYYYMMDD() const
{
  return _ymd;
}

const YYYYMMDD * YYYYMMDDHHMMSS::GetYYYYMMDDPtr() const
{
  return &_ymd;
}

void YYYYMMDDHHMMSS::GetYMD(YMD & ymdrtn) const
{
  ymdrtn.Set(_ymd);
}

const HHMMSS YYYYMMDDHHMMSS::GetHHMMSS() const
{
  return _hms;
}

const HHMMSS * YYYYMMDDHHMMSS::GetHHMMSSPtr() const
{
  return &_hms;
}

bool YYYYMMDDHHMMSS::operator==(const YYYYMMDDHHMMSS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if (_ymd == ymdhms._ymd &&
      _hms == ymdhms._hms)
    return true;
  else return false;
}

bool YYYYMMDDHHMMSS::operator!=(const YYYYMMDDHHMMSS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if (_ymd == ymdhms._ymd &&
      _hms == ymdhms._hms) return false;
  else return true;
}
bool YYYYMMDDHHMMSS::operator> (const YYYYMMDDHHMMSS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if      (_ymd > ymdhms._ymd) return true;
  else if (_ymd < ymdhms._ymd) return false;
  else if (_hms > ymdhms._hms) return true;
  else                               return false;
}

bool YYYYMMDDHHMMSS::operator>=(const YYYYMMDDHHMMSS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if      (_ymd > ymdhms._ymd) return true;
  else if (_ymd < ymdhms._ymd) return false;
  else if (_hms > ymdhms._hms) return true;
  else if (_hms < ymdhms._hms) return false;
  else                               return true;
}
bool YYYYMMDDHHMMSS::operator< (const YYYYMMDDHHMMSS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if      (_ymd > ymdhms._ymd) return false;
  else if (_ymd < ymdhms._ymd) return true;
  else if (_hms < ymdhms._hms) return true;
  else                               return false;
}

bool YYYYMMDDHHMMSS::operator<=(const YYYYMMDDHHMMSS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if      (_ymd > ymdhms._ymd) return false;
  else if (_ymd < ymdhms._ymd) return true;
  else if (_hms < ymdhms._hms) return true;
  else if (_hms > ymdhms._hms) return false;
  else                               return true;
}

bool YYYYMMDDHHMMSS::operator==(const YMDHMS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if (_ymd == (ymdhms._ymd) &&
      _hms == (ymdhms._hms))
    return true;
  else return false;
}

bool YYYYMMDDHHMMSS::operator!=(const YMDHMS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if (_ymd == ymdhms._ymd &&
      _hms == ymdhms._hms) return false;
  else return true;
}
bool YYYYMMDDHHMMSS::operator> (const YMDHMS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if      (_ymd > ymdhms._ymd) return true;
  else if (_ymd < ymdhms._ymd) return false;
  else if (_hms > ymdhms._hms) return true;
  else                              return false;
}

bool YYYYMMDDHHMMSS::operator>=(const YMDHMS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if      (_ymd > ymdhms._ymd) return true;
  else if (_ymd < ymdhms._ymd) return false;
  else if (_hms > ymdhms._hms) return true;
  else if (_hms < ymdhms._hms) return false;
  else                              return true;
}
bool YYYYMMDDHHMMSS::operator< (const YMDHMS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if      (_ymd > ymdhms._ymd) return false;
  else if (_ymd < ymdhms._ymd) return true;
  else if (_hms < ymdhms._hms) return true;
  else                              return false;
}

bool YYYYMMDDHHMMSS::operator<=(const YMDHMS & ymdhms) const
{
  if (!this->IsValid() || !ymdhms.IsValid()) return false;

  if      (_ymd > ymdhms._ymd) return false;
  else if (_ymd < ymdhms._ymd) return true;
  else if (_hms < ymdhms._hms) return true;
  else if (_hms > ymdhms._hms) return false;
  else                              return true;
}

const string YYYYMMDDHHMMSS::ToStr() const
{
  return _ymd.ToStr_() + " " + _hms.ToStr_();
}
const long YYYYMMDDHHMMSS::ToInt() const
{
  return _ymd.ToInt() * 1000001 + _hms.ToInt();
}

long YYYYMMDDHHMMSS::operator-(const YYYYMMDDHHMMSS & ymdhms2) const
{
  long hms1_sec = this->_hms.Hour()*60*60 + this->_hms.Minute()*60 + this->_hms.Second();
  long hms2_sec = ymdhms2._hms.Hour()*60*60 + ymdhms2._hms.Minute()*60 + ymdhms2._hms.Second();
  long lHMSDiff = hms2_sec - hms1_sec;

  long lDayDiff_sec = SDateTime::CalendarDayDiff(this->_ymd, ymdhms2._ymd) * 24*60*60;
  return -lDayDiff_sec - lHMSDiff;
}
long YYYYMMDDHHMMSS::operator-(const YMDHMS & ymdhms2) const
{
  long hms1_sec = this->_hms.Hour()*60*60 + this->_hms.Minute()*60 + this->_hms.Second();
  long hms2_sec = ymdhms2._hms.Hour()*60*60 + ymdhms2._hms.Minute()*60 + ymdhms2._hms.Second();
  long lHMSDiff = hms2_sec - hms1_sec;

  long lDayDiff_sec = SDateTime::CalendarDayDiff(this->_ymd, ymdhms2._ymd) * 24*60*60;
  return -lDayDiff_sec - lHMSDiff;
}
YYYYMMDDHHMMSS& YYYYMMDDHHMMSS::operator=(const YYYYMMDDHHMMSS& ymdhms)
{
  if(this == &ymdhms) return *this;

  _ymd.Set(ymdhms.GetYYYYMMDD());
  _hms.Set(ymdhms.GetHHMMSS());

  return *this;
}

const string YYYYMMDDHHMMSS::ToCashTimestampStr() const
{
  return GetYYYYMMDD().ToStr() + "_" + GetHHMMSS().ToStr() + "_000000";
}
const string YYYYMMDDHHMMSS::ToCashTimestampStrNoMillisec() const
{
  return GetYYYYMMDD().ToStr() + "_" + GetHHMMSS().ToStr();
}
const bool YYYYMMDDHHMMSS::IsValid() const
{
  return (GetYYYYMMDD().IsValid() && GetHHMMSS().IsValid());
}

void YYYYMMDDHHMMSS::Invalidate()
{
  _ymd.Set(YYYYMMDD::BAD_DATE);
  _hms.Set(HHMMSS::BAD_TIME);
}

const bool YMDHMS::IsValid() const
{
  return (_ymd.IsValid() && _hms.IsValid());
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------

bool SDateTime::IsLeapYear(int y)
{
  if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)) return true;
  else return false;    // not a leap year
}

// void SDateTime::JulianDayNum2YMD(JulianDayNum jdn, YMD & ymd)
// {
// 	if (jdn <= 0) throw std::exception();
//
// 	// Algorithm From Wikipedia
// 	int
// 		y = 4716,
// 			j = 1401,
// 			m = 2,
// 			n = 12,
// 			r = 4,
// 			p = 1461,
// 			v = 3,
// 			u = 5,
// 			s = 153,
// 			w = 2,
// 			B = 274277,
// 			C = -38;
//
// 	int f = jdn + j + (((4 * jdn + B) / 146097) * 3)/4 + C;
// 	int e = r * f + v;
// 	int g = (e % p) / r;
// 	int h = u * g + w;
//
// 	int iM = ((h/s + m) % n) + 1;
// 	int iY = e / p - y + (n + m - iM)/n;
// 	int iD = (h % s) / u + 1;
// 	ymd.SetYear(iY);
// 	ymd.SetMonth(iM);
// 	ymd.SetDay(iD);
// 	return;
//
// }

bool SDateTime::IsValidYMD(int year, int month, int day)
{
  if (year < 1900 ||
      year > 2100 ||
      month <= 0 ||
      month > 12 ||
      day <= 0)
    return false;

  if (month != 2 || !SDateTime::IsLeapYear(year))
  {
    if (day > DAYSINMTH[month-1]) return false;
  }
  else
  {
    if (day > DAYSINMTH[month-1]+1) return false;
  }

  return true;
}

GregorianDayNum SDateTime::CalcGregorianDayNum(int y, int m, int d)
{
  // Stack Overflow
  m = (m + 9) % 12;
  y = y - m/10;
  return 365*y + y/4 - y/100 + y/400 + (m*306 + 5)/10 + ( d - 1 );
}

JulianDayNum SDateTime::CalcJulianDayNum(int y, int m, int d)
{
  // Following the calculation method in Wikipedia
  JulianDayNum a = (14 - m)/12;
  JulianDayNum y1 = y+4800-a;
  JulianDayNum m1 = m+12*a-3;
  return d+(153*m1+2)/5+365*y1+y1/4-y1/100+y1/400-32045;
}


// Getting the no of days for the particular month
int SDateTime::DaysInMonth(int y,int m)
{
  return (m == 2 && IsLeapYear(y)) ? 29 : DAYSINMTH[m-1];
}

int SDateTime::DaysInYear(int year)
{
  return (IsLeapYear(year) ? 366 : 365);
}

std::string SDateTime::FormatPTimeYYYYMMDDHHMMSS(const boost::posix_time::ptime & pt)
{
  using namespace boost::posix_time;
  static std::locale loc(std::cout.getloc(), new time_facet("%Y%m%d_%H%M%s"));

  std::basic_stringstream<char> ss;
  ss.imbue(loc);
  ss << pt;
  string s = ss.str();
  boost::replace_all(s,".","_");
  return s;
}

std::string SDateTime::FormatPTimeYYYYMMDD(const boost::posix_time::ptime & pt)
{
  using namespace boost::posix_time;
  static std::locale loc(std::cout.getloc(), new time_facet("%Y%m%d"));

  std::basic_stringstream<char> ss;
  ss.imbue(loc);
  ss << pt;
  return ss.str();
}

std::string SDateTime::FormatPTimeHHMMSS(const boost::posix_time::ptime & pt)
{
  using namespace boost::posix_time;
  static std::locale loc(std::cout.getloc(), new time_facet("%H%M%S"));

  std::basic_stringstream<char> ss;
  ss.imbue(loc);
  ss << pt;
  return ss.str();
}

boost::shared_ptr<boost::posix_time::ptime> SDateTime::fromUnixTimeToPTime(const unsigned long ulUnixTime, TIMEPRECISION timePrecision)
{
  return SDateTime::fromUnixTimeToPTime(ulUnixTime, timePrecision, SDateTime::GMT, SDateTime::GMT);
}
boost::shared_ptr<boost::posix_time::ptime> SDateTime::fromUnixTimeToPTime(const unsigned long ulUnixTime, TIMEPRECISION timePrecision, TIMEZONE tzSrc, TIMEZONE tzDest)
{
  if (!SDateTime::EpochDate) SDateTime::EpochDate.reset(new boost::gregorian::date(1970,1,1));

  unsigned long ulAdjSeconds = 0;
  if      (tzSrc == HKT && tzDest == GMT) ulAdjSeconds -= 8*60*60;
  else if (tzSrc == GMT && tzDest == HKT) ulAdjSeconds += 8*60*60;

  switch(timePrecision)
  {
    case NANOSEC:  { return boost::shared_ptr<boost::posix_time::ptime>(new boost::posix_time::ptime(*SDateTime::EpochDate, boost::posix_time::microsec(ulUnixTime/1000     + ulAdjSeconds*1000000000))); break; }
    case MICROSEC: { return boost::shared_ptr<boost::posix_time::ptime>(new boost::posix_time::ptime(*SDateTime::EpochDate, boost::posix_time::microsec(ulUnixTime          + ulAdjSeconds*1000000   ))); break; }
    case MILLISEC: { return boost::shared_ptr<boost::posix_time::ptime>(new boost::posix_time::ptime(*SDateTime::EpochDate, boost::posix_time::millisec(ulUnixTime          + ulAdjSeconds*1000      ))); break; }
    case SECOND:   { return boost::shared_ptr<boost::posix_time::ptime>(new boost::posix_time::ptime(*SDateTime::EpochDate, boost::posix_time::seconds (ulUnixTime          + ulAdjSeconds           ))); break; }
    case DAY:      { return boost::shared_ptr<boost::posix_time::ptime>(new boost::posix_time::ptime(*SDateTime::EpochDate, boost::posix_time::seconds (ulUnixTime*24*60*60 + ulAdjSeconds           ))); break; }
    default:       {                                                                                                                                                                                      break; }
  }
}

string SDateTime::fromUnixTimeToString(const unsigned long ulUnixTime, TIMEPRECISION timePrecision)
{
  return SDateTime::fromUnixTimeToString(ulUnixTime, timePrecision, SDateTime::GMT, SDateTime::GMT);
}

string SDateTime::fromUnixTimeToString(const unsigned long ulUnixTime, TIMEPRECISION timePrecision, TIMEZONE tzSrc, TIMEZONE tzDest)
{
  boost::shared_ptr<boost::posix_time::ptime> p_pt = SDateTime::fromUnixTimeToPTime(ulUnixTime, timePrecision, tzSrc, tzDest);
  if (!p_pt) return "";
  return FormatPTimeYYYYMMDDHHMMSS(*p_pt);
}

unsigned long SDateTime::fromStringToUnixTime(const string & yyyymmdd_hhmmss_ffffff, TIMEPRECISION timePrecision)
{
  vector<string> vs;
  boost::split(vs, yyyymmdd_hhmmss_ffffff, boost::is_any_of("_"));
  if (vs.size() != 3) throw std::exception();
  //--------------------------------------------------
  const int ymd      = boost::lexical_cast<int>(vs[0]);
  const int hms      = boost::lexical_cast<int>(vs[1]);
  const int microsec = boost::lexical_cast<int>(vs[2]);
  const int y        = ymd / 10000;
  const int m        = (ymd % 10000) / 100;
  const int d        = ymd % 100;
  const int H        = hms / 10000;
  const int M        = (hms % 10000) / 100;
  const int S        = hms % 100;
  //--------------------------------------------------
  tm t;
  t.tm_year = y - 1900; // years since 1900
  t.tm_mon = m - 1; // 0-11
  t.tm_mday = d; // day of the month
  t.tm_hour = H;
  t.tm_min = M;
  t.tm_sec = S;
  t.tm_isdst = 0;

  const long sec_since_epoch = timegm(&t);

  long mul = 1;
  long frac_sec = 0;
  switch(timePrecision)
  {
    case NANOSEC:  { mul = 1000000000; frac_sec = microsec * 1000; return mul * sec_since_epoch + frac_sec; break; }
    case MICROSEC: { mul = 1000000   ; frac_sec = microsec       ; return mul * sec_since_epoch + frac_sec; break; }
    case MILLISEC: { mul = 1000      ; frac_sec = microsec / 1000; return mul * sec_since_epoch + frac_sec; break; }
    case SECOND:   { mul = 1         ; frac_sec = 0              ; return mul * sec_since_epoch + frac_sec; break; }
    case DAY:      {                                               return       sec_since_epoch / 86400   ; break; }
    default:       { mul = 1         ; frac_sec = 0              ; return mul * sec_since_epoch + frac_sec; break; }
  }

  return mul * sec_since_epoch + frac_sec;
}

//--------------------------------------------------
// For MsgType 304 ExpirationDate
//--------------------------------------------------
string SDateTime::fromGeniumDateToString(unsigned int expdate)
{
  unsigned int a=expdate;
  unsigned int siza=(sizeof(a)-2)*8;
  unsigned int month=(a<<(7+siza))>>(7+siza+5) ;
  unsigned int day=(a<<(7+siza+4))>>(7+siza+4) ;
  unsigned int year= (a>>9)-1+1990;
  char tmp[100];
  sprintf(tmp,"%d-%02d-%02d",year,month,day);
  return string(tmp);
}

string SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_000000()
{
  time_t t = time(0);   // get time now
  struct tm * now = localtime( & t );
  ostringstream coo;
  coo << (now->tm_year + 1900);
  coo << std::setw(2) << std::setfill('0') << (now->tm_mon + 1);
  coo << std::setw(2) << std::setfill('0') << now->tm_mday;
  coo << '_';
  coo << std::setw(2) << std::setfill('0') << (now->tm_hour);
  coo << std::setw(2) << std::setfill('0') << now->tm_min;
  coo << std::setw(2) << std::setfill('0') << now->tm_sec;
  coo << "_000000";
  //--------------------------------------------------
  return coo.str();
}


string SDateTime::GetCurrentTimeYYYYMMDD_HHMMSS_ffffff()
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  struct tm *tm = localtime(&tp.tv_sec);
  char sYYYYMMDD_HHMMSS[64];
  strftime(sYYYYMMDD_HHMMSS, sizeof(sYYYYMMDD_HHMMSS), "%Y%m%d_%H%M%S", tm);
  char sMicrosec[16];
  sprintf(sMicrosec,"_%06d",tp.tv_usec);
  strcat(sYYYYMMDD_HHMMSS,sMicrosec);
  return string(sYYYYMMDD_HHMMSS);
}



YYYYMMDDHHMMSS SDateTime::ChangeTimeZone(const YYYYMMDDHHMMSS & ymdhmsFrom, const TIMEZONE fromTZ, const TIMEZONE toTZ)
{
  YMD ymd(ymdhmsFrom.GetYYYYMMDD());
  HMS hms(ymdhmsFrom.GetHHMMSS());

  //--------------------------------------------------
  // Change to HKT first
  //--------------------------------------------------
  switch(fromTZ)
  {
    case GMT:
      {
        int iCarry = hms.AddMinute(8*60);
        if      (iCarry > 0) ymd.AddDay(iCarry);
        else if (iCarry < 0) ymd.SubtractDay(iCarry);
        break;
      }
    case EST:
      {
        int iCarry = hms.AddMinute(12*60);
        if      (iCarry > 0) ymd.AddDay(iCarry);
        else if (iCarry < 0) ymd.SubtractDay(iCarry);
        break;
      }
    case CST:
      {
        int iCarry = hms.AddMinute(13*60);
        if      (iCarry > 0) ymd.AddDay(iCarry);
        else if (iCarry < 0) ymd.SubtractDay(iCarry);
        break;
      }
  }

  //--------------------------------------------------
  // Change to target TIMEZONE
  //--------------------------------------------------
  switch(toTZ)
  {
    case GMT:
      {
        int iCarry = hms.SubtractMinute(8*60);
        if      (iCarry > 0) ymd.SubtractDay(iCarry);
        else if (iCarry < 0) ymd.AddDay(iCarry);
        break;
      }
    case EST:
      {
        int iCarry = hms.SubtractMinute(12*60);
        if      (iCarry > 0) ymd.SubtractDay(iCarry);
        else if (iCarry < 0) ymd.AddDay(iCarry);
        break;
      }
    case CST:
      {
        int iCarry = hms.SubtractMinute(13*60);
        if      (iCarry > 0) ymd.SubtractDay(iCarry);
        else if (iCarry < 0) ymd.AddDay(iCarry);
        break;
      }
  }

  return YYYYMMDDHHMMSS(ymd,hms);
}

unsigned long SDateTime::GetCurrentUnixTimeInMillisecGMT()
{
  return GetCurrentUnixTimeInMicrosecGMT(TIMEZONE::GMT) / 1000;
}
unsigned long SDateTime::GetCurrentUnixTimeInMillisecGMT(const TIMEZONE tz)
{
  return GetCurrentUnixTimeInMicrosecGMT(tz) / 1000;
}
unsigned long SDateTime::GetCurrentUnixTimeInMicrosecGMT()
{
  return GetCurrentUnixTimeInMicrosecGMT(TIMEZONE::GMT);
}
unsigned long SDateTime::GetCurrentUnixTimeInMicrosecGMT(const TIMEZONE tz)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  long lExtraOffsetSec = 0;
  switch(tz)
  {
    case HKT: { lExtraOffsetSec = 8*60*60; break; }
    default:  {                            break; }
  }
  return (tp.tv_sec + lExtraOffsetSec) * 1000000 + tp.tv_usec;
}
