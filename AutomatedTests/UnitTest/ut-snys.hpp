#include "UTest.h"
#include "Util/STimer.h"
#include "Math/RandomEngine.h"
#include "Math/SNYStat.h"
#include <iostream>

using namespace std;
using namespace boost;

int TestSNYS()
{
	RandomEngine re = RandomEngine::Instance();
	UTest ut;
	vector<int> vDelay;
	vDelay.clear(); for (int i = 0; i < 100000; i++) vDelay.push_back(0);

	SNYStat snys(2,vDelay,7,70,true);

	//Test Ready
	ut.AssertF(snys.Ready(), __FILE__,__FUNCTION__,__LINE__);

	for (int i = 0; i < 10; i++) snys.Add(1, 10);
	ut.AssertF(snys.Ready(), __FILE__,__FUNCTION__,__LINE__);
	for (int i = 0; i < 10; i++) snys.Add(0, 10);
	ut.Assert(snys.Ready(), __FILE__,__FUNCTION__,__LINE__);

	//Test Count
	snys.Reset(100,vDelay,100,200,true);
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < i+1; j++)
			snys.Add(i, (double)j+0.5);
	for (int i = 0; i < 99; i++)
		ut.Assert(snys.Count(i) == i+1, __FILE__,__FUNCTION__,__LINE__);


	//Non-existent queue
	snys.Reset(2,vDelay,1,10,true);
	try
	{
		snys.Add(3,10);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}


	//Non-existent queue
	snys.Reset(1,vDelay,7,8,true);
	try
	{
		snys.Add(1,10);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}


	//Test erroneous input
	try
	{
		snys.Reset(0,vDelay,10,11,true);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}
	try
	{
		snys.Reset(3,vDelay,0,1,true);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}
	try
	{
		snys.Reset(0,vDelay,0,1,true);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}



	//Test mean - normal
	snys.Reset(10,vDelay,10,11,true);
	for (int i = 0; i < 10; i++)
		for (int k = 0; k < 1000; k++)
			snys.Add(i % 10, (double) re.NextInt(1,1000));

	for (int i = 0; i < 10; i++)
		ut.Assert(snys.AMean(i) > 0 && snys.AMean(i) < 1000, __FILE__,__FUNCTION__,__LINE__);

/*
	//Test mean - small size
	snys.Reset(10,vDelay,10,11,true);
	snys.Add(0, 46);
	snys.Add(0, 64);
	ut.Assert(snys.AMean(0) == 55, __FILE__,__FUNCTION__,__LINE__);
*/

	//Test Var, SD, Skew, Kurt, SemiVar, Opulent >> Target
	snys.Reset(1,vDelay,8,10000,true);
	snys.Add(0,350);
	snys.Add(0,10);
	snys.Add(0,9);
	snys.Add(0,26);
	snys.Add(0,8);
	snys.Add(0,97);
	snys.Add(0,77);
	snys.Add(0,97);
	snys.Add(0,35);
	//AMean not calculated first
	ut.Assert(abs(snys.SD(0)-39.24807) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.Variance(0)-1540.411) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.Skew(0)-0.351451) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.Kurt(0)- -1.89635) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.SemiVar(0)-539.6035) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.SemiVar0(0)-0) < 0.01, __FILE__,__FUNCTION__,__LINE__);

	//Test Var, SD, Skew, Kurt, SemiVar, Opulent = Target+1
	snys.Reset(1,vDelay,8,9,true);
	snys.Add(0,350);
	snys.Add(0,10);
	snys.Add(0,9);
	snys.Add(0,26);
	snys.Add(0,8);
	snys.Add(0,97);
	snys.Add(0,77);
	snys.Add(0,97);
	snys.Add(0,35);
	ut.Assert(abs(snys.AMean(0)-44.875) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.SD(0)-39.24807) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.Variance(0)-1540.411) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.Skew(0)-0.351451) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.Kurt(0)- -1.89635) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.SemiVar(0)-539.6035) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.SemiVar0(0)-0) < 0.01, __FILE__,__FUNCTION__,__LINE__);


