#ifndef HKFE_H
#define HKFE_H

#include "PCH.h"
#include "Constants.h"
#include "STool.h"
#include "SDateTime.h"
#include "SLowerBound.h"
#include "Tokenizer.h"
#include "ValidityChk.h"
#include "CustType.h"

using namespace std;

// 20000101,94500,123000,143000,161500
// 20110307,91500,120000,133000,161500
// 20120305,91500,120000,130000,161500
// 20130408,91500,120000,130000,161500,after-hours

class HKFE
{
  public:
    enum ContractType {FUTURES,CALL,PUT};
    enum TRDGSESS {BEFORE_OPEN=7,MRNG_SESS=1,LUNCH_BRK=8,AFTN_SESS=2,AFT_CLOSE=9,TERMINALMKTSTS=10,UNDEFMKTSTS=99};
    static const double _TRDG_DAY_PER_YR;

    //--------------------------------------------------
    static boost::shared_ptr<HKFE> Instance();
    virtual ~HKFE();

    void     LoadCalendar(const string &);
    string   GetSymbol(char cInstr, int iY, int iM);
    string   GetSymbol(char cInstr, int iK, int iY, int iM);
    bool     IsCall(const string &sSym);
    bool     IsPut(const string &sSym);
    bool     IsFut(const string &sSym);
    double   GetStrike(const string &sSym);
    YYYYMMDD GetExpy(const string &sSym, int iFM);
    YYYYMMDD GetExpy(YMD ymd, int iFM);
    YYYYMMDD GetExpyFmtMxx(const ContractType, const string &);
    string   FutCode(int iY, int iM);
    bool     WhichFrontMth(const int iCurYMD, const int iCtrtYM, int & iWhichFM);

    long     GetDTM(YYYYMMDD ymd, int iFM);
    double   GetTTM(YYYYMMDDHHMMSS ymdhms, int iFM);

    void     NextTrdgDate(YMD & ymd, unsigned iNumDay);
    void     NextTrdgMin(YMD & ymd, HMS & hms);

    TRDGSESS GetTrdgSess(const YMD & ymd, const HMS & hms);
    void     GetMrngOpen(const YMD & ymd, HMS & hms);
    void     GetMrngClose(const YMD & ymd, HMS & hms);
    void     GetAftnOpen(const YMD & ymd, HMS & hms);
    void     GetAftnClose(const YMD & ymd, HMS & hms);

    bool     IsExpyDay(const YYYYMMDD & ymd);
    bool     IsExpyDay(const YMD & ymd);
    bool     IsHalfDay(const YYYYMMDD & ymd);
    bool     IsHalfDay(const YMD & ymd);
    bool     IsTrdgDay(const YYYYMMDD & ymd);
    bool     IsTrdgDay(const YMD & ymd);
    // bool     IsTradingHour(const YYYYMMDDHHMMSS & ymdhms);
    // bool     IsTradingHour(const YYYYMMDD & ymd, const HHMMSS & hms);
    // bool     IsTradingHour(const YMD & ymd, const HMS & hms);
    bool     IsTrdgHour5m(const YYYYMMDD & ymd, const HHMMSS & hms);
    bool     IsTrdgHour5m(const YMD & ymd, const HMS & hms);
    bool     IsTrdgHour10m(const YYYYMMDD & ymd, const HHMMSS & hms);
    bool     IsTrdgHour10m(const YMD & ymd, const HMS & hms);

    string   GetCtrtCode1(const ContractType, const YYYYMMDD & ymd, const int);
    string   GetCtrtCode2(const ContractType, const YYYYMMDD & ymd, const int);
    string   GetCtrtCode1_WithDTM(const ContractType, const YYYYMMDD & ymd, const int);

    string   Get1FMFutCodeAll();
    string   Get2FMFutCodeAll();

    bool     IsAProductInThisMarket(const string &);
    double   GetContractMultiplier(const string &);

  private:

    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    HKFE();
    HKFE(const HKFE&);
    // HKFE& operator=(const HKFE&) {};
    static boost::weak_ptr<HKFE> m_pInstance;

    static YYYYMMDD   ymd20130408;
    static YYYYMMDD   ymd20120305;
    static YYYYMMDD   ymd20110307;
    static YYYYMMDD   ymd20000000;
    static HHMMSS     hms91500;
    static HHMMSS     hms94500;
    static HHMMSS     hms120000;
    static HHMMSS     hms123000;
    static HHMMSS     hms130000;
    static HHMMSS     hms133000;
    static HHMMSS     hms143000;
    static HHMMSS     hms161500;

    vector<YYYYMMDD>  _vTrdgD;
    vector<int>       _vExpyDIdx;   //Idx of TrdgD at each ExpyD
    vector<int>       _vHalfDIdx;   //Idx of TrdgD at each HalfD
    map<int,int>      _mCtrt_TrdgDIdx; //mapping ctrt and TrdgD Idx
    map<int,int>      _mCtrt_1FMthNum;
    map<int,int>      _mCtrt_2FMthNum;
    map<int,char>     _mCtrt_1FMthFutCode;
    map<int,char>     _mCtrt_2FMthFutCode;
    map<int,char>     _mCtrt_1FMthCallCode;
    map<int,char>     _mCtrt_2FMthCallCode;
    map<int,char>     _mCtrt_1FMthPutCode;
    map<int,char>     _mCtrt_2FMthPutCode;

    int LocateTrdgD(YYYYMMDD);
    // static double GetTTM_hidden(YYYYMMDD ymd, int iCtrtYM);
    int VerbatimYMi(const string &sSym);
};

#endif
