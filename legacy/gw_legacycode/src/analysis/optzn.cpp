// BACKTESTING RULE-BASED SYSTEMS
#include "../rulesys/A01.hpp" //[INPUT]
#include "../corelib/STool.hpp"
#include "../corelib/Acct.hpp"
#include "../corelib/HKFE.hpp"
#include "../corelib/IParam.hpp"
#include "../corelib/OParam.hpp"
#include "../corelib/BarProvider.hpp"
#include "../corelib/Bar.hpp"
#include "../corelib/SException.hpp"
#include <boost/thread.hpp>
#include <vector>

using namespace std;





// Global variables + User inputs
const int _iThrdNo = 4;// [INPUT]
vector<BarProvider> bphsi;
const int iParamVecLen = OParam::VecLenA01; // [INPUT]
double dParamVec[iParamVecLen];
double dParamMatrix[iParamVecLen][3];
string sThrdOutput[_iThrdNo];





//returns true if there are still unevaluated param vectors
//returns false if all vectors are evaluated
bool GenNextParamVec(double *dV, int iVecLen)
{
	do //[INPUT]
	{
		dV[0] += dParamMatrix[0][2];
		for (int i=0; i<iVecLen-1; i++)
		{
			if (dV[i] > dParamMatrix[i][1] || dV[i] < dParamMatrix[i][0])
			{
				dV[i] = dParamMatrix[i][0];
				dV[i+1] += dParamMatrix[i+1][2];
			}
		}
		if (dV[iVecLen-1] > dParamMatrix[iVecLen-1][1]) return false;

	}
	while (dV[0] >= dV[1] || dV[2] >= dV[3]); //INPUT

	return true;
}


void workerFunc(int iThrdId, double dThrdPVec[])
{
	//---------------------------------------------------
	// Internal variables
	int iLastTrdgDay = -1;
	int iMastTrdPos = 0;
	Acct ac;

	A01 rulesys(dThrdPVec); // [INPUT]
	bphsi[iThrdId].ResetPsn();

	while (bphsi[iThrdId].HasNextBar())
	{
		bphsi[iThrdId].MoveNext();
		if (bphsi[iThrdId].YMDi() != iLastTrdgDay)
		{
			rulesys.Reset();
			// Error handling: Just in case our historical data feed is truncated and we have an unclosed position.
			if (iMastTrdPos != 0)
			{
				ac.Trade("HSIF", -iMastTrdPos, ((Bar<double,int>)bphsi[iThrdId].PeekLastBar()).Close,
									IParam::TRDG_FRICT_FUT_SLIP+IParam::TRDG_FRICT_CMSN);
				iMastTrdPos = 0;
			}
		}

		// Update trading systems
		rulesys.Add(bphsi[iThrdId]);

		// To trade
		int iToTrd = 0;
		if (!HKFE::IsTrdgHour(bphsi[iThrdId].YMDi(),bphsi[iThrdId].HMi()) && iMastTrdPos != 0)
		{
			iToTrd = -iMastTrdPos;
			iMastTrdPos = 0;
		}
		else if (HKFE::IsTrdgHour(bphsi[iThrdId].YMDi(),bphsi[iThrdId].HMi()) && rulesys.Pos() != iMastTrdPos)
		{
			iToTrd = rulesys.Pos()-iMastTrdPos;
			iMastTrdPos = rulesys.Pos();
		}

		if (iToTrd != 0) ac.Trade("HSIF", iToTrd, bphsi[iThrdId].Close,
									IParam::TRDG_FRICT_FUT_SLIP+IParam::TRDG_FRICT_CMSN);

		// Update date
		iLastTrdgDay = bphsi[iThrdId].YMDi();

	}

	// Output result and Reset Acct
	stringstream ssRtn;
	for (int k=0; k<iParamVecLen; k++) ssRtn << "[" << k << "]\t" << dThrdPVec[k] << "\t";
	ssRtn << ac.CPnL() << endl;
	ac.Reset();

	sThrdOutput[iThrdId] = ssRtn.str();

	return;
}


int main(int argc, char *argv[])
{
	// Init Data Feed
	for (int i=0; i<_iThrdNo; i++)
		bphsi.push_back(BarProvider(IParam::DATA_INTRADAY_HSIF, "DTOHLCV", 0, 'F', 'N', 'P')); // [INPUT]

	// Init Param Matrix and Vector
	memcpy(dParamMatrix, OParam::MatrixA01, sizeof(double)*iParamVecLen*3); // [INPUT]
	for (int i=0; i<iParamVecLen; i++) dParamVec[i] = dParamMatrix[i][0];

	int iThrdCreated = 0;
	do
	{
		iThrdCreated = 0;
		double dTmpPVec[_iThrdNo][iParamVecLen];
		vector<boost::thread *> workerThrd;

		for (int i=0; i<_iThrdNo; i++)
		{
			if (GenNextParamVec(dParamVec,iParamVecLen))
			{
				iThrdCreated++;
				memcpy(dTmpPVec[i], dParamVec, sizeof(double)*iParamVecLen);
				workerThrd.push_back(new boost::thread(workerFunc, i, dTmpPVec[i]));
			}
		}

		for (int i=0; i<iThrdCreated; i++)
		{
			workerThrd[i]->join();
			delete workerThrd[i];
			cout << sThrdOutput[i];
		}

	} while (iThrdCreated > 0);

	return 0;
}
