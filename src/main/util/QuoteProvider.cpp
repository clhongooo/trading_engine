#include "QuoteProvider.h"

using namespace std;

const string QuoteProvider::_sSSE = "SSE";

QuoteProvider::QuoteProvider() :
  _iPsn(-1),
  _iNumOfBars(-1),
  _viYMD(NULL),
  _viTime(NULL),
  _vdPrice(NULL),
  _vlVolume(NULL),
  _viInitiator(NULL),
  _vdBestBidPrice(NULL),
  _vlBestBidSize(NULL),
  _vdBestAskPrice(NULL),
  _vlBestAskSize(NULL) {}

QuoteProvider::QuoteProvider(
    const char *ccPath,
    const string &sFormat,
    char cHeader,
    char cCompressionType)
  :
    _viYMD(NULL),
    _viTime(NULL),
    _vdPrice(NULL),
    _vlVolume(NULL),
    _viInitiator(NULL),
    _vdBestBidPrice(NULL),
    _vlBestBidSize(NULL),
    _vdBestAskPrice(NULL),
    _vlBestAskSize(NULL)
{
  LoadDataFile(
      ccPath,
      sFormat,
      cHeader,
      cCompressionType);
}

void QuoteProvider::FreeVec() {
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
  if (_vdPrice != NULL)               { delete _vdPrice;  _vdPrice = NULL;              }
  if (_vlVolume != NULL)              { delete _vlVolume; _vlVolume = NULL;             }
  if (_viInitiator != NULL)           { delete _viInitiator; _viInitiator = NULL;       }
  if (_vdBestBidPrice != NULL)        { delete _vdBestBidPrice;_vdBestBidPrice = NULL;  }
  if (_vlBestBidSize != NULL)         { delete _vlBestBidSize;_vlBestBidSize = NULL;    }
  if (_vdBestAskPrice != NULL)        { delete _vdBestAskPrice;_vdBestAskPrice = NULL;  }
  if (_vlBestAskSize != NULL)         { delete _vlBestAskSize;_vlBestAskSize = NULL;    }
}

QuoteProvider::~QuoteProvider() {
  FreeVec();
}

