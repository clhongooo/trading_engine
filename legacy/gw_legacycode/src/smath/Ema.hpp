#ifndef EMA_HPP
#define EMA_HPP

#include "../corelib/IParam.hpp"
#include "../corelib/SException.hpp"
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

	public:
		Ema();
		Ema(double,bool);
		void Reset(int,bool);
		void SetAlpha(double);
		bool WarmedUp() const;
		T Value() const;
};


template <typename T>
Ema<T>::Ema()
{
	Reset(0,false);
}

template <typename T>
Ema<T>::Ema(int iPeriod, bool bC)
{
  double dAlpha = (double)1/(double)iPeriod;
	if (dAlpha <= 0 || dAlpha >= 1)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	this->_iTargetSize = 0;
	this->_bCheckDataAsPrice = bC;
	_dAlpha = dAlpha;
	_dLastEMA = IParam::MAGIC_NUM;
}

template <typename T>
void Ema<T>::SetAlpha(double dAlpha)
{
	_dAlpha = dAlpha;
}

template <typename T>
bool Ema<T>::WarmedUp() const { if (_dLastEMA != IParam::MAGIC_NUM) return true; else return false;}


template <typename T>
void Ema<T>::AddChild(T dP)
{
	if (_dLastEMA == IParam::MAGIC_NUM) _dLastEMA = dP;
	else _dLastEMA = _dAlpha * dP + (1-_dAlpha) * _dLastEMA;
}

template <typename T>
T Ema<T>::Value() const
{
	if (_dLastEMA != IParam::MAGIC_NUM)
		return _dLastEMA;
	else
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
}

#endif
