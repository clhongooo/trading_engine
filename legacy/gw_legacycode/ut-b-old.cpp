#include <iostream>
#include "UTest.hpp"
#include "../corelib/Bar.hpp"

int main()
{
	UTest ut;
  Bar<double,long> b("2012-12-31", "13:58:11.968544", 20000, 20100,19900,20010,123);

	ut.Assert(b.YYYYMMDDs().compare("2012-12-31") == 0, __LINE__);
	ut.Assert(b.YMDi() == 20121231, __LINE__);
	ut.Assert(b.YMi() == 201212, __LINE__);
	ut.Assert(b.HMSi() == 135811, __LINE__);
	ut.Assert(b.HMi() == 1358, __LINE__);
	ut.Assert(b.Year() == 2012, __LINE__);
	ut.Assert(b.Month() == 12, __LINE__);
	ut.Assert(b.Day() == 31, __LINE__);
	ut.Assert(b.Hour() == 13, __LINE__);
	ut.Assert(b.Minute() == 58, __LINE__);
	ut.Assert(b.Second() == 11, __LINE__);
	ut.Assert(b.Open == 20000, __LINE__);
	ut.Assert(b.High == 20100, __LINE__);
	ut.Assert(b.Low == 19900, __LINE__);
	ut.Assert(b.Close == 20010, __LINE__);
	ut.Assert(b.Volume == 123, __LINE__);

  Bar<double,long> b2("2012-2-3", "", 20000, 20100,19900,20010,123);
	ut.Assert(b2.YMDi() == 20120203, __LINE__);
	ut.Assert(b2.YMi() == 201202, __LINE__);
	ut.Assert(b2.HMSi() == 0, __LINE__);
	ut.Assert(b2.HMi() == 0, __LINE__);
	ut.Assert(b2.Year() == 2012, __LINE__);
	ut.Assert(b2.Month() == 2, __LINE__);
	ut.Assert(b2.Day() == 3, __LINE__);
	ut.Assert(b2.Hour() == 0, __LINE__);
	ut.Assert(b2.Minute() == 0, __LINE__);
	ut.Assert(b2.Second() == 0, __LINE__);

  Bar<double,long> b3("2012-2-3", "1:6:8", 20000, 20100,19900,20010,123);
	ut.Assert(b3.YMDi() == 20120203, __LINE__);
	ut.Assert(b3.YMi() == 201202, __LINE__);
	ut.Assert(b3.HMSi() == 10608, __LINE__);
	ut.Assert(b3.HMi() == 106, __LINE__);
	ut.Assert(b3.Hour() == 1, __LINE__);
	ut.Assert(b3.Minute() == 6, __LINE__);
	ut.Assert(b3.Second() == 8, __LINE__);

	try
	{
		Bar<double,long> b4("2012-2-", "1:6:8", 20000, 20100,19900,20010,123);
	} catch (std::exception &e) {}
	try
	{
		Bar<double,long> b5("2012-2-", "::8", 20000, 20100,19900,20010,123);
	} catch (std::exception &e) {}

	ut.PrintResult();

	for (int i = 0; i < 100; i++)
	{
		auto_ptr<Bar<double,long> > bb1(new Bar<double,long>("2012-05-06", "13:12:11.968544", 20000, 20100,19900,20010,123));
		Bar<double,long>* bb2 = new Bar<double,long>("2012-05-06", "13:12:11.968544", 20000, 20100,19900,20010,123);
		delete bb2;
	}


	return 0;
}
