#include <CME.h>

const double CME::_TRDG_DAY_PER_YR = 252;

boost::weak_ptr<CME> CME::m_pInstance;

boost::shared_ptr<CME> CME::Instance() {
  boost::shared_ptr<CME> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new CME());
    m_pInstance = instance;
  }
  return instance;
}

CME::CME()
{
}

CME::~CME()
{
}


bool CME::IsAProductInThisMarket(const string & symbol)
{
  if (symbol.length() <= 2) return false;

  string sSubStr = symbol.substr(0,2);

  if (
      sSubStr == UNDERLYING_ES ||
      sSubStr == UNDERLYING_NQ ||
      sSubStr == UNDERLYING_YM ||
      sSubStr == UNDERLYING_TF) return true;
  else return false;
}

double CME::GetContractMultiplier(const string & symbol)
{
  if (symbol.length() <= 2) return NAN;

  string sSubStr = symbol.substr(0,2);

  if      (sSubStr == UNDERLYING_ES) return 50;
  else if (sSubStr == UNDERLYING_NQ) return 20;
  else if (sSubStr == UNDERLYING_YM) return 5;
  else if (sSubStr == UNDERLYING_TF) return 100;
  else                               return NAN;
}

string CME::GetCtrtCode1(const ContractType ct, const YYYYMMDD & ymd, const int fm)
{
  string sTmp = GetCtrtCode2(ct,ymd,fm);
  if (sTmp.length() != 3) return "";
  ostringstream oo;
  oo << sTmp.at(0);
  oo << sTmp.at(2);
  return oo.str();
}

string CME::GetCtrtCode2(const ContractType, const YYYYMMDD & ymd, const int fm)
{
  if (fm > 2) return "";

  string sCurYearCode  = boost::lexical_cast<string>(ymd.Year()%100);
  string sNextYearCode = boost::lexical_cast<string>((ymd.Year()+1)%100);

  //--------------------------------------------------
  // Easy months
  //--------------------------------------------------
  switch(ymd.Month())
  {
    case 1:
    case 2:
      if      (fm == 1) return "H" + sCurYearCode;
      else if (fm == 2) return "M" + sCurYearCode;
    case 4:
    case 5:
      if      (fm == 1) return "M" + sCurYearCode;
      else if (fm == 2) return "U" + sCurYearCode;
    case 7:
    case 8:
      if      (fm == 1) return "U" + sCurYearCode;
      else if (fm == 2) return "Z" + sCurYearCode;
    case 10:
    case 11:
      if      (fm == 1) return "Z" + sCurYearCode;
      else if (fm == 2) return "H" + sNextYearCode;
  }

  //--------------------------------------------------
  // Roll months
  //--------------------------------------------------
  bool bRoll = false;
  if (ymd.Day() >= CalcRollDay(ymd)) bRoll = true;


  if (!bRoll) 
  {
    switch(ymd.Month())
    {
      case 3:
        if      (fm == 1) return "H" + sCurYearCode;
        else if (fm == 2) return "M" + sCurYearCode;
      case 6:
        if      (fm == 1) return "M" + sCurYearCode;
        else if (fm == 2) return "U" + sCurYearCode;
      case 9:
        if      (fm == 1) return "U" + sCurYearCode;
        else if (fm == 2) return "Z" + sCurYearCode;
      case 12:
        if      (fm == 1) return "Z" + sCurYearCode;
        else if (fm == 2) return "H" + sNextYearCode;
    }
  }
  else
  {
    switch(ymd.Month())
    {
      case 3:
        if      (fm == 1) return "M" + sCurYearCode;
        else if (fm == 2) return "U" + sCurYearCode;
      case 6:
        if      (fm == 1) return "U" + sCurYearCode;
        else if (fm == 2) return "Z" + sCurYearCode;
      case 9:
        if      (fm == 1) return "Z" + sCurYearCode;
        else if (fm == 2) return "H" + sNextYearCode;
      case 12:
        if      (fm == 1) return "H" + sNextYearCode;
        else if (fm == 2) return "M" + sNextYearCode;
    }
  }

  return "";
}



int CME::CalcRollDay(const YYYYMMDD & ymd)
{
  int iRollDay = -1;

  YMD ymd_1(ymd.Year(),ymd.Month(),1);
  int iDoW1 = SDateTime::DayOfWeek(ymd_1);

  iRollDay = 7-iDoW1+5;
  return iRollDay;
}

bool CME::IsRollDate(const YYYYMMDD & ymd)
{

  switch(ymd.Month())
  {
    case  1: return false;
    case  2: return false;
    case  4: return false;
    case  5: return false;
    case  7: return false;
    case  8: return false;
    case 10: return false;
    case 11: return false;
  }

  if (ymd.Day() == CalcRollDay(ymd)) return true;
  return false;
}

