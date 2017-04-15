#ifndef SGNLPRC_HPP
#define SGNLPRC_HPP

#include "PCH.h"
#include "Constants.h"
#include "STool.h"
#include "Ema.hpp"
#include "CirDataQue.hpp"
#include "HighLow.hpp"
#include "DataQue.hpp"

class SgnlPrc : public DataQue<double>
{
	public:
		SgnlPrc();
		virtual ~SgnlPrc() {}
		bool Ready() const;
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


};

#endif
