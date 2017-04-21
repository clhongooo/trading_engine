#ifndef TESTLINEARSYSSOLVER_H_
#define TESTLINEARSYSSOLVER_H_

#include <cmath>
#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "LinearSysSolver.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestLinearSysSolver : public CppUnit::TestFixture
{
  public:
    TestLinearSysSolver();
    virtual ~TestLinearSysSolver();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTLINEARSYSSOLVER_H_ */
