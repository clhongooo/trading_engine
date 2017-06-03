#include "ATU_IB_MDI.h"

ATU_IB_MDI::ATU_IB_MDI(boost::shared_ptr<ExpandableCirBuffer> p, const string &p_host, unsigned int p_port, int p_clientId) :
  m_host(p_host)
  ,m_port(p_port)
  ,m_clientId(p_clientId)
  ,m_account(IBString(""))
  ,m_isReady(false)
  ,m_clientSocket(this)
  ,m_nextTickerId(0)
  ,m_ecbMD(p)
{
  m_Logger = StdStreamLogger::Instance();
  m_subscribeList.clear();
}

ATU_IB_MDI::ATU_IB_MDI(boost::shared_ptr<ExpandableCirBuffer> p) :
  m_port(-1)
  ,m_clientId(-1)
  ,m_isReady(false)
  ,m_clientSocket(this)
  ,m_nextTickerId(0)
  ,m_ecbMD(p)
{
  m_Logger = StdStreamLogger::Instance();
  m_subscribeList.clear();
}

ATU_IB_MDI::~ATU_IB_MDI()
{
  if (m_contractExtractor != NULL) {
    delete m_contractExtractor;
  }
}

//--------------------------------------------------
// setters
//--------------------------------------------------
void ATU_IB_MDI::setIP(const string & s)
{
  m_host = s;
}
void ATU_IB_MDI::setPort(const unsigned int p)
{
  m_port = p;
}
void ATU_IB_MDI::setClientID(const int id)
{
  m_clientId = id;
}
void ATU_IB_MDI::setAccount(const string & s)
{
  m_account = IBString(s);
}
//--------------------------------------------------

void ATU_IB_MDI::readContracts(string p_filepath) {
  m_contractExtractor = new ContractManager();
  cout << "Reading contracts..." << endl;
  IniContractReader* iniReader = new IniContractReader(p_filepath);
  m_contractExtractor->extract(iniReader);
  delete iniReader;
}

bool ATU_IB_MDI::isReady() const
{
  return m_isReady;
}

void ATU_IB_MDI::run()
{
  if (m_host.size() == 0 || m_port == -1 || m_clientId == -1) {
    m_Logger->Write(StdStreamLogger::INFO,"%s: --->>> IP, port, and client ID are all necessary", __FILENAME__);
    return;
  }

  if (m_isReady)
    return;

  // All connection stuff asigned to socket thread
  // Connect to server
  m_Logger->Write(StdStreamLogger::INFO,"%s: --->>> Initiating MDI connection to TWS...", __FILENAME__);

  bool bConn = m_clientSocket.eConnect(m_host.c_str(), m_port, m_clientId);
  if (bConn) {
    m_Logger->Write(StdStreamLogger::INFO,"%s: --->>> MDI connected to TWS!", __FILENAME__);
    m_isReady = true;
    m_clientSocket.startHandlingSendRecvMsg();
  } else {
    m_Logger->Write(StdStreamLogger::INFO,"%s: --->>> MDI failed to connect to TWS.", __FILENAME__);
  }
}

// Interfaces in ATU_IB_Client_MDI
void ATU_IB_MDI::tickPrice(TickerId p_tickerId, TickType p_field, double p_price, int p_canAutoExecute)
{
  //	cout << "*********************[tickPrice]" << p_field << " | " << p_price << endl;
  bool usingLevel2 = m_usingLevel2[p_tickerId];
  if (!usingLevel2) {
    //		cout << "Using level 1 " << this->m_marketDepthData[p_tickerId].m_feedcode << endl;
    // TODO:
    // DON'T implement this. Data is incorrect.
    MarketDepthData &depthData = m_marketDepthData[p_tickerId];
    IBString secType = m_subscribeList[p_tickerId].secType;
    // Best bid
    if (p_field == BID && secType != "IND") {
      //			cout << "Best bid price: " << p_price << endl;
      double *price = &(depthData.m_bidsPriceVol[0].first);
      *price = p_price;
      notify_marketfeed(depthData);
    }
    // Best ask
    else if (p_field == ASK && secType != "IND") {
      //			cout << "Best ask price: " << p_price << endl;
      double *price = &(depthData.m_asksPriceVol[0].first);
      *price = p_price;
      notify_marketfeed(depthData);
    }
    // last price
    else if (p_field == LAST && secType == "IND") {
      depthData.m_traded_price = p_price;
      notify_marketfeed(depthData);
    }
    // Mark price (just for IB internal usage)
    //		else if (p_field == 37 && secType == "IND") {
    //			depthData.m_traded_price = p_price;
    //			ATU_MDI_marketfeed_struct mf = depthData.getMarketfeed();
    //			notify_marketfeed(mf);
    //		}
  }
}

