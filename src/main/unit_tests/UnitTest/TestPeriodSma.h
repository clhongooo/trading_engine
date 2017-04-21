#ifndef TESTPERIODSMA_H
#define TESTPERIODSMA_H

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "PeriodSma.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestPeriodSma : public CppUnit::TestFixture
{
  public:
    TestPeriodSma();
    virtual ~TestPeriodSma();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
