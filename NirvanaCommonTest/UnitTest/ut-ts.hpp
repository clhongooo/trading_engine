#include "UTest.h"
#include "Math/TrendStren.hpp"
#include "Math/RandomEngine.h"
#include <iostream>
#include <cmath>

using namespace std;

int TestTS()
{
	UTest ut;
	TrendStren<double> s(20,10,2,true);

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


	RandomEngine re = RandomEngine::Instance();
	for (int i = 0; i < 100; i++)
	{
		try
		{
			s.Add(re.NextInt(1,1000));
			if (i > 50) cout << s.Value() << ", ";
		} catch (std::exception& e) {}
	}

	s.Reset(5,10,2,true);
	ut.AssertF(s.Ready(), __FILE__,__FUNCTION__,__LINE__);


	ut.PrintResult();


	return 0;
}
