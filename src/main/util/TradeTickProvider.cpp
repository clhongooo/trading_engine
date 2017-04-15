#include "TradeTickProvider.h"

using namespace std;

const string TradeTickProvider::_sSSE = "SSE";

TradeTickProvider::TradeTickProvider() : _iPsn(-1),_iNumOfBars(-1),
  _viYMD(NULL),_viTime(NULL),_vdPrice(NULL),_vlVolume(NULL),_viInitiator(NULL)
{}

TradeTickProvider::TradeTickProvider(
    const char *ccPath,
    const string &sFormat, 
    char cHeader,
    char cCompressionType)
  :
    _viYMD(NULL),
    _viTime(NULL),
    _vdPrice(NULL),
    _vlVolume(NULL),
    _viInitiator(NULL)
{
  LoadDataFile(
      ccPath,
      sFormat,
      cHeader,
      cCompressionType);
}

void TradeTickProvider::FreeVec() {
    if (_viYMD != NULL)
    {
      _viYMD->clear();
      delete _viYMD; _viYMD = NULL;
    }
    if (_viTime != NULL)
    {
      _viTime->clear();
      delete _viTime; _viTime = NULL;
    }
    if (_vdPrice != NULL)   { delete _vdPrice;  _vdPrice = NULL; }
    if (_vlVolume != NULL)  { delete _vlVolume; _vlVolume = NULL; }
    if (_viInitiator != NULL)    { delete _viInitiator;   _viInitiator = NULL; }
}

TradeTickProvider::~TradeTickProvider() {
  FreeVec();
}

void TradeTickProvider::LoadDataFile(
    const char *ccPath,
    const string &sFormat,
    char cHeader,
    char cCompressionType)
{
  int iReserveSize = 30000;

  if (_viYMD          == NULL) { _viYMD          = new vector<YYYYMMDD>(); _viYMD->reserve(iReserveSize);  }
  if (_viTime         == NULL) { _viTime         = new vector<HHMMSS>();   _viTime->reserve(iReserveSize);   }
  if (_vdPrice        == NULL) { _vdPrice        = new vector<double>();   _vdPrice->reserve(iReserveSize);   }
  if (_vlVolume       == NULL) { _vlVolume       = new vector<long>();     _vlVolume->reserve(iReserveSize);   }
  if (_viInitiator    == NULL) { _viInitiator    = new vector<int>();      _viInitiator->reserve(iReserveSize);   }

  if (
      (sFormat.compare(TradeTickProvider::_sSSE) != 0) ||
      (cCompressionType != 'G' && cCompressionType != 'P') ||
      (cHeader != 'H' && cHeader != 'N'))
  {
    FreeVec();
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }

  ifstream ifs(ccPath, std::ios_base::in | std::ios_base::binary);

  try {
    boost::iostreams::filtering_istream fis;


    if (cCompressionType == 'G') fis.push(boost::iostreams::gzip_decompressor());
    fis.push(ifs);

    int iLineNum = 0;


    for (string str; getline(fis, str); )
    {
      ++iLineNum;
      if (cHeader == 'H' && iLineNum == 1) continue; // Skip header row

      string sDate="";
      string sTime="";
      string sTmp="";
      string sInitr="";
      string sPrice="";
      string sVolume="";
      Tokenizer tknzr(str, ",");

      // SSE Sample: "28",20130315,27,92501000,NA,"S",177.52,400
      if (sFormat.compare(TradeTickProvider::_sSSE) == 0)
      {
        if (tknzr.NextToken()) sTmp    = tknzr.GetToken();
        if (tknzr.NextToken()) sDate   = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp    = tknzr.GetToken();
        if (tknzr.NextToken()) sTime   = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp    = tknzr.GetToken();
        if (tknzr.NextToken()) sInitr  = tknzr.GetToken();
        if (tknzr.NextToken()) sPrice  = tknzr.GetToken();
        if (tknzr.NextToken()) sVolume = tknzr.GetToken();

        _viYMD->push_back(YYYYMMDD(lexical_cast<int>(sDate)));
        _viTime->push_back(HHMMSS(lexical_cast<int>(sTime) / 1000));
        _vdPrice->push_back(lexical_cast<double>(sPrice));
        _vlVolume->push_back(lexical_cast<long>(sVolume));
        if      (sInitr[1] == 'B') _viInitiator->push_back(1);
        else if (sInitr[1] == 'S') _viInitiator->push_back(-1);
        else                       _viInitiator->push_back(0);
      }

    }

    if (_viYMD->size() != _viTime->size() ||
        _viYMD->size() != _vdPrice->size() ||
        _viYMD->size() != _vlVolume->size() ||
        _viYMD->size() != _viInitiator->size())
    {
      FreeVec();
      SException se;
      se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
      throw se;
    }
    else _iNumOfBars = _viYMD->size();

    _iPsn = -1;

    if (iLineNum == 0)
    {
      FreeVec();
      SException se;
      se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
      throw se;
    }
  }
  catch(const boost::iostreams::gzip_error& e)
  {
    cout << e.what() << endl;
    {
      FreeVec();
      SException se;
      se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
      throw se;
    }
  }

}

void TradeTickProvider::ResetPsn()
{
  _iPsn = -1;
}

int TradeTickProvider::GetNumOfBars() const
{
  return _iNumOfBars;
}

bool TradeTickProvider::HasNextBar() const
{
  if (_iPsn < _iNumOfBars-1) return true;
  else return false;
}

void TradeTickProvider::MoveNext()
{
  ++_iPsn;

  if (_iPsn >= _iNumOfBars)
  {
    FreeVec();
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}

//returns -1 if there is no more bars
const YYYYMMDD TradeTickProvider::PeekNextYMDi() const
{
  if (HasNextBar())
  {
    return ((*_viYMD)[_iPsn+1]);
  }
  else return YYYYMMDD::BAD_DATE;
}

const YYYYMMDD TradeTickProvider::PeekPrevYMDi()
{
  if (_iPsn > 0) return (*_viYMD)[_iPsn-1];
  else
  {
    FreeVec();
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}

const double * TradeTickProvider::GetPriceArr(int iLen)
{
  if (iLen > 0)
  {
    if (_iPsn >= iLen-1)
      return &((*_vdPrice)[_iPsn-iLen+1]);
  }
  else
  {
    FreeVec();
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
  return &((*_vdPrice)[0]); // impossible path, just to sooth compiler
}


const YYYYMMDD & TradeTickProvider::GetYYYYMMDD() const { return (*_viYMD)[_iPsn]; }
const HHMMSS & TradeTickProvider::GetHHMMSS() const { return (*_viTime)[_iPsn]; }
const double TradeTickProvider::Price() const { return (*_vdPrice)[_iPsn]; }
const long TradeTickProvider::Volume() const { return (*_vlVolume)[_iPsn]; }
const int TradeTickProvider::Initiator() const { return (*_viInitiator)[_iPsn]; }


