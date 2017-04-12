#include "OrderBook.h"

// ------------------------------------------------------------------
// OrderBook Implementation
// ------------------------------------------------------------------
void OrderBook::Reset()
{
  Reset(BID);
  Reset(ASK);
}
void OrderBook::Reset(BASide Side)
{
  if (Side == BID) _BIDQend = _BIDQstart;
  else _ASKQend = _ASKQstart;
}
int16_t OrderBook::Bounded(int16_t bnd) const
{
  while (bnd < 0) bnd += _CIRARRSIZE;
  while (bnd >= _CIRARRSIZE) bnd -= _CIRARRSIZE;
  return bnd;
}

// Semi-binary search
// Returns the price level if Price can be found
// Returns a negative price level if Price cannot be found, negating this will give
//         the correct price level to use for Add()
int16_t OrderBook::LocatePriceLev(BASide Side, int32_t Price) const
{
  int iNumOfPriceLev = NumOfPriceLev(Side);
  if (iNumOfPriceLev == 0) return -1;

  // Further analysis
  OBElement* Que = GetQue(Side);
  const int16_t & queStart = GetQueStartConst(Side); // Getting as reference
  const int16_t & queEnd = GetQueEndConst(Side); // Getting as reference

  // Get the mid-pt
  int16_t iMidIdx = Bounded(queStart+iNumOfPriceLev/2);
  double dMidPx = Que[iMidIdx].Price;

  if (dMidPx == Price) return (iNumOfPriceLev/2+1);
  else if ((Side == BID && dMidPx > Price) ||
      (Side == ASK && dMidPx < Price)) // Search tail part
  {
    int i = Bounded(iMidIdx+1);
    while (i != queEnd)
    {
      if (Que[i].Price == Price) return (i+(i<iMidIdx?_CIRARRSIZE:0)-iMidIdx+iNumOfPriceLev/2+1); // Found
      else if ((Side == BID && Que[i].Price < Price) ||
          (Side == ASK && Que[i].Price > Price)) return -(i+(i<iMidIdx?_CIRARRSIZE:0)-iMidIdx+iNumOfPriceLev/2+1); // Can't be found
      i=Bounded(i+1);
    }
    return -(iNumOfPriceLev+1); // Can't be found
  }
  else //if ((Side == BID && dMidPx < Price) ||
    //(Side == ASK && dMidPx > Price)) // Search head part
  {
    int i = queStart;
    while (i != iMidIdx)
    {
      if (Que[i].Price == Price) return (i+(i<queStart?_CIRARRSIZE:0)-queStart+1); // Found
      else if ((Side == BID && Que[i].Price < Price) ||
          (Side == ASK && Que[i].Price > Price)) return -(i+(i<queStart?_CIRARRSIZE:0)-queStart+1); // Can't be found
      i=Bounded(i+1);
    }
    return -(iNumOfPriceLev/2+1); // Can't be found
  }
}

bool OrderBook::AssessPriceIntegrity() const
{
  // BID
  // Checking if Prices are strictly monotonically decreasing
  double dLastPrice = 0;
  for (int16_t iLoc = _BIDQstart; iLoc != _BIDQend; iLoc=Bounded(iLoc+1))
  {
    if (dLastPrice == 0)
    {
      dLastPrice = (*_BIDQ)[iLoc].Price;
    }
    else
    {
      if ((*_BIDQ)[iLoc].Price >= dLastPrice) return false;
      else dLastPrice = (*_BIDQ)[iLoc].Price;
    }
  }
  // ASK
  dLastPrice = 0;
  for (int16_t iLoc = _ASKQstart; iLoc != _ASKQend; iLoc=Bounded(iLoc+1))
  {
    if (dLastPrice == 0)
    {
      dLastPrice = (*_ASKQ)[iLoc].Price;
    }
    else
    {
      if ((*_ASKQ)[iLoc].Price <= dLastPrice) return false;
      else dLastPrice = (*_ASKQ)[iLoc].Price;
    }
  }

  // No exception noted in Price
  return true;
}

OrderBook::OrderBook() :
  _BIDQstart(0),
  _BIDQend(0),
  _ASKQstart(0),
  _ASKQend(0)
{
  _BIDQ.reset(new vector<OBElement>());
  _ASKQ.reset(new vector<OBElement>());
  (*_BIDQ).resize(_CIRARRSIZE);
  (*_ASKQ).resize(_CIRARRSIZE);
}
OrderBook::~OrderBook() {}

