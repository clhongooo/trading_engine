#include "UTest.h"
#include "Util/OrderBook.h"
#include <iostream>
#include <string>

bool Check(
		int iaBid[10][3],
		int iaAsk[10][3],
		int iaBidAns[10][3],
		int iaAskAns[10][3])
{
	bool bRtn = true;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (iaBid[i][j] != iaBidAns[i][j]) bRtn = false;
			if (iaAsk[i][j] != iaAskAns[i][j]) bRtn = false;
		}
	}
	return bRtn;
}


int TestOBS()
{
	UTest ut;

	int iaBid[10][3];
	int iaAsk[10][3];
	OrderBook osob;



	// -------------------------------------
	// LocatePriceLev: Empty queues
	// -------------------------------------
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9750) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9740) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9640) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9630) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9750) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9740) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9640) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9630) == -1, __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Roll the circular array different start/end points and Test LocatePriceLev()
	// -------------------------------------
	for (int i = 0; i < 48; ++i)
	{
		osob.Reset();

		ut.Assert(osob.Add(OrderBook::BID, 9900-i*10, 1, 1, 300), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.Add(OrderBook::BID, 9900-i*10+5, 1, 1, 300), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.Delete(OrderBook::BID, 9900-i*10, 2, 1, 300), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.Delete(OrderBook::BID, 9900-i*10+5, 1, 1, 300), __FILE__,__FUNCTION__,__LINE__);

		ut.Assert(osob.Add(OrderBook::ASK, 9900+i*10, 1, 1, 300), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.Delete(OrderBook::ASK, 9900+i*10, 1, 1, 300), __FILE__,__FUNCTION__,__LINE__);

		// -------------------------------------
		// LocatePriceLev: 3 Price Levels
		// -------------------------------------
		ut.Assert(osob.Add(OrderBook::BID, 34650, 1, 1, 600000), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.Add(OrderBook::BID, 34550, 2, 1, 55000), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.Add(OrderBook::BID, 34600, 2, 1, 725000), __FILE__,__FUNCTION__,__LINE__); // Insert 2nd


		ut.Assert(osob.LocatePriceLev(OrderBook::BID, 34700) == -1, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.LocatePriceLev(OrderBook::BID, 34650) == 1, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.LocatePriceLev(OrderBook::BID, 34620) == -2, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.LocatePriceLev(OrderBook::BID, 34600) == 2, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.LocatePriceLev(OrderBook::BID, 34570) == -3, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.LocatePriceLev(OrderBook::BID, 34550) == 3, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(osob.LocatePriceLev(OrderBook::BID, 34500) == -4, __FILE__,__FUNCTION__,__LINE__);
	}



	// -------------------------------------
	// Setting up the Initial Order Book
	// -------------------------------------
	osob.Reset();
	ut.Assert(osob.Add(OrderBook::BID, 9730, 1, 1, 700), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9720, 2, 1, 350), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9710, 3, 1, 150), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9700, 4, 1, 250), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9690, 5, 1, 100), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9680, 6, 1, 150), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9670, 7, 1,  50), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9660, 8, 1, 200), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9650, 9, 1, 100), __FILE__,__FUNCTION__,__LINE__);

	ut.Assert(osob.Add(OrderBook::ASK, 9760, 1, 1, 500), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 9770, 2, 1, 300), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 9780, 3, 1, 100), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 9790, 4, 1, 150), __FILE__,__FUNCTION__,__LINE__);

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid1 [10][3] = {
		{1,700,9730}
		,{1,350,9720}
		,{1,150,9710}
		,{1,250,9700}
		,{1,100,9690}
		,{1,150,9680}
		,{1, 50,9670}
		,{1,200,9660}
		,{1,100,9650}
		,{0,  0,   0}
	};
	int iaAsk1 [10][3] = {
		{1,500,9760}
		,{1,300,9770}
		,{1,100,9780}
		,{1,150,9790}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid1, iaAsk1), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// LocatePriceLev: 9 and 4 Price Levels
	// -------------------------------------
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9730) == 1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9720) == 2, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9710) == 3, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9700) == 4, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9690) == 5, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9680) == 6, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9670) == 7, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9660) == 8, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9650) == 9, __FILE__,__FUNCTION__,__LINE__);


	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9750) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9740) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9735) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9725) == -2, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9715) == -3, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9705) == -4, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9695) == -5, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9685) == -6, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9675) == -7, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9665) == -8, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9655) == -9, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9640) == -10, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::BID, 9630) == -10, __FILE__,__FUNCTION__,__LINE__);


	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9760) == 1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9770) == 2, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9780) == 3, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9790) == 4, __FILE__,__FUNCTION__,__LINE__);

	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9755) == -1, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9765) == -2, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9775) == -3, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9785) == -4, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9795) == -5, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.LocatePriceLev(OrderBook::ASK, 9800) == -5, __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Example 1 P.40/48
	// -------------------------------------
	ut.Assert(osob.Change(OrderBook::ASK, 9770, 2, 1, 200), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 9850, 5, 1, 300), __FILE__,__FUNCTION__,__LINE__);

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid2 [10][3] = {
		{1,700,9730}
		,{1,350,9720}
		,{1,150,9710}
		,{1,250,9700}
		,{1,100,9690}
		,{1,150,9680}
		,{1, 50,9670}
		,{1,200,9660}
		,{1,100,9650}
		,{0,  0,   0}
	};
	int iaAsk2 [10][3] = {
		{1,500,9760}
		,{1,200,9770}
		,{1,100,9780}
		,{1,150,9790}
		,{1,300,9850} // We won't store empty rows
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid2, iaAsk2), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Example 2 P.41/48
	// -------------------------------------
	ut.Assert(osob.Add(OrderBook::BID, 9740, 1, 1, 50), __FILE__,__FUNCTION__,__LINE__);

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid3 [10][3] = {
		{1, 50,9740}
		,{1,700,9730}
		,{1,350,9720}
		,{1,150,9710}
		,{1,250,9700}
		,{1,100,9690}
		,{1,150,9680}
		,{1, 50,9670}
		,{1,200,9660}
		,{1,100,9650}
	};
	int iaAsk3 [10][3] = {
		{1,500,9760}
		,{1,200,9770}
		,{1,100,9780}
		,{1,150,9790}
		,{1,300,9850} // We won't store empty rows
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid3, iaAsk3), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Example 3 P.41/48
	// -------------------------------------
	ut.Assert(osob.Add(OrderBook::BID, 9750, 1, 1, 250), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 9660, 10, 1, 150), __FILE__,__FUNCTION__,__LINE__);

	//osob.Dump(OrderBook::BID);
	//osob.Dump(OrderBook::ASK);

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid4 [10][3] = {
		{1,250,9750}
		,{1, 50,9740}
		,{1,700,9730}
		,{1,350,9720}
		,{1,150,9710}
		,{1,250,9700}
		,{1,100,9690}
		,{1,150,9680}
		,{1, 50,9670}
		,{1,150,9660}
	};
	int iaAsk4 [10][3] = {
		{1,500,9760}
		,{1,200,9770}
		,{1,100,9780}
		,{1,150,9790}
		,{1,300,9850} // We won't store empty rows
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid4, iaAsk4), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Example 4 P.42/48
	// -------------------------------------
	ut.Assert(osob.Delete(OrderBook::BID, 9750, 1, 1, 250), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9650, 10, 1, 100), __FILE__,__FUNCTION__,__LINE__);

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid5 [10][3] = {
		{1, 50,9740}
		,{1,700,9730}
		,{1,350,9720}
		,{1,150,9710}
		,{1,250,9700}
		,{1,100,9690}
		,{1,150,9680}
		,{1, 50,9670}
		,{1,150,9660}
		,{1,100,9650}
	};
	int iaAsk5 [10][3] = {
		{1,500,9760}
		,{1,200,9770}
		,{1,100,9780}
		,{1,150,9790}
		,{1,300,9850} // We won't store empty rows
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid5, iaAsk5), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Example 5 P.43/48
	// -------------------------------------
	ut.Assert(osob.Add(OrderBook::ASK, 9750, 1, 1, 300), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Delete(OrderBook::ASK, 9850, 6, 1, 300), __FILE__,__FUNCTION__,__LINE__);

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid6 [10][3] = {
		{1, 50,9740}
		,{1,700,9730}
		,{1,350,9720}
		,{1,150,9710}
		,{1,250,9700}
		,{1,100,9690}
		,{1,150,9680}
		,{1, 50,9670}
		,{1,150,9660}
		,{1,100,9650}
	};
	int iaAsk6 [10][3] = {
		{1,300,9750}
		,{1,500,9760}
		,{1,200,9770}
		,{1,100,9780}
		,{1,150,9790}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid6, iaAsk6), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Explicit Deletions vs Implicit Deletions P.44/48
	// -------------------------------------

	// Reset Bid Queue
	osob.Reset(OrderBook::BID);

	// Build initial order book
	ut.Assert(osob.Add(OrderBook::BID, 9800, 1, 1, 700), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9790, 2, 1, 350), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9780, 3, 1, 150), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9760, 4, 1, 250), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9750, 5, 1, 100), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9730, 6, 1, 400), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9720, 7, 1, 200), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9710, 8, 1, 300), __FILE__,__FUNCTION__,__LINE__);
	//osob.Dump(OrderBook::BID);
	//osob.Dump(OrderBook::ASK);

	// Then make changes
	ut.Assert(osob.Add(OrderBook::BID, 9860, 1, 1, 450), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9850, 2, 1, 550), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 9840, 3, 1, 650), __FILE__,__FUNCTION__,__LINE__);
	//osob.Dump(OrderBook::BID);
	//osob.Dump(OrderBook::ASK);

	ut.Assert(osob.Delete(OrderBook::BID, 9760, 7, 1, 250), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Delete(OrderBook::BID, 9750, 7, 1, 100), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Delete(OrderBook::BID, 9730, 7, 1, 400), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Delete(OrderBook::BID, 9720, 7, 1, 200), __FILE__,__FUNCTION__,__LINE__);
	//osob.Dump(OrderBook::BID);
	//osob.Dump(OrderBook::ASK);

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid7 [10][3] = {
		{1,450,9860}
		,{1,550,9850}
		,{1,650,9840}
		,{1,700,9800}
		,{1,350,9790}
		,{1,150,9780}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};
	int iaAsk7 [10][3] = {
		 {1,300,9750}
		,{1,500,9760}
		,{1,200,9770}
		,{1,100,9780}
		,{1,150,9790}
		//{0,  0,   0} // Crossing detected, implicit deletion
		//,{0,  0,   0}
		//,{0,  0,   0}
		//,{0,  0,   0}
		//,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid7, iaAsk7), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Example 6: P.46/48
	// -------------------------------------
	osob.Reset();

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid8 [10][3] = {
		{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};
	int iaAsk8 [10][3] = {
		{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid8, iaAsk8), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// My Own Test Cases
	// Focusing on tesing ASK
	// -------------------------------------
	osob.Reset();

	// -------------------------------------
	// Add(): Various normal cases
	// -------------------------------------
	ut.Assert(osob.Add(OrderBook::ASK, 9730, 1, 1, 700), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 9720, 1, 1, 600), __FILE__,__FUNCTION__,__LINE__); // Immediately before head
	ut.Assert(osob.Add(OrderBook::ASK, 9680, 1, 1, 500), __FILE__,__FUNCTION__,__LINE__); // 4 Levels before head
	ut.Assert(osob.Add(OrderBook::ASK, 9760, 4, 1, 800), __FILE__,__FUNCTION__,__LINE__); // 3 Levels after tail
	ut.Assert(osob.Add(OrderBook::ASK, 9790, 5, 1, 100), __FILE__,__FUNCTION__,__LINE__); // 3 Levels after tail
	ut.Assert(osob.Add(OrderBook::ASK, 9780, 5, 1, 900), __FILE__,__FUNCTION__,__LINE__); // Insert at 2nd last
	ut.Assert(osob.Add(OrderBook::ASK, 9800, 7, 1, 200), __FILE__,__FUNCTION__,__LINE__); // Immediately after tail
	ut.Assert(osob.Add(OrderBook::ASK, 9740, 4, 1, 200), __FILE__,__FUNCTION__,__LINE__); // Insert at middle, Price Level 4

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid9 [10][3] = {
		{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};
	int iaAsk9 [10][3] = {
		{1,500,9680}
		,{1,600,9720}
		,{1,700,9730}
		,{1,200,9740}
		,{1,800,9760}
		,{1,900,9780}
		,{1,100,9790}
		,{1,200,9800}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid9, iaAsk9), __FILE__,__FUNCTION__,__LINE__);



	// -------------------------------------
	// Setting up for second round testing
	// -------------------------------------

	// To remove additional item
	ut.Assert(osob.Delete(OrderBook::ASK, 9740, 4, 1, 200), __FILE__,__FUNCTION__,__LINE__); // Remove our additional 9740
	// To fully stuff the internal buffer
	ut.Assert(osob.Add(OrderBook::ASK, 9810, 8, 1, 168), __FILE__,__FUNCTION__,__LINE__); // Dummy
	ut.Assert(osob.Add(OrderBook::ASK, 9820, 9, 1, 168), __FILE__,__FUNCTION__,__LINE__); // Dummy
	ut.Assert(osob.Add(OrderBook::ASK, 9890, 10, 1, 168), __FILE__,__FUNCTION__,__LINE__); // Dummy
	ut.Assert(osob.Add(OrderBook::ASK, 9880, 10, 1, 168), __FILE__,__FUNCTION__,__LINE__); // Push the 9890 out
	ut.Assert(osob.Add(OrderBook::ASK, 9870, 10, 1, 168), __FILE__,__FUNCTION__,__LINE__); // Push the 9890 out
	ut.Assert(osob.Add(OrderBook::ASK, 9860, 10, 1, 168), __FILE__,__FUNCTION__,__LINE__); // Push the 9890 out

	// Restore to original state
	ut.Assert(osob.Delete(OrderBook::ASK, 9860, 10, 1, 168), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Delete(OrderBook::ASK, 9820, 9, 1, 168), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Delete(OrderBook::ASK, 9810, 8, 1, 168), __FILE__,__FUNCTION__,__LINE__);

	// -------------------------------------
	// Try its error catching and lenient handling ability!!
	// -------------------------------------
	// Add()
	// -------------------------------------
	ut.Assert(osob.Add(OrderBook::ASK, 9680, 1, 1, 501), __FILE__,__FUNCTION__,__LINE__); // Clashing with existing head, implicit conversion to Change()
	ut.Assert(osob.Add(OrderBook::ASK, 9760, 4, 1, 801), __FILE__,__FUNCTION__,__LINE__); // Clashing with existing mid-pt, implicit conversion to Change()
	ut.Assert(osob.Add(OrderBook::ASK, 9800, 7, 1, 201), __FILE__,__FUNCTION__,__LINE__); // Clashing with existing tail, implicit conversion to Change()

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid10 [10][3] = {
		{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};
	int iaAsk10 [10][3] = {
		{1,501,9680}
		,{1,600,9720}
		,{1,700,9730}
		,{1,801,9760}
		,{1,900,9780}
		,{1,100,9790}
		,{1,201,9800}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid10, iaAsk10), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Add()
	// -------------------------------------
	ut.Assert(osob.Add(OrderBook::ASK, 9680, 2, 1, 502), __FILE__,__FUNCTION__,__LINE__); // Clashing with existing head, but price level wrong, still implicit conversion to Change()
	ut.Assert(osob.Add(OrderBook::ASK, 9760, 3, 1, 802), __FILE__,__FUNCTION__,__LINE__); // Clashing with existing mid-pt, but price level wrong, still implicit conversion to Change()
	ut.Assert(osob.Add(OrderBook::ASK, 9800, 9, 1, 202), __FILE__,__FUNCTION__,__LINE__); // Clashing with existing tail, but price level wrong, still implicit conversion to Change()

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid11 [10][3] = {
		{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};
	int iaAsk11 [10][3] = {
		{1,502,9680}
		,{1,600,9720}
		,{1,700,9730}
		,{1,802,9760}
		,{1,900,9780}
		,{1,100,9790}
		,{1,202,9800}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid11, iaAsk11), __FILE__,__FUNCTION__,__LINE__);



	// -------------------------------------
	// Add()
	// -------------------------------------
	ut.Assert(osob.Add(OrderBook::ASK, 9900, 1, 1, 990), __FILE__,__FUNCTION__,__LINE__); // Way high and way low prices with incorrect price levels, still insert using abs price
	ut.Assert(osob.Add(OrderBook::ASK, 1900, 10, 1, 190), __FILE__,__FUNCTION__,__LINE__); // Way high and way low prices with incorrect price levels, still insert using abs price

	ut.AssertF(osob.Add(OrderBook::ASK, 9999, 30, 1, 190), __FILE__,__FUNCTION__,__LINE__); // Price Level too high, reject
	ut.AssertF(osob.Add(OrderBook::ASK, -1, 3, 1, 190), __FILE__,__FUNCTION__,__LINE__); // Price can't be negative
	//ut.AssertF(osob.Add(OrderBook::ASK, 9790, 7, -1, 190), __FILE__,__FUNCTION__,__LINE__); // NumOfOrd can't be negative
	//ut.AssertF(osob.Add(OrderBook::ASK, 9790, 7, 1, -190), __FILE__,__FUNCTION__,__LINE__); // AggQty can't be negative


	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid12 [10][3] = {
		{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};
	int iaAsk12 [10][3] = {
		{1,190,1900}
		,{1,502,9680}
		,{1,600,9720}
		,{1,700,9730}
		,{1,802,9760}
		,{1,900,9780}
		,{1,100,9790}
		,{1,202,9800}
		,{1,990,9900}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid12, iaAsk12), __FILE__,__FUNCTION__,__LINE__);


	// -------------------------------------
	// Change()
	// -------------------------------------
	// Clean up a bit first
	ut.Assert(osob.Delete(OrderBook::ASK, 1900, 1, 1, 190), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Delete(OrderBook::ASK, 9680, 1, 1, 502), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Delete(OrderBook::ASK, 9900, 7, 1, 990), __FILE__,__FUNCTION__,__LINE__);


	ut.Assert(osob.Change(OrderBook::ASK, 9190, 4, 1, 400), __FILE__,__FUNCTION__,__LINE__); // Price at Price Level 4 is wrong, and Price 9190 is non-existent. Implicit conversion to Add()
	ut.Assert(osob.Change(OrderBook::ASK, 9760, 5, 1, 803), __FILE__,__FUNCTION__,__LINE__); // Price at Price Level 5 is wrong, but handle leniently - update Price Level 4
	ut.Assert(osob.Change(OrderBook::ASK, 9800, 7, 1, 203), __FILE__,__FUNCTION__,__LINE__); // This is correct, handle normally
	ut.Assert(osob.Change(OrderBook::ASK, 9810, 3, 1, 300), __FILE__,__FUNCTION__,__LINE__); // Price non-existent, implicit conversion to Add()

	//ut.AssertF(osob.Change(OrderBook::ASK, 9680, 3, 1, -345), __FILE__,__FUNCTION__,__LINE__); // AggQty can't be -ve.
	//ut.AssertF(osob.Change(OrderBook::ASK, 9680, 3, -1, 345), __FILE__,__FUNCTION__,__LINE__); // NumOfOrd can't be -ve.
	//ut.AssertF(osob.Change(OrderBook::ASK, 9680, -3, 1, 345), __FILE__,__FUNCTION__,__LINE__); // Price Level can't be negative


	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid13 [10][3] = {
		{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};
	int iaAsk13 [10][3] = {
		{1,400,9190}
		,{1,600,9720}
		,{1,700,9730}
		,{1,803,9760}
		,{1,900,9780}
		,{1,100,9790}
		,{1,203,9800}
		,{1,300,9810}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid13, iaAsk13), __FILE__,__FUNCTION__,__LINE__);





	// -------------------------------------
	// Delete()
	// -------------------------------------

	ut.Assert(osob.Delete(OrderBook::ASK, 9190, 4, 1, 345), __FILE__,__FUNCTION__,__LINE__); // Price Level is wrong, AggQty is also wrong, use absolute price
	ut.Assert(osob.Delete(OrderBook::ASK, 9760, 3, 1, 345), __FILE__,__FUNCTION__,__LINE__); // Price Level right, but AggQty wrong, delete 9760
	ut.Assert(osob.Delete(OrderBook::ASK, 9790, 9, 1, 100), __FILE__,__FUNCTION__,__LINE__); // Price Level  wrong, Price right, AggQty right, delete 9790

	ut.AssertF(osob.Delete(OrderBook::ASK, 9999, 1, 1, 345), __FILE__,__FUNCTION__,__LINE__); // Nothing is wring, including the Price, return error and do nothing
	//ut.AssertF(osob.Delete(OrderBook::ASK, 9800, -1, 1, 345), __FILE__,__FUNCTION__,__LINE__); // Price Level can't be negative
	//ut.AssertF(osob.Delete(OrderBook::ASK, 9800, 1, 1, -345), __FILE__,__FUNCTION__,__LINE__); // AggQty can't be negative
	//ut.AssertF(osob.Delete(OrderBook::ASK, 9800, 1, -2, 500), __FILE__,__FUNCTION__,__LINE__); // NumOfOrd can't be negative

	osob.Dump(OrderBook::BID,iaBid);
	osob.Dump(OrderBook::ASK,iaAsk);

	int iaBid14 [10][3] = {
		{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};
	int iaAsk14 [10][3] = {
		{1,600,9720}
		,{1,700,9730}
		,{1,900,9780}
		,{1,203,9800}
		,{1,300,9810}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
		,{0,  0,   0}
	};

	ut.Assert(Check(iaBid, iaAsk, iaBid14, iaAsk14), __FILE__,__FUNCTION__,__LINE__);

	//Test memory leak
	for (long i = 0; i < 10; i++)
	{
		OrderBook *d1 = new OrderBook();
		delete d1;

		OrderBook d2;
	}


	//Data from HKEx simulator for OMD
	//Security Code 50
	osob.Reset();
	ut.Assert(osob.Add(OrderBook::BID, 5080, 1, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5060, 2, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 3, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 6, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5090, 2, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 7, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 4, 100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5070, 2, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 5, 115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 6, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 7, 155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 8, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 9, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 8, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 9, 255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 2, 2, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 10, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 10, 240000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 11, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 12, 305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 13, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 12, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 13, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 15, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 14, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 16, 410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 17, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 18, 460000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 19, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 15, 445000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 3, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 20, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 18, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 18, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 19, 525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 19, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 20, 560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 18, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 19, 545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 24, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 20, 585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 25, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 21, 620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 26, 705000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 4, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 3, 5, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5100, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 24, 645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 25, 695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 26, 730000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 27, 745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 26, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 28, 770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 29, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 30, 830000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 31, 860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 27, 755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 28, 775000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 29, 800000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 30, 820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 31, 850000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 2, 5, 145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 4, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 33, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 5, 125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 34, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 32, 870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 35, 940000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 36, 955000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 37, 985000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 38, 1020000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 33, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 39, 1025000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 40, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 41, 1055000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 43, 1110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 44, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 45, 1180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 34, 925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 46, 1235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 35, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 47, 1260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 6, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 48, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 37, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 42, 1125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 43, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 38, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 39, 1045000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 40, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 41, 1115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 42, 1155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5110, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 43, 1185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 14, 350000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 15, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 46, 1205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 44, 1200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 47, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 45, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 48, 1235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 49, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 46, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 16, 385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 50, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 17, 410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 47, 1270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 51, 1320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 48, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 52, 1355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 49, 1315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 50, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 53, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 51, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 52, 1385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 54, 1440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 55, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 18, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 53, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 54, 1430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 56, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 57, 1525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 55, 1465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 56, 1495000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 57, 1515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 58, 1560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 58, 1560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 59, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 60, 1630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 59, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 61, 1645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 60, 1595000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 62, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 63, 1700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 64, 1720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 19, 450000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 20, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 65, 1750000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 21, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 66, 1770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 67, 1795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 68, 1830000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 59, 1645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 60, 1670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 61, 1690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 64, 1695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 62, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 63, 1760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 64, 1780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 65, 1805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 66, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 67, 1855000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 68, 1860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 65, 1735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 69, 1885000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 70, 1905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 2, 12, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 71, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5120, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 72, 1955000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 66, 1755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 73, 1990000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 74, 2015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5120, 2, 2, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 75, 2025000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 76, 2040000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 77, 2065000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 67, 1820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 68, 1845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 69, 1860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 78, 2080000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 70, 1880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5080, 1, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5060, 2, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 3, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 6, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5090, 2, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 7, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 4, 100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5070, 2, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 5, 115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 6, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 7, 155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 8, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 9, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 8, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 9, 255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 2, 2, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 10, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 10, 240000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 11, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 12, 305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 13, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 12, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 13, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 15, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 14, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 16, 410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 17, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 18, 460000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 19, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 15, 445000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 3, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 20, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 18, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 18, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 19, 525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 19, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 20, 560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 18, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 19, 545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 24, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 20, 585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 25, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 21, 620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 26, 705000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 4, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 3, 5, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5100, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 24, 645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 25, 695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 26, 730000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 27, 745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 26, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 28, 770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 29, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 30, 830000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 31, 860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 27, 755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 28, 775000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 29, 800000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 30, 820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 31, 850000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 2, 5, 145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 4, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 33, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 5, 125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 34, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 32, 870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 35, 940000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 36, 955000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 37, 985000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 38, 1020000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 33, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 39, 1025000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 40, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 41, 1055000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 43, 1110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 44, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 45, 1180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 34, 925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 46, 1235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 35, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 47, 1260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 6, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 48, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 37, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 42, 1125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 43, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 38, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 39, 1045000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 40, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 41, 1115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 42, 1155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5110, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 43, 1185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 14, 350000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 15, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 46, 1205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 44, 1200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 47, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 45, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 48, 1235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 49, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 46, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 16, 385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 50, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 17, 410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 47, 1270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 51, 1320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 48, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 52, 1355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 49, 1315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 50, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 53, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 51, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 52, 1385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 54, 1440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 55, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 18, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 53, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 54, 1430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 56, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 57, 1525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 55, 1465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 56, 1495000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 57, 1515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 58, 1560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 58, 1560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 59, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 60, 1630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 59, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 61, 1645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 60, 1595000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 62, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 63, 1700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 64, 1720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 19, 450000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 20, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 65, 1750000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 21, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 66, 1770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 67, 1795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 68, 1830000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 59, 1645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 60, 1670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 61, 1690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 64, 1695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 62, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 63, 1760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 64, 1780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 65, 1805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 66, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 67, 1855000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 68, 1860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 65, 1735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 69, 1885000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 70, 1905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 2, 12, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 71, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5120, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 72, 1955000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 66, 1755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 73, 1990000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 74, 2015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5120, 2, 2, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 75, 2025000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 76, 2040000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 77, 2065000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 67, 1820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 68, 1845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 69, 1860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 78, 2080000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 70, 1880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5080, 1, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5060, 2, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 3, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 6, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5090, 2, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 7, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 4, 100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5070, 2, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 5, 115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 6, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 7, 155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 8, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 9, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 8, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 9, 255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 2, 2, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 10, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 10, 240000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 11, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 12, 305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 13, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 14, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 13, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 12, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 13, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 15, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 14, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 16, 410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 17, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 18, 460000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 19, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 15, 445000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 3, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 20, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 21, 560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 18, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 18, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 19, 525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 19, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 20, 560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 21, 580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 18, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 19, 545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 24, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 20, 585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 25, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 21, 620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 26, 705000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 4, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 27, 740000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 3, 5, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5100, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 24, 645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 25, 695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 26, 730000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 27, 745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 26, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 28, 770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 29, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 30, 830000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 31, 860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 27, 755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 28, 775000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 29, 800000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 30, 820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 31, 850000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 2, 5, 145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 4, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 33, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 5, 125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 34, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 32, 870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 35, 940000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 36, 955000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 37, 985000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 38, 1020000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 33, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 39, 1025000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 40, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 41, 1055000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 43, 1110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 44, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 45, 1180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 34, 925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 46, 1235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 35, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 47, 1260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 6, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 48, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 49, 1315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 37, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 42, 1125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 43, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 38, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 44, 1175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 39, 1045000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 52, 1390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 40, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 41, 1115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 42, 1155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5110, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 43, 1185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 14, 350000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 15, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 46, 1205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 44, 1200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 47, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 45, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 48, 1235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 49, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 46, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 16, 385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 50, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 17, 410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 47, 1270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 51, 1320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 48, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 52, 1355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 49, 1315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 50, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 53, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 51, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 52, 1385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 54, 1440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 55, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 18, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 53, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 54, 1430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 56, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 57, 1525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 55, 1465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 56, 1495000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 57, 1515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 58, 1560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 58, 1560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 59, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 60, 1630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 59, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 61, 1645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 60, 1595000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 62, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 63, 1700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 64, 1720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 19, 450000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 20, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 65, 1750000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 21, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 66, 1770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 67, 1795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 68, 1830000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 69, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 59, 1645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 60, 1670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 61, 1690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 64, 1695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 62, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 63, 1760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 64, 1780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 65, 1805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 66, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 67, 1855000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 68, 1860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 65, 1735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 69, 1885000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 70, 1905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 2, 12, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 71, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5120, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 72, 1955000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 66, 1755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 73, 1990000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 74, 2015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5120, 2, 2, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 75, 2025000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 76, 2040000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 77, 2065000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 67, 1820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 68, 1845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 69, 1860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 78, 2080000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 70, 1880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5080, 1, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5060, 2, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 3, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 6, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5090, 2, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 7, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 4, 100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5070, 2, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 5, 115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 6, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 7, 155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 8, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 9, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 8, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 9, 255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 2, 2, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 10, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 10, 240000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 11, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 12, 305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 13, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 14, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 13, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 12, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 13, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 15, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 14, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 16, 410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 17, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 18, 460000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 19, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 15, 445000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 3, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 20, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 21, 560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 18, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 18, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 19, 525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 19, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 20, 560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 21, 580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 18, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 19, 545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 24, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 20, 585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 25, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5080, 1, 21, 620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 26, 705000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 2, 4, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 1, 27, 740000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5070, 3, 5, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5100, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 24, 645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 25, 695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 26, 730000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 27, 745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 26, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 28, 770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 29, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 30, 830000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 31, 860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 27, 755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 28, 775000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 29, 800000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 30, 820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 31, 850000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5080, 2, 5, 145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 4, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 33, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 5, 125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 34, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 32, 870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 35, 940000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 36, 955000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 37, 985000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 38, 1020000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 33, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 39, 1025000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 40, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 41, 1055000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 43, 1110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 44, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 45, 1180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 34, 925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 46, 1235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 35, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 47, 1260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 2, 6, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 48, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 49, 1315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 37, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 42, 1125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 43, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 38, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 44, 1175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5090, 1, 39, 1045000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 1, 52, 1390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 40, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 41, 1115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 42, 1155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5110, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 43, 1185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 14, 350000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 15, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 46, 1205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 44, 1200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 47, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 45, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 48, 1235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 49, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 46, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 16, 385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 50, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 2, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 17, 410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 47, 1270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 51, 1320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 48, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 52, 1355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 49, 1315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 50, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 53, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 51, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 52, 1385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 54, 1440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 55, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 18, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 53, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 54, 1430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 56, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 57, 1525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 55, 1465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 56, 1495000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 57, 1515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 58, 1560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 58, 1560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 59, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 60, 1630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 59, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 61, 1645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5100, 1, 60, 1595000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 62, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 63, 1700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 64, 1720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 19, 450000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 20, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 65, 1750000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5090, 2, 21, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 66, 1770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 67, 1795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 68, 1830000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 1, 69, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 59, 1645000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 60, 1670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 61, 1690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 64, 1695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 62, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 63, 1760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 64, 1780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 65, 1805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 66, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 67, 1855000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 68, 1860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 65, 1735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 69, 1885000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 70, 1905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5100, 2, 12, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 71, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5120, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 72, 1955000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 66, 1755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 73, 1990000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 74, 2015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5120, 2, 2, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 75, 2025000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 76, 2040000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 77, 2065000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 67, 1820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 68, 1845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 69, 1860000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5110, 1, 78, 2080000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 5110, 1, 70, 1880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);

	cout << endl << endl << "Security code = 50" << endl;
	cout << osob.Dump(OrderBook::BID);
	cout << osob.Dump(OrderBook::ASK);



	//Security Code 48
	osob.Reset();

	ut.Assert(osob.Add(OrderBook::BID, 34700, 1, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 34750, 1, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 10, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 11, 235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 12, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 11, 255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 12, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 13, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 13, 285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 14, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 15, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 14, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 16, 355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 15, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 16, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 16, 315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 17, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 20, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 20, 435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 21, 455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 22, 465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 21, 440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 23, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 22, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34600, 2, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 23, 505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 2, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 24, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 4, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 25, 555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 24, 540000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 3, 2, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 3, 3, 35000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 27, 585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 3, 45000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 29, 620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 30, 635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 32, 680000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 2, 4, 55000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 33, 700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 31, 655000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 34, 715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 32, 720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34550, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 7, 130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 34, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 36, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 35, 755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 36, 780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 37, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 37, 865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 5, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 38, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 4, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 38, 835000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 6, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 39, 920000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 39, 855000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 40, 945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 41, 970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 5, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 42, 980000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 43, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 40, 870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 46, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 37, 910000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 38, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 47, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 39, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 40, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 9, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 48, 1045000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 41, 975000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 49, 1065000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 50, 1075000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 42, 1010000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 43, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 51, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 10, 200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 44, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34550, 2, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 11, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 52, 1120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 45, 1080000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 53, 1145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 54, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 46, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 55, 1190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 47, 1120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 56, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 48, 1130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 57, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 58, 1265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 60, 1370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 51, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 13, 265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 53, 1250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 10, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 65, 1480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 66, 1520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 54, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 55, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 7, 125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 56, 1290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 11, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 57, 1305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 67, 1540000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 68, 1570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 56, 1330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 58, 1385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 59, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 60, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 15, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34550, 2, 3, 55000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 70, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 71, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 72, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 73, 1615000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 61, 1455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 74, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 16, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 62, 1480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 75, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 13, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 65, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 8, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 66, 1555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 79, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 80, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 9, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 10, 170000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 14, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 67, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 81, 1885000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 67, 1620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34700, 1, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 34750, 1, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 10, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 11, 235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 12, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 11, 255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 12, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 13, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 13, 285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 14, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 15, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 14, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 16, 355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 15, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 16, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 16, 315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 17, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 20, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 20, 435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 21, 455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 22, 465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 21, 440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 23, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 22, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34600, 2, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 23, 505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 2, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 24, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 4, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 25, 555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 24, 540000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 3, 2, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 3, 3, 35000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 27, 585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 3, 45000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 29, 620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 30, 635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 32, 680000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 2, 4, 55000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 33, 700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 31, 655000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 34, 715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 32, 720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34550, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 7, 130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 34, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 36, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 35, 755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 36, 780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 37, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 37, 865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 5, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 38, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 4, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 38, 835000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 6, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 39, 920000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 39, 855000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 40, 945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 41, 970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 5, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 42, 980000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 43, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 40, 870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 46, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 37, 910000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 38, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 47, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 39, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 40, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 9, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 48, 1045000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 41, 975000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 49, 1065000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 50, 1075000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 42, 1010000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 43, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 51, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 10, 200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 44, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34550, 2, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 11, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 52, 1120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 45, 1080000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 53, 1145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 54, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 46, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 55, 1190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 47, 1120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 56, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 48, 1130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 57, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 58, 1265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 60, 1370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 51, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 13, 265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 53, 1250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 10, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 65, 1480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 66, 1520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 54, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 55, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 7, 125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 56, 1290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 11, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 57, 1305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 67, 1540000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 68, 1570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 56, 1330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 58, 1385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 59, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 60, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 15, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34550, 2, 3, 55000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 70, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 71, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 72, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 73, 1615000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 61, 1455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 74, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 16, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 62, 1480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 75, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 13, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 65, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 8, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 66, 1555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 79, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 80, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 9, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 10, 170000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 14, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 67, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 81, 1885000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 67, 1620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34700, 1, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 34750, 1, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 10, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 11, 235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 12, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 11, 255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 12, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 13, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 13, 285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 14, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 15, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 14, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 16, 355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 17, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 15, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 16, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 16, 315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 17, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 18, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 19, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 19, 395000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 20, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 20, 435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 21, 455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 22, 465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 21, 440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 23, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 24, 515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 22, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 22, 480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34600, 2, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 23, 505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 26, 570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 2, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 24, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 4, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 25, 555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 24, 540000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 25, 560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 30, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 26, 575000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 31, 665000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 3, 2, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 3, 3, 35000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 27, 585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 28, 600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 3, 45000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 29, 620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 30, 635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 32, 680000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 2, 4, 55000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 33, 700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 31, 655000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 34, 715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 35, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 32, 720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 33, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 35, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 34, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 7, 130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 36, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 34, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 36, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 35, 755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 36, 780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 37, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 37, 865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 5, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 38, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 4, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 38, 835000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 6, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 39, 920000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 39, 855000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 40, 945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 41, 970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 5, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 42, 980000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 43, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 40, 870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 44, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 46, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 37, 910000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 38, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 47, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 39, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 40, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 9, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 48, 1045000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 41, 975000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 49, 1065000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 50, 1075000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 42, 1010000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 43, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 51, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 10, 200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 44, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34550, 2, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 11, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 52, 1120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 45, 1080000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 53, 1145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 54, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 46, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 55, 1190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 47, 1120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 56, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 48, 1130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 57, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 58, 1265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 59, 1290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 60, 1370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 61, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 60, 1320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 62, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 51, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 13, 265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 61, 1355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 53, 1250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 10, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 65, 1480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 66, 1520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 54, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 55, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 7, 125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 56, 1290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 11, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 57, 1305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 67, 1540000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 68, 1570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 69, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 56, 1330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 69, 1520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 71, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 58, 1385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 59, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 60, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 15, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34550, 2, 3, 55000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 70, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 71, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 72, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 73, 1615000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 61, 1455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 74, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 16, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 62, 1480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 75, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 76, 1710000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 13, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 65, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 8, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 66, 1555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 79, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 80, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 9, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 10, 170000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 14, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 67, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 81, 1885000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 82, 1915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 67, 1620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34700, 1, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 34750, 1, 1, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 10, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 11, 235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 12, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 11, 255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 12, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 13, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 13, 285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 14, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 15, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 14, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 16, 355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 17, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 15, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 16, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 16, 315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 17, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 18, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 19, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 19, 395000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 20, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 20, 435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 21, 455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 22, 465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 21, 440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 23, 490000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 24, 515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 22, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 22, 480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34600, 2, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 23, 505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 26, 570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 2, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 24, 530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 4, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 25, 555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 24, 540000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 25, 560000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 30, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 26, 575000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 31, 665000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 3, 2, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 3, 3, 35000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 27, 585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 28, 600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 3, 45000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 29, 620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 30, 635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 32, 680000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34750, 2, 4, 55000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 33, 700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34700, 1, 31, 655000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 34, 715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 1, 35, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 32, 720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 33, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 35, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 34, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 34550, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 7, 130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 36, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 34, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 36, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 35, 755000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 36, 780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 37, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 37, 865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 5, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 38, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 4, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 38, 835000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 6, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 39, 920000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 39, 855000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 40, 945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 41, 970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 5, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 42, 980000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 43, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 40, 870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 44, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 46, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 37, 910000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 38, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 47, 1000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 39, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 40, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 9, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 48, 1045000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 41, 975000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 49, 1065000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 50, 1075000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 42, 1010000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 43, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 51, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 10, 200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 44, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34550, 2, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 11, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 52, 1120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 45, 1080000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 53, 1145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 54, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 46, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 55, 1190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 47, 1120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 56, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 48, 1130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 57, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 58, 1265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 59, 1290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 60, 1370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 61, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 60, 1320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 62, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 51, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 13, 265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 61, 1355000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 53, 1250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 10, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 65, 1480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 66, 1520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 54, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 55, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 7, 125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 56, 1290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 11, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 57, 1305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 67, 1540000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 68, 1570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 69, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 56, 1330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 69, 1520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 71, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 58, 1385000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 59, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 60, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 15, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34550, 2, 3, 55000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 70, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 71, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 72, 1590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 73, 1615000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 61, 1455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 74, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 2, 16, 330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 62, 1480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 75, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 1, 76, 1710000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 13, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 65, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 8, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 66, 1555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 79, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 80, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 9, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34700, 2, 10, 170000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34600, 2, 14, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 34650, 1, 67, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 81, 1885000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34650, 1, 82, 1915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 34600, 1, 67, 1620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);

	cout << endl << endl << "Security code = 48" << endl;
	cout << osob.Dump(OrderBook::BID);
	cout << osob.Dump(OrderBook::ASK);


	//Security Code 49
	osob.Reset();
	ut.Assert(osob.Add(OrderBook::BID, 17240, 1, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 17220, 2, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 6, 190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 3, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 4, 115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 7, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 8, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 5, 145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 17260, 2, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 6, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 7, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 9, 270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 8, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 2, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 9, 250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 10, 285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 11, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 10, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 11, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 12, 320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 12, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 13, 345000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 13, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 14, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 15, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 16, 445000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 14, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 15, 405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 3, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 17, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 16, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 18, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 4, 120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 19, 550000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 3, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 17, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 18, 500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 20, 605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 5, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 21, 630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 4, 75000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 22, 665000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 19, 520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 23, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 24, 705000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 20, 555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 21, 575000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 5, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 25, 720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 22, 600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 26, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 23, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 24, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 27, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 28, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 29, 805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 26, 745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 27, 775000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 6, 155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 30, 835000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 31, 850000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 28, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 29, 815000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 33, 895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 34, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 7, 185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 30, 865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 35, 950000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 31, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 36, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 37, 985000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 38, 1010000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 32, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 33, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 39, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 34, 975000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 40, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 41, 1070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 35, 995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 36, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 37, 1070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 38, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 43, 1090000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 39, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 40, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 44, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 6, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 8, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 9, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 41, 1190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 42, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 45, 1125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 46, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 47, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 48, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 49, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 43, 1250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 44, 1270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 50, 1225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 45, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 46, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 47, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 51, 1260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 48, 1375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 49, 1395000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 50, 1420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 52, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 53, 1310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 54, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 51, 1455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 10, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 52, 1475000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 53, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 54, 1535000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 55, 1565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 56, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 55, 1405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 57, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 11, 315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 7, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 58, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 56, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 57, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 59, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 58, 1515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 12, 340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 60, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 59, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 61, 1745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 60, 1570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 62, 1770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 61, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 63, 1790000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 64, 1810000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 62, 1605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 65, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 8, 175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 63, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 9, 195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 66, 1870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 67, 1890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 64, 1660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 68, 1915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 65, 1690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 69, 1935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 70, 1945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 10, 220000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 71, 1960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 66, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 67, 1760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 72, 1970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 73, 1995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 68, 1790000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 69, 1800000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 70, 1825000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 74, 2015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 71, 1845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 75, 2040000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 76, 2060000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 11, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 13, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 14, 400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 77, 2110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 12, 265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 13, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 78, 2130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 79, 2155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 72, 1880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 80, 2185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 73, 1890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 81, 2210000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 74, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 82, 2225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 15, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 83, 2250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 84, 2275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 75, 1950000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 76, 1980000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 77, 2020000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 78, 2035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 85, 2290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 14, 320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 79, 2070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 80, 2085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 86, 2315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 15, 360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 87, 2360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 88, 2390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 81, 2120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 82, 2150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 89, 2425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 83, 2175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 90, 2440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 84, 2190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 85, 2205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 86, 2230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 91, 2500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 16, 465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 87, 2255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 88, 2280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 92, 2545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 89, 2310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 90, 2330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 91, 2370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 93, 2580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 94, 2605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 92, 2380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 95, 2610000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 96, 2620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 17, 495000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 97, 2660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 18, 525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 93, 2400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 98, 2695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 94, 2440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 99, 2700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 100, 2725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 95, 2455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 96, 2470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 97, 2480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 98, 2500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 17240, 1, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 17220, 2, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 6, 190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 3, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 4, 115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 7, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 8, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 5, 145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 17260, 2, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 6, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 7, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 9, 270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 8, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 2, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 9, 250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 10, 285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 11, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 10, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 11, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 12, 320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 12, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 13, 345000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 13, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 14, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 15, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 16, 445000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 14, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 15, 405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 3, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 17, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 16, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 18, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 4, 120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 19, 550000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 3, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 17, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 18, 500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 20, 605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 5, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 21, 630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 4, 75000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 22, 665000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 19, 520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 23, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 24, 705000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 20, 555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 21, 575000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 5, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 25, 720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 22, 600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 26, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 23, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 24, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 27, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 28, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 29, 805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 26, 745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 27, 775000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 6, 155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 30, 835000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 31, 850000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 28, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 29, 815000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 33, 895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 34, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 7, 185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 30, 865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 35, 950000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 31, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 36, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 37, 985000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 38, 1010000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 32, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 33, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 39, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 34, 975000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 40, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 41, 1070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 35, 995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 36, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 37, 1070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 38, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 43, 1090000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 39, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 40, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 44, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 6, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 8, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 9, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 41, 1190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 42, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 45, 1125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 46, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 47, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 48, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 49, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 43, 1250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 44, 1270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 50, 1225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 45, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 46, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 47, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 51, 1260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 48, 1375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 49, 1395000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 50, 1420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 52, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 53, 1310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 54, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 51, 1455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 10, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 52, 1475000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 53, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 54, 1535000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 55, 1565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 56, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 55, 1405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 57, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 11, 315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 7, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 58, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 56, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 57, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 59, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 58, 1515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 12, 340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 60, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 59, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 61, 1745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 60, 1570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 62, 1770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 61, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 63, 1790000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 64, 1810000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 62, 1605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 65, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 8, 175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 63, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 9, 195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 66, 1870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 67, 1890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 64, 1660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 68, 1915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 65, 1690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 69, 1935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 70, 1945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 10, 220000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 71, 1960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 66, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 67, 1760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 72, 1970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 73, 1995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 68, 1790000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 69, 1800000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 70, 1825000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 74, 2015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 71, 1845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 75, 2040000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 76, 2060000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 11, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 13, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 14, 400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 77, 2110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 12, 265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 13, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 78, 2130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 79, 2155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 72, 1880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 80, 2185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 73, 1890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 81, 2210000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 74, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 82, 2225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 15, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 83, 2250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 84, 2275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 75, 1950000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 76, 1980000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 77, 2020000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 78, 2035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 85, 2290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 14, 320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 79, 2070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 80, 2085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 86, 2315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 15, 360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 87, 2360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 88, 2390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 81, 2120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 82, 2150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 89, 2425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 83, 2175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 90, 2440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 84, 2190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 85, 2205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 86, 2230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 91, 2500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 16, 465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 87, 2255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 88, 2280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 92, 2545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 89, 2310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 90, 2330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 91, 2370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 93, 2580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 94, 2605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 92, 2380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 95, 2610000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 96, 2620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 17, 495000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 97, 2660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 18, 525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 93, 2400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 98, 2695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 94, 2440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 99, 2700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 100, 2725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 95, 2455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 96, 2470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 97, 2480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 98, 2500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 17240, 1, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 17220, 2, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 6, 190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 3, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 4, 115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 7, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 8, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 5, 145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 17260, 2, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 6, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 7, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 9, 270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 8, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 2, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 9, 250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 10, 285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 11, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 10, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 11, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 12, 320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 12, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 13, 345000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 13, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 14, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 15, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 16, 445000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 14, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 15, 405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 3, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 17, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 16, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 18, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 4, 120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 19, 550000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 3, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 17, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 18, 500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 20, 605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 5, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 21, 630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 4, 75000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 22, 665000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 19, 520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 23, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 24, 705000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 20, 555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 21, 575000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 5, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 25, 720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 22, 600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 26, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 23, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 24, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 27, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 28, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 29, 805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 26, 745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 27, 775000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 6, 155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 30, 835000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 31, 850000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 28, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 29, 815000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 33, 895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 34, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 7, 185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 30, 865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 35, 950000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 31, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 36, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 37, 985000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 38, 1010000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 32, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 33, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 39, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 34, 975000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 40, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 41, 1070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 35, 995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 36, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 37, 1070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 38, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 43, 1090000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 39, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 40, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 44, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 6, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 8, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 9, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 41, 1190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 42, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 45, 1125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 46, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 47, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 48, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 49, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 43, 1250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 44, 1270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 50, 1225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 45, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 46, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 47, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 51, 1260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 48, 1375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 49, 1395000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 50, 1420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 52, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 53, 1310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 54, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 51, 1455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 10, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 52, 1475000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 53, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 54, 1535000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 55, 1565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 56, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 55, 1405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 57, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 11, 315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 7, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 58, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 56, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 57, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 59, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 58, 1515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 12, 340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 60, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 59, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 61, 1745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 60, 1570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 62, 1770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 61, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 63, 1790000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 64, 1810000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 62, 1605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 65, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 8, 175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 63, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 9, 195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 66, 1870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 67, 1890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 64, 1660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 68, 1915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 65, 1690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 69, 1935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 70, 1945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 10, 220000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 71, 1960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 66, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 67, 1760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 72, 1970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 73, 1995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 68, 1790000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 69, 1800000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 70, 1825000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 74, 2015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 71, 1845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 75, 2040000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 76, 2060000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 11, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 13, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 14, 400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 77, 2110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 12, 265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 13, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 78, 2130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 79, 2155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 72, 1880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 80, 2185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 73, 1890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 81, 2210000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 74, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 82, 2225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 15, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 83, 2250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 84, 2275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 75, 1950000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 76, 1980000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 77, 2020000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 78, 2035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 85, 2290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 14, 320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 79, 2070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 80, 2085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 86, 2315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 15, 360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 87, 2360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 88, 2390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 81, 2120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 82, 2150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 89, 2425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 83, 2175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 90, 2440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 84, 2190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 85, 2205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 86, 2230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 91, 2500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 16, 465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 87, 2255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 88, 2280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 92, 2545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 89, 2310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 90, 2330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 91, 2370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 93, 2580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 94, 2605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 92, 2380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 95, 2610000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 96, 2620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 17, 495000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 97, 2660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 18, 525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 93, 2400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 98, 2695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 94, 2440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 99, 2700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 100, 2725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 95, 2455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 96, 2470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 97, 2480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 98, 2500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 17240, 1, 1, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 17220, 2, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 6, 190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 3, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 4, 115000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 7, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 8, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 5, 145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 17260, 2, 1, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 6, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 7, 180000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 9, 270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 8, 205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 2, 70000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 9, 250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 10, 285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 11, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 10, 275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 11, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 12, 320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 2, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 12, 335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 13, 345000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 13, 365000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 14, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 15, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 16, 445000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 14, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 15, 405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 3, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 17, 485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 16, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 18, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 4, 120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 19, 550000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 3, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 17, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 18, 500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 20, 605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 5, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 21, 630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 4, 75000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 22, 665000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 19, 520000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 23, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 24, 705000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 20, 555000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 21, 575000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 5, 90000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 25, 720000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 22, 600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 26, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 23, 640000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 24, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 27, 760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 28, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 29, 805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 26, 745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 27, 775000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 6, 155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 30, 835000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 31, 850000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 28, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 29, 815000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 33, 895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 34, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 7, 185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 30, 865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 35, 950000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 31, 890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 36, 960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 37, 985000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 38, 1010000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 32, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 33, 935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 39, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 34, 975000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 40, 1050000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 41, 1070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 35, 995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 36, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 37, 1070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 38, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 43, 1090000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 39, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 40, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 44, 1100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 6, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 8, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 9, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 41, 1190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 42, 1215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 45, 1125000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 46, 1135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 47, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 48, 1165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 49, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 43, 1250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 44, 1270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 50, 1225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 45, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 46, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 47, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 51, 1260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 48, 1375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 49, 1395000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 50, 1420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 52, 1295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 53, 1310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 54, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 51, 1455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 10, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 52, 1475000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 53, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 54, 1535000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 55, 1565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 56, 1580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 55, 1405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 57, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 11, 315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 7, 150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 58, 1650000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 56, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 57, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 59, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 58, 1515000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 12, 340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 60, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 59, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 61, 1745000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 60, 1570000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 62, 1770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 61, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 63, 1790000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 64, 1810000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 62, 1605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 65, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 8, 175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 63, 1625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 9, 195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 66, 1870000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 67, 1890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 64, 1660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 68, 1915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 65, 1690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 69, 1935000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 70, 1945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 10, 220000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 71, 1960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 66, 1715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 67, 1760000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 72, 1970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 73, 1995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 68, 1790000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 69, 1800000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 70, 1825000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 74, 2015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 71, 1845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 75, 2040000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 76, 2060000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 11, 225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 13, 375000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 14, 400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 77, 2110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 12, 265000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 13, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 78, 2130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 79, 2155000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 72, 1880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 80, 2185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 73, 1890000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 81, 2210000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 74, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 82, 2225000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 15, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 83, 2250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 84, 2275000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 75, 1950000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 76, 1980000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 77, 2020000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 78, 2035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 85, 2290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 14, 320000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 79, 2070000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 80, 2085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 86, 2315000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17220, 2, 15, 360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 87, 2360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 88, 2390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 81, 2120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 82, 2150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 89, 2425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 83, 2175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 90, 2440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 84, 2190000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 85, 2205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 86, 2230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 91, 2500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 16, 465000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 87, 2255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 88, 2280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 92, 2545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 89, 2310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 90, 2330000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 91, 2370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 93, 2580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 94, 2605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 92, 2380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 95, 2610000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 96, 2620000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 17, 495000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 97, 2660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17260, 2, 18, 525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 93, 2400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 98, 2695000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 94, 2440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 99, 2700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 17240, 1, 100, 2725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 95, 2455000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 96, 2470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 97, 2480000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 17240, 1, 98, 2500000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);


	cout << endl << endl << "Security code = 49" << endl;
	cout << osob.Dump(OrderBook::BID);
	cout << osob.Dump(OrderBook::ASK);



	//Security Code 44
	osob.Reset();

	ut.Assert(osob.Add(OrderBook::ASK, 1280, 1, 1, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1270, 1, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 3, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1260, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 2, 2, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 4, 75000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 7, 170000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 1, 9, 250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 8, 235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 9, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 9, 245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 10, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 5, 100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 11, 325000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 12, 380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 10, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 11, 325000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 13, 405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 12, 360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 14, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 15, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 16, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 6, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 13, 380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 14, 390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 17, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 18, 535000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 15, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 19, 565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 7, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 20, 590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 16, 440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 17, 460000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 21, 605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 22, 635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 23, 660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 24, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 26, 730000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 18, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 19, 545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 2, 4, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 21, 580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1290, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 27, 700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 22, 615000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 23, 625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 28, 715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 2, 2, 45000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 29, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 31, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 2, 9, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 26, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 27, 750000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 28, 765000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 29, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 31, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 30, 810000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 31, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 32, 805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 31, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 32, 880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 33, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 35, 880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 34, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 36, 900000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 35, 930000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 37, 925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 4, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 36, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 38, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 39, 995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 37, 990000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 40, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 41, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 38, 1005000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 39, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 40, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 5, 120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 43, 1130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 41, 1060000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 44, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 45, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 43, 1095000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 44, 1105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 46, 1205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 45, 1145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 46, 1175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1300, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 47, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 47, 1200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 48, 1230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 49, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 50, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 51, 1310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 48, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 49, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 50, 1305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 51, 1335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 52, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 52, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 53, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 54, 1380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 55, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 56, 1410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 57, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 58, 1470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 59, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 60, 1525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 6, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 53, 1370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 61, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 62, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 63, 1605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 55, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1300, 1, 56, 1425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 2, 10, 270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 56, 1450000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 2, 11, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1300, 1, 57, 1440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 57, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 57, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 58, 1530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 7, 185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 59, 1565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 69, 1780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 70, 1820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 71, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 72, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 60, 1600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 73, 1895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 61, 1630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 74, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 63, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 8, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 76, 1960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 77, 1970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 78, 2000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 79, 2030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1280, 1, 1, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1270, 1, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 3, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1260, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 2, 2, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 4, 75000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 7, 170000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 1, 9, 250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 8, 235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 9, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 9, 245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 10, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 5, 100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 11, 325000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 12, 380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 10, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 11, 325000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 13, 405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 12, 360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 14, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 15, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 16, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 6, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 13, 380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 14, 390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 17, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 18, 535000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 15, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 19, 565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 7, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 20, 590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 16, 440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 17, 460000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 21, 605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 22, 635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 23, 660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 24, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 26, 730000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 18, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 19, 545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 2, 4, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 21, 580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1290, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 27, 700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 22, 615000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 23, 625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 28, 715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 2, 2, 45000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 29, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 31, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 2, 9, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 26, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 27, 750000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 28, 765000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 29, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 31, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 30, 810000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 31, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 32, 805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 31, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 32, 880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 33, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 35, 880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 34, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 36, 900000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 35, 930000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 37, 925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 4, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 36, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 38, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 39, 995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 37, 990000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 40, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 41, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 38, 1005000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 39, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 40, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 5, 120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 43, 1130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 41, 1060000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 44, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 45, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 43, 1095000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 44, 1105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 46, 1205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 45, 1145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 46, 1175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1300, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 47, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 47, 1200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 48, 1230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 49, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 50, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 51, 1310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 48, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 49, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 50, 1305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 51, 1335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 52, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 52, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 53, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 54, 1380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 55, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 56, 1410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 57, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 58, 1470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 59, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 60, 1525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 6, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 53, 1370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 61, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 62, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 63, 1605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 55, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1300, 1, 56, 1425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 2, 10, 270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 56, 1450000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 2, 11, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1300, 1, 57, 1440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 57, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 57, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 58, 1530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 7, 185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 59, 1565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 69, 1780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 70, 1820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 71, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 72, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 60, 1600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 73, 1895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 61, 1630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 74, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 63, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 8, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 76, 1960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 77, 1970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 78, 2000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 79, 2030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1280, 1, 1, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1270, 1, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 3, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1260, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 2, 2, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 6, 165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 1, 8, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 4, 75000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 7, 170000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 8, 210000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 1, 9, 250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 1, 10, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 8, 235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 9, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 9, 245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 10, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 5, 100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 11, 325000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 12, 380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 10, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 11, 325000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 13, 405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 12, 360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 14, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 15, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 16, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 6, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 13, 380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 14, 390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 17, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 18, 535000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 15, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 19, 565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 7, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 20, 590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 16, 440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 17, 460000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 21, 605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 22, 635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 23, 660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 24, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 26, 730000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 18, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 19, 545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 2, 4, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 27, 770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 21, 580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1290, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 27, 700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 22, 615000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 23, 625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 28, 715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 2, 2, 45000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 29, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 30, 765000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 31, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 32, 895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 2, 9, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 26, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 27, 750000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 28, 765000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 29, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 31, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 30, 810000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 31, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 32, 805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 31, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 32, 880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 33, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 35, 880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 34, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 36, 900000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 35, 930000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 37, 925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 4, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 36, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 38, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 39, 995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 37, 990000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 40, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 41, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 38, 1005000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 39, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 40, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 5, 120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 43, 1130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 41, 1060000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 44, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 45, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 43, 1095000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 44, 1105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 46, 1205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 45, 1145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 46, 1175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1300, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 47, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 47, 1200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 48, 1230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 49, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 50, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 51, 1310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 48, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 49, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 50, 1305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 51, 1335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 52, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 52, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 53, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 54, 1380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 55, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 56, 1410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 57, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 58, 1470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 59, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 60, 1525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 6, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 53, 1370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 61, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 62, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 63, 1605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 64, 1635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 55, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1300, 1, 56, 1425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 2, 10, 270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 56, 1450000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 2, 11, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1300, 1, 57, 1440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 57, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 58, 1510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 57, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 58, 1530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 7, 185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 59, 1565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 69, 1780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 70, 1820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 71, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 72, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 60, 1600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 73, 1895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 61, 1630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 74, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 75, 1945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 64, 1700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 63, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 8, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 76, 1960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 77, 1970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 78, 2000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 79, 2030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1280, 1, 1, 40000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 2, 50000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1270, 1, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 3, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1260, 2, 1, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 2, 2, 60000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 6, 165000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 1, 8, 230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 4, 75000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 7, 170000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 8, 210000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 1, 9, 250000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 1, 10, 290000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 8, 235000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 9, 260000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 9, 245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 10, 295000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 5, 100000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 11, 325000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 12, 380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 10, 280000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 11, 325000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 13, 405000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 12, 360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 14, 425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 15, 430000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 16, 470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 6, 110000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 13, 380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 14, 390000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 17, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 18, 535000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 15, 420000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 19, 565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 2, 7, 135000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 20, 590000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 16, 440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 17, 460000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 21, 605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 22, 635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 23, 660000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 24, 670000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 26, 730000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 18, 510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1270, 1, 19, 545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1260, 2, 4, 105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 27, 770000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 21, 580000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1290, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 27, 700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 22, 615000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 23, 625000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 28, 715000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 2, 2, 45000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 29, 735000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 30, 765000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 31, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 1, 32, 895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1270, 2, 9, 300000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 25, 690000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 26, 725000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 27, 750000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 28, 765000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 29, 785000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 31, 795000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 30, 810000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 31, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1280, 1, 32, 805000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 1, 32, 875000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 31, 845000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 32, 880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 33, 905000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 35, 880000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 34, 915000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 36, 900000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 35, 930000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 3, 80000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 37, 925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 4, 95000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 36, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 38, 965000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 39, 995000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 37, 990000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 40, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 41, 1035000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 38, 1005000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 39, 1015000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 40, 1030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 5, 120000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 43, 1130000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 41, 1060000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 44, 1150000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 45, 1195000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 42, 1085000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 43, 1095000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 44, 1105000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 46, 1205000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 45, 1145000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 46, 1175000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1300, 2, 1, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 47, 1245000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 47, 1200000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 48, 1230000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 49, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 50, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 51, 1310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 48, 1255000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 49, 1285000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 50, 1305000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 51, 1335000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 52, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 52, 1340000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 53, 1360000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 54, 1380000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 55, 1400000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 56, 1410000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 57, 1435000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 58, 1470000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 59, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 60, 1525000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 6, 160000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 53, 1370000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 61, 1545000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 62, 1585000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 63, 1605000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 64, 1635000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 55, 1415000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1300, 1, 56, 1425000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 2, 10, 270000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 56, 1450000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 2, 11, 310000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1300, 1, 57, 1440000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 57, 1485000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 58, 1510000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 57, 1505000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 58, 1530000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 7, 185000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 59, 1565000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 69, 1780000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 70, 1820000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 71, 1840000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 72, 1865000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 60, 1600000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 73, 1895000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 61, 1630000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 74, 1925000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 75, 1945000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1300, 1, 64, 1700000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1290, 1, 63, 1685000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1280, 2, 8, 215000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 76, 1960000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 77, 1970000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 78, 2000000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1290, 1, 79, 2030000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);

	cout << endl << endl << "Security code = 44" << endl;
	cout << osob.Dump(OrderBook::BID);
	cout << osob.Dump(OrderBook::ASK);



	//Security Code 8
	osob.Reset();

	ut.Assert(osob.Add(OrderBook::BID, 5730, 1, 1, 3000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5730, 1, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5730, 1, 3, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5700, 2, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5680, 3, 1, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5640, 4, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5740, 1, 1, 5000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5750, 2, 1, 3000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5760, 3, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5770, 4, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5730, 1, 1, 3000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5730, 1, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5730, 1, 3, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5700, 2, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5680, 3, 1, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5640, 4, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5740, 1, 1, 5000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5750, 2, 1, 3000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5760, 3, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5770, 4, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5730, 1, 1, 3000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5730, 1, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5730, 1, 3, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5700, 2, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5680, 3, 1, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5640, 4, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5740, 1, 1, 5000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5750, 2, 1, 3000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5760, 3, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5770, 4, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5730, 1, 1, 3000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5730, 1, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 5730, 1, 3, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5700, 2, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5680, 3, 1, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 5640, 4, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5740, 1, 1, 5000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5750, 2, 1, 3000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5760, 3, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 5770, 4, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);


	cout << endl << endl << "Security code = 8" << endl;
	cout << osob.Dump(OrderBook::BID);
	cout << osob.Dump(OrderBook::ASK);






	//Security Code 951
	osob.Reset();

	ut.Assert(osob.Add(OrderBook::ASK, 1510, 1, 1, 4500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 2, 5500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 3, 14000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 4, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1490, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1480, 2, 1, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 5, 18000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 6, 23000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1520, 2, 1, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 2, 10500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 3, 16500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1470, 2, 1, 1500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 3, 13000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 4, 17000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 2, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 6500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 2, 5, 17500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 2, 2, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1470, 1, 1, 500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1460, 2, 1, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 2, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 2, 6, 25500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 2, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 3, 9000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 3, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 3, 7, 26000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 4, 8500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 1, 1, 1500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 4500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 3, 7500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 1, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 4, 11500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 3, 9500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 2, 5, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 4, 12500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 7, 22500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1510, 1, 1, 4500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 2, 5500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 3, 14000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 4, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1490, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1480, 2, 1, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 5, 18000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 6, 23000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1520, 2, 1, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 2, 10500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 3, 16500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1470, 2, 1, 1500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 3, 13000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 4, 17000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 2, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 6500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 2, 5, 17500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 2, 2, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1470, 1, 1, 500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1460, 2, 1, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 2, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 2, 6, 25500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 2, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 3, 9000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 3, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 3, 7, 26000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 4, 8500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 1, 1, 1500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 4500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 3, 7500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 1, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 4, 11500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 3, 9500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 2, 5, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 4, 12500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 7, 22500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 18500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 5, 17500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 10500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 7500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 6500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 6000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 3, 6, 14500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 17000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1500, 2, 1, 3500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1500, 2, 2, 5500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 22000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 22000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 17500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 4, 21500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 6, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 26500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 5, 26000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 7, 33000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 5, 23500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 5, 18500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 31500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 5, 16500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 30500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 4, 16000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 14000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 27500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 24500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 3, 7, 18500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 25500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 13000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 4, 14500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 7, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1500, 2, 3, 9500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1500, 2, 4, 11000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 4, 22000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1510, 1, 1, 4500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 2, 5500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 3, 14000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 4, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1490, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1480, 2, 1, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 5, 18000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 6, 23000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1520, 2, 1, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 2, 10500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 3, 16500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1470, 2, 1, 1500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 3, 13000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 4, 17000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 2, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 6500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 2, 5, 17500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 2, 2, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1470, 1, 1, 500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1460, 2, 1, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 2, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 2, 6, 25500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 2, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 3, 9000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 3, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 3, 7, 26000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 4, 8500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 1, 1, 1500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 4500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 3, 7500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 1, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 4, 11500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 3, 9500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 2, 5, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 4, 12500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 7, 22500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1510, 1, 1, 4500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 2, 5500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 3, 14000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 4, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1490, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1480, 2, 1, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 5, 18000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1510, 1, 6, 23000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1520, 2, 1, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 2, 10500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 3, 16500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1470, 2, 1, 1500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 3, 13000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 4, 17000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 2, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 6500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 2, 5, 17500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 2, 2, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1470, 1, 1, 500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::BID, 1460, 2, 1, 2000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 2, 2, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 2, 6, 25500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 2, 4000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 3, 9000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 3, 7000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 3, 7, 26000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 1, 4, 8500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 1, 1, 1500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 4500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 2, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 3, 7500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1480, 1, 1, 1000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 4, 11500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 3, 9500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 2, 5, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 1, 4, 12500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 7, 22500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 18500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 5, 17500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 10500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 7500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 6500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 6000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 3, 6, 14500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 30000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 1, 2500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 17000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Add(OrderBook::ASK, 1500, 2, 1, 3500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1500, 2, 2, 5500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 22000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 22000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 2, 15000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 17500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 4, 21500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 6, 20000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 26500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 5, 26000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 7, 33000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 5, 23500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 5, 18500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 31500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 5, 16500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 30500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 10000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 4, 16000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 14000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 27500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 5, 24500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1460, 3, 7, 18500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 6, 25500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 3, 13000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1480, 1, 4, 14500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::BID, 1470, 2, 7, 25000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1500, 2, 3, 9500), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1500, 2, 4, 11000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(osob.Change(OrderBook::ASK, 1490, 1, 4, 22000), __FILE__,__FUNCTION__,__LINE__); ut.Assert(osob.AssessPriceIntegrity(), __FILE__,__FUNCTION__,__LINE__);



	cout << endl << endl << "Security code = 951" << endl;
	cout << osob.Dump(OrderBook::BID);
	cout << osob.Dump(OrderBook::ASK);





	ut.PrintResult();


	return 0;
}
