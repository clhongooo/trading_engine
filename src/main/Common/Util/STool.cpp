#include "STool.h"


//returns the larger
double STool::Max(double dA, double dB)
{
  if (IsNaN(dA) || IsNaN(dB))
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
  return (dA > dB ? dA : dB);
}
//returns the smaller
double STool::Min(double dA, double dB)
{
  if (IsNaN(dA) || IsNaN(dB))
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
  return (dA < dB ? dA : dB);
}
//returns the larger
int STool::Max(int iA, int iB)
{
  return (iA > iB ? iA : iB);
}
//returns the largest
int STool::Max(int iA, int iB, int iC)
{
  return Max(Max(iA,iB),iC);
}
//returns the smaller
int STool::Min(int iA, int iB)
{
  return (iA < iB ? iA : iB);
}
//returns the smallest
int STool::Min(int iA, int iB, int iC)
{
  return Min(Min(iA,iB),iC);
}
//returns an int closer to zero
double STool::IntPart1(double d)
{
  if (d < 0.0) return ceil(d); else return floor(d);
}
//returns an int farther from zero
double STool::IntPart2(double d)
{
  if (d < 0.0) return floor(d); else return ceil(d);
}

double STool::Round(double d, unsigned u)
{
  bool bPve = true; if (d < 0) bPve = false; d = abs(d);
  double dFac = 1;
  if (u > 0) dFac = pow(10,u); else dFac = 1;
  return floor((dFac*d) + 0.5) / dFac * (bPve?1:-1);
}

//no abs() for int
int STool::Abs(int iA)
{
  if (iA >= 0) return iA;
  else return -iA;
}


//round the double to the closest interval
double STool::RoundIntvl(double d, double dintvl)
{ return Round(d/dintvl,0)*dintvl;
}

string STool::DecToBin(int iDec)
{
  if (iDec == 0) return "0";
  if (iDec == 1) return "1";

  if (iDec%2 == 0) return DecToBin(iDec/2)+"0";
  else return DecToBin(iDec/2)+"1";
}
int STool::BinToDec(const string &sBin)
{
  int iDec = 0;
  int pow = 1;
  for (int i = sBin.length()-1; i >= 0; --i, pow <<= 1)
    iDec += (sBin[i] - '0') * pow;
  return iDec;
}


//std::to_string() is missing
//boost::lexical_cast<string> requires the boost library
string STool::ToStr(int i)
{
  stringstream ss;
  ss << i;
  return ss.str();
}
string STool::CharToStr(char c)
{
  string sR("");
  sR += c;
  return sR;
}
string STool::ToStr(double d, unsigned u)
{
  stringstream ss;
  ss << Round(d,u);
  return ss.str();
}
double STool::ToDbl(const string &s)
{
  // can't distinguish error from conversion of 0
  //return (double)atof(s.c_str());
  // an alternative way
  //double num = std::strtod(nptr, NULL);

  istringstream ss(s);
  double d;
  if (!(ss >> d))
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
  return d;
}
double STool::ToDbl(const string &s, unsigned u)
{
  return Round(ToDbl(s),u);
  return 0; //stub
}
// int STool::ToInt(const string &s)
// {
//   return IntPart1(ToDbl(s));
//   return 0; //stub
// }

//PadLeft, PadRight
string STool::PadLeft(unsigned iOri, char cPad, unsigned iLen)
{
  return PadLeft(ToStr(iOri), cPad, iLen);
}
string STool::PadLeft(const char *sOri, char cPad, unsigned iLen)
{
  string sTmp(sOri);
  return PadLeft(sTmp,cPad,iLen);
}
string STool::PadLeftS(string sOri, char cPad, unsigned iLen)
{
  return PadLeft(sOri,cPad,iLen);
}
string STool::PadLeft(const string &sOri, char cPad, unsigned iLen)
{
  if (iLen < 0 || sOri.length() >= iLen) return sOri;
  else
  {
    string sPad; sPad = cPad;
    string sR(sOri);
    while (sR.length() < iLen) sR = sPad + sR;
    return sR;
  }
}

string STool::PadRight(unsigned iOri, char cPad, unsigned iLen)
{
  return PadRight(ToStr(iOri), cPad, iLen);
}
string STool::PadRight(const char *sOri, char cPad, unsigned iLen)
{
  string sTmp(sOri);
  return PadRight(sTmp,cPad,iLen);
}
string STool::PadRightS(string sOri, char cPad, unsigned iLen)
{
  return PadRight(sOri,cPad,iLen);
}
string STool::PadRight(const string &sOri, char cPad, unsigned iLen)
{
  if (iLen < 0 || sOri.length() >= iLen) return sOri;
  else
  {
    string sPad; sPad = cPad;
    string sR(sOri);
    while (sR.length() < iLen) sR = sR + sPad;
    return sR;
  }
}