int16_t OrderBook::NumOfPriceLev(BASide Side) const
{
  const int16_t & queStart = GetQueStartConst(Side); // Getting as reference
  const int16_t & queEnd = GetQueEndConst(Side); // Getting as reference
  if (queEnd >= queStart) return queEnd-queStart;
  else return queEnd+_CIRARRSIZE-queStart;
}
OBElement* OrderBook::GetQue(BASide Side) const
{
  if (Side == BID) return &((*_BIDQ)[0]); else return &((*_ASKQ)[0]);
}
int16_t & OrderBook::GetQueStart(BASide Side)
{
  if (Side == BID) return _BIDQstart; else return _ASKQstart;
}
int16_t & OrderBook::GetQueEnd(BASide Side)
{
  if (Side == BID) return _BIDQend; else return _ASKQend;
}
const int16_t & OrderBook::GetQueStartConst(BASide Side) const
{
  if (Side == BID) return _BIDQstart; else return _ASKQstart;
}
const int16_t & OrderBook::GetQueEndConst(BASide Side) const
{
  if (Side == BID) return _BIDQend; else return _ASKQend;
}
bool OrderBook::Delete(
    BASide Side,
    int32_t Price,
    uint8_t PriceLev,
    uint32_t NumOfOrd,
    uint64_t AggQty)
{
  // ------------------------------------------------------------
  // Integrity check
  // ------------------------------------------------------------
  if ((Price <= 0) ||
      (PriceLev <= 0) ||
      (PriceLev > _MAXTICKLEVEL) ||
      (NumOfOrd <= 0) ||
      (AggQty <= 0))
  {
    return false;
  }

  OBElement* Que = GetQue(Side);
  int16_t & queStart = GetQueStart(Side); // Getting as reference
  int16_t & queEnd = GetQueEnd(Side); // Getting as reference

  int16_t iDelLoc = Bounded(queStart+PriceLev-1);

  // ------------------------------------------------------------
  // Normal case
  // ------------------------------------------------------------
  if (Que[iDelLoc].Price == Price) // Not checking NumOfOrd / AggQty as they are not so essential
  {
    if (iDelLoc == queStart)
    {
      queStart = Bounded(queStart+1);
    }
    else if (iDelLoc == queEnd)
    {
      queEnd = Bounded(queEnd-1);
    }
    else // The removal is in the middle of the queue
    {
      int i = iDelLoc;
      while (i != Bounded(queEnd-1))
      {
        memmove(&(Que[Bounded(i)]),&(Que[Bounded(i+1)]),sizeof(OBElement));
        i = Bounded(i+1);
      }
      queEnd = Bounded(queEnd-1);
    }
    return true;
  }

  // ------------------------------------------------------------
  // Lenient Treatment below
  // Delete by Absolute Price only, ignore other info
  // ------------------------------------------------------------

  int16_t iSearchedPxLev = LocatePriceLev(Side, Price);
  if (iSearchedPxLev > 0) // Price found
  {
    iDelLoc = Bounded(queStart+iSearchedPxLev-1);
    // Recursive f() to avoid code repetition
    return Delete(
        Side,
        Price,
        iSearchedPxLev,
        Que[iDelLoc].NumOfOrd,
        Que[iDelLoc].AggQty
        );
  }
  else // Price not found, report error
  {
    return false;
  }

}
bool OrderBook::Change(
    BASide Side,
    int32_t Price,
    uint8_t PriceLev,
    uint32_t NumOfOrd,
    uint64_t AggQty)
{
  // ------------------------------------------------------------
  // Integrity checks
  // ------------------------------------------------------------
  if ((Price <= 0) ||
      (PriceLev <= 0) ||
      (PriceLev > _MAXTICKLEVEL) ||
      (NumOfOrd <= 0) ||
      (AggQty <= 0))
  {
    return false;
  }

  OBElement* Que = GetQue(Side);
  const int16_t & queStart = GetQueStartConst(Side); // Getting as reference

  int16_t iChgLoc = Bounded(queStart+PriceLev-1);

  // ------------------------------------------------------------
  // Normal Order Book Change
  // Check for normal cases first because they should occur with the highest freq
  // From P.40/48, "Change" can only change AggQty but not Price
  // ------------------------------------------------------------
  if (Price == Que[iChgLoc].Price && NumOfPriceLev(Side) >= PriceLev)
  {
    Que[iChgLoc].AggQty = AggQty;
    Que[iChgLoc].NumOfOrd = NumOfOrd;
    return true;
  }

  // ------------------------------------------------------------
  // Lenient Treatment below
  // ------------------------------------------------------------
  int16_t iNumOfPriceLev = NumOfPriceLev(Side);


  // ------------------------------------------------------------
  // From HKEx's binary data, HKEx may use Change() in the place of Add()
  // Yes, it's strange, but it's true
  // ------------------------------------------------------------
  // Implicit conversion to Add()
  // Case 1. The local order book is empty
  // ------------------------------------------------------------
  if (iNumOfPriceLev == 0)
  {
    return Add(
        Side,
        Price,
        1,
        NumOfOrd,
        AggQty);
  }

  // ------------------------------------------------------------
  // Case 2. Either this Price exists / does not exist in our local order book
  // ------------------------------------------------------------
  int16_t iSearchedPxLev = LocatePriceLev(Side, Price);
  if (iSearchedPxLev > 0) // Price found, but of course the HKEx Price Level is inconsistent with our local order book
  {
    iChgLoc = Bounded(queStart+iSearchedPxLev-1);
    Que[iChgLoc].AggQty = AggQty;
    Que[iChgLoc].NumOfOrd = NumOfOrd;
    return true;
  }
  else // No such Price found, can Add() it safely
  {
    return Add(
        Side,
        Price,
        -iSearchedPxLev,
        NumOfOrd,
        AggQty);
  }

}
bool OrderBook::Add(
    BASide Side,
    int32_t Price,
    uint8_t PriceLev,
    uint32_t NumOfOrd,
    uint64_t AggQty)
{
  // ------------------------------------------------------------
  // Integrity checks
  // ------------------------------------------------------------
  if ((Price <= 0) ||
      (PriceLev <= 0) ||
      (PriceLev > _MAXTICKLEVEL) ||
      (NumOfOrd <= 0) ||
      (AggQty <= 0))
  {
    return false;
  }

  OBElement* Que = GetQue(Side);
  int16_t & queStart = GetQueStart(Side); // Getting as reference
  int16_t & queEnd = GetQueEnd(Side); // Getting as reference
  int16_t iNumOfPriceLev = NumOfPriceLev(Side);

  // ------------------------------------------------------------
  // Normal cases (lenient treatment)
  // ------------------------------------------------------------

  // ------------------------------------------------------------
  // The simplest case of an empty queue
  // ------------------------------------------------------------
  if ((queStart == queEnd)) // Empty Queue
  {
    Que[queEnd].AggQty = AggQty;
    Que[queEnd].NumOfOrd = NumOfOrd;
    Que[queEnd].Price = Price;
    queEnd = Bounded(queEnd+1);
  }

  // Adding at the Tail of non-empty queue
  // Skip checking Price Level - lenient approach right away
  else if ((Side == BID && Que[Bounded(queEnd-1)].Price > Price) ||
      (Side == ASK && Que[Bounded(queEnd-1)].Price < Price))
  {
    Que[queEnd].AggQty = AggQty;
    Que[queEnd].NumOfOrd = NumOfOrd;
    Que[queEnd].Price = Price;
    queEnd = Bounded(queEnd+1);
  }

  // Adding at the Head of non-empty queue
  // Skip checking Price Level - lenient approach right away
  else if ((Side == BID && Que[queStart].Price < Price) ||
      (Side == ASK && Que[queStart].Price > Price))
  {
    queStart = Bounded(queStart-1);
    Que[queStart].AggQty = AggQty;
    Que[queStart].NumOfOrd = NumOfOrd;
    Que[queStart].Price = Price;
  }
  else
  {
    // ------------------------------------------------------------
    int16_t iSearchedPxLev = LocatePriceLev(Side, Price);

    // ------------------------------------------------------------
    // Normal insertion in the middle
    // ------------------------------------------------------------
    if (iSearchedPxLev < 0)
    {
      // Move existing entries down
      int i = queEnd;
      int16_t iAddLoc = Bounded(queStart+(-iSearchedPxLev)-1);
      while (i != iAddLoc)
      {
        memmove(&(Que[Bounded(i)]),&(Que[Bounded(i-1)]),sizeof(OBElement));
        i = Bounded(i-1);
      }
      queEnd = Bounded(queEnd+1);

      // Insert at the specified price level
      Que[iAddLoc].AggQty = AggQty;
      Que[iAddLoc].NumOfOrd = NumOfOrd;
      Que[iAddLoc].Price = Price;
    }
    // ------------------------------------------------------------
    // Clashing with existing prices
    // Implicait conversion to Change()
    // ------------------------------------------------------------
    else if (iSearchedPxLev <= iNumOfPriceLev)
    {
      return Change(
          Side,
          Price,
          iSearchedPxLev,
          NumOfOrd,
          AggQty);
    }
  }

  // Automatic Deletion beyond 10 Price Levels
  // Orders exceeding 10 Tick but within 10 PriceLevel: Explicit Deletion, therefore don't need to delete myself
  int iExcess = NumOfPriceLev(Side) - _MAXPRICELEVEL;
  if (iExcess > 0) queEnd = Bounded(queEnd-iExcess);

  // // ------------------------------------------------------------
  // // From HKEx's binary data, after BID and ASK has crossed,
  // // It may omit the Delete() instruction for the removed Best BID / ASK
  // // Note that the prices at Best Bid and Best Ask are allowed to be equal,
  // // because it's permitted during the auction period.
  // // But crossing of prices with Best Bid price > Best Ask price is not allowed.
  // // ------------------------------------------------------------
  // OBElement *BidQue = GetQue(BID);
  // OBElement *AskQue = GetQue(ASK);

  // BASide sOppSide = (Side == BID ? ASK : BID);
  // int16_t & oppQueStart = GetQueStart(sOppSide);

  // while (NumOfPriceLev(BID) > 0 &&
  // 		NumOfPriceLev(ASK) > 0 &&
  // 		BidQue[GetQueStart(BID)].Price > AskQue[GetQueStart(ASK)].Price)
  // {
  // 	// Abnormal crossing occurred,
  // 	// Delete the crossed items beyond the best price
  // 	oppQueStart = Bounded(oppQueStart+1);
  // }

  return true;
}
string OrderBook::DumpJson(const uint32_t uiSecurityCode, BASide Side) const
{
  stringstream ssRtn;
  ssRtn << "{\"Order_Book_Snapshot\":{\"SecurityCode\":" << uiSecurityCode << ",\"Side\":" << (Side == BID ? "\"B\"" : "\"A\"") << ",";
  if (Side == BID)
  {
    for (int i = _BIDQstart; i != _BIDQend; i = Bounded(i+1))
    {
      if (i != _BIDQstart) ssRtn << ",";
      ssRtn << "\"BID\":{\"Price\":" << (*_BIDQ)[i].Price << ",\"NumOfOrders\":" << (*_BIDQ)[i].NumOfOrd << ",\"AggQty\":" << (*_BIDQ)[i].AggQty << "}";
    }
  }
  else
  {
    for (int i = _ASKQstart; i != _ASKQend; i = Bounded(i+1))
    {
      if (i != _ASKQstart) ssRtn << ",";
      ssRtn << "\"ASK\":{\"Price\":" << (*_ASKQ)[i].Price << ",\"NumOfOrders\":" << (*_ASKQ)[i].NumOfOrd << ",\"AggQty\":" << (*_ASKQ)[i].AggQty << "}";
    }
  }
  ssRtn << "}}";
  return ssRtn.str();
}
string OrderBook::Dump(BASide Side) const
{
  stringstream ssRtn;
  if (Side == BID)
  {
    ssRtn << "_BIDQstart = " << _BIDQstart << endl;
    ssRtn << "_BIDQend = " << _BIDQend << endl;

    ssRtn << "#Ord\tAggQty\tPrice" << endl;
    for (int i = _BIDQstart; i != _BIDQend; i = Bounded(i+1))
      ssRtn << (*_BIDQ)[i].NumOfOrd << "\t" << (*_BIDQ)[i].AggQty << "\t" << (*_BIDQ)[i].Price << "\t" << endl;
  }
  else
  {
    ssRtn << "_ASKQstart = " << _ASKQstart << endl;
    ssRtn << "_ASKQend = " << _ASKQend << endl;

    ssRtn << "#Ord\tAggQty\tPrice" << endl;
    for (int i = _ASKQstart; i != _ASKQend; i = Bounded(i+1))
      ssRtn << (*_ASKQ)[i].NumOfOrd << "\t" << (*_ASKQ)[i].AggQty << "\t" << (*_ASKQ)[i].Price << "\t" << endl;
  }
  return ssRtn.str();
}
void OrderBook::Dump(BASide Side, int iOut[_MAXPRICELEVEL][3]) const
{
  for (int i = 0; i < _MAXPRICELEVEL; ++i)
    for (int j = 0; j < 3; ++j)
      iOut[i][j] = 0;

  if (Side == BID)
  {
    if (_BIDQstart == _BIDQend) return;

    int j = 0;
    for (int i = _BIDQstart; i != _BIDQend; i = Bounded(i+1), ++j)
    {
      iOut[j][0] = (*_BIDQ)[i].NumOfOrd;
      iOut[j][1] = (*_BIDQ)[i].AggQty;
      iOut[j][2] = (*_BIDQ)[i].Price;
    }
  }
  else
  {
    if (_ASKQstart == _ASKQend) return;

    int j = 0;
    for (int i = _ASKQstart; i != _ASKQend; i = Bounded(i+1), ++j)
    {
      iOut[j][0] = (*_ASKQ)[i].NumOfOrd;
      iOut[j][1] = (*_ASKQ)[i].AggQty;
      iOut[j][2] = (*_ASKQ)[i].Price;
    }
  }
}



