#include "DataProcFunctions.h"

//--------------------------------------------------
// Factory method
//--------------------------------------------------
DataProcFunctions* DataProcFuncFactory::GetDataProcFunc(SystemConfig::Identity id)
{
  if (id == SystemConfig::OMDC) return new DataProcFunctions_OMDC();
  else if (id == SystemConfig::OMDD) return new DataProcFunctions_OMDD();
}


//--------------------------------------------------
// omd_mdi
//--------------------------------------------------
void DataProcFunctions::ProcessMessageForMDI(boost::shared_ptr<SharedObjects> shrobj, BYTE* pbMsg, unsigned short usMsgType)
{
  switch (usMsgType)
  {
    // case OMDC_SECURITY_DEFINITION:
    //   {
    //     OMDC_Security_Definition* osd = (OMDC_Security_Definition *) pbMsg;
    //     shrobj->AddSpreadTable(osd->SecurityCode, lexical_cast<unsigned short>(string(osd->SpreadTableCode)));
    //     break;
    //   }
    case OMDC_TRADE:
      {
        OMDC_Trade * ost = (OMDC_Trade *) pbMsg;
        if (shrobj->omd_mdi_check_if_subscribed(ost->SecurityCode))
        {
          shrobj->notify_trade(ost->TradeID,ost->SecurityCode,(double)(ost->Price)/1000,(double)ost->Quantity);
        }
        break;
      }
      // case OMDC_TRADE_CANCEL:
    case OMDC_TRADE_TICKER:
      {
        OMDC_Trade_Ticker * ost = (OMDC_Trade_Ticker *) pbMsg;
        if (shrobj->omd_mdi_check_if_subscribed(ost->SecurityCode))
        {
          shrobj->notify_trade(ost->TickerID,ost->SecurityCode,(double)(ost->Price)/1000,(double)ost->AggregateQuantity);
        }
        break;
      }
    case OMDD_TRADE:
      {
        OMDD_Trade * ost = (OMDD_Trade *) pbMsg;
        if (shrobj->omd_mdi_check_if_subscribed(ost->OrderbookID))
        {
          shrobj->notify_trade(ost->TradeID,ost->OrderbookID,(double)(ost->Price)/1000,(double)ost->Quantity);
        }
        break;
      }
      // case OMDD_TRADE_AMENDMENT:
      // case OMDD_TRADE_STATISTICS:

    default:  break;
  }
  return;
}


//--------------------------------------------------
// OMD-C
//--------------------------------------------------
void DataProcFunctions_OMDC::OutputJsonToLog(const char * sCaller, const unsigned short usChnlID, boost::shared_ptr<Logger> logger, const BYTE *pbMsg, char caJsonBuffer[])
{
  unsigned short usMsgType = 0;
  usMsgType = READ_UINT16(pbMsg+2);
  OutputJsonToLog(sCaller, usChnlID, logger, pbMsg, usMsgType, caJsonBuffer, 0);
  return;
}

