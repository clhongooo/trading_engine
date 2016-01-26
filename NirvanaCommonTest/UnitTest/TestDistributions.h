#ifndef TESTCAUCHY_H_
#define TESTCAUCHY_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "GaussianDistribution.h"
#include "CauchyDistribution.h"
#include "GumbelCopula.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>


class TestDistributions : public CppUnit::TestFixture
{
  public:
    TestDistributions();
    virtual ~TestDistributions();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
