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
#include "SFunctional.h"
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
    virtual void ProcessMessageForTransmission(const BYTE*,unsigned short) {}

    //--------------------------------------------------
    // MDP
    //--------------------------------------------------
    virtual vector<uint32_t> Get_LastMsgSeqNumProcessed(const BYTE *) {}

    virtual void  HandleMDPRaw                     (const BYTE *, const unsigned short, const McastIdentifier::EMcastType) {}
    virtual void  OnHeartBeat                      ()                                                  {}
    virtual void  OnRefreshBook                    (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnInstrumentDefinitionOption     (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnRefreshVolume                  (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnRefreshTrade                   (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnRefreshTradeSummary            (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnRefreshDailyStatistics         (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnRefreshSessionStatistics       (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnRefreshLimitsBanding           (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnRefreshSecurityDefinitionFuture(const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnRefreshSecurityDefinitionSpread(const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnQuoteRequest                   (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnSecurityStatus                 (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnChannelReset                   (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}
    virtual void  OnSnapshotFullRefresh            (const unsigned short, const mktdata::MessageHeader &, char *, const int) {}

  protected:
    boost::shared_ptr<SystemConfig>      m_SysCfg;
    boost::shared_ptr<DataTransmission>  m_DataTrans;
    boost::shared_ptr<Logger>            m_Logger;
};

class DataProcFunctions_OMDC : public DataProcFunctions {
  public:
    DataProcFunctions_OMDC() : DataProcFunctions() {}
    virtual ~DataProcFunctions_OMDC() {}
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, char[]);
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long);
    void ProcessOrderBookInstruction(const char *,boost::shared_ptr<Logger>,const BYTE*,boost::shared_ptr<SharedObjects>,bool);
    void ProcessMessageForTransmission(const BYTE*,unsigned short);
};

class DataProcFunctions_OMDD : public DataProcFunctions {
  public:
    DataProcFunctions_OMDD() : DataProcFunctions() {}
    virtual ~DataProcFunctions_OMDD() {}
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, char[]);
    void OutputJsonToLog(const char *, const unsigned short, boost::shared_ptr<Logger>, const BYTE*, unsigned short, char[], unsigned long);
    void ProcessOrderBookInstruction(const char *,boost::shared_ptr<Logger>,const BYTE*,boost::shared_ptr<SharedObjects>,bool);
    void ProcessMessageForTransmission(const BYTE*,unsigned short);
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

    vector<uint32_t> Get_LastMsgSeqNumProcessed(const BYTE *);

    void HandleMDPRaw                     (const BYTE *, const unsigned short, const McastIdentifier::EMcastType);
    void OnHeartBeat                      ();
    void OnRefreshBook                    (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnInstrumentDefinitionOption     (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnRefreshVolume                  (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnRefreshTrade                   (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnRefreshTradeSummary            (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnRefreshDailyStatistics         (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnRefreshSessionStatistics       (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnRefreshLimitsBanding           (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnRefreshSecurityDefinitionFuture(const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnRefreshSecurityDefinitionSpread(const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnQuoteRequest                   (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnSecurityStatus                 (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnChannelReset                   (const unsigned short, const mktdata::MessageHeader &, char *, const int);
    void OnSnapshotFullRefresh            (const unsigned short, const mktdata::MessageHeader &, char *, const int);

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
