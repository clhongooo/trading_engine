#ifndef DAYSMA_H_
#define DAYSMA_H_

#include "PCH.h"
#include "Constants.h"
#include "DataQue.hpp"
#include "SDateTime.h"

class PeriodSma
{
  private:
    deque<YYYYMMDDHHMMSS> m_dq_yyyymmddhhmmss;
    deque<double>         m_dq_dX;
    double                m_d_RunningTotal;
    long                  m_i_NoOfSeconds;

  public:
    PeriodSma();
    PeriodSma(const long);
    virtual ~PeriodSma();

    void Add(const YYYYMMDDHHMMSS &,double);
    void Reset(const long);
    bool Ready() const;
    double Value() const;

};

#endif /* DAYSMA_H_ */
