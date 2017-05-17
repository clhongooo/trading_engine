#ifndef SGNLPRC_HPP
#define SGNLPRC_HPP

#include "../corelib/STool.hpp"
#include "Ema.hpp"
#include "CirDataQue.hpp"
#include "HighLow.hpp"
#include "DataQue.hpp"
#include <deque>

class SgnlPrc : public DataQue<double>
{
	private:
		//Prices
		deque<double> dqHistM;
		Ema<double> ema10_M;
		Ema<double> ema2_M;
		Ema<double> emaPA_M;
		deque<double> dq_ema10_M;

		//For calculating oscillation period
		deque<double> dq_fastdetrender;
		deque<double> dq_diff2_fdtdr;
		deque<double> dq_diff2_px;

		deque<double> dqTaylor;
		deque<double> dqDetrender;
		deque<double> dqjI;
		deque<double> dqjQ;
		deque<double> dqI1;
		deque<double> dqQ1;
		deque<double> dqI2;
		deque<double> dqQ2;
		deque<double> dqI3;
		deque<double> dqQ3;
		deque<double> Re;
		deque<double> Im;


		//Oscillation Period
		static const double dInitGuessPeriod;
		deque<double> dqPeriod_Guess;
		deque<double> dqPeriod_DD;
		deque<double> dqPeriod_HD;
		deque<double> dqPeriod_HD_DD;

		deque<double> dqSignal;
		Ema<double> emaNoise;
		deque<double> dqSNR;

		deque<double> dqDCPhase;
		deque<double> dqSineWaveI;
		deque<double> dqSineWaveILead;
		deque<double> dqSSineWave;

		deque<double> dqPASMA;


		//One-bar Lag Moving Average
		deque<double> dq_1bl_wma_431;
		deque<double> dq_1bl_wma_42;
		deque<double> dq_1bl_taylor;

		//Trend mode or Cycle Mode
		int iBarsAftSinCross;
		bool bTrendCycleMode; //True=Trend, False=Cycle


		void Reset(int,bool) {}

		void AddChild(double) {}
		void AddChild(double,double) {}
		void AddChild(double dH, double dL, double dC);
		void AddChild(double,double,double,double) {}
		void AddChild(double,double,double,double,double) {}

		void Cal_S_D_I_Q();
		void CalGuessPeriod();
		void CalDualDifferentiator();
		void CalHomodyneDiscriminator();
		void CalAvgSmoothPeriod();
		void CalSineWaveIndicator();
		void CalSNR();
		void CalPASMA();
		void CalOneBarLagMA();
		void DetTrendCycleMode();
		double GetLatestPeriod();


	public:
		SgnlPrc();
		bool WarmedUp() const;
		double Period_HD_DD();
		double Period_HD();
		double Period_DD();
		double Period_Guess();
		double SNR();
		double SineWaveI();
		double SineWaveILead();
		double PASMA();
		double CentreOsc();
		double OBLMA();
		bool TrendCycleMode();
};

const double SgnlPrc::dInitGuessPeriod = 20;




SgnlPrc::SgnlPrc()
{
	this->_iTargetSize = 0;
	this->_bCheckDataAsPrice = true;

	ema10_M.Reset(0.1,true);
	ema2_M.Reset(0.5,true);
	emaNoise.Reset(0.1,true);
	emaPA_M.Reset((double)1/SgnlPrc::dInitGuessPeriod,true);
	dqPeriod_Guess.push_back(SgnlPrc::dInitGuessPeriod);
	iBarsAftSinCross = 0;
	bTrendCycleMode = true;


}

bool SgnlPrc::WarmedUp() const
{
	if (!dqPASMA.empty()) return true; else return false;
}

double SgnlPrc::Period_HD()
{ if (!dqPeriod_HD.empty()) return dqPeriod_HD.back(); else return dqPeriod_Guess.back(); }

double SgnlPrc::Period_DD()
{ if (!dqPeriod_DD.empty()) return dqPeriod_DD.back(); else return dqPeriod_Guess.back(); }

double SgnlPrc::Period_Guess()
{ if (!dqPeriod_Guess.empty()) return dqPeriod_Guess.back(); }

double SgnlPrc::Period_HD_DD()
{ if (!dqPeriod_HD_DD.empty()) return dqPeriod_HD_DD.back(); else return dqPeriod_Guess.back(); }

