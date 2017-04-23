#include "DataTransmission.h"

boost::weak_ptr<DataTransmission> DataTransmission::m_pInstance;

boost::shared_ptr<DataTransmission> DataTransmission::Instance() {
  boost::shared_ptr<DataTransmission> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new DataTransmission());
    m_pInstance = instance;
  }
  return instance;
}

DataTransmission::DataTransmission()
{
  m_Logger = Logger::Instance();
  m_ShrObj = SharedObjects::Instance();
}

bool DataTransmission::NotifyTrade(long tradeid, long code, double price, double qty)
{
  if (m_omd_trade_call_back_func!=NULL) {
    (*m_omd_trade_call_back_func)(tradeid,code,price,qty);
  }
  return true;
}
bool DataTransmission::NotifyOrderBookUpdate(long tradeid, ATU_MDI_marketfeed_struct &s)
{
  if (m_omd_orderbook_call_back_func!=NULL) {
    (*m_omd_orderbook_call_back_func)(tradeid,s);
  }
  return true;
}
void DataTransmission::SubscribeInstrument(const string &sFeedCode)
{
  try
  {
    unsigned long ulCode = boost::lexical_cast<unsigned long>(sFeedCode);

    boost::unique_lock<boost::shared_mutex> lock(m_omd_mdi_subscription_Mutex);
    set<unsigned long>::iterator it = m_omd_mdi_subscribed_instrument.find(ulCode);
    if (it == m_omd_mdi_subscribed_instrument.end())
    {
      m_omd_mdi_subscribed_instrument.insert(ulCode);
      m_Logger->Write(Logger::INFO, "OMD_MDI: New marketfeed subscription received. %s", sFeedCode.c_str());
    }
  }
  catch (const boost::bad_lexical_cast & e)
  {
    m_Logger->Write(Logger::ERROR, "%s:: lexical_cast: instrument = |%s|. %s", __FUNCTION__, sFeedCode.c_str(), e.what());
  }
  return;
}
bool DataTransmission::CheckIfSubscribed(unsigned long ulCode)
{
  boost::shared_lock<boost::shared_mutex> lock(m_omd_mdi_subscription_Mutex);
  return (m_omd_mdi_subscribed_instrument.find(ulCode) != m_omd_mdi_subscribed_instrument.end());
}
