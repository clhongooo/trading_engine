#ifndef RSI_HPP
#define RSI_HPP

#include "PCH.h"
#include "Constants.h"
#include "DataQue.hpp"
#include "Ema.hpp"
#include "Sma.hpp"

using namespace std;

class Rsi : public DataQue<double>
{
  public:
    enum Method {EMA, SMA};
    Rsi(int,bool,Rsi::Method);
    void Reset(int,bool,Rsi::Method);
    bool Ready() const;
    double Value() const;

  private:
    int _iPeriod;
    double _dCloseNow, _dClosePrev, _RSI;
    Rsi::Method _method;
    Ema<double> _emaU;
    Ema<double> _emaD;
    Sma<double> _smaU;
    Sma<double> _smaD;
    void Reset(int,bool) {}
    void AddChild(double);
    void AddChild(double,double) {}
    void AddChild(double,double,double) {}
    void AddChild(double,double,double,double) {}
    void AddChild(double,double,double,double,double) {}
};
#endif
