#ifndef TESTBARAGGREGATOR_H_
#define TESTBARAGGREGATOR_H_

#include <cmath>
#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "BarAggregator.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestBarAggregator : public CppUnit::TestFixture
{
  public:
    TestBarAggregator();
    virtual ~TestBarAggregator();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};
#endif
