#ifndef DAYSMA_H_
#define DAYSMA_H_

#include "PCH.h"
#include "Constants.h"
#include "DataQue.hpp"
#include "SDateTime.h"

class DaySma
{
  private:
    deque<YYYYMMDD> m_dq_yyyymmdd;
    deque<double>   m_dq_dX;
    double          m_d_RunningTotal;
    int             m_i_NoOfDays;

  public:
    DaySma();
    DaySma(const int);
    virtual ~DaySma();

    void Add(const YYYYMMDD &,double);
    void Reset(const int);
    bool Ready() const;
    double Value() const;

};

#endif /* DAYSMA_H_ */
