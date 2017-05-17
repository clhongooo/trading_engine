#include "../smath/Dynamics.hpp"
#include "../smath/SNYStat.hpp"
#include "../corelib/STool.hpp"
#include "../corelib/Acct.hpp"
#include <deque>

using namespace std;


double GetInvmPctg(double dDest, double dCur, double dCurv)
{
	if (dDest < dCur) return 0;
	else return pow((dDest - dCur) / dDest, dCurv);
}


int main(int argc, char *argv[])
{
	Acct ac;

	enum Strategy
	{
		DOLLAR_COST_AVG,
		INV_BY_VALUE
	};


	// ----------------------------------------
	// INPUT
	//Strategy stgy = DOLLAR_COST_AVG;
	Strategy stgy = INV_BY_VALUE;
	const bool PRINT_DETAILS1 = false;
	const bool PRINT_DETAILS2 = false;
	const bool PRINT_DETAILS3 = true;
	// ----------------------------------------
	const int NO_OF_DAYS = 500;
	const double INIT_Asset = 21.6; //2800.HK 2013-06
	//const double _DEST_Asset = 30; //2800.HK 2015-06
	const double _DEST_Asset = 33; //2800.HK 2016-06
	//const double _DEST_Asset = 30; //2800.HK 2016-06 conservative
	// ----------------------------------------
	//const int _M = 750;
	const int _M = 1000;
	const double _V = 0.3/sqrt(250); //volatility
	const double _MAXUPPCTGCHG = 0.03;
	const double _MAXDWPCTGCHG = 0.05;
	const int MONTE_CARLO_RUNS = 50000;
	const double PRINCIPAL = 800000;
	const double MINREQCPNL = 150000;
	// ----------------------------------------
	double INV_BY_VAL_CURV;
	double INV_BY_VAL_CURV_START = 2.8; //2.8
	double INV_BY_VAL_CURV_END = 2.8;
	double INV_BY_VAL_CURV_INC = 0.1;
	double LEVERAGE;
	double LEVERAGE_START = 6.8; //6.8
	double LEVERAGE_END = 6.8;
	double LEVERAGE_INC = 0.2;
	// ----------------------------------------




	// Init once only - otherwise the seed will be the same
	Dynamics dyn;

	INV_BY_VAL_CURV = INV_BY_VAL_CURV_START;
	do
	{
		LEVERAGE = LEVERAGE_START;
		do
		{
			// LOGIC
			int iTimesBroke = 0;
			double dInmAmt = 0;

			if (stgy == DOLLAR_COST_AVG) dInmAmt = (double) PRINCIPAL / NO_OF_DAYS * LEVERAGE;

			double dTotUtil = 0;
			deque<double> dqCPnL;

			for (int i=0; i<MONTE_CARLO_RUNS; i++)
			{
				ac.Reset();
				dyn.SetDynamicsA(_DEST_Asset,INIT_Asset,_M,_V,_MAXUPPCTGCHG,_MAXDWPCTGCHG);

				SNYStat sny(NO_OF_DAYS,false);
				double dLastCPnL = 0;
				double dAsset = INIT_Asset;
				for (int iTimeCnt=0; iTimeCnt<NO_OF_DAYS; iTimeCnt++)
				{
					// Generate next S
					dAsset = dyn.NextS();

					// Time to trade!
					if (stgy == DOLLAR_COST_AVG)
					{
						ac.Trade("AssetF",(int)(dInmAmt/dAsset),dAsset);
					}
					else if (stgy == INV_BY_VALUE)
					{
						dInmAmt = GetInvmPctg(_DEST_Asset,dAsset,INV_BY_VAL_CURV) * PRINCIPAL * LEVERAGE;
						double dInmPos = (dInmAmt/dAsset) - ac.GetPos("AssetF");
						if (dInmPos > 0) ac.Trade("AssetF",(int)dInmPos,dAsset);
					}

					// For getting intermediate CPnL
					int iPos = ac.GetPos("AssetF");
					if (iPos != 0) ac.Trade("AssetF",-iPos,dAsset);
					if (PRINT_DETAILS1) cout << iTimeCnt << "\t\t\t" << ac.CPnL()/*-dLastCPnL*/ << endl;
					if (ac.CPnL() < -PRINCIPAL) { iTimesBroke++; break; } // We are broke
					if (iPos != 0)
					{
						dLastCPnL = ac.CPnL();
						ac.Trade("AssetF",iPos,dAsset);
					}
					if (PRINT_DETAILS2) cout << iTimeCnt << "\t" << dAsset << "\t" << ac.GetPos("AssetF") << endl;
				}

				// Close all positions at the end of this Monte Carlo run
				if (ac.GetPos("AssetF")!=0) ac.Trade("AssetF",-ac.GetPos("AssetF"),dAsset);

				// Our utility function
				dqCPnL.push_back(ac.CPnL());

				if (PRINT_DETAILS1 || PRINT_DETAILS2) cout << endl << endl;
			}

			// Our utility function
			SNYStat ss(dqCPnL.size(),false);
			for (int i=0; i<dqCPnL.size(); i++) ss.Add(0,dqCPnL[i]);
			dTotUtil = ss.AMean(0) / ss.SD(0);
			if (ss.AMean(0) < MINREQCPNL) dTotUtil = 0;


			cout << fixed << "\t\t" << "AvgUtil=\t" << (dTotUtil) << "\tAvgCPnL=\t" << ss.AMean(0) << "\tTimesBroke=\t" << iTimesBroke << "/" << MONTE_CARLO_RUNS << "(" << (double)iTimesBroke / (double)MONTE_CARLO_RUNS * 100 << "%)";
			if (PRINT_DETAILS3) cout << endl;
			if (stgy == INV_BY_VALUE)
			{
				cout << "\t\tLeverage =\t" << LEVERAGE << "\tINV_BY_VAL_CURV =\t" << INV_BY_VAL_CURV;
				if (PRINT_DETAILS3) cout << endl;
				double dInitInvm = GetInvmPctg(_DEST_Asset,INIT_Asset,INV_BY_VAL_CURV) * PRINCIPAL * LEVERAGE;
				cout << "\t\tRequired invested capital at " << INIT_Asset << " =\t" << (long)dInitInvm;
				if (PRINT_DETAILS3) cout << endl;

				long lReqAtHalf = (long)(GetInvmPctg(2,1,INV_BY_VAL_CURV) * PRINCIPAL * LEVERAGE);
				cout << "\t\tRequired invested capital at " << _DEST_Asset/2 << " =\t" << lReqAtHalf;
				if (PRINT_DETAILS3) cout << endl;

				long lMaxPsb = (long)(((dInitInvm * _DEST_Asset/2 / INIT_Asset) + (PRINCIPAL - dInitInvm)) * 1.5);
				cout << "\t\tMax possible portfolio size at " << _DEST_Asset/2 << " =\t" << lMaxPsb << "\t(assume using 50% margin lending)";
				if (PRINT_DETAILS3) cout << endl;

				cout << "\t\tNot enough capital at " << _DEST_Asset/2 << "? " << (lReqAtHalf>lMaxPsb?"Y":"N");
				if (PRINT_DETAILS3) cout << endl;
				cout << endl << endl;
			}

			LEVERAGE += LEVERAGE_INC;
		}
		while (LEVERAGE < LEVERAGE_END);

		INV_BY_VAL_CURV += INV_BY_VAL_CURV_INC;
	}
	while (INV_BY_VAL_CURV < INV_BY_VAL_CURV_END);

}
