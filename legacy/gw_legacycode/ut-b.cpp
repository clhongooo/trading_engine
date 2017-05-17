#include <iostream>
#include "UTest.hpp"
#include "../corelib/Bar.hpp"

int main()
{
	UTest ut;

	for (unsigned long i = 0; i < 99; i++)
	{
		Bar b(20.01,35.1,10.4,23.3,567);
		ut.Assert(b.Open == 20.01, __LINE__);
		ut.Assert(b.High == 35.1, __LINE__);
		ut.Assert(b.Low == 10.4, __LINE__);
		ut.Assert(b.Close == 23.3, __LINE__);
		ut.Assert(b.Volume == 567, __LINE__);

		FBar fb(20130830,95631,20.01,35.1,10.4,23.3,567);
		ut.Assert(fb.YMDi == 20130830, __LINE__);
		ut.Assert(fb.HMSi == 95631, __LINE__);
		ut.Assert(fb.Open == 20.01, __LINE__);
		ut.Assert(fb.High == 35.1, __LINE__);
		ut.Assert(fb.Low == 10.4, __LINE__);
		ut.Assert(fb.Close == 23.3, __LINE__);
		ut.Assert(fb.Volume == 567, __LINE__);
	}


	ut.PrintResult();


	return 0;
}