//--------------------------------------------------
// OrderBookOMDDSP
//--------------------------------------------------
OrderBookOMDDSP::OrderBookOMDDSP()
{
  Reset();
  return;
}

OrderBookOMDDSP::~OrderBookOMDDSP()
{
  return;
}

void OrderBookOMDDSP::Reset()
{
  OrderBook::Reset();
  _BidAggregated11.AggQty = 0;
  _BidAggregated11.NumOfOrd = 0;
  _AskAggregated11.AggQty = 0;
  _AskAggregated11.NumOfOrd = 0;
  _BidMktOrder.AggQty = 0;
  _BidMktOrder.NumOfOrd = 0;
  _AskMktOrder.AggQty = 0;
  _AskMktOrder.NumOfOrd = 0;

  return;
}

void OrderBookOMDDSP::Reset(BASide Side)
{
  OrderBook::Reset(Side);

  if (Side == BID)
  {
    _BidAggregated11.AggQty = 0;
    _BidAggregated11.NumOfOrd = 0;
    _BidMktOrder.AggQty = 0;
    _BidMktOrder.NumOfOrd = 0;
  }
  else
  {
    _AskAggregated11.AggQty = 0;
    _AskAggregated11.NumOfOrd = 0;
    _AskMktOrder.AggQty = 0;
    _AskMktOrder.NumOfOrd = 0;
  }

  return;
}

