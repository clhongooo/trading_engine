#include "TestRglrMiniCtrtMgr.h"

TestRglrMiniCtrtMgr::TestRglrMiniCtrtMgr() {

}

TestRglrMiniCtrtMgr::~TestRglrMiniCtrtMgr() {
}

void TestRglrMiniCtrtMgr::RunTest()
{
  {
    RegularMiniCtrtManager rmcm;
    rmcm.SetMode(RegularMiniCtrtManager::RMCM_OPTZMARGINREQ);

    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("DUMMY") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",3.4);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 2);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-2.5);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == -2);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == -2);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0.1);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0.2);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 1);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-0.3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == -1);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-0.4);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == -2);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-9.9);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == -9);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == -4);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);
  }
  {
    RegularMiniCtrtManager rmcm;
    rmcm.SetMode(RegularMiniCtrtManager::RMCM_OPTZDELTA);

    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("DUMMY") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",3.4);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 2);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-2.5);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == -3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0.1);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == -1);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 5);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0.2);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == -1);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 6);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-0.3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == -1);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 4);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-0.4);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == -1);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 3);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);

    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-9.9);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == -10);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 1);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HHIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MCHc1") == 0);
  }
  {
    RegularMiniCtrtManager rmcm;
    rmcm.SetMode(RegularMiniCtrtManager::RMCM_OPTZDELTA);

    //--------------------------------------------------
    // Testing round off
    //--------------------------------------------------
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0.4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",1.2);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",1.6);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",2.0);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",2.4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",2.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",3.2);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",3.6);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",4.0);
    rmcm.NetOffRndOffRegularMini();
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 4);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 0);
  }
  {
    RegularMiniCtrtManager rmcm;
    rmcm.SetMode(RegularMiniCtrtManager::RMCM_OPTZDELTA);

    //--------------------------------------------------
    // Testing round off
    //--------------------------------------------------
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-0.4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-0.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-1.2);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-1.6);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-2.0);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-2.4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-2.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-3.2);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-3.6);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-4.0);
    rmcm.NetOffRndOffRegularMini();
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == -4);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 0);
  }
  {
    RegularMiniCtrtManager rmcm;
    rmcm.SetMode(RegularMiniCtrtManager::RMCM_OPTZDELTA);

    //--------------------------------------------------
    // Testing net off
    //--------------------------------------------------
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",3.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",3.6);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",3.2);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",2.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",2.4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",2.0);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",1.6);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",1.2);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0.4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0);
    rmcm.NetOffRndOffRegularMini();
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 0);
  }
{
    RegularMiniCtrtManager rmcm;
    rmcm.SetMode(RegularMiniCtrtManager::RMCM_OPTZDELTA);

    //--------------------------------------------------
    // Testing net off
    //--------------------------------------------------
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-3.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-3.6);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-3.2);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-2.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-2.4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-2.0);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-1.6);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-1.2);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-0.8);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",-0.4);
    rmcm.UpdateRegularCtrtTgtSgnl("HSIc1",0);
    rmcm.NetOffRndOffRegularMini();
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("HSIc1") == 0);
    CPPUNIT_ASSERT(rmcm.GetSuggestedCtrtSize("MHIc1") == 0);
  }



  return;
}

void TestRglrMiniCtrtMgr::setUp()
{
}

void TestRglrMiniCtrtMgr::tearDown()
{
}

CppUnit::Test *TestRglrMiniCtrtMgr::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestRglrMiniCtrtMgr");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestRglrMiniCtrtMgr>(
      "TestRglrMiniCtrtMgr",
      &TestRglrMiniCtrtMgr::RunTest));

  return suiteOfTests;
}
