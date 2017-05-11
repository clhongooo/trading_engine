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
    virtual vector<uint32_t> Get_LastMsgSeqNumProcessed(const unsigned short, const BYTE *, const uint16_t) {}

    virtual void  HandleMDPRaw                     (const BYTE *, const unsigned short, const McastIdentifier::EMcastType, const uint16_t) {}
    virtual void  OnHeartBeat                      ()                                                  {}
    virtual void  OnRefreshBook                    (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnInstrumentDefinitionOption     (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnRefreshVolume                  (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnRefreshTrade                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnRefreshTradeSummary            (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnRefreshDailyStatistics         (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnRefreshSessionStatistics       (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnRefreshLimitsBanding           (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnRefreshSecurityDefinitionFuture(const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnRefreshSecurityDefinitionSpread(const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnQuoteRequest                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnSecurityStatus                 (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnChannelReset                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}
    virtual void  OnSnapshotFullRefresh            (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &) {}

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

    vector<uint32_t> Get_LastMsgSeqNumProcessed(const unsigned short, const BYTE *, const uint16_t);

    void HandleMDPRaw                     (const BYTE *, const unsigned short, const McastIdentifier::EMcastType, const uint16_t);
    void OnHeartBeat                      ();
    void OnRefreshBook                    (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnInstrumentDefinitionOption     (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnRefreshVolume                  (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnRefreshTrade                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnRefreshTradeSummary            (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnRefreshDailyStatistics         (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnRefreshSessionStatistics       (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnRefreshLimitsBanding           (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnRefreshSecurityDefinitionFuture(const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnRefreshSecurityDefinitionSpread(const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnQuoteRequest                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnSecurityStatus                 (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnChannelReset                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);
    void OnSnapshotFullRefresh            (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &);

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
