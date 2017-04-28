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
#include "SDateTime.h"
#include "DataTransmission.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <sstream>
#include <boost/lexical_cast.hpp>

class DataProcFunctions {
  public:
    DataProcFunctions();
    virtual ~DataProcFunctions() {}

    //--------------------------------------------------
    // OMD
    //--------------------------------------------------
    virtual void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, char[]) {}
    virtual void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long) {}
    virtual void ProcessOrderBookInstruction(const char *,boost::shared_ptr<Logger>,const BYTE*,boost::shared_ptr<SharedObjects>,bool) {}
    virtual void ProcessMessageForTransmission(BYTE*,unsigned short) {}

    //--------------------------------------------------
    // MDP
    //--------------------------------------------------
    virtual void OnHeartBeat                      ()                                                  {}
    virtual void OnRefreshBook                    (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnInstrumentDefinitionOption     (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnRefreshVolume                  (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnRefreshTrade                   (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnRefreshTradeSummary            (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnRefreshDailyStatistics         (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnRefreshSessionStatistics       (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnRefreshLimitsBanding           (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnRefreshSecurityDefinitionFuture(const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnRefreshSecurityDefinitionSpread(const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnQuoteRequest                   (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnSecurityStatus                 (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnChannelReset                   (const mktdata::MessageHeader &, char *, const int) {}
    virtual void OnSnapshotFullRefresh            (const mktdata::MessageHeader &, char *, const int) {}

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
    void ProcessMessageForTransmission(BYTE*,unsigned short);
};

class DataProcFunctions_OMDD : public DataProcFunctions {
  public:
    DataProcFunctions_OMDD() : DataProcFunctions() {}
    virtual ~DataProcFunctions_OMDD() {}
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, char[]);
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long);
    void ProcessOrderBookInstruction(const char *,boost::shared_ptr<Logger>,const BYTE*,boost::shared_ptr<SharedObjects>,bool);
    void ProcessMessageForTransmission(BYTE*,unsigned short);
};

//--------------------------------------------------
// FIXME CME stuff copied from Kenny
//--------------------------------------------------
class DataProcFunctions_MDP : public DataProcFunctions {
  public:
    typedef struct {
      int64_t price;
      int size;
      int norder;
    } level;

    typedef struct {
      uint64_t transact_time;
      int seqno;
      int secid;
      level bid[10], ask[10];
      char instid[16];
    } OrderBookKenny;

    void OnHeartBeat                      ();
    void OnRefreshBook                    (const mktdata::MessageHeader &, char *, const int);
    void OnInstrumentDefinitionOption     (const mktdata::MessageHeader &, char *, const int);
    void OnRefreshVolume                  (const mktdata::MessageHeader &, char *, const int);
    void OnRefreshTrade                   (const mktdata::MessageHeader &, char *, const int);
    void OnRefreshTradeSummary            (const mktdata::MessageHeader &, char *, const int);
    void OnRefreshDailyStatistics         (const mktdata::MessageHeader &, char *, const int);
    void OnRefreshSessionStatistics       (const mktdata::MessageHeader &, char *, const int);
    void OnRefreshLimitsBanding           (const mktdata::MessageHeader &, char *, const int);
    void OnRefreshSecurityDefinitionFuture(const mktdata::MessageHeader &, char *, const int);
    void OnRefreshSecurityDefinitionSpread(const mktdata::MessageHeader &, char *, const int);
    void OnQuoteRequest                   (const mktdata::MessageHeader &, char *, const int);
    void OnSecurityStatus                 (const mktdata::MessageHeader &, char *, const int);
    void OnChannelReset                   (const mktdata::MessageHeader &, char *, const int);
    void OnSnapshotFullRefresh            (const mktdata::MessageHeader &, char *, const int);

  private:

    map<int,string> instmap;
    map<string,OrderBookKenny> obmap;
};

//--------------------------------------------------
// Factory
//--------------------------------------------------
class DataProcFuncFactory {
  public:
    DataProcFuncFactory();
    virtual ~DataProcFuncFactory();
    static DataProcFunctions* GetDataProcFunc(dma::Identity);
};

#endif