bool OrderBookOMDDSP::Add(
    BASide Side,
    int32_t Price,
    uint8_t PriceLev,
    uint32_t NumOfOrd,
    uint64_t AggQty)
{
  //--------------------------------------------------
  // Aggregated 11th level
  //--------------------------------------------------
  if (PriceLev == 255)
  {
    if (Side == BID)
    {
      _BidAggregated11.AggQty = AggQty;
      _BidAggregated11.NumOfOrd = NumOfOrd;
    }
    else
    {
      _AskAggregated11.AggQty = AggQty;
      _AskAggregated11.NumOfOrd = NumOfOrd;
    }
    return true;
  }
  //--------------------------------------------------
  // Market Order
  //--------------------------------------------------
  else if (Price <= 0 && PriceLev == 1)
  {
    if (Side == BID)
    {
      _BidMktOrder.AggQty = AggQty;
      _BidMktOrder.NumOfOrd = NumOfOrd;
    }
    else
    {
      _AskMktOrder.AggQty = AggQty;
      _AskMktOrder.NumOfOrd = NumOfOrd;
    }
    return true;
  }
  //--------------------------------------------------
  // Normal Order Book
  //--------------------------------------------------
  else
  {
    return OrderBook::Add(Side,Price,PriceLev,NumOfOrd,AggQty);
  }
  return false;
}

