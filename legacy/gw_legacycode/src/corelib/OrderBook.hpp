#ifndef ORDERBOOKSTRUCT_HPP
#define ORDERBOOKSTRUCT_HPP

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cmath>
using namespace std;
// Key assumptions:
// Frequency rank of different tasks:
// 1. Check order book integrity (against every HKEx order book instructions)
// 2. Update on order quantity / number of orders (including best or ordinary bids / offers)
// 3. Add / remove best bid / offer price level
// 4. Add / remove price levels other than best bid / offer
// 5. Handle snapshot requests (on subscribed instruments)
// 6. Handling HKEx instructions with minor inconsistencies (leniently).
// 
// Argument for such assumptions about frequency distribution:
// Here's an example order book for an active stock:
// 8620  12 K
// 8615  21 K
// 8610  93 K
// 8605 191 K
// 8600 866 K best offer
// 8595 170 K best bid
// 8590 545 K
// 8585 146 K
// 8580 245 K
// 8575  26 K
// 
// #1. Order book integrity has to be checked on every HKEx order book instruction, so its frequency is the highest.
// #2. There are 10 places where order quantity can change, therefore we expect this to be extremely frequent.
// #3. Best bid / offer will change places only if the orders at best bid / offer are exhausted. It should be less common than #2.
// #4. For active stocks, usually every tick level will have some orders queuing already. Therefore the frequency of adding / removing a price level in the middle of the queue is relatively low.  For inactive stocks, they won't have frequent updates by definition, so don't worry about them.
// #5. Only need to generate snapshots for certain subscribed instruments, which is only expected to be a small subset of the total number of instruments in the whole market.
// 
// For our order book case, the best candidates (in descending order) that give the best performance are:
// A. Circular array of price levels (not tick level) [offers superb performance except #4 and #5]
// B. Double-ended queue of price levels [still needs memory allocation / deallocation, slower than circular array]
// C. Doubly linked list of price levels [problem is with traversal from head / tail is required]
// D. (lightweight) doubly linked list of price levels + circular array of tick levels [maintaining 2 data structures drags performance]
// E. Regular array of tick levels
// F. Any kind of sets / maps. [have to access all elements / keys on every operation; Too slow]
// 
// Trees, (Hash) Maps are inappropriate.
// 
// To enable association of price level with array index, don't store empty price levels.
// 
// Use circular array of pre-allocated structs, or pointers to structs? Use array of pre-allocated structs. Storing pointes don't offer much performance boost, not worth the overhead in memory allocation / de-allocation.
// 
// Each element in the Order Book contains:
// . Aggregate quantity
// . Number of orders
// . Price
// 
// Lenient treatment of erroneous order book instructions:
// It is inevitable that some of the order book instructions contain contradictory information,
// whether due to missing packets or other reasons.
// This class will handle erroneous instructions on a best-effort basis.
//
// On receiving contradictory information:
// - Absolute prices have precedence over relative price levels
// - Change() instructions may be implicitly converted to Add() instructions, and vice versa
// - There is no implicit conversion for Delete()
//
// The roles of Add() and Change() is blurred after adopting lenient treatment.

struct OBElement
{
	ssize_t AggQty;
	ssize_t NumOfOrd;
	double Price;
};

class OrderBook {
	public:
		enum BASide {BID, ASK};

		OrderBook();
		~OrderBook();
		inline void Reset();
		inline void Reset(BASide);
		bool Add(BASide,double,ssize_t,ssize_t,ssize_t);
		bool Change(BASide,double,ssize_t,ssize_t,ssize_t);
		bool Delete(BASide,double,ssize_t,ssize_t,ssize_t);
		void DumpRaw(BASide) const;
		void Dump(BASide) const;
		void Dump(BASide,int[10][3]) const;

		// Info
		inline ssize_t NumOfPriceLev(BASide) const;
		ssize_t LocatePriceLev(BASide,double) const;
		bool AssessPriceIntegrity() const;

	private:
		OBElement* _BIDQ;
		OBElement* _ASKQ;
		ssize_t _BIDQstart;
		ssize_t _BIDQend;
		ssize_t _ASKQstart;
		ssize_t _ASKQend;
		static const ssize_t _MAXPRICELEVEL;
		static const ssize_t _MAXTICKLEVEL;
		static const ssize_t _CIRARRSIZE;

		inline OBElement* GetQue(BASide) const;
		inline ssize_t & GetQueStart(BASide);
		inline ssize_t & GetQueEnd(BASide);
		inline const ssize_t & GetQueStartConst(BASide) const;
		inline const ssize_t & GetQueEndConst(BASide) const;

