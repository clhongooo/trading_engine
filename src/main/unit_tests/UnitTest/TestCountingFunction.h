#ifndef UNITTEST_TESTCOUNTINGFUNCTION_H_
#define UNITTEST_TESTCOUNTINGFUNCTION_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "CountingFunction.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestCountingFunction {
public:
	TestCountingFunction();
	virtual ~TestCountingFunction();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* UNITTEST_TESTCOUNTINGFUNCTION_H_ */