void DataProcFunctions_OMDC::ProcessOrderBookInstruction(const char *sCaller, boost::shared_ptr<Logger> logger, const BYTE* pbMsg, boost::shared_ptr<SharedObjects> shrobj, bool bPrintOrderBookAsInfo)
{
  // Special handling for Order Book
  OMDC_Aggregate_Order_Book_Update* aobu = (OMDC_Aggregate_Order_Book_Update*) pbMsg;
  OrderBook* ob = shrobj->GetOrderBookCache()->GetOrderBook(aobu->SecurityCode);

  for (unsigned int i = 0; i < aobu->NoEntries; ++i)
  {
    OMDC_Aggregate_Order_Book_Update_Details* aobud =
      (OMDC_Aggregate_Order_Book_Update_Details*)
      ((BYTE*)pbMsg+sizeof(OMDC_Aggregate_Order_Book_Update)+i*sizeof(OMDC_Aggregate_Order_Book_Update_Details));

    OrderBook::BASide side = (aobud->Side == 0 ? OrderBook::BID : OrderBook::ASK);

    switch (aobud->UpdateAction)
    {
      case 0: //New
        ob->Add(side,aobud->Price,aobud->PriceLevel,aobud->NumberOfOrders,aobud->AggregateQuantity);
        break;
      case 1: //Change
        ob->Change(side,aobud->Price,aobud->PriceLevel,aobud->NumberOfOrders,aobud->AggregateQuantity);
        break;
      case 2: //Delete
        ob->Delete(side,aobud->Price,aobud->PriceLevel,aobud->NumberOfOrders,aobud->AggregateQuantity);
        break;
      case 74: //Order book clear
        ob->Reset();
        break;
      default:
        break;
    }
  }

  //--------------------------------------------------
  // Output OrderBook to Log
  //--------------------------------------------------
  if (bPrintOrderBookAsInfo)
  {
    {
      stringstream ss;

      ss << "DataProcFunctions_OMDC: " << string(sCaller) << ", Security Code: " << aobu->SecurityCode << ", ";
      ss << ob->DumpJson(aobu->SecurityCode,OrderBook::BID) << endl;

      if (bPrintOrderBookAsInfo)
        logger->Write(Logger::INFO,ss.str().c_str());
      else
        logger->Write(Logger::DEBUG,ss.str().c_str());
    }

    {
      stringstream ss;

      ss << "DataProcFunctions_OMDC: " << string(sCaller) << ", Security Code: " << aobu->SecurityCode << ", ";
      ss << ob->DumpJson(aobu->SecurityCode,OrderBook::ASK) << endl;

      if (bPrintOrderBookAsInfo)
        logger->Write(Logger::INFO,ss.str().c_str());
      else
        logger->Write(Logger::DEBUG,ss.str().c_str());
    }
  }

  //--------------------------------------------------
  // omd_mdi Callback
  //--------------------------------------------------
  if (shrobj->omd_mdi_check_if_subscribed(aobu->SecurityCode))
  {
    ATU_MDI_marketfeed_struct mfs;
    ob->Dump(mfs);
    shrobj->notify_orderbook(aobu->SecurityCode,mfs);
  }

  return;
}


