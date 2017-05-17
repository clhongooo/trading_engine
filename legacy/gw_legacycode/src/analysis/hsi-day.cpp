#include "../rulesys/B2010.hpp" //[INPUT]
#include "../modelsys/FSMC.hpp" //[INPUT]
#include "../corelib/STool.hpp"
#include "../corelib/Acct.hpp"
#include "../corelib/HKFE.hpp"
#include "../corelib/IParam.hpp"
#include "../corelib/PParam.hpp"
#include "../corelib/OParam.hpp"
#include "../corelib/BarProvider.hpp"
#include "../corelib/SException.hpp"

using namespace std;


bool bPrintTrades = false;
BarProvider bphsi;
int iStartDate = 0;
int iEndDate = 0;


int main(int argc, char *argv[])
{
	//---------------------------------------------------
	// User inputs per run
	if (argc < 5)
	{
		cout << "Too few parameters." << endl;
		cout << "We expect: [Print Trade?] [Data File] [Start Date] [End Date]" << endl;
		return 0;
	}

	if (argv[1][0] == 'Y' || argv[1][0] == 'y') bPrintTrades = true;
	bphsi.LoadDataFile(argv[2], "DOHLCV", 0, 'F', 'N', 'P');
	iStartDate = STool::ToInt(argv[3]);
	iEndDate = STool::ToInt(argv[4]);

	if (iStartDate >= iEndDate)
	{
		cout << "Start Date should be before End Date. Exiting" << endl;
		return 0;
	}


	//---------------------------------------------------
	// Internal variables
	double dLastClosePx = 0;
	int iMastTrdPos = 0;
	Acct ac;


	double dB3ParamVec[3] = {PParam::B3_SIZE, PParam::B5_SIZE, PParam::B10_SIZE}; //[INPUT]
	double dFSMCParamVec[1] = {PParam::FSMC_SIZE}; //[INPUT]
	B2010 sysB(dB3ParamVec); //[INPUT]
	FSMC sysFSMC(dFSMCParamVec); //[INPUT]
	bphsi.ResetPsn();

	while (bphsi.HasNextBar())
	{
		bphsi.MoveNext();
		if (bphsi.YMDi() < iStartDate) continue;
		if (bphsi.YMDi() > iEndDate) continue;
		dLastClosePx = bphsi.Close();

		bool bAtDayOpen = true;
		while (true)
		{
			// Update trading systems
			sysB.Add(bphsi,bAtDayOpen); //[INPUT]
			sysFSMC.Add(bphsi,bAtDayOpen); //[INPUT]

			double dAggrPos = sysB.Pos() + sysFSMC.Pos(); //[INPUT]

			// To trade
			int iToTrd = 0;
			if (dAggrPos != iMastTrdPos)
			{
				iToTrd = dAggrPos-iMastTrdPos;
				iMastTrdPos = dAggrPos;
				if (iToTrd != 0) ac.Trade("HSIF", iToTrd, bAtDayOpen?bphsi.Open():bphsi.Close());
				if (bPrintTrades) cout << "T:\t" << (bAtDayOpen?"[O]":"[C]") << "\t" << bphsi.YMDi() << "\t" << bphsi.Open() << "\t" << bphsi.High() << "\t" << bphsi.Low() << "\t" << bphsi.Close() << "\t\t" << iMastTrdPos << "\t" << (ac.CPnL()==IParam::MAGIC_NUM?"NA":STool::ToStr(ac.CPnL())) << "" << endl;
			}

			if (!bAtDayOpen)
			{
				// Temporarily close position for CPnL
				ac.Trade("HSIF", -iMastTrdPos, bphsi.Close());
				cout << "\t\t" << bphsi.YMDi() << "\t" << bphsi.Open() << "\t" << bphsi.High() << "\t" << bphsi.Low() << "\t" << bphsi.Close() << "\t\t" << iMastTrdPos << "\t" << (ac.CPnL()==IParam::MAGIC_NUM?"NA":STool::ToStr(ac.CPnL())) << "" << endl;
				ac.Trade("HSIF", iMastTrdPos, bphsi.Close());
			}

			if (bAtDayOpen) bAtDayOpen = false;
			else if (!bAtDayOpen) break;
		}
	}

	// Close all trades
	ac.Trade("HSIF", -iMastTrdPos, dLastClosePx);

	// Output Extra Info from FSMC
	sysFSMC.PrintExtraInfo();

	// Output result and Reset Acct
	cout << "Final CPnL = " << ac.CPnL() << endl;
	ac.Reset();


	return 0;
}