bool OrderBookOMDDSP::Change(
    BASide Side,
    int32_t Price,
    uint8_t PriceLev,
    uint32_t NumOfOrd,
    uint64_t AggQty)
{
  //--------------------------------------------------
  // Aggregated 11th level
  //--------------------------------------------------
  if (PriceLev == 255)
  {
    if (Side == BID)
    {
      _BidAggregated11.AggQty = AggQty;
      _BidAggregated11.NumOfOrd = NumOfOrd;
    }
    else
    {
      _AskAggregated11.AggQty = AggQty;
      _AskAggregated11.NumOfOrd = NumOfOrd;
    }
    return true;
  }
  //--------------------------------------------------
  // Market Order
  //--------------------------------------------------
  else if (Price <= 0 && PriceLev == 1)
  {
    if (Side == BID)
    {
      _BidMktOrder.AggQty = AggQty;
      _BidMktOrder.NumOfOrd = NumOfOrd;
    }
    else
    {
      _AskMktOrder.AggQty = AggQty;
      _AskMktOrder.NumOfOrd = NumOfOrd;
    }
    return true;
  }
  //--------------------------------------------------
  // Normal Order Book
  //--------------------------------------------------
  else
  {
    return OrderBook::Change(Side,Price,PriceLev,NumOfOrd,AggQty);
  }
  return false;
}

