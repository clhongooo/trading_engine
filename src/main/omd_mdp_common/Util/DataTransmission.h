//**************************************************
//  Author:      Sunny Yan
//  Description: For the transmission of data to strategy engine
//**************************************************
#ifndef DATATRANSMISSION_HPP_
#define DATATRANSMISSION_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "ATU_Abstract_MDI.h"
#include "Logger.h"
#include "BinaryTools.h"
#include "SharedObjects.h"
#include <vector>
#include <string>
#include <map>

using namespace std;
using namespace boost;

class DataTransmission : boost::noncopyable
{
  public:
    static boost::shared_ptr<DataTransmission> Instance();
    ~DataTransmission(){};

    void SubscribeInstrument(const string &);
    bool CheckIfSubscribed(unsigned long);
    void NotifyTrade(const string &, const double, const double);
    void NotifyOrderBookUpdate(const string &, const ATU_MDI_marketfeed_struct &);

  private:
    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    DataTransmission();
    DataTransmission(DataTransmission const&);
    DataTransmission& operator=(DataTransmission const&){};
    static boost::weak_ptr<DataTransmission> m_pInstance;

    //--------------------------------------------------
    boost::shared_ptr<Logger>        m_Logger;
    boost::shared_ptr<SharedObjects> m_ShrObj;

    set<unsigned long>           m_omd_mdi_subscribed_instrument;
    boost::shared_mutex          m_omd_mdi_subscription_Mutex;
    //--------------------------------------------------

};

#endif
