#ifndef ATR_H
#define ATR_H

#include "../Util/IParam.h"
#include "../Util/SException.h"
#include "DataQue.hpp"
#include "Ema.hpp"
#include <cmath>
#include <deque>

using namespace std;

template <typename T>
class Atr : public DataQue<T>
{
	private:
		T _dLastC;
		Ema<T> _emaATR;
		void AddChild(T){}
		void AddChild(T,T){}
		void AddChild(T,T,T);
		void AddChild(T,T,T,T){}
		void AddChild(T,T,T,T,T){}
		void Reset(int,bool) {}

	public:
		Atr();
		Atr(double,bool);
		void Reset(double,bool);
		void SetAlpha(double);
		bool Ready() const;
		T Value() const;
};

template <typename T>
Atr<T>::Atr()
{
	Reset(0,false);
}

template <typename T>
Atr<T>::Atr(double dAlpha, bool bC)
{
	Reset(dAlpha,bC);
}

template <typename T>
void Atr<T>::Reset(double dAlpha, bool bC)
{
	if (dAlpha <= 0 || dAlpha >= 1)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	this->_iTargetSize = 0;
	this->_bCheckDataAsPrice = bC;
	_emaATR.Reset(dAlpha,bC);
	_dLastC = MAGIC_NUM;
}

template <typename T>
void Atr<T>::SetAlpha(double dAlpha)
{
	_emaATR.SetAlpha(dAlpha);
}

template <typename T>
bool Atr<T>::Ready() const { return _emaATR.Ready(); }


template <typename T>
void Atr<T>::AddChild(T dH, T dL, T dC)
{
	if (_dLastC != MAGIC_NUM)
	{
		double dTR1 = abs(dH-_dLastC);
		double dTR2 = abs(dL-_dLastC);
		double dTR3 = abs(dH-dL);

		double dTR = dTR1;
		if (dTR2 > dTR) dTR = dTR2;
		if (dTR3 > dTR) dTR = dTR3;

		_emaATR.Add(dTR);
	}
	_dLastC = dC;

}

	template <typename T>
T Atr<T>::Value() const
{
	if (_emaATR.Ready())
		return _emaATR.Value();
	else
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
}



#endif
