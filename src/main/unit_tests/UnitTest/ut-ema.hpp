#include "UTest.h"
#include "Math/Ema.hpp"
#include <iostream>
#include <cmath>

int TestEMA()
{
	UTest ut;
	Ema<double> e;
	e.Reset(0.3,true);

	ut.AssertF(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	try
	{
		e.Value();
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& ex) {}

	//should not enter ema at all
	e.Add(0.0000001);
	e.Add(0.0000001);
	e.Add(0.0000001);
	e.Add(0.0000001);
	e.Add(0.0000001);
	e.Add(0.0000001);
	ut.AssertF(e.Ready(), __FILE__,__FUNCTION__,__LINE__);



	e.Add(0.35);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.35) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Add(0.69);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.452) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Add(0.59);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.4934) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Add(0.73);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.56438) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Add(0.75);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.620066) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Add(0.38);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.548046) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Add(0.11);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.416632) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Add(0.97);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.582643) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Reset(0.8,true);
	ut.AssertF(e.Ready(), __FILE__,__FUNCTION__,__LINE__);


	try
	{
		e.Reset(8.0,true);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}

	try
	{
		e.Reset(-1.0,true);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}



	//SetAlpha
	e.Reset(0.5,true);
	e.SetAlpha(0.3);

	e.Add(0.35);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.35) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Add(0.69);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.452) < 0.0001, __FILE__,__FUNCTION__,__LINE__);

	e.Add(0.59);
	ut.Assert(e.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(e.Value() - 0.4934) < 0.0001, __FILE__,__FUNCTION__,__LINE__);



	ut.PrintResult();


	return 0;
}
