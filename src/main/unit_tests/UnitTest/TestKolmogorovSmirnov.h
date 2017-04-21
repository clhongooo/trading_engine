#ifndef UNITTEST_TESTKOLMOGOROVSMIRNOV_H_
#define UNITTEST_TESTKOLMOGOROVSMIRNOV_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "KolmogorovSmirnov.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestKolmogorovSmirnov : public CppUnit::TestFixture
{
  public:
    TestKolmogorovSmirnov();
    virtual ~TestKolmogorovSmirnov();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* UNITTEST_TESTKOLMOGOROVSMIRNOV_H_ */
