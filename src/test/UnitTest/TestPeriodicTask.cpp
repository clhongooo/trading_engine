#include "TestPeriodicTask.h"

TestPeriodicTask::TestPeriodicTask()
{
}

TestPeriodicTask::~TestPeriodicTask()
{
}

void TestPeriodicTask::RunTest()
{
  {
    //--------------------------------------------------
    // Normal cases
    //--------------------------------------------------
    PeriodicTask pt;
    YYYYMMDDHHMMSS ymdhms("2015-11-17","12:17:00");

    const int ciToken1 = pt.GetNewTokenAndSetIntervalInSec(10, ymdhms);
    const int ciToken2 = pt.GetNewTokenAndSetIntervalInSec(30, ymdhms);
    const int ciToken3 = pt.GetNewTokenAndSetIntervalInSec(60, ymdhms);

    ymdhms.Set("2015-11-17","12:17:05");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

    ymdhms.Set("2015-11-17","12:17:09");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

    ymdhms.Set("2015-11-17","12:17:10");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

    ymdhms.Set("2015-11-17","12:17:19");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

    ymdhms.Set("2015-11-17","12:17:20");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

    ymdhms.Set("2015-11-17","12:17:29");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

    ymdhms.Set("2015-11-17","12:17:31");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

    ymdhms.Set("2015-11-17","12:25:59");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

    ymdhms.Set("2015-11-17","12:29:59");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

    ymdhms.Set("2015-11-17","12:30:00");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken2, ymdhms));
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken3, ymdhms));

  }

  {
    //--------------------------------------------------
    // Test wrapping around the 00:00
    //--------------------------------------------------
    PeriodicTask pt;
    YYYYMMDDHHMMSS ymdhms("2015-11-17","23:59:00");

    const int ciToken1 = pt.GetNewTokenAndSetIntervalInSec(10, ymdhms);

    ymdhms.Set("2015-11-17","23:59:09");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","23:59:11");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","23:59:50");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","23:59:59");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-18","00:00:01");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-18","00:00:03");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-18","00:00:07");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-18","00:00:10");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

  }

  {
    //--------------------------------------------------
    // Test initial bad date time
    //--------------------------------------------------
    PeriodicTask pt;
    YYYYMMDDHHMMSS ymdhms;

    const int ciToken1 = pt.GetNewTokenAndSetIntervalInSec(10, ymdhms);

    for (unsigned int i = 20; i < 10000; ++i)
    {
      pt.GetNewTokenAndSetIntervalInSec(i, ymdhms);
      CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));
    }

    ymdhms.Set("2015-11-17","13:59:09");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","13:59:18");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","13:59:19");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","13:59:20");
    CPPUNIT_ASSERT(!pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","14:05:08");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","14:05:09");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","14:07:04");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

    ymdhms.Set("2015-11-17","14:09:09");
    CPPUNIT_ASSERT( pt.CheckIfItIsTimeToWakeUp(ciToken1, ymdhms));

  }




  return;
}

void TestPeriodicTask::setUp()
{
}

void TestPeriodicTask::tearDown()
{
}

CppUnit::Test *TestPeriodicTask::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestPeriodicTask");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestPeriodicTask>(
        "TestPeriodicTask", 
        &TestPeriodicTask::RunTest));

  return suiteOfTests;
}
