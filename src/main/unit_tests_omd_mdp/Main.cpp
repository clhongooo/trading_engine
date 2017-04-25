//**************************************************
//  Author:      Sunny Yan
//  Created On:  Fri Apr 11 20:28:13 HKT 2014
//  Description: Unit tests for the code in omd_common
//
//**************************************************
#include "BinaryTools.h"
#include "Testing/UTest/UTest.h"

#include "MemoryManagement/CentralMemMgr.h"

#include "TestExpandableCirBuffer.hpp"
#include "Container/ExpandableCirBuffer.h"
#include "TestExpandableCirBuffer4Msg.hpp"
#include "Container/ExpandableCirBuffer4Msg.h"


#include "Testing/UTest/UTest.h"
#include "Testing/RTSTestingServer/RTSTestingServer.h"

#include "Testing/UnitTest/TestSwitchRTSSvr.h"
#include "Testing/UnitTest/TestTriggeringRTSClient.h"
#include "Testing/UnitTest/TestRefresh.h"
#include "Testing/UnitTest/TestRealTime.h"
#include "Testing/UnitTest/TestSysConfig.h"
#include "Testing/UnitTest/TestMsgHandling.h"
#include "Testing/UnitTest/TestOrderBook.h"
#include "Testing/UnitTest/TestOrderBookOMDDSP.h"
#include "Testing/UnitTest/TestSeqReset.h"
#include "Testing/UnitTest/TestRTSResponse.h"
#include "Testing/UnitTest/TestSharedObjects.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestResult.h>

#include <boost/shared_ptr.hpp>
#include <deque>
#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;
using namespace boost;

int main(int argc, const char* argv[])
{

  //--------------------------------------------------
  // Circular Buffer
  //--------------------------------------------------
  {
    UTest ut;
    boost::shared_ptr<CentralMemMgr> cmm = CentralMemMgr::Instance();
    extern const unsigned int MSGSIZE;

    // //--------------------------------------------------
    // // Test ExpandableCirBuffer
    // //--------------------------------------------------
    // ecbPkt = new ExpandableCirBuffer(91,3,sizeof(uint32_t)*2,cmm);
    // TestExpandableCirBuffer(ut);
    // delete ecbPkt;
    //
    // ecbPkt = new ExpandableCirBuffer(91,65536,sizeof(uint32_t)*2,cmm);
    // TestExpandableCirBuffer(ut);
    // delete ecbPkt;
    // //--------------------------------------------------

    //--------------------------------------------------
    // Test ExpandableCirBuffer4Msg
    //--------------------------------------------------
    ecbMsg  = new ExpandableCirBuffer4Msg(91,3,MSGSIZE,cmm,409600);
    ecbMsg2 = new ExpandableCirBuffer4Msg(91,3,MSGSIZE,cmm,409600);
    ecbMsg3 = new ExpandableCirBuffer4Msg(91,3,MSGSIZE,cmm,409600);
    ecbMsg4 = new ExpandableCirBuffer4Msg(91,3,MSGSIZE,cmm,4096);
    ecbMsg5 = new ExpandableCirBuffer4Msg(91,3,MSGSIZE,cmm,409600);
    TestExpandableCirBuffer4Msg(ut);
    TestExpandableCirBuffer4Msg2(ut);
    TestExpandableCirBuffer4Msg3(ut);
    TestExpandableCirBuffer4Msg4(ut);
    TestExpandableCirBuffer4Msg5(ut);
    delete ecbMsg;
    delete ecbMsg2;
    delete ecbMsg3;
    delete ecbMsg4;
    delete ecbMsg5;

    ecbMsg  = new ExpandableCirBuffer4Msg(91,65536,MSGSIZE,cmm,409600);
    ecbMsg2 = new ExpandableCirBuffer4Msg(91,65536,MSGSIZE,cmm,409600);
    ecbMsg3 = new ExpandableCirBuffer4Msg(91,65536,MSGSIZE,cmm,409600);
    ecbMsg4 = new ExpandableCirBuffer4Msg(91,65536,MSGSIZE,cmm,4096);
    ecbMsg5 = new ExpandableCirBuffer4Msg(91,65536,MSGSIZE,cmm,409600);
    TestExpandableCirBuffer4Msg(ut);
    TestExpandableCirBuffer4Msg2(ut);
    TestExpandableCirBuffer4Msg3(ut);
    TestExpandableCirBuffer4Msg4(ut);
    TestExpandableCirBuffer4Msg5(ut);
    delete ecbMsg;
    delete ecbMsg2;
    delete ecbMsg3;
    delete ecbMsg4;
    delete ecbMsg5;

    ut.PrintResult();
  }


  // //--------------------------------------------------
  // // Others
  // //--------------------------------------------------
  // boost::shared_ptr<Logger>          pLogger   =  Logger::Instance();
  // boost::shared_ptr<SystemConfig>    pSysCfg   =  SystemConfig::Instance();
  // boost::shared_ptr<SharedObjects>   pShrObj   =  SharedObjects::Instance();
  //
  // string sConfigPath("Config.ini"); // default Config path
  // pSysCfg->ReadConfig(sConfigPath);
  //
  // //--------------------------------------------------
  // // Test canned data
  // //--------------------------------------------------
  //
  // CppUnit::TextUi::TestRunner utTestRunner;
  //
  // boost::shared_ptr<RTSClient> rtsClt = RTSClient::Instance();
  // boost::thread rtsCltThd(&RTSClient::Run, rtsClt.get());
  //
  // // utTestRunner.addTest(TestSharedObjects::suite());
  // // utTestRunner.addTest(TestTriggeringRTSClient::suite());
  // // utTestRunner.addTest(TestOrderBook::suite());
  // // utTestRunner.addTest(TestOrderBookOMDDSP::suite());
  // // utTestRunner.addTest(TestRTSResponse::suite());
  // // utTestRunner.addTest(TestTriggeringRTSClient::suite());
  // // utTestRunner.addTest(TestRealTime::suite());
  // // utTestRunner.addTest(TestSwitchRTSSvr::suite());
  // // utTestRunner.addTest(TestMsgHandling::suite());
  // // utTestRunner.addTest(TestRefresh::suite());
  // // utTestRunner.addTest(TestSeqReset::suite());
  //
  // //utTestRunner.addTest(TestSysConfig::suite());
  // utTestRunner.run();

  return 0;
}