bool OrderBookOMDDSP::Delete(
    BASide Side,
    int32_t Price,
    uint8_t PriceLev,
    uint32_t NumOfOrd,
    uint64_t AggQty)
{
  //--------------------------------------------------
  // Aggregated 11th level
  //--------------------------------------------------
  if (PriceLev == 255)
  {
    if (Side == BID)
    {
      _BidAggregated11.AggQty = 0;
      _BidAggregated11.NumOfOrd = 0;
    }
    else
    {
      _AskAggregated11.AggQty = 0;
      _AskAggregated11.NumOfOrd = 0;
    }
    return true;
  }
  //--------------------------------------------------
  // Market Order
  //--------------------------------------------------
  else if (Price <= 0 && PriceLev == 1)
  {
    if (Side == BID)
    {
      _BidMktOrder.AggQty = 0;
      _BidMktOrder.NumOfOrd = 0;
    }
    else
    {
      _AskMktOrder.AggQty = 0;
      _AskMktOrder.NumOfOrd = 0;
    }
    return true;
  }
  //--------------------------------------------------
  // Normal Order Book
  //--------------------------------------------------
  else
  {
    return OrderBook::Delete(Side,Price,PriceLev,NumOfOrd,AggQty);
  }
  return false;
}

string OrderBookOMDDSP::DumpJson(const uint32_t uiOrderBookID, BASide Side) const
{
  stringstream ssRtn;
  ssRtn << "{\"Order_Book_Snapshot\":{\"OrderBookID\":" << uiOrderBookID << ",\"Side\":" << (Side == BID ? "\"B\"" : "\"A\"") << ",";
  if (Side == BID)
  {
    ssRtn << "\"MarketOrder\":{\"NumOfOrders\":" << _BidMktOrder.NumOfOrd << ",\"AggQty\":" << _BidMktOrder.AggQty << "}";
    for (int i = _BIDQstart; i != _BIDQend; i = Bounded(i+1))
    {
      if (i != _BIDQstart) ssRtn << ",";
      ssRtn << "\"BID\":{\"Price\":" << (*_BIDQ)[i].Price << ",\"NumOfOrders\":" << (*_BIDQ)[i].NumOfOrd << ",\"AggQty\":" << (*_BIDQ)[i].AggQty << "}";
    }
    ssRtn << "\"Aggregated11thLevel\":{\"\"NumOfOrders\":" << _BidAggregated11.NumOfOrd << ",\"AggQty\":" << _BidAggregated11.AggQty << "}";
  }
  else
  {
    ssRtn << "\"MarketOrder\":{\"NumOfOrders\":" << _AskMktOrder.NumOfOrd << ",\"AggQty\":" << _AskMktOrder.AggQty << "}";
    for (int i = _ASKQstart; i != _ASKQend; i = Bounded(i+1))
    {
      if (i != _ASKQstart) ssRtn << ",";
      ssRtn << "\"ASK\":{\"Price\":" << (*_ASKQ)[i].Price << ",\"NumOfOrders\":" << (*_ASKQ)[i].NumOfOrd << ",\"AggQty\":" << (*_ASKQ)[i].AggQty << "}";
    }
    ssRtn << "\"Aggregated11thLevel\":{\"\"NumOfOrders\":" << _AskAggregated11.NumOfOrd << ",\"AggQty\":" << _AskAggregated11.AggQty << "}";
  }
  ssRtn << "}}";
  return ssRtn.str();
}

