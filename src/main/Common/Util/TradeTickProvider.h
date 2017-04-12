#ifndef TRADETICKPROVIDER_H
#define TRADETICKPROVIDER_H

#include "PCH.h"
#include "Constants.h"
#include "boost/iostreams/filtering_stream.hpp"
#include "boost/iostreams/filter/gzip.hpp"
#include "STool.h"
#include "SDateTime.h"
#include "Tokenizer.h"
#include "SException.h"

using namespace std;
using namespace boost;

class TradeTickProvider
{
  private:
    int _iPsn;
    int _iNumOfBars;

    vector<YYYYMMDD> * _viYMD;
    vector<HHMMSS> *   _viTime;
    vector<double> * _vdPrice;
    vector<long> *   _vlVolume;
    vector<int> *    _viInitiator;


  public:
    const static string _sSSE;

    TradeTickProvider();
    TradeTickProvider(const char *, const string &, char, char);

    ~TradeTickProvider();
    void FreeVec();
    void LoadDataFile(const char *, const string &, char, char);
    void ResetPsn();
    int GetNumOfBars() const;
    bool HasNextBar() const;
    const YYYYMMDD PeekNextYMDi() const;
    const YYYYMMDD PeekPrevYMDi();
    void MoveNext();
    const double* GetPriceArr(int);
    const YYYYMMDD & GetYYYYMMDD() const;
    const HHMMSS & GetHHMMSS() const;
    const double Price() const;
    const long Volume() const;
    const int Initiator() const;

};



#endif
