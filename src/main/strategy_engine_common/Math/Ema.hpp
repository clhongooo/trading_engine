#ifndef EMA_H
#define EMA_H

#include "Constants.h"
#include "SException.h"
#include "DataQue.hpp"
#include <deque>

using namespace std;

template <typename T>
class Ema : public DataQue<T>
{
	private:
		T _dLastEMA;
		double _dAlpha;
		void AddChild(T);
		void AddChild(T,T){}
		void AddChild(T,T,T){}
		void AddChild(T,T,T,T){}
		void AddChild(T,T,T,T,T){}
		void Reset(int,bool){}

	public:
		Ema();
		Ema(double,bool);
		void Reset(double,bool);
		void SetAlpha(double);
		bool Ready() const;
		T Value() const;
};

template <typename T>
Ema<T>::Ema()
{
	Reset(0.1,false);
}

template <typename T>
Ema<T>::Ema(double dAlpha, bool bC)
{
	Reset(dAlpha,bC);
}


template <typename T>
void Ema<T>::Reset(double dAlpha, bool bC)
{
	if (dAlpha <= 0 || dAlpha > 1)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	this->_iTargetSize = 0;
	this->_bCheckDataAsPrice = bC;
	_dAlpha = dAlpha;
	_dLastEMA = MAGIC_NUM;
}

template <typename T>
void Ema<T>::SetAlpha(double dAlpha)
{
	_dAlpha = dAlpha;
}

template <typename T>
bool Ema<T>::Ready() const { if (_dLastEMA != MAGIC_NUM) return true; else return false;}


template <typename T>
void Ema<T>::AddChild(T dP)
{
	if (_dLastEMA == MAGIC_NUM) _dLastEMA = dP;
	else _dLastEMA = _dAlpha * dP + (1-_dAlpha) * _dLastEMA;
}

template <typename T>
T Ema<T>::Value() const
{
  return _dLastEMA;
}



#endif
