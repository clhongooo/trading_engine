/*
 * MainThread.cpp
 *
 *  Created on: May 15, 2014
 *      Author: sunny
 */

#include "OMD_MainThread.h"

MainThread::MainThread() {
}

MainThread::~MainThread() {
}

void MainThread::RunMainThread()
{
  //--------------------------------------------------
  // Init the singleton instances first
  //--------------------------------------------------
  boost::shared_ptr<Logger>              pLogger     =  Logger::Instance();
  boost::shared_ptr<SystemConfig>        pSysCfg     =  SystemConfig::Instance();
  boost::shared_ptr<SharedObjects>       pShrObj     =  SharedObjects::Instance();
  boost::shared_ptr<RTSClient>           pRTSClt     =  RTSClient::Instance();
  boost::shared_ptr<ThreadHealthMonitor> pThrdHth    =  ThreadHealthMonitor::Instance();
  //--------------------------------------------------

  boost::thread_group BoostThreadGrp;

  //--------------------------------------------------
  // Start threads 
  //--------------------------------------------------
  ChannelController ChnlCtrlr;
  BoostThreadGrp.add_thread(new boost::thread(&ChannelController::StartRecvMcast, &ChnlCtrlr));
  BoostThreadGrp.add_thread(new boost::thread(&ChannelController::StartDataCompletenessInspectors, &ChnlCtrlr));
  BoostThreadGrp.add_thread(new boost::thread(&RTSClient::Run, pRTSClt.get()));
  BoostThreadGrp.add_thread(new boost::thread(&ThreadHealthMonitor::Run, pThrdHth.get()));

  DataProcController DataProcCtrlr;
  BoostThreadGrp.add_thread(new boost::thread(&DataProcController::StartDataProcressing, &DataProcCtrlr));

  TerminalThread tthd(&ChnlCtrlr);
  BoostThreadGrp.add_thread(new boost::thread(&TerminalThread::Run, &tthd));

  //--------------------------------------------------
  pLogger->Write(Logger::NOTICE,"All threads in the Main function have just been triggered.");

  BoostThreadGrp.join_all();

  //--------------------------------------------------
  // System is shutting down
  //--------------------------------------------------
  pLogger->Write(Logger::NOTICE,"All threads in the Main function have stopped.");

  pLogger->Write(Logger::NOTICE,"Now wait 3 seconds.");
  boost::this_thread::sleep(boost::posix_time::seconds(3));

  return;
}
