#ifndef TrdTickAna_H_
#define TrdTickAna_H_

#include "PCH.h"
#include "Constants.h"
#include "SDateTime.h"
#include "TradeTick.h"

using namespace std;

// Formula: sum(S_i V_i)
class TrdTickAna
{
  public:
    TrdTickAna(const unsigned short);
    ~TrdTickAna();
    void Reset();
    void Add(const HMS &, const long, const int);
    void Add(TradeTick &);
    double IMBNmlzd();
    double IMBRaw();
    unsigned long NoOfTrades();
    long TotalVolume();
    double MeanVolume();
    double ProbBuyerInitiated();
    double CovInitr();
    double CorrInitr();
    //long MeanInterarrivalTimeMillisec();

  private:
    unsigned short m_usRollgWinInMinutes;
    deque<TradeTick> m_dqTrdTick;
    double m_dSum_Initr_x_Vol;
    long m_lTotalVol;
    long m_lTotalBuyerInitiated;
};

#endif
