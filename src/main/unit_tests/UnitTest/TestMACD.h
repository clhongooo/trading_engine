#ifndef TESTMACD_H
#define TESTMACD_H

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "MACD.h"
#include "BarProvider.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestMACD : public CppUnit::TestFixture
{
  public:
    TestMACD();
    virtual ~TestMACD();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
