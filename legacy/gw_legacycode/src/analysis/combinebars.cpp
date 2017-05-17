#include "../smath/Sma.hpp"
#include "../smath/HighLow.hpp"
#include "../corelib/STool.hpp"
#include "../corelib/BarProvider.hpp"
#include "../corelib/Bar.hpp"
#include "../corelib/SException.hpp"

using namespace std;


int _iNBarsTo1;
BarProvider bphsi;


int main(int argc, char *argv[])
{
	//---------------------------------------------------
	// User inputs per run
	if (argc < 3)
	{
		cout << "Too few parameters." << endl;
		cout << "We expect: [N bars to 1] [Data File]" << endl;
		return 0;
	}

	_iNBarsTo1 = STool::ToInt(argv[1]);	
	bphsi.LoadDataFile(argv[2], "DOHLCV", 0, 'F', 'N', 'P');


	//---------------------------------------------------
	// Internal variables
	int iBarCnt = 0;
	Sma<double> sma(_iNBarsTo1,true);
	HighLow<double> hl(_iNBarsTo1,true);
	double dOpen = 0;

	while (bphsi.HasNextBar())
	{
		Bar<double,int> bhsi = bphsi.GetNextBar();

		sma.Add(bhsi.Open);
		sma.Add(bhsi.High);
		sma.Add(bhsi.High);
		sma.Add(bhsi.Low);
		sma.Add(bhsi.Low);
		sma.Add(bhsi.Close);
		sma.Add(bhsi.Close);
		hl.Add(bhsi.High,bhsi.Low);

		if (iBarCnt == 0) dOpen = bhsi.Open;

		iBarCnt++;

		if (iBarCnt >= _iNBarsTo1)
		{
			cout << bhsi.YMDi() << "\t" << dOpen << "\t" << hl.HighestHigh << "\t" << hl.LowestLow << "\t" << bhsi.Close << "\t" << sma.Value() << endl;
			sma.Reset(_iNBarsTo1,true);
			hl.Reset(_iNBarsTo1,true);
			iBarCnt = 0;
		}

	}

	return 0;
}
