#include "UTest.h"
#include "Math/Atr.hpp"
#include <iostream>
#include <cmath>

int TestATR()
{
	UTest ut;
	Atr<double> a;
	a.Reset(0.1,true);

	ut.AssertF(a.Ready(), __FILE__,__FUNCTION__,__LINE__);
	try
	{
		a.Value();
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& ex) {}

	//should not enter ema at all
	a.Add(0.0000001,1,1);
	a.Add(0.0000001,1,1);
	a.Add(0.0000001,1,1);
	a.Add(0.0000001,1,1);
	a.Add(0.0000001,1,1);
	a.Add(0.0000001,1,1);
	ut.AssertF(a.Ready(), __FILE__,__FUNCTION__,__LINE__);



	a.Add(22141,21991,22006);
	ut.AssertF(a.Ready(), __FILE__,__FUNCTION__,__LINE__);
	a.Add(22150,22020,22111);
	ut.Assert(a.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(a.Value() - 144) < 2, __FILE__,__FUNCTION__,__LINE__);

	a.Add(21833,21506,21822);
	ut.Assert(a.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(a.Value() - 190.1) < 1, __FILE__,__FUNCTION__,__LINE__);

	a.Reset(0.8,true);
	ut.AssertF(a.Ready(), __FILE__,__FUNCTION__,__LINE__);


	try
	{
		a.Reset(8.0,true);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& a) {}

	try
	{
		a.Reset(-1.0,true);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& a) {}





	ut.PrintResult();


	return 0;
}
