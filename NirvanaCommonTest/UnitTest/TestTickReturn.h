#ifndef TESTTICKRETURN
#define TESTTICKRETURN

#include <cmath>
#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "TickReturn.h"
#include "SDateTime.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestTickReturn : public CppUnit::TestFixture
{
  public:
    TestTickReturn();
    virtual ~TestTickReturn();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
