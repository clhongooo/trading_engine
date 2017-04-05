#include "UTest.h"
#include "Util/Acct.h"
#include <iostream>
#include <string>

int TestAcct()
{
	UTest ut;

	Acct ac;
	ut.Assert(ac.CPnL() == 0, __FILE__,__FUNCTION__,__LINE__);

  ac.Trade("HSIU3",3,20000);
	ut.Assert(ac.Pos("HSIU3") == 3, __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIU3",-2,20100);
	ut.Assert(ac.Pos("HSIU3") == 1, __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIU3",1,20000);
	ut.Assert(ac.Pos("HSIU3") == 2, __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIU3",-2,20200);
	ut.Assert(ac.Pos("HSIU3") == 0, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(ac.CPnL() == 600, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(ac.CPnL() == 600, __FILE__,__FUNCTION__,__LINE__);

  ac.Trade("HHIU3",-3,20000);
  ac.Trade("HHIU3",2,20100);
  ac.Trade("HHIU3",-1,20000);
  ac.Trade("HHIU3",2,20200);
	ut.Assert(ac.CPnL() == 0, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(ac.CPnL() == 0, __FILE__,__FUNCTION__,__LINE__);

  ac.Trade("HSIU3",-3,20000);
	ut.Assert(ac.Pos("HSIU3") == -3, __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIU3",2,20100);
	ut.Assert(ac.Pos("HSIU3") == -1, __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIU3",-1,20000);
	ut.Assert(ac.Pos("HSIU3") == -2, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(ac.CPnL() == 0, __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIU3",2,20200);
	ut.Assert(ac.Pos("HSIU3") == 0, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(ac.CPnL() == -600, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(ac.CPnL() == -600, __FILE__,__FUNCTION__,__LINE__);


  double mtm_pnl = 0;
  ac.Reset();
  ac.Trade("HSIc1",2,20000);
	ut.AssertF(ac.Get_MTM_CPnL(mtm_pnl), __FILE__,__FUNCTION__,__LINE__);

  ac.ProvidePriceForMTM("HSIc1",21000);
	ut.Assert(ac.Get_MTM_CPnL(mtm_pnl), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(mtm_pnl == 2000, __FILE__,__FUNCTION__,__LINE__);

  ac.Trade("HSIc1",-2,20000);
  ac.ProvidePriceForMTM("HSIc1",30000);
	ut.Assert(ac.Get_MTM_CPnL(mtm_pnl), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(mtm_pnl == 0, __FILE__,__FUNCTION__,__LINE__);

  ac.Trade("HSIc1",-2,20000);
  ac.ProvidePriceForMTM("HSIc1",22000);
	ut.Assert(ac.Get_MTM_CPnL(mtm_pnl), __FILE__,__FUNCTION__,__LINE__);

	ut.Assert(mtm_pnl == -4000, __FILE__,__FUNCTION__,__LINE__);
  ac.ProvidePriceForMTM("HSIc1",20000);
	ut.Assert(ac.Get_MTM_CPnL(mtm_pnl), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(mtm_pnl == 0, __FILE__,__FUNCTION__,__LINE__);
  ac.ProvidePriceForMTM("HSIc1",19000);
	ut.Assert(ac.Get_MTM_CPnL(mtm_pnl), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(mtm_pnl == 2000, __FILE__,__FUNCTION__,__LINE__);

  //--------------------------------------------------
  ac.Reset();
  ac.Trade("HSIc1",-3,25000); ac.ProvidePriceForMTM("HSIc1",25000); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == 0      , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",3 ,25024); ac.ProvidePriceForMTM("HSIc1",25024); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -72    , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,24810); ac.ProvidePriceForMTM("HSIc1",24810); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -72    , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-7,24833); ac.ProvidePriceForMTM("HSIc1",24833); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -118   , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-6,24954); ac.ProvidePriceForMTM("HSIc1",24954); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -1207  , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",4 ,24968); ac.ProvidePriceForMTM("HSIc1",24968); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -1417  , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-3,24846); ac.ProvidePriceForMTM("HSIc1",24846); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -75    , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-9,25082); ac.ProvidePriceForMTM("HSIc1",25082); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -3379  , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,24832); ac.ProvidePriceForMTM("HSIc1",24832); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == 2371   , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-3,24993); ac.ProvidePriceForMTM("HSIc1",24993); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -1654  , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-7,25087); ac.ProvidePriceForMTM("HSIc1",25087); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -4286  , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-9,25182); ac.ProvidePriceForMTM("HSIc1",25182); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -7611  , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25352); ac.ProvidePriceForMTM("HSIc1",25352); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -15091 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-6,25473); ac.ProvidePriceForMTM("HSIc1",25473); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -20657 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",4 ,25245); ac.ProvidePriceForMTM("HSIc1",25245); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -8801  , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",1 ,25037); ac.ProvidePriceForMTM("HSIc1",25037); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == 1183   , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-1,25182); ac.ProvidePriceForMTM("HSIc1",25182); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -5632  , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25263); ac.ProvidePriceForMTM("HSIc1",25263); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -9520  , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",6 ,25294); ac.ProvidePriceForMTM("HSIc1",25294); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -11070 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-7,25285); ac.ProvidePriceForMTM("HSIc1",25285); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -10674 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",9 ,25383); ac.ProvidePriceForMTM("HSIc1",25383); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -15672 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",4 ,25284); ac.ProvidePriceForMTM("HSIc1",25284); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -11514 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-3,25470); ac.ProvidePriceForMTM("HSIc1",25470); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -18582 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25575); ac.ProvidePriceForMTM("HSIc1",25575); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -22887 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",7 ,25702); ac.ProvidePriceForMTM("HSIc1",25702); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28348 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25801); ac.ProvidePriceForMTM("HSIc1",25801); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -31912 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25782); ac.ProvidePriceForMTM("HSIc1",25782); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -31190 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",2 ,25598); ac.ProvidePriceForMTM("HSIc1",25598); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -23830 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-3,25751); ac.ProvidePriceForMTM("HSIc1",25751); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -29644 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",9 ,25724); ac.ProvidePriceForMTM("HSIc1",25724); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28537 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-9,25843); ac.ProvidePriceForMTM("HSIc1",25843); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -32345 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",7 ,26079); ac.ProvidePriceForMTM("HSIc1",26079); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -42021 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",3 ,25842); ac.ProvidePriceForMTM("HSIc1",25842); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -33963 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",4 ,26033); ac.ProvidePriceForMTM("HSIc1",26033); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -39884 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",2 ,25919); ac.ProvidePriceForMTM("HSIc1",25919); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -36806 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-9,25904); ac.ProvidePriceForMTM("HSIc1",25904); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -36431 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",6 ,25762); ac.ProvidePriceForMTM("HSIc1",25762); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -31603 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",6 ,25909); ac.ProvidePriceForMTM("HSIc1",25909); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -35719 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",3 ,25781); ac.ProvidePriceForMTM("HSIc1",25781); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -32903 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",8 ,26028); ac.ProvidePriceForMTM("HSIc1",26028); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -37596 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",5 ,25792); ac.ProvidePriceForMTM("HSIc1",25792); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -35000 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-7,25597); ac.ProvidePriceForMTM("HSIc1",25597); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -33830 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-1,25804); ac.ProvidePriceForMTM("HSIc1",25804); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -36521 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-1,25870); ac.ProvidePriceForMTM("HSIc1",25870); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -37445 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-1,26059); ac.ProvidePriceForMTM("HSIc1",26059); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -40280 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",5 ,26090); ac.ProvidePriceForMTM("HSIc1",26090); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -40776 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",1 ,26123); ac.ProvidePriceForMTM("HSIc1",26123); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -41139 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25909); ac.ProvidePriceForMTM("HSIc1",25909); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -38999 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",9 ,25886); ac.ProvidePriceForMTM("HSIc1",25886); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -38723 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",8 ,25802); ac.ProvidePriceForMTM("HSIc1",25802); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -38471 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-1,25752); ac.ProvidePriceForMTM("HSIc1",25752); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -38721 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",5 ,25562); ac.ProvidePriceForMTM("HSIc1",25562); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -39481 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",7 ,25661); ac.ProvidePriceForMTM("HSIc1",25661); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -38590 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25840); ac.ProvidePriceForMTM("HSIc1",25840); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -35726 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-4,26045); ac.ProvidePriceForMTM("HSIc1",26045); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -32856 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-7,26078); ac.ProvidePriceForMTM("HSIc1",26078); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -32526 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",7 ,26276); ac.ProvidePriceForMTM("HSIc1",26276); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -31932 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-8,26252); ac.ProvidePriceForMTM("HSIc1",26252); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -32172 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",9 ,26049); ac.ProvidePriceForMTM("HSIc1",26049); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -32578 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",7 ,25976); ac.ProvidePriceForMTM("HSIc1",25976); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -33381 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",4 ,25798); ac.ProvidePriceForMTM("HSIc1",25798); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -36585 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",3 ,25549); ac.ProvidePriceForMTM("HSIc1",25549); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -42063 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",3 ,25560); ac.ProvidePriceForMTM("HSIc1",25560); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -41788 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25756); ac.ProvidePriceForMTM("HSIc1",25756); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -36300 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",7 ,25715); ac.ProvidePriceForMTM("HSIc1",25715); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -37366 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-4,25634); ac.ProvidePriceForMTM("HSIc1",25634); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -40039 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",7 ,25598); ac.ProvidePriceForMTM("HSIc1",25598); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -41083 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-3,25826); ac.ProvidePriceForMTM("HSIc1",25826); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -32875 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-6,25905); ac.ProvidePriceForMTM("HSIc1",25905); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -30268 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-8,26081); ac.ProvidePriceForMTM("HSIc1",26081); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -25516 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-8,26294); ac.ProvidePriceForMTM("HSIc1",26294); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -21469 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-1,26241); ac.ProvidePriceForMTM("HSIc1",26241); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -22052 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-5,26225); ac.ProvidePriceForMTM("HSIc1",26225); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -22212 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",2 ,26142); ac.ProvidePriceForMTM("HSIc1",26142); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -22627 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",8 ,25984); ac.ProvidePriceForMTM("HSIc1",25984); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -23733 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-6,25739); ac.ProvidePriceForMTM("HSIc1",25739); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27408 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-1,25608); ac.ProvidePriceForMTM("HSIc1",25608); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28587 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25754); ac.ProvidePriceForMTM("HSIc1",25754); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27419 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25784); ac.ProvidePriceForMTM("HSIc1",25784); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27239 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-2,25610); ac.ProvidePriceForMTM("HSIc1",25610); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27935 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-4,25670); ac.ProvidePriceForMTM("HSIc1",25670); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27815 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-5,25690); ac.ProvidePriceForMTM("HSIc1",25690); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27855 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",4 ,25937); ac.ProvidePriceForMTM("HSIc1",25937); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -29584 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",4 ,26157); ac.ProvidePriceForMTM("HSIc1",26157); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -30244 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",5 ,25973); ac.ProvidePriceForMTM("HSIc1",25973); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -30428 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-1,26140); ac.ProvidePriceForMTM("HSIc1",26140); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -29426 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",4 ,26242); ac.ProvidePriceForMTM("HSIc1",26242); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28916 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-9,26361); ac.ProvidePriceForMTM("HSIc1",26361); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27845 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-3,26143); ac.ProvidePriceForMTM("HSIc1",26143); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27845 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",6 ,26119); ac.ProvidePriceForMTM("HSIc1",26119); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27773 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-4,26197); ac.ProvidePriceForMTM("HSIc1",26197); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27539 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-6,26138); ac.ProvidePriceForMTM("HSIc1",26138); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -27480 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-6,26350); ac.ProvidePriceForMTM("HSIc1",26350); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28964 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",2 ,26329); ac.ProvidePriceForMTM("HSIc1",26329); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28691 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",6 ,26506); ac.ProvidePriceForMTM("HSIc1",26506); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -30638 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",2 ,26338); ac.ProvidePriceForMTM("HSIc1",26338); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -29798 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-6,26219); ac.ProvidePriceForMTM("HSIc1",26219); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -29441 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",9 ,26080); ac.ProvidePriceForMTM("HSIc1",26080); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28190 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",3 ,25857); ac.ProvidePriceForMTM("HSIc1",25857); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28190 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",5 ,25720); ac.ProvidePriceForMTM("HSIc1",25720); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28601 , __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIc1",-8,25672); ac.ProvidePriceForMTM("HSIc1",25672); ac.Get_MTM_CPnL(mtm_pnl);	ut.Assert(mtm_pnl == -28985 , __FILE__,__FUNCTION__,__LINE__);

	ut.Assert(ac.CPnL() == -28985, __FILE__,__FUNCTION__,__LINE__);



  //--------------------------------------------------
	ac.Reset();
	ut.Assert(ac.CPnL() == 0, __FILE__,__FUNCTION__,__LINE__);

  ac.TradeWithTC("HSIU3",3,20000,10);
	ut.Assert(ac.Pos("HSIU3") == 3, __FILE__,__FUNCTION__,__LINE__);
  ac.TradeWithTC("HSIU3",-2,20100,10);
	ut.Assert(ac.Pos("HSIU3") == 1, __FILE__,__FUNCTION__,__LINE__);
  ac.TradeWithTC("HSIU3",1,20000,10);
	ut.Assert(ac.Pos("HSIU3") == 2, __FILE__,__FUNCTION__,__LINE__);
  ac.TradeWithTC("HSIU3",-2,20200,10);
	ut.Assert(ac.Pos("HSIU3") == 0, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(ac.CPnL() == 520, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(ac.CPnL() == 520, __FILE__,__FUNCTION__,__LINE__);



  //--------------------------------------------------
	ac.Reset();
  ac.SetRecord("SPY",3,300);
  ac.SetRecord("HSIF5",-2,-44000);
  //--------------------------------------------------
  ac.Trade("SPY",-3,120);
	ut.Assert(ac.CPnL() == 60, __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIF5",1,23000);
	ut.Assert(ac.CPnL() == 60, __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIF5",1,23000);
	ut.Assert(ac.CPnL() == -1940, __FILE__,__FUNCTION__,__LINE__);



  //--------------------------------------------------
	ac.Reset();
	ut.Assert(ac.CheckIfPosChgd() == false, __FILE__,__FUNCTION__,__LINE__);
  ac.SetRecord("SPY",3,300);
	ut.Assert(ac.CheckIfPosChgd() == true, __FILE__,__FUNCTION__,__LINE__);
  ac.ClearPosChgFlag();
	ut.Assert(ac.CheckIfPosChgd() == false, __FILE__,__FUNCTION__,__LINE__);
  ac.Trade("HSIF5",1,23000);
	ut.Assert(ac.CheckIfPosChgd() == true, __FILE__,__FUNCTION__,__LINE__);
  ac.ClearPosChgFlag();
	ut.Assert(ac.CheckIfPosChgd() == false, __FILE__,__FUNCTION__,__LINE__);


  //--------------------------------------------------
  // Test mtm by symbol
  //--------------------------------------------------
	ac.Reset();
  ac.Trade("SPY",100,200);
  ac.Trade("SPY",-100,210);
  ac.Trade("QQQ",100,200);
  ac.Trade("QQQ",-100,190);
  map<string,double> map_sym_mtm = ac.GetSymbolsAndMTMPnL();
	ut.Assert(map_sym_mtm["SPY"] == 1000, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(map_sym_mtm["QQQ"] == -1000, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(map_sym_mtm.size() == 2, __FILE__,__FUNCTION__,__LINE__);



	ut.PrintResult();

	return 0;
}