void ATU_IB_MDI::tickSize(TickerId p_tickerId, TickType p_field, int p_size)
{
  //	cout << "------------------[tickSize]" << p_field << " | " << p_size << endl;
  bool usingLevel2 = m_usingLevel2[p_tickerId];
  if (!usingLevel2) {
    //		cout << "Using level 1 " << this->m_marketDepthData[p_tickerId].m_feedcode << endl;
    // TODO:
    // Don't implement this. Data is incorrect.
    MarketDepthData &depthData = m_marketDepthData[p_tickerId];
    IBString secType = m_subscribeList[p_tickerId].secType;
    // Best bid
    if (p_field == BID_SIZE && secType != "IND") {
      //			cout << "Best bid volume: " << p_size << endl;
      depthData.m_bidsPriceVol[0].second = p_size;
      notify_marketfeed(depthData);
    }
    // Best ask
    else if (p_field == ASK_SIZE && secType != "IND") {
      //			cout << "Best ask volume: " << p_size << endl;
      depthData.m_asksPriceVol[0].second = p_size;
      notify_marketfeed(depthData);
    }
    // Last size
    else if (p_field == LAST_SIZE && secType == "IND") {
      depthData.m_traded_volume = p_size;
      notify_marketfeed(depthData);
    }
  }
}

void ATU_IB_MDI::tickOptionComputation(TickerId p_tickerId, TickType p_tickType, double p_impliedVok, double p_delta,
                                       double p_optPrice, double p_pvDividend, double p_gamma, double p_vega, double p_theta, double p_undPrice)
{

}

void ATU_IB_MDI::tickGeneric(TickerId p_tickerId, TickType p_tickType, double p_value)
{

}

/*
 * tickString() is for updating last traded price and volume of non-index products
 */
void ATU_IB_MDI::tickString(TickerId p_tickerId, TickType p_tickType, const IBString& p_value)
{
  if (p_tickType == RT_VOLUME) {
    //		cout << ">>>>>>>>>>>>>>>>>>>>>[tickString]" << p_value << endl;
    vector<string> segments;
    string s = p_value;
    boost::split(segments, s, boost::is_any_of(";"));
    if (segments.size() > 0) {
      MarketDepthData &depthData = m_marketDepthData[p_tickerId];
      double price = segments[0].size() > 0 ? boost::lexical_cast<double>(segments[0]) : depthData.m_traded_price;
      double volume = segments[3].size() > 0 ? boost::lexical_cast<double>(segments[3]) : depthData.m_traded_volume;

      depthData.m_traded_price = price;
      depthData.m_traded_volume = volume;

      notify_marketfeed(depthData);
    }
  }
}

void ATU_IB_MDI::tickEFP(TickerId p_tickerId, TickType p_tickType, double p_basisPoints, const IBString& p_formattedBasisPoints,
                         double p_totalDividends, int p_holdDays, const IBString& p_futureExpiry, double p_dividendImpact, double p_dividendsToExpiry)
{

}

void ATU_IB_MDI::tickSnapshotEnd(int p_reqId)
{

}

void ATU_IB_MDI::marketDataType(TickerId p_reqId, int p_marketDataType)
{

}

void ATU_IB_MDI::updateMktDepth(TickerId p_id, int p_position, int p_operation, int p_side, double p_price, int p_size) {
  m_usingLevel2[p_id] = true;
  //	cout << "Using level 2 " << this->m_marketDepthData[p_id].m_feedcode << endl;
  //	cout << (p_operation == 0 ? "Insert" : (p_operation == 1 ? "Update" : "Delete")) << " [" << (p_position + 1) << "] " << (p_side == 0 ? "Ask" : "Bid") << ": price " << p_price << " size " << p_size << endl;
  bool valid = m_marketDepthData[p_id].update(p_position, p_operation, p_side, p_price, p_size);
  if (valid) {

    ATU_MDI_marketfeed_struct mf;
    m_marketDepthData[p_id].getMarketfeed(mf);

    IBString secType = m_subscribeList[p_id].secType;
    if (secType == "CASH") {
      mf.m_traded_price = ( mf.m_ask_price_1 + mf.m_bid_price_1 ) / 2;
    }
    notify_marketfeed(mf);
  }
}

