#ifndef QUOTEPROVIDER_H
#define QUOTEPROVIDER_H

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

class QuoteProvider
{
  private:
    int _iPsn;
    int _iNumOfBars;

    vector<YYYYMMDD> * _viYMD;
    vector<HHMMSS> *   _viTime;
    //--------------------------------------------------
    // Quote
    //--------------------------------------------------
    vector<double> * _vdPrice;
    vector<long> *   _vlVolume;
    vector<int> *    _viInitiator;
    vector<double> * _vdBestBidPrice;
    vector<long> *   _vlBestBidSize;
    vector<double> * _vdBestAskPrice;
    vector<long> *   _vlBestAskSize;


  public:
    const static string _sSSE;

    QuoteProvider();
    QuoteProvider(const char *, const string &, char, char);

    ~QuoteProvider();
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
    const double BestBidPrice() const;
    const long BestBidSize() const;
    const double BestAskPrice() const;
    const long BestAskSize() const;
    const double WeightedMidQuote() const;

};

#endif
