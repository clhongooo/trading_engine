#ifndef SNYSTAT_HPP
#define SNYSTAT_HPP

#include "../corelib/IParam.hpp"
#include "../corelib/SException.hpp"
#include "../smath/DelayedCirDataQue.hpp"
#include "gsl/gsl_cdf.h"
#include "gsl/gsl_fit.h"
#include "gsl/gsl_statistics.h"
#include <cmath>
#include <vector>

using namespace std;

class SNYStat
{
	private:
		bool _bCheckDataAsPrice;
		int _iTargetSize;
		vector<DelayedCirDataQue<double> > _vDCDQ;

		void ResetCache(int);
		void Clear1QueCache(int);
		vector<double> _Cached_AMean;
		vector<double> _Cached_SD;
		vector<double> _Cached_Variance;
		vector<double> _Cached_SemiVar;
		vector<double> _Cached_SemiVar0;
		vector<double> _Cached_Skew;
		vector<double> _Cached_Kurt;
		vector<double> _Cached_Slope;

	public:
		SNYStat(); //simple version
		SNYStat(int,bool); //simple version
		SNYStat(int,vector<int>,int,int,bool); //regular version
		void Reset(int,bool); //simple version
		void Reset(int,vector<int>,int,int,bool);
		bool WarmedUp() const;
		void Add(int,double);
		int Count(int);

		double AMean(int);
		double SD(int);
		double Variance(int);
		double SemiVar(int);
		double SemiVar0(int);
		double Skew(int);
		double Kurt(int);
		bool Correl(int,int,double &);
};





//simple versions
SNYStat::SNYStat()
{
	vector<int> v; v.push_back(0);
	Reset(1, v, 10, 11, true);
}
SNYStat::SNYStat(int iTargetSize, bool bCheckDataAsPrice)
{
	vector<int> v; v.push_back(0);
	Reset(1, v, iTargetSize, iTargetSize+1, bCheckDataAsPrice);
}
void SNYStat::Reset(int iTargetSize, bool bCheckDataAsPrice)
{
	vector<int> v; v.push_back(0);
	Reset(1, v, iTargetSize, iTargetSize+1, bCheckDataAsPrice);
}

//regular version
SNYStat::SNYStat(int iNumQue, vector<int> viDelay, int iTargetSize, int iOpulentSize, bool bCheckDataAsPrice)
{
	Reset(iNumQue, viDelay, iTargetSize, iOpulentSize, bCheckDataAsPrice);
}

void SNYStat::Reset(int iNumQue, vector<int> viDelay, int iTargetSize, int iOpulentSize, bool bCheckDataAsPrice)
{
	if (iNumQue <= 0 || iTargetSize <= 0 || viDelay.size() < iNumQue)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	_iTargetSize = iTargetSize;
	_vDCDQ.clear();
	_vDCDQ.reserve(iTargetSize);

	for (int i = 0; i < iNumQue; ++i)
	{
			DelayedCirDataQue<double> dcdq;
			_vDCDQ.push_back(dcdq);
	}

	for (int i = 0; i < iNumQue; ++i)
		_vDCDQ[i].Reset(iTargetSize,iOpulentSize,viDelay[i],bCheckDataAsPrice);

	ResetCache(iNumQue);
}

void SNYStat::ResetCache(int iQ)
{
	_Cached_AMean.clear();
	_Cached_SD.clear();
	_Cached_Variance.clear();
	_Cached_SemiVar.clear();
	_Cached_SemiVar0.clear();
	_Cached_Skew.clear();
	_Cached_Kurt.clear();
	_Cached_Slope.clear();
	
	_Cached_AMean.reserve(iQ);
	_Cached_SD.reserve(iQ);
	_Cached_Variance.reserve(iQ);
	_Cached_SemiVar.reserve(iQ);
	_Cached_SemiVar0.reserve(iQ);
	_Cached_Skew.reserve(iQ);
	_Cached_Kurt.reserve(iQ);
	_Cached_Slope.reserve(iQ);
	
	for (int i = 0; i < iQ; ++i)
	{
		_Cached_AMean.push_back(IParam::MAGIC_NUM);
		_Cached_SD.push_back(IParam::MAGIC_NUM);
		_Cached_Variance.push_back(IParam::MAGIC_NUM);
		_Cached_SemiVar.push_back(IParam::MAGIC_NUM);
		_Cached_SemiVar0.push_back(IParam::MAGIC_NUM);
		_Cached_Skew.push_back(IParam::MAGIC_NUM);
		_Cached_Kurt.push_back(IParam::MAGIC_NUM);
		_Cached_Slope.push_back(IParam::MAGIC_NUM);
	}
}

void SNYStat::Clear1QueCache(int iQ)
{
	_Cached_AMean[iQ] = IParam::MAGIC_NUM;
	_Cached_SD[iQ] = IParam::MAGIC_NUM;
	_Cached_Variance[iQ] = IParam::MAGIC_NUM;
	_Cached_SemiVar[iQ] = IParam::MAGIC_NUM;
	_Cached_SemiVar0[iQ] = IParam::MAGIC_NUM;
	_Cached_Skew[iQ] = IParam::MAGIC_NUM;
	_Cached_Kurt[iQ] = IParam::MAGIC_NUM;
	_Cached_Slope[iQ] = IParam::MAGIC_NUM;
}

bool SNYStat::WarmedUp() const
{
	bool bR = true;
	for (int i = 0; i < _vDCDQ.size(); ++i)
		if (!_vDCDQ[i].WarmedUp()) bR = false;	
	return bR;
}

void SNYStat::Add(int iQ, double dP)
{
	if (iQ < 0 || iQ+1 > _vDCDQ.size())
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	_vDCDQ[iQ].Add(dP);

	Clear1QueCache(iQ);
}