double SgnlPrc::SNR()
{ if (!dqSNR.empty()) return dqSNR.back(); }

double SgnlPrc::SineWaveI()
{ if (!dqSineWaveI.empty()) return dqSineWaveI.back(); }

double SgnlPrc::SineWaveILead()
{ if (!dqSineWaveILead.empty()) return dqSineWaveILead.back(); }

double SgnlPrc::PASMA()
{ if (!dqPASMA.empty()) return dqPASMA.back(); }

double SgnlPrc::OBLMA()
{ if (!dq_1bl_wma_42.empty()) return dq_1bl_wma_42.back(); }

bool SgnlPrc::TrendCycleMode()
{ return bTrendCycleMode; }


void SgnlPrc::AddChild(double dH, double dL, double dC)
{
	double dM = (dH+dL+dC) / 3;

	emaNoise.Add((dH-dL)*(dH-dL)*0.25);

	dqHistM.push_back(dM);
	ema2_M.Add(dqHistM.back());
	emaPA_M.Add(dqHistM.back());
	if (!ema2_M.WarmedUp() || !emaPA_M.WarmedUp()) return;
	dq_fastdetrender.push_back(ema2_M.Value() - emaPA_M.Value());


	if (dqHistM.size() >= 3)
		dq_diff2_px.push_back(
				+ dqHistM[dqHistM.size()-1]
				- 2*dqHistM[dqHistM.size()-2]
				+ dqHistM[dqHistM.size()-3]
				);

	//Calculations...
	Cal_S_D_I_Q();
	CalGuessPeriod();
	CalDualDifferentiator();
	CalHomodyneDiscriminator();
	CalAvgSmoothPeriod();
	CalPASMA(); //dependencies: period
	CalSNR(); //dependencies: period, dq_ema10_M (Cal_S_D_I_Q)
	CalSineWaveIndicator(); //dependencies: period, dqDetrender
	CalOneBarLagMA();
	DetTrendCycleMode(); //dependencies: period, sine wave
}


void SgnlPrc::CalGuessPeriod()
{
	if (dqHistM.empty()) return;

	if (dq_fastdetrender.size() < 3) return;
	dq_diff2_fdtdr.push_back(
			dq_fastdetrender[dq_fastdetrender.size()-1]
			-
			2*dq_fastdetrender[dq_fastdetrender.size()-2]
			+
			dq_fastdetrender[dq_fastdetrender.size()-3]
			);

	double dPeriodGuess = IParam::MAGIC_NUM;
	if (dq_fastdetrender[dq_fastdetrender.size()-2] * dq_diff2_fdtdr.back() < 0)
		dPeriodGuess =
			sqrt(-dq_fastdetrender[dq_fastdetrender.size()-2] / dq_diff2_fdtdr.back())
			* 2 * M_PI;

	if (dPeriodGuess != IParam::MAGIC_NUM)
	{
		if (dPeriodGuess > 1.6 * dqPeriod_Guess.back())
			dPeriodGuess = 1.6 * dqPeriod_Guess.back();
		else if (dPeriodGuess < 0.5 * dqPeriod_Guess.back())
			dPeriodGuess = 0.5 * dqPeriod_Guess.back();

		dqPeriod_Guess.push_back(
				0.3 * dPeriodGuess + 0.7 * dqPeriod_Guess.back());
	}

}