// string& STool::TrimInPlace(string &s)      { boost::algorithm::trim(s);       return s;}
// string& STool::TrimLeftInPlace(string &s)  { boost::algorithm::trim_left(s);  return s;}
// string& STool::TrimRightInPlace(string &s) { boost::algorithm::trim_right(s); return s;}

string& STool::TrimLeftInPlace(string& s, string sTrim)
{
  boost::trim_left_if(s, boost::is_any_of(sTrim) );
  return s;
}

string STool::TrimLeft(const string & s, const string sTrim)
{
  string s1 = s;
  return TrimLeftInPlace(s1,sTrim);
}

string& STool::TrimInPlace(string& s)
{
  return TrimLeftInPlace(TrimRightInPlace(s));
}

string STool::Trim(const string & s)
{
  string s1 = s;
  return TrimInPlace(s1);
}

string& STool::TrimLeftInPlace(string& s)
{
  s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
  return s;
}

string STool::TrimLeft(const string & s)
{
  string s1 = s;
  return TrimLeftInPlace(s1);
}

string& STool::TrimRightInPlace(string& s)
{
  s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
  return s;
}

string STool::TrimRight(const string & s)
{
  string s1 = s;
  return TrimRightInPlace(s1);
}

//Construct String
string STool::ConstructStr(const string *s, int iSize, const string &sDelim)
{
  string sR = "";
  //int iSize = sizeof(s) / sizeof(s[0]);

  if (iSize == 0) return sR;
  else
  {
    for (int i = 0; i < iSize; ++i)
      if (i < iSize - 1) sR += s[i] + sDelim;
      else sR += s[i];
    return sR;
  }
}

//NaN
bool STool::IsNaN(double value)
{
  return value != value;
}

//encapsulation for reading file
void STool::ReadFile(const string & sPath, deque<string> & dqS)
{
  ReadFile(sPath.c_str(),dqS);
}
void STool::ReadFile(const char *sPath, deque<string> & dqS)
{
  ifstream ifs(sPath, ios::in);
  if (ifs.is_open())
  {
    for (string str; getline(ifs,str); ) dqS.push_back(str);
  }
  else
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
  ifs.close();
}

void STool::ReadCSVFile(const string & sPath, deque<vector<string> > & dqvS)
{
  ReadCSVFile(sPath.c_str(),dqvS);
}

void STool::ReadCSVFile(const char *sPath, deque<vector<string> > & dqvS)
{
  dqvS.clear();

  deque<string> dqS;
  ReadFile(sPath,dqS);

  for (deque<string>::iterator it = dqS.begin(); it != dqS.end(); ++it)
  {
    const string & s = *it;
    vector<string> vs;
    boost::split(vs, s, boost::is_any_of(","));

    dqvS.push_back(vs);
  }
}


bool STool::IsStringAllLetters(const string & s)
{
  if (s.length() == 0) return true;

  bool bAllLetters = true;
  for (unsigned int i = 0; i < s.length(); ++i)
  {
    if (!((s.at(i) <= 'z' && s.at(i) >= 'a') || (s.at(i) <= 'Z' && s.at(i) >= 'A')))
      bAllLetters = false;
  }
  return bAllLetters;
}


double STool::KellyCriterion(const double winProb, const double netOdds)
{
  return (winProb * (netOdds + 1) - 1) / netOdds;
}

bool STool::ChkIfFileExists(const string & sFile)
{
  return ChkIfFileExists(sFile.c_str());
}

bool STool::ChkIfFileExists(const char *sFile)
{
  if (FILE *file = fopen(sFile,"r"))
  {
    fclose(file);
    return true;
  }
  else
  {
    return false;
  }
}

bool STool::IsValidPriceOrVol(const double d)
{
  if (std::isnan(d)         ) return false;
  if (d >= ATU_INVALID_PRICE) return false;
  if (d <= NIR_EPSILON      ) return false;
  return true;
}

string STool::ToUpperCase(string & s)
{
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);
  return s;
}

string STool::ToLowerCase(string & s)
{
  std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  return s;
}

bool STool::IsNumber(const string & s)
{
  try 
  {
    double x = boost::lexical_cast<double>(s);
    return true;
  }
  catch(boost::bad_lexical_cast e) 
  {
    return false;
  }
  return true;
}


string STool::GetNthItemFromCSV(const string & s, const int n_th)
{
  size_t iStrLen = s.length();
  int iNthComma = 0;
  int iLastLoc = 0;
  int iLocUpTo = 0;

  std::size_t found = s.find(',',iLocUpTo);
  while (
    found != std::string::npos
    &&
    iLocUpTo < iStrLen
    &&
    iNthComma < n_th
    )
  {
    iLastLoc = iLocUpTo;
    iLocUpTo = found + 1;
    iNthComma++;
    found = s.find(',',iLocUpTo);
  }

  return s.substr(iLastLoc,iLocUpTo-iLastLoc-1);
}


double STool::Sign(const double a)
{
  if (a < 0) return -1;
  else       return 1;
}

