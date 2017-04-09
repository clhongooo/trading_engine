#ifndef CIRDATAQUE_H
#define CIRDATAQUE_H

#include "DataQue.hpp"
#include <cstring>
#include <vector>

using namespace std;

template <typename T>
class CirDataQue : public DataQue<T>
{
  protected:
    vector<T> _vCirDQ;
    vector<T> _vCirDQInsOrd;
    int _iCirDQPsn;
    unsigned int _iOpulentSize; //for speedy GetArrayInInsOrd(), fewer wrap-arounds, less memcpy
    void AddChild(T);
    void AddChild(T,T){}
    void AddChild(T,T,T){}
    void AddChild(T,T,T,T){}
    void AddChild(T,T,T,T,T){}

  public:
    CirDataQue();
    ~CirDataQue();
    bool Ready() const;
    void Reset(int,bool);
    void Reset(int,int,bool); //the use of opulent vector
    int Count();
    int CirPsn();
    const T* GetArrayUnord(int &) const;
    const T* GetArrayInInsOrd();
    string DebugMsg();
};


  template <typename T>
CirDataQue<T>::CirDataQue()
{
  this->_iTargetSize = 0;
  this->_bCheckDataAsPrice = true;
  _iOpulentSize = 0;
  _iCirDQPsn = 0;
  _vCirDQ.clear();
  _vCirDQInsOrd.clear();
}

  template <typename T>
CirDataQue<T>::~CirDataQue()
{
  _vCirDQ.clear();
  _vCirDQInsOrd.clear();
}

  template <typename T>
void CirDataQue<T>::Reset(int iS, bool bC)
{
  this->_iTargetSize = iS;
  this->_bCheckDataAsPrice = bC;
  _iOpulentSize = 0;
  _iCirDQPsn = 0;
  _vCirDQ.clear();
  _vCirDQ.reserve(iS);
  _vCirDQInsOrd.clear();
  _vCirDQInsOrd.reserve(iS);
  for (int i = 0; i < iS; ++i) _vCirDQInsOrd.push_back(0);
}

  template <typename T>
void CirDataQue<T>::Reset(int iS, int iOS, bool bC)
{
  if (iOS <= iS) return;

  this->_iTargetSize = iS;
  this->_bCheckDataAsPrice = bC;
  _iOpulentSize = iOS;
  _iCirDQPsn = 0;
  _vCirDQ.clear();
  _vCirDQ.reserve(iOS);
  _vCirDQInsOrd.clear();
  _vCirDQInsOrd.reserve(iS);
  for (int i = 0; i < iS; ++i) _vCirDQInsOrd.push_back(0);
}

template <typename T>
bool CirDataQue<T>::Ready() const
{
  if (_iOpulentSize == 0 || _iOpulentSize > this->_iTargetSize)
  {
    if (this->_iTargetSize > 0 && _vCirDQ.size() >= this->_iTargetSize) return true;
    else return false;
  }
  else
  {
    return false;
  }
}

  template <typename T>
int CirDataQue<T>::Count()
{
  return _vCirDQ.size();
}

  template <typename T>
int CirDataQue<T>::CirPsn()
{
  return _iCirDQPsn;
}

  template <typename T>
void CirDataQue<T>::AddChild(T dP)
{
  if (this->_iTargetSize == 0) return;

  if (_iOpulentSize == 0)
  {
    if (_vCirDQ.size() < this->_iTargetSize) _vCirDQ.push_back(dP);
    else if (_vCirDQ.size() == this->_iTargetSize)
    {
      _vCirDQ[_iCirDQPsn] = dP;
      _iCirDQPsn = (_iCirDQPsn+1) % this->_iTargetSize;
    }
    else if (_vCirDQ.size() > this->_iTargetSize)
    {
      return;
    }
  }
  else if (_iOpulentSize > this->_iTargetSize)
  {
    if (_vCirDQ.size() < _iOpulentSize) _vCirDQ.push_back(dP);
    else if (_vCirDQ.size() == _iOpulentSize)
    {
      _vCirDQ[_iCirDQPsn] = dP;
      _iCirDQPsn = (_iCirDQPsn+1) % _iOpulentSize;
    }
    else if (_vCirDQ.size() > _iOpulentSize)
    {
      return;
    }
  }
  else if (_iOpulentSize <= this->_iTargetSize)
  {
    return;
  }
}

