/*
 * TestSharedObjects.cpp
 *
 *  Created on: May 12, 2014
 *      Author: sunny
 */

#include "TestSharedObjects.h"

TestSharedObjects::TestSharedObjects()
{
}

TestSharedObjects::~TestSharedObjects()
{
}

void TestSharedObjects::RunTest()
{
  shared_ptr<SystemConfig> pSysCfg  = SystemConfig::Instance();
  unsigned int uiChannelID          = 91;

  //--------------------------------------------------
  // 1.1: Test empty LatestSeqNoOffset
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: SharedObjects: 1.1" << endl << flush;
    UTest ut;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-SO1.1.ini");

    //--------------------------------------------------
    // RT
    //--------------------------------------------------
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
    pShrObj->AddRTChnlSeqNoOffset(uiChannelID, 9413);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 9413);
    pShrObj->AddRTChnlSeqNoOffset(uiChannelID, 1314);
    pShrObj->AddRTChnlSeqNoOffset(uiChannelID, 1516);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 1516);
    pShrObj->AddRTChnlSeqNoOffset(uiChannelID, 709394);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 709394);

    pShrObj->ResetRTChnlSeqNoOffset();

    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 0);
    pShrObj->AddRTChnlSeqNoOffset(uiChannelID, 9413);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 9413);
    pShrObj->AddRTChnlSeqNoOffset(uiChannelID, 1314);
    pShrObj->AddRTChnlSeqNoOffset(uiChannelID, 1516);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 1516);
    pShrObj->AddRTChnlSeqNoOffset(uiChannelID, 709394);
    CPPUNIT_ASSERT(pShrObj->CheckRTChnlLatestSeqNoOffset(uiChannelID) == 709394);

    //--------------------------------------------------
    // RF
    //--------------------------------------------------
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);
    pShrObj->AddRFChnlSeqNoOffset(uiChannelID, 9413);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 9413);
    pShrObj->AddRFChnlSeqNoOffset(uiChannelID, 1314);
    pShrObj->AddRFChnlSeqNoOffset(uiChannelID, 1516);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 1516);
    pShrObj->AddRFChnlSeqNoOffset(uiChannelID, 709394);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 709394);

    pShrObj->ResetRFChnlSeqNoOffset();

    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 0);
    pShrObj->AddRFChnlSeqNoOffset(uiChannelID, 9413);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 9413);
    pShrObj->AddRFChnlSeqNoOffset(uiChannelID, 1314);
    pShrObj->AddRFChnlSeqNoOffset(uiChannelID, 1516);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 1516);
    pShrObj->AddRFChnlSeqNoOffset(uiChannelID, 709394);
    CPPUNIT_ASSERT(pShrObj->CheckRFChnlLatestSeqNoOffset(uiChannelID) == 709394);


    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

  //--------------------------------------------------
  // 1.1: Test seq reset already handled
  //--------------------------------------------------
  {
    cout << __FILE__ << ": Unit Test: SharedObjects: 2.1" << endl << flush;
    UTest ut;
    shared_ptr<SharedObjects> pShrObj = SharedObjects::Instance();
    pSysCfg->ReadConfigOptional("Testing/Config-SO2.1.ini");

    //--------------------------------------------------
    // RT
    //--------------------------------------------------
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID, 100000) == false);
    pShrObj->AddRTSeqResetAlreadyHandled(uiChannelID, 100000);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID, 100000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID, 160000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID,  40000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID, 170000) == false);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID,  30000) == false);

    pShrObj->ResetRTSeqResetAlreadyHandled();

    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID, 100000) == false);
    pShrObj->AddRTSeqResetAlreadyHandled(uiChannelID, 100000);
    pShrObj->AddRTSeqResetAlreadyHandled(uiChannelID, 110000);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID, 100000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID, 170000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID,  40000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID, 180000) == false);
    CPPUNIT_ASSERT(pShrObj->CheckRTSeqResetAlreadyHandled(uiChannelID,  30000) == false);

    //--------------------------------------------------
    // RF
    //--------------------------------------------------
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID, 100000) == false);
    pShrObj->AddRFSeqResetAlreadyHandled(uiChannelID, 100000);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID, 100000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID, 160000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID,  40000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID, 170000) == false);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID,  30000) == false);

    pShrObj->ResetRFSeqResetAlreadyHandled();

    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID, 100000) == false);
    pShrObj->AddRFSeqResetAlreadyHandled(uiChannelID, 100000);
    pShrObj->AddRFSeqResetAlreadyHandled(uiChannelID, 110000);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID, 100000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID, 170000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID,  40000) == true);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID, 180000) == false);
    CPPUNIT_ASSERT(pShrObj->CheckRFSeqResetAlreadyHandled(uiChannelID,  30000) == false);

    if (!ut.ChkResult()) ut.PrintResult();
    CPPUNIT_ASSERT(ut.ChkResult());
  }

}

void TestSharedObjects::setUp()
{
}

void TestSharedObjects::tearDown()
{
}

CppUnit::Test *TestSharedObjects::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSharedObjects");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestSharedObjects>(
        "TestSharedObjects", 
        &TestSharedObjects::RunTest));

  return suiteOfTests;
}
