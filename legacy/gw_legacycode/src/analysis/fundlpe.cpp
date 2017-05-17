#include "../smath/FundlMdlPE.hpp"
#include "../smath/SNYStat.hpp"
#include <deque>

using namespace std;

int main(int argc, char *argv[])
{
	const int NO_OF_MONTHS = 36;
	const int MONTE_CARLO_RUNS = 100000;

	deque<deque<double> > dqdqX;

	for (int i=0; i<NO_OF_MONTHS; i++)
		dqdqX.push_back(deque<double>());

	FundlMdlPE fu;
	for (int i=0; i<MONTE_CARLO_RUNS; i++)
	{
		fu.Reset();
		for (int j=0; j<NO_OF_MONTHS; j++)
		{
			fu.NextMonth();
			dqdqX[j].push_back(fu.GetHSI());
		}
	}

	for (int i=0; i<NO_OF_MONTHS; i++)
	{
		SNYStat ss(dqdqX[i].size(),true);
		for (int j=0; j<dqdqX[i].size(); j++)
			ss.Add(0, dqdqX[i][j]);
		cout << i << "\t" << ss.AMean(0) << "\t" << ss.SD(0) << endl;
	}
}
