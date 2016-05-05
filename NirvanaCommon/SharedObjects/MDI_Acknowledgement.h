#ifndef MDI_ACKNOWLEDGEMENT_H_
#define MDI_ACKNOWLEDGEMENT_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/thread.hpp>
#include <deque>
#include <vector>
#include "SystemConfig.h"
#define MDI_ACK_DEBUG_MSG false

using namespace std;
using namespace boost;

class MDI_Acknowledgement {
  public:
    static boost::shared_ptr<MDI_Acknowledgement> Instance();
    virtual ~MDI_Acknowledgement();

    void ReportAckTechIndUpd();
    void ReportAckMTM();
    void ReportAck(const StrategyID);
    void ReportNotMktData();
    void ReportDataArrived(const int);

    //--------------------------------------------------
    // Observer
    //--------------------------------------------------
    void WaitForAck();

  private:
    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    MDI_Acknowledgement();
    MDI_Acknowledgement(MDI_Acknowledgement const&);
    MDI_Acknowledgement& operator=(MDI_Acknowledgement const&){};
    static boost::weak_ptr<MDI_Acknowledgement> m_pInstance;

    bool CheckIfAllAckRecvdLock();
    bool CheckIfAllAckRecvdNoLock();
    void ResetRecvAckLock();
    void ResetRecvAckNoLock();
    void ClearAckReportedFromStyNoLock();
    void ClearAckReportedFromStyLock();

    //--------------------------------------------------
    // System Objects
    //--------------------------------------------------
    boost::shared_ptr<Logger>         m_Logger;
    boost::shared_ptr<SystemConfig>   m_SysCfg;

    vector<int>                m_NoOfAckReceived_sty;
    int                        m_NoOfAckReceived_techind;
    int                        m_NoOfAckReceived_mtm;
    set<int>                   m_MDIThatNeedAck;
    boost::shared_mutex        m_ReceivedAck_mutex;

    //--------------------------------------------------
    // Observer
    //--------------------------------------------------
    // boost::mutex m_ObserverMutex_Ack;
    // boost::condition_variable m_cvAck;
    // void NotifyConsumers();
    //--------------------------------------------------

};

#endif
