#ifndef DATAQUE_HPP
#define DATAQUE_HPP

#include "../corelib/ValidityChk.hpp"
#include <vector>

using namespace std;

template <typename T>
class DataQue
{
	protected:
		int _iTargetSize;
		bool _bCheckDataAsPrice;
		virtual void AddChild(T)=0;
		virtual void AddChild(T,T)=0;
		virtual void AddChild(T,T,T)=0;
		virtual void AddChild(T,T,T,T)=0;
		virtual void AddChild(T,T,T,T,T)=0;

	public:
		virtual bool WarmedUp() const =0;
		virtual void Reset(int,bool)=0;

		void Add(T);
		void Add(T,T);
		void Add(T,T,T);
		void Add(T,T,T,T);
		void Add(T,T,T,T,T);
};

template <typename T>
void DataQue<T>::AddChild(T d1) {}
template <typename T>
void DataQue<T>::AddChild(T d1, T d2) {}
template <typename T>
void DataQue<T>::AddChild(T d1, T d2, T d3) {}
template <typename T>
void DataQue<T>::AddChild(T d1, T d2, T d3, T d4) {}
template <typename T>
void DataQue<T>::AddChild(T d1, T d2, T d3, T d4, T d5) {}




template <typename T>
void DataQue<T>::Add(T d1)
{
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d1)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d1))) return;
	AddChild(d1);
}
template <typename T>
void DataQue<T>::Add(T d1, T d2)
{
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d1)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d1))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d2)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d2))) return;
	AddChild(d1,d2);
}
template <typename T>
void DataQue<T>::Add(T d1, T d2, T d3)
{
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d1)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d1))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d2)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d2))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d3)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d3))) return;
	AddChild(d1,d2,d3);
}
template <typename T>
void DataQue<T>::Add(T d1, T d2, T d3, T d4)
{
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d1)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d1))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d2)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d2))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d3)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d3))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d4)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d4))) return;
	AddChild(d1,d2,d3,d4);
}
template <typename T>
void DataQue<T>::Add(T d1, T d2, T d3, T d4, T d5)
{
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d1)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d1))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d2)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d2))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d3)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d3))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d4)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d4))) return;
	if ((_bCheckDataAsPrice && !ValidityChk::Price(d5)) ||
			(!_bCheckDataAsPrice && !ValidityChk::GeneralNumber(d5))) return;
	AddChild(d1,d2,d3,d4,d5);
}

#endif
