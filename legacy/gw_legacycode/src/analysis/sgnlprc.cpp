#include "../smath/SgnlPrc.hpp"
#include "../corelib/STool.hpp"
#include "../corelib/IParam.hpp"
#include "../corelib/BarProvider.hpp"
#include "../corelib/SException.hpp"

using namespace std;


bool bPrintTrades = false;
bool bPrintAllBars = false;
BarProvider bphsi;


int main(int argc, char *argv[])
{
	//---------------------------------------------------
	// User inputs per run
	if (argc < 2)
	{
		cout << "Too few parameters." << endl;
		cout << "We expect: [Data File]" << endl;
		return 0;
	}

	bphsi.LoadDataFile(argv[1], "DOHLCV", 0, 'F', 'N', 'P');


	//---------------------------------------------------
	// Internal variables
	SgnlPrc sp;

	while (bphsi.HasNextBar())
	{
		bphsi.MoveNext();

		// Update trading systems
		sp.Add(bphsi.High(),bphsi.Low(),bphsi.Close()); //[INPUT]

		if (sp.WarmedUp())
			cout << bphsi.YMDi() << "\tPeriod=\t" << sp.Period_HD_DD() << "\tSNR=\t" << sp.SNR() << "\tT/C=\t" << sp.TrendCycleMode() << endl;


	}




	return 0;
}
