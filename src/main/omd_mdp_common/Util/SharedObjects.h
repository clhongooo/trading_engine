//**************************************************
//  Author:      Sunny Yan
//  Created On:  Thu Apr 10 21:11:13 HKT 2014
//  Description:
//
//**************************************************

#ifndef SHAREDOBJ_H_
#define SHAREDOBJ_H_

#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <cstring>
#include <deque>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "McastIdentifier.h"
#include "BinaryTools.h"
#include "OMD_Message_Headers.h"
#include "OrderBook.h"
#include "OrderBookCache.h"
#include "SystemConfig.h"
#include "Logger.h"
#include "ExpandableCirBuffer.h"
#include "ExpandableCirBuffer4Msg.h"
#include "CentralMemMgr.h"
#include "ATU_Abstract_MDI.h"

using namespace std;
using namespace boost;

class SystemConfig;

typedef boost::function < bool(long, long, double, double) >          OMD_Trade_CallBackFunc;
typedef boost::function < bool(long, ATU_MDI_marketfeed_struct &) >   OMD_OrderBook_CallBackFunc;

class SharedObjects
{
  public:
    static boost::shared_ptr<SharedObjects> Instance();
    void ResetObjects();
    void ResetObjectsPurgeRawCirBuf();
    void DestroyObjects();
    ~SharedObjects();
    void InitializeSharedObjects(bool);
    ExpandableCirBuffer      *GetRawPktCirBufPtr(const McastIdentifier &);
    ExpandableCirBuffer4Msg  *GetMsgCirBufPtr(McastIdentifier::EMcastType,const unsigned short);

    //--------------------------------------------------
    // Order Book
    //--------------------------------------------------
    boost::shared_ptr<OrderBookCache> GetOrderBookCache();
    void AddOrderBookIDInChnl(unsigned short, unsigned long);
    set<unsigned long> * GetOrderBookIDInChnl(unsigned short);
    void ResetOrderBooksInChnl(unsigned short);

    // //--------------------------------------------------
    // // Spread Table
    // //--------------------------------------------------
    // unsigned short GetSpreadTable(unsigned long);
    // void           AddSpreadTable(unsigned long,unsigned short);

    //------------------------------
    // RTS limit of max num of connections per day
    //------------------------------
    void           AddCountNumOfRTSConn();
    void           ResetCountNumOfRTSConn();
    unsigned int   GetCountNumOfRTSConn();

    //--------------------------------------------------
    // Refresh Activation
    //--------------------------------------------------
    void             ActivateRefresh(unsigned short);
    void             DeactivateRefresh(unsigned short);
    bool             CheckRefreshActivatnStatus(unsigned short);
    unsigned short   CheckRefreshActivatnStatusCount(unsigned short);

    //--------------------------------------------------
    // OMD Readiness Test: Session 4: Capacity test
    //--------------------------------------------------
    void             TurnOnCapTest();
    void             TurnOffCapTest();
    bool             CheckCapTestStatus();

    //--------------------------------------------------
    // To handle OMD_SEQUENCE_RESET
    //--------------------------------------------------
    uint32_t         CheckRTChnlLatestSeqNoOffset   (const unsigned short);
    void             AddRTChnlSeqNoOffset           (const unsigned short,const uint32_t);
    void             ResetRTChnlSeqNoOffset         ();
    uint32_t         CheckRFChnlLatestSeqNoOffset   (const unsigned short);
    void             AddRFChnlSeqNoOffset           (const unsigned short,const uint32_t);
    void             ResetRFChnlSeqNoOffset         ();
    bool             CheckRTSeqResetAlreadyHandled  (const unsigned short, const long);
    bool             CheckRFSeqResetAlreadyHandled  (const unsigned short, const long);
    void             AddRTSeqResetAlreadyHandled    (const unsigned short, const long);
    void             AddRFSeqResetAlreadyHandled    (const unsigned short, const long);
    void             ResetRTSeqResetAlreadyHandled  ();
    void             ResetRFSeqResetAlreadyHandled  ();

