#include "DataProcFunctions.h"

//--------------------------------------------------
// Factory method
//--------------------------------------------------
DataProcFunctions* DataProcFuncFactory::GetDataProcFunc(SystemConfig::Identity id)
{
  if (id == SystemConfig::OMDC) return new DataProcFunctions_OMDC();
  else if (id == SystemConfig::OMDD) return new DataProcFunctions_OMDD();
}

DataProcFunctions::DataProcFunctions()
{
  m_SysCfg    = SystemConfig::Instance();
  m_DataTrans = DataTransmission::Instance();
}

//--------------------------------------------------
// omd_mdi
//--------------------------------------------------
void DataProcFunctions::ProcessMessageForTransmission(BYTE* pbMsg, unsigned short usMsgType)
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
        if (m_DataTrans->CheckIfSubscribed(ost->SecurityCode))
        {
          m_DataTrans->NotifyTrade(ost->TradeID,ost->SecurityCode,(double)(ost->Price)/1000,(double)ost->Quantity);
        }
        break;
      }
      // case OMDC_TRADE_CANCEL:
    case OMDC_TRADE_TICKER:
      {
        OMDC_Trade_Ticker * ost = (OMDC_Trade_Ticker *) pbMsg;
        if (m_DataTrans->CheckIfSubscribed(ost->SecurityCode))
        {
          m_DataTrans->NotifyTrade(ost->TickerID,ost->SecurityCode,(double)(ost->Price)/1000,(double)ost->AggregateQuantity);
        }
        break;
      }
    case OMDD_TRADE:
      {
        OMDD_Trade * ost = (OMDD_Trade *) pbMsg;
        if (m_DataTrans->CheckIfSubscribed(ost->OrderbookID))
        {
          m_DataTrans->NotifyTrade(ost->TradeID,ost->OrderbookID,(double)(ost->Price)/1000,(double)ost->Quantity);
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
  if (m_DataTrans->CheckIfSubscribed(aobu->SecurityCode))
  {
    ATU_MDI_marketfeed_struct mfs;
    ob->Dump(mfs);
    m_DataTrans->NotifyOrderBookUpdate(aobu->SecurityCode,mfs);
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
  if (m_DataTrans->CheckIfSubscribed(aobu->OrderbookID))
  {
    ATU_MDI_marketfeed_struct mfs;
    ob->Dump(mfs);
    m_DataTrans->NotifyOrderBookUpdate(aobu->OrderbookID,mfs);
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


//--------------------------------------------------
// FIXME copied from Kenny
//--------------------------------------------------
map<int,string> instmap;
map<string,OrderBookKenny> obmap;


void OnHeartBeat()
{
  return;
}

  void
OnRefreshBook(mktdata::MessageHeader& hdr, char *buf, int len)
{
  MDIncrementalRefreshBook32 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noMDEntries();

  while (ent.hasNext()) {
    ent.next();

    const int secid = ent.securityID();

    string instid = instmap[secid];

    auto& ob = obmap[instid];
    snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

    if (ent.mDEntryType() == MDEntryTypeBook::Bid || ent.mDEntryType() == MDEntryTypeBook::Offer) {
      struct level *level = ent.mDEntryType() == MDEntryTypeBook::Bid ? ob.bid : ob.ask;
      const int nlevel = ent.mDPriceLevel() - 1;

      switch (ent.mDUpdateAction()) {
        case MDUpdateAction::New:
          for (int i = 9; i > nlevel; --i)
            // for (int i = 4; i > nlevel; --i)
            level[i] = level[i - 1];
          // fall through
        case MDUpdateAction::Change:
          // if (ent.mDUpdateAction() == MDUpdateAction::Change) {
          // 	printf("Change order: level-%d price-%ld size-%d\n", nlevel,
          // 			 ent.mDEntryPx().mantissa(), ent.mDEntrySize());
          // }
          // if (ent.mDUpdateAction() == MDUpdateAction::Change &&
          // 		level[nlevel].price != ent.mDEntryPx().mantissa()) {
          // 	printf("Warning: Change level-%d prices[%ld-%ld] not matched\n",
          // 				 nlevel + 1, level[nlevel].price, ent.mDEntryPx().mantissa());
          // }
          level[nlevel].price = ent.mDEntryPx().mantissa();
          level[nlevel].size = ent.mDEntrySize();
          level[nlevel].norder = ent.numberOfOrders();

          ob.secid = secid;
          ob.seqno = ent.rptSeq();
          ob.transact_time = msg.transactTime();

          // if (ob.bid[0].price < ob.ask[0].price)
          //   OnRefreshBook(&ob);

          break;
        case MDUpdateAction::Delete: {
                                       // printf("Delete order: level-%d price-%ld size-%d\n", nlevel,
                                       // 			 ent.mDEntryPx().mantissa(), ent.mDEntrySize());
                                       for (int i = nlevel; i < 9; ++i)
                                         // for (int i = nlevel; i < 4; ++i)
                                         level[i] = level[i + 1];

                                       break;
                                     }
        case MDUpdateAction::DeleteThru:
                                     // printf("Delete thru: delete all book on one side\n");
                                     for (int i = 0; i < 10; i++)
                                       level[i].price = level[i].size = level[i].norder = 0;
                                     // OnRefreshBook(&ob);
                                     break;
        case MDUpdateAction::DeleteFrom:
                                     // printf("Delete from: delete top %d levels of book on one side\n",
                                     // 			 nlevel + 1);
                                     for (int i = 0; i < nlevel + 1; i++)
                                       level[i].price = level[i].size = level[i].norder = 0;
                                     // OnRefreshBook(&ob);
                                     break;

        default:
                                     printf(",unknown");
                                     break;
      }
    } else if (ent.mDEntryType() == MDEntryTypeBook::ImpliedBid || ent.mDEntryType() == MDEntryTypeBook::ImpliedOffer) {
      ob.secid = secid;
      ob.seqno = ent.rptSeq();
      ob.transact_time = msg.transactTime();

      // OnRefreshBook(&ob);
    }
  }
}

  void
OnInstrumentDefinitionOption(mktdata::MessageHeader& hdr, char *buf, int len)
{
  //   MDInstrumentDefinitionOption41 msg;
  //
  //   msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);
  //
  //   auto& ent = msg.noMDEntries();
  //
  //   while (ent.hasNext()) {
  //   ent.next();
  //
  //   const int secid = ent.securityID();
  //
  //   int rpt_seq = ent.rptSeq();
  //   int md_entry_size = ent.mDEntrySize();
  //   int md_update_action = ent.mDUpdateAction();
  //   string md_entry_type = ent.mDEntryType();
  //
  //   cout << "RefreshVolume: SecurityID:" << secid
  //     << " RptSeq:" << rpt_seq
  //     << " EntrySize:" << md_entry_size
  //     << " UpdateAction:" << md_update_action
  //     << " EntryType:" << md_entry_type << endl;
  //   //if (instmap.find(secid) == instmap.end())
  //   //	continue;
  //   string instid = instmap[secid];
  //   // if (subscribe.find(instid) == subscribe.end())
  //   //	continue;
  //
  //   auto& ob = obmap[instid];
  //   snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());
  //
  //   ob.secid = secid;
  //   ob.seqno = ent.rptSeq();
  //   ob.transact_time = msg.transactTime();
  //
  //   OnRefreshBook(&ob);
  // }
}

  void
OnRefreshVolume(mktdata::MessageHeader& hdr, char *buf, int len)
{
  MDIncrementalRefreshVolume37 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noMDEntries();

  while (ent.hasNext()) {
    ent.next();

    const int secid = ent.securityID();

    int rpt_seq = ent.rptSeq();
    int md_entry_size = ent.mDEntrySize();
    int md_update_action = ent.mDUpdateAction();
    string md_entry_type = ent.mDEntryType();

    cout << "RefreshVolume: SecurityID:" << secid
      << " RptSeq:" << rpt_seq
      << " EntrySize:" << md_entry_size
      << " UpdateAction:" << md_update_action
      << " EntryType:" << md_entry_type << endl;
    string instid = instmap[secid];

    auto& ob = obmap[instid];
    snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

    ob.secid = secid;
    ob.seqno = ent.rptSeq();
    ob.transact_time = msg.transactTime();

    // OnRefreshBook(&ob);
  }
}


  void
OnRefreshTrade(mktdata::MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
  return;
#endif
  MDIncrementalRefreshTrade36 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noMDEntries();

  while (ent.hasNext()) {
    ent.next();

    const int secid = ent.securityID();

    int rpt_seq = ent.rptSeq();
    int64_t md_entry_px = ent.mDEntryPx().mantissa();
    int md_entry_size = ent.mDEntrySize();
    int no_orders = ent.numberOfOrders();
    int aggressor_side = ent.aggressorSide();
    int trade_id = ent.tradeID();
    string md_entry_type = ent.mDEntryType();
    int md_update_action = ent.mDUpdateAction();
    // int md_trade_entry_id = ent.mDTradeEntryID();
    cout << "TradeSummary: SecurityID:" << secid
      << " RptSeq:" << rpt_seq
      << " Price:" << md_entry_px
      << " EntrySize:" << md_entry_size
      << " OrderNo:" << no_orders
      << " AggressorSide:" << aggressor_side
      << " TradeID:" << trade_id
      << " UpdateAction:" << md_update_action
      << " EntryType:" << md_entry_type << endl;
  }
}

  void
OnRefreshTradeSummary(mktdata::MessageHeader& hdr, char *buf, int len)
{
  MDIncrementalRefreshTradeSummary42 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noMDEntries();
  auto& order_id_ent = msg.noOrderIDEntries();

  while(order_id_ent.hasNext()) {
    order_id_ent.next();
    int order_id = order_id_ent.orderID();
    int last_qty = order_id_ent.lastQty();
    cout << "NoOrderIDEntries: OrderID:" << order_id 
      << "	Qty:" << last_qty << endl;
  }

  while (ent.hasNext()) {
    ent.next();

    const int secid = ent.securityID();

    int rpt_seq = ent.rptSeq();
    int64_t md_entry_px = ent.mDEntryPx().mantissa();
    int md_entry_size = ent.mDEntrySize();
    int no_orders = ent.numberOfOrders();
    int aggressor_side = ent.aggressorSide();
    string md_entry_type = ent.mDEntryType();
    // int md_trade_entry_id = ent.mDTradeEntryID();
    cout << "TradeSummary: SecurityID:" << secid
      << " RptSeq:" << rpt_seq
      << " Price:" << md_entry_px
      << " EntrySize:" << md_entry_size
      << " OrderNo:" << no_orders
      << " AggressorSide:" << aggressor_side
      << " EntryType:" << md_entry_type << endl;
    // << " TradeEntryID:" << md_trade_entry_id << endl;
    string instid = instmap[secid];

    auto& ob = obmap[instid];
    snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

    ob.secid = secid;
    ob.seqno = ent.rptSeq();
    ob.transact_time = msg.transactTime();

    // OnRefreshBook(&ob);
  }
}

  void
OnRefreshDailyStatistics(mktdata::MessageHeader& hdr, char *buf, int len)
{
  MDIncrementalRefreshDailyStatistics33 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noMDEntries();

  while (ent.hasNext()) {
    ent.next();

    const int secid = ent.securityID();

    string instid = instmap[secid];

    auto& ob = obmap[instid];
    snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

    ob.secid = secid;
    ob.seqno = ent.rptSeq();
    ob.transact_time = msg.transactTime();

    int64_t md_entry_px = ent.mDEntryPx().mantissa();
    int rpt_seq = ent.rptSeq();
    auto md_entry_type = ent.mDEntryType();
    // int md_entry_size = ent.mDEntrySize();
    auto trading_date = ent.tradingReferenceDate();
    auto settl_price_type = ent.settlPriceType();

    switch(md_entry_type) {
      case MDEntryTypeDailyStatistics::ClearedVolume: {
                                                        printf("Cleared Volume:\n");
                                                      }
                                                      break;
      case MDEntryTypeDailyStatistics::SettlementPrice: {
                                                          printf("Settlement Price:\n");
                                                          if (settl_price_type.finalfinal()) 
                                                            cout << "finalfinal " << endl;
                                                          if (settl_price_type.actual()) 
                                                            cout << "actual " << endl;
                                                          if (settl_price_type.rounded()) 
                                                            cout << "rounded " << endl;
                                                          if (settl_price_type.intraday()) 
                                                            cout << "intraday " << endl;
                                                          if (settl_price_type.reservedBits()) 
                                                            cout << "reservedBits" << endl;
                                                          if (settl_price_type.nullValue()) 
                                                            cout << "nullValue" << endl;
                                                        }
                                                        break;
      case MDEntryTypeDailyStatistics::OpenInterest: {
                                                       printf("Open Interest:\n");
                                                     }
                                                     break;
      case MDEntryTypeDailyStatistics::FixingPrice: {
                                                      printf("Fixing Price:\n");
                                                    }
                                                    break;
      case MDEntryTypeStatistics::OpenPrice: {
                                               printf("Open Price:\n");
                                             }
                                             break;
      case MDEntryTypeStatistics::HighTrade: {
                                               printf("High Trade:\n");
                                             }
                                             break;
      case MDEntryTypeStatistics::LowTrade: {
                                              printf("Low Trade:\n");
                                            }
                                            break;
      case MDEntryTypeStatistics::HighestBid: {
                                                printf("Highest Bid:\n");
                                              }
                                              break;
      case MDEntryTypeStatistics::LowestOffer: {
                                                 printf("Lowest Offer:\n");
                                               }
                                               break;
      default:
                                               break;
    }
    cout << "Session Statistics: SecurityID:" << secid
      << " RptSeq:" << rpt_seq
      << " EntryPx:" << md_entry_px
      // << " EntrySize:" << md_entry_size
      << " EntryType:" << md_entry_type
      << " TradingDate:" << trading_date << endl;

    // auto md_entry_type = ent.mDEntryType();

    // OnRefreshBook(&ob);
  }
}

  void
OnRefreshSessionStatistics(mktdata::MessageHeader& hdr, char *buf, int len)
{
  MDIncrementalRefreshSessionStatistics35 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noMDEntries();

  while (ent.hasNext()) {
    ent.next();

    const int secid = ent.securityID();

    string instid = instmap[secid];

    int64_t md_entry_px = ent.mDEntryPx().mantissa();
    int rpt_seq = ent.rptSeq();
    int open_close_settl_flag = ent.openCloseSettlFlag();
    int md_update_action = ent.mDUpdateAction();
    auto md_entry_type = ent.mDEntryType();
    cout << "Session Statistics: SecurityID:" << secid
      << " RptSeq:" << rpt_seq
      << " EntryPx:" << md_entry_px
      << " EntryType:" << md_entry_type
      << " OpenCloseSettlFlag:" << open_close_settl_flag << endl;


    auto& ob = obmap[instid];
    snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

    ob.secid = secid;
    ob.seqno = ent.rptSeq();
    ob.transact_time = msg.transactTime();

    // OnRefreshBook(&ob);
  }
}

  void
OnRefreshLimitsBanding(mktdata::MessageHeader& hdr, char *buf, int len)
{
  return;

  MDIncrementalRefreshLimitsBanding34 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noMDEntries();

  while (ent.hasNext()) {
    ent.next();

    const int secid = ent.securityID();

    string instid = instmap[secid];

    auto& ob = obmap[instid];
    snprintf(ob.instid, sizeof(ob.instid), "%s", instid.c_str());

    ob.secid = secid;
    ob.seqno = ent.rptSeq();
    ob.transact_time = msg.transactTime();

    int64_t high_limit = ent.highLimitPrice().mantissa();
    int64_t low_limit = ent.lowLimitPrice().mantissa();
    int64_t max_price_variation = ent.maxPriceVariation().mantissa();
    int rpt_seq = ent.rptSeq();
    int md_update_action = ent.mDUpdateAction();
    string md_entry_type = ent.mDEntryType();
    cout << "Limits Banding: SecurityID:" << secid
      << " RptSeq:" << rpt_seq
      << " HighLimit:" << high_limit
      << " LowLimit:" << low_limit
      << " MaxPriceVariation:" << max_price_variation
      << " UpdateAction:" << md_update_action
      << " EntryType:" << md_entry_type << endl;

    // OnRefreshBook(&ob);
  }

}

  void
OnRefreshSecurityDefinitionFuture(mktdata::MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
  return;
#endif
  MDInstrumentDefinitionFuture27 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noEvents();
  auto security_id = msg.securityID();
  int tot_num_reports = msg.totNumReports();
  while(ent.hasNext()) {
    ent.next();
    const auto event_time = ent.eventTime();
    auto event_type = ent.eventType();
    string security_group = msg.getSecurityGroupAsString();
    auto market_segment_id = msg.marketSegmentID();
    auto symbol = msg.symbol();
    cout << "Instrument Definition : " \
      << " event time:" << TransferTimestamp(event_time) \
      << " event type:" << event_type \
      << " security ID:" << security_id \
      << " symbol:" << symbol \
      << " security group:" << security_group 
      << " total number reports:" << tot_num_reports 
      << " market segment ID:" << market_segment_id << endl;
  }

  auto& feed_types = msg.noMDFeedTypes();
  while(feed_types.hasNext()) {
    feed_types.next();
    int market_depth = feed_types.marketDepth();
    cout << "NoMDFeedTypes : market depth - " << market_depth << endl;
  }
}

  void
OnRefreshSecurityDefinitionSpread(mktdata::MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
  return;
#endif
  MDInstrumentDefinitionSpread29 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto security_id = msg.securityID();
  auto& ent = msg.noEvents();
  while(ent.hasNext()) {
    ent.next();
    const auto event_time = ent.eventTime();
    auto event_type = ent.eventType();
    string security_group = msg.getSecurityGroupAsString();
    auto market_segment_id = msg.marketSegmentID();
    auto symbol = msg.symbol();

    cout << "Instrument Definition : "
      << " event time:" << TransferTimestamp(event_time)
      << " event type:" << event_type
      << " security ID:" << security_id
      << " symbol:" << symbol
      << " security group:" << security_group
      << " market segment ID:" << market_segment_id << endl;
  }
}

  void
OnQuoteRequest(mktdata::MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
  return;
#endif
  QuoteRequest39 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& sym = msg.noRelatedSym();
  while(sym.hasNext()) {
    sym.next();
    string symbol = sym.symbol();
    int security_id = sym.securityID();
    int order_qty = sym.orderQty();
    cout << "Quote Request : " \
      << " symbol:" << symbol << " securityID:" << security_id \
      << " OrderQty:" << order_qty << endl;
  }
}

  void
OnSecurityStatus(mktdata::MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
  return;
#endif
  SecurityStatus30 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto security_status = msg.securityTradingStatus();

  switch(security_status) {
    case SecurityTradingStatus::TradingHalt: {
                                               cout << "Status: Trading Halt:" << security_status << endl;
                                             }
                                             break;
    case SecurityTradingStatus::Close: {
                                         cout << "Status: Close:" << security_status << endl;
                                       }
                                       break;
    case SecurityTradingStatus::NewPriceIndication: {
                                                      cout << "Status: New Price Indication:" << security_status << endl;
                                                    }
                                                    break;
    case SecurityTradingStatus::ReadyToTrade: {
                                                cout << "Status: Ready To Trade:" << security_status << endl;
                                              }
                                              break;
    case SecurityTradingStatus::NotAvailableForTrading: {
                                                          cout << "Status: Not Available For Trading:" << security_status << endl;
                                                        }
                                                        break;
    case SecurityTradingStatus::UnknownorInvalid: {
                                                    cout << "Status: Unknown or Invalid:" << security_status << endl;
                                                  }
                                                  break;
    case SecurityTradingStatus::PreOpen: {
                                           cout << "Status: Pre-Open:" << security_status << endl;
                                         }
                                         break;
    case SecurityTradingStatus::PreCross: {
                                            cout << "Status: Pre-Cross:" << security_status << endl;
                                          }
                                          break;
    case SecurityTradingStatus::Cross: {
                                         cout << "Status: Cross:" << security_status << endl;
                                       }
                                       break;
    case SecurityTradingStatus::PostClose: {
                                             cout << "Status: Post-Close:" << security_status << endl;
                                           }
                                           break;
    case SecurityTradingStatus::NoChange: {
                                            cout << "Status: No Change:" << security_status << endl;
                                          }
    case SecurityTradingStatus::NULL_VALUE: {
                                              cout << "Status: NULL VALUE:" << security_status << endl;
                                            }
                                            break;
  }

  string security_group = msg.securityGroup();
  string asset = msg.asset();
  int secid = msg.securityID();
  // int security_trading_status = msg.securityTradingStatus();
  auto trade_date = msg.tradeDate();
  int halt_reason = msg.haltReason();
  int security_trading_event = msg.securityTradingEvent();

  cout << "SecurityStatus: SecurityID:" << secid
    << " SecurityGroup:" << security_group
    << " Asset:" << asset
    << " TradeDate:" << trade_date
    << " SecurityTradingStatus:" << security_status
    << " HaltReason:" << halt_reason
    << " SecurityTradingEvent:" << security_trading_event << endl;
}

  void
OnChannelReset(mktdata::MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
  return;
#endif
  ChannelReset4 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noMDEntries();

  while(ent.hasNext()) {
    ent.next();
    int md_update_action = ent.mDUpdateAction();
    string md_entry_type = ent.mDEntryType();
    cout << "Channel Reset: MDUpdateAction:" << md_update_action
      << " MDEntryType:" << md_entry_type << endl;
  }
  // ResetBooks();
}

  void
OnSnapshotFullRefresh(mktdata::MessageHeader& hdr, char *buf, int len)
{
#ifndef __DEBUG_OUTPUT__
  return;
#endif
  SnapshotFullRefresh38 msg;

  msg.wrapForDecode(buf, hdr.encodedLength(), hdr.blockLength(), MDP_VERSION, len);

  auto& ent = msg.noMDEntries();

  int last_seq_no = msg.lastMsgSeqNumProcessed();
  int tot_num_reports = msg.totNumReports();
  int secid = msg.securityID();
  int rpt_seq = msg.rptSeq();
  auto trade_date = msg.tradeDate();
  int security_trading_status = msg.mDSecurityTradingStatus();
  int64_t high_limit = msg.highLimitPrice().mantissa();
  int64_t low_limit = msg.lowLimitPrice().mantissa();
  int64_t max_price_variation = msg.maxPriceVariation().mantissa();

  cout << "SnapshotFullRefresh: SecurityID:" << secid
    << " TotNumReport:" << tot_num_reports
    << " LastMsgSeqNumProcessed:" << last_seq_no
    << " RptSeq:" << rpt_seq
    << " TradeDate:" << trade_date
    << " SecurityTradingStatus:" << security_trading_status
    << " HighLimit:" << high_limit
    << " LowLimit:" << low_limit
    << " MaxPriceVariation:" << max_price_variation << endl;
  while(ent.hasNext()) {
    ent.next();
    int64_t md_entry_px = ent.mDEntryPx().mantissa();
    int md_entry_size = ent.mDEntrySize();
    int no_orders = ent.numberOfOrders();
    int md_price_level = ent.mDPriceLevel();
    auto trading_date = ent.tradingReferenceDate();
    int open_close_settl_flag = ent.openCloseSettlFlag();
    auto settl_price_type = ent.settlPriceType();
    char md_entry_type = ent.mDEntryType();

    cout << "noMDEntries: SecurityID:" << secid
      << " Price:" << md_entry_px
      << " EntrySize:" << md_entry_size
      << " OrderNo:" << no_orders
      << " MDPriceLevel:" << md_price_level
      << " tradingReferenceDate:" << trading_date
      << " OpenCloseSettlFlag:" << open_close_settl_flag
      << " EntryType:" << md_entry_type << endl;
    printf("Settlement Price Type: ");
    if (settl_price_type.finalfinal()) 
      cout << "finalfinal ";
    if (settl_price_type.actual()) 
      cout << "actual ";
    if (settl_price_type.rounded()) 
      cout << "rounded ";
    if (settl_price_type.intraday()) 
      cout << "intraday ";
    if (settl_price_type.reservedBits()) 
      cout << "reservedBits ";
    if (settl_price_type.nullValue()) 
      cout << "nullValue ";
    cout << endl;		
  }
  // ResetBooks();
}

string TransferTimestamp(uint64_t micro_sec) {
  uint64_t seconds = micro_sec / 1000000000;
  long int msec = (long int)seconds;
  struct tm *timeinfo = localtime(&msec);
  struct timeval tv;
  gettimeofday(&tv, NULL);
  char str[32];
  sprintf(str, "%d%02d%02d-%02d%02d", 1900 + timeinfo->tm_year,
          1 + timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour,
          timeinfo->tm_min);
  return string(str);
}

