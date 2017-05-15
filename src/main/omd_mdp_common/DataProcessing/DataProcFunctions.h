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
#include "Constants.h"

#include <boost/shared_ptr.hpp>
#include <vector>
#include <sstream>
#include <boost/lexical_cast.hpp>

class DataProcFunctions {
  public:
    enum DPF_ProcFlag {DPF_PRINT_ONLY=0, DPF_DO_ACTUAL_PROCESSING};
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

    virtual void  PeekTemplateID                   (const BYTE *, const unsigned short, const string &, const uint16_t) {}
    virtual void  HandleMDPRaw                     (const BYTE *, const unsigned short, const string &, const uint16_t, const DPF_ProcFlag) {}
    virtual void  OnHeartBeat                      (const unsigned short, const string &) {}
    virtual void  OnRefreshBook                    (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnInstrumentDefinitionOption     (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnRefreshVolume                  (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnRefreshTrade                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnRefreshTradeSummary            (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnRefreshDailyStatistics         (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnRefreshSessionStatistics       (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnRefreshLimitsBanding           (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnRefreshSecurityDefinitionFuture(const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnRefreshSecurityDefinitionSpread(const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnQuoteRequest                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnSecurityStatus                 (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnChannelReset                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}
    virtual void  OnSnapshotFullRefresh            (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag) {}

    virtual const string CheckMsgTypeFromTemplateID(const unsigned short) {}

    //--------------------------------------------------
    const string ProcFlagToString(const DPF_ProcFlag pf) { return pf == DPF_PRINT_ONLY ? "DPF_PRINT_ONLY" : "DPF_DO_ACTUAL_PROCESSING"; }
    const string PrintCMEPriceNull(const int64_t pn) { return (pn == 9223372036854775807 ? "Null" : boost::lexical_cast<string>((double)pn/10000000.0)); }
    const bool ConvertCMEPriceNull(const int64_t pn, double & dRtn)
    {
      if (pn == 9223372036854775807) return false;
      dRtn = (double)pn/10000000.0;
      return true;
    }
    const string PrintCMEInt32Null(const int32_t pn) { return (pn == 2147483647? "Null" : boost::lexical_cast<string>(pn)); }
    const bool ConvertCMEInt32Null(const int32_t pn, int & iRtn)
    {
      if (pn == 2147483647) return false;
      iRtn = pn;
      return true;
    }
    const string PrintCMEInt8Null(const int32_t pn) { return (pn == 127? "Null" : boost::lexical_cast<string>(pn)); }
    const string PrintCMESettlementType(const SettlPriceType & spt)
    {
      if (spt.finalfinal())   return "finalfinal";
      if (spt.actual())       return "actual";
      if (spt.rounded())      return "rounded";
      if (spt.intraday())     return "intraday";
      if (spt.reservedBits()) return "reservedBits";
      if (spt.nullValue())    return "null";
    }

  protected:
    boost::shared_ptr<SystemConfig>      m_SysCfg;
    boost::shared_ptr<SharedObjects>     m_ShrObj;
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

    void PeekTemplateID                   (const BYTE *, const unsigned short, const string &, const uint16_t);
    void HandleMDPRaw                     (const BYTE *, const unsigned short, const string &, const uint16_t, const DPF_ProcFlag);
    void OnHeartBeat                      (const unsigned short, const string &);
    void OnRefreshBook                    (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnInstrumentDefinitionOption     (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnRefreshVolume                  (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnRefreshTrade                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnRefreshTradeSummary            (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnRefreshDailyStatistics         (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnRefreshSessionStatistics       (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnRefreshLimitsBanding           (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnRefreshSecurityDefinitionFuture(const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnRefreshSecurityDefinitionSpread(const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnQuoteRequest                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnSecurityStatus                 (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnChannelReset                   (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);
    void OnSnapshotFullRefresh            (const unsigned short, const mktdata::MessageHeader &, char *, const int, const string &, const DPF_ProcFlag);

    const string CheckMsgTypeFromTemplateID(const unsigned short);

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
