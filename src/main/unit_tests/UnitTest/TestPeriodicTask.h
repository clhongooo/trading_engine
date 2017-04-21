#ifndef UNITTEST_TESTPERIODICTASK_H_
#define UNITTEST_TESTPERIODICTASK_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "PeriodicTask.h"
#include "SDateTime.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestPeriodicTask {
public:
	TestPeriodicTask();
	virtual ~TestPeriodicTask();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* UNITTEST_TESTPERIODICTASK_H_ */
