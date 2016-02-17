#include "UTest.h"
#include "Util/SDateTime.h"
#include "Util/STool.h"
#include <iostream>
#include <string>
#include <vector>
#include <deque>

int TestSDT()
{
  UTest ut;

  {
    ut.Assert(STool::Abs(SDateTime::CalcJulianDayNum(2013,1,1)-2456293.520833) < 1,__FILE__,__FUNCTION__,__LINE__);
    // http://mathforum.org/library/drmath/view/62338.html
    ut.Assert(STool::Abs(SDateTime::CalcJulianDayNum(1582,10,15)-2299161) < 2,__FILE__,__FUNCTION__,__LINE__);

    //CalcGregorianDayNum is tested through CalendarDayDiff, because CalcGregorianDayNum is used in CalendarDayDiff

    YMD ymd1;
    YMD ymd2;
    ymd1.Set(2013,9,12); ymd2.Set(2013,9,13); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == 1,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,9, 1); ymd2.Set(2013,9,13); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == 12,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,8,31); ymd2.Set(2013,9,13); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == 13,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,8,21); ymd2.Set(2013,9,13); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == 23,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,8,12); ymd2.Set(2013,9,13); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == 32,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2000,3,20); ymd2.Set(2004,3,21); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == 1462,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(1996,3,20); ymd2.Set(2004,3,21); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == 2923,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,9,13); ymd2.Set(2013,9,12); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == -1,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2004,3,21); ymd2.Set(2000,3,20); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == -1462,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2004,3,21); ymd2.Set(1996,3,20); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == -2923,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2004,3,21); ymd2.Set(2004,3,20); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == -1,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2004,3,21); ymd2.Set(2004,3,21); ut.Assert(SDateTime::CalendarDayDiff(ymd1,ymd2) == 0,__FILE__,__FUNCTION__,__LINE__);

    ut.Assert(SDateTime::IsLeapYear(2000),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsLeapYear(2001),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsLeapYear(2002),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsLeapYear(2003),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::IsLeapYear(2004),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::IsLeapYear(2012),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsLeapYear(2013),__FILE__,__FUNCTION__,__LINE__);


    ut.Assert(SDateTime::IsValidYMD(2004,12,31),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::IsValidYMD(2004,1,1),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::IsValidYMD(2004,2,29),__FILE__,__FUNCTION__,__LINE__);

    ut.Assert(SDateTime::IsValidYMD(2003,2,28),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(2003,2,29),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::IsValidYMD(2003,2,28),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(2003,2,29),__FILE__,__FUNCTION__,__LINE__);

    ut.AssertF(SDateTime::IsValidYMD(2013,12,32),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(2013,1,32),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(2013,13,1),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(2113,11,1),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(2013,11,0),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(2013,11,-1),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(2013,1,0),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(2013,1,-1),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(SDateTime::IsValidYMD(-2013,1,1),__FILE__,__FUNCTION__,__LINE__);


    ut.Assert(SDateTime::DaysInMonth(2004,2) == 29,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::DaysInMonth(2012,2) == 29,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::DaysInMonth(2002,2) == 28,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::DaysInMonth(2013,2) == 28,__FILE__,__FUNCTION__,__LINE__);

    ut.Assert(SDateTime::DaysInYear(2004) == 366,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::DaysInYear(2012) == 366,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::DaysInYear(2002) == 365,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(SDateTime::DaysInYear(2013) == 365,__FILE__,__FUNCTION__,__LINE__);

    ymd1.Set(2013,12,26); ut.Assert(SDateTime::DayOfWeek(ymd1)==4,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,12,28); ut.Assert(SDateTime::DayOfWeek(ymd1)==6,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,12,29); ut.Assert(SDateTime::DayOfWeek(ymd1)==0,__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,12,30); ut.Assert(SDateTime::DayOfWeek(ymd1)==1,__FILE__,__FUNCTION__,__LINE__);

    ymd1.Set(2013,12,26); ut.Assert(SDateTime::IsWeekDay(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,12,28); ut.AssertF(SDateTime::IsWeekDay(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,12,29); ut.AssertF(SDateTime::IsWeekDay(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,12,30); ut.Assert(SDateTime::IsWeekDay(ymd1),__FILE__,__FUNCTION__,__LINE__);


    ymd1.Set(2004, 2,29); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2012, 2,29); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2002, 2,28); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013, 2,28); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);

    ymd1.Set(2004, 2,28); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2012, 2,28); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2002, 2,27); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013, 2,27); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);

    ymd1.Set(2004, 1,31); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2012, 1,31); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2002, 1,31); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013, 1,31); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);

    ymd1.Set(2004,12,31); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2012,12,31); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2002,12,31); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013,12,31); ut.Assert(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);

    ymd1.Set(2004, 3,28); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2012, 3,28); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2002, 3,27); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013, 3,27); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);

    ymd1.Set(2004, 8,28); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2012, 8,28); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2002, 8,27); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);
    ymd1.Set(2013, 8,27); ut.AssertF(SDateTime::IsEndOfMonth(ymd1),__FILE__,__FUNCTION__,__LINE__);


    //-----------------------------------------------------

    YYYYMMDD ymd3;
    ut.AssertF(ymd3.IsValid(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 == YYYYMMDD(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 == YMD(),__FILE__,__FUNCTION__,__LINE__);
    ymd3.Set(2013,12,11);
    YYYYMMDD ymd4(ymd3);
    ut.Assert(ymd4 == YYYYMMDD(20131211),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd4 == YYYYMMDD("20131211"),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd4 == YYYYMMDD("2013-12-11"),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd4.IsValid(),__FILE__,__FUNCTION__,__LINE__);

    ymd3.Set(20120228);
    YYYYMMDD ymd5 = ymd3;
    ut.Assert(ymd5 == YYYYMMDD(20120228),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd5 == YYYYMMDD("20120228"),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd5 == YYYYMMDD("2012-02-28"),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd5 == YMD(2012,2,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd5.Value() == 20120228,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd5.IsValid(),__FILE__,__FUNCTION__,__LINE__);

    ymd3.Set("19871027");
    ut.Assert(ymd3 == YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 != YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 > YYYYMMDD(19871026),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 >= YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 >= YYYYMMDD(19871026),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 < YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 <= YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 <= YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 == YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 != YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 != YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 > YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 >= YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 >= YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 < YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 <= YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 <= YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);

    ut.AssertF(ymd3 == YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 != YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 > YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 > YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 >= YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 < YYYYMMDD(19871026),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 < YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 <= YYYYMMDD(19871026),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 == YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 == YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 != YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 > YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 > YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 >= YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 < YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 < YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3 <= YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);

    ut.Assert(ymd3.Value() == 19871027,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3.IsValid(),__FILE__,__FUNCTION__,__LINE__);


    ymd3.Invalidate();
    ut.Assert(ymd3 == YYYYMMDD(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 == YMD(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 != YYYYMMDD(20131130),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd3 != YMD(20131130),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd3.IsValid(),__FILE__,__FUNCTION__,__LINE__);


    ut.Assert(YYYYMMDD(20140204).ToStrYM() == "201402",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(YYYYMMDD(20140204).ToStr() == "20140204",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(YYYYMMDD(20140204).ToStr_() == "2014-02-04",__FILE__,__FUNCTION__,__LINE__);
    YMD ymd6(YYYYMMDD(20140204));
    ut.Assert(ymd6 == YMD(2014,2,4),__FILE__,__FUNCTION__,__LINE__);




    YMD ymd7;
    ut.Assert(ymd7 == YMD(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 == YYYYMMDD(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 != YMD(2010,2,22),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 != YYYYMMDD(20100222),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7.IsValid(),__FILE__,__FUNCTION__,__LINE__);

    ymd7.Set(2013,1,28);
    ut.Assert(ymd7 == YMD(2013,1,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 == YYYYMMDD(20130128),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.Year() == 2013,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.Month() == 1,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.Day() == 28,__FILE__,__FUNCTION__,__LINE__);

    ymd7.SetYear(1982);
    ut.Assert(ymd7 == YMD(1982,1,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 == YYYYMMDD(19820128),__FILE__,__FUNCTION__,__LINE__);

    ymd7.SetMonth(12);
    ut.Assert(ymd7 == YMD(1982,12,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 == YYYYMMDD(19821228),__FILE__,__FUNCTION__,__LINE__);

    ymd7.SetDay(23);
    ut.Assert(ymd7 == YMD(1982,12,23),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 == YYYYMMDD(19821223),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.Year() == 1982,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.Month() == 12,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.Day() == 23,__FILE__,__FUNCTION__,__LINE__);

    ymd7.Invalidate();
    ut.Assert(ymd7 == YMD(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 == YYYYMMDD(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 != YMD(2010,2,22),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 != YYYYMMDD(20100222),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7.IsValid(),__FILE__,__FUNCTION__,__LINE__);


    ymd7.Set(1987,10,27);
    ut.Assert(ymd7 == YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 != YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 > YYYYMMDD(19871026),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 >= YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 >= YYYYMMDD(19871026),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 < YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 <= YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 <= YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 == YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 != YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 != YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 > YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 >= YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 >= YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 < YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 <= YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7 <= YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);

    ut.AssertF(ymd7 == YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 != YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 > YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 > YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 >= YYYYMMDD(19871028),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 < YYYYMMDD(19871026),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 < YYYYMMDD(19871027),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 <= YYYYMMDD(19871026),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 == YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 == YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 != YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 > YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 > YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 >= YMD(1987,10,28),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 < YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 < YMD(1987,10,27),__FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymd7 <= YMD(1987,10,26),__FILE__,__FUNCTION__,__LINE__);

    ut.Assert(ymd7.IsValid(),__FILE__,__FUNCTION__,__LINE__);


    ymd7.Set(1987,10,27);
    ymd7.AddYear(1);
    ut.Assert(ymd7 == YYYYMMDD(19881027),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddYear(-2);
    ut.Assert(ymd7 == YYYYMMDD(19861027),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddYear(50);
    ut.Assert(ymd7 == YYYYMMDD(20361027),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddYear(-50);
    ut.Assert(ymd7 == YYYYMMDD(19861027),__FILE__,__FUNCTION__,__LINE__);

    ymd7.Set(1987,10,27);
    ymd7.SubtractYear(-1);
    ut.Assert(ymd7 == YYYYMMDD(19881027),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractYear(2);
    ut.Assert(ymd7 == YYYYMMDD(19861027),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractYear(-50);
    ut.Assert(ymd7 == YYYYMMDD(20361027),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractYear(50);
    ut.Assert(ymd7 == YYYYMMDD(19861027),__FILE__,__FUNCTION__,__LINE__);

    ymd7.Set(1986,10,27);
    ymd7.AddMonth(1);
    ut.Assert(ymd7 == YYYYMMDD(19861127),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddMonth(-2);
    ut.Assert(ymd7 == YYYYMMDD(19860927),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddMonth(50);
    ut.Assert(ymd7 == YYYYMMDD(19901127),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddMonth(-50);
    ut.Assert(ymd7 == YYYYMMDD(19860927),__FILE__,__FUNCTION__,__LINE__);

    ymd7.Set(1986,10,27);
    ymd7.SubtractMonth(-1);
    ut.Assert(ymd7 == YYYYMMDD(19861127),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractMonth(2);
    ut.Assert(ymd7 == YYYYMMDD(19860927),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractMonth(-50);
    ut.Assert(ymd7 == YYYYMMDD(19901127),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractMonth(50);
    ut.Assert(ymd7 == YYYYMMDD(19860927),__FILE__,__FUNCTION__,__LINE__);


    ymd7.Set(2012,2,29);
    ymd7.AddDay(1);
    ut.Assert(ymd7 == YYYYMMDD(20120301),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddDay(-2);
    ut.Assert(ymd7 == YYYYMMDD(20120228),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddDay(50);
    ut.Assert(ymd7 == YYYYMMDD(20120418),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddDay(-350);
    ut.Assert(ymd7 == YYYYMMDD(20110504),__FILE__,__FUNCTION__,__LINE__);
    ymd7.AddDay(66);
    ut.Assert(ymd7 == YYYYMMDD(20110709),__FILE__,__FUNCTION__,__LINE__);

    ymd7.Set(2012,2,29);
    ymd7.SubtractDay(-1);
    ut.Assert(ymd7 == YYYYMMDD(20120301),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractDay(2);
    ut.Assert(ymd7 == YYYYMMDD(20120228),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractDay(-50);
    ut.Assert(ymd7 == YYYYMMDD(20120418),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractDay(350);
    ut.Assert(ymd7 == YYYYMMDD(20110504),__FILE__,__FUNCTION__,__LINE__);
    ymd7.SubtractDay(-66);
    ut.Assert(ymd7 == YYYYMMDD(20110709),__FILE__,__FUNCTION__,__LINE__);


    ymd7.Set(2012,2,29);
    YYYYMMDD ymd8;
    ymd7.ToYYYYMMDD(ymd8);
    ut.Assert(ymd7 == ymd8,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.ToStrYM() == "201202",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.ToStrYM_() == "2012-02",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.ToStr() == "20120229",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd7.ToStr_() == "2012-02-29",__FILE__,__FUNCTION__,__LINE__);

    ymd7.FromYYYYMMDD(YYYYMMDD(20110320));
    ut.Assert(ymd7 == YYYYMMDD(20110320),__FILE__,__FUNCTION__,__LINE__);

    //---------------------------------------------
    {
      YYYYMMDD ymd9(20150823);
      YYYYMMDD ymd10(20150723);
      ut.Assert(ymd9 - ymd10 == 31, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(ymd10 - ymd9 == 31, __FILE__,__FUNCTION__,__LINE__);
    }



    //---------------------------------------------
    HHMMSS hms1;
    ut.AssertF(hms1.IsValid(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms1 == HHMMSS(),__FILE__,__FUNCTION__,__LINE__);

    hms1.Set(5,45,13);
    HHMMSS hms2(hms1);
    ut.Assert(hms2 == HHMMSS(54513),__FILE__,__FUNCTION__,__LINE__);

    HHMMSS hms3("23:03:59");
    ut.Assert(hms3 == HHMMSS(230359),__FILE__,__FUNCTION__,__LINE__);

    HHMMSS hms4(53309);
    ut.Assert(hms4 == HHMMSS(53309),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms4.Value() == 53309,__FILE__,__FUNCTION__,__LINE__);

    hms4.Set("16:00:12");
    ut.Assert(hms4 == HHMMSS(160012),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms4.Value() == 160012,__FILE__,__FUNCTION__,__LINE__);

    hms4.Set(190112);
    ut.Assert(hms4 == HHMMSS(190112),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms4.Value() == 190112,__FILE__,__FUNCTION__,__LINE__);

    hms4.Set(2,22);
    ut.Assert(hms4 == HHMMSS(22200),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms4.Value() == 22200,__FILE__,__FUNCTION__,__LINE__);

    hms4.Invalidate();
    ut.AssertF(hms4.IsValid(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms4 == HHMMSS(),__FILE__,__FUNCTION__,__LINE__);

    HHMMSS hms5(151500);
    ut.Assert(hms5 == HHMMSS(151500),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 != HHMMSS(151501),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 >  HHMMSS(151459),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 >= HHMMSS(151459),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 >= HHMMSS(151500),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 <  HHMMSS(151501),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 <= HHMMSS(151501),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 <= HHMMSS(151500),__FILE__,__FUNCTION__,__LINE__);

    ut.Assert(hms5 == HMS(15,15, 0),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 != HMS(15,15, 1),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 >  HMS(15,14,59),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 >= HMS(15,14,59),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 >= HMS(15,15, 0),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 <  HMS(15,15, 1),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 <= HMS(15,15, 1),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5 <= HMS(15,15, 0),__FILE__,__FUNCTION__,__LINE__);


    hms5.Set(1,1,0);
    ut.Assert(hms5.ToStrHM() == "0101",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5.ToStr() == "010100",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5.ToStrHM_() == "01:01",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5.ToStr_() == "01:01:00",__FILE__,__FUNCTION__,__LINE__);

    hms5.Set(19,11,59);
    ut.Assert(hms5.ToStrHM() == "1911",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5.ToStr() == "191159",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5.ToStrHM_() == "19:11",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms5.ToStr_() == "19:11:59",__FILE__,__FUNCTION__,__LINE__);

    //--------------------------------------------------

    //---------------------------------------------
    HHMM hm1;
    ut.AssertF(hm1.IsValid(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm1 == HHMM(),__FILE__,__FUNCTION__,__LINE__);

    hm1.Set(5,45);
    HHMM hm2(hm1);
    ut.Assert(hm2 == HHMM(545),__FILE__,__FUNCTION__,__LINE__);

    HHMM hm3("23:03");
    ut.Assert(hm3 == HHMM(2303),__FILE__,__FUNCTION__,__LINE__);

    hm3.Set(hms3);
    ut.Assert(hm3 == HHMM(2303),__FILE__,__FUNCTION__,__LINE__);

    HHMM hm4(533);
    ut.Assert(hm4 == HHMM(533),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm4.Value() == 533,__FILE__,__FUNCTION__,__LINE__);

    hm4.Set("16:00");
    ut.Assert(hm4 == HHMM(1600),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm4.Value() == 1600,__FILE__,__FUNCTION__,__LINE__);

    hm4.Set(1901);
    ut.Assert(hm4 == HHMM(1901),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm4.Value() == 1901,__FILE__,__FUNCTION__,__LINE__);

    hm4.Set(2,22);
    ut.Assert(hm4 == HHMM(222),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm4.Value() == 222,__FILE__,__FUNCTION__,__LINE__);

    hm4.Invalidate();
    ut.AssertF(hm4.IsValid(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm4 == HHMM(),__FILE__,__FUNCTION__,__LINE__);

    HHMM hm5(1515);
    ut.Assert(hm5 == HHMM(1515),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm5 != HHMM(1516),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm5 >  HHMM(1514),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm5 >= HHMM(1514),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm5 >= HHMM(1515),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm5 <  HHMM(1517),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm5 <= HHMM(1515),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm5 <= HHMM(1515),__FILE__,__FUNCTION__,__LINE__);

    hm5.Set(1,1);
    ut.Assert(hm5.ToStr() == "0101",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm5.ToStr_() == "01:01",__FILE__,__FUNCTION__,__LINE__);

    hm5.Set(19,11);
    ut.Assert(hm5.ToStr() == "1911",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hm5.ToStr_() == "19:11",__FILE__,__FUNCTION__,__LINE__);

    //--------------------------------------------------


    HMS hms6(hms5);
    ut.Assert(hms6 == HMS(19,11,59),__FILE__,__FUNCTION__,__LINE__);


    HMS hms7;
    ut.AssertF(hms7.IsValid(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms7 == HMS(),__FILE__,__FUNCTION__,__LINE__);

    hms7.Set(4,6,23);
    HMS hms8(hms7);
    ut.Assert(hms8 == HMS(4,6,23),__FILE__,__FUNCTION__,__LINE__);

    hms7.SetMinute(12);
    ut.Assert(hms7 == HMS(4,12,23),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms7.Minute() == 12,__FILE__,__FUNCTION__,__LINE__);

    hms7.SetHour(13);
    ut.Assert(hms7 == HMS(13,12,23),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms7.Hour() == 13,__FILE__,__FUNCTION__,__LINE__);

    hms7.SetSecond(7);
    ut.Assert(hms7 == HMS(13,12,7),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms7.Second() == 7,__FILE__,__FUNCTION__,__LINE__);

    hms7.Invalidate();
    ut.AssertF(hms7.IsValid(),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms7 == HMS(),__FILE__,__FUNCTION__,__LINE__);


    HMS hms9(15,15,0);
    ut.Assert(hms9 == HMS(15,15, 0),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 != HMS(15,15, 1),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 >  HMS(15,14,59),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 >= HMS(15,14,59),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 >= HMS(15,15, 0),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 <  HMS(15,15, 1),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 <= HMS(15,15, 1),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 <= HMS(15,15, 0),__FILE__,__FUNCTION__,__LINE__);

    ut.Assert(hms9 == HHMMSS(151500),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 != HHMMSS(151501),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 >  HHMMSS(151459),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 >= HHMMSS(151459),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 >= HHMMSS(151500),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 <  HHMMSS(151501),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 <= HHMMSS(151501),__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9 <= HHMMSS(151500),__FILE__,__FUNCTION__,__LINE__);


    hms9.Set(3,9,1);
    HHMMSS hms10;
    hms9.ToHHMMSS(hms10);
    ut.Assert(hms10 == HHMMSS(30901),__FILE__,__FUNCTION__,__LINE__);


    ut.Assert(hms9.ToStrHM()    == "0309"    ,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9.ToStr()  == "030901"  ,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9.ToStrHM_()   == "03:09"   ,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9.ToStr_() == "03:09:01",__FILE__,__FUNCTION__,__LINE__);

    hms9.Set(19,59,31);
    ut.Assert(hms9.ToStrHM()    == "1959"    ,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9.ToStr()  == "195931"  ,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9.ToStrHM_()   == "19:59"   ,__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms9.ToStr_() == "19:59:31",__FILE__,__FUNCTION__,__LINE__);

    HMS hms;
    hms.Set( 0, 0, 0); ut.Assert(hms.AddMinute     (     1) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 1, 0),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0, 0, 0); ut.Assert(hms.AddMinute     (  1440) ==  1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 0, 0),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0,10, 3); ut.Assert(hms.AddMinute     (     1) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0,11, 3),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 9,10, 3); ut.Assert(hms.AddMinute     (     2) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 9,12, 3),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(12,59,59); ut.Assert(hms.AddMinute     (    10) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(13, 9,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(19,59,59); ut.Assert(hms.AddMinute     (   240) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(23,59,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(23,59,59); ut.Assert(hms.AddMinute     (    18) ==  1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0,17,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(23,59,59); ut.Assert(hms.AddMinute     (     1) ==  1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 0,59),__FILE__,__FUNCTION__,__LINE__);

    hms.Set( 0, 0, 0); ut.Assert(hms.SubtractMinute(     1) == -1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(23,59, 0),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0, 0, 0); ut.Assert(hms.SubtractMinute(  1440) == -1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 0, 0),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0,10, 3); ut.Assert(hms.SubtractMinute(     1) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 9, 3),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 9,10, 3); ut.Assert(hms.SubtractMinute(     2) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 9, 8, 3),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(12, 0,59); ut.Assert(hms.SubtractMinute(    10) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(11,50,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(19,59,59); ut.Assert(hms.SubtractMinute(   240) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(15,59,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0, 9,59); ut.Assert(hms.SubtractMinute(    18) == -1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(23,51,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(23,59,59); ut.Assert(hms.SubtractMinute(     1) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(23,58,59),__FILE__,__FUNCTION__,__LINE__);

    hms.Set( 0, 0, 0); ut.Assert(hms.AddSecond     (     1) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 0, 1),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0, 0, 0); ut.Assert(hms.AddSecond     ( 86400) ==  1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 0, 0),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0, 0, 0); ut.Assert(hms.AddSecond     ( 86401) ==  1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 0, 1),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0,10, 3); ut.Assert(hms.AddSecond     (     1) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0,10, 4),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 9,10, 3); ut.Assert(hms.AddSecond     (     2) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 9,10, 5),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(12,59,59); ut.Assert(hms.AddSecond     (    10) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(13, 0, 9),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(19,59,59); ut.Assert(hms.AddSecond     ( 14400) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(23,59,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(23,59,59); ut.Assert(hms.AddSecond     (    18) ==  1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 0,17),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(23,59,59); ut.Assert(hms.AddSecond     (     1) ==  1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 0, 0),__FILE__,__FUNCTION__,__LINE__);

    hms.Set( 0, 0, 0); ut.Assert(hms.SubtractSecond(     1) == -1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(23,59,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0, 0, 0); ut.Assert(hms.SubtractSecond( 86400) == -1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0, 0, 0),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0,10, 3); ut.Assert(hms.SubtractSecond(     1) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 0,10, 2),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 9,10, 3); ut.Assert(hms.SubtractSecond(     2) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS( 9,10, 1),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(12, 0, 9); ut.Assert(hms.SubtractSecond(    10) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(11,59,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(19,59,59); ut.Assert(hms.SubtractSecond( 14400) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(15,59,59),__FILE__,__FUNCTION__,__LINE__);
    hms.Set( 0, 0, 9); ut.Assert(hms.SubtractSecond(    18) == -1,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(23,59,51),__FILE__,__FUNCTION__,__LINE__);
    hms.Set(23,59,59); ut.Assert(hms.SubtractSecond(     1) ==  0,__FILE__,__FUNCTION__,__LINE__); ut.Assert(hms == HMS(23,59,58),__FILE__,__FUNCTION__,__LINE__);


    //---------------------------------------------

    YMDHMS yh(2014,6,8,13,30,15);
    ut.Assert(yh.GetYMD() == YMD(2014,6,8), __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(yh.GetHMS() == HMS(13,30,15), __FILE__,__FUNCTION__,__LINE__);
    YMDHMS yh2(2014,6,8,13,30,16);
    YMDHMS yh3(2015,6,8,13,30,15);
    ut.AssertF(yh == yh2, __FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(yh == yh3, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(yh == yh, __FILE__,__FUNCTION__,__LINE__);



    //---------------------------------------------
    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddMinute(     1); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0, 1, 0),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddMinute(  1440); ut.Assert(yh.GetYMD() == YMD(2014,6,9),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0, 0, 0),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS( 0,10, 3); yh.AddMinute(     1); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0,11, 3),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS( 9,10, 3); yh.AddMinute(     2); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 9,12, 3),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS(12,59,59); yh.AddMinute(    10); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS(13, 9,59),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS(19,59,59); yh.AddMinute(   240); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS(23,59,59),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS(23,59,59); yh.AddMinute(    18); ut.Assert(yh.GetYMD() == YMD(2014,6,9),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0,17,59),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS(23,59,59); yh.AddMinute(     1); ut.Assert(yh.GetYMD() == YMD(2014,6,9),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0, 0,59),__FILE__,__FUNCTION__,__LINE__);

    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddSecond(     1); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0, 0, 1),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddSecond( 86400); ut.Assert(yh.GetYMD() == YMD(2014,6,9),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0, 0, 0),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddSecond( 86401); ut.Assert(yh.GetYMD() == YMD(2014,6,9),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0, 0, 1),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS( 0,10, 3); yh.AddSecond(     1); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0,10, 4),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS( 9,10, 3); yh.AddSecond(     2); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 9,10, 5),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS(12,59,59); yh.AddSecond(    10); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS(13, 0, 9),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS(19,59,59); yh.AddSecond( 14400); ut.Assert(yh.GetYMD() == YMD(2014,6,8),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS(23,59,59),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS(23,59,59); yh.AddSecond(    18); ut.Assert(yh.GetYMD() == YMD(2014,6,9),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0, 0,17),__FILE__,__FUNCTION__,__LINE__);
    yh.SetYMD(2014,6,8); yh.SetHMS(23,59,59); yh.AddSecond(     1); ut.Assert(yh.GetYMD() == YMD(2014,6,9),__FILE__,__FUNCTION__,__LINE__); ut.Assert(yh.GetHMS() == HMS( 0, 0, 0),__FILE__,__FUNCTION__,__LINE__);


  }




  {
    YYYYMMDD ymd1;
    ymd1.Set(2014,6,29);
    ut.Assert(ymd1.Year() == 2014, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd1.Month() == 6, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymd1.Day() == 29, __FILE__,__FUNCTION__,__LINE__);
  }


  {
    cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << endl << flush;
    YYYYMMDDHHMMSS ymdhms1(20150118,114200);
    const YYYYMMDD & ymd = ymdhms1.GetYYYYMMDD();
    const HHMMSS & hms = ymdhms1.GetHHMMSS();
    ut.Assert(ymd.ToInt() == 20150118, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms.ToInt() == 114200, __FILE__,__FUNCTION__,__LINE__);

    ut.AssertF(ymdhms1 < ymdhms1, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymdhms1 <= ymdhms1, __FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymdhms1 >  ymdhms1, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymdhms1 >= ymdhms1, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymdhms1 == ymdhms1, __FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymdhms1 != ymdhms1, __FILE__,__FUNCTION__,__LINE__);

    YYYYMMDDHHMMSS ymdhms2(20150118,114201);
    ut.Assert(ymdhms1 < ymdhms2, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymdhms1 <= ymdhms2, __FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymdhms1 >  ymdhms2, __FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymdhms1 >= ymdhms2, __FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymdhms1 == ymdhms2, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymdhms1 != ymdhms2, __FILE__,__FUNCTION__,__LINE__);

    YYYYMMDDHHMMSS ymdhms3(20150119,114200);
    ut.Assert(ymdhms1 < ymdhms3, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymdhms1 <= ymdhms3, __FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymdhms1 >  ymdhms3, __FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymdhms1 >= ymdhms3, __FILE__,__FUNCTION__,__LINE__);
    ut.AssertF(ymdhms1 == ymdhms3, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymdhms1 != ymdhms3, __FILE__,__FUNCTION__,__LINE__);

  }

  {
    YYYYMMDDHHMMSS ymdhms1(20150119,114200);
    YYYYMMDDHHMMSS ymdhms2(20150119,114201);
    ut.Assert(ymdhms2 - ymdhms1 == 1, __FILE__,__FUNCTION__,__LINE__);
    cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << ymdhms2 - ymdhms1 << endl << flush;
  }
  {
    YYYYMMDDHHMMSS ymdhms1(20150119,104200);
    YYYYMMDDHHMMSS ymdhms2(20150120,94200);
    ut.Assert(ymdhms2 - ymdhms1 == 23*60*60, __FILE__,__FUNCTION__,__LINE__);
    cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << ymdhms2 - ymdhms1 << endl << flush;
  }
  {
    YYYYMMDDHHMMSS ymdhms1(20150120,94200);
    YYYYMMDDHHMMSS ymdhms2(20150119,104200);
    ut.Assert(ymdhms2 - ymdhms1 == -23*60*60, __FILE__,__FUNCTION__,__LINE__);
    cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << ymdhms2 - ymdhms1 << endl << flush;
  } 


  {
    HHMMSS hms1(91800);
    HHMMSS hms2(101800);

    ut.Assert(hms2 - hms1 ==  60*60, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms1 - hms2 == -60*60, __FILE__,__FUNCTION__,__LINE__);
  }

  {
    HMS hms1(91800);
    ut.Assert(hms1.Hour()   == 9, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms1.Minute() ==18, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms1.Second() == 0, __FILE__,__FUNCTION__,__LINE__);
    HMS hms2(191850);
    ut.Assert(hms2.Hour()   ==19, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms2.Minute() ==18, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(hms2.Second() ==50, __FILE__,__FUNCTION__,__LINE__);
  }

  {
    YMDHMS ymdhms;
    ymdhms.Set(YYYYMMDD(20150407));
    ymdhms.Set(HHMMSS(140138));
    ut.Assert(ymdhms.GetYMD() == YMD(2015, 4, 7), __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(ymdhms.GetHMS() == HMS(14, 1,38), __FILE__,__FUNCTION__,__LINE__);
  }

  {
    YYYYMMDDHHMMSS ymdhms("2015-10-17","04:30:00");
    YYYYMMDDHHMMSS ymdhmsOut = SDateTime::ChangeTimeZone(ymdhms, SDateTime::HKT, SDateTime::EST);
    ut.Assert(ymdhmsOut == YYYYMMDDHHMMSS("2015-10-16","16:30:00"), __FILE__,__FUNCTION__,__LINE__);
  }
  {
    YYYYMMDDHHMMSS ymdhms("2015-10-16","21:30:00");
    YYYYMMDDHHMMSS ymdhmsOut = SDateTime::ChangeTimeZone(ymdhms, SDateTime::HKT, SDateTime::EST);
    ut.Assert(ymdhmsOut == YYYYMMDDHHMMSS("2015-10-16","09:30:00"), __FILE__,__FUNCTION__,__LINE__);
  }
  {
    YYYYMMDDHHMMSS ymdhms("2015-10-16","11:30:00");
    YYYYMMDDHHMMSS ymdhmsOut = SDateTime::ChangeTimeZone(ymdhms, SDateTime::EST, SDateTime::HKT);
    ut.Assert(ymdhmsOut == YYYYMMDDHHMMSS("2015-10-16","23:30:00"), __FILE__,__FUNCTION__,__LINE__);
  }
  {
    YYYYMMDDHHMMSS ymdhms("2015-10-16","16:30:00");
    YYYYMMDDHHMMSS ymdhmsOut = SDateTime::ChangeTimeZone(ymdhms, SDateTime::EST, SDateTime::HKT);
    ut.Assert(ymdhmsOut == YYYYMMDDHHMMSS("2015-10-17","04:30:00"), __FILE__,__FUNCTION__,__LINE__);
  }

  //	//Get Year / Month / Day
  //	ut.Assert(SDateTime::GetYear(20110531) == 2011,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetYear(20110501) == 2011,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetYear(19701201) == 1970,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetYear(19991231) == 1999,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetYear(201105) == 2011,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetYear(202105) == 2021,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetYear(197012) == 1970,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetYear(199912) == 1999,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetMonth(20110531) == 5,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetMonth(20110501) == 5,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetMonth(20111201) == 12,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetMonth(20111231) == 12,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetMonth(201112) == 12,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetMonth(200101) == 1,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetMonth(190005) == 5,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetDay(20110531) == 31,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetDay(20110501) == 1,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetDay(20111201) == 1,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::GetDay(20111231) == 31,__FILE__,__FUNCTION__,__LINE__);
  //
  //	int iY,iM,iD;
  //	SDateTime::GetYMD(20130103,iY,iM,iD); ut.Assert(iY == 2013 && iM == 1 && iD == 3,__FILE__,__FUNCTION__,__LINE__);
  //	SDateTime::GetYMD(19801231,iY,iM,iD); ut.Assert(iY == 1980 && iM == 12 && iD == 31,__FILE__,__FUNCTION__,__LINE__);
  //
  //
  //
  //	ut.Assert(SDateTime::NextMonthStrict(20130131) == 20130228,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130228) == 20130328,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130328) == 20130428,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130331) == 20130430,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130430) == 20130530,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000130) == 20000229,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000131) == 20000229,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000101) == 20000201,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000229) == 20000329,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000228) == 20000328,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130120,31) == 20130228,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130220,28) == 20130328,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130220,31) == 20130331,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130328,28) == 20130428,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130328,30) == 20130430,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20130328,31) == 20130430,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000120,31) == 20000229,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000220,28) == 20000328,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000220,31) == 20000331,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000328,28) == 20000428,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000328,30) == 20000430,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextMonthStrict(20000328,31) == 20000430,__FILE__,__FUNCTION__,__LINE__);
  //
  //
  //
  //
  //	ut.Assert(SDateTime::YYYY_YY(2010).compare("10") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YYYY_YY(2009).compare("09") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YYYY_YY(1980).compare("80") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YYYY_YY(2000).compare("00") == 0,__FILE__,__FUNCTION__,__LINE__);
  //
  //
  //	ut.Assert(SDateTime::YMDi("2011,3,4") == 20110304,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMDi("2013-01-23") == 20130123,__FILE__,__FUNCTION__,__LINE__);
  //	try { SDateTime::YMDi("2013-01"); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //	ut.Assert(SDateTime::YMDi(2011,3,4) == 20110304,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMDi(2013,01,23) == 20130123,__FILE__,__FUNCTION__,__LINE__);
  //
  //	ut.Assert(SDateTime::YMi("2011,3,4") == 201103,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMi("2013-01-23") == 201301,__FILE__,__FUNCTION__,__LINE__);
  //
  //	ut.Assert(SDateTime::YMi("2010-2") == 201002,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMi("2007-08") == 200708,__FILE__,__FUNCTION__,__LINE__);
  //	try { SDateTime::YMi("2013"); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //	ut.Assert(SDateTime::YMi(2011,3,4) == 201103,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMi(2013,01,23) == 201301,__FILE__,__FUNCTION__,__LINE__);
  //
  //	ut.Assert(SDateTime::YMi(2010,2) == 201002,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMi(2007,8) == 200708,__FILE__,__FUNCTION__,__LINE__);
  //
  //
  //
  //	ut.Assert(SDateTime::YMi(20061005) == 200610,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMi(200611) == 200611,__FILE__,__FUNCTION__,__LINE__);
  //
  //
  //
  //
  //
  //	ut.Assert(SDateTime::HMSi("02:12:58") == 21258,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::HMSi("19:45") == 194500,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::HMSi("03:56") == 35600,__FILE__,__FUNCTION__,__LINE__);
  //
  //	try { SDateTime::HMSi("ew:45:22"); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //	ut.Assert(SDateTime::HMSi(2,12,58) == 21258,__FILE__,__FUNCTION__,__LINE__);
  //
  //
  //
  //	ut.Assert(SDateTime::HMSs(94234).compare("09:42:34") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::HMSs(194234).compare("19:42:34") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::HMSs(190234).compare("19:02:34") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	try { SDateTime::HMSs(1945); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //
  //
  //
  //	ut.Assert(SDateTime::HMi("02:13:58") == 213,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::HMi("13:21:58") == 1321,__FILE__,__FUNCTION__,__LINE__);
  //	try { SDateTime::HMi("19"); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //
  //
  //	ut.Assert(SDateTime::HMi(2,13,58) == 213,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::HMi(13,21,58) == 1321,__FILE__,__FUNCTION__,__LINE__);
  //
  //
  //
  //	ut.Assert(SDateTime::HMs(194234).compare("19:42") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	try { SDateTime::HMs(19); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //
  //
  //
  //	ut.Assert(SDateTime::YMDs(20130123).compare("2013-01-23") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMDs(19991003).compare("1999-10-03") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	try { SDateTime::YMDs(199709); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //	try { SDateTime::YMDs(1996); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //
  //
  //
  //	ut.Assert(SDateTime::YMs(200211).compare("2002-11") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMs(20141123).compare("2014-11") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	try { SDateTime::YMs(1996); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //	ut.Assert(SDateTime::YMDs(2001,04,11).compare("2001-04-11") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMs(2009,04,11).compare("2009-04") == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YMs(2003,03).compare("2003-03") == 0,__FILE__,__FUNCTION__,__LINE__);
  //
  //	//NextYMi
  //	ut.Assert(SDateTime::NextYMi(200303,-4) == 200211,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextYMi(200303,-3) == 200212,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextYMi(200303,-2) == 200301,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextYMi(200303,-1) == 200302,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextYMi(200303,0) == 200303,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextYMi(200303,1) == 200304,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextYMi(200303,5) == 200308,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextYMi(200303,9) == 200312,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextYMi(200303,10) == 200401,__FILE__,__FUNCTION__,__LINE__);
  //
  //
  //	//NextHMi
  //	ut.Assert(SDateTime::NextHMi(0003,-80) == 2243,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(0003,-4) == 2359,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(1615,-3) == 1612,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(2301,-2) == 2259,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(1801,-1) == 1800,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(2359,0) == 2359,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(2359,1) == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(2359,5) == 4,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(2359,9) == 8,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(1345,10) == 1355,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(1345,70) == 1455,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::NextHMi(2345,70) == 55,__FILE__,__FUNCTION__,__LINE__);
  //
  //	ut.Assert(SDateTime::YYYYMM(20230625) == 202306,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YYYYMM(20030625) == 200306,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::YYYYMM(19970625) == 199706,__FILE__,__FUNCTION__,__LINE__);
  //
  //	//Month diff
  //	ut.Assert(SDateTime::MthDiff(201010,201105) == 7,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::MthDiff(201105,201010) == -7,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::MthDiff(201105,201105) == 0,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::MthAbsDiff(201010,201105) == 7,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::MthAbsDiff(201105,201010) == 7,__FILE__,__FUNCTION__,__LINE__);
  //	ut.Assert(SDateTime::MthAbsDiff(201105,201105) == 0,__FILE__,__FUNCTION__,__LINE__);










  ut.PrintResult();

  return 0;
}