int SNYStat::Count(int iQ)
{
	return _vDCDQ[iQ].Count();
}

double SNYStat::AMean(int iQ)
{
	if (_Cached_AMean[iQ] == IParam::MAGIC_NUM)
	{
		const double *dA = _vDCDQ[iQ].GetArrayInInsOrd();
		_Cached_AMean[iQ] = gsl_stats_mean(dA, 1, _iTargetSize);
		return _Cached_AMean[iQ];
	}
	else return _Cached_AMean[iQ];
}

double SNYStat::SD(int iQ)
{
	if (_Cached_SD[iQ] == IParam::MAGIC_NUM)
	{
		const double *dA = _vDCDQ[iQ].GetArrayInInsOrd();
		if (_Cached_AMean[iQ] == IParam::MAGIC_NUM) AMean(iQ);
		_Cached_SD[iQ] = gsl_stats_sd_m(dA, 1, _iTargetSize, _Cached_AMean[iQ]);
		return _Cached_SD[iQ];
	}
	else return _Cached_SD[iQ];
}

double SNYStat::Variance(int iQ)
{
	if (_Cached_Variance[iQ] == IParam::MAGIC_NUM)
	{
		const double *dA = _vDCDQ[iQ].GetArrayInInsOrd();
		if (_Cached_AMean[iQ] == IParam::MAGIC_NUM) AMean(iQ);
		_Cached_Variance[iQ] = gsl_stats_variance_m(dA, 1, _iTargetSize, _Cached_AMean[iQ]);
		return _Cached_Variance[iQ];
	}
	else return _Cached_Variance[iQ];
}

double SNYStat::Skew(int iQ)
{
	if (_Cached_Skew[iQ] == IParam::MAGIC_NUM)
	{
		const double *dA = _vDCDQ[iQ].GetArrayInInsOrd();
		if (_Cached_AMean[iQ] == IParam::MAGIC_NUM) AMean(iQ);
		if (_Cached_SD[iQ] == IParam::MAGIC_NUM) SD(iQ);
		_Cached_Skew[iQ] = gsl_stats_skew_m_sd(dA, 1, _iTargetSize, _Cached_AMean[iQ], _Cached_SD[iQ]);
		return _Cached_Skew[iQ];
	}
	else return _Cached_Skew[iQ];
}

double SNYStat::Kurt(int iQ)
{
	if (_Cached_Kurt[iQ] == IParam::MAGIC_NUM)
	{
		const double *dA = _vDCDQ[iQ].GetArrayInInsOrd();
		if (_Cached_AMean[iQ] == IParam::MAGIC_NUM) AMean(iQ);
		if (_Cached_SD[iQ] == IParam::MAGIC_NUM) SD(iQ);
		_Cached_Kurt[iQ] = gsl_stats_kurtosis_m_sd(dA, 1, _iTargetSize, _Cached_AMean[iQ], _Cached_SD[iQ]);
		return _Cached_Kurt[iQ];
	}
	else return _Cached_Kurt[iQ];
}

double SNYStat::SemiVar(int iQ)
{
	if (_Cached_SemiVar[iQ] == IParam::MAGIC_NUM)
	{
		const double *dA = _vDCDQ[iQ].GetArrayInInsOrd();

		if (_Cached_AMean[iQ] == IParam::MAGIC_NUM) AMean(iQ);

		//not available in gsl
		double dSum = 0;

		for (int i = 0; i < _iTargetSize; ++i)
			if (dA[i] < _Cached_AMean[iQ])
				dSum += pow(dA[i]-_Cached_AMean[iQ], 2);

		dSum /= (double)_iTargetSize;
		_Cached_SemiVar[iQ] = dSum;

		return _Cached_SemiVar[iQ];
	}
	else return _Cached_SemiVar[iQ];
}

double SNYStat::SemiVar0(int iQ)
{
	if (_Cached_SemiVar0[iQ] == IParam::MAGIC_NUM)
	{
		const double *dA = _vDCDQ[iQ].GetArrayInInsOrd();

		//not available in gsl
		double dSum = 0;

		for (int i = 0; i < _iTargetSize; ++i)
			if (dA[i] < 0) dSum += pow(dA[i], 2);

		dSum /= (double)_iTargetSize;
		if (dSum == 0) dSum = 0.0001;
		_Cached_SemiVar0[iQ] = dSum;

		return _Cached_SemiVar0[iQ];
	}
	else return _Cached_SemiVar0[iQ];
}


bool SNYStat::Correl(int iQ1, int iQ2, double & dCorrel)
{
	if (iQ1 < 0 || iQ2 < 0 || iQ1+1 > _vDCDQ.size() || iQ2+1 > _vDCDQ.size())
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	if (!_vDCDQ[iQ1].WarmedUp() || !_vDCDQ[iQ2].WarmedUp()) return false;
	else
	{
		const double *dA = _vDCDQ[iQ1].GetArrayInInsOrd();
		const double *dB = _vDCDQ[iQ2].GetArrayInInsOrd();

		//can't locate gsl_stats_correlation, therefore implement myself
		if (_Cached_AMean[iQ1] == IParam::MAGIC_NUM) AMean(iQ1);
		if (_Cached_AMean[iQ2] == IParam::MAGIC_NUM) AMean(iQ2);
		if (_Cached_SD[iQ1] == IParam::MAGIC_NUM) SD(iQ1);
		if (_Cached_SD[iQ2] == IParam::MAGIC_NUM) SD(iQ2);

		dCorrel = gsl_stats_covariance_m(dA,1,dB,1,_iTargetSize,
										_Cached_AMean[iQ1],_Cached_AMean[iQ2]);

		dCorrel = dCorrel / _Cached_SD[iQ1] / _Cached_SD[iQ2];

		return true;
	}
}








#endif
