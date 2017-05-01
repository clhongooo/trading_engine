/*
 * TestOrderBook.cpp
 *
 *  Created on: May 26, 2014
 *      Author: sunny
 */

#include "TestOrderBook.h"

TestOrderBook::TestOrderBook() {
  // TODO Auto-generated constructor stub

}

TestOrderBook::~TestOrderBook() {
  // TODO Auto-generated destructor stub
}

bool TestOrderBook::Check(
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

void TestOrderBook::RunTest()
{

  int iaBid[10][3];
  int iaAsk[10][3];
  OrderBook osob;


  // -------------------------------------
  // LocatePriceLev: Empty queues
  // -------------------------------------
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9750) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9740) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9640) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9630) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9750) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9740) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9640) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9630) == -1);


  // -------------------------------------
  // Roll the circular array different start/end points and Test LocatePriceLev()
  // -------------------------------------
  for (int i = 0; i < 48; ++i)
  {
    osob.Reset();

    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9900-i*10, 1, 1, 300));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9900-i*10+5, 1, 1, 300));
    CPPUNIT_ASSERT(osob.Delete(OrderBook::BID, 9900-i*10, 2, 1, 300));
    CPPUNIT_ASSERT(osob.Delete(OrderBook::BID, 9900-i*10+5, 1, 1, 300));

    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9900+i*10, 1, 1, 300));
    CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9900+i*10, 1, 1, 300));

    // -------------------------------------
    // LocatePriceLev: 3 Price Levels
    // -------------------------------------
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34650, 1, 1, 600000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34550, 2, 1, 55000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34600, 2, 1, 725000)); // Insert 2nd

    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 34700) == -1);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 34650) == 1);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 34620) == -2);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 34600) == 2);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 34570) == -3);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 34550) == 3);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 34500) == -4);

    // -------------------------------------
    // LocatePriceLev: 4 Price Levels
    // -------------------------------------
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 44700, 1, 1, 725000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 44650, 1, 1, 600000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 44600, 1, 1, 55000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 44550, 1, 1, 725000));

    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 44750) == -5);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 44700) == 4);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 44650) == 3);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 44620) == -3);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 44600) == 2);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 44570) == -2);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 44550) == 1);
    CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 44500) == -1);

  }



  // -------------------------------------
  // Setting up the Initial Order Book
  // -------------------------------------
  osob.Reset();
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9730, 1, 1, 700));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9720, 2, 1, 350));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9710, 3, 1, 150));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9700, 4, 1, 250));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9690, 5, 1, 100));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9680, 6, 1, 150));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9670, 7, 1,  50));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9660, 8, 1, 200));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9650, 9, 1, 100));

  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9760, 1, 1, 500));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9770, 2, 1, 300));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9780, 3, 1, 100));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9790, 4, 1, 150));

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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid1, iaAsk1));


  // -------------------------------------
  // LocatePriceLev: 9 and 4 Price Levels
  // -------------------------------------
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9730) == 1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9720) == 2);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9710) == 3);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9700) == 4);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9690) == 5);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9680) == 6);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9670) == 7);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9660) == 8);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9650) == 9);


  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9750) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9740) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9735) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9725) == -2);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9715) == -3);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9705) == -4);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9695) == -5);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9685) == -6);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9675) == -7);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9665) == -8);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9655) == -9);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9640) == -10);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9630) == -10);


  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9760) == 1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9770) == 2);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9780) == 3);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9790) == 4);

  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9755) == -1);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9765) == -2);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9775) == -3);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9785) == -4);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9795) == -5);
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::ASK, 9800) == -5);


  // -------------------------------------
  // Example 1 P.40/48
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 9770, 2, 1, 200));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9850, 5, 1, 300));

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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid2, iaAsk2));


  // -------------------------------------
  // Example 2 P.41/48
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9740, 1, 1, 50));

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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid3, iaAsk3));


  // -------------------------------------
  // Example 3 P.41/48
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9750, 1, 1, 250));
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 9660, 10, 1, 150));

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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid4, iaAsk4));


  // -------------------------------------
  // Example 4 P.42/48
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Delete(OrderBook::BID, 9750, 1, 1, 250));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9650, 10, 1, 100));

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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid5, iaAsk5));


  // -------------------------------------
  // Example 5 P.43/48
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9750, 1, 1, 300));
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9850, 6, 1, 300));

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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid6, iaAsk6));


  // -------------------------------------
  // Explicit Deletions vs Implicit Deletions P.44/48
  // -------------------------------------

  // Reset Bid Queue
  osob.Reset(OrderBook::BID);

  // Build initial order book
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9800, 1, 1, 700));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9790, 2, 1, 350));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9780, 3, 1, 150));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9760, 4, 1, 250));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9750, 5, 1, 100));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9730, 6, 1, 400));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9720, 7, 1, 200));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9710, 8, 1, 300));
  //osob.Dump(OrderBook::BID);
  //osob.Dump(OrderBook::ASK);

  // Then make changes
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9860, 1, 1, 450));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9850, 2, 1, 550));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9840, 3, 1, 650));
  //osob.Dump(OrderBook::BID);
  //osob.Dump(OrderBook::ASK);

  CPPUNIT_ASSERT(osob.Delete(OrderBook::BID, 9760, 7, 1, 250));
  CPPUNIT_ASSERT(osob.Delete(OrderBook::BID, 9750, 7, 1, 100));
  CPPUNIT_ASSERT(osob.Delete(OrderBook::BID, 9730, 7, 1, 400));
  CPPUNIT_ASSERT(osob.Delete(OrderBook::BID, 9720, 7, 1, 200));
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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid7, iaAsk7));


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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid8, iaAsk8));


  // -------------------------------------
  // My Own Test Cases
  // Focusing on tesing ASK
  // -------------------------------------
  osob.Reset();

  // -------------------------------------
  // Add(): Various normal cases
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9730, 1, 1, 700));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9720, 1, 1, 600)); // Immediately before head
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9680, 1, 1, 500)); // 4 Levels before head
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9760, 4, 1, 800)); // 3 Levels after tail
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9790, 5, 1, 100)); // 3 Levels after tail
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9780, 5, 1, 900)); // Insert at 2nd last
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9800, 7, 1, 200)); // Immediately after tail
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9740, 4, 1, 200)); // Insert at middle, Price Level 4

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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid9, iaAsk9));



  // -------------------------------------
  // Setting up for second round testing
  // -------------------------------------

  // To remove additional item
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9740, 4, 1, 200)); // Remove our additional 9740
  // To fully stuff the internal buffer
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9810, 8, 1, 168)); // Dummy
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9820, 9, 1, 168)); // Dummy
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9890, 10, 1, 168)); // Dummy
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9880, 10, 1, 168)); // Push the 9890 out
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9870, 10, 1, 168)); // Push the 9890 out
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9860, 10, 1, 168)); // Push the 9890 out

  // Restore to original state
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9860, 10, 1, 168));
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9820, 9, 1, 168));
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9810, 8, 1, 168));

  // -------------------------------------
  // Try its error catching and lenient handling ability!!
  // -------------------------------------
  // Add()
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9680, 1, 1, 501)); // Clashing with existing head, implicit conversion to Change()
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9760, 4, 1, 801)); // Clashing with existing mid-pt, implicit conversion to Change()
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9800, 7, 1, 201)); // Clashing with existing tail, implicit conversion to Change()

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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid10, iaAsk10));


  // -------------------------------------
  // Add()
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9680, 2, 1, 502)); // Clashing with existing head, but price level wrong, still implicit conversion to Change()
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9760, 3, 1, 802)); // Clashing with existing mid-pt, but price level wrong, still implicit conversion to Change()
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9800, 9, 1, 202)); // Clashing with existing tail, but price level wrong, still implicit conversion to Change()

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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid11, iaAsk11));



  // -------------------------------------
  // Add()
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9900, 1, 1, 990)); // Way high and way low prices with incorrect price levels, still insert using abs price
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1900, 10, 1, 190)); // Way high and way low prices with incorrect price levels, still insert using abs price

  CPPUNIT_ASSERT(!osob.Add(OrderBook::ASK, 9999, 30, 1, 190)); // Price Level too high, reject
  CPPUNIT_ASSERT(!osob.Add(OrderBook::ASK, -1, 3, 1, 190)); // Price can't be negative
  //CPPUNIT_ASSERT(!osob.Add(OrderBook::ASK, 9790, 7, -1, 190)); // NumOfOrd can't be negative
  //CPPUNIT_ASSERT(!osob.Add(OrderBook::ASK, 9790, 7, 1, -190)); // AggQty can't be negative


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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid12, iaAsk12));


  // -------------------------------------
  // Change()
  // -------------------------------------
  // Clean up a bit first
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 1900, 1, 1, 190));
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9680, 1, 1, 502));
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9900, 7, 1, 990));


  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 9190, 4, 1, 400)); // Price at Price Level 4 is wrong, and Price 9190 is non-existent. Implicit conversion to Add()
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 9760, 5, 1, 803)); // Price at Price Level 5 is wrong, but handle leniently - update Price Level 4
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 9800, 7, 1, 203)); // This is correct, handle normally
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 9810, 3, 1, 300)); // Price non-existent, Price Level wrong, implicit conversion to Add()
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 9820, 9, 1, 300)); // Price non-existent, Price Level correct, implicit conversion to Add()

  //CPPUNIT_ASSERT(!osob.Change(OrderBook::ASK, 9680, 3, 1, -345)); // AggQty can't be -ve.
  //CPPUNIT_ASSERT(!osob.Change(OrderBook::ASK, 9680, 3, -1, 345)); // NumOfOrd can't be -ve.
  //CPPUNIT_ASSERT(!osob.Change(OrderBook::ASK, 9680, -3, 1, 345)); // Price Level can't be negative


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
    ,{1,300,9820}
    ,{0,  0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid13, iaAsk13));





  // -------------------------------------
  // Delete()
  // -------------------------------------

  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9190, 4, 1, 345)); // Price Level is wrong, AggQty is also wrong, use absolute price
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9760, 3, 1, 345)); // Price Level right, but AggQty wrong, delete 9760
  CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, 9790, 9, 1, 100)); // Price Level  wrong, Price right, AggQty right, delete 9790

  CPPUNIT_ASSERT(!osob.Delete(OrderBook::ASK, 9999, 1, 1, 345)); // Nothing is wring, including the Price, return error and do nothing
  //CPPUNIT_ASSERT(!osob.Delete(OrderBook::ASK, 9800, -1, 1, 345)); // Price Level can't be negative
  //CPPUNIT_ASSERT(!osob.Delete(OrderBook::ASK, 9800, 1, 1, -345)); // AggQty can't be negative
  //CPPUNIT_ASSERT(!osob.Delete(OrderBook::ASK, 9800, 1, -2, 500)); // NumOfOrd can't be negative

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
    ,{1,300,9820}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid14, iaAsk14));

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
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5080, 1, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5060, 2, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 3, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 6, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5090, 2, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 7, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 4, 100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5070, 2, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 5, 115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 6, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 7, 155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 8, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 9, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 8, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 9, 255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 2, 2, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 10, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 10, 240000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 11, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 12, 305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 13, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 12, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 13, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 15, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 14, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 16, 410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 17, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 18, 460000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 19, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 15, 445000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 3, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 20, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 18, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 18, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 19, 525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 19, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 20, 560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 18, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 19, 545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 24, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 20, 585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 25, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 21, 620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 26, 705000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 4, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 3, 5, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5100, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 24, 645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 25, 695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 26, 730000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 27, 745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 26, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 28, 770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 29, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 30, 830000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 31, 860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 27, 755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 28, 775000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 29, 800000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 30, 820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 31, 850000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 2, 5, 145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 4, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 33, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 5, 125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 34, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 32, 870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 35, 940000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 36, 955000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 37, 985000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 38, 1020000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 33, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 39, 1025000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 40, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 41, 1055000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 43, 1110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 44, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 45, 1180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 34, 925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 46, 1235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 35, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 47, 1260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 6, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 48, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 37, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 42, 1125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 43, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 38, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 39, 1045000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 40, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 41, 1115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 42, 1155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5110, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 43, 1185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 14, 350000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 15, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 46, 1205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 44, 1200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 47, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 45, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 48, 1235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 49, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 46, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 16, 385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 50, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 17, 410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 47, 1270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 51, 1320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 48, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 52, 1355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 49, 1315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 50, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 53, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 51, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 52, 1385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 54, 1440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 55, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 18, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 53, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 54, 1430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 56, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 57, 1525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 55, 1465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 56, 1495000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 57, 1515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 58, 1560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 58, 1560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 59, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 60, 1630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 59, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 61, 1645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 60, 1595000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 62, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 63, 1700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 64, 1720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 19, 450000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 20, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 65, 1750000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 21, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 66, 1770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 67, 1795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 68, 1830000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 59, 1645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 60, 1670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 61, 1690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 64, 1695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 62, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 63, 1760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 64, 1780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 65, 1805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 66, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 67, 1855000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 68, 1860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 65, 1735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 69, 1885000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 70, 1905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 2, 12, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 71, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5120, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 72, 1955000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 66, 1755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 73, 1990000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 74, 2015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5120, 2, 2, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 75, 2025000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 76, 2040000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 77, 2065000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 67, 1820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 68, 1845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 69, 1860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 78, 2080000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 70, 1880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5080, 1, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5060, 2, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 3, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 6, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5090, 2, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 7, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 4, 100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5070, 2, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 5, 115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 6, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 7, 155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 8, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 9, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 8, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 9, 255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 2, 2, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 10, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 10, 240000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 11, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 12, 305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 13, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 12, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 13, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 15, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 14, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 16, 410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 17, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 18, 460000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 19, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 15, 445000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 3, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 20, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 18, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 18, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 19, 525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 19, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 20, 560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 18, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 19, 545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 24, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 20, 585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 25, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 21, 620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 26, 705000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 4, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 3, 5, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5100, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 24, 645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 25, 695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 26, 730000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 27, 745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 26, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 28, 770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 29, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 30, 830000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 31, 860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 27, 755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 28, 775000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 29, 800000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 30, 820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 31, 850000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 2, 5, 145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 4, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 33, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 5, 125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 34, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 32, 870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 35, 940000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 36, 955000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 37, 985000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 38, 1020000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 33, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 39, 1025000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 40, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 41, 1055000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 43, 1110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 44, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 45, 1180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 34, 925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 46, 1235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 35, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 47, 1260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 6, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 48, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 37, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 42, 1125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 43, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 38, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 39, 1045000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 40, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 41, 1115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 42, 1155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5110, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 43, 1185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 14, 350000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 15, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 46, 1205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 44, 1200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 47, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 45, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 48, 1235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 49, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 46, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 16, 385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 50, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 17, 410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 47, 1270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 51, 1320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 48, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 52, 1355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 49, 1315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 50, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 53, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 51, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 52, 1385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 54, 1440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 55, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 18, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 53, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 54, 1430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 56, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 57, 1525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 55, 1465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 56, 1495000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 57, 1515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 58, 1560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 58, 1560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 59, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 60, 1630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 59, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 61, 1645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 60, 1595000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 62, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 63, 1700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 64, 1720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 19, 450000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 20, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 65, 1750000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 21, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 66, 1770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 67, 1795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 68, 1830000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 59, 1645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 60, 1670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 61, 1690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 64, 1695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 62, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 63, 1760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 64, 1780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 65, 1805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 66, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 67, 1855000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 68, 1860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 65, 1735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 69, 1885000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 70, 1905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 2, 12, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 71, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5120, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 72, 1955000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 66, 1755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 73, 1990000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 74, 2015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5120, 2, 2, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 75, 2025000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 76, 2040000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 77, 2065000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 67, 1820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 68, 1845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 69, 1860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 78, 2080000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 70, 1880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5080, 1, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5060, 2, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 3, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 6, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5090, 2, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 7, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 4, 100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5070, 2, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 5, 115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 6, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 7, 155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 8, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 9, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 8, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 9, 255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 2, 2, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 10, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 10, 240000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 11, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 12, 305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 13, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 14, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 13, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 12, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 13, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 15, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 14, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 16, 410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 17, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 18, 460000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 19, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 15, 445000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 3, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 20, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 21, 560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 18, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 18, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 19, 525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 19, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 20, 560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 21, 580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 18, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 19, 545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 24, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 20, 585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 25, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 21, 620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 26, 705000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 4, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 27, 740000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 3, 5, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5100, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 24, 645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 25, 695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 26, 730000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 27, 745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 26, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 28, 770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 29, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 30, 830000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 31, 860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 27, 755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 28, 775000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 29, 800000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 30, 820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 31, 850000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 2, 5, 145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 4, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 33, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 5, 125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 34, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 32, 870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 35, 940000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 36, 955000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 37, 985000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 38, 1020000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 33, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 39, 1025000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 40, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 41, 1055000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 43, 1110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 44, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 45, 1180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 34, 925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 46, 1235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 35, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 47, 1260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 6, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 48, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 49, 1315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 37, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 42, 1125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 43, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 38, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 44, 1175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 39, 1045000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 52, 1390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 40, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 41, 1115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 42, 1155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5110, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 43, 1185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 14, 350000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 15, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 46, 1205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 44, 1200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 47, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 45, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 48, 1235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 49, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 46, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 16, 385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 50, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 17, 410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 47, 1270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 51, 1320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 48, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 52, 1355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 49, 1315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 50, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 53, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 51, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 52, 1385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 54, 1440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 55, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 18, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 53, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 54, 1430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 56, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 57, 1525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 55, 1465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 56, 1495000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 57, 1515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 58, 1560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 58, 1560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 59, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 60, 1630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 59, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 61, 1645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 60, 1595000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 62, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 63, 1700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 64, 1720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 19, 450000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 20, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 65, 1750000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 21, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 66, 1770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 67, 1795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 68, 1830000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 69, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 59, 1645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 60, 1670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 61, 1690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 64, 1695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 62, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 63, 1760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 64, 1780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 65, 1805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 66, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 67, 1855000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 68, 1860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 65, 1735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 69, 1885000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 70, 1905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 2, 12, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 71, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5120, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 72, 1955000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 66, 1755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 73, 1990000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 74, 2015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5120, 2, 2, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 75, 2025000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 76, 2040000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 77, 2065000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 67, 1820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 68, 1845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 69, 1860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 78, 2080000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 70, 1880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5080, 1, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5060, 2, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 3, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 6, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5090, 2, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 7, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 4, 100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5070, 2, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 5, 115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 6, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 7, 155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 8, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 9, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 8, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 9, 255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 2, 2, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 10, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 10, 240000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 11, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 12, 305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 13, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 14, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 13, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 12, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 13, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 15, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 14, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 16, 410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 17, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 18, 460000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 19, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 15, 445000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 3, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 20, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 21, 560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 18, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 18, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 19, 525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 19, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 20, 560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 21, 580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 18, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 19, 545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 24, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 20, 585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 25, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5080, 1, 21, 620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 26, 705000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 2, 4, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 1, 27, 740000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5070, 3, 5, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5100, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 24, 645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 25, 695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 26, 730000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 27, 745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 26, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 28, 770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 29, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 30, 830000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 31, 860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 27, 755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 28, 775000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 29, 800000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 30, 820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 31, 850000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5080, 2, 5, 145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 4, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 33, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 5, 125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 34, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 32, 870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 35, 940000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 36, 955000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 37, 985000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 38, 1020000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 33, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 39, 1025000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 40, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 41, 1055000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 43, 1110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 44, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 45, 1180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 34, 925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 46, 1235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 35, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 47, 1260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 2, 6, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 48, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 49, 1315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 37, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 42, 1125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 43, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 38, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 44, 1175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5090, 1, 39, 1045000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 1, 52, 1390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 40, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 41, 1115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 42, 1155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5110, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 43, 1185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 14, 350000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 15, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 46, 1205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 44, 1200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 47, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 45, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 48, 1235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 49, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 46, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 16, 385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 50, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 2, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 17, 410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 47, 1270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 51, 1320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 48, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 52, 1355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 49, 1315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 50, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 53, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 51, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 52, 1385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 54, 1440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 55, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 18, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 53, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 54, 1430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 56, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 57, 1525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 55, 1465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 56, 1495000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 57, 1515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 58, 1560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 58, 1560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 59, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 60, 1630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 59, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 61, 1645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5100, 1, 60, 1595000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 62, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 63, 1700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 64, 1720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 19, 450000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 20, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 65, 1750000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5090, 2, 21, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 66, 1770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 67, 1795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 68, 1830000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 1, 69, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 59, 1645000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 60, 1670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 61, 1690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 64, 1695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 62, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 63, 1760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 64, 1780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 65, 1805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 66, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 67, 1855000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 68, 1860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 65, 1735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 69, 1885000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 70, 1905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5100, 2, 12, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 71, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5120, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 72, 1955000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 66, 1755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 73, 1990000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 74, 2015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5120, 2, 2, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 75, 2025000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 76, 2040000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 77, 2065000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 67, 1820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 68, 1845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 69, 1860000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5110, 1, 78, 2080000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 5110, 1, 70, 1880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());

  cout << endl << endl << "Security code = 50" << endl;
  cout << osob.Dump(OrderBook::BID);
  cout << osob.Dump(OrderBook::ASK);



  //Security Code 48
  osob.Reset();

  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34700, 1, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 34750, 1, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 10, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 11, 235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 12, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 11, 255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 12, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 13, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 13, 285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 14, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 15, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 14, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 16, 355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 15, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 16, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 16, 315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 17, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 20, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 20, 435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 21, 455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 22, 465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 21, 440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 23, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 22, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34600, 2, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 23, 505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 2, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 24, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 4, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 25, 555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 24, 540000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 3, 2, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 3, 3, 35000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 27, 585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 3, 45000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 29, 620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 30, 635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 32, 680000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 2, 4, 55000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 33, 700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 31, 655000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 34, 715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 32, 720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34550, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 7, 130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 34, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 36, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 35, 755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 36, 780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 37, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 37, 865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 5, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 38, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 4, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 38, 835000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 6, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 39, 920000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 39, 855000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 40, 945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 41, 970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 5, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 42, 980000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 43, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 40, 870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 46, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 37, 910000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 38, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 47, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 39, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 40, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 9, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 48, 1045000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 41, 975000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 49, 1065000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 50, 1075000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 42, 1010000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 43, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 51, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 10, 200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 44, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34550, 2, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 11, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 52, 1120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 45, 1080000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 53, 1145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 54, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 46, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 55, 1190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 47, 1120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 56, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 48, 1130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 57, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 58, 1265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 60, 1370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 51, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 13, 265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 53, 1250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 10, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 65, 1480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 66, 1520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 54, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 55, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 7, 125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 56, 1290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 11, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 57, 1305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 67, 1540000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 68, 1570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 56, 1330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 58, 1385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 59, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 60, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 15, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34550, 2, 3, 55000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 70, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 71, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 72, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 73, 1615000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 61, 1455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 74, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 16, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 62, 1480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 75, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 13, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 65, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 8, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 66, 1555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 79, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 80, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 9, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 10, 170000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 14, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 67, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 81, 1885000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 67, 1620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34700, 1, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 34750, 1, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 10, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 11, 235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 12, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 11, 255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 12, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 13, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 13, 285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 14, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 15, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 14, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 16, 355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 15, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 16, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 16, 315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 17, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 20, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 20, 435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 21, 455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 22, 465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 21, 440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 23, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 22, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34600, 2, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 23, 505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 2, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 24, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 4, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 25, 555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 24, 540000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 3, 2, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 3, 3, 35000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 27, 585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 3, 45000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 29, 620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 30, 635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 32, 680000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 2, 4, 55000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 33, 700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 31, 655000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 34, 715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 32, 720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34550, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 7, 130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 34, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 36, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 35, 755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 36, 780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 37, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 37, 865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 5, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 38, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 4, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 38, 835000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 6, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 39, 920000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 39, 855000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 40, 945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 41, 970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 5, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 42, 980000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 43, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 40, 870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 46, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 37, 910000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 38, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 47, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 39, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 40, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 9, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 48, 1045000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 41, 975000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 49, 1065000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 50, 1075000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 42, 1010000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 43, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 51, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 10, 200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 44, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34550, 2, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 11, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 52, 1120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 45, 1080000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 53, 1145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 54, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 46, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 55, 1190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 47, 1120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 56, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 48, 1130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 57, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 58, 1265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 60, 1370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 51, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 13, 265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 53, 1250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 10, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 65, 1480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 66, 1520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 54, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 55, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 7, 125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 56, 1290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 11, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 57, 1305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 67, 1540000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 68, 1570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 56, 1330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 58, 1385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 59, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 60, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 15, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34550, 2, 3, 55000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 70, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 71, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 72, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 73, 1615000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 61, 1455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 74, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 16, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 62, 1480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 75, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 13, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 65, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 8, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 66, 1555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 79, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 80, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 9, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 10, 170000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 14, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 67, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 81, 1885000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 67, 1620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34700, 1, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 34750, 1, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 10, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 11, 235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 12, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 11, 255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 12, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 13, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 13, 285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 14, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 15, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 14, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 16, 355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 17, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 15, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 16, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 16, 315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 17, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 18, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 19, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 19, 395000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 20, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 20, 435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 21, 455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 22, 465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 21, 440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 23, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 24, 515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 22, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 22, 480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34600, 2, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 23, 505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 26, 570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 2, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 24, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 4, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 25, 555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 24, 540000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 25, 560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 30, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 26, 575000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 31, 665000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 3, 2, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 3, 3, 35000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 27, 585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 28, 600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 3, 45000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 29, 620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 30, 635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 32, 680000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 2, 4, 55000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 33, 700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 31, 655000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 34, 715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 35, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 32, 720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 33, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 35, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 34, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 7, 130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 36, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 34, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 36, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 35, 755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 36, 780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 37, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 37, 865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 5, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 38, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 4, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 38, 835000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 6, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 39, 920000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 39, 855000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 40, 945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 41, 970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 5, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 42, 980000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 43, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 40, 870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 44, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 46, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 37, 910000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 38, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 47, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 39, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 40, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 9, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 48, 1045000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 41, 975000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 49, 1065000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 50, 1075000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 42, 1010000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 43, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 51, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 10, 200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 44, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34550, 2, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 11, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 52, 1120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 45, 1080000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 53, 1145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 54, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 46, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 55, 1190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 47, 1120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 56, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 48, 1130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 57, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 58, 1265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 59, 1290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 60, 1370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 61, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 60, 1320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 62, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 51, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 13, 265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 61, 1355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 53, 1250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 10, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 65, 1480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 66, 1520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 54, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 55, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 7, 125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 56, 1290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 11, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 57, 1305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 67, 1540000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 68, 1570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 69, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 56, 1330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 69, 1520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 71, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 58, 1385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 59, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 60, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 15, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34550, 2, 3, 55000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 70, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 71, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 72, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 73, 1615000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 61, 1455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 74, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 16, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 62, 1480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 75, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 76, 1710000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 13, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 65, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 8, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 66, 1555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 79, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 80, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 9, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 10, 170000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 14, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 67, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 81, 1885000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 82, 1915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 67, 1620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34700, 1, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 34750, 1, 1, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 10, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 11, 235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 12, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 11, 255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 12, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 13, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 13, 285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 14, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 15, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 14, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 16, 355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 17, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 15, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 16, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 16, 315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 17, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 18, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 19, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 19, 395000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 20, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 20, 435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 21, 455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 22, 465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 21, 440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 23, 490000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 24, 515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 22, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 22, 480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34600, 2, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 23, 505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 26, 570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 2, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 24, 530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 4, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 25, 555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 24, 540000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 25, 560000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 30, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 26, 575000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 31, 665000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 3, 2, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 3, 3, 35000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 27, 585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 28, 600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 3, 45000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 29, 620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 30, 635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 32, 680000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34750, 2, 4, 55000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 33, 700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34700, 1, 31, 655000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 34, 715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 1, 35, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 32, 720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 33, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 35, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 34, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 34550, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 7, 130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 36, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 34, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 36, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 35, 755000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 36, 780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 37, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 37, 865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 5, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 38, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 4, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 38, 835000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 6, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 39, 920000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 39, 855000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 40, 945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 41, 970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 5, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 42, 980000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 43, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 40, 870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 44, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 46, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 37, 910000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 38, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 47, 1000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 39, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 40, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 9, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 48, 1045000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 41, 975000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 49, 1065000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 50, 1075000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 42, 1010000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 43, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 51, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 10, 200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 44, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34550, 2, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 11, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 52, 1120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 45, 1080000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 53, 1145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 54, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 46, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 55, 1190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 47, 1120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 56, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 48, 1130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 57, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 58, 1265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 59, 1290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 60, 1370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 61, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 60, 1320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 62, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 51, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 13, 265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 61, 1355000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 53, 1250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 10, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 65, 1480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 66, 1520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 54, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 55, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 7, 125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 56, 1290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 11, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 57, 1305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 67, 1540000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 68, 1570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 69, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 56, 1330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 69, 1520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 71, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 58, 1385000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 59, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 60, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 15, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34550, 2, 3, 55000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 70, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 71, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 72, 1590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 73, 1615000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 61, 1455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 74, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 2, 16, 330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 62, 1480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 75, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 1, 76, 1710000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 13, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 65, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 8, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 66, 1555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 79, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 80, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 9, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34700, 2, 10, 170000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34600, 2, 14, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 34650, 1, 67, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 81, 1885000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34650, 1, 82, 1915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 34600, 1, 67, 1620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());

  cout << endl << endl << "Security code = 48" << endl;
  cout << osob.Dump(OrderBook::BID);
  cout << osob.Dump(OrderBook::ASK);


  //Security Code 49
  osob.Reset();
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 17240, 1, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 17220, 2, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 6, 190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 3, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 4, 115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 7, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 8, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 5, 145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 17260, 2, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 6, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 7, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 9, 270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 8, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 2, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 9, 250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 10, 285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 11, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 10, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 11, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 12, 320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 12, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 13, 345000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 13, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 14, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 15, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 16, 445000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 14, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 15, 405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 3, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 17, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 16, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 18, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 4, 120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 19, 550000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 3, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 17, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 18, 500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 20, 605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 5, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 21, 630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 4, 75000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 22, 665000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 19, 520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 23, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 24, 705000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 20, 555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 21, 575000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 5, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 25, 720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 22, 600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 26, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 23, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 24, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 27, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 28, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 29, 805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 26, 745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 27, 775000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 6, 155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 30, 835000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 31, 850000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 28, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 29, 815000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 33, 895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 34, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 7, 185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 30, 865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 35, 950000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 31, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 36, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 37, 985000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 38, 1010000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 32, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 33, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 39, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 34, 975000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 40, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 41, 1070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 35, 995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 36, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 37, 1070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 38, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 43, 1090000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 39, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 40, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 44, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 6, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 8, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 9, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 41, 1190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 42, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 45, 1125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 46, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 47, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 48, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 49, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 43, 1250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 44, 1270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 50, 1225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 45, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 46, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 47, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 51, 1260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 48, 1375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 49, 1395000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 50, 1420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 52, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 53, 1310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 54, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 51, 1455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 10, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 52, 1475000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 53, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 54, 1535000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 55, 1565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 56, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 55, 1405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 57, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 11, 315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 7, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 58, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 56, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 57, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 59, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 58, 1515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 12, 340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 60, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 59, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 61, 1745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 60, 1570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 62, 1770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 61, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 63, 1790000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 64, 1810000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 62, 1605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 65, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 8, 175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 63, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 9, 195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 66, 1870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 67, 1890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 64, 1660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 68, 1915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 65, 1690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 69, 1935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 70, 1945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 10, 220000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 71, 1960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 66, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 67, 1760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 72, 1970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 73, 1995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 68, 1790000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 69, 1800000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 70, 1825000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 74, 2015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 71, 1845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 75, 2040000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 76, 2060000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 11, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 13, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 14, 400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 77, 2110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 12, 265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 13, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 78, 2130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 79, 2155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 72, 1880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 80, 2185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 73, 1890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 81, 2210000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 74, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 82, 2225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 15, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 83, 2250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 84, 2275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 75, 1950000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 76, 1980000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 77, 2020000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 78, 2035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 85, 2290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 14, 320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 79, 2070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 80, 2085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 86, 2315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 15, 360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 87, 2360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 88, 2390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 81, 2120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 82, 2150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 89, 2425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 83, 2175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 90, 2440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 84, 2190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 85, 2205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 86, 2230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 91, 2500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 16, 465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 87, 2255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 88, 2280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 92, 2545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 89, 2310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 90, 2330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 91, 2370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 93, 2580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 94, 2605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 92, 2380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 95, 2610000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 96, 2620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 17, 495000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 97, 2660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 18, 525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 93, 2400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 98, 2695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 94, 2440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 99, 2700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 100, 2725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 95, 2455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 96, 2470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 97, 2480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 98, 2500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 17240, 1, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 17220, 2, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 6, 190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 3, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 4, 115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 7, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 8, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 5, 145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 17260, 2, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 6, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 7, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 9, 270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 8, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 2, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 9, 250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 10, 285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 11, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 10, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 11, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 12, 320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 12, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 13, 345000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 13, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 14, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 15, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 16, 445000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 14, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 15, 405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 3, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 17, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 16, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 18, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 4, 120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 19, 550000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 3, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 17, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 18, 500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 20, 605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 5, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 21, 630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 4, 75000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 22, 665000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 19, 520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 23, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 24, 705000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 20, 555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 21, 575000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 5, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 25, 720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 22, 600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 26, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 23, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 24, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 27, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 28, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 29, 805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 26, 745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 27, 775000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 6, 155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 30, 835000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 31, 850000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 28, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 29, 815000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 33, 895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 34, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 7, 185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 30, 865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 35, 950000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 31, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 36, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 37, 985000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 38, 1010000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 32, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 33, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 39, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 34, 975000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 40, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 41, 1070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 35, 995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 36, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 37, 1070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 38, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 43, 1090000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 39, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 40, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 44, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 6, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 8, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 9, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 41, 1190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 42, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 45, 1125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 46, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 47, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 48, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 49, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 43, 1250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 44, 1270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 50, 1225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 45, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 46, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 47, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 51, 1260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 48, 1375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 49, 1395000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 50, 1420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 52, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 53, 1310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 54, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 51, 1455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 10, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 52, 1475000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 53, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 54, 1535000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 55, 1565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 56, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 55, 1405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 57, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 11, 315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 7, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 58, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 56, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 57, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 59, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 58, 1515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 12, 340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 60, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 59, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 61, 1745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 60, 1570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 62, 1770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 61, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 63, 1790000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 64, 1810000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 62, 1605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 65, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 8, 175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 63, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 9, 195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 66, 1870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 67, 1890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 64, 1660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 68, 1915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 65, 1690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 69, 1935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 70, 1945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 10, 220000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 71, 1960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 66, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 67, 1760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 72, 1970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 73, 1995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 68, 1790000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 69, 1800000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 70, 1825000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 74, 2015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 71, 1845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 75, 2040000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 76, 2060000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 11, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 13, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 14, 400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 77, 2110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 12, 265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 13, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 78, 2130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 79, 2155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 72, 1880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 80, 2185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 73, 1890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 81, 2210000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 74, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 82, 2225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 15, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 83, 2250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 84, 2275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 75, 1950000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 76, 1980000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 77, 2020000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 78, 2035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 85, 2290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 14, 320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 79, 2070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 80, 2085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 86, 2315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 15, 360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 87, 2360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 88, 2390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 81, 2120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 82, 2150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 89, 2425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 83, 2175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 90, 2440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 84, 2190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 85, 2205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 86, 2230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 91, 2500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 16, 465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 87, 2255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 88, 2280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 92, 2545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 89, 2310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 90, 2330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 91, 2370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 93, 2580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 94, 2605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 92, 2380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 95, 2610000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 96, 2620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 17, 495000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 97, 2660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 18, 525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 93, 2400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 98, 2695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 94, 2440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 99, 2700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 100, 2725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 95, 2455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 96, 2470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 97, 2480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 98, 2500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 17240, 1, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 17220, 2, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 6, 190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 3, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 4, 115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 7, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 8, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 5, 145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 17260, 2, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 6, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 7, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 9, 270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 8, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 2, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 9, 250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 10, 285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 11, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 10, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 11, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 12, 320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 12, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 13, 345000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 13, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 14, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 15, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 16, 445000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 14, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 15, 405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 3, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 17, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 16, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 18, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 4, 120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 19, 550000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 3, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 17, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 18, 500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 20, 605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 5, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 21, 630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 4, 75000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 22, 665000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 19, 520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 23, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 24, 705000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 20, 555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 21, 575000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 5, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 25, 720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 22, 600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 26, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 23, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 24, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 27, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 28, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 29, 805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 26, 745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 27, 775000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 6, 155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 30, 835000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 31, 850000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 28, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 29, 815000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 33, 895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 34, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 7, 185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 30, 865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 35, 950000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 31, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 36, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 37, 985000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 38, 1010000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 32, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 33, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 39, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 34, 975000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 40, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 41, 1070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 35, 995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 36, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 37, 1070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 38, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 43, 1090000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 39, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 40, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 44, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 6, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 8, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 9, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 41, 1190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 42, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 45, 1125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 46, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 47, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 48, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 49, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 43, 1250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 44, 1270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 50, 1225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 45, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 46, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 47, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 51, 1260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 48, 1375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 49, 1395000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 50, 1420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 52, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 53, 1310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 54, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 51, 1455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 10, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 52, 1475000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 53, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 54, 1535000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 55, 1565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 56, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 55, 1405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 57, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 11, 315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 7, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 58, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 56, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 57, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 59, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 58, 1515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 12, 340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 60, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 59, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 61, 1745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 60, 1570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 62, 1770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 61, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 63, 1790000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 64, 1810000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 62, 1605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 65, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 8, 175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 63, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 9, 195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 66, 1870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 67, 1890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 64, 1660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 68, 1915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 65, 1690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 69, 1935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 70, 1945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 10, 220000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 71, 1960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 66, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 67, 1760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 72, 1970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 73, 1995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 68, 1790000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 69, 1800000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 70, 1825000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 74, 2015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 71, 1845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 75, 2040000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 76, 2060000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 11, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 13, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 14, 400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 77, 2110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 12, 265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 13, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 78, 2130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 79, 2155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 72, 1880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 80, 2185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 73, 1890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 81, 2210000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 74, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 82, 2225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 15, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 83, 2250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 84, 2275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 75, 1950000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 76, 1980000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 77, 2020000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 78, 2035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 85, 2290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 14, 320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 79, 2070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 80, 2085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 86, 2315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 15, 360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 87, 2360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 88, 2390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 81, 2120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 82, 2150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 89, 2425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 83, 2175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 90, 2440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 84, 2190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 85, 2205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 86, 2230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 91, 2500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 16, 465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 87, 2255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 88, 2280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 92, 2545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 89, 2310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 90, 2330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 91, 2370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 93, 2580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 94, 2605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 92, 2380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 95, 2610000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 96, 2620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 17, 495000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 97, 2660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 18, 525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 93, 2400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 98, 2695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 94, 2440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 99, 2700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 100, 2725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 95, 2455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 96, 2470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 97, 2480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 98, 2500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 17240, 1, 1, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 17220, 2, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 6, 190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 3, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 4, 115000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 7, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 8, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 5, 145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 17260, 2, 1, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 6, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 7, 180000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 9, 270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 8, 205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 2, 70000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 9, 250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 10, 285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 11, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 10, 275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 11, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 12, 320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 2, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 12, 335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 13, 345000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 13, 365000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 14, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 15, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 16, 445000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 14, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 15, 405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 3, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 17, 485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 16, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 18, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 4, 120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 19, 550000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 3, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 17, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 18, 500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 20, 605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 5, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 21, 630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 4, 75000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 22, 665000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 19, 520000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 23, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 24, 705000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 20, 555000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 21, 575000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 5, 90000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 25, 720000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 22, 600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 26, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 23, 640000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 24, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 27, 760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 28, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 29, 805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 26, 745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 27, 775000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 6, 155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 30, 835000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 31, 850000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 28, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 29, 815000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 33, 895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 34, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 7, 185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 30, 865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 35, 950000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 31, 890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 36, 960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 37, 985000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 38, 1010000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 32, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 33, 935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 39, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 34, 975000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 40, 1050000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 41, 1070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 35, 995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 36, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 37, 1070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 38, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 43, 1090000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 39, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 40, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 44, 1100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 6, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 8, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 9, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 41, 1190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 42, 1215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 45, 1125000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 46, 1135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 47, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 48, 1165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 49, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 43, 1250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 44, 1270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 50, 1225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 45, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 46, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 47, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 51, 1260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 48, 1375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 49, 1395000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 50, 1420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 52, 1295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 53, 1310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 54, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 51, 1455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 10, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 52, 1475000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 53, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 54, 1535000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 55, 1565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 56, 1580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 55, 1405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 57, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 11, 315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 7, 150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 58, 1650000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 56, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 57, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 59, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 58, 1515000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 12, 340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 60, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 59, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 61, 1745000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 60, 1570000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 62, 1770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 61, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 63, 1790000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 64, 1810000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 62, 1605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 65, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 8, 175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 63, 1625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 9, 195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 66, 1870000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 67, 1890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 64, 1660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 68, 1915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 65, 1690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 69, 1935000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 70, 1945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 10, 220000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 71, 1960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 66, 1715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 67, 1760000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 72, 1970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 73, 1995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 68, 1790000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 69, 1800000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 70, 1825000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 74, 2015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 71, 1845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 75, 2040000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 76, 2060000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 11, 225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 13, 375000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 14, 400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 77, 2110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 12, 265000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 13, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 78, 2130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 79, 2155000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 72, 1880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 80, 2185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 73, 1890000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 81, 2210000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 74, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 82, 2225000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 15, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 83, 2250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 84, 2275000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 75, 1950000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 76, 1980000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 77, 2020000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 78, 2035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 85, 2290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 14, 320000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 79, 2070000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 80, 2085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 86, 2315000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17220, 2, 15, 360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 87, 2360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 88, 2390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 81, 2120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 82, 2150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 89, 2425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 83, 2175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 90, 2440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 84, 2190000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 85, 2205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 86, 2230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 91, 2500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 16, 465000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 87, 2255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 88, 2280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 92, 2545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 89, 2310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 90, 2330000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 91, 2370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 93, 2580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 94, 2605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 92, 2380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 95, 2610000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 96, 2620000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 17, 495000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 97, 2660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17260, 2, 18, 525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 93, 2400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 98, 2695000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 94, 2440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 99, 2700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 17240, 1, 100, 2725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 95, 2455000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 96, 2470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 97, 2480000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 17240, 1, 98, 2500000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());


  cout << endl << endl << "Security code = 49" << endl;
  cout << osob.Dump(OrderBook::BID);
  cout << osob.Dump(OrderBook::ASK);



  //Security Code 44
  osob.Reset();

  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1280, 1, 1, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1270, 1, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 3, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1260, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 2, 2, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 4, 75000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 7, 170000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 1, 9, 250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 8, 235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 9, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 9, 245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 10, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 5, 100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 11, 325000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 12, 380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 10, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 11, 325000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 13, 405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 12, 360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 14, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 15, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 16, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 6, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 13, 380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 14, 390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 17, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 18, 535000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 15, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 19, 565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 7, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 20, 590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 16, 440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 17, 460000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 21, 605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 22, 635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 23, 660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 24, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 26, 730000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 18, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 19, 545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 2, 4, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 21, 580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1290, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 27, 700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 22, 615000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 23, 625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 28, 715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 2, 2, 45000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 29, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 31, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 2, 9, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 26, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 27, 750000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 28, 765000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 29, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 31, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 30, 810000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 31, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 32, 805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 31, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 32, 880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 33, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 35, 880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 34, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 36, 900000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 35, 930000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 37, 925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 4, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 36, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 38, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 39, 995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 37, 990000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 40, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 41, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 38, 1005000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 39, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 40, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 5, 120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 43, 1130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 41, 1060000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 44, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 45, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 43, 1095000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 44, 1105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 46, 1205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 45, 1145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 46, 1175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1300, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 47, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 47, 1200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 48, 1230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 49, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 50, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 51, 1310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 48, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 49, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 50, 1305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 51, 1335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 52, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 52, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 53, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 54, 1380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 55, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 56, 1410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 57, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 58, 1470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 59, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 60, 1525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 6, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 53, 1370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 61, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 62, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 63, 1605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 55, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1300, 1, 56, 1425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 2, 10, 270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 56, 1450000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 2, 11, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1300, 1, 57, 1440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 57, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 57, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 58, 1530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 7, 185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 59, 1565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 69, 1780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 70, 1820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 71, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 72, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 60, 1600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 73, 1895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 61, 1630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 74, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 63, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 8, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 76, 1960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 77, 1970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 78, 2000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 79, 2030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1280, 1, 1, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1270, 1, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 3, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1260, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 2, 2, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 4, 75000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 7, 170000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 1, 9, 250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 8, 235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 9, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 9, 245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 10, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 5, 100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 11, 325000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 12, 380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 10, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 11, 325000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 13, 405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 12, 360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 14, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 15, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 16, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 6, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 13, 380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 14, 390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 17, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 18, 535000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 15, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 19, 565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 7, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 20, 590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 16, 440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 17, 460000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 21, 605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 22, 635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 23, 660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 24, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 26, 730000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 18, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 19, 545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 2, 4, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 21, 580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1290, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 27, 700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 22, 615000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 23, 625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 28, 715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 2, 2, 45000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 29, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 31, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 2, 9, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 26, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 27, 750000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 28, 765000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 29, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 31, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 30, 810000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 31, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 32, 805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 31, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 32, 880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 33, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 35, 880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 34, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 36, 900000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 35, 930000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 37, 925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 4, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 36, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 38, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 39, 995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 37, 990000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 40, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 41, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 38, 1005000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 39, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 40, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 5, 120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 43, 1130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 41, 1060000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 44, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 45, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 43, 1095000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 44, 1105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 46, 1205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 45, 1145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 46, 1175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1300, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 47, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 47, 1200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 48, 1230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 49, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 50, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 51, 1310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 48, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 49, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 50, 1305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 51, 1335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 52, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 52, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 53, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 54, 1380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 55, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 56, 1410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 57, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 58, 1470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 59, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 60, 1525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 6, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 53, 1370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 61, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 62, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 63, 1605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 55, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1300, 1, 56, 1425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 2, 10, 270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 56, 1450000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 2, 11, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1300, 1, 57, 1440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 57, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 57, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 58, 1530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 7, 185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 59, 1565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 69, 1780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 70, 1820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 71, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 72, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 60, 1600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 73, 1895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 61, 1630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 74, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 63, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 8, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 76, 1960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 77, 1970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 78, 2000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 79, 2030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1280, 1, 1, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1270, 1, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 3, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1260, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 2, 2, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 6, 165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 1, 8, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 4, 75000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 7, 170000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 8, 210000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 1, 9, 250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 1, 10, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 8, 235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 9, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 9, 245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 10, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 5, 100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 11, 325000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 12, 380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 10, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 11, 325000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 13, 405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 12, 360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 14, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 15, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 16, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 6, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 13, 380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 14, 390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 17, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 18, 535000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 15, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 19, 565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 7, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 20, 590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 16, 440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 17, 460000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 21, 605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 22, 635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 23, 660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 24, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 26, 730000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 18, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 19, 545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 2, 4, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 27, 770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 21, 580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1290, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 27, 700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 22, 615000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 23, 625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 28, 715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 2, 2, 45000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 29, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 30, 765000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 31, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 32, 895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 2, 9, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 26, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 27, 750000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 28, 765000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 29, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 31, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 30, 810000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 31, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 32, 805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 31, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 32, 880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 33, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 35, 880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 34, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 36, 900000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 35, 930000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 37, 925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 4, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 36, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 38, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 39, 995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 37, 990000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 40, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 41, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 38, 1005000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 39, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 40, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 5, 120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 43, 1130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 41, 1060000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 44, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 45, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 43, 1095000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 44, 1105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 46, 1205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 45, 1145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 46, 1175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1300, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 47, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 47, 1200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 48, 1230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 49, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 50, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 51, 1310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 48, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 49, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 50, 1305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 51, 1335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 52, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 52, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 53, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 54, 1380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 55, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 56, 1410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 57, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 58, 1470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 59, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 60, 1525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 6, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 53, 1370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 61, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 62, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 63, 1605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 64, 1635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 55, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1300, 1, 56, 1425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 2, 10, 270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 56, 1450000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 2, 11, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1300, 1, 57, 1440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 57, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 58, 1510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 57, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 58, 1530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 7, 185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 59, 1565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 69, 1780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 70, 1820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 71, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 72, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 60, 1600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 73, 1895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 61, 1630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 74, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 75, 1945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 64, 1700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 63, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 8, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 76, 1960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 77, 1970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 78, 2000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 79, 2030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1280, 1, 1, 40000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 2, 50000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1270, 1, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 3, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1260, 2, 1, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 2, 2, 60000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 6, 165000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 1, 8, 230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 4, 75000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 7, 170000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 8, 210000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 1, 9, 250000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 1, 10, 290000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 8, 235000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 9, 260000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 9, 245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 10, 295000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 5, 100000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 11, 325000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 12, 380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 10, 280000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 11, 325000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 13, 405000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 12, 360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 14, 425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 15, 430000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 16, 470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 6, 110000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 13, 380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 14, 390000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 17, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 18, 535000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 15, 420000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 19, 565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 2, 7, 135000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 20, 590000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 16, 440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 17, 460000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 21, 605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 22, 635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 23, 660000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 24, 670000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 26, 730000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 18, 510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1270, 1, 19, 545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1260, 2, 4, 105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 27, 770000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 21, 580000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1290, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 27, 700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 22, 615000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 23, 625000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 28, 715000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 2, 2, 45000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 29, 735000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 30, 765000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 31, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 1, 32, 895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1270, 2, 9, 300000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 25, 690000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 26, 725000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 27, 750000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 28, 765000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 29, 785000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 31, 795000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 30, 810000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 31, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1280, 1, 32, 805000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 1, 32, 875000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 31, 845000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 32, 880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 33, 905000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 35, 880000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 34, 915000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 36, 900000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 35, 930000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 3, 80000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 37, 925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 4, 95000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 36, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 38, 965000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 39, 995000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 37, 990000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 40, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 41, 1035000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 38, 1005000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 39, 1015000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 40, 1030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 5, 120000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 43, 1130000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 41, 1060000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 44, 1150000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 45, 1195000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 42, 1085000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 43, 1095000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 44, 1105000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 46, 1205000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 45, 1145000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 46, 1175000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1300, 2, 1, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 47, 1245000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 47, 1200000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 48, 1230000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 49, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 50, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 51, 1310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 48, 1255000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 49, 1285000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 50, 1305000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 51, 1335000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 52, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 52, 1340000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 53, 1360000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 54, 1380000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 55, 1400000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 56, 1410000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 57, 1435000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 58, 1470000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 59, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 60, 1525000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 6, 160000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 53, 1370000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 61, 1545000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 62, 1585000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 63, 1605000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 64, 1635000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 55, 1415000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1300, 1, 56, 1425000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 2, 10, 270000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 56, 1450000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 2, 11, 310000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1300, 1, 57, 1440000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 57, 1485000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 58, 1510000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 57, 1505000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 58, 1530000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 7, 185000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 59, 1565000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 69, 1780000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 70, 1820000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 71, 1840000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 72, 1865000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 60, 1600000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 73, 1895000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 61, 1630000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 74, 1925000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 75, 1945000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1300, 1, 64, 1700000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1290, 1, 63, 1685000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1280, 2, 8, 215000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 76, 1960000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 77, 1970000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 78, 2000000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1290, 1, 79, 2030000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());

  cout << endl << endl << "Security code = 44" << endl;
  cout << osob.Dump(OrderBook::BID);
  cout << osob.Dump(OrderBook::ASK);



  //Security Code 8
  osob.Reset();

  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5730, 1, 1, 3000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5730, 1, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5730, 1, 3, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5700, 2, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5680, 3, 1, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5640, 4, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5740, 1, 1, 5000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5750, 2, 1, 3000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5760, 3, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5770, 4, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5730, 1, 1, 3000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5730, 1, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5730, 1, 3, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5700, 2, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5680, 3, 1, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5640, 4, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5740, 1, 1, 5000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5750, 2, 1, 3000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5760, 3, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5770, 4, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5730, 1, 1, 3000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5730, 1, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5730, 1, 3, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5700, 2, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5680, 3, 1, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5640, 4, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5740, 1, 1, 5000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5750, 2, 1, 3000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5760, 3, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5770, 4, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5730, 1, 1, 3000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5730, 1, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 5730, 1, 3, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5700, 2, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5680, 3, 1, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 5640, 4, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5740, 1, 1, 5000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5750, 2, 1, 3000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5760, 3, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 5770, 4, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());


  cout << endl << endl << "Security code = 8" << endl;
  cout << osob.Dump(OrderBook::BID);
  cout << osob.Dump(OrderBook::ASK);






  //Security Code 951
  osob.Reset();

  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1510, 1, 1, 4500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 2, 5500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 3, 14000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 4, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1490, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1480, 2, 1, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 5, 18000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 6, 23000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1520, 2, 1, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 2, 10500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 3, 16500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1470, 2, 1, 1500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 3, 13000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 4, 17000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 2, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 6500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 2, 5, 17500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 2, 2, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1470, 1, 1, 500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1460, 2, 1, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 2, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 2, 6, 25500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 2, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 3, 9000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 3, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 3, 7, 26000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 4, 8500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 1, 1, 1500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 4500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 3, 7500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 1, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 4, 11500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 3, 9500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 2, 5, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 4, 12500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 7, 22500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1510, 1, 1, 4500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 2, 5500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 3, 14000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 4, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1490, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1480, 2, 1, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 5, 18000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 6, 23000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1520, 2, 1, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 2, 10500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 3, 16500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1470, 2, 1, 1500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 3, 13000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 4, 17000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 2, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 6500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 2, 5, 17500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 2, 2, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1470, 1, 1, 500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1460, 2, 1, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 2, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 2, 6, 25500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 2, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 3, 9000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 3, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 3, 7, 26000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 4, 8500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 1, 1, 1500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 4500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 3, 7500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 1, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 4, 11500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 3, 9500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 2, 5, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 4, 12500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 7, 22500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 18500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 5, 17500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 10500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 7500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 6500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 6000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 3, 6, 14500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 17000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1500, 2, 1, 3500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1500, 2, 2, 5500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 22000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 22000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 17500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 4, 21500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 6, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 26500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 5, 26000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 7, 33000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 5, 23500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 5, 18500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 31500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 5, 16500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 30500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 4, 16000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 14000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 27500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 24500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 3, 7, 18500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 25500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 13000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 4, 14500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 7, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1500, 2, 3, 9500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1500, 2, 4, 11000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 4, 22000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1510, 1, 1, 4500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 2, 5500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 3, 14000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 4, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1490, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1480, 2, 1, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 5, 18000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 6, 23000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1520, 2, 1, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 2, 10500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 3, 16500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1470, 2, 1, 1500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 3, 13000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 4, 17000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 2, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 6500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 2, 5, 17500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 2, 2, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1470, 1, 1, 500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1460, 2, 1, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 2, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 2, 6, 25500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 2, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 3, 9000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 3, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 3, 7, 26000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 4, 8500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 1, 1, 1500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 4500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 3, 7500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 1, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 4, 11500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 3, 9500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 2, 5, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 4, 12500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 7, 22500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1510, 1, 1, 4500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 2, 5500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 3, 14000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 4, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1490, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1480, 2, 1, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 5, 18000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1510, 1, 6, 23000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1520, 2, 1, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 2, 10500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 3, 16500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1470, 2, 1, 1500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 3, 13000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 4, 17000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 2, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 6500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 2, 5, 17500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 2, 2, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1470, 1, 1, 500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 1460, 2, 1, 2000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 2, 2, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 2, 6, 25500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 2, 4000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 3, 9000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 3, 7000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 3, 7, 26000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 1, 4, 8500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 1, 1, 1500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 4500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 2, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 3, 7500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1480, 1, 1, 1000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 4, 11500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 3, 9500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 2, 5, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 1, 4, 12500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 7, 22500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 18500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 5, 17500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 10500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 7500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 6500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 6000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 3, 6, 14500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 30000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 1, 2500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 17000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 1500, 2, 1, 3500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1500, 2, 2, 5500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 22000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 22000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 2, 15000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 17500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 4, 21500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 6, 20000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 26500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 5, 26000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 7, 33000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 5, 23500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 5, 18500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 31500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 5, 16500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 30500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 10000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 4, 16000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 14000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 27500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 5, 24500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1460, 3, 7, 18500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 6, 25500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 3, 13000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1480, 1, 4, 14500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 1470, 2, 7, 25000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1500, 2, 3, 9500)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1500, 2, 4, 11000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 1490, 1, 4, 22000)); CPPUNIT_ASSERT(osob.AssessPriceIntegrity());



  cout << endl << endl << "Security code = 951" << endl;
  cout << osob.Dump(OrderBook::BID);
  cout << osob.Dump(OrderBook::ASK);



  //--------------------------------------------------
  // DeleteTopLevels()
  //--------------------------------------------------
  osob.Reset();

  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9730, 1, 1, 700));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9720, 2, 1, 350));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9710, 3, 1, 150));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9700, 4, 1, 250));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9690, 5, 1, 100));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9680, 6, 1, 150));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9670, 7, 1,  50));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9660, 8, 1, 200));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9650, 9, 1, 100));

  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9760, 1, 1, 500));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9770, 2, 1, 300));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9780, 3, 1, 100));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9790, 4, 1, 150));

  CPPUNIT_ASSERT(!osob.DeleteTopLevels(OrderBook::ASK, 100));
  CPPUNIT_ASSERT(!osob.DeleteTopLevels(OrderBook::ASK, 0));
  CPPUNIT_ASSERT(!osob.DeleteTopLevels(OrderBook::ASK, -10));

  CPPUNIT_ASSERT(osob.DeleteTopLevels(OrderBook::ASK, 3));
  CPPUNIT_ASSERT(osob.DeleteTopLevels(OrderBook::BID, 1));

  osob.Dump(OrderBook::BID,iaBid);
  osob.Dump(OrderBook::ASK,iaAsk);

  int iaBid15 [10][3] = {
    {1,350,9720}
    ,{1,150,9710}
    ,{1,250,9700}
    ,{1,100,9690}
    ,{1,150,9680}
    ,{1, 50,9670}
    ,{1,200,9660}
    ,{1,100,9650}
    ,{0,  0,   0}
    ,{0,  0,   0}
  };
  int iaAsk15 [10][3] = {
    {1,150,9790}
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

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid15, iaAsk15));

  return;
}

void TestOrderBook::setUp()
{
  return;
}


void TestOrderBook::tearDown()
{
  return;
}

CppUnit::Test *TestOrderBook::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestOrderBook");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestOrderBook>(
      "TestOrderBook", 
      &TestOrderBook::RunTest));

  return suiteOfTests;
}
