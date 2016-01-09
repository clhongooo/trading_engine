#ifndef WMA_HPP
#define WMA_HPP

#include "../Util/SException.h"
#include "DataQue.hpp"
#include <deque>
#include <vector>

using namespace std;

template <typename T>
class Wma : public DataQue<T>
{
	private:
		vector<int> vWeight; //convention is that the first element is the weight for the last price
		deque<T> dqX;
		void AddChild(T);
		void AddChild(T,T){}
		void AddChild(T,T,T){}
		void AddChild(T,T,T,T){}
		void AddChild(T,T,T,T,T){}
		void Reset(int,bool) {}

	public:
		Wma();
		Wma(const vector<int> &,bool);
		void Reset(const vector<int> &,bool);
		bool Ready() const;
		T Value() const;
};

template <typename T>
Wma<T>::Wma()
{
}

template <typename T>
Wma<T>::Wma(const vector<int> & vArg, bool bC)
{
	Reset(vArg,bC);
}

template <typename T>
void Wma<T>::Reset(const vector<int> &vArg, bool bC)
{
	if (vArg.empty())
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	dqX.clear();
	vWeight = vArg; //should call copy constructor
	this->_iTargetSize = vArg.size(); //actually not used
	this->_bCheckDataAsPrice = bC;
}

template <typename T>
bool Wma<T>::Ready() const { if (!vWeight.empty() && dqX.size() >= vWeight.size()) return true; else return false;}


template <typename T>
void Wma<T>::AddChild(T dP)
{
	if (vWeight.empty())
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	dqX.push_back(dP);
	if (dqX.size() > vWeight.size() + 2) dqX.pop_front();
}

template <typename T>
T Wma<T>::Value() const
{
	if (Ready())
	{
		int iSumWeights = 0;
		T tRes = 0;
		for (unsigned i=0; i<vWeight.size(); ++i)
		{
			iSumWeights += vWeight[i];
			tRes += vWeight[i] * dqX[dqX.size()-1-i];
		}
		return tRes / (double) iSumWeights;
	}
	else
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
}

#endif