//--------------------------------------------------
// FIXME
//--------------------------------------------------
void DataProcFunctions_OMDC::OutputJsonToLog(const char * sCaller, const unsigned short usChnlID, boost::shared_ptr<Logger> logger, const BYTE *pbMsg, unsigned short usMsgType, char caJsonBuffer[], unsigned long ulHKExSendTime)
{
}
// void DataProcFunctions_OMDC::OutputJsonToLog(const char * sCaller, const unsigned short usChnlID, boost::shared_ptr<Logger> logger, const BYTE *pbMsg, unsigned short usMsgType, char caJsonBuffer[], unsigned long ulHKExSendTime)
// {
//   switch (usMsgType)
//   {
//     //--------------------------------------------------
//     // OMD
//     //--------------------------------------------------
//     case OMD_LOGON                               :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Logon                              *) pbMsg), string("Logon"                              ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_LOGON_RESPONSE                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Logon_Response                     *) pbMsg), string("Logon_Response"                     ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_REFRESH_COMPLETE                    :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Refresh_Complete                   *) pbMsg), string("Refresh_Complete"                   ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_RETRANSMISSION_REQUEST              :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Retransmission_Request             *) pbMsg), string("Retransmission_Request"             ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_RETRANSMISSION_RESPONSE             :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Retransmission_Response            *) pbMsg), string("Retransmission_Response"            ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_SEQUENCE_RESET                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Sequence_Reset                     *) pbMsg), string("Sequence_Reset"                     ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//
//                                                     //--------------------------------------------------
//                                                     // OMD-C
//                                                     //--------------------------------------------------
//     case OMDC_MARKET_DEFINITION                  :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Market_Definition                 *) pbMsg), string("Market_Definition"                  ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_SECURITY_DEFINITION                :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Security_Definition               *) pbMsg), string("Security_Definition"                ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_LIQUIDITY_PROVIDER                 :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Liquidity_Provider                *) pbMsg), string("Liquidity_Provider"                 ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_CURRENCY_RATE                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Currency_Rate                     *) pbMsg), string("Currency_Rate"                      ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_TRADING_SESSION_STATUS             :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Trading_Session_Status            *) pbMsg), string("Trading_Session_Status"             ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_SECURITY_STATUS                    :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Security_Status                   *) pbMsg), string("Security_Status"                    ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_NEWS                               :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_News                              *) pbMsg), string("News"                               ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_ADD_ORDER                          :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Add_Order                         *) pbMsg), string("Add_Order"                          ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_MODIFY_ORDER                       :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Modify_Order                      *) pbMsg), string("Modify_Order"                       ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_DELETE_ORDER                       :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Delete_Order                      *) pbMsg), string("Delete_Order"                       ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_ADD_ODD_LOT_ORDER                  :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Add_Odd_Lot_Order                 *) pbMsg), string("Add_Odd_Lot_Order"                  ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_DELETE_ODD_LOT_ORDER               :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Delete_Odd_Lot_Order              *) pbMsg), string("Delete_Odd_Lot_Order"               ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_NOMINAL_PRICE                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Nominal_Price                     *) pbMsg), string("Nominal_Price"                      ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_INDICATIVE_EQUILIBRIUM_PRICE       :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Indicative_Equilibrium_Price      *) pbMsg), string("Indicative_Equilibrium_Price"       ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_YIELD                              :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Yield                             *) pbMsg), string("Yield"                              ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_TRADE                              :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Trade                             *) pbMsg), string("Trade"                              ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_TRADE_CANCEL                       :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Trade_Cancel                      *) pbMsg), string("Trade_Cancel"                       ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_TRADE_TICKER                       :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Trade_Ticker                      *) pbMsg), string("Trade_Ticker"                       ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_AGGREGATE_ORDER_BOOK_UPDATE        :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Aggregate_Order_Book_Update       *) pbMsg), string("Aggregate_Order_Book_Update"        ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_BROKER_QUEUE                       :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Broker_Queue                      *) pbMsg), string("Broker_Queue"                       ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_STATISTICS                         :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Statistics                        *) pbMsg), string("Statistics"                         ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_MARKET_TURNOVER                    :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Market_Turnover                   *) pbMsg), string("Market_Turnover"                    ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_CLOSING_PRICE                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Closing_Price                     *) pbMsg), string("Closing_Price"                      ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_INDEX_DEFINITION                   :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Index_Definition                  *) pbMsg), string("Index_Definition"                   ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDC_INDEX_DATA                         :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDC: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDC_Index_Data                        *) pbMsg), string("Index_Data"                         ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//
//     default                                      :  break;
//   }
//
//   return;
// }


//--------------------------------------------------
// OMD-D
//--------------------------------------------------
void DataProcFunctions_OMDD::OutputJsonToLog(const char * sCaller, const unsigned short usChnlID, boost::shared_ptr<Logger> logger, const BYTE *pbMsg, char caJsonBuffer[])
{
  unsigned short usMsgType = 0;
  usMsgType = READ_UINT16(pbMsg+2);
  OutputJsonToLog(sCaller, usChnlID, logger, pbMsg, usMsgType, caJsonBuffer, 0);
  return;
}


void DataProcFunctions_OMDD::ProcessOrderBookInstruction(const char *sCaller, boost::shared_ptr<Logger> logger, const BYTE* pbMsg, boost::shared_ptr<SharedObjects> shrobj, bool bPrintOrderBookAsInfo)
{
  // Special handling for Order Book
  OMDD_Aggregate_Order_Book_Update* aobu = (OMDD_Aggregate_Order_Book_Update*) pbMsg;
  OrderBook* ob = shrobj->GetOrderBookCache()->GetOrderBook(aobu->OrderbookID);

  for (unsigned int i = 0; i < aobu->NoEntries; ++i)
  {
    OMDD_Aggregate_Order_Book_Update_Details* aobud =
      (OMDD_Aggregate_Order_Book_Update_Details*)
      ((BYTE*)pbMsg+sizeof(OMDD_Aggregate_Order_Book_Update)+i*sizeof(OMDD_Aggregate_Order_Book_Update_Details));

    OrderBook::BASide side = (aobud->Side == 0 ? OrderBook::BID : OrderBook::ASK);

    switch (aobud->UpdateAction)
    {
      case 0: //New
        ob->Add(side,aobud->Price,aobud->PriceLevel,aobud->NumberOfOrders,aobud->AggregateQuantity);
        break;
      case 1: //Change
        ob->Change(side,aobud->Price,aobud->PriceLevel,aobud->NumberOfOrders,aobud->AggregateQuantity);
        break;
      case 2: //Delete
        ob->Delete(side,aobud->Price,aobud->PriceLevel,aobud->NumberOfOrders,aobud->AggregateQuantity);
        break;
      case 74: //Order book clear
        ob->Reset();
        break;
      default:
        break;
    }
  }

  //--------------------------------------------------
  // Output OrderBook to Log
  //--------------------------------------------------
  if (bPrintOrderBookAsInfo)
  {
    {
      stringstream ss;

      ss << "DataProcFunctions_OMDD: " << string(sCaller) << ", OrderBookID: " << aobu->OrderbookID<< ", ";
      ss << ob->DumpJson(aobu->OrderbookID,OrderBook::BID) << endl;

      if (bPrintOrderBookAsInfo)
        logger->Write(Logger::INFO,ss.str().c_str());
      else
        logger->Write(Logger::DEBUG,ss.str().c_str());
    }

    {
      stringstream ss;

      ss << "DataProcFunctions_OMDD: " << string(sCaller) << ", OrderbookID: " << aobu->OrderbookID<< ", ";
      ss << ob->DumpJson(aobu->OrderbookID,OrderBook::ASK) << endl;

      if (bPrintOrderBookAsInfo)
        logger->Write(Logger::INFO,ss.str().c_str());
      else
        logger->Write(Logger::DEBUG,ss.str().c_str());
    }
  }

  //--------------------------------------------------
  // omd_mdi Callback
  //--------------------------------------------------
  if (shrobj->omd_mdi_check_if_subscribed(aobu->OrderbookID))
  {
    ATU_MDI_marketfeed_struct mfs;
    ob->Dump(mfs);
    shrobj->notify_orderbook(aobu->OrderbookID,mfs);
  }

  return;
}

//--------------------------------------------------
// FIXME
//--------------------------------------------------
void DataProcFunctions_OMDD::OutputJsonToLog(const char * sCaller, const unsigned short usChnlID, boost::shared_ptr<Logger> logger, const BYTE *pbMsg, unsigned short usMsgType, char caJsonBuffer[], unsigned long ulHKExSendTime)
{
}
// void DataProcFunctions_OMDD::OutputJsonToLog(const char * sCaller, const unsigned short usChnlID, boost::shared_ptr<Logger> logger, const BYTE *pbMsg, unsigned short usMsgType, char caJsonBuffer[], unsigned long ulHKExSendTime)
// {
//   switch (usMsgType)
//   {
//
//     //--------------------------------------------------
//     // OMD
//     //--------------------------------------------------
//     case OMD_LOGON                               :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Logon                              *) pbMsg), string("Logon"                              ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_LOGON_RESPONSE                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Logon_Response                     *) pbMsg), string("Logon_Response"                     ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_REFRESH_COMPLETE                    :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Refresh_Complete                   *) pbMsg), string("Refresh_Complete"                   ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_RETRANSMISSION_REQUEST              :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Retransmission_Request             *) pbMsg), string("Retransmission_Request"             ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_RETRANSMISSION_RESPONSE             :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Retransmission_Response            *) pbMsg), string("Retransmission_Response"            ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMD_SEQUENCE_RESET                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMD_Sequence_Reset                     *) pbMsg), string("Sequence_Reset"                     ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//
//                                                     //--------------------------------------------------
//                                                     // OMD-D
//                                                     //--------------------------------------------------
//     case OMDD_COMMODITY_DEFINITION               :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Commodity_Definition              *) pbMsg), string("Commodity_Definition"               ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_CLASS_DEFINITION                   :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Class_Definition                  *) pbMsg), string("Class_Definition"                   ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_SERIES_DEFINITION_BASE             :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Series_Definition_Base            *) pbMsg), string("Series_Definition_Base"             ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_SERIES_DEFINITION_EXTENDED         :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Series_Definition_Extended        *) pbMsg), string("Series_Definition_Extended"         ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_COMBINATION_DEFINITION             :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Combination_Definition            *) pbMsg), string("Combination_Definition"             ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_MARKET_STATUS                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Market_Status                     *) pbMsg), string("Market_Status"                      ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_SERIES_STATUS                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Series_Status                     *) pbMsg), string("Series_Status"                      ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_COMMODITY_STATUS                   :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Commodity_Status                  *) pbMsg), string("Commodity_Status"                   ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_ADD_ORDER                          :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Add_Order                         *) pbMsg), string("Add_Order"                          ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_MODIFY_ORDER                       :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Modify_Order                      *) pbMsg), string("Modify_Order"                       ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_DELETE_ORDER                       :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Delete_Order                      *) pbMsg), string("Delete_Order"                       ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_AGGREGATE_ORDER_BOOK_UPDATE        :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Aggregate_Order_Book_Update       *) pbMsg), string("Aggregate_Order_Book_Update"        ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_ORDER_BOOK_CLEAR                   :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Order_Book_Clear                  *) pbMsg), string("Order_Book_Clear"                   ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_QUOTE_REQUEST                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Quote_Request                     *) pbMsg), string("Quote_Request"                      ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_TRADE                              :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Trade                             *) pbMsg), string("Trade"                              ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_TRADE_AMENDMENT                    :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Trade_Amendment                   *) pbMsg), string("Trade_Amendment"                    ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_TRADE_STATISTICS                   :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Trade_Statistics                  *) pbMsg), string("Trade_Statistics"                   ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_SERIES_STATISTICS                  :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Series_Statistics                 *) pbMsg), string("Series_Statistics"                  ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_CALCULATED_OPENING_PRICE           :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Calculated_Opening_Price          *) pbMsg), string("Calculated_Opening_Price"           ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_ESTIMATED_AVERAGE_SETTLEMENT_PRICE :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Estimated_Average_Settlement_Price*) pbMsg), string("Estimated_Average_Settlement_Price" ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_MARKET_ALERT                       :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Market_Alert                      *) pbMsg), string("Market_Alert"                       ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_OPEN_INTEREST                      :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Open_Interest                     *) pbMsg), string("Open_Interest"                      ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//     case OMDD_IMPLIED_VOLATILITY                 :  logger->Write(Logger::INFO, "HKEx Send Time: %s, DataProcFunctions_OMDD: %s, ChannelID:%u. %s", (ulHKExSendTime == 0 ? "Nil" : Util::fromUnixTimeToString(ulHKExSendTime, Util::NANOSEC, Util::GMT, Util::HKT, true).c_str()), sCaller, usChnlID, printStruct( *((OMDD_Implied_Volatility                *) pbMsg), string("Implied_Volatility"                 ) , caJsonBuffer , JSON_BUFFER_SIZE  ).c_str() );   break;
//
//     default                                      :  break;
//   }
//
//   return;
// }