void SgnlPrc::Cal_S_D_I_Q()
{
	double dLatestPeriod;
	if (dqPeriod_HD_DD.empty()) dLatestPeriod = dqPeriod_Guess.back();
	else dLatestPeriod = dqPeriod_HD_DD.back();

	//Sunny's implementation
	//Smooth historical mid-prices using EMA
	ema10_M.Add(dqHistM.back());
	if (!ema10_M.WarmedUp()) return;
	dq_ema10_M.push_back(ema10_M.Value());

	//Taylor series prediction
	if (dq_ema10_M.size() < 3) return;
	dqTaylor.push_back(
			dq_ema10_M[dq_ema10_M.size()-2]
			+ 
			(dq_ema10_M[dq_ema10_M.size()-1] - dq_ema10_M[dq_ema10_M.size()-3]) / 2
			+
			0.5 *
			(dq_ema10_M[dq_ema10_M.size()-1]
			 - 2 * dq_ema10_M[dq_ema10_M.size()-2]
			 + dq_ema10_M[dq_ema10_M.size()-3])
			);

	//Detrend using Hilbert filter
	if (dqTaylor.size() < 7) return;
	double dDetrender =
		(+ 0.0962 * dqTaylor[dqTaylor.size()-1]
		 + 0.5769 * dqTaylor[dqTaylor.size()-3]
		 - 0.5769 * dqTaylor[dqTaylor.size()-5]
		 - 0.0962 * dqTaylor[dqTaylor.size()-7]) *
		(0.075 * dLatestPeriod + 0.54);
	dqDetrender.push_back(dDetrender);

	//Pay particular attention to the order of calculation
	//because we are not using circular array

	//I1
	if (dqDetrender.size() < 4) return;
	dqI1.push_back(dqDetrender[dqDetrender.size()-4]);

	//Q1
	if (dqDetrender.size() < 7) return;
	double dQ1 =
		(+ 0.0962 * dqDetrender[dqDetrender.size()-1]
		 + 0.5769 * dqDetrender[dqDetrender.size()-3]
		 - 0.5769 * dqDetrender[dqDetrender.size()-5]
		 - 0.0962 * dqDetrender[dqDetrender.size()-7]) *
		(0.075 * dLatestPeriod + 0.54);
	dqQ1.push_back(dQ1);

	//jI
	if (dqI1.size() < 7) return;
	double djI =
		(+ 0.0962 * dqI1[dqI1.size()-1]
		 + 0.5769 * dqI1[dqI1.size()-3]
		 - 0.5769 * dqI1[dqI1.size()-5]
		 - 0.0962 * dqI1[dqI1.size()-7]) *
		(0.075 * dLatestPeriod + 0.54);
	dqjI.push_back(djI);

	//Q2
	dqQ2.push_back(dqQ1.back() + dqjI.back());

	//jQ
	if (dqQ1.size() < 7) return;
	double djQ =
		(+ 0.0962 * dqQ1[dqQ1.size()-1]
		 + 0.5769 * dqQ1[dqQ1.size()-3]
		 - 0.5769 * dqQ1[dqQ1.size()-5]
		 - 0.0962 * dqQ1[dqQ1.size()-7]) *
		(0.075 * dLatestPeriod + 0.54);
	dqjQ.push_back(djQ);

	//I2
	dqI2.push_back(dqI1.back() - dqjQ.back());
}


void SgnlPrc::CalDualDifferentiator()
{
	if (dqI2.size() < 2) return;
	if (dqQ2.size() < 2) return;

	double dValue1 =
		dqQ2.back() *
		(dqI2[dqI2.size()-1] - dqI2[dqI2.size()-2])
		-
		dqI2.back() *
		(dqQ2[dqQ2.size()-1] - dqQ2[dqQ2.size()-2]);

	dValue1 = abs(dValue1);

	double dPeriodDD = IParam::MAGIC_NUM;
	if (dValue1 > 0.01)
	{
		dPeriodDD = (2 * M_PI / dValue1 *
				(pow(dqI2.back(),2) + pow(dqQ2.back(),2)));
	} else return;

	double dLastPeriodDD;
	if (dqPeriod_DD.empty()) dLastPeriodDD = dqPeriod_Guess.back();
	else dLastPeriodDD = dqPeriod_DD.back();

	if (dPeriodDD > 1.5 * dLastPeriodDD) dPeriodDD = 1.5 * dLastPeriodDD;
	else if (dPeriodDD < 0.67 * dLastPeriodDD) dPeriodDD = 0.67 * dLastPeriodDD;

	if (dPeriodDD > 50) dPeriodDD = 50;
	else if (dPeriodDD < 6) dPeriodDD = 6;

	dqPeriod_DD.push_back(0.3 * dPeriodDD + 0.7 * dLastPeriodDD);
}



