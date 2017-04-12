#ifndef FUNDLMDLPE_HPP
#define FUNDLMDLPE_HPP

#include "PCH.h"
#include "Constants.h"
#include "../Util/STool.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_rng.h"

using namespace std;

class FundlMdlPE
{
	private:
		const gsl_rng_type * T;
		gsl_rng * r;

		int _dTime;
		double _dEPS;
		double _dNmlzPE;
		double _dHSI;

	public:
		static const double _EPS_k;
		static const double _EPS_SD;

		static const double _PE_MEAN;
		static const double _PE_k;
		static const double _PE_SD;

		static const double _E;

		static const double _EXP_EPS1;
		static const double _EXP_EPS2;
		static const double _EXP_EPS3;

		static const int _dInitTime;
		static const double _dInitEPS;
		static const double _dInitNmlzPE;
		static const double _dInitHSI;

		static const double _HSI_LIM_MTH;
		static const double _HSI_LIM_DAY;

		FundlMdlPE();
		void Reset();
		void NextMonth();
		double GetTime() { return _dTime; }
		double GetEPS() { return _dEPS; }
		double GetNmlzPE() { return _dNmlzPE; }
		double GetHSI() { return _dHSI; }
};

#endif


