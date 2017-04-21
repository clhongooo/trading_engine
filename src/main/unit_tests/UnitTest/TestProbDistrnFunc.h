#ifndef TESTPROBDISTRNFUNC_H_
#define TESTPROBDISTRNFUNC_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "ProbDistrnFunc.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestProbDistrnFunc : public CppUnit::TestFixture
{
  public:
    TestProbDistrnFunc();
    virtual ~TestProbDistrnFunc();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTPROBDISTRNFUNC_H_ */
