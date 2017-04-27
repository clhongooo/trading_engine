#include "OMD_Message_Headers.h"
#include <string>

//--------------------------------------------------
// OMD
//--------------------------------------------------
JSONNode genNode(OMD_Sequence_Reset &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("NewSeqNo",s.NewSeqNo));
return jsn;}
JSONNode genNode(OMD_Retransmission_Header &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("PktSize",s.PktSize));
  jsn.push_back(JSONNode("MsgCount",s.MsgCount));
  jsn.push_back(JSONNode("SeqNum",s.SeqNum));
  jsn.push_back(JSONNode("SendTime",SDateTime::fromUnixTimeToString(s.SendTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT)));
return jsn;}
JSONNode genNode(OMD_Logon &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  string sUsername(s.Username, sizeof(s.Username));
  jsn.push_back(JSONNode("Username", STool::Trim(sUsername)));
return jsn;}
JSONNode genNode(OMD_Logon_Response &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SessionStatus",s.SessionStatus));
return jsn;}
JSONNode genNode(OMD_Retransmission_Request &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("ChannelID",s.ChannelID));
  jsn.push_back(JSONNode("BeginSeqNum",s.BeginSeqNum));
  jsn.push_back(JSONNode("EndSeqNum",s.EndSeqNum));
return jsn;}
JSONNode genNode(OMD_Retransmission_Response &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("ChannelID",s.ChannelID));
  jsn.push_back(JSONNode("RetransStatus",s.RetransStatus));
  jsn.push_back(JSONNode("BeginSeqNum",s.BeginSeqNum));
  jsn.push_back(JSONNode("EndSeqNum",s.EndSeqNum));
return jsn;}
JSONNode genNode(OMD_Refresh_Complete &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("LastSeqNum",s.LastSeqNum));
return jsn;}


//--------------------------------------------------
// OMDC
//--------------------------------------------------
JSONNode genNode(OMDC_Market_Definition &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  string sMarketCode(s.MarketCode, sizeof(s.MarketCode));
  jsn.push_back(JSONNode("MarketCode", STool::Trim(sMarketCode)));
  string sMarketName(s.MarketName, sizeof(s.MarketName));
  jsn.push_back(JSONNode("MarketName", STool::Trim(sMarketName)));
  string sCurrencyCode(s.CurrencyCode, sizeof(s.CurrencyCode));
  jsn.push_back(JSONNode("CurrencyCode", STool::Trim(sCurrencyCode)));
  jsn.push_back(JSONNode("NumberOfSecurities",s.NumberOfSecurities));
return jsn;}
JSONNode genNode(OMDC_UnderlyingSecurity &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("UnderlyingSecurityCode",s.UnderlyingSecurityCode));
  jsn.push_back(JSONNode("UnderlyingSecurityWeight",s.UnderlyingSecurityWeight));
return jsn;}
JSONNode genNode(OMDC_Security_Definition &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  string sMarketCode(s.MarketCode, sizeof(s.MarketCode));
  jsn.push_back(JSONNode("MarketCode", STool::Trim(sMarketCode)));
  string sISINCode(s.ISINCode, sizeof(s.ISINCode));
  jsn.push_back(JSONNode("ISINCode", STool::Trim(sISINCode)));
  string sInstrumentType(s.InstrumentType, sizeof(s.InstrumentType));
  jsn.push_back(JSONNode("InstrumentType", STool::Trim(sInstrumentType)));
  string sSpreadTableCode(s.SpreadTableCode, sizeof(s.SpreadTableCode));
  jsn.push_back(JSONNode("SpreadTableCode", STool::Trim(sSpreadTableCode)));
  string sSecurityShortName(s.SecurityShortName, sizeof(s.SecurityShortName));
  jsn.push_back(JSONNode("SecurityShortName", STool::Trim(sSecurityShortName)));
  string sCurrencyCode(s.CurrencyCode, sizeof(s.CurrencyCode));
  jsn.push_back(JSONNode("CurrencyCode", STool::Trim(sCurrencyCode)));

  {
    buffer[60 + 1] = '\0';
    strncpy(buffer, (const char*)(&s.SecurityNameGCCS), sizeof(s.SecurityNameGCCS));
    jsn.push_back(JSONNode("SecurityNameGCCS", buffer));
  }
  {
    buffer[60 + 1] = '\0';
    strncpy(buffer, (const char*)(&s.SecurityNameGB), sizeof(s.SecurityNameGB));
    jsn.push_back(JSONNode("SecurityNameGB", buffer));
  }

  jsn.push_back(JSONNode("LotSize",s.LotSize));
  jsn.push_back(JSONNode("PreviousClosingPrice",s.PreviousClosingPrice));
  if (s.ShortSellFlag != 0)
    jsn.push_back(JSONNode("ShortSellFlag",string(&s.ShortSellFlag,sizeof(s.ShortSellFlag))));
  else
    jsn.push_back(JSONNode("ShortSellFlag",""));
  if (s.CCASSFlag != 0)
    jsn.push_back(JSONNode("CCASSFlag",string(&s.CCASSFlag,sizeof(s.CCASSFlag))));
  else
    jsn.push_back(JSONNode("CCASSFlag",""));
  if (s.DummySecurityFlag != 0)
    jsn.push_back(JSONNode("DummySecurityFlag",string(&s.DummySecurityFlag,sizeof(s.DummySecurityFlag))));
  else
    jsn.push_back(JSONNode("DummySecurityFlag",""));
  if (s.TestSecurityFlag != 0)
    jsn.push_back(JSONNode("TestSecurityFlag",string(&s.TestSecurityFlag,sizeof(s.TestSecurityFlag))));
  else
    jsn.push_back(JSONNode("TestSecurityFlag",""));
  if (s.StampDutyFlag != 0)
    jsn.push_back(JSONNode("StampDutyFlag",string(&s.StampDutyFlag,sizeof(s.StampDutyFlag))));
  else
    jsn.push_back(JSONNode("StampDutyFlag",""));
  jsn.push_back(JSONNode("ListingDate",s.ListingDate));
  jsn.push_back(JSONNode("DelistingDate",s.DelistingDate));
  string sFreeText(s.FreeText, sizeof(s.FreeText));
  jsn.push_back(JSONNode("FreeText", STool::Trim(sFreeText)));
  if (s.EFNFlag != 0)
    jsn.push_back(JSONNode("EFNFlag",string(&s.EFNFlag,sizeof(s.EFNFlag))));
  else
    jsn.push_back(JSONNode("EFNFlag",""));
  jsn.push_back(JSONNode("AccruedInterest",s.AccruedInterest));
  jsn.push_back(JSONNode("CouponRate",s.CouponRate));
  jsn.push_back(JSONNode("ConversionRatio",s.ConversionRatio));
  jsn.push_back(JSONNode("StrikePrice",s.StrikePrice));
  jsn.push_back(JSONNode("MaturityDate",s.MaturityDate));
  if (s.CallPutFlag != 0)
    jsn.push_back(JSONNode("CallPutFlag",string(&s.CallPutFlag,sizeof(s.CallPutFlag))));
  else
    jsn.push_back(JSONNode("CallPutFlag",""));
  if (s.Style != 0)
    jsn.push_back(JSONNode("Style",string(&s.Style,sizeof(s.Style))));
  else
    jsn.push_back(JSONNode("Style",""));
  jsn.push_back(JSONNode("NoUnderlyingSecurities",s.NoUnderlyingSecurities));
  for (unsigned int  i = 0; i < s.NoUnderlyingSecurities ; ++i)
  {
    OMDC_UnderlyingSecurity us;
    us.UnderlyingSecurityCode = READ_UINT32((BYTE *)&s +280 +8*i);
    us.UnderlyingSecurityWeight = READ_UINT32((BYTE *)&s +280 +4 +8*i);
    JSONNode s_DOT_UnderlyingSecurities = genNode(us,"UnderlyingSecurities", buffer, bufferLength);
    s_DOT_UnderlyingSecurities.set_name("UnderlyingSecurities");
    jsn.push_back(s_DOT_UnderlyingSecurities);
  }
return jsn;}
JSONNode genNode(OMDC_Liquidity_Provider &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("NoLiquidityProviders",s.NoLiquidityProviders));
  for (unsigned int i = 0; i < s.NoLiquidityProviders; ++i)
  {
    jsn.push_back(JSONNode("LPBrokerNumber",READ_UINT16((BYTE*)&s+10+2*i)));
  }
return jsn;}
JSONNode genNode(OMDC_Currency_Rate &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  string sCurrencyCode(s.CurrencyCode, sizeof(s.CurrencyCode));
  jsn.push_back(JSONNode("CurrencyCode", STool::Trim(sCurrencyCode)));
  jsn.push_back(JSONNode("CurrencyFactor",s.CurrencyFactor));
  jsn.push_back(JSONNode("CurrencyRate",s.CurrencyRate));
return jsn;}
JSONNode genNode(OMDC_Trading_Session_Status &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  string sMarketCode(s.MarketCode, sizeof(s.MarketCode));
  jsn.push_back(JSONNode("MarketCode", STool::Trim(sMarketCode)));
  jsn.push_back(JSONNode("TradingSessionID",s.TradingSessionID));
  jsn.push_back(JSONNode("TradingSessionSubID",s.TradingSessionSubID));
  jsn.push_back(JSONNode("TradingSesStatus",s.TradingSesStatus));
  if (s.TradingSesControlFlag != 0)
    jsn.push_back(JSONNode("TradingSesControlFlag",string(&s.TradingSesControlFlag,sizeof(s.TradingSesControlFlag))));
  else
    jsn.push_back(JSONNode("TradingSesControlFlag",""));
  jsn.push_back(JSONNode("StartDateTime",SDateTime::fromUnixTimeToString(s.StartDateTime,SDateTime::NANOSEC,SDateTime::GMT,SDateTime::HKT)));
  jsn.push_back(JSONNode("EndDateTime",SDateTime::fromUnixTimeToString(s.EndDateTime,SDateTime::NANOSEC,SDateTime::GMT,SDateTime::HKT)));
return jsn;}
JSONNode genNode(OMDC_Security_Status &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("SecurityTradingStatus",s.SecurityTradingStatus));
return jsn;}
JSONNode genNode(OMDC_Add_Order &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("OrderID",s.OrderID));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("Quantity",s.Quantity));
  jsn.push_back(JSONNode("Side",s.Side));
  if (s.OrderType != 0)
    jsn.push_back(JSONNode("OrderType",string(&s.OrderType,sizeof(s.OrderType))));
  else
    jsn.push_back(JSONNode("OrderType",""));
  jsn.push_back(JSONNode("OrderBookPosition",s.OrderBookPosition));
return jsn;}
JSONNode genNode(OMDC_Modify_Order &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("OrderID",s.OrderID));
  jsn.push_back(JSONNode("Quantity",s.Quantity));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("Side",s.Side));
  jsn.push_back(JSONNode("OrderBookPosition",s.OrderBookPosition));
return jsn;}
JSONNode genNode(OMDC_Delete_Order &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("OrderID",s.OrderID));
  jsn.push_back(JSONNode("Side",s.Side));
return jsn;}
JSONNode genNode(OMDC_Add_Odd_Lot_Order &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("OrderID",s.OrderID));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("Quantity",s.Quantity));
  jsn.push_back(JSONNode("BrokerID",s.BrokerID));
  jsn.push_back(JSONNode("Side",s.Side));
return jsn;}
JSONNode genNode(OMDC_Delete_Odd_Lot_Order &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("OrderID",s.OrderID));
  jsn.push_back(JSONNode("BrokerID",s.BrokerID));
  jsn.push_back(JSONNode("Side",s.Side));
return jsn;}
JSONNode genNode(OMDC_Aggregate_Order_Book_Update_Details &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("AggregateQuantity",s.AggregateQuantity));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("NumberOfOrders",s.NumberOfOrders));
  jsn.push_back(JSONNode("Side",s.Side));
  jsn.push_back(JSONNode("PriceLevel",s.PriceLevel));
  jsn.push_back(JSONNode("UpdateAction",s.UpdateAction));
return jsn;}
JSONNode genNode(OMDC_Aggregate_Order_Book_Update &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("NoEntries",s.NoEntries));
  for (unsigned int i = 0; i < s.NoEntries; ++i)
  {
    OMDC_Aggregate_Order_Book_Update_Details* paobud;
    paobud = (OMDC_Aggregate_Order_Book_Update_Details*)((BYTE*)&s+12+24*i);

    JSONNode s_DOT_Aggregate_Order_Book_Update_Details = genNode(*paobud,"AggOrdBookUpdateDetails", buffer, bufferLength);
    s_DOT_Aggregate_Order_Book_Update_Details.set_name("AggOrdBookUpdateDetails");
    jsn.push_back(s_DOT_Aggregate_Order_Book_Update_Details);
  }
return jsn;}
JSONNode genNode(OMDC_Broker_Queue_Item &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("Item",s.Item));
  if (s.Type != 0)
    jsn.push_back(JSONNode("Type",string(&s.Type,sizeof(s.Type))));
  else
    jsn.push_back(JSONNode("Type",""));
return jsn;}
JSONNode genNode(OMDC_Broker_Queue &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("ItemCount",s.ItemCount));
  jsn.push_back(JSONNode("Side",lexical_cast<string>(s.Side)));
  if (s.BQMoreFlag != 0)
    jsn.push_back(JSONNode("BQMoreFlag",string(&s.BQMoreFlag,sizeof(s.BQMoreFlag))));
  else
    jsn.push_back(JSONNode("BQMoreFlag",""));
  for (unsigned int i = 0; i < s.ItemCount; ++i)
  {
    OMDC_Broker_Queue_Item bqi;
    bqi.Item = READ_UINT16((BYTE *)&s+12+4*i); 
    bqi.Type = *((BYTE *)&s+12+2+4*i);
    JSONNode s_DOT_BrokerQueueItem = genNode(bqi,"BrokerQueueItem", buffer, bufferLength);
    s_DOT_BrokerQueueItem.set_name("BrokerQueueItem");
    jsn.push_back(s_DOT_BrokerQueueItem);
  }
return jsn;}
JSONNode genNode(OMDC_Trade &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("TradeID",s.TradeID));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("Quantity",s.Quantity));
  jsn.push_back(JSONNode("TrdType",s.TrdType));
  jsn.push_back(JSONNode("TradeTime",SDateTime::fromUnixTimeToString(s.TradeTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT)));
return jsn;}
JSONNode genNode(OMDC_Trade_Cancel &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("TradeID",s.TradeID));
return jsn;}
JSONNode genNode(OMDC_Trade_Ticker &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("TickerID",s.TickerID));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("AggregateQuantity",s.AggregateQuantity));
  jsn.push_back(JSONNode("TradeTime",SDateTime::fromUnixTimeToString(s.TradeTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT)));
  jsn.push_back(JSONNode("TrdType",s.TrdType));
  if (s.TrdCancelFlag != 0)
    jsn.push_back(JSONNode("TrdCancelFlag",string(&s.TrdCancelFlag,sizeof(s.TrdCancelFlag))));
  else
    jsn.push_back(JSONNode("TrdCancelFlag",""));
return jsn;}
JSONNode genNode(OMDC_Closing_Price &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("ClosingPrice",s.ClosingPrice));
  jsn.push_back(JSONNode("NumberOfTrades",s.NumberOfTrades));
return jsn;}
JSONNode genNode(OMDC_Nominal_Price &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("NominalPrice",s.NominalPrice));
return jsn;}
JSONNode genNode(OMDC_Indicative_Equilibrium_Price &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("AggregateQuantity",s.AggregateQuantity));
return jsn;}
JSONNode genNode(OMDC_Statistics &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("SharesTraded",s.SharesTraded));
  jsn.push_back(JSONNode("Turnover",s.Turnover));
  jsn.push_back(JSONNode("HighPrice",s.HighPrice));
  jsn.push_back(JSONNode("LowPrice",s.LowPrice));
  jsn.push_back(JSONNode("LastPrice",s.LastPrice));
  jsn.push_back(JSONNode("VWAP",s.VWAP));
  jsn.push_back(JSONNode("ShortSellSharesTraded",s.ShortSellSharesTraded));
  jsn.push_back(JSONNode("ShortSellTurnover",s.ShortSellTurnover));
return jsn;}
JSONNode genNode(OMDC_Market_Turnover &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  string sMarketCode(s.MarketCode, sizeof(s.MarketCode));
  jsn.push_back(JSONNode("MarketCode", STool::Trim(sMarketCode)));
  string sCurrencyCode(s.CurrencyCode, sizeof(s.CurrencyCode));
  jsn.push_back(JSONNode("CurrencyCode", STool::Trim(sCurrencyCode)));
  jsn.push_back(JSONNode("Turnover",s.Turnover));
return jsn;}
JSONNode genNode(OMDC_Yield &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("SecurityCode",s.SecurityCode));
  jsn.push_back(JSONNode("Yield",s.Yield));
return jsn;}
JSONNode genNode(OMDC_News &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  string sNewsType(s.NewsType, sizeof(s.NewsType));
  jsn.push_back(JSONNode("NewsType", STool::Trim(sNewsType)));
  string sNewsID(s.NewsID, sizeof(s.NewsID));
  jsn.push_back(JSONNode("NewsID", STool::Trim(sNewsID)));

  {
    buffer[320 + 1] = '\0';
    strncpy(buffer, (const char*)(&s.Headline), 320);
    jsn.push_back(JSONNode("Headline", buffer));
  }

  if (s.CancelFlag != 0)
    jsn.push_back(JSONNode("CancelFlag",string(&s.CancelFlag,sizeof(s.CancelFlag))));
  else
    jsn.push_back(JSONNode("CancelFlag",""));
  if (s.LastFragment != 0)
    jsn.push_back(JSONNode("LastFragment",string(&s.LastFragment,sizeof(s.LastFragment))));
  else
    jsn.push_back(JSONNode("LastFragment",""));
  jsn.push_back(JSONNode("ReleaseTime",SDateTime::fromUnixTimeToString(s.ReleaseTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT)));
  jsn.push_back(JSONNode("NoMarketCodes",s.NoMarketCodes));

  for (unsigned int i = 0; i < s.NoMarketCodes; ++i)
  {
    memset(buffer,'\0',6);
    strncpy(buffer, (const char*)((BYTE*)&s+348+4*i), 4);
    string sMarketCode(buffer);
    jsn.push_back(JSONNode("MarketCode", STool::Trim(sMarketCode)));
  }

  uint16_t iNoSecurityCodes = *((uint16_t*)((BYTE*)&s+350+4*s.NoMarketCodes));
  jsn.push_back(JSONNode("NoSecurityCodes", iNoSecurityCodes));

  for (unsigned int i = 0; i < iNoSecurityCodes; ++i)
  {
    jsn.push_back(JSONNode("SecurityCode",*((uint32_t*)((BYTE*)&s+352+4*s.NoMarketCodes+4*i))));
  }

  uint16_t iNoNewsLines = *((uint16_t*)((BYTE*)&s+354+4*s.NoMarketCodes+4*iNoSecurityCodes));
  jsn.push_back(JSONNode("NoNewsLines",iNoNewsLines));

  for (unsigned int i = 0; i < iNoNewsLines; ++i)
  {
    buffer[160 + 1] = '\0';
    strncpy(buffer, (const char *)((BYTE*)&s+356+4*s.NoMarketCodes+4*iNoSecurityCodes), 160);
    jsn.push_back(JSONNode("NewsLine", buffer));
  }
return jsn;}
JSONNode genNode(OMDC_Index_Definition &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  string sIndexCode(s.IndexCode, sizeof(s.IndexCode));
  jsn.push_back(JSONNode("IndexCode", STool::Trim(sIndexCode)));
  if (s.IndexSource != 0)
  {
    string sIndexSource(&s.IndexSource,sizeof(s.IndexSource));
    jsn.push_back(JSONNode("IndexSource",STool::Trim(sIndexSource)));
  }
  else
  {
    jsn.push_back(JSONNode("IndexSource",""));
  }
  string sCurrencyCode(s.CurrencyCode, sizeof(s.CurrencyCode));
  jsn.push_back(JSONNode("CurrencyCode", STool::Trim(sCurrencyCode)));
return jsn;}
JSONNode genNode(OMDC_Index_Data &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  string sIndexCode(s.IndexCode, sizeof(s.IndexCode));
  jsn.push_back(JSONNode("IndexCode", STool::Trim(sIndexCode)));
  if (s.IndexStatus != 0)
  {
    string sIndexStatus(&s.IndexStatus,sizeof(s.IndexStatus));
    jsn.push_back(JSONNode("IndexStatus",STool::Trim(sIndexStatus)));
  }
  else
  {
    jsn.push_back(JSONNode("IndexStatus",""));
  }
  jsn.push_back(JSONNode("IndexTime",SDateTime::fromUnixTimeToString(s.IndexTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT)));
  jsn.push_back(JSONNode("IndexValue",s.IndexValue));
  jsn.push_back(JSONNode("NetChgPrevDay",s.NetChgPrevDay));
  jsn.push_back(JSONNode("HighValue",s.HighValue));
  jsn.push_back(JSONNode("LowValue",s.LowValue));
  jsn.push_back(JSONNode("EASValue",s.EASValue));
  jsn.push_back(JSONNode("IndexTurnover",s.IndexTurnover));
  jsn.push_back(JSONNode("OpeningValue",s.OpeningValue));
  jsn.push_back(JSONNode("ClosingValue",s.ClosingValue));
  jsn.push_back(JSONNode("PreviousSesClose",s.PreviousSesClose));
  jsn.push_back(JSONNode("IndexVolume",s.IndexVolume));
  jsn.push_back(JSONNode("NetChgPrevDayPct",s.NetChgPrevDayPct));
  if (s.Exception != 0)
  {
    string sException(&s.Exception,sizeof(s.Exception));
    jsn.push_back(JSONNode("Exception",STool::Trim(sException)));
  }
  else
  {
    jsn.push_back(JSONNode("Exception",""));
  }
return jsn;}


//--------------------------------------------------
// OMDD
//--------------------------------------------------
JSONNode genNode (OMDD_Commodity_Definition &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("CommodityCode",s.CommodityCode));
  jsn.push_back(JSONNode("DecimallnUnderlyingPrice",s.DecimallnUnderlyingPrice));
  string sISINCode(s.ISINCode,sizeof(s.ISINCode));
  jsn.push_back(JSONNode("ISINCode",STool::Trim(sISINCode)));
  string sBaseCurrency(s.BaseCurrency,sizeof(s.BaseCurrency));
  jsn.push_back(JSONNode("BaseCurrency",STool::Trim(sBaseCurrency)));
  jsn.push_back(JSONNode("UnderlyingPriceUnit",s.UnderlyingPriceUnit));
  string sCommodityName(s.CommodityName,sizeof(s.CommodityName));
  jsn.push_back(JSONNode("CommodityName",STool::Trim(sCommodityName)));
  jsn.push_back(JSONNode("NominalValue",s.NominalValue));
  string sUnderlyingCode(s.UnderlyingCode,sizeof(s.UnderlyingCode));
  jsn.push_back(JSONNode("UnderlyingCode",STool::Trim(sUnderlyingCode)));
  jsn.push_back(JSONNode("UnderlyingType",s.UnderlyingType));
  jsn.push_back(JSONNode("EffectiveTomorrow",s.EffectiveTomorrow));
return jsn;}
JSONNode genNode (OMDD_Class_Definition &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("Country",s.Country));
  jsn.push_back(JSONNode("Market",s.Market));
  jsn.push_back(JSONNode("InstrumentGroup",s.InstrumentGroup));
  jsn.push_back(JSONNode("Modifier",s.Modifier));
  jsn.push_back(JSONNode("CommodityCode",s.CommodityCode));
  jsn.push_back(JSONNode("PriceQuotationFactor",s.PriceQuotationFactor));
  jsn.push_back(JSONNode("ContractSize",s.ContractSize));
  jsn.push_back(JSONNode("DecimallnStrikePrice",s.DecimallnStrikePrice));
  jsn.push_back(JSONNode("DecimallnContractSize",s.DecimallnContractSize));
  jsn.push_back(JSONNode("DecimallnPremium",s.DecimallnPremium));
  jsn.push_back(JSONNode("RankingType",s.RankingType));
  jsn.push_back(JSONNode("Tradable",s.Tradable));
  jsn.push_back(JSONNode("PremiumUnit4Price",s.PremiumUnit4Price));
  string sBaseCurrency(s.BaseCurrency ,sizeof(s.BaseCurrency));
  jsn.push_back(JSONNode("BaseCurrency",STool::Trim(sBaseCurrency)));
  string sInstrumentClassID(s.InstrumentClassID ,sizeof(s.InstrumentClassID));
  jsn.push_back(JSONNode("InstrumentClassID",STool::Trim(sInstrumentClassID)));
  string sInstrumentClassName(s.InstrumentClassName,sizeof(s.InstrumentClassName));
  jsn.push_back(JSONNode("InstrumentClassName",STool::Trim(sInstrumentClassName)));
  if (s.IsFractions != 0)
    jsn.push_back(JSONNode("IsFractions",string(&s.IsFractions,sizeof(s.IsFractions))));
  else
    jsn.push_back(JSONNode("IsFractions",""));
  string sSettlementCurrencyID(s.SettlementCurrencyID,sizeof(s.SettlementCurrencyID));
  jsn.push_back(JSONNode("SettlementCurrencyID",STool::Trim(sSettlementCurrencyID)));
  jsn.push_back(JSONNode("EffectiveTomorrow",s.EffectiveTomorrow));
return jsn;}
JSONNode genNode (OMDD_Series_Definition_Base &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  string sSymbol(s.Symbol,sizeof(s.Symbol));
  jsn.push_back(JSONNode("Symbol",STool::Trim(sSymbol)));
  jsn.push_back(JSONNode("FinancialProduct",s.FinancialProduct));
  jsn.push_back(JSONNode("NumberOfDecimalsPrice",s.NumberOfDecimalsPrice));
  jsn.push_back(JSONNode("NumberOfLegs",s.NumberOfLegs));
  jsn.push_back(JSONNode("StrikePrice",s.StrikePrice));
  string sExpirationDate(s.ExpirationDate,sizeof(s.ExpirationDate));
  jsn.push_back(JSONNode("ExpirationDate",STool::Trim(sExpirationDate)));
  jsn.push_back(JSONNode("PutOrCall",s.PutOrCall));
return jsn;}
JSONNode genNode (OMDD_Series_Definition_Extended &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  string sSymbol(s.Symbol,sizeof(s.Symbol));
  jsn.push_back(JSONNode("Symbol",STool::Trim(sSymbol)));
  jsn.push_back(JSONNode("Country",s.Country));
  jsn.push_back(JSONNode("Market",s.Market));
  jsn.push_back(JSONNode("InstrumentGroup",s.InstrumentGroup));
  jsn.push_back(JSONNode("Modifier",s.Modifier));
  jsn.push_back(JSONNode("CommodityCode",s.CommodityCode));
  jsn.push_back(JSONNode("ExpirationDate",SDateTime::fromGeniumDateToString(s.ExpirationDate)));
  jsn.push_back(JSONNode("StrikePrice",s.StrikePrice));
  jsn.push_back(JSONNode("ContractSize",s.ContractSize));
  string sISINCode(s.ISINCode,sizeof(s.ISINCode));
  jsn.push_back(JSONNode("ISINCode",STool::Trim(sISINCode)));
  jsn.push_back(JSONNode("SeriesStatus",s.SeriesStatus));
  jsn.push_back(JSONNode("EffectiveTomorrow",s.EffectiveTomorrow));
  string sEffectiveExpDate(s.EffectiveExpDate,sizeof(s.EffectiveExpDate));
  jsn.push_back(JSONNode("EffectiveExpDate",STool::Trim(sEffectiveExpDate)));
  jsn.push_back(JSONNode("DateTimeLastTrading",SDateTime::fromUnixTimeToString(s.DateTimeLastTrading,SDateTime::NANOSEC,SDateTime::GMT,SDateTime::HKT)));
return jsn;}
JSONNode genNode (OMDD_Combination_Definition &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("ComboOrderbookID",s.ComboOrderbookID));
  jsn.push_back(JSONNode("LegOrderbookID",s.LegOrderbookID));
  string sLegSide(&s.LegSide ,sizeof(s.LegSide));
  jsn.push_back(JSONNode("LegSide",STool::Trim(sLegSide)));
  jsn.push_back(JSONNode("LegRatio",s.LegRatio));
return jsn;}
JSONNode genNode (OMDD_Market_Status &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("StateLevel",s.StateLevel));
  jsn.push_back(JSONNode("Market",s.Market));
  jsn.push_back(JSONNode("Instrument",s.Instrument));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("CommodityCode",s.CommodityCode));
  string sActualStartDate(s.ActualStartDate,sizeof(s.ActualStartDate));
  jsn.push_back(JSONNode("ActualStartDate",STool::Trim(sActualStartDate)));
  string sActualStartTime(s.ActualStartTime,sizeof(s.ActualStartTime));
  jsn.push_back(JSONNode("ActualStartTime",STool::Trim(sActualStartTime)));
  string sPlannedStartDate(s.PlannedStartDate ,sizeof(s.PlannedStartDate));
  jsn.push_back(JSONNode("PlannedStartDate",STool::Trim(sPlannedStartDate)));
  string sPlannedStartTime(s.PlannedStartTime ,sizeof(s.PlannedStartTime));
  jsn.push_back(JSONNode("PlannedStartTime",STool::Trim(sPlannedStartTime)));
  jsn.push_back(JSONNode("SecondsToStateChange",s.SecondsToStateChange));
  jsn.push_back(JSONNode("State",s.State));
  jsn.push_back(JSONNode("Priority",s.Priority));
return jsn;}
JSONNode genNode (OMDD_Series_Status &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  string sSuspended(&s.Suspended ,sizeof(s.Suspended));
  jsn.push_back(JSONNode("Suspended",STool::Trim(sSuspended)));
return jsn;}
JSONNode genNode (OMDD_Commodity_Status &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("CommodityCode",s.CommodityCode));
  string sSuspended(&s.Suspended ,sizeof(s.Suspended));
  jsn.push_back(JSONNode("Suspended",STool::Trim(sSuspended)));
return jsn;}
JSONNode genNode (OMDD_Add_Order &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("OrderID",s.OrderID));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("Quantity",s.Quantity));
  jsn.push_back(JSONNode("Side",s.Side));
  jsn.push_back(JSONNode("LotType",s.LotType));
  jsn.push_back(JSONNode("OrderType",s.OrderType));
  jsn.push_back(JSONNode("OrderBookPosition",s.OrderBookPosition));
return jsn;}
JSONNode genNode (OMDD_Modify_Order &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("OrderID",s.OrderID));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("Quantity",s.Quantity));
  jsn.push_back(JSONNode("Side",s.Side));
  jsn.push_back(JSONNode("OrderType",s.OrderType));
  jsn.push_back(JSONNode("OrderBookPosition",s.OrderBookPosition));
return jsn;}
JSONNode genNode (OMDD_Delete_Order &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("OrderID",s.OrderID));
  jsn.push_back(JSONNode("Side",s.Side));
return jsn;}
JSONNode genNode (OMDD_Aggregate_Order_Book_Update_Details &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("AggregateQuantity",s.AggregateQuantity));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("NumberOfOrders",s.NumberOfOrders));
  jsn.push_back(JSONNode("Side",s.Side));
  jsn.push_back(JSONNode("PriceLevel",s.PriceLevel));
  jsn.push_back(JSONNode("UpdateAction",s.UpdateAction));
return jsn;}
JSONNode genNode (OMDD_Aggregate_Order_Book_Update &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("NoEntries",s.NoEntries));
  for (unsigned int i = 0; i < s.NoEntries; ++i)
  {
    OMDD_Aggregate_Order_Book_Update_Details* paobud;
    paobud = (OMDD_Aggregate_Order_Book_Update_Details*)((BYTE*)&s+12+24*i);

    JSONNode s_DOT_Aggregate_Order_Book_Update_Details = genNode(*paobud,"AggOrdBookUpdateDetails", buffer, bufferLength);
    s_DOT_Aggregate_Order_Book_Update_Details.set_name("AggOrdBookUpdateDetails");
    jsn.push_back(s_DOT_Aggregate_Order_Book_Update_Details);
  }
return jsn;}
JSONNode genNode (OMDD_Order_Book_Clear &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
return jsn;}
JSONNode genNode (OMDD_Quote_Request &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("NumberOfLots",s.NumberOfLots));
  jsn.push_back(JSONNode("BidAskFlag",s.BidAskFlag));
return jsn;}
JSONNode genNode (OMDD_Trade &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("OrderID",s.OrderID));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("TradeID",s.TradeID));
  jsn.push_back(JSONNode("ComboGroupID",s.ComboGroupID));
  jsn.push_back(JSONNode("Side",s.Side));
  jsn.push_back(JSONNode("DealType",s.DealType));
  jsn.push_back(JSONNode("TradeCondition",s.TradeCondition));
  jsn.push_back(JSONNode("DealInfo",s.DealInfo));
  jsn.push_back(JSONNode("Quantity",s.Quantity));
  jsn.push_back(JSONNode("TradeTime",SDateTime::fromUnixTimeToString(s.TradeTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT)));
return jsn;}
JSONNode genNode (OMDD_Trade_Amendment &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("TradeID",s.TradeID));
  jsn.push_back(JSONNode("ComboGroupID",s.ComboGroupID));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("Quantity",s.Quantity));
  jsn.push_back(JSONNode("TradeTime",SDateTime::fromUnixTimeToString(s.TradeTime, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::HKT)));
  jsn.push_back(JSONNode("TradeState",s.TradeState));
return jsn;}
JSONNode genNode (OMDD_Trade_Statistics &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("DealSource",s.DealSource));
  jsn.push_back(JSONNode("Session",s.Session));
  jsn.push_back(JSONNode("AggregrateQuantity",s.AggregrateQuantity));
  jsn.push_back(JSONNode("Open",s.Open));
  jsn.push_back(JSONNode("High",s.High));
  jsn.push_back(JSONNode("Low",s.Low));
  jsn.push_back(JSONNode("TradeReportVolume",s.TradeReportVolume));
  jsn.push_back(JSONNode("DealCount",s.DealCount));
  jsn.push_back(JSONNode("Turnover",s.Turnover));
return jsn;}
JSONNode genNode (OMDD_Series_Statistics &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("Session",s.Session));
  jsn.push_back(JSONNode("Open",s.Open));
  jsn.push_back(JSONNode("High",s.High));
  jsn.push_back(JSONNode("Low",s.Low));
  jsn.push_back(JSONNode("TradeReportVolume",s.TradeReportVolume));
  jsn.push_back(JSONNode("DealCount",s.DealCount));
  jsn.push_back(JSONNode("Price",s.Price));
  jsn.push_back(JSONNode("Turnover",s.Turnover));
return jsn;}
JSONNode genNode (OMDD_Calculated_Opening_Price &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("CalculatedOpeningPrice",s.CalculatedOpeningPrice));
  jsn.push_back(JSONNode("Quantity",s.Quantity));
return jsn;}
JSONNode genNode (OMDD_Estimated_Average_Settlement_Price &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  string sEASType(&s.EASType,sizeof(s.EASType));
  jsn.push_back(JSONNode("EASType",STool::Trim(sEASType)));
  string sInstrumentCode(s.InstrumentCode,sizeof(s.InstrumentCode));
  jsn.push_back(JSONNode("InstrumentCode",STool::Trim(sInstrumentCode)));
  jsn.push_back(JSONNode("EAS",s.EAS));
return jsn;}
JSONNode genNode (OMDD_Market_Alert &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("AlertID",s.AlertID));
  if (s.Source != 0)
    jsn.push_back(JSONNode("Source",string(&s.Source,sizeof(s.Source))));
  else
    jsn.push_back(JSONNode("Source",""));

  {
    buffer[320 + 1] = '\0';
    strncpy(buffer, (const char*)(&s.Header), 320);
    jsn.push_back(JSONNode("Header", buffer));
  }

  if (s.LastFragment != 0)
    jsn.push_back(JSONNode("LastFragment",string(&s.LastFragment,sizeof(s.LastFragment))));
  else
    jsn.push_back(JSONNode("LastFragment",""));
  jsn.push_back(JSONNode("InfoType",s.InfoType));
  jsn.push_back(JSONNode("Priority",s.Priority));
  jsn.push_back(JSONNode("NoLines",s.NoLines));
  for (unsigned int i = 0; i < s.NoLines; ++i)
  {
    buffer[320 + 1] = '\0';
    strncpy(buffer, (const char*)((BYTE*)&s+332+320*i), 320);
    jsn.push_back(JSONNode("Content", buffer));
  }
return jsn;}
JSONNode genNode (OMDD_Open_Interest &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("DayIndicator",s.DayIndicator));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("Settlement",s.Settlement));
  jsn.push_back(JSONNode("DealCount",s.DealCount));
  jsn.push_back(JSONNode("GrossOI",s.GrossOI));
  jsn.push_back(JSONNode("NetOI",s.NetOI));
  jsn.push_back(JSONNode("Turnover",s.Turnover));
return jsn;}
JSONNode genNode (OMDD_Implied_Volatility &s, string name, char *buffer, int bufferLength) {
  JSONNode jsn(JSON_NODE);
  jsn.push_back(JSONNode("MsgSize",s.MsgSize));
  jsn.push_back(JSONNode("MsgType",s.MsgType));
  jsn.push_back(JSONNode("OrderbookID",s.OrderbookID));
  jsn.push_back(JSONNode("ImpliedVolatility",s.ImpliedVolatility));
return jsn;}



//--------------------------------------------------
// OMD
//--------------------------------------------------
  string printStruct(OMD_Sequence_Reset &s, string varName, char *buffer, int bufferLength)                         { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMD_Retransmission_Header &s, string varName, char *buffer, int bufferLength)                  { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMD_Logon &s, string varName, char *buffer, int bufferLength)                                  { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMD_Logon_Response &s, string varName, char *buffer, int bufferLength)                         { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMD_Retransmission_Request &s, string varName, char *buffer, int bufferLength)                 { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMD_Retransmission_Response &s, string varName, char *buffer, int bufferLength)                { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMD_Refresh_Complete &s, string varName, char *buffer, int bufferLength)                       { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}

//--------------------------------------------------
// OMDC
//--------------------------------------------------
  string printStruct(OMDC_Market_Definition &s, string varName, char *buffer, int bufferLength)                     { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_UnderlyingSecurity &s, string varName, char *buffer, int bufferLength)                    { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Security_Definition &s, string varName, char *buffer, int bufferLength)                   { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Liquidity_Provider &s, string varName, char *buffer, int bufferLength)                    { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Currency_Rate &s, string varName, char *buffer, int bufferLength)                         { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Trading_Session_Status &s, string varName, char *buffer, int bufferLength)                { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Security_Status &s, string varName, char *buffer, int bufferLength)                       { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Add_Order &s, string varName, char *buffer, int bufferLength)                             { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Modify_Order &s, string varName, char *buffer, int bufferLength)                          { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Delete_Order &s, string varName, char *buffer, int bufferLength)                          { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Add_Odd_Lot_Order &s, string varName, char *buffer, int bufferLength)                     { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Delete_Odd_Lot_Order &s, string varName, char *buffer, int bufferLength)                  { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Aggregate_Order_Book_Update_Details &s, string varName, char *buffer, int bufferLength)   { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Aggregate_Order_Book_Update &s, string varName, char *buffer, int bufferLength)           { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Broker_Queue_Item &s, string varName, char *buffer, int bufferLength)                     { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Broker_Queue &s, string varName, char *buffer, int bufferLength)                          { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Trade &s, string varName, char *buffer, int bufferLength)                                 { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Trade_Cancel &s, string varName, char *buffer, int bufferLength)                          { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Trade_Ticker &s, string varName, char *buffer, int bufferLength)                          { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Closing_Price &s, string varName, char *buffer, int bufferLength)                         { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Nominal_Price &s, string varName, char *buffer, int bufferLength)                         { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Indicative_Equilibrium_Price &s, string varName, char *buffer, int bufferLength)          { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Statistics &s, string varName, char *buffer, int bufferLength)                            { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Market_Turnover &s, string varName, char *buffer, int bufferLength)                       { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Yield &s, string varName, char *buffer, int bufferLength)                                 { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_News &s, string varName, char *buffer, int bufferLength)                                  { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Index_Definition &s, string varName, char *buffer, int bufferLength)                      { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDC_Index_Data &s, string varName, char *buffer, int bufferLength)                            { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}

//--------------------------------------------------
// OMDD
//--------------------------------------------------
  string printStruct(OMDD_Commodity_Definition &s, string varName, char *buffer, int bufferLength)                  { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Class_Definition &s, string varName, char *buffer, int bufferLength)                      { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Series_Definition_Base &s, string varName, char *buffer, int bufferLength)                { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Series_Definition_Extended &s, string varName, char *buffer, int bufferLength)            { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Combination_Definition &s, string varName, char *buffer, int bufferLength)                { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Market_Status &s, string varName, char *buffer, int bufferLength)                         { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Series_Status &s, string varName, char *buffer, int bufferLength)                         { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Commodity_Status &s, string varName, char *buffer, int bufferLength)                      { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Add_Order &s, string varName, char *buffer, int bufferLength)                             { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Modify_Order &s, string varName, char *buffer, int bufferLength)                          { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Delete_Order &s, string varName, char *buffer, int bufferLength)                          { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Aggregate_Order_Book_Update_Details &s, string varName, char *buffer, int bufferLength)   { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Aggregate_Order_Book_Update &s, string varName, char *buffer, int bufferLength)           { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Order_Book_Clear &s, string varName, char *buffer, int bufferLength)                      { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Quote_Request &s, string varName, char *buffer, int bufferLength)                         { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Trade &s, string varName, char *buffer, int bufferLength)                                 { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Trade_Amendment &s, string varName, char *buffer, int bufferLength)                       { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Trade_Statistics &s, string varName, char *buffer, int bufferLength)                      { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Series_Statistics &s, string varName, char *buffer, int bufferLength)                     { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Calculated_Opening_Price &s, string varName, char *buffer, int bufferLength)              { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Estimated_Average_Settlement_Price &s, string varName, char *buffer, int bufferLength)    { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Market_Alert &s, string varName, char *buffer, int bufferLength)                          { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Open_Interest &s, string varName, char *buffer, int bufferLength)                         { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
  string printStruct(OMDD_Implied_Volatility &s, string varName, char *buffer, int bufferLength)                    { JSONNode n = genNode(s, varName, buffer, bufferLength); JSONNode on(JSON_NODE); n.set_name(varName); on.push_back(n); return on.write();}
