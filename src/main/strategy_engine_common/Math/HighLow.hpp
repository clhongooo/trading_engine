#ifndef HIGHLOW_HPP
#define HIGHLOW_HPP

#include <deque>
#include "SException.h"
#include "DataQue.hpp"
#include "SLowerBound.h"

using namespace std;

template <typename T>
class HighLow : public DataQue<T>
{
  private:
    deque<T> _dqdLows;
    deque<T> _dqdHighs;
    deque<T> _dqdLowsSorted;
    deque<T> _dqdHighsSorted;
    void AddChild(T);
    void AddChild(T,T);
    void AddChild(T,T,T) {}
    void AddChild(T,T,T,T) {}
    void AddChild(T,T,T,T,T) {}
    void Reset();

  public:
    HighLow();
    HighLow(int,bool);
    void Reset(int,bool);
    bool Ready() const;
    int Count();
    T HighestHigh() const;
    T HighestLow() const;
    T LowestLow() const;
    T LowestHigh() const;
    T Height() const;
};

  template <typename T>
HighLow<T>::HighLow()
{
  Reset();
}
  template <typename T>
HighLow<T>::HighLow(int iS, bool bC)
{
  Reset(iS,bC);
}

  template <typename T>
void HighLow<T>::Reset(int iS, bool bC)
{
  Reset();
  this->_iTargetSize = iS;
  this->_bCheckDataAsPrice = bC;
}

  template <typename T>
void HighLow<T>::Reset()
{
  this->_iTargetSize = 0;
  this->_bCheckDataAsPrice = true;
  _dqdLows.clear();
  _dqdHighs.clear();
  _dqdLowsSorted.clear();
  _dqdHighsSorted.clear();
}

  template <typename T>
int HighLow<T>::Count()
{
  if (_dqdLows.size() == _dqdHighs.size())
    return _dqdLows.size();
  else
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}

template <typename T>
bool HighLow<T>::Ready() const
{
  if (_dqdLows.size() < this->_iTargetSize || this->_iTargetSize == 0) return false;
  else return true;
}

  template <typename T>
void HighLow<T>::AddChild(T d)
{
  AddChild(d,d);
}

  template <typename T>
void HighLow<T>::AddChild(T d1, T d2)
{
  if (this->_iTargetSize == 0) return;
  //Determine which is dH, dL
  T dH, dL;
  if (d1 < d2)
  {
    dL = d1;
    dH = d2;
  }
  else
  {
    dH = d1;
    dL = d2;
  }

  //New item insertion
  _dqdHighs.push_back(dH);
  _dqdLows.push_back(dL);

  typename deque<T>::iterator it;

  if (_dqdHighsSorted.empty()) _dqdHighsSorted.push_back(dH);
  else
  {
    bool bWi = SLowerBound<typename deque<T>::iterator, T>::JustLarger(_dqdHighsSorted.begin(), _dqdHighsSorted.end(), dH, it);
    if (bWi) _dqdHighsSorted.insert(it, dH);
    else _dqdHighsSorted.push_back(dH);
  }

  if (_dqdLowsSorted.empty()) _dqdLowsSorted.push_back(dL);
  else
  {
    bool bWi = SLowerBound<typename deque<T>::iterator, T>::JustLarger(_dqdLowsSorted.begin(), _dqdLowsSorted.end(), dL, it);
    if (bWi) _dqdLowsSorted.insert(it, dL);
    else _dqdLowsSorted.push_back(dL);
  }

  //Old item removal
  if (_dqdHighs.size() > this->_iTargetSize)
  {
    T dOld = _dqdHighs[0];
    _dqdHighs.pop_front();
    if (!SLowerBound<typename deque<T>::iterator, T>::ExactMatch(_dqdHighsSorted.begin(), _dqdHighsSorted.end(), dOld, it))
    {
      SException se;
      se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
      throw se;
    }
    _dqdHighsSorted.erase(it);
  }
  if (_dqdLows.size() > this->_iTargetSize)
  {
    T dOld = _dqdLows[0];
    _dqdLows.pop_front();
    if (!SLowerBound<typename deque<T>::iterator, T>::ExactMatch(_dqdLowsSorted.begin(), _dqdLowsSorted.end(), dOld, it))
    {
      SException se;
      se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
      throw se;
    }
    _dqdLowsSorted.erase(it);
  }
}

template <typename T>
T HighLow<T>::HighestHigh() const
{
  if (!_dqdHighsSorted.empty()) return _dqdHighsSorted[_dqdHighsSorted.size()-1];
  else
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}
template <typename T>
T HighLow<T>::LowestHigh() const
{
  if (!_dqdHighsSorted.empty()) return _dqdHighsSorted[0];
  else
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}
template <typename T>
T HighLow<T>::HighestLow() const
{
  if (!_dqdLowsSorted.empty()) return _dqdLowsSorted[_dqdLowsSorted.size()-1];
  else
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}
template <typename T>
T HighLow<T>::LowestLow() const
{
  if (!_dqdLowsSorted.empty()) return _dqdLowsSorted[0];
  else
  {
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}

template <typename T>
T HighLow<T>::Height() const
{
  return HighestHigh() - LowestLow();
}



#endif