void SgnlPrc::CalHomodyneDiscriminator()
{
	if (dqI2.size() < 2) return;
	if (dqQ2.size() < 2) return;
	Re.push_back(
			dqI2[dqI2.size()-1] * dqI2[dqI2.size()-2] +
			dqQ2[dqQ2.size()-1] * dqQ2[dqQ2.size()-2]
			);

	Im.push_back(
			dqI2[dqI2.size()-1] * dqQ2[dqQ2.size()-2] -
			dqQ2[dqQ2.size()-1] * dqI2[dqI2.size()-2]
			);

	if (Re.size() < 2) return;
	if (Im.size() < 2) return;
	Re[Re.size()-1] = 0.2 * Re[Re.size()-1] + 0.8 * Re[Re.size()-2];
	Im[Im.size()-1] = 0.2 * Im[Im.size()-1] + 0.8 * Im[Im.size()-2];


	double dPeriodHD = IParam::MAGIC_NUM;
	if (Im.back() != 0 && Re.back() != 0)
		dPeriodHD = 2 * M_PI / (atan(Im.back() / Re.back()));
	else return;

	double dLastPeriodHD;
	if (dqPeriod_HD.empty()) dLastPeriodHD = dqPeriod_Guess.back();
	else dLastPeriodHD = dqPeriod_HD.back();

	if (dPeriodHD > 1.5 * dLastPeriodHD) dPeriodHD = 1.5 * dLastPeriodHD;
	else if (dPeriodHD < 0.67 * dLastPeriodHD) dPeriodHD = 0.67 * dLastPeriodHD;

	if (dPeriodHD > 50) dPeriodHD = 50;
	else if (dPeriodHD < 6) dPeriodHD = 6;

	dqPeriod_HD.push_back(0.3 * dPeriodHD + 0.7 * dLastPeriodHD);
}

void SgnlPrc::CalAvgSmoothPeriod()
{
	if (dqPeriod_DD.empty()) return;
	if (dqPeriod_HD.empty()) return;

	double dLastPeriod;
	if (dqPeriod_HD_DD.empty()) dLastPeriod = dqPeriod_Guess.back();
	else dLastPeriod = dqPeriod_HD_DD.back();

	double dThisPeriod = (dqPeriod_DD.back() * 3 + dqPeriod_HD.back() * 2) / 5;

	dqPeriod_HD_DD.push_back(0.33 * dThisPeriod + 0.67 * dLastPeriod);
}

void SgnlPrc::CalSineWaveIndicator()
{
	//Calculate Dominant Cycle Phase
	double dDCPeriod = GetLatestPeriod();

	double iDCPeriod = (int)(dDCPeriod);
	double dRealPart = 0;
	double dImagPart = 0;
	double dDCPhase = 0;

	if (dqDetrender.size() < iDCPeriod+1) return;

	for (int i = 0; i < (int)iDCPeriod - 1; ++i)
	{
		//must use the detrended signal, not the original price
		dRealPart += cos(2 * M_PI * i / dDCPeriod) * dqDetrender[dqDetrender.size()-1-i];
		dImagPart += sin(2 * M_PI * i / dDCPeriod) * dqDetrender[dqDetrender.size()-1-i];
	}

	//atan2 has to be used to resolve quadrant ambiguity
	if (abs(dRealPart) > 0.001) dDCPhase = atan2(dImagPart,dRealPart);
	else dDCPhase = M_PI / 2 * STool::Sign(dImagPart);

	dDCPhase += (M_PI / 2);

	//seems no need now (Compensate for one bar lag of the Weighted Moving Average)
	//dDCPhase += (2 * M_PI / dDCPeriod);

	//seems the signal is a bit too early
	dDCPhase -= (2 * M_PI / dDCPeriod * 1.6); //emphically tested with perfect sine wave

	//enabling this will kill the indicator: if (dImagPart < 0) dDCPhase += M_PI;
	if (dDCPhase > 315 * M_PI / 180) dDCPhase -= 2 * M_PI;
	//while (dDCPhase > 2 * M_PI) dDCPhase -= (2 * M_PI);

	dqSineWaveI.push_back(sin(dDCPhase));
	dqSineWaveILead.push_back(sin(dDCPhase + (M_PI / 4)));

	dqDCPhase.push_back(dDCPhase);
}


double SgnlPrc::GetLatestPeriod()
{
	double dRtn = IParam::MAGIC_NUM;

	if (!dqPeriod_HD_DD.empty()) dRtn = dqPeriod_HD_DD.back();
	else if (!dqPeriod_Guess.empty()) dRtn = dqPeriod_Guess.back();

	return (dRtn > 6 ? dRtn : 6);
}