		inline ssize_t Bounded(ssize_t) const;

};


const ssize_t OrderBook::_MAXPRICELEVEL = 10;
const ssize_t OrderBook::_MAXTICKLEVEL = 10;
const ssize_t OrderBook::_CIRARRSIZE = _MAXPRICELEVEL+2; // For buffering in Add()

// ------------------------------------------------------------------
// OrderBook Implementation
// ------------------------------------------------------------------
ssize_t OrderBook::Bounded(ssize_t bnd) const
{
	while (bnd < 0) bnd += _CIRARRSIZE;
	while (bnd >= _CIRARRSIZE) bnd -= _CIRARRSIZE;
	return bnd;
}

// Semi-binary search
// Returns the price level if Price can be found
// Returns a negative price level if Price cannot be found, negating this will give
//         the correct price level to use for Add()
ssize_t OrderBook::LocatePriceLev(BASide Side, double Price) const
{
	int iNumOfPriceLev = NumOfPriceLev(Side);
	if (iNumOfPriceLev == 0) return -1;

	// Further analysis
	OBElement *Que = GetQue(Side);
	const ssize_t & queStart = GetQueStartConst(Side); // Getting as reference
	const ssize_t & queEnd = GetQueEndConst(Side); // Getting as reference

	// Get the mid-pt
	ssize_t iMidIdx = Bounded(queStart+iNumOfPriceLev/2);
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
	for (ssize_t iLoc = _BIDQstart; iLoc != _BIDQend; iLoc=Bounded(iLoc+1))
	{
		if (dLastPrice == 0)
		{
			dLastPrice = _BIDQ[iLoc].Price;
		}
		else
		{
			if (_BIDQ[iLoc].Price >= dLastPrice) return false;
			else dLastPrice = _BIDQ[iLoc].Price;
		}
	}
	// ASK
	dLastPrice = 0;
	for (ssize_t iLoc = _ASKQstart; iLoc != _ASKQend; iLoc=Bounded(iLoc+1))
	{
		if (dLastPrice == 0)
		{
			dLastPrice = _ASKQ[iLoc].Price;
		}
		else
		{
			if (_ASKQ[iLoc].Price <= dLastPrice) return false;
			else dLastPrice = _ASKQ[iLoc].Price;
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
	_BIDQ = new OBElement[_CIRARRSIZE];
	_ASKQ = new OBElement[_CIRARRSIZE];
}
OrderBook::~OrderBook()
{
	delete [] _BIDQ;
	delete [] _ASKQ;
}
ssize_t OrderBook::NumOfPriceLev(BASide Side) const
{
	const ssize_t & queStart = GetQueStartConst(Side); // Getting as reference
	const ssize_t & queEnd = GetQueEndConst(Side); // Getting as reference
	if (queEnd >= queStart) return queEnd-queStart;
	else return queEnd+_CIRARRSIZE-queStart;
}
OBElement* OrderBook::GetQue(BASide Side) const
{
	if (Side == BID) return _BIDQ; else return _ASKQ;
}
ssize_t & OrderBook::GetQueStart(BASide Side)
{
	if (Side == BID) return _BIDQstart; else return _ASKQstart;
}
ssize_t & OrderBook::GetQueEnd(BASide Side)
{
	if (Side == BID) return _BIDQend; else return _ASKQend;
}
const ssize_t & OrderBook::GetQueStartConst(BASide Side) const
{
	if (Side == BID) return _BIDQstart; else return _ASKQstart;
}
const ssize_t & OrderBook::GetQueEndConst(BASide Side) const
{
	if (Side == BID) return _BIDQend; else return _ASKQend;
}
bool OrderBook::Delete(
		BASide Side,
		double Price,
		ssize_t PriceLev,
		ssize_t NumOfOrd,
		ssize_t AggQty)
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

	OBElement *Que = GetQue(Side);
	ssize_t & queStart = GetQueStart(Side); // Getting as reference
	ssize_t & queEnd = GetQueEnd(Side); // Getting as reference

	ssize_t iDelLoc = Bounded(queStart+PriceLev-1);

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

	ssize_t iSearchedPxLev = LocatePriceLev(Side, Price);
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
		double Price,
		ssize_t PriceLev,
		ssize_t NumOfOrd,
		ssize_t AggQty)
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

	OBElement *Que = GetQue(Side);
	const ssize_t & queStart = GetQueStartConst(Side); // Getting as reference

	ssize_t iChgLoc = Bounded(queStart+PriceLev-1);

	// ------------------------------------------------------------
	// Normal Order Book Change
	// Check for normal cases first because they should occur with the highest freq
	// From P.40/48, "Change" can only change AggQty but not Price
	// ------------------------------------------------------------
	if (Price == Que[iChgLoc].Price)
	{
		Que[iChgLoc].AggQty = AggQty;
		Que[iChgLoc].NumOfOrd = NumOfOrd;
		return true;
	}

	// ------------------------------------------------------------
	// Lenient Treatment below
	// ------------------------------------------------------------
	ssize_t iNumOfPriceLev = NumOfPriceLev(Side);


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
	ssize_t iSearchedPxLev = LocatePriceLev(Side, Price);
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
		double Price,
		ssize_t PriceLev,
		ssize_t NumOfOrd,
		ssize_t AggQty)
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

	OBElement *Que = GetQue(Side);
	ssize_t & queStart = GetQueStart(Side); // Getting as reference
	ssize_t & queEnd = GetQueEnd(Side); // Getting as reference
	ssize_t iNumOfPriceLev = NumOfPriceLev(Side);

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
		ssize_t iSearchedPxLev = LocatePriceLev(Side, Price);

		// ------------------------------------------------------------
		// Normal insertion in the middle
		// ------------------------------------------------------------
		if (iSearchedPxLev < 0)
		{
			// Move existing entries down
			int i = queEnd;
			ssize_t iAddLoc = Bounded(queStart+(-iSearchedPxLev)-1);
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
	// ssize_t & oppQueStart = GetQueStart(sOppSide);

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
void OrderBook::DumpRaw(BASide Side) const
{
	if (Side == BID)
	{
		cout << "_BIDQstart = " << _BIDQstart << endl;
		cout << "_BIDQend = " << _BIDQend << endl;

		cout << "#Ord\tAggQty\tPrice" << endl;
		for (int i = 0; i < _CIRARRSIZE; ++i)
			cout << _BIDQ[i].NumOfOrd << "\t" << _BIDQ[i].AggQty << "\t" << _BIDQ[i].Price << "\t" << endl;
	}
	else
	{
		cout << "_ASKQstart = " << _ASKQstart << endl;
		cout << "_ASKQend = " << _ASKQend << endl;

		cout << "#Ord\tAggQty\tPrice" << endl;
		for (int i = 0; i < _CIRARRSIZE; ++i)
			cout << _ASKQ[i].NumOfOrd << "\t" << _ASKQ[i].AggQty << "\t" << _ASKQ[i].Price << "\t" << endl;
	}
}
void OrderBook::Dump(BASide Side) const
{
	if (Side == BID)
	{
		cout << "_BIDQstart = " << _BIDQstart << endl;
		cout << "_BIDQend = " << _BIDQend << endl;

		cout << "#Ord\tAggQty\tPrice" << endl;
		for (int i = _BIDQstart; i != _BIDQend; i = Bounded(i+1))
			cout << _BIDQ[i].NumOfOrd << "\t" << _BIDQ[i].AggQty << "\t" << _BIDQ[i].Price << "\t" << endl;
	}
	else
	{
		cout << "_ASKQstart = " << _ASKQstart << endl;
		cout << "_ASKQend = " << _ASKQend << endl;

		cout << "#Ord\tAggQty\tPrice" << endl;
		for (int i = _ASKQstart; i != _ASKQend; i = Bounded(i+1))
			cout << _ASKQ[i].NumOfOrd << "\t" << _ASKQ[i].AggQty << "\t" << _ASKQ[i].Price << "\t" << endl;
	}
}
void OrderBook::Dump(BASide Side, int iOut[10][3]) const
{
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 3; ++j)
			iOut[i][j] = 0;

	if (Side == BID)
	{
		if (_BIDQstart == _BIDQend) return;

		int j = 0;
		for (int i = _BIDQstart; i != _BIDQend; i = Bounded(i+1), ++j)
		{
			iOut[j][0] = _BIDQ[i].NumOfOrd;
			iOut[j][1] = _BIDQ[i].AggQty;
			iOut[j][2] = _BIDQ[i].Price;
		}
	}
	else
	{
		if (_ASKQstart == _ASKQend) return;

		int j = 0;
		for (int i = _ASKQstart; i != _ASKQend; i = Bounded(i+1), ++j)
		{
			iOut[j][0] = _ASKQ[i].NumOfOrd;
			iOut[j][1] = _ASKQ[i].AggQty;
			iOut[j][2] = _ASKQ[i].Price;
		}
	}
}
#endif
