#ifndef SMA_HPP
#define SMA_HPP

#include "../Util/SException.h"
#include "DataQue.hpp"
#include <deque>

using namespace std;

template <typename T>
class Sma : public DataQue<T>
{
  private:
    T _dRunningTotal;
    deque<T> _dqdX;
    void AddChild(T);
    void AddChild(T,T){}
    void AddChild(T,T,T){}
    void AddChild(T,T,T,T){}
    void AddChild(T,T,T,T,T){}

  public:
    Sma();
    Sma(int,bool);
    void Reset(int,bool);
    bool Ready() const;
    T Value() const;
};

  template <typename T>
Sma<T>::Sma()
{
  Reset(0,false);
}

  template <typename T>
Sma<T>::Sma(int iS, bool bC)
{
  Reset(iS,bC);
}

  template <typename T>
void Sma<T>::Reset(int iS, bool bC)
{
  if (iS <= 0) this->_iTargetSize = 1;
  else this->_iTargetSize = iS;

  this->_bCheckDataAsPrice = bC;
  _dRunningTotal = 0;
  _dqdX.clear();
}


template <typename T>
bool Sma<T>::Ready() const { if (_dqdX.size() >= this->_iTargetSize && this->_iTargetSize > 0) return true; else return false;}


  template <typename T>
void Sma<T>::AddChild(T dP)
{
  if (_dqdX.size() > this->_iTargetSize)
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }

  if (_dqdX.size() == this->_iTargetSize)
  {
    _dRunningTotal -= _dqdX.front();
    _dqdX.pop_front();
  }

  _dRunningTotal += dP;
  _dqdX.push_back(dP);

}

template <typename T>
T Sma<T>::Value() const
{
  if (!_dqdX.empty())
    return _dRunningTotal / _dqdX.size();
  else
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}


#endif
