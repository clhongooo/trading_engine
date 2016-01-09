#ifndef GKYZ_H
#define GKYZ_H

#define DAYS_IN_YEAR 252

#include "PCH.h"
#include "Constants.h"
#include "DataQue.hpp"

using namespace std;

class GKYZ : public DataQue<double>
{
  private:
    void AddChild(double){}
    void AddChild(double,double){}
    void AddChild(double,double,double){}
    void AddChild(double,double,double,double);
    void AddChild(double,double,double,double,double){}
    void Reset(int,bool){}
    void debugPrint();

    unsigned int m_WinSize;
    double m_PrevClose;
    deque<double> m_dq_LogOoverPreviousCSquared;
    deque<double> m_dq_LogHoverLSquared;
    deque<double> m_dq_LogCoverOSquared;

    double m_sumLogOoverPreviousCSquared;
    double m_sumLogHoverLSquared;
    double m_sumLogCoverOSquared;

  public:
    GKYZ();
    GKYZ(const unsigned int);
    GKYZ(const GKYZ &);
    GKYZ& operator=(const GKYZ &);
    GKYZ(const unsigned int,bool);
    void Reset(const unsigned int,bool);
    bool Ready() const;
    double Value() const;
    double getGKZYRealizedVolatility() const;
};

#endif
