/*
 * MainThread.h
 *
 *  Created on: May 15, 2014
 *      Author: sunny
 */

#ifndef MAINTHREAD_H_
#define MAINTHREAD_H_

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/ini_parser.hpp"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "Logger/Logger.h"
#include "ChannelAggregation/ChannelController.h"
#include "ChannelAggregation/DataCompletenessInspector.h"
#include "DataProcessing/DataProcController.h"
#include "SystemConfig.h"
#include "Terminal.h"
#include "ChannelAggregation/RTSClient.h"
#include "ThreadHealthMonitor.h"

class MainThread {
  public:
    MainThread();
    virtual ~MainThread();
    void RunMainThread();

};

#endif /* MAINTHREAD_H_ */
