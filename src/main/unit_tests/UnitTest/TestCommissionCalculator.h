#ifndef UNITTEST_TESTCOMMISSIONCALCULATOR_H_
#define UNITTEST_TESTCOMMISSIONCALCULATOR_H_

#include <cmath>
#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "CommissionCalculator.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestCommissionCalculator : public CppUnit::TestFixture
{
public:
	TestCommissionCalculator();
	virtual ~TestCommissionCalculator();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* UNITTEST_TESTCOMMISSIONCALCULATOR_H_ */
