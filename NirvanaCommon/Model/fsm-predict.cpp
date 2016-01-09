// FINITE STATE MACHINE PREDICTION
/*
#include "../Math/StateMach2.hpp"
#include "../Util/STool.h"
#include "../Util/IParam.hpp"
#include "../Util/BarProvider.hpp"
#include "../Util/HKFE.hpp"
#include "../Util/SException.hpp"
#include <fstream>
#include <deque>
#include <map>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
	//------------------------------------------
	// Command Parameters
	cout << "Usage: fsm-predict 20120811 h/c i/h/s" << endl;

	// e.g. fsm-predict 20120811 h/c i/h/s
	int iTgtTrdgDate = 99999999;
	if (argc > 1) iTgtTrdgDate = STool::ToInt(argv[1]);

	string sInputStockList;
	if (argc > 2)
	{
		if (argv[2][0] == 'h') sInputStockList = string(IParam::STOCKLIST_HSI);
		else if (argv[2][0] == 'c') sInputStockList = string(IParam::STOCKLIST_HSI_HIGH_COR);
	}
	else
	{
		sInputStockList = string(IParam::STOCKLIST_HSI);
	}

	char cMarginalPMF = 'i';
	if (argc > 3)
	{
		if (argv[3][0] == 'i') cMarginalPMF = 'i'; //use indvl distribution
		if (argv[3][0] == 'h') cMarginalPMF = 'h'; //use joint distr, HSI side
		if (argv[3][0] == 's') cMarginalPMF = 's'; //use joint distr, stock side
	}

	char cUseOpenPx = 'c'; //use full bar OHLC
	if (argc > 4)
	{
		if (argv[4][0] == 'o') cUseOpenPx = 'o'; //use only bar open
	}


	//------------------------------------------
	// Read input file list
	deque<string> dqStkList;
	deque<double> dqStkListWeight;

	deque<string> dqRawFile = STool::ReadFile(sInputStockList.c_str());

	if (dqRawFile.empty()) return 0;
	for (int i = 0; i < dqRawFile.size(); i++)
	{
		string sPart;
		Tokenizer tz(dqRawFile[i],",");
		if (tz.NextToken()) sPart = tz.GetToken(); else continue;
		if (sPart.length() > 2) dqStkList.push_back(sPart);
		if (tz.NextToken()) sPart = tz.GetToken(); else continue;
		if (sPart.length() > 1) dqStkListWeight.push_back(STool::ToDbl(sPart,2));
	}
	//------------------------------------------




	//------------------------------------------
	// Accumulate PMF of the latest state for each HSI constit
	map<const double,double> mHSIPMF;
	deque<map<const double,double> > dqmStateMarginalPMF;
	double dWAStateCnt = 0;
	BarProvider bphsi(IParam::DATA_DAILY_HSI, "DOHLCV", 2, 'F', 'N', 'P');
	BarProvider bpv(IParam::DATA_DAILY_VHSI, "DOHLCV", 2, 'F', 'N', 'P');

	for (int i = 0; i < dqStkList.size(); i++)
	{
		StateMach2 sm2_stk;
		StateMach2 sm2_hsi;
		deque<map<const double,double> > dqmStatePMF;
		map<const int,int> mStateCnt;

		for (int k = 0; k < sm2_stk.TotalStates(); k++)
			dqmStatePMF.push_back(map<const double,double>());

		if (cMarginalPMF != 'i' && dqmStateMarginalPMF.size() == 0)
		{
			for (int k = 0; k < sm2_stk.TotalStates()*sm2_hsi.TotalStates(); k++)
				dqmStateMarginalPMF.push_back(map<const double,double>());

			//-------------------------------------------
			//read joint PMF
			//-------------------------------------------
			string sPath(IParam::FINSTATEMACH_PMF_JOINT);
			{
				ifstream ifs(sPath.c_str(), ios::in);
				if (ifs.is_open())
				{
					string sLine;
					while (ifs.good())
					{
						getline(ifs,sLine);
						string sState="";
						string sPxChgStk="";
						string sPxChgHsi="";
						string sProb="";
						string sStateCnt="";
						Tokenizer tz(sLine,",");
						if (tz.NextToken()) sState = tz.GetToken(); else continue;
						if (tz.NextToken()) sPxChgStk = tz.GetToken(); else continue;
						if (tz.NextToken()) sPxChgHsi = tz.GetToken(); else continue;
						if (tz.NextToken()) sProb = tz.GetToken(); else continue;
						if (tz.NextToken()) sStateCnt = tz.GetToken(); else continue;

						if (sState == "" || sPxChgStk == "" || sPxChgHsi == "" || sProb == "" || sStateCnt == "") continue;

						string sPxChg;
						if (cMarginalPMF == 's') sPxChg = sPxChgStk;
						else if (cMarginalPMF == 'h') sPxChg = sPxChgHsi;

						int iState = STool::ToInt(sState);
						double dPxChg = STool::ToDbl(sPxChg,10);
						if (STool::ContainsKey((map<const double,double>)(dqmStateMarginalPMF[iState]),dPxChg))
							dqmStateMarginalPMF[iState][dPxChg] += STool::ToDbl(sProb,10);
						else
							dqmStateMarginalPMF[iState][dPxChg] = STool::ToDbl(sProb,10);

						if (STool::ContainsKey(mStateCnt,iState))
							mStateCnt[iState] += STool::ToInt(sStateCnt);
						else
							mStateCnt[iState] = STool::ToInt(sStateCnt);
					}
				}
				else
				{
					SException se;
					se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
					throw se;
				}
				ifs.close();
			}
		}

		//-------------------------------------------
		//read individual PMF
		if (cMarginalPMF == 'i')
		{
			string sPath(IParam::FINSTATEMACH_PMF); sPath += dqStkList[i];
			{
				ifstream ifs(sPath.c_str(), ios::in);
				if (ifs.is_open())
				{
					string sLine;
					while (ifs.good())
					{
						getline(ifs,sLine);
						string sState="";
						string sPxChg="";
						string sProb="";
						string sStateCnt="";
						Tokenizer tz(sLine,",");
						if (tz.NextToken()) sState = tz.GetToken(); else continue;
						if (tz.NextToken()) sPxChg = tz.GetToken(); else continue;
						if (tz.NextToken()) sProb = tz.GetToken(); else continue;
						if (tz.NextToken()) sStateCnt = tz.GetToken(); else continue;

						if (sState == "" || sPxChg == "" || sProb == "" || sStateCnt == "") continue;

						int iState = STool::ToInt(sState);
						dqmStatePMF[iState][STool::ToDbl(sPxChg,10)]
							= STool::ToDbl(sProb,10);

						if (STool::ContainsKey(mStateCnt,iState))
							mStateCnt[iState] += STool::ToInt(sStateCnt);
						else
							mStateCnt[iState] = STool::ToInt(sStateCnt);
					}
				}
				else
				{
					SException se;
					se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
					throw se;
				}
				ifs.close();
			}
		}
		//-------------------------------------------


		string sPath = IParam::DATA_ALL_STOCKS; sPath += dqStkList[i];

		cout << sPath << ": ";
		BarProvider bp(sPath.c_str(), "DOHLCV", 2, 'F', 'N', 'P');
		bphsi.ResetPsn();
		bpv.ResetPsn();

		Bar<double,long> b = bp.GetNextBar();
		Bar<double,long> bhsi = bphsi.GetNextBar();
		Bar<double,long> bv = bpv.GetNextBar();

		int iTrdgDate = bv.YMDi();

		while (iTrdgDate > 0)
		{
			while (b.YMDi() < iTrdgDate && bp.HasNextBar()) b = bp.GetNextBar();
			while (bhsi.YMDi() < iTrdgDate && bphsi.HasNextBar()) bhsi = bphsi.GetNextBar();
			while (bv.YMDi() < iTrdgDate && bpv.HasNextBar()) bv = bpv.GetNextBar();

			if (b.YMDi() == iTrdgDate && bv.YMDi() == iTrdgDate && bhsi.YMDi() == iTrdgDate)
			{
				if (cUseOpenPx == 'o' && iTrdgDate >= iTgtTrdgDate)
				{
					sm2_stk.Add(b.Open, b.Open(), b.Open(), bv.Open());
					sm2_hsi.Add(bhsi.Open, bhsi.Open(), bhsi.Open(), bv.Open());
				}
				else
				{
					sm2_stk.Add(b.High, b.Low, b.Close, bv.Close());
					sm2_hsi.Add(bhsi.High, bhsi.Low, bhsi.Close, bv.Close());
				}
			}

			if (iTrdgDate >= iTgtTrdgDate) break;
			iTrdgDate = HKFE::NextTrdgDate(iTrdgDate,1);
		}

		if (cMarginalPMF == 'i' && sm2_stk.Ready())
		{
			int iStateStk = sm2_stk.State();
			cout << b.YMDi() << ": " << bhsi.YMDi() << ": " << bv.YMDi() << ", " << bhsi.Close << "," << b.Close() << ","  << bv.Close() << ", State: " << iStateStk << ": StateCnt: " << mStateCnt[iStateStk] << endl;
			dWAStateCnt += dqStkListWeight[i] * mStateCnt[iStateStk];
			for (map<const double,double>::iterator it=dqmStatePMF[iStateStk].begin(); it!=dqmStatePMF[iStateStk].end(); ++it)
			{
				if (STool::ContainsKey((map<const double,double>)(mHSIPMF),(double)(it->first)))
					mHSIPMF[it->first] += dqStkListWeight[i] * it->second;
				else
					mHSIPMF[it->first] = dqStkListWeight[i] * it->second;
			}
		}
		else if (cMarginalPMF != 'i' && sm2_stk.Ready() && sm2_hsi.Ready())
		{
			int iStateHsi = sm2_hsi.State();
			int iStateStk = sm2_stk.State();
			int iJointState = iStateStk*sm2_hsi.TotalStates()+iStateHsi;
			cout << b.YMDi() << ": " << bhsi.YMDi() << ": " << bv.YMDi() << ": State: Stk = " << iStateStk << ", Hsi = " << iStateHsi << ": StateCnt: " << mStateCnt[iJointState] << endl;
			dWAStateCnt += dqStkListWeight[i] * mStateCnt[iJointState];
			for (map<const double,double>::iterator it=dqmStateMarginalPMF[iJointState].begin(); it!=dqmStateMarginalPMF[iJointState].end(); ++it)
			{
				if (STool::ContainsKey((map<const double,double>)(mHSIPMF),(double)(it->first)))
					mHSIPMF[it->first] += dqStkListWeight[i] * it->second;
				else
					mHSIPMF[it->first] = dqStkListWeight[i] * it->second;
			}
		}


	}
	//------------------------------------------


	//------------------------------------------
	// Output the resulting PMF for HSI
	string sPath = string(IParam::FINSTATEMACH_PMF);
	sPath += string(IParam::FINSTATEMACH_PMF_HSILAST);
	ofstream ofs;
	ofs.open(sPath.c_str(), ios::out);
	if (ofs.is_open())
	{
		double dAMean = 0;
		double dVar = 0;
		double dSD = 0;

		double dTotalProb = 0;
		for (map<const double,double>::iterator it=mHSIPMF.begin(); it!=mHSIPMF.end(); ++it)
			dTotalProb += it->second;

		for (map<const double,double>::iterator it=mHSIPMF.begin(); it!=mHSIPMF.end(); ++it)
		{
			ofs << it->first << "," << it->second / dTotalProb << endl;
			dAMean += it->second / dTotalProb * it->first;
		}

		for (map<const double,double>::iterator it=mHSIPMF.begin(); it!=mHSIPMF.end(); ++it)
			dVar += it->second / dTotalProb * pow(it->first-dAMean,2);

		dSD = sqrt(dVar);

		cout << "AMean =\t" << STool::Round(dAMean,4)*100 << "%\tSD =\t" << STool::Round(dSD,4)*100 << "%\tWA Cnt=\t" << STool::Round(dWAStateCnt,2) << endl;
	}
	ofs.close();
	//------------------------------------------



	return 0;
}
*/
