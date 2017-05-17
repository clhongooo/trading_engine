// CALCULATING THE CORRELATION BETWEEN HSI AND HK STOCKS
#include "../smath/SNYStat.hpp"
#include "../corelib/IParam.hpp"
#include "../corelib/BarProvider.hpp"
#include "../corelib/Bar.hpp"
#include "../corelib/HKFE.hpp"
#include "../corelib/SException.hpp"
#include <fstream>
#include <deque>
#include <map>
#include <string>

using namespace std;

int main()
{
	//------------------------------------------
	// Read file list
	deque<string> dqStk;
	deque<double> dqStkWeight;

	deque<string> dqRawFile = STool::ReadFile(IParam::HK_INDEX_STOCKS);

	if (dqRawFile.empty()) return 0;
	for (int i = 0; i < dqRawFile.size(); i++)
	{
		string sPart;
		Tokenizer tz(dqRawFile[i],",");
		if (tz.NextToken()) sPart = tz.GetToken(); else continue;
		if (sPart.length() > 2) dqStk.push_back(sPart);
		if (tz.NextToken()) sPart = tz.GetToken(); else continue;
		if (sPart.length() > 1) dqStkWeight.push_back(STool::ToDbl(sPart,2));
	}
	//------------------------------------------

	//------------------------------------------
	// Calculate correl between HSI and each of the stocks
	deque<string> dqOutput;
	for (int i = 0; i < dqStk.size(); i++)
	{
		try
		{
			double dEMACorrel0 = 0;
			double dEMACorrel0Alpha = (double)1/44; //2-month average
			double dEMACorrel1 = 0;
			double dEMACorrel1Alpha = (double)1/44; //2-month average
			double dEMACorrel2 = 0;
			double dEMACorrel2Alpha = (double)1/44; //2-month average
			double dEMACorrel3 = 0;
			double dEMACorrel3Alpha = (double)1/44; //2-month average

			string sPath(IParam::DATA_ALL_STOCKS); sPath += dqStk[i];
			BarProvider bp(sPath.c_str(), "DOHLCV", 2, 'B', 'H', 'P');
			BarProvider bph(IParam::DATA_DAILY_HSI_OLD, "DOHLCV", 2, 'F', 'N', 'P');

			Bar<double,long> b = bp.GetNextBar();
			Bar<double,long> bh = bph.GetNextBar();

			vector<int> vDelay0; vDelay0.push_back(0); vDelay0.push_back(0);
			SNYStat ss_offset0(2,vDelay0,20,1000,true);
			vector<int> vDelay1; vDelay1.push_back(0); vDelay1.push_back(1);
			SNYStat ss_offset1(2,vDelay1,20,1000,true);
			vector<int> vDelay2; vDelay2.push_back(0); vDelay2.push_back(2);
			SNYStat ss_offset2(2,vDelay2,20,1000,true);
			vector<int> vDelay3; vDelay3.push_back(0); vDelay3.push_back(3);
			SNYStat ss_offset3(2,vDelay3,20,1000,true);

			int iTrdgDate = bh.YMDi();

			while (iTrdgDate > 0)
			{
				while (b.YMDi() < iTrdgDate && bp.HasNextBar()) b = bp.GetNextBar();
				while (bh.YMDi() < iTrdgDate && bph.HasNextBar()) bh = bph.GetNextBar();

				if (b.YMDi() == iTrdgDate && bh.YMDi() == iTrdgDate)
				{
					ss_offset0.Add(0,bh.Close);
					ss_offset0.Add(1,b.Close);
					ss_offset1.Add(0,bh.Close);
					ss_offset1.Add(1,b.Close);
					ss_offset2.Add(0,bh.Close);
					ss_offset2.Add(1,b.Close);
					ss_offset3.Add(0,bh.Close);
					ss_offset3.Add(1,b.Close);

					if (ss_offset0.WarmedUp())
					{
						double dCorrel=0;
						if (ss_offset0.Correl(0,1,dCorrel) && dCorrel == dCorrel)
						{
							if (dEMACorrel0 == 0) dEMACorrel0 = dCorrel;
							else dEMACorrel0 = (1-dEMACorrel0Alpha)*dEMACorrel0 +
								dEMACorrel0Alpha*dCorrel;
						}
					}
					if (ss_offset1.WarmedUp())
					{
						double dCorrel=0;
						if (ss_offset1.Correl(0,1,dCorrel) && dCorrel == dCorrel)
						{
							if (dEMACorrel1 == 0) dEMACorrel1 = dCorrel;
							else dEMACorrel1 = (1-dEMACorrel1Alpha)*dEMACorrel1 +
								dEMACorrel1Alpha*dCorrel;
						}
					}
					if (ss_offset2.WarmedUp())
					{
						double dCorrel=0;
						if (ss_offset2.Correl(0,1,dCorrel) && dCorrel == dCorrel)
						{
							if (dEMACorrel2 == 0) dEMACorrel2 = dCorrel;
							else dEMACorrel2 = (1-dEMACorrel2Alpha)*dEMACorrel2 +
								dEMACorrel2Alpha*dCorrel;
						}
					}
					if (ss_offset3.WarmedUp())
					{
						double dCorrel=0;
						if (ss_offset3.Correl(0,1,dCorrel) && dCorrel == dCorrel)
						{
							if (dEMACorrel3 == 0) dEMACorrel3 = dCorrel;
							else dEMACorrel3 = (1-dEMACorrel3Alpha)*dEMACorrel3 +
								dEMACorrel3Alpha*dCorrel;
						}
					}
				}
				iTrdgDate = HKFE::NextTrdgDate(iTrdgDate,1);
			}

			string s = dqStk[i]+","+STool::ToStr(dEMACorrel0,10)+","+STool::ToStr(dEMACorrel1,10)+","+STool::ToStr(dEMACorrel2,10)+","+STool::ToStr(dEMACorrel3,10);
			dqOutput.push_back(s);

			cout << s << endl;

		} catch (std::exception& e) {}



	}
	//------------------------------------------



	//------------------------------------------
	// Output correlation result
	{
		string sPath(IParam::HSI_STK_CORREL);
		ofstream ofs;
		ofs.open(sPath.c_str(), ios::out);
		if (ofs.is_open())
		{
			for (int k = 0; k < dqOutput.size(); k++)
			{
				ofs << dqOutput[k] << endl;
			}
		}
		ofs.close();
	}
	//------------------------------------------


	return 0;
}