//not support opulent vector
//support not warmedup
template <typename T>
const T* CirDataQue<T>::GetArrayUnord(int &iSize) const
{
  if (_iOpulentSize == 0)
  {
    if (!_vCirDQ.empty())
    {
      iSize = _vCirDQ.size();
      return &(_vCirDQ[0]);
    }
    else
    {
      return NULL;
    }
  }
  else //if one uses an opulent vector, why call this function?
  {
    return NULL;
  }
}

//supports both with or without opulent vector
//not support not warmedup
  template <typename T>
const T* CirDataQue<T>::GetArrayInInsOrd()
{
  // //avoid including cmath
  // int iMax;
  // if (this->_iTargetSize > _iOpulentSize)
  //   iMax = this->_iTargetSize;
  // else
  //   iMax = _iOpulentSize;

  //too few data, we refuse to give
  if (_vCirDQ.size() < this->_iTargetSize) return NULL;

  //normal cases: opulent vector not used
  if (_iOpulentSize == 0 && _iCirDQPsn == 0)
  {
    return &(_vCirDQ[0]);
  }
  else if (_iOpulentSize == 0 && _iCirDQPsn != 0)
  {
    memcpy(&(_vCirDQInsOrd[0]), &(_vCirDQ[_iCirDQPsn]), sizeof(T)*(this->_iTargetSize-_iCirDQPsn));
    memcpy(&(_vCirDQInsOrd[this->_iTargetSize-_iCirDQPsn]), &(_vCirDQ[0]), sizeof(T)*(_iCirDQPsn));
    return &(_vCirDQInsOrd[0]);
  }
  //normal cases: opulent vector used
  else if (_iOpulentSize > this->_iTargetSize)
  {
    if (_vCirDQ.size() < _iOpulentSize && _vCirDQ.size() >= this->_iTargetSize)
    {
      return &(_vCirDQ[_vCirDQ.size() - this->_iTargetSize]);
    }
    else if (_vCirDQ.size() == _iOpulentSize)
    {
      if ((unsigned int) _iCirDQPsn >= this->_iTargetSize)
      {
        return &(_vCirDQ[_iCirDQPsn - this->_iTargetSize]);
      }
      else if ((unsigned int) _iCirDQPsn < this->_iTargetSize)
      {
        int iDangling = this->_iTargetSize - _iCirDQPsn;
        memcpy(&(_vCirDQInsOrd[0]), &(_vCirDQ[_iOpulentSize-iDangling]), sizeof(T)*(iDangling));
        memcpy(&(_vCirDQInsOrd[iDangling]), &(_vCirDQ[0]), sizeof(T)*(_iCirDQPsn));
        return &(_vCirDQInsOrd[0]);
      }
    }
  }

  return NULL;
}


/*
   template <typename T>
   string CirDataQue<T>::DebugMsg()
   {
   string sR = "";
   sR += "_vCirDQ.size() = " + boost::lexical_cast<string>(_vCirDQ.size()) + "\n";
   sR += "_vCirDQInsOrd.size() = " + boost::lexical_cast<string>(_vCirDQInsOrd.size()) + "\n";
   for (int i = 0; i < _vCirDQ.size(); ++i) sR += boost::lexical_cast<string>(_vCirDQ[i]) + ", ";
   sR += "\n";
   for (int i = 0; i < _vCirDQInsOrd.size(); ++i) sR += boost::lexical_cast<string>(_vCirDQInsOrd[i]) + ", ";
   sR += "\n";
   sR += "_iTargetSize: " + boost::lexical_cast<string>(this->_iTargetSize) + "\n";
   sR += "_bCheckDataAsPrice: " + boost::lexical_cast<string>(this->_bCheckDataAsPrice) + "\n";
   sR += "_iOpulentSize: " + boost::lexical_cast<string>(_iOpulentSize) + "\n";
   sR += "_iCirDQPsn: " + boost::lexical_cast<string>(_iCirDQPsn) + "\n";
   return sR;
   }
 */



#endif