void QuoteProvider::LoadDataFile(
    const char *ccPath,
    const string &sFormat,
    char cHeader,
    char cCompressionType)
{
  int iReserveSize = 30000;

  if (_viYMD               == NULL) { _viYMD                  = new vector<YYYYMMDD>(); _viYMD->reserve(iReserveSize);                 }
  if (_viTime              == NULL) { _viTime                 = new vector<HHMMSS>();   _viTime->reserve(iReserveSize);                }
  if (_vdPrice             == NULL) { _vdPrice                = new vector<double>();   _vdPrice->reserve(iReserveSize);               }
  if (_vlVolume            == NULL) { _vlVolume               = new vector<long>();     _vlVolume->reserve(iReserveSize);              }
  if (_viInitiator         == NULL) { _viInitiator            = new vector<int>();      _viInitiator->reserve(iReserveSize);           }
  if (_vdBestBidPrice      == NULL) { _vdBestBidPrice         = new vector<double>();   _vdBestBidPrice->reserve(iReserveSize);        }
  if (_vlBestBidSize       == NULL) { _vlBestBidSize          = new vector<long>();     _vlBestBidSize->reserve(iReserveSize);         }
  if (_vdBestAskPrice      == NULL) { _vdBestAskPrice         = new vector<double>();   _vdBestAskPrice->reserve(iReserveSize);        }
  if (_vlBestAskSize       == NULL) { _vlBestAskSize          = new vector<long>();     _vlBestAskSize->reserve(iReserveSize);         }

  if (
      (sFormat.compare(QuoteProvider::_sSSE) != 0) ||
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
      string sBestBidPrice="";
      string sBestBidSize="";
      string sBestAskPrice="";
      string sBestAskSize="";
      Tokenizer tknzr(str, ",");

      // SSEQ Sample:
      // "","date","time","price","volume","turnover","ntrade","BS","acc_volume","acc_turnover",
      // "AskPrice1","AskPrice2","AskPrice3","AskPrice4","AskPrice5","AskPrice6","AskPrice7","AskPrice8","AskPrice9","AskPrice10",
      // "AskVolume1","AskVolume2","AskVolume3","AskVolume4","AskVolume5","AskVolume6","AskVolume7","AskVolume8","AskVolume9","AskVolume10",
      // "BidPrice1","BidPrice2","BidPrice3","BidPrice4","BidPrice5","BidPrice6","BidPrice7","BidPrice8","BidPrice9","BidPrice10",
      // "BidVolume1","BidVolume2","BidVolume3","BidVolume4","BidVolume5","BidVolume6","BidVolume7","BidVolume8","BidVolume9","BidVolume10"

      // "1999",20130814,111741,181.19,200,36238,4679,"B",
      // 1404871,254915201,
      // 181.19,181.2,181.29,181.3,181.38,181.39,181.4,181.42,181.45,181.47,1958,306,19,600,50,4300,3000,2000,1210,600,181.06,181.03,181.02,181.01,181,180.9,180.88,180.85,180.77,180.71,2242,1294,1003,7693,11700,200,8600,100,2000,2000
      if (sFormat.compare(QuoteProvider::_sSSE) == 0)
      {
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sDate          = tknzr.GetToken();
        if (tknzr.NextToken()) sTime          = tknzr.GetToken();
        if (tknzr.NextToken()) sPrice         = tknzr.GetToken();
        if (tknzr.NextToken()) sVolume        = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sInitr         = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sBestAskPrice  = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sBestAskSize   = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sBestBidPrice  = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sBestBidSize   = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();
        if (tknzr.NextToken()) sTmp           = tknzr.GetToken();

        _viYMD->push_back(YYYYMMDD(lexical_cast<int>(sDate)));
        _viTime->push_back(HHMMSS(lexical_cast<int>(sTime)));
        _vdPrice->push_back(lexical_cast<double>(sPrice));
        _vlVolume->push_back(lexical_cast<long>(sVolume));
        if      (sInitr[1] == 'B') _viInitiator->push_back(1);
        else if (sInitr[1] == 'S') _viInitiator->push_back(-1);
        else                       _viInitiator->push_back(0);
        _vdBestBidPrice->push_back(lexical_cast<double>(sBestBidPrice));
        _vlBestBidSize->push_back(lexical_cast<long>(sBestBidSize));
        _vdBestAskPrice->push_back(lexical_cast<double>(sBestAskPrice));
        _vlBestAskSize->push_back(lexical_cast<long>(sBestAskSize));
      }

    }

    if (_viYMD->size() != _viTime->size() ||
        _viYMD->size() != _vdPrice->size() ||
        _viYMD->size() != _vlVolume->size() ||
        _viYMD->size() != _viInitiator->size() ||
        _viYMD->size() != _vdBestBidPrice->size() ||
        _viYMD->size() != _vlBestBidSize->size() ||
        _viYMD->size() != _vdBestAskPrice->size() ||
        _viYMD->size() != _vlBestAskSize->size())
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

void QuoteProvider::ResetPsn()
{
  _iPsn = -1;
}

int QuoteProvider::GetNumOfBars() const
{
  return _iNumOfBars;
}

bool QuoteProvider::HasNextBar() const
{
  if (_iPsn < _iNumOfBars-1) return true;
  else return false;
}

void QuoteProvider::MoveNext()
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
const YYYYMMDD QuoteProvider::PeekNextYMDi() const
{
  if (HasNextBar())
  {
    return ((*_viYMD)[_iPsn+1]);
  }
  else return YYYYMMDD::BAD_DATE;
}

const YYYYMMDD QuoteProvider::PeekPrevYMDi()
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

const double * QuoteProvider::GetPriceArr(int iLen)
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


const YYYYMMDD & QuoteProvider::GetYYYYMMDD() const { return (*_viYMD)[_iPsn]; }
const HHMMSS & QuoteProvider::GetHHMMSS() const { return (*_viTime)[_iPsn]; }
const double QuoteProvider::Price() const { return (*_vdPrice)[_iPsn]; }
const long QuoteProvider::Volume() const { return (*_vlVolume)[_iPsn]; }
const int QuoteProvider::Initiator() const { return (*_viInitiator)[_iPsn]; }
const double QuoteProvider::BestBidPrice() const { return (*_vdBestBidPrice)[_iPsn]; }
const long QuoteProvider::BestBidSize() const { return (*_vlBestBidSize)[_iPsn]; }
const double QuoteProvider::BestAskPrice() const { return (*_vdBestAskPrice)[_iPsn]; }
const long QuoteProvider::BestAskSize() const { return (*_vlBestAskSize)[_iPsn]; }
const double QuoteProvider::WeightedMidQuote() const { return (BestBidPrice()*BestAskSize()+BestAskPrice()*BestBidSize())/(BestAskSize()+BestBidSize()); }

