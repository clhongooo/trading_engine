#ifndef TICK_RETURN_H
#define TICK_RETURN_H

#include "PCH.h"
#include "Constants.h"
#include "SDateTime.h"

using namespace std;

class TickReturn
{
  public:
    TickReturn();
    TickReturn(const long);
    ~TickReturn();
    void Reset();
    void Reset(const long);
    bool Ready();
    void Add(const YYYYMMDDHHMMSS &, const double);
    double Return();
    double ReturnR();
    double LnReturnR();

  private:
    long                  m_lRollgWinInSeconds;
    deque<YYYYMMDDHHMMSS> m_dqYMDHMS;
    deque<double>         m_dqTradePrice;
    bool                  m_b_Ready;
    static const double   m_undefined;
};

#endif
