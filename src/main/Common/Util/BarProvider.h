#ifndef BARPROVIDER_H
#define BARPROVIDER_H

#include "PCH.h"
#include "Constants.h"
#include "boost/iostreams/filtering_stream.hpp"
#include "boost/iostreams/filter/gzip.hpp"
#include "STool.h"
#include "SDateTime.h"
#include "Tokenizer.h"
#include "SException.h"

using namespace std;

// The design goal is to make this backbone class as fast and clean as possible
// Template for flexible member variable type is removed, because there is no such need
class BarProvider
{
  public:
    // Compression Type: 'G' = gzip, 'P' = plain
    // Format: "DOHLCV" for Date,O,H,L,C,V
    // Format: "DOHLCVA" for Date,A,A,A,A,V
    // Forward / Backward: 'F' forward 'B' backward
    // Header: 'H' has header, 'N' has no header
    BarProvider();
    BarProvider(const char *, const string &, int, char, char, char);
    BarProvider(const BarProvider &);
    const BarProvider& operator=(const BarProvider&);

    ~BarProvider();
    void Reset();
    void FreeVec();
    void LoadDataFile(const char *, const string &, int, char, char, char);

    //--------------------------------------------------
    void ResetPsn();
    int GetNumOfBars();
    bool HasNextBar();
    const YYYYMMDD PeekNextYMD();
    const YYYYMMDD PeekPrevYMD();
    const double PeekPrevClose();
    void MoveNext();

    //--------------------------------------------------
    void GetOHLCVInDateRange(const YYYYMMDD &,const YYYYMMDD &,vector<YYYYMMDD> &,vector<double> &,vector<double> &,vector<double> &,vector<double> &,vector<long> &);
    void GetOHLCVInDateTimeRange(const YYYYMMDD &,const HHMMSS &,const YYYYMMDD &,const HHMMSS &,vector<YYYYMMDD> &,vector<HHMMSS> &,vector<double> &,vector<double> &,vector<double> &,vector<double> &,vector<long> &);

    const YYYYMMDD & GetYYYYMMDD();
    const HHMMSS & GetHHMMSS();
    const double Open();
    const double High();
    const double Low();
    const double Close();
    const double HLCAvg();
    const long Volume();
    bool GetCloseArr(const int, vector<double> &);

    //--------------------------------------------------
    void AddBar(const YYYYMMDD &,const double,const double,const double,const double,const long);
    void AddBar(const YYYYMMDD &,const HHMMSS &,const double,const double,const double,const double,const long);


  private:
    void MakeDeepCopy(const BarProvider &);

    bool m_bMaster;
    int  m_iPsn;
    int  m_iNumOfBars;
    char m_cFwdBkwd; // Determines the direction m_iPsn moves

    // Low level structure exposed, but speed is more desirable than encapsulation. Also, we don't expect the bar structure to change at all
    vector<YYYYMMDD> * m_viYMD; //YYYYMMDD
    vector<HHMMSS> * m_viTime; //HHMMSS or HHMM

    // Can't use float in place of double because of the lack of support from GSL
    vector<double> * m_vdOpen;
    vector<double> * m_vdHigh;
    vector<double> * m_vdLow;
    vector<double> * m_vdClose;
    vector<long>   * m_vlVolume;

    bool m_ColumnPresent_A;
    bool m_ColumnPresent_C;
    bool m_ColumnPresent_D;
    bool m_ColumnPresent_H;
    bool m_ColumnPresent_L;
    bool m_ColumnPresent_O;
    bool m_ColumnPresent_T;
    bool m_ColumnPresent_V;
    bool m_ColumnPresent_x;

    bool m_DataModified;
    shared_mutex m_BarProvider_Mutex;

};

#endif
