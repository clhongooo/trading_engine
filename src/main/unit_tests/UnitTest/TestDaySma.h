#ifndef TESTDAYSMA_H
#define TESTDAYSMA_H

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "DaySma.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestDaySma : public CppUnit::TestFixture
{
  public:
    TestDaySma();
    virtual ~TestDaySma();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
