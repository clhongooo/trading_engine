#ifndef BARAGGREGATOR_H_
#define BARAGGREGATOR_H_

#include "PCH.h"
#include "Constants.h"
#include "SDateTime.h"

class BarAggregator {
  public:
    enum BarInterval {UNSET, SECOND, MINUTE, HOUR, DAY};
    BarAggregator();
    virtual ~BarAggregator();
    void SetBarInterval(BarInterval);
    void AddTick(const YYYYMMDD &,const HHMMSS &, double,long);
    bool GetOHLCBar(YYYYMMDD &,HHMMSS &,double &,double &,double &,double &,long &);
    bool PeekOHLCBar(YYYYMMDD &,HHMMSS &,double &,double &,double &,double &,long &);
    void DebugPrintContents();

  private:
    void Cutoff();

    //--------------------------------------------------
    // An individual bar
    //--------------------------------------------------
    BarInterval m_BarInterval;
    bool        m_BarIsValid;
    YYYYMMDD    m_YYYYMMDD;
    HHMMSS      m_HHMMSS;
    double      m_Open;
    double      m_High;
    double      m_Low;
    double      m_Close;
    long        m_Volume;

    //--------------------------------------------------
    // All bars
    //--------------------------------------------------
    deque<YYYYMMDD> m_dq_YYYYMMDD;
    deque<HHMMSS>   m_dq_HHMMSS;
    deque<double>   m_dq_Open;
    deque<double>   m_dq_High;
    deque<double>   m_dq_Low;
    deque<double>   m_dq_Close;
    deque<long>     m_dq_Volume;

};

#endif /* BARAGGREGATOR_H_ */
