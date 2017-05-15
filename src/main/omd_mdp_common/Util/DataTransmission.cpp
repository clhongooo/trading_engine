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

void DataTransmission::NotifyTrade(const string & symbol, const double price, const double qty)
{
  m_Logger->Write(Logger::INFO,"%s: NotifyTrade: %s Price %f Qty %f",__FILENAME__, symbol.c_str(), price, qty);
}
void DataTransmission::NotifyOrderBookUpdate(const string & symbol, const ATU_MDI_marketfeed_struct &s)
{
  m_Logger->Write(Logger::INFO,"%s: NotifyOrderBookUpdate: %s %s",__FILENAME__, symbol.c_str(), ATU_MDI_marketfeed_struct::ToString(s).c_str());
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
