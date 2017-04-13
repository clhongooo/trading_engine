/*
 * TestSysConfig.cpp
 *
 *  Created on: May 12, 2014
 *      Author: sunny
 */

#include "TestSysConfig.h"

TestSysConfig::TestSysConfig() {
  // TODO Auto-generated constructor stub

}

TestSysConfig::~TestSysConfig() {
  // TODO Auto-generated destructor stub
}

void TestSysConfig::RunTest()
{
  //--------------------------------------------------
  // Test ReadConfig
  //--------------------------------------------------

  shared_ptr<SystemConfig> pSysCfg  = SystemConfig::Instance();
  //pSysCfg->Reset("Testing/Config-SysCfg1.1.ini");


  CPPUNIT_ASSERT(pSysCfg->GetMcastIdentifiers()->size()               ==   36    ); 
  CPPUNIT_ASSERT(pSysCfg->GetActiveMcastChnl()->size()                ==   9   ); 


  // CPPUNIT_ASSERT(pSysCfg->GetPreProcessorRTCannedChnl()               ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetPreProcessorRFCannedChnl()               ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRealTimeProcCannedChnl()                 ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRefreshProcCannedChnl()                  ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetMcastRecvrPrintPktHdr()                  ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetPreProcessorRTJson()                     ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetPreProcessorRFJson()                     ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRealTimeProcessorJson()                  ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRefreshProcessorJson()                   ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRTSClientJson()                          ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetMemoryBlockSize()                        ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRefreshProcSleepMillisec()               ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetDataComplInspectorSleepMillisec()        ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRTSClientSleepMillisec()                 ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetCannedMcastFilePath()                    ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetCannedProcessedDataFilePath()            ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetCannedRefreshProcFilePath()              ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetCannedMcastFopenFlag()                   ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetCannedProcessedDataFopenFlag()           ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetCannedRefreshProcFopenFlag()             ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetTriggerRetransmissionSeqNoGap()          ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetTriggerRetransmissionTimeGapMillisec()   ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetTriggerRefreshSeqNoGap()                 ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetTriggerRefreshTimeGapMillisec()          ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRTSRequestMaxSeqNoRange()                ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRTSRequestMaxMsg()                       ==       ); 
  // CPPUNIT_ASSERT(pSysCfg->GetRTSMaxNumRequestPerDay()                 ==       ); 

}

void TestSysConfig::setUp()
{
}
void TestSysConfig::tearDown()
{
}

CppUnit::Test *TestSysConfig::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSysConfig");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestSysConfig>(
        "TestSysConfig", 
        &TestSysConfig::RunTest));

  return suiteOfTests;
}
