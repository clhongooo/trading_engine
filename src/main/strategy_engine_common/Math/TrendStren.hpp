#ifndef TRENDSTREN_HPP
#define TRENDSTREN_HPP

#include "SException.h"
#include "DataQue.hpp"
#include "Sma.hpp"
#include <vector>

using namespace std;

template <typename T>
class TrendStren : public DataQue<T>
{
	private:
		int _iNumOfMAs;
		int _iStartingMA;
		int _iStepSize;
		vector<Sma<T> > _vSma;
		double _dLatestPrice;

		void AddChild(T);
		void AddChild(T,T){}
		void AddChild(T,T,T){}
		void AddChild(T,T,T,T){}
		void AddChild(T,T,T,T,T){}

	public:
		TrendStren(int,int,int,bool);
		void Reset(int,bool);
		void Reset(int,int,int,bool);
		bool Ready() const;
		T Value() const;
};


template <typename T>
TrendStren<T>::TrendStren(int iNMA, int iST, int iSS, bool bC)
{
	Reset(iNMA, iST, iSS, bC);
}

template <typename T>
void TrendStren<T>::Reset(int i, bool bC)
{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
}

template <typename T>
void TrendStren<T>::Reset(int iNMA, int iST, int iSS, bool bC)
{
	this->_bCheckDataAsPrice = bC;
	_iNumOfMAs = iNMA;
	_iStartingMA = iST;
	_iStepSize = iSS;
	_dLatestPrice = -1;

	int i = _iStartingMA;

	_vSma.clear();
	for (int j = 0; j < _iNumOfMAs; ++j)
	{
		_vSma.push_back(Sma<T>(i,bC));
		i += _iStepSize;
	}
}


template <typename T>
bool TrendStren<T>::Ready() const
{
	bool bR = true;
	for (int i = 0; i < _iNumOfMAs; ++i)
		if (!_vSma[i].Ready()) bR = false;
	return bR;
}

template <typename T>
void TrendStren<T>::AddChild(T dPx)
{
	_dLatestPrice = dPx;
	for (int i = 0; i < _iNumOfMAs; ++i)
		_vSma[i].Add(dPx);
}


template <typename T>
T TrendStren<T>::Value() const
{
	if (!Ready())
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	T dScore = 0;
	for (int i = 0; i < _iNumOfMAs; ++i)
	{
		if (_dLatestPrice > _vSma[i].Value())
			dScore += (T) 100 / _iNumOfMAs;
		else dScore -= (T) 100 / _iNumOfMAs;
	}
	return dScore;
}




#endif