bool ATU_IB_MDI::on_process_subscription(const string & sFeedcode){
  m_updateTickerIdMutex.lock();

  if (m_feedcodeSet.find(sFeedcode) != m_feedcodeSet.end()) {
    //		cout << "The feedcode: " << sFeedcode << " has been subscribed." << endl;
    m_updateTickerIdMutex.unlock();
    return false;
  }

  Contract contract;
  AtuContract* atuContract = NULL;
  bool wrongFeedcode = false;

  if ( m_contractExtractor->hasProductCode(sFeedcode) ) {
    atuContract = m_contractExtractor->getContract(sFeedcode);
    Utility::initIBContractFromAtuContract(&contract, atuContract, m_contractExtractor);
  }
  // else {
  //   // The content of feedcode follows the pattern: {symbol}-{localSymbol}-{secType}
  //   vector<string> parts;
  //   boost::split(parts, sFeedcode, boost::is_any_of("-"));
  //   if (parts.size() >= 3) {
  //     contract.symbol = parts[0];
  //     contract.localSymbol = parts[1];
  //     contract.secType = parts[2];
  //     if (parts.size() > 3) {
  //       contract.currency = parts[3];
  //     }
  //     contract.exchange = s.m_market;
  //   }
  //   //	else if (sFeedcode.find('_') != string::npos) {
  //   //		Toolbox::split(parts, sFeedcode, "_");
  //   //		if (parts.size() == 2) {
  //   //			contract.conId = boost::lexical_cast<long>(parts[1]);
  //   //		} else {
  //   //			wrongFeedcode = true;
  //   //		}
  //   //	}
  //   else {
  //     wrongFeedcode = true;
  //   }
  // }
  //
  // if (wrongFeedcode) {
  //   m_Logger->Write(StdStreamLogger::INFO,"%s: Feedcode is incorrect. it should include symbol-localSymbol-secType.", __FILENAME__);
  //   m_updateTickerIdMutex.unlock();
  //   return false;
  // }

  IBString geneticTicks("100,101,104,105,106,107,165,221,225,233,236,258,293,294,295,318");
  bool snapShot = false;
  TagValueListSPtr valList;

  m_feedcodeSet.insert(sFeedcode);
  // By default, usingLevel2 is set to false. When level2 data are returned, the value will be changed to true
  m_usingLevel2[m_nextTickerId] = false;
  m_subscribeList[m_nextTickerId] = contract;
  m_marketDepthData[m_nextTickerId] = MarketDepthData(sFeedcode);
  // snapshot should be false here!
  m_clientSocket.reqMktDataSync(m_nextTickerId, contract, geneticTicks, snapShot, valList);
  m_clientSocket.reqMktDepthSync(m_nextTickerId, contract, 20, valList);
  m_nextTickerId++;

  m_updateTickerIdMutex.unlock();

  return true;
}

void ATU_IB_MDI::error(const int p_id, const int p_errorCode, const IBString p_errorString)
{
  const string errMsg = boost::lexical_cast<string>(p_errorCode) + string(" | ") + string(p_errorString);
  m_Logger->Write(StdStreamLogger::INFO,"%s: Error message: %s", __FILENAME__, errMsg.c_str());
}

void ATU_IB_MDI::notify_marketfeed(const MarketDepthData & mdd)
{
  ATU_MDI_marketfeed_struct* ecb_mfs = (ATU_MDI_marketfeed_struct *)(m_ecbMD->GetWritingPtr());
  mdd.getMarketfeed(*ecb_mfs);
  m_ecbMD->PushBack(sizeof(ATU_MDI_marketfeed_struct));
}

void ATU_IB_MDI::notify_marketfeed(const ATU_MDI_marketfeed_struct & mfs)
{
  ATU_MDI_marketfeed_struct* ecb_mfs = (ATU_MDI_marketfeed_struct *)(m_ecbMD->GetWritingPtr());
  memcpy(ecb_mfs,&mfs,sizeof(ATU_MDI_marketfeed_struct));
  m_ecbMD->PushBack(sizeof(ATU_MDI_marketfeed_struct));
}