string OrderBookOMDDSP::Dump(BASide Side) const
{
  stringstream ssRtn;
  if (Side == BID)
  {
    ssRtn << "_BIDQstart = " << _BIDQstart << endl;
    ssRtn << "_BIDQend = " << _BIDQend << endl;

    ssRtn << "#Ord\tAggQty\tPrice" << endl;
    for (int i = _BIDQstart; i != _BIDQend; i = Bounded(i+1))
      ssRtn << (*_BIDQ)[i].NumOfOrd << "\t" << (*_BIDQ)[i].AggQty << "\t" << (*_BIDQ)[i].Price << "\t" << endl;

    ssRtn << "_Agg11.AggQty = " << _BidAggregated11.AggQty << endl;
    ssRtn << "_Agg11.NumOfOrd = " << _BidAggregated11.NumOfOrd << endl;
    ssRtn << "_MktOrd.AggQty = " << _BidMktOrder.AggQty << endl;
    ssRtn << "_MktOrd.NumOfOrd = " << _BidMktOrder.NumOfOrd << endl;
  }
  else
  {
    ssRtn << "_ASKQstart = " << _ASKQstart << endl;
    ssRtn << "_ASKQend = " << _ASKQend << endl;

    ssRtn << "#Ord\tAggQty\tPrice" << endl;
    for (int i = _ASKQstart; i != _ASKQend; i = Bounded(i+1))
      ssRtn << (*_ASKQ)[i].NumOfOrd << "\t" << (*_ASKQ)[i].AggQty << "\t" << (*_ASKQ)[i].Price << "\t" << endl;

    ssRtn << "_Agg11.AggQty = " << _AskAggregated11.AggQty << endl;
    ssRtn << "_Agg11.NumOfOrd = " << _AskAggregated11.NumOfOrd << endl;
    ssRtn << "_MktOrd.AggQty = " << _AskMktOrder.AggQty << endl;
    ssRtn << "_MktOrd.NumOfOrd = " << _AskMktOrder.NumOfOrd << endl;
  }
  return ssRtn.str();
}

void OrderBookOMDDSP::Dump(BASide Side, int iOut[_MAXPRICELEVEL+2][3]) const
{
  for (int i = 0; i < _MAXPRICELEVEL+2; ++i)
    for (int j = 0; j < 3; ++j)
      iOut[i][j] = 0;

  if (Side == BID)
  {
    if (_BIDQstart == _BIDQend) return;

    int j = 1;
    for (int i = _BIDQstart; i != _BIDQend; i = Bounded(i+1), ++j)
    {
      iOut[j][0] = (*_BIDQ)[i].NumOfOrd;
      iOut[j][1] = (*_BIDQ)[i].AggQty;
      iOut[j][2] = (*_BIDQ)[i].Price;
    }
    //--------------------------------------------------
    // the Aggregated 11th level
    //--------------------------------------------------
    iOut[11][0] = _BidAggregated11.NumOfOrd;
    iOut[11][1] = _BidAggregated11.AggQty;
    iOut[11][2] = 0;
    //--------------------------------------------------
    // Market Order
    //--------------------------------------------------
    iOut[0][0] = _BidMktOrder.NumOfOrd;
    iOut[0][1] = _BidMktOrder.AggQty;
    iOut[0][2] = 0;
  }
  else
  {
    if (_ASKQstart == _ASKQend) return;

    int j = 1;
    for (int i = _ASKQstart; i != _ASKQend; i = Bounded(i+1), ++j)
    {
      iOut[j][0] = (*_ASKQ)[i].NumOfOrd;
      iOut[j][1] = (*_ASKQ)[i].AggQty;
      iOut[j][2] = (*_ASKQ)[i].Price;
    }
    //--------------------------------------------------
    // the Aggregated 11th level
    //--------------------------------------------------
    iOut[11][0] = _AskAggregated11.NumOfOrd;
    iOut[11][1] = _AskAggregated11.AggQty;
    iOut[11][2] = 0;
    //--------------------------------------------------
    // Market Order
    //--------------------------------------------------
    iOut[0][0] = _AskMktOrder.NumOfOrd;
    iOut[0][1] = _AskMktOrder.AggQty;
    iOut[0][2] = 0;
  }
}
