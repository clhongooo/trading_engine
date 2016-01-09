#ifndef LINREGR_HPP
#define LINREGR_HPP

#include "PCH.h"
#include "Constants.h"
#include "../Util/SException.h"
#include "gsl/gsl_statistics.h"
#include "CirDataQue.hpp"

using namespace std;

class LinRegr
{
	private:
		int _iTargetSize;
    long _lXCnt;
		bool _bCheckDataAsPrice;
		CirDataQue<double> _cdqY;
		CirDataQue<double> _cdqX;

	public:
		LinRegr();
		LinRegr(int,bool);
		void AddYX(const double,const double);
		void AddY(const double);
		void Reset(int,bool);
		bool Ready() const;
		double Value() const;
};

#endif
