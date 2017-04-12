#ifndef DELAYEDCIRDATAQUE_H
#define DELAYEDCIRDATAQUE_H

#include "../Util/SException.h"
#include "CirDataQue.hpp"
#include <deque>

using namespace std;

template <typename T>
class DelayedCirDataQue : public CirDataQue<T>
{
	private:
		int _iDelayPeriod;
		deque<T> _dqBuf;
		void AddChild(T);

	public:
		DelayedCirDataQue();
		void Reset(int,bool);
		void Reset(int,int,bool);
		void Reset(int,int,int,bool);
};

template <typename T>
DelayedCirDataQue<T>::DelayedCirDataQue() : _iDelayPeriod(0)
{
	typename CirDataQue<T>::CirDataQue();
	_dqBuf.clear();
}

template <typename T>
void DelayedCirDataQue<T>::Reset(int i, bool bC)
{
	SException se;
	se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
	throw se;
}

template <typename T>
void DelayedCirDataQue<T>::Reset(int iS, int iD, bool bC)
{
	CirDataQue<T>::Reset(iS,bC);
	_dqBuf.clear();
	_iDelayPeriod = iD;
}

template <typename T>
void DelayedCirDataQue<T>::Reset(int iS, int iO, int iD, bool bC)
{
	CirDataQue<T>::Reset(iS,iO,bC);
	_dqBuf.clear();
	_iDelayPeriod = iD;
}


template <typename T>
void DelayedCirDataQue<T>::AddChild(T dP)
{
	_dqBuf.push_back(dP);
	if (_dqBuf.size() > (unsigned int) _iDelayPeriod)
	{
		CirDataQue<T>::AddChild(_dqBuf.front());
		_dqBuf.pop_front();
	}
}




#endif
