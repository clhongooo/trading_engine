#include "UTest.h"
#include "Math/Sma.hpp"
#include <iostream>
#include <cmath>

int TestSMA()
{
	UTest ut;
	Sma<double> s;
	s.Reset(5,true);

	ut.AssertF(s.Ready(), __FILE__,__FUNCTION__,__LINE__);
	try
	{
		s.Value();
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}

	//should not enter sma at all
	s.Add(0.0000001);
	s.Add(0.0000001);
	s.Add(0.0000001);
	s.Add(0.0000001);
	s.Add(0.0000001);
	s.Add(0.0000001);
	ut.AssertF(s.Ready(), __FILE__,__FUNCTION__,__LINE__);


	s.Add(0.35);
	ut.AssertF(s.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(s.Value() - 0.35) > 0.0001, __FILE__,__FUNCTION__,__LINE__);

	s.Add(0.69);
	ut.AssertF(s.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(s.Value() - 0.52) > 0.0001, __FILE__,__FUNCTION__,__LINE__);

	s.Add(0.59);
	ut.AssertF(s.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(s.Value() - 0.54333333) > 0.0001, __FILE__,__FUNCTION__,__LINE__);

	s.Add(0.73);
	ut.AssertF(s.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(s.Value() - 0.59) > 0.0001, __FILE__,__FUNCTION__,__LINE__);

	s.Add(0.75);
	ut.AssertF(!s.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(s.Value() - 0.622) > 0.0001, __FILE__,__FUNCTION__,__LINE__);

	s.Add(0.38);
	ut.AssertF(!s.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(s.Value() - 0.628) > 0.0001, __FILE__,__FUNCTION__,__LINE__);

	s.Add(0.11);
	ut.AssertF(!s.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(s.Value() - 0.512) > 0.0001, __FILE__,__FUNCTION__,__LINE__);

	s.Add(0.97);
	ut.AssertF(!s.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(s.Value() - 0.588) > 0.0001, __FILE__,__FUNCTION__,__LINE__);

	s.Reset(3,true);
	ut.AssertF(s.Ready(), __FILE__,__FUNCTION__,__LINE__);


	ut.PrintResult();


	return 0;
}
