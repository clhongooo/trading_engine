#include "../smath/Dynamics.hpp"
#include "../smath/SNYStat.hpp"
#include "../corelib/BarProvider.hpp"
#include "../corelib/Acct.hpp"
#include <deque>

using namespace std;

int main(int argc, char *argv[])
{
	Acct ac;

	deque<double> dqCPnL;
	BarProvider bp("../../dataRT/data-old.csv","DOHLCV",2,'F','N','P');

	const double EACH_INVM = 15000;
	const int IMOD = 22; //monthly
	//const int IMOD = 5; //5-day

	int iTimeCnt = 0;
	int iYMDi = 0;
	double dTotInvAmt = 0;
	double dTotUtil = 0;
	double dLastCPnL = 0;
	double dETFPx;

	//------------------------------------
	dqCPnL.clear();
	ac.Reset();
	bp.ResetPsn();

	//------------------------------------
	while (bp.HasNextBar())
	{
		// Generate next S
		bp.MoveNext();
		iYMDi = bp.YMDi();
		dETFPx = bp.Close() / 10;

		// Time to trade!
		if (iTimeCnt % IMOD == 0)
		{
			int iQty = (int)(EACH_INVM/dETFPx);
			ac.Trade("HSIF",iQty,dETFPx);
			dTotInvAmt += dETFPx*iQty;
		}

		// For getting intermediate CPnL
		int iPos = ac.GetPos("HSIF");
		if (iPos != 0)
		{
			ac.Trade("HSIF",-iPos,dETFPx);
			dqCPnL.push_back(ac.CPnL()-dLastCPnL);
		}
		cout << iTimeCnt << "\t\t\t\t" << ac.CPnL()-dLastCPnL << endl;
		if (iPos != 0)
		{
			dLastCPnL = ac.CPnL();
			ac.Trade("HSIF",iPos,dETFPx);
		}
		//cout << iTimeCnt << "\t" << bp.YMDi() << "\t" << dETFPx << "\t" << ac.GetPos("HSIF") << endl;

		iTimeCnt++;
	}

	// Close all
	ac.Trade("HSIF",-ac.GetPos("HSIF"),dETFPx);

	// Calculate Utility
	if (!dqCPnL.empty())
	{
		SNYStat ss(dqCPnL.size(),false);
		for (int i=0; i<dqCPnL.size(); i++) ss.Add(0,dqCPnL[i]);
		dTotUtil += pow(ss.AMean(0),2) / ss.SD(0); // Our utility function
	}

	cout << fixed << endl << endl << "\t" << iYMDi << "\t" << "\tTotInvAmt=\t" << dTotInvAmt << "\tFinalCPnL=\t" << ac.CPnL() << "\tUtil=\t" << dTotUtil << endl;
}


