// BACKTESTING RULE-BASED SYSTEMS
#include "../rulesys/A01.hpp" //[INPUT]
#include "../rulesys/A06.hpp" //[INPUT]
#include "../rulesys/R01.hpp" //[INPUT]
#include "../rulesys/R08.hpp" //[INPUT]
#include "../corelib/STool.hpp"
#include "../corelib/Acct.hpp"
#include "../corelib/HKFE.hpp"
#include "../corelib/IParam.hpp"
#include "../corelib/OParam.hpp"
#include "../corelib/PParam.hpp"
#include "../corelib/BarProvider.hpp"
#include "../corelib/SException.hpp"

using namespace std;


bool bPrintTrades = false;
bool bPrintAllBars = false;
bool bPrintDaily = false;
bool bBacktest = false;
int iRelevantDate = 0;
BarProvider bphsi;


int main(int argc, char *argv[])
{
	//---------------------------------------------------
	// User inputs per run
	if (argc < 6) // [Data File] is not needed if [Backtest] is 'Y'
	{
		cout << "Too few parameters." << endl;
		cout << "We expect: [Print Trades?] [Print All Bars?] [Print Daily CPnL?] [Backtest?] [Data File] [YYYYMMDD]" << endl;
		return 0;
	}

	if (argv[1][0] == 'Y' || argv[1][0] == 'y') bPrintTrades = true;
	if (argv[2][0] == 'Y' || argv[2][0] == 'y') bPrintAllBars = true;
	if (argv[3][0] == 'Y' || argv[3][0] == 'y') bPrintDaily = true;
	if (argv[4][0] == 'Y' || argv[4][0] == 'y') bBacktest = true;
	bphsi.LoadDataFile(argv[5], "DTOHLCV", 0, 'F', 'N', 'P');
	if (!bBacktest)
	{
		string s(argv[6]);
		iRelevantDate = STool::ToInt(s);
	}


	//---------------------------------------------------
	// Internal variables
	int iLastTrdgDay = -1;
	int iMastTrdPos = 0;
	Acct ac;
	int iBarCnt = 1; // Just to facilitate manual inspection in Excel


	A01 sysA01a(OParam::OptimalVecA01a); // [INPUT]
	A01 sysA01b(OParam::OptimalVecA01b); // [INPUT]
	A06 sysA06a(OParam::OptimalVecA06a); // [INPUT]
	A06 sysA06b(OParam::OptimalVecA06b); // [INPUT]
	R01 sysR01(OParam::OptimalVecR01); // [INPUT]
	R08 sysR08(OParam::OptimalVecR08); // [INPUT]
	bphsi.ResetPsn();

	while (bphsi.HasNextBar())
	{
		bphsi.MoveNext();

		// Skip when doing real time trade and the data isn't the relevant date
		if (!bBacktest && bphsi.YMDi() != iRelevantDate) continue;

		if (bphsi.YMDi() != iLastTrdgDay)
		{
			iBarCnt = 1;
			sysA01a.Reset(); //[INPUT]
			sysA01b.Reset(); //[INPUT]
			sysA06a.Reset(); //[INPUT]
			sysA06b.Reset(); //[INPUT]
			sysR01.Reset(); //[INPUT]
			sysR08.Reset(); //[INPUT]
			// Error handling: Just in case our historical data feed is truncated and we have an unclosed position.
			if (iMastTrdPos != 0)
			{
				ac.Trade(
						"HSIF",
						-iMastTrdPos,
						bphsi.PeekPrevClose(),
						IParam::TRDG_FRICT_FUT_SLIP + IParam::TRDG_FRICT_CMSN);

				iMastTrdPos = 0;
			}
			if (bPrintDaily) cout << iLastTrdgDay << "\t\t\t\t\t\t\t\t" << ac.CPnL() << endl;
		}

		// Update trading systems
		sysA01a.Add(bphsi); //[INPUT]
		sysA01b.Add(bphsi); //[INPUT]
		sysA06a.Add(bphsi); //[INPUT]
		sysA06b.Add(bphsi); //[INPUT]
		sysR01.Add(bphsi); //[INPUT]
		sysR08.Add(bphsi); //[INPUT]

		double dAggrPos = PParam::A01a_SIZE * sysA01a.Pos() +
											PParam::A01b_SIZE * sysA01b.Pos() +
											PParam::A06a_SIZE * sysA06a.Pos() +
											PParam::A06b_SIZE * sysA06b.Pos() +
											PParam::R01_SIZE * sysR01.Pos() +
											PParam::R08_SIZE * sysR08.Pos(); //[INPUT]

		// To trade
		int iToTrd = 0;
		if (!HKFE::IsTrdgHour(bphsi.YMDi(),bphsi.HMi()) && iMastTrdPos != 0)
		{
			iToTrd = -iMastTrdPos;
			iMastTrdPos = 0;
		}
		else if (HKFE::IsTrdgHour(bphsi.YMDi(),bphsi.HMi()) && dAggrPos != iMastTrdPos)
		{
			iToTrd = dAggrPos-iMastTrdPos;
			iMastTrdPos = dAggrPos;
		}


		if (iToTrd != 0)
		{
			ac.Trade("HSIF", iToTrd, bphsi.Close(),
								IParam::TRDG_FRICT_FUT_SLIP+IParam::TRDG_FRICT_CMSN);
			if (bPrintTrades) cout << "T:\t" << bphsi.YMDi() << "\t" << bphsi.HMi() << "\t" << bphsi.Open() << "\t" << bphsi.High() << "\t" << bphsi.Low() << "\t" << bphsi.Close() << "\t<" << iToTrd << ">\t" << iMastTrdPos << "\t" << (ac.CPnL()==IParam::MAGIC_NUM?"NA":STool::ToStr(ac.CPnL())) << "" << endl;
		}
		else if (bPrintAllBars) cout << iBarCnt << "\t" << bphsi.YMDi() << "\t" << bphsi.HMi() << "\t" << bphsi.Open() << "\t" << bphsi.High() << "\t" << bphsi.Low() << "\t" << bphsi.Close() << "\t\t" << iMastTrdPos << "\t" << (ac.CPnL()==IParam::MAGIC_NUM?"NA":STool::ToStr(ac.CPnL())) << "" << endl;

		iBarCnt++;


		// Update date
		iLastTrdgDay = bphsi.YMDi();

	}

	// Output result and Reset Acct
	cout << "Final CPnL = " << ac.CPnL() << endl;
	ac.Reset();




	return 0;
}