/*
	//Test Var - small size
	snys.Reset(1,vDelay,8,9,true);
	snys.Add(0,46);
	snys.Add(0,64);
	ut.Assert(abs(snys.Variance(0)-81) < 0.01, __FILE__,__FUNCTION__,__LINE__);
*/


	//Test SemiVar0
	snys.Reset(1,vDelay,8,9,false);
	snys.Add(0,-350);
	snys.Add(0,-10);
	snys.Add(0,-9);
	snys.Add(0,-26);
	snys.Add(0,-8);
	snys.Add(0,-97);
	snys.Add(0,-77);
	snys.Add(0,-97);
	snys.Add(0,-35);
	ut.Assert(abs(snys.SemiVar0(0)-3361.625) < 0.01, __FILE__,__FUNCTION__,__LINE__);
	cout << snys.SemiVar0(0) << endl;

	//Test Correl
	snys.Reset(2,vDelay,10,11,true);
	snys.Add(0,61);
	snys.Add(0,59);
	snys.Add(0,54);
	snys.Add(0,27);
	snys.Add(0,93);
	snys.Add(0,73);
	snys.Add(0,75);
	snys.Add(0,11);
	snys.Add(0,80);
	snys.Add(0,52);
	snys.Add(1,37);
	snys.Add(1,8);
	snys.Add(1,82);
	snys.Add(1,13);
	snys.Add(1,16);
	snys.Add(1,62);
	snys.Add(1,61);
	snys.Add(1,93);
	snys.Add(1,55);
	snys.Add(1,65);
	double dCrl;
	bool bCrl = snys.Correl(0,1,dCrl);
	ut.Assert(bCrl && abs(dCrl- -0.28786) < 0.01, __FILE__,__FUNCTION__,__LINE__);


	//Stress test
	snys.Reset(100,vDelay,1000,1001,true);
	cout << "Begin Stress Test: " << STimer::CurrentTimeHMS() << endl;
	for (int i = 0; i < 100; i++)
		for (int k = 0; k < 500; k++)
		{
			snys.Add(i % 100, re.NextInt(1,10000));
			if (snys.Ready())
			{
				snys.AMean(i % 100);
				snys.AMean(i % 100);
			}
		}
	cout << "  End Stress Test: " << STimer::CurrentTimeHMS() << endl;



	//Test Delay
	vDelay.clear();
	vDelay.push_back(0);
	vDelay.push_back(1);
	vDelay.push_back(2);
	snys.Reset(3,vDelay,5,8,true);
	snys.Add(0,15);
	snys.Add(0,55);
	snys.Add(0,17);
	snys.Add(0,84);
	snys.Add(0,56);
	snys.Add(0,14);
	snys.Add(0,28);
	snys.Add(0,49);
	snys.Add(0,8);
	snys.Add(0,95);
	snys.Add(1,64);
	snys.Add(1,68);
	snys.Add(1,89);
	snys.Add(1,10);
	snys.Add(1,37);
	snys.Add(1,68);
	snys.Add(1,33);
	snys.Add(1,93);
	snys.Add(1,33);
	snys.Add(1,19);
	snys.Add(2,80);
	snys.Add(2,60);
	snys.Add(2,79);
	snys.Add(2,6);
	snys.Add(2,13);
	snys.Add(2,42);
	snys.Add(2,62);
	snys.Add(2,29);
	snys.Add(2,93);
	snys.Add(2,81);

	ut.Assert(abs(snys.AMean(0)-38.8) < 0.1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.AMean(1)-52.8) < 0.1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.AMean(2)-30.4) < 0.1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.SD(0)-35.15) < 0.1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.SD(1)-26.83) < 0.1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(snys.SD(2)-22.54) < 0.1, __FILE__,__FUNCTION__,__LINE__);
	bool bCrl2; double dCrl2;
	bCrl2 = snys.Correl(0,2,dCrl2);
	ut.Assert(bCrl && abs(dCrl2- -0.04435) < 0.01, __FILE__,__FUNCTION__,__LINE__);
  if (bCrl2) bCrl2 = true; //dummy use to avoid compiler warning





	ut.PrintResult();


	return 0;
}
