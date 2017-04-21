#ifndef TESTSDATETIME
#define TESTSDATETIME

#include <cmath>
#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "SDateTime.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestSDateTime : public CppUnit::TestFixture
{
  public:
    TestSDateTime();
    virtual ~TestSDateTime();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
