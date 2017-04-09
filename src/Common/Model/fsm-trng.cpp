// FINITE STATE MACHINE TRAINING
/*
#include "../Math/StateMach.hpp"
#include "../Math/StateMach1.hpp"
#include "../Math/StateMach2.hpp"
#include "../Util/STool.h"
#include "../Util/BarProvider.hpp"
#include "../Util/HKFE.hpp"
#include "../Util/SException.hpp"
#include "../Util/IParam.hpp"
#include <fstream>
#include <deque>
#include <map>
#include <string>

using namespace std;

int main()
{
	//------------------------------------------
	// Getting FSM input from user
	int iFSMNum = 1;
	char cStkLs = 't';
	int iTrngStartDate = IParam::MAGIC_NUM_INT;
	int iTrngEndDate = IParam::MAGIC_NUM_INT;
	cout << "====================================================" << endl;
	cout << "FSM BOOTCAMP" << endl;
	cout << "Enter State Machine #: ";
	cin >> iFSMNum;
	cout << endl;
	cout << "Stock List: [t]esting, [h]si, [i]ndices: ";
	cin >> cStkLs;
	cout << endl;
	cout << "Training Start Date: ";
	cin >> iTrngStartDate;
	cout << "Training End Date: ";
	cin >> iTrngEndDate;
	cout << endl;
	cout << "====================================================" << endl;
	//------------------------------------------


	//------------------------------------------
	// Read input file list
	deque<string> dqInputFiles;

	deque<string> dqRawFile;
	if (cStkLs == 't')
		dqRawFile = STool::ReadFile(IParam::STOCKLIST_TESTING);
	else if (cStkLs == 'h')
		dqRawFile = STool::ReadFile(IParam::STOCKLIST_HSI);
	else if (cStkLs == 'i')
		dqRawFile = STool::ReadFile(IParam::STOCKLIST_HK_INDICES);

	if (dqRawFile.empty()) return 0;
	for (int i = 0; i < dqRawFile.size(); i++)
	{
		string sPart;
		Tokenizer tz(dqRawFile[i],",");
		if (tz.NextToken()) sPart = tz.GetToken(); else continue;
		if (sPart.length() > 2) dqInputFiles.push_back(sPart);
		if (tz.NextToken()) sPart = tz.GetToken(); else continue;
	}
	//------------------------------------------

	//------------------------------------------
	// Collect Stat for each Stock
	BarProvider bphsi(IParam::DATA_DAILY_HSI, "DOHLCV", 2, 'F', 'N', 'P');
	BarProvider bpv(IParam::DATA_DAILY_VHSI, "DOHLCV", 2, 'F', 'N', 'P');
	double dTotalAbsMean = 0; //FSM scoring
	deque<map<const string,int> > dqmStateJointPMF; //ordered pair: stk x hsi
	int iHSINumStates = 0;

	for (int i = 0; i < dqInputFiles.size(); i++)
	{
		StateMach *sm_stk;
		StateMach *sm_hsi;
		switch (iFSMNum)
		{
			case 1: sm_stk = new StateMach1(); sm_hsi = new StateMach1(); break;
			case 2: sm_stk = new StateMach2(); sm_hsi = new StateMach2(); break;
			default: sm_stk = new StateMach2(); sm_hsi = new StateMach2(); break;
		}
		cout << "Using FSM: [" << sm_stk->Name() << "].\t";
		cout << "Using FSM: [" << sm_hsi->Name() << "].\t";

		iHSINumStates = sm_hsi->TotalStates();

		deque<double> dqdCloseStk;
		deque<double> dqdCloseHsi;
		deque<int> dqiStateHsi;
		deque<int> dqiStateStk;
		deque<map<const double,int> > dqmStatePMF;

		for (int k = 0; k < sm_stk->TotalStates(); k++)
			dqmStatePMF.push_back(map<const double,int>());

		if (dqmStateJointPMF.empty())
		for (int k = 0; k < sm_stk->TotalStates()*sm_hsi->TotalStates(); k++)
			dqmStateJointPMF.push_back(map<const string,int>());


		string sPath(IParam::DATA_ALL_STOCKS); sPath += dqInputFiles[i];

		cout << "Reading: " << sPath << endl;
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

			if (b.YMDi() == iTrdgDate && bv.YMDi() == iTrdgDate && bhsi.YMDi() == iTrdgDate &&
					iTrdgDate >= iTrngStartDate && iTrdgDate <= iTrngEndDate)
			{
				sm_stk->Add(b.High, b.Low, b.Close, bv.Close());
				sm_hsi->Add(bhsi.High, bhsi.Low, bhsi.Close, bv.Close());

				dqdCloseStk.push_back(b.Close);
				dqdCloseHsi.push_back(bhsi.Close);
				if (sm_stk->Ready() && sm_hsi->Ready())
				{
					dqiStateHsi.push_back(sm_hsi->State());
					dqiStateStk.push_back(sm_stk->State());

					if (dqiStateStk.size() >= 2 && dqiStateHsi.size() >= 2)
					{
						int iStateHsi = dqiStateHsi[dqiStateHsi.size()-2];
						int iStateStk = dqiStateStk[dqiStateStk.size()-2];
						double dRtnStk = dqdCloseStk[dqdCloseStk.size()-1]/dqdCloseStk[dqdCloseStk.size()-2]-1;
						double dRtnHsi = dqdCloseHsi[dqdCloseHsi.size()-1]/dqdCloseHsi[dqdCloseHsi.size()-2]-1;
						dRtnStk = STool::RoundIntvl(dRtnStk,IParam::FSM_PMF_STK_INTVL);
						dRtnHsi = STool::RoundIntvl(dRtnHsi,IParam::FSM_PMF_STK_INTVL);

						if (dRtnStk > IParam::FSM_PMF_STK_ABS_MAX)
							dRtnStk = IParam::FSM_PMF_STK_ABS_MAX;
						else if (dRtnStk < -IParam::FSM_PMF_STK_ABS_MAX)
							dRtnStk = -IParam::FSM_PMF_STK_ABS_MAX;

						if (dRtnHsi > IParam::FSM_PMF_STK_ABS_MAX)
							dRtnHsi = IParam::FSM_PMF_STK_ABS_MAX;
						else if (dRtnHsi < -IParam::FSM_PMF_STK_ABS_MAX)
							dRtnHsi = -IParam::FSM_PMF_STK_ABS_MAX;

						//PMF
						if (STool::ContainsKey((map<const double,int>)(dqmStatePMF[iStateStk]),dRtnStk))
							dqmStatePMF[iStateStk][dRtnStk] += 1;
						else
							dqmStatePMF[iStateStk][dRtnStk] = 1;

						//Joint PMF
						string sRtnJoint = "";
						sRtnJoint = STool::ToStr(dRtnStk,5)+","+STool::ToStr(dRtnHsi,5); //ordered pair: stk x hsi
						int iJointState = iStateStk*sm_hsi->TotalStates()+iStateHsi;
						if (STool::ContainsKey((map<const string,int>)(dqmStateJointPMF[iJointState]),sRtnJoint))
							dqmStateJointPMF[iJointState][sRtnJoint] += 1;
						else
							dqmStateJointPMF[iJointState][sRtnJoint] = 1;

					}
				}
			}
			iTrdgDate = HKFE::NextTrdgDate(iTrdgDate,1);
		}

		//output individual FSM stat
		sPath = IParam::FINSTATEMACH_PMF; sPath += dqInputFiles[i];
		ofstream ofs;
		ofs.open(sPath.c_str(), ios::out);
		if (ofs.is_open())
		{
			for (int k = 0; k < sm_stk->TotalStates(); k++)
			{
				int iTotCnt = 0;
				double dMean = 0; //FSM scoring

				for (map<const double,int>::iterator it=dqmStatePMF[k].begin(); it!=dqmStatePMF[k].end(); ++it) iTotCnt += it->second;

				for (map<const double,int>::iterator it=dqmStatePMF[k].begin(); it!=dqmStatePMF[k].end(); ++it)
				{
					ofs << k << "," << it->first << "," << (double)(it->second)/(double)(iTotCnt) << "," << (it->second) << ",=Freq," << STool::DecToBin(k) << "," << sm_stk->StateDesc(k) << endl;

					if (iTotCnt > 15) //FSM scoring
					dMean += (it->second/(double)iTotCnt) * it->first / sm_stk->TotalStates(); //FSM scoring
				}

				dTotalAbsMean += abs(dMean); //FSM scoring

			}
		}
		ofs.close();

		//------------------------------------------
		// Free up memory
		delete sm_stk;
		delete sm_hsi;
		//------------------------------------------
	}








	//FSM scoring output
	cout << "FSM Score (/stk /state) = " << dTotalAbsMean / dqInputFiles.size() *30000  << endl;










	cout << "Output Joint PMF: Start." << endl;
	//output joint FSM stat
	string sPath = IParam::FINSTATEMACH_PMF_JOINT;
	ofstream ofs;
	ofs.open(sPath.c_str(), ios::out);
	if (ofs.is_open())
	{
		for (int k = 0; k < dqmStateJointPMF.size(); k++)
		{
			int iTotCnt = 0;

			for (map<const string,int>::iterator it=dqmStateJointPMF[k].begin(); it!=dqmStateJointPMF[k].end(); ++it) iTotCnt += it->second;

			for (map<const string,int>::iterator it=dqmStateJointPMF[k].begin(); it!=dqmStateJointPMF[k].end(); ++it)
			{
				ofs << k << "," << it->first << "," << (double)(it->second)/(double)(iTotCnt) << "," << (it->second) << ",=Freq," << STool::DecToBin(k) << "," << k/iHSINumStates << ":" << k%iHSINumStates << endl;

			}

		}
	}
	ofs.close();
	cout << "Output Joint PMF: End." << endl;












	//------------------------------------------

	return 0;
}
*/