void SgnlPrc::CalSNR()
{
	double dPeriod = GetLatestPeriod();

	if (dq_ema10_M.size() < 2) return;

	double dQ3 =
		0.5
		* (dq_ema10_M.back() - dq_ema10_M[dq_ema10_M.size()-2])
		* (0.1759 * dPeriod + 0.4607);
	dqQ3.push_back(dQ3);

	double dI3 = 0;

	if (dqQ3.size() < dPeriod+1) return;
	for (int i = 0; i < (int)(dPeriod / 2) - 1; ++i)
		dI3 += dqQ3[dqQ3.size()-1-i];

	dI3 = 1.57 * dI3 / (dPeriod / 2);

	/*
		 double dValue1 = 0;

		 for (int i = 0; i < (int)(dPeriod / 4) - 1; ++i)
		 dValue1 += dqQ3[dqQ3.size()-1-i];

		 dValue1 = 1.25*dValue1 / (dPeriod/4);
	 */


	dqSignal.push_back(
			pow(dI3,2) + pow(dqQ3.back(),2)
			);

	double dSNR = IParam::MAGIC_NUM;
	if (!emaNoise.WarmedUp() || dqSignal.empty()) return;
	if (emaNoise.Value() != 0 && dqSignal.back() != 0)
		dSNR = 10 * log10(dqSignal.back() / emaNoise.Value());

	if (dSNR != IParam::MAGIC_NUM)
	{
		if (dqSNR.empty()) dqSNR.push_back(dSNR);
		else dqSNR.push_back(0.3 * dSNR + 0.7 * dqSNR.back());
	}
}
void SgnlPrc::CalPASMA()
{
	double dPeriod = GetLatestPeriod();
	int iPeriod = (int)dPeriod;

	if (dqHistM.size() < iPeriod+1) return;

	double dPASMA = 0;
	deque<double>::iterator it=dqHistM.end();
	int iCnt = 0;
	while (iCnt < iPeriod)
	{
		it--;
		dPASMA += *it;
		++iCnt;
	}
	dqPASMA.push_back(dPASMA / dPeriod);
}



void SgnlPrc::CalOneBarLagMA()
{
	if (dqHistM.size() < 3) return;

	dq_1bl_wma_431.push_back(
			(+ 4 * dqHistM[dqHistM.size()-1]
			 + 3 * dqHistM[dqHistM.size()-2]
			 + 1 * dqHistM[dqHistM.size()-3]) / 8
			);

	if (dq_ema10_M.size() < 3) return;

	dq_1bl_taylor.push_back(
			dq_1bl_wma_431.back()
			+ 1.1 * (dq_ema10_M.back() - dq_ema10_M[dq_ema10_M.size()-2])
			+ 0.15 * (dq_ema10_M.back() - 2*dq_ema10_M[dq_ema10_M.size()-2] + dq_ema10_M[dq_ema10_M.size()-3] )
			);

	if (dq_1bl_taylor.size() < 2) return;

	dq_1bl_wma_42.push_back(
			(+ 4 * dq_1bl_taylor[dq_1bl_taylor.size()-1]
			 + 2 * dq_1bl_taylor[dq_1bl_taylor.size()-2]) / 6
			);

}


void SgnlPrc::DetTrendCycleMode()
{
	if (dqSineWaveI.size() < 2 || dqSineWaveILead.size() < 2 ||
			dqDCPhase.size() < 2) return;
	double dPeriod = GetLatestPeriod();

	if (
			(dqSineWaveILead.back() > dqSineWaveI.back() &&
			 dqSineWaveILead[dqSineWaveILead.size()-2] < dqSineWaveI[dqSineWaveI.size()-2])
			||
			(dqSineWaveILead.back() < dqSineWaveI.back() &&
			 dqSineWaveILead[dqSineWaveILead.size()-2] > dqSineWaveI[dqSineWaveI.size()-2])
		 )
		iBarsAftSinCross = 0;

	++iBarsAftSinCross;

	if (iBarsAftSinCross < 0.5 * dPeriod) bTrendCycleMode = false;
	else bTrendCycleMode = true;

	if (dqDCPhase.back() - dqDCPhase[dqDCPhase.size()-2] > 0.67*360/dPeriod &&
			dqDCPhase.back() - dqDCPhase[dqDCPhase.size()-2] < 1.5*360/dPeriod)
		bTrendCycleMode = false;



}






#endif
