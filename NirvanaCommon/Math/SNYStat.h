#ifndef SNYSTAT_HPP
#define SNYSTAT_HPP

#include "PCH.h"
#include "Constants.h"
#include "SException.h"
#include "DelayedCirDataQue.hpp"
#include "gsl/gsl_cdf.h"
#include "gsl/gsl_fit.h"
#include "gsl/gsl_statistics.h"

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
		bool Ready() const;
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









#endif