    //--------------------------------------------------
    // Global flag of the system state
    //--------------------------------------------------
    enum ESysState { UP, DOWN };
    ESysState m_SysState;
    //boost::shared_mutex m_SysStateMutex; // for less overhead
    bool ThreadShouldExit();
    void SetSystemState(ESysState);

    //--------------------------------------------------
    // Instrument Mapping
    //--------------------------------------------------
    string GetSymbolFromInstrumentID(const unsigned long);
    void AddInstrumentIDToSymbolMapping(const unsigned long, const string &);

    //--------------------------------------------------
    void SetProgramStartTime();
    boost::posix_time::ptime GetProgramStartTime() const;
    //--------------------------------------------------

  private:
    SharedObjects();
    static boost::weak_ptr<SharedObjects>         m_pInstance;

    //------------------------------
    // System config
    //------------------------------
    boost::shared_ptr<SystemConfig>               m_SysCfg;
    boost::shared_ptr<Logger>                     m_Logger;
    boost::shared_ptr<CentralMemMgr>              m_MemMgr;
    //------------------------------
    // Shared objects
    //------------------------------

    //------------------------------
    vector<boost::shared_ptr<ExpandableCirBuffer> >      m_RawPktCirBuf;
    vector<boost::shared_ptr<ExpandableCirBuffer4Msg> >  m_MsgCirBuf;

    //------------------------------
    // RTS limit of max num of connections per day
    //------------------------------
    boost::shared_mutex                    m_RTSNumOfConnMutex;
    unsigned int                           m_NumOfRTSConn;

    //--------------------------------------------------
    // Refresh Activation
    // The nature is just bool, == 0 or > 0, but to facilitate counting the number of times refresh is activated during unit testing, used unsigned short rather than bool
    //--------------------------------------------------
    vector<unsigned short>                 m_bRefreshActivated;
    vector<boost::shared_mutex*>           m_RefreshActnMutex;

    //--------------------------------------------------
    // OMD Readiness Test: Session 4: Capacity test
    //--------------------------------------------------
    bool                                   m_bCapTest;
    //boost::shared_mutex                  m_CapTestMutex; // no need

    //--------------------------------------------------
    // To handle OMD_SEQUENCE_RESET
    //--------------------------------------------------
    vector<vector<uint32_t>* >             m_RTSeqResetOffset;
    vector<boost::shared_mutex*>           m_RTSeqResetOffsetMutex;
    vector<vector<uint32_t>* >             m_RFSeqResetOffset;
    vector<boost::shared_mutex*>           m_RFSeqResetOffsetMutex;
    vector<vector<long> *>                 m_RTSeqResetAlreadyHandled;
    vector<boost::shared_mutex*>           m_RTSeqResetAlreadyHandledMutex;
    vector<vector<long> *>                 m_RFSeqResetAlreadyHandled;
    vector<boost::shared_mutex*>           m_RFSeqResetAlreadyHandledMutex;

    //--------------------------------------------------
    // Order Books
    //--------------------------------------------------
    boost::shared_ptr<OrderBookCache>      m_OrderBookCache;
    vector<set<unsigned long> *>           m_OrderBookIDInEachChnl;
    vector<boost::shared_mutex*>           m_OrderBookIDInEachChnlMutex;

    // //--------------------------------------------------
    // // Spread table
    // //--------------------------------------------------
    // map<unsigned long,unsigned short>      m_SpreadTable; // OrderBookID
    // boost::shared_mutex                    m_SpreadTableMutex;

    //--------------------------------------------------
    // Instrument Mapping
    // OMD-D : OrderBookID - Symbol
    // CME   : InstrumentID - Symbol
    //--------------------------------------------------
    map<unsigned long, string>             m_InstrumentMap;
    boost::shared_mutex                    m_InstrumentMapMutex;

    //--------------------------------------------------
    // Program start time
    //--------------------------------------------------
    boost::posix_time::ptime               m_ProgramStartTime;
};

#endif
