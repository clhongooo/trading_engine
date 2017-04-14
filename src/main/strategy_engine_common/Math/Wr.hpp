#ifndef WR_HPP
#define WR_HPP

#include "../Util/SException.h"
#include "../Math/DataQue.hpp"
#include "HighLow.hpp"

using namespace std;

template <typename T>
class Wr : public DataQue<T>
{
	private:
		HighLow<T> _highlow;
		double _dClose;
		void AddChild(T) {}
		void AddChild(T,T) {}
		void AddChild(T,T,T);
		void AddChild(T,T,T,T) {}
		void AddChild(T,T,T,T,T) {}

	public:
		Wr();
		void Reset(int,bool);
		bool Ready() const;
		T Value() const;
};

template <typename T>
Wr<T>::Wr()
{
	Reset(0,false);
}

template <typename T>
void Wr<T>::Reset(int iS, bool bC)
{
	_highlow.Reset(iS,bC);
	this->_bCheckDataAsPrice = bC;
	_dClose = 0;
}

template <typename T>
void Wr<T>::AddChild(T dH, T dL, T dC)
{
	_highlow.Add(dH, dL);
	_dClose = dC;
}

template <typename T>
bool Wr<T>::Ready() const { return _highlow.Ready(); }

template <typename T>
T Wr<T>::Value() const
{
	return (_highlow.HighestHigh() - _dClose) / (_highlow.HighestHigh() - _highlow.LowestLow()) * -100;
}



#endif
