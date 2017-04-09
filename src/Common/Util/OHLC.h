#ifndef UTIL_OHLC_H_
#define UTIL_OHLC_H_

#include "PCH.h"
#include "Constants.h"

class OHLC
{
  public:
    OHLC();
    virtual ~OHLC();
    void Add(const double);
    void Add(const double,const double,const double,const double);
    void Reset();
    double Open()  const;
    double High()  const;
    double Low()   const;
    double Close() const;
  private:
    double m_Open;
    double m_High;
    double m_Low;
    double m_Close;
    bool   m_Initialized;
};

#endif
