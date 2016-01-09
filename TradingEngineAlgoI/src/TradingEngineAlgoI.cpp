#include "TradingEngineAlgoI.h"

TradingEngineAlgoI::TradingEngineAlgoI() : ATU_Abstract_AlgoI()
{
}

TradingEngineAlgoI::~TradingEngineAlgoI()
{
}
bool TradingEngineAlgoI::on_marketfeed_process(ATU_MDI_marketfeed_struct& s)
{
  // m_MainThdCls->p_dahkfe->OnRecvMsgMDIStruct(s);
  m_MainThdCls->p_daib->OnRecvMsgMDIStruct(s);

  //--------------------------------------------------
  m_PortAndOrders = PortfoliosAndOrders::Instance();

  vector<ATU_OTI_signalfeed_struct> v;
  m_PortAndOrders->ConstructSignalFeedsAndChgOrderState(v);
  for (vector<ATU_OTI_signalfeed_struct>::iterator it = v.begin(); it != v.end(); ++it)
  {
    process_signalfeed(*it);
    // m_Logger->Write(Logger::INFO, "OrderExecutorHKFE: %s : %s : %s Sent: %s", m_OTI_Server_IP.c_str(), m_OTI_Server_Port.c_str(), m_MarketData->GetSystemTime().ToStr().c_str(), v[i].c_str());
  }

  //--------------------------------------------------
  YYYYMMDD yyyymmdd;
  HHMMSS hhmmss;
  SDateTime::FromCashTSToYMDHMS(s.m_timestamp,yyyymmdd,hhmmss);
  m_PortAndOrders->CheckAndReplaceOrders(hhmmss);
  //--------------------------------------------------

}
bool TradingEngineAlgoI::on_tradefeed_process(ATU_OTI_tradefeed_struct &s)
{
  cout << s.m_timestamp        << ",";
  cout << "tradefeed"          << ",";
  cout << s.m_market           << ",";
  cout << s.m_feedcode         << ",";
  cout << s.m_order_id         << ",";
  cout << s.m_price            << ",";
  cout << s.m_qty              << ",";
  cout << s.m_buy_or_sell      << ",";
  cout << s.m_trade_id         << ",";
  cout << s.m_source           << endl;

  m_MainThdCls->p_oe->HandleTradeFeedRecv(s);
}
bool TradingEngineAlgoI::on_riskfeed_process(ATU_OTI_riskfeed_struct &s)
{

}
bool TradingEngineAlgoI::on_orderfeed_process(ATU_OTI_orderfeed_struct &s)
{
  m_MainThdCls->p_oe->HandleOrderFeedRecv(s);
}
bool TradingEngineAlgoI::on_portfoliofeed_process(ATU_OTI_portfoliofeed_struct &s)
{

}
bool TradingEngineAlgoI::on_accumperffeed_process(ATU_OTI_accumperffeed_struct &s)
{

}
bool TradingEngineAlgoI::on_pnlperffeed_process(ATU_OTI_pnlperffeed_struct &pnlfeed)
{
}
bool TradingEngineAlgoI::on_errorfeedp_process(ATU_ErrorMsgStruct *s)
{
  on_errorfeed(*s);
}
bool TradingEngineAlgoI::on_errorfeed_process(ATU_ErrorMsgStruct &s)
{
}
bool TradingEngineAlgoI::on_dailyperffeed_process(ATU_OTI_dailyperffeed_struct &s)
{
}
void TradingEngineAlgoI::detach() {

  m_MainThdCls.reset(new TradingEngineMainThread(m_connectString));
  m_MainThd.reset(new boost::thread(&TradingEngineMainThread::RunMainThread, m_MainThdCls.get()));

  while (!m_MainThdCls->HasFinishedInit())
  {
    usleep(1000);
  }

  vector<ATU_MDI_subscription_struct> vSub;

  // vector<string> vsubsym = m_MainThdCls->p_SysCfg->GetMDISubscriptionSymbols(OTIMDI_HKFE);
  // string sstart          = m_MainThdCls->p_SysCfg->GetMDISubscriptionStartDate(OTIMDI_HKFE);
  // string send            = m_MainThdCls->p_SysCfg->GetMDISubscriptionEndDate(OTIMDI_HKFE);
  // string sreplayspd      = m_MainThdCls->p_SysCfg->GetMDISubscriptionReplaySpeed(OTIMDI_HKFE);

  vector<string> vsubsym = m_MainThdCls->p_SysCfg->GetMDISubscriptionSymbols(OTIMDI_IB);
  string sstart          = m_MainThdCls->p_SysCfg->GetMDISubscriptionStartDate(OTIMDI_IB);
  string send            = m_MainThdCls->p_SysCfg->GetMDISubscriptionEndDate(OTIMDI_IB);
  string sreplayspd      = m_MainThdCls->p_SysCfg->GetMDISubscriptionReplaySpeed(OTIMDI_IB);

  // m_MainThdCls->p_dahkfe->BatchMktFeedSubscription(vsubsym, sstart, send, sreplayspd, vSub);
  m_MainThdCls->p_daib->BatchMktFeedSubscription(vsubsym, sstart, send, sreplayspd, vSub);

  for (vector<ATU_MDI_subscription_struct>::iterator it = vSub.begin(); it != vSub.end(); ++it)
  {
    process_subscription(*it);
  }

  cout << "start process msg queue" << endl;
  shared_ptr<boost::thread> init_and_run_thread;
  init_and_run_thread.reset(new boost::thread(boost::bind(&ATU_Abstract_AlgoI::process_msgqueue,this)));
}

