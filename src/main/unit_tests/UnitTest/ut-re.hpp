#include "UTest.h"
#include "Math/RandomEngine.h"
#include <iostream>
#include <cmath>

int TestRE()
{
	UTest ut;
	RandomEngine re = RandomEngine::Instance();

	cout << "Sample: " << endl;
	for (int i = 0; i < 10; i++)
	{
		int iR = re.NextInt(1,100);
		cout << iR << ", ";
	}
	cout << endl;

	for (int i = 0; i < 100000; i++)
	{
		int iR = re.NextInt(1,100);
		ut.AssertF(iR >= 100 || iR < 1, __FILE__,__FUNCTION__,__LINE__);
	}

	for (int i = 0; i < 100000; i++)
	{
		int iR = re.NextInt(51,100);
		ut.AssertF(iR >= 100 || iR < 51, __FILE__,__FUNCTION__,__LINE__);
	}

	long lCnt = 0;
	for (int i = 0; i < 1000000; i++)
	{
		double dR = re.Rand01();
		if (dR >= 0.5) lCnt++;
		ut.AssertF(dR > 1 || dR < 0, __FILE__,__FUNCTION__,__LINE__);
	}
	ut.Assert(abs(lCnt-500000) < 1000, __FILE__,__FUNCTION__,__LINE__);


	long lTrueCnt = 0;
	long lFalseCnt = 0;
	for (int i = 0; i < 1000000; i++)
		if (re.CoinFlip()) lTrueCnt++; else lFalseCnt++;
	ut.AssertF(abs((double)lTrueCnt/(double)lFalseCnt - 1) > 0.01, __FILE__,__FUNCTION__,__LINE__);
	cout << "Coin Flip: " << lTrueCnt << ":" << lFalseCnt << ", " << lTrueCnt-lFalseCnt << endl;


	lTrueCnt = 0;
	lFalseCnt = 0;
	for (int i = 0; i < 1000000; i++)
	{
		re = RandomEngine::Instance();
		if (re.CoinFlip()) lTrueCnt++; else lFalseCnt++;
	}
	ut.AssertF(abs((double)lTrueCnt/(double)lFalseCnt - 1) > 0.01, __FILE__,__FUNCTION__,__LINE__);
	cout << "Coin Flip: " << lTrueCnt << ":" << lFalseCnt << ", " << lTrueCnt-lFalseCnt << endl;






	ut.PrintResult();


	return 0;
}
