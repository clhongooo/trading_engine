// MACD = [stockPrices,12]EMA - [stockPrices,26]EMA
// signal = [MACD,9]EMA
// divergence = MACD – signal

#ifndef MACD_H
#define MACD_H

#include "PCH.h"
#include "Constants.h"
#include "Ema.hpp"

using namespace std;

class MACD : public DataQue<double>
{
  private:
    Ema<double> _ema1;
    Ema<double> _ema2;
    Ema<double> _ema3; //signal
    double _dLastMACD;
    double _dLastMACDDiv;
    void AddChild(double);
    void AddChild(double,double){}
    void AddChild(double,double,double){}
    void AddChild(double,double,double,double){}
    void AddChild(double,double,double,double,double){}
    void Reset(int,bool){}

  public:
    MACD();
    MACD(unsigned int,unsigned int,unsigned int,bool);
    void Reset(unsigned int,unsigned int,unsigned int,bool);
    bool Ready() const;
    double Value() const;
    double MACDDiv() const;
    double MACDVal() const;
};

#endif
