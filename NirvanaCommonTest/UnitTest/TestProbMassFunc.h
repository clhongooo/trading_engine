#ifndef TESTPROBMASSFUNC_H_
#define TESTPROBMASSFUNC_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "VolSurf.h"
#include <cmath>
#include "ProbMassFunc.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestProbMassFunc : public CppUnit::TestFixture
{
  public:
    TestProbMassFunc();
    virtual ~TestProbMassFunc();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
