//**************************************************
//  Author:      Sunny Yan
//  Created On:  Wed Apr 30 09:57:45 HKT 2014
//  Description: Centralize a place for common data processing functions
//
//**************************************************

#ifndef DATAPROCFUNC_H_
#define DATAPROCFUNC_H_

#include "OrderBook.h"
#include "BinaryTools.h"
#include "OMD_Message_Headers.h"
#include "MDP_Message_Headers.h"
#include "Logger.h"
#include "SystemConfig.h"
#include "DataTransmission.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <sstream>
#include <boost/lexical_cast.hpp>

class DataProcFunctions {
  public:
    DataProcFunctions();
    virtual ~DataProcFunctions() {}
    virtual void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, char[]) = 0;
    virtual void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long) = 0;
    virtual void ProcessOrderBookInstruction(const char *,boost::shared_ptr<Logger>,const BYTE*,boost::shared_ptr<SharedObjects>,bool) = 0;
    virtual void ProcessMessageForTransmission(BYTE*,unsigned short);

  protected:
    boost::shared_ptr<SystemConfig>      m_SysCfg;
    boost::shared_ptr<DataTransmission>  m_DataTrans;
};

class DataProcFunctions_OMDC : public DataProcFunctions {
  public:
    DataProcFunctions_OMDC() : DataProcFunctions() {}
    virtual ~DataProcFunctions_OMDC() {}
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, char[]);
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long);
    void ProcessOrderBookInstruction(const char *,boost::shared_ptr<Logger>,const BYTE*,boost::shared_ptr<SharedObjects>,bool);
};

class DataProcFunctions_OMDD : public DataProcFunctions {
  public:
    DataProcFunctions_OMDD() : DataProcFunctions() {}
    virtual ~DataProcFunctions_OMDD() {}
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, char[]);
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long);
    void ProcessOrderBookInstruction(const char *,boost::shared_ptr<Logger>,const BYTE*,boost::shared_ptr<SharedObjects>,bool);
};

class DataProcFuncFactory {
  public:
    DataProcFuncFactory();
    virtual ~DataProcFuncFactory();
    static DataProcFunctions* GetDataProcFunc(SystemConfig::Identity);
};


//--------------------------------------------------
// FIXME CME stuff
//--------------------------------------------------
void OnHeartBeat                      ();
void OnRefreshBook                    (mktdata::MessageHeader & hdr, char *buf, int len);
void OnInstrumentDefinitionOption     (mktdata::MessageHeader & hdr, char *buf, int len);
void OnRefreshVolume                  (mktdata::MessageHeader & hdr, char *buf, int len);
void OnRefreshTrade                   (mktdata::MessageHeader & hdr, char *buf, int len);
void OnRefreshTradeSummary            (mktdata::MessageHeader & hdr, char *buf, int len);
void OnRefreshDailyStatistics         (mktdata::MessageHeader & hdr, char *buf, int len);
void OnRefreshSessionStatistics       (mktdata::MessageHeader & hdr, char *buf, int len);
void OnRefreshLimitsBanding           (mktdata::MessageHeader & hdr, char *buf, int len);
void OnRefreshSecurityDefinitionFuture(mktdata::MessageHeader & hdr, char *buf, int len);
void OnRefreshSecurityDefinitionSpread(mktdata::MessageHeader & hdr, char *buf, int len);
void OnQuoteRequest                   (mktdata::MessageHeader & hdr, char *buf, int len);
void OnSecurityStatus                 (mktdata::MessageHeader & hdr, char *buf, int len);
void OnChannelReset                   (mktdata::MessageHeader & hdr, char *buf, int len);
void OnSnapshotFullRefresh            (mktdata::MessageHeader & hdr, char *buf, int len);

struct level {
  int64_t price;
  int size;
  int norder;
};

struct OrderBookKenny {
  uint64_t transact_time;
  int seqno;
  int secid;
  struct level bid[10], ask[10];
  char instid[16];
};

std::string TransferTimestamp(uint64_t micro_sec);

#endif /* DATAPROCFUNC_H_ */
