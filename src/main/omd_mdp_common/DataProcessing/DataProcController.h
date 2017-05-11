/*
 * DataProcController.h
 *
 *  Created on: Apr 11, 2014
 *      Author: sunny
 */

#ifndef DATAPROCCONTROLLER_H_
#define DATAPROCCONTROLLER_H_

#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/thread.hpp>
#include "PreProcessor.h"
#include "RealTimeProcessor.h"
#include "RefreshProcessor.h"
#include "InstrumentReplayProcessor.h"
#include "Logger.h"

using namespace std;
using namespace boost;

class DataProcController {
  public:
    DataProcController();
    virtual ~DataProcController();
    void StartDataProcessing();

  private:
    boost::thread_group                m_BoostThreadGrp;

    //--------------------------------------------------
    // Various Processores
    //--------------------------------------------------
    ptr_vector<PreProcessor>              m_PreProcessors;
    ptr_vector<RealTimeProcessor>         m_RealTimeProcessors;
    ptr_vector<RefreshProcessor>          m_RefreshProcessors;
    ptr_vector<InstrumentReplayProcessor> m_InstrumentReplayProcessors;

    //------------------------------
    // System objects
    //------------------------------
    boost::shared_ptr<SystemConfig>  m_SysCfg;
    boost::shared_ptr<Logger>        m_Logger;
};

#endif /* DATAPROCCONTROLLER_H_ */
