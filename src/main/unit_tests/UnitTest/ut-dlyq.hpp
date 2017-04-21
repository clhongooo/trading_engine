#include "UTest.h"
#include "Math/RandomEngine.h"
#include "Math/DelayedCirDataQue.hpp"
#include <iostream>

using namespace std;

int TestDlydQ()
{
	RandomEngine re = RandomEngine::Instance();
	UTest ut;
	DelayedCirDataQue<double> dq1;

	ut.AssertF(dq1.Ready(), __FILE__,__FUNCTION__,__LINE__);

	for (int i = 0; i < 10; i++) dq1.Add(10);
	ut.AssertF(dq1.Ready(), __FILE__,__FUNCTION__,__LINE__);

	try {
	dq1.Reset(10,true); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch(exception &e) {}
	

	//Half full, no opulent vector
	dq1.Reset(10,2,true);
	for (int i = 0; i < 3; i++) dq1.Add(re.NextInt(1,1000));
	ut.Assert(dq1.Count() == 1, __FILE__,__FUNCTION__,__LINE__);
	for (int i = 0; i < 2; i++) dq1.Add(re.NextInt(1,1000));
	ut.Assert(dq1.Count() == 3, __FILE__,__FUNCTION__,__LINE__);
	for (int i = 0; i < 8; i++) dq1.Add(re.NextInt(1,1000));
	ut.Assert(dq1.Count() == 10, __FILE__,__FUNCTION__,__LINE__);
	for (int i = 0; i < 5; i++) dq1.Add(re.NextInt(1,1000));
	ut.Assert(dq1.Count() == 10, __FILE__,__FUNCTION__,__LINE__);


	//Half full, opulent vector
	dq1.Reset(10,100,2,true);
	for (int i = 0; i < 3; i++) dq1.Add(re.NextInt(1,1000));
	ut.Assert(dq1.Count() == 1, __FILE__,__FUNCTION__,__LINE__);
	for (int i = 0; i < 2; i++) dq1.Add(re.NextInt(1,1000));
	ut.Assert(dq1.Count() == 3, __FILE__,__FUNCTION__,__LINE__);
	for (int i = 0; i < 8; i++) dq1.Add(re.NextInt(1,1000));
	ut.Assert(dq1.Count() == 11, __FILE__,__FUNCTION__,__LINE__);
	for (int i = 0; i < 5; i++) dq1.Add(re.NextInt(1,1000));
	ut.Assert(dq1.Count() == 16, __FILE__,__FUNCTION__,__LINE__);


	//Get array, no opulent vector
	cout << "GetArrayUnord" << endl;
	dq1.Reset(10,2,true);
	int iSz;
	for (int i = 1; i < 100; i++) dq1.Add(i);
	const double *dA = dq1.GetArrayUnord(iSz);
	for (int i = 0; i < 9; i++) cout << dA[i] << endl;
	ut.Assert(dA[0] == 91, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dA[1] == 92, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dA[2] == 93, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dA[3] == 94, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dA[4] == 95, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dA[5] == 96, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dA[6] == 97, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dA[7] == 88, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dA[8] == 89, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dA[9] == 90, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(iSz == 10, __FILE__,__FUNCTION__,__LINE__);

	//Get array, opulent vector
	cout << "GetArrayInInsOrd" << endl;
	dq1.Reset(10,50,2,true);
	for (int i = 1; i < 100; i++) dq1.Add(i);
	const double *dB = dq1.GetArrayInInsOrd();
	for (int i = 0; i < 10; i++) cout << dB[i] << endl;
	ut.Assert(dB[0] == 88, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dB[1] == 89, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dB[2] == 90, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dB[3] == 91, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dB[4] == 92, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dB[5] == 93, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dB[6] == 94, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dB[7] == 95, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dB[8] == 96, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dB[9] == 97, __FILE__,__FUNCTION__,__LINE__);


	//Get array, opulent vector second time
	dq1.Reset(10,30,2,true);
	for (int i = 1; i < 100; i++) dq1.Add(i);
	const double *dC = dq1.GetArrayInInsOrd();
	for (int i = 0; i < 10; i++) cout << dC[i] << endl;
	ut.Assert(dC[0] == 88, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dC[1] == 89, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dC[2] == 90, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dC[3] == 91, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dC[4] == 92, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dC[5] == 93, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dC[6] == 94, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dC[7] == 95, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dC[8] == 96, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(dC[9] == 97, __FILE__,__FUNCTION__,__LINE__);




	//Stress test
	dq1.Reset(5000,30,true);
	for (int i = 0; i < 6000; i++) dq1.Add(re.NextInt(1,10000));
	ut.Assert(dq1.Count() == 5000, __FILE__,__FUNCTION__,__LINE__);

	//Test erroneous input
	dq1.Reset(20,4,true);
	dq1.Add(-4);
	dq1.Add(-4.5);
	dq1.Add(0.00000001);
	ut.Assert(dq1.Count() == 0, __FILE__,__FUNCTION__,__LINE__);



	ut.PrintResult();


	return 0;
}
