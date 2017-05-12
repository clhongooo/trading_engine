#include "TestSDateTime.h"

TestSDateTime::TestSDateTime()
{
}

TestSDateTime::~TestSDateTime()
{
}

void TestSDateTime::RunTest()
{
  {
    CPPUNIT_ASSERT(STool::Abs(SDateTime::CalcJulianDayNum(2013,1,1)-2456293.520833) < 1);
    // http://mathforum.org/library/drmath/view/62338.html
    CPPUNIT_ASSERT(STool::Abs(SDateTime::CalcJulianDayNum(1582,10,15)-2299161) < 2);

    //CalcGregorianDayNum is tested through CalendarDayDiff, because CalcGregorianDayNum is used in CalendarDayDiff

    YMD ymd1;
    YMD ymd2;
    ymd1.Set(2013,9,12); ymd2.Set(2013,9,13); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == 1);
    ymd1.Set(2013,9, 1); ymd2.Set(2013,9,13); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == 12);
    ymd1.Set(2013,8,31); ymd2.Set(2013,9,13); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == 13);
    ymd1.Set(2013,8,21); ymd2.Set(2013,9,13); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == 23);
    ymd1.Set(2013,8,12); ymd2.Set(2013,9,13); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == 32);
    ymd1.Set(2000,3,20); ymd2.Set(2004,3,21); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == 1462);
    ymd1.Set(1996,3,20); ymd2.Set(2004,3,21); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == 2923);
    ymd1.Set(2013,9,13); ymd2.Set(2013,9,12); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == -1);
    ymd1.Set(2004,3,21); ymd2.Set(2000,3,20); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == -1462);
    ymd1.Set(2004,3,21); ymd2.Set(1996,3,20); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == -2923);
    ymd1.Set(2004,3,21); ymd2.Set(2004,3,20); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == -1);
    ymd1.Set(2004,3,21); ymd2.Set(2004,3,21); CPPUNIT_ASSERT(SDateTime::CalendarDayDiff(ymd1,ymd2) == 0);

    CPPUNIT_ASSERT(SDateTime::IsLeapYear(2000));
    CPPUNIT_ASSERT(!(SDateTime::IsLeapYear(2001)));
    CPPUNIT_ASSERT(!(SDateTime::IsLeapYear(2002)));
    CPPUNIT_ASSERT(!(SDateTime::IsLeapYear(2003)));
    CPPUNIT_ASSERT(SDateTime::IsLeapYear(2004));
    CPPUNIT_ASSERT(SDateTime::IsLeapYear(2012));
    CPPUNIT_ASSERT(!(SDateTime::IsLeapYear(2013)));


    CPPUNIT_ASSERT(SDateTime::IsValidYMD(2004,12,31));
    CPPUNIT_ASSERT(SDateTime::IsValidYMD(2004,1,1));
    CPPUNIT_ASSERT(SDateTime::IsValidYMD(2004,2,29));

    CPPUNIT_ASSERT(SDateTime::IsValidYMD(2003,2,28));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2003,2,29)));
    CPPUNIT_ASSERT(SDateTime::IsValidYMD(2003,2,28));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2003,2,29)));

    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2013,12,32)));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2013,1,32)));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2013,13,1)));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2113,11,1)));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2013,11,0)));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2013,11,-1)));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2013,1,0)));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(2013,1,-1)));
    CPPUNIT_ASSERT(!(SDateTime::IsValidYMD(-2013,1,1)));


    CPPUNIT_ASSERT(SDateTime::DaysInMonth(2004,2) == 29);
    CPPUNIT_ASSERT(SDateTime::DaysInMonth(2012,2) == 29);
    CPPUNIT_ASSERT(SDateTime::DaysInMonth(2002,2) == 28);
    CPPUNIT_ASSERT(SDateTime::DaysInMonth(2013,2) == 28);

    CPPUNIT_ASSERT(SDateTime::DaysInYear(2004) == 366);
    CPPUNIT_ASSERT(SDateTime::DaysInYear(2012) == 366);
    CPPUNIT_ASSERT(SDateTime::DaysInYear(2002) == 365);
    CPPUNIT_ASSERT(SDateTime::DaysInYear(2013) == 365);

    ymd1.Set(2013,12,26); CPPUNIT_ASSERT(SDateTime::DayOfWeek(ymd1)==4);
    ymd1.Set(2013,12,28); CPPUNIT_ASSERT(SDateTime::DayOfWeek(ymd1)==6);
    ymd1.Set(2013,12,29); CPPUNIT_ASSERT(SDateTime::DayOfWeek(ymd1)==0);
    ymd1.Set(2013,12,30); CPPUNIT_ASSERT(SDateTime::DayOfWeek(ymd1)==1);

    ymd1.Set(2013,12,26); CPPUNIT_ASSERT(SDateTime::IsWeekDay(ymd1));
    ymd1.Set(2013,12,28); CPPUNIT_ASSERT(!(SDateTime::IsWeekDay(ymd1)));
    ymd1.Set(2013,12,29); CPPUNIT_ASSERT(!(SDateTime::IsWeekDay(ymd1)));
    ymd1.Set(2013,12,30); CPPUNIT_ASSERT(SDateTime::IsWeekDay(ymd1));


    ymd1.Set(2004, 2,29); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));
    ymd1.Set(2012, 2,29); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));
    ymd1.Set(2002, 2,28); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));
    ymd1.Set(2013, 2,28); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));

    ymd1.Set(2004, 2,28); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));
    ymd1.Set(2012, 2,28); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));
    ymd1.Set(2002, 2,27); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));
    ymd1.Set(2013, 2,27); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));

    ymd1.Set(2004, 1,31); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));
    ymd1.Set(2012, 1,31); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));
    ymd1.Set(2002, 1,31); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));
    ymd1.Set(2013, 1,31); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));

    ymd1.Set(2004,12,31); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));
    ymd1.Set(2012,12,31); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));
    ymd1.Set(2002,12,31); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));
    ymd1.Set(2013,12,31); CPPUNIT_ASSERT(SDateTime::IsEndOfMonth(ymd1));

    ymd1.Set(2004, 3,28); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));
    ymd1.Set(2012, 3,28); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));
    ymd1.Set(2002, 3,27); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));
    ymd1.Set(2013, 3,27); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));

    ymd1.Set(2004, 8,28); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));
    ymd1.Set(2012, 8,28); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));
    ymd1.Set(2002, 8,27); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));
    ymd1.Set(2013, 8,27); CPPUNIT_ASSERT(!(SDateTime::IsEndOfMonth(ymd1)));


    //-----------------------------------------------------

    YYYYMMDD ymd3;
    CPPUNIT_ASSERT(!(ymd3.IsValid()));
    CPPUNIT_ASSERT(ymd3 == YYYYMMDD());
    CPPUNIT_ASSERT(ymd3 == YMD());
    ymd3.Set(2013,12,11);
    YYYYMMDD ymd4(ymd3);
    CPPUNIT_ASSERT(ymd4 == YYYYMMDD(20131211));
    CPPUNIT_ASSERT(ymd4 == YYYYMMDD("20131211"));
    CPPUNIT_ASSERT(ymd4 == YYYYMMDD("2013-12-11"));
    CPPUNIT_ASSERT(ymd4.IsValid());

    ymd3.Set(20120228);
    YYYYMMDD ymd5 = ymd3;
    CPPUNIT_ASSERT(ymd5 == YYYYMMDD(20120228));
    CPPUNIT_ASSERT(ymd5 == YYYYMMDD("20120228"));
    CPPUNIT_ASSERT(ymd5 == YYYYMMDD("2012-02-28"));
    CPPUNIT_ASSERT(ymd5 == YMD(2012,2,28));
    CPPUNIT_ASSERT(ymd5.Value() == 20120228);
    CPPUNIT_ASSERT(ymd5.IsValid());

    ymd3.Set("19871027");
    CPPUNIT_ASSERT(ymd3 == YYYYMMDD(19871027));
    CPPUNIT_ASSERT(ymd3 != YYYYMMDD(19871028));
    CPPUNIT_ASSERT(ymd3 > YYYYMMDD(19871026));
    CPPUNIT_ASSERT(ymd3 >= YYYYMMDD(19871027));
    CPPUNIT_ASSERT(ymd3 >= YYYYMMDD(19871026));
    CPPUNIT_ASSERT(ymd3 < YYYYMMDD(19871028));
    CPPUNIT_ASSERT(ymd3 <= YYYYMMDD(19871028));
    CPPUNIT_ASSERT(ymd3 <= YYYYMMDD(19871027));
    CPPUNIT_ASSERT(ymd3 == YMD(1987,10,27));
    CPPUNIT_ASSERT(ymd3 != YMD(1987,10,26));
    CPPUNIT_ASSERT(ymd3 != YMD(1987,10,28));
    CPPUNIT_ASSERT(ymd3 > YMD(1987,10,26));
    CPPUNIT_ASSERT(ymd3 >= YMD(1987,10,26));
    CPPUNIT_ASSERT(ymd3 >= YMD(1987,10,27));
    CPPUNIT_ASSERT(ymd3 < YMD(1987,10,28));
    CPPUNIT_ASSERT(ymd3 <= YMD(1987,10,28));
    CPPUNIT_ASSERT(ymd3 <= YMD(1987,10,27));

    CPPUNIT_ASSERT(!(ymd3 == YYYYMMDD(19871028)));
    CPPUNIT_ASSERT(!(ymd3 != YYYYMMDD(19871027)));
    CPPUNIT_ASSERT(!(ymd3 > YYYYMMDD(19871028)));
    CPPUNIT_ASSERT(!(ymd3 > YYYYMMDD(19871027)));
    CPPUNIT_ASSERT(!(ymd3 >= YYYYMMDD(19871028)));
    CPPUNIT_ASSERT(!(ymd3 < YYYYMMDD(19871026)));
    CPPUNIT_ASSERT(!(ymd3 < YYYYMMDD(19871027)));
    CPPUNIT_ASSERT(!(ymd3 <= YYYYMMDD(19871026)));
    CPPUNIT_ASSERT(!(ymd3 == YMD(1987,10,28)));
    CPPUNIT_ASSERT(!(ymd3 == YMD(1987,10,26)));
    CPPUNIT_ASSERT(!(ymd3 != YMD(1987,10,27)));
    CPPUNIT_ASSERT(!(ymd3 > YMD(1987,10,28)));
    CPPUNIT_ASSERT(!(ymd3 > YMD(1987,10,27)));
    CPPUNIT_ASSERT(!(ymd3 >= YMD(1987,10,28)));
    CPPUNIT_ASSERT(!(ymd3 < YMD(1987,10,26)));
    CPPUNIT_ASSERT(!(ymd3 < YMD(1987,10,27)));
    CPPUNIT_ASSERT(!(ymd3 <= YMD(1987,10,26)));

    CPPUNIT_ASSERT(ymd3.Value() == 19871027);
    CPPUNIT_ASSERT(ymd3.IsValid());


    ymd3.Invalidate();
    CPPUNIT_ASSERT(ymd3 == YYYYMMDD());
    CPPUNIT_ASSERT(ymd3 == YMD());
    CPPUNIT_ASSERT(ymd3 != YYYYMMDD(20131130));
    CPPUNIT_ASSERT(ymd3 != YMD(20131130));
    CPPUNIT_ASSERT(!(ymd3.IsValid()));


    CPPUNIT_ASSERT(YYYYMMDD(20140204).ToStrYM() == "201402");
    CPPUNIT_ASSERT(YYYYMMDD(20140204).ToStr() == "20140204");
    CPPUNIT_ASSERT(YYYYMMDD(20140204).ToStr_() == "2014-02-04");
    YMD ymd6(YYYYMMDD(20140204));
    CPPUNIT_ASSERT(ymd6 == YMD(2014,2,4));




    YMD ymd7;
    CPPUNIT_ASSERT(ymd7 == YMD());
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD());
    CPPUNIT_ASSERT(ymd7 != YMD(2010,2,22));
    CPPUNIT_ASSERT(ymd7 != YYYYMMDD(20100222));
    CPPUNIT_ASSERT(!(ymd7.IsValid()));

    ymd7.Set(2013,1,28);
    CPPUNIT_ASSERT(ymd7 == YMD(2013,1,28));
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20130128));
    CPPUNIT_ASSERT(ymd7.Year() == 2013);
    CPPUNIT_ASSERT(ymd7.Month() == 1);
    CPPUNIT_ASSERT(ymd7.Day() == 28);

    ymd7.SetYear(1982);
    CPPUNIT_ASSERT(ymd7 == YMD(1982,1,28));
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19820128));

    ymd7.SetMonth(12);
    CPPUNIT_ASSERT(ymd7 == YMD(1982,12,28));
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19821228));

    ymd7.SetDay(23);
    CPPUNIT_ASSERT(ymd7 == YMD(1982,12,23));
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19821223));
    CPPUNIT_ASSERT(ymd7.Year() == 1982);
    CPPUNIT_ASSERT(ymd7.Month() == 12);
    CPPUNIT_ASSERT(ymd7.Day() == 23);

    ymd7.Invalidate();
    CPPUNIT_ASSERT(ymd7 == YMD());
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD());
    CPPUNIT_ASSERT(ymd7 != YMD(2010,2,22));
    CPPUNIT_ASSERT(ymd7 != YYYYMMDD(20100222));
    CPPUNIT_ASSERT(!(ymd7.IsValid()));


    ymd7.Set(1987,10,27);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19871027));
    CPPUNIT_ASSERT(ymd7 != YYYYMMDD(19871028));
    CPPUNIT_ASSERT(ymd7 > YYYYMMDD(19871026));
    CPPUNIT_ASSERT(ymd7 >= YYYYMMDD(19871027));
    CPPUNIT_ASSERT(ymd7 >= YYYYMMDD(19871026));
    CPPUNIT_ASSERT(ymd7 < YYYYMMDD(19871028));
    CPPUNIT_ASSERT(ymd7 <= YYYYMMDD(19871028));
    CPPUNIT_ASSERT(ymd7 <= YYYYMMDD(19871027));
    CPPUNIT_ASSERT(ymd7 == YMD(1987,10,27));
    CPPUNIT_ASSERT(ymd7 != YMD(1987,10,26));
    CPPUNIT_ASSERT(ymd7 != YMD(1987,10,28));
    CPPUNIT_ASSERT(ymd7 > YMD(1987,10,26));
    CPPUNIT_ASSERT(ymd7 >= YMD(1987,10,26));
    CPPUNIT_ASSERT(ymd7 >= YMD(1987,10,27));
    CPPUNIT_ASSERT(ymd7 < YMD(1987,10,28));
    CPPUNIT_ASSERT(ymd7 <= YMD(1987,10,28));
    CPPUNIT_ASSERT(ymd7 <= YMD(1987,10,27));

    CPPUNIT_ASSERT(!(ymd7 == YYYYMMDD(19871028)));
    CPPUNIT_ASSERT(!(ymd7 != YYYYMMDD(19871027)));
    CPPUNIT_ASSERT(!(ymd7 > YYYYMMDD(19871028)));
    CPPUNIT_ASSERT(!(ymd7 > YYYYMMDD(19871027)));
    CPPUNIT_ASSERT(!(ymd7 >= YYYYMMDD(19871028)));
    CPPUNIT_ASSERT(!(ymd7 < YYYYMMDD(19871026)));
    CPPUNIT_ASSERT(!(ymd7 < YYYYMMDD(19871027)));
    CPPUNIT_ASSERT(!(ymd7 <= YYYYMMDD(19871026)));
    CPPUNIT_ASSERT(!(ymd7 == YMD(1987,10,28)));
    CPPUNIT_ASSERT(!(ymd7 == YMD(1987,10,26)));
    CPPUNIT_ASSERT(!(ymd7 != YMD(1987,10,27)));
    CPPUNIT_ASSERT(!(ymd7 > YMD(1987,10,28)));
    CPPUNIT_ASSERT(!(ymd7 > YMD(1987,10,27)));
    CPPUNIT_ASSERT(!(ymd7 >= YMD(1987,10,28)));
    CPPUNIT_ASSERT(!(ymd7 < YMD(1987,10,26)));
    CPPUNIT_ASSERT(!(ymd7 < YMD(1987,10,27)));
    CPPUNIT_ASSERT(!(ymd7 <= YMD(1987,10,26)));

    CPPUNIT_ASSERT(ymd7.IsValid());


    ymd7.Set(1987,10,27);
    ymd7.AddYear(1);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19881027));
    ymd7.AddYear(-2);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19861027));
    ymd7.AddYear(50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20361027));
    ymd7.AddYear(-50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19861027));

    ymd7.Set(1987,10,27);
    ymd7.SubtractYear(-1);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19881027));
    ymd7.SubtractYear(2);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19861027));
    ymd7.SubtractYear(-50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20361027));
    ymd7.SubtractYear(50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19861027));

    ymd7.Set(1986,10,27);
    ymd7.AddMonth(1);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19861127));
    ymd7.AddMonth(-2);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19860927));
    ymd7.AddMonth(50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19901127));
    ymd7.AddMonth(-50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19860927));

    ymd7.Set(1986,10,27);
    ymd7.SubtractMonth(-1);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19861127));
    ymd7.SubtractMonth(2);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19860927));
    ymd7.SubtractMonth(-50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19901127));
    ymd7.SubtractMonth(50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(19860927));


    ymd7.Set(2012,2,29);
    ymd7.AddDay(1);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20120301));
    ymd7.AddDay(-2);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20120228));
    ymd7.AddDay(50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20120418));
    ymd7.AddDay(-350);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20110504));
    ymd7.AddDay(66);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20110709));

    ymd7.Set(2012,2,29);
    ymd7.SubtractDay(-1);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20120301));
    ymd7.SubtractDay(2);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20120228));
    ymd7.SubtractDay(-50);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20120418));
    ymd7.SubtractDay(350);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20110504));
    ymd7.SubtractDay(-66);
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20110709));


    ymd7.Set(2012,2,29);
    YYYYMMDD ymd8;
    ymd7.ToYYYYMMDD(ymd8);
    CPPUNIT_ASSERT(ymd7 == ymd8);
    CPPUNIT_ASSERT(ymd7.ToStrYM() == "201202");
    CPPUNIT_ASSERT(ymd7.ToStrYM_() == "2012-02");
    CPPUNIT_ASSERT(ymd7.ToStr() == "20120229");
    CPPUNIT_ASSERT(ymd7.ToStr_() == "2012-02-29");

    ymd7.FromYYYYMMDD(YYYYMMDD(20110320));
    CPPUNIT_ASSERT(ymd7 == YYYYMMDD(20110320));

    //---------------------------------------------
    {
      YYYYMMDD ymd9(20150823);
      YYYYMMDD ymd10(20150723);
      CPPUNIT_ASSERT(ymd9 - ymd10 == 31);
      CPPUNIT_ASSERT(ymd10 - ymd9 == 31);
    }



    //---------------------------------------------
    HHMMSS hms1;
    CPPUNIT_ASSERT(!(hms1.IsValid()));
    CPPUNIT_ASSERT(hms1 == HHMMSS());

    hms1.Set(5,45,13);
    HHMMSS hms2(hms1);
    CPPUNIT_ASSERT(hms2 == HHMMSS(54513));

    HHMMSS hms3("23:03:59");
    CPPUNIT_ASSERT(hms3 == HHMMSS(230359));

    HHMMSS hms4(53309);
    CPPUNIT_ASSERT(hms4 == HHMMSS(53309));
    CPPUNIT_ASSERT(hms4.Value() == 53309);

    hms4.Set("16:00:12");
    CPPUNIT_ASSERT(hms4 == HHMMSS(160012));
    CPPUNIT_ASSERT(hms4.Value() == 160012);

    hms4.Set(190112);
    CPPUNIT_ASSERT(hms4 == HHMMSS(190112));
    CPPUNIT_ASSERT(hms4.Value() == 190112);

    hms4.Set(2,22);
    CPPUNIT_ASSERT(hms4 == HHMMSS(22200));
    CPPUNIT_ASSERT(hms4.Value() == 22200);

    hms4.Invalidate();
    CPPUNIT_ASSERT(!(hms4.IsValid()));
    CPPUNIT_ASSERT(hms4 == HHMMSS());

    HHMMSS hms5(151500);
    CPPUNIT_ASSERT(hms5 == HHMMSS(151500));
    CPPUNIT_ASSERT(hms5 != HHMMSS(151501));
    CPPUNIT_ASSERT(hms5 >  HHMMSS(151459));
    CPPUNIT_ASSERT(hms5 >= HHMMSS(151459));
    CPPUNIT_ASSERT(hms5 >= HHMMSS(151500));
    CPPUNIT_ASSERT(hms5 <  HHMMSS(151501));
    CPPUNIT_ASSERT(hms5 <= HHMMSS(151501));
    CPPUNIT_ASSERT(hms5 <= HHMMSS(151500));

    CPPUNIT_ASSERT(hms5 == HMS(15,15, 0));
    CPPUNIT_ASSERT(hms5 != HMS(15,15, 1));
    CPPUNIT_ASSERT(hms5 >  HMS(15,14,59));
    CPPUNIT_ASSERT(hms5 >= HMS(15,14,59));
    CPPUNIT_ASSERT(hms5 >= HMS(15,15, 0));
    CPPUNIT_ASSERT(hms5 <  HMS(15,15, 1));
    CPPUNIT_ASSERT(hms5 <= HMS(15,15, 1));
    CPPUNIT_ASSERT(hms5 <= HMS(15,15, 0));


    hms5.Set(1,1,0);
    CPPUNIT_ASSERT(hms5.ToStrHM() == "0101");
    CPPUNIT_ASSERT(hms5.ToStr() == "010100");
    CPPUNIT_ASSERT(hms5.ToStrHM_() == "01:01");
    CPPUNIT_ASSERT(hms5.ToStr_() == "01:01:00");

    hms5.Set(19,11,59);
    CPPUNIT_ASSERT(hms5.ToStrHM() == "1911");
    CPPUNIT_ASSERT(hms5.ToStr() == "191159");
    CPPUNIT_ASSERT(hms5.ToStrHM_() == "19:11");
    CPPUNIT_ASSERT(hms5.ToStr_() == "19:11:59");

    //--------------------------------------------------

    //---------------------------------------------
    HHMM hm1;
    CPPUNIT_ASSERT(!(hm1.IsValid()));
    CPPUNIT_ASSERT(hm1 == HHMM());

    hm1.Set(5,45);
    HHMM hm2(hm1);
    CPPUNIT_ASSERT(hm2 == HHMM(545));

    HHMM hm3("23:03");
    CPPUNIT_ASSERT(hm3 == HHMM(2303));

    hm3.Set(hms3);
    CPPUNIT_ASSERT(hm3 == HHMM(2303));

    HHMM hm4(533);
    CPPUNIT_ASSERT(hm4 == HHMM(533));
    CPPUNIT_ASSERT(hm4.Value() == 533);

    hm4.Set("16:00");
    CPPUNIT_ASSERT(hm4 == HHMM(1600));
    CPPUNIT_ASSERT(hm4.Value() == 1600);

    hm4.Set(1901);
    CPPUNIT_ASSERT(hm4 == HHMM(1901));
    CPPUNIT_ASSERT(hm4.Value() == 1901);

    hm4.Set(2,22);
    CPPUNIT_ASSERT(hm4 == HHMM(222));
    CPPUNIT_ASSERT(hm4.Value() == 222);

    hm4.Invalidate();
    CPPUNIT_ASSERT(!(hm4.IsValid()));
    CPPUNIT_ASSERT(hm4 == HHMM());

    HHMM hm5(1515);
    CPPUNIT_ASSERT(hm5 == HHMM(1515));
    CPPUNIT_ASSERT(hm5 != HHMM(1516));
    CPPUNIT_ASSERT(hm5 >  HHMM(1514));
    CPPUNIT_ASSERT(hm5 >= HHMM(1514));
    CPPUNIT_ASSERT(hm5 >= HHMM(1515));
    CPPUNIT_ASSERT(hm5 <  HHMM(1517));
    CPPUNIT_ASSERT(hm5 <= HHMM(1515));
    CPPUNIT_ASSERT(hm5 <= HHMM(1515));

    hm5.Set(1,1);
    CPPUNIT_ASSERT(hm5.ToStr() == "0101");
    CPPUNIT_ASSERT(hm5.ToStr_() == "01:01");

    hm5.Set(19,11);
    CPPUNIT_ASSERT(hm5.ToStr() == "1911");
    CPPUNIT_ASSERT(hm5.ToStr_() == "19:11");

    //--------------------------------------------------


    HMS hms6(hms5);
    CPPUNIT_ASSERT(hms6 == HMS(19,11,59));


    HMS hms7;
    CPPUNIT_ASSERT(!(hms7.IsValid()));
    CPPUNIT_ASSERT(hms7 == HMS());

    hms7.Set(4,6,23);
    HMS hms8(hms7);
    CPPUNIT_ASSERT(hms8 == HMS(4,6,23));

    hms7.SetMinute(12);
    CPPUNIT_ASSERT(hms7 == HMS(4,12,23));
    CPPUNIT_ASSERT(hms7.Minute() == 12);

    hms7.SetHour(13);
    CPPUNIT_ASSERT(hms7 == HMS(13,12,23));
    CPPUNIT_ASSERT(hms7.Hour() == 13);

    hms7.SetSecond(7);
    CPPUNIT_ASSERT(hms7 == HMS(13,12,7));
    CPPUNIT_ASSERT(hms7.Second() == 7);

    hms7.Invalidate();
    CPPUNIT_ASSERT(!(hms7.IsValid()));
    CPPUNIT_ASSERT(hms7 == HMS());


    HMS hms9(15,15,0);
    CPPUNIT_ASSERT(hms9 == HMS(15,15, 0));
    CPPUNIT_ASSERT(hms9 != HMS(15,15, 1));
    CPPUNIT_ASSERT(hms9 >  HMS(15,14,59));
    CPPUNIT_ASSERT(hms9 >= HMS(15,14,59));
    CPPUNIT_ASSERT(hms9 >= HMS(15,15, 0));
    CPPUNIT_ASSERT(hms9 <  HMS(15,15, 1));
    CPPUNIT_ASSERT(hms9 <= HMS(15,15, 1));
    CPPUNIT_ASSERT(hms9 <= HMS(15,15, 0));

    CPPUNIT_ASSERT(hms9 == HHMMSS(151500));
    CPPUNIT_ASSERT(hms9 != HHMMSS(151501));
    CPPUNIT_ASSERT(hms9 >  HHMMSS(151459));
    CPPUNIT_ASSERT(hms9 >= HHMMSS(151459));
    CPPUNIT_ASSERT(hms9 >= HHMMSS(151500));
    CPPUNIT_ASSERT(hms9 <  HHMMSS(151501));
    CPPUNIT_ASSERT(hms9 <= HHMMSS(151501));
    CPPUNIT_ASSERT(hms9 <= HHMMSS(151500));


    hms9.Set(3,9,1);
    HHMMSS hms10;
    hms9.ToHHMMSS(hms10);
    CPPUNIT_ASSERT(hms10 == HHMMSS(30901));


    CPPUNIT_ASSERT(hms9.ToStrHM()    == "0309"    );
    CPPUNIT_ASSERT(hms9.ToStr()  == "030901"  );
    CPPUNIT_ASSERT(hms9.ToStrHM_()   == "03:09"   );
    CPPUNIT_ASSERT(hms9.ToStr_() == "03:09:01");

    hms9.Set(19,59,31);
    CPPUNIT_ASSERT(hms9.ToStrHM()    == "1959"    );
    CPPUNIT_ASSERT(hms9.ToStr()  == "195931"  );
    CPPUNIT_ASSERT(hms9.ToStrHM_()   == "19:59"   );
    CPPUNIT_ASSERT(hms9.ToStr_() == "19:59:31");

    HMS hms;
    hms.Set( 0, 0, 0); CPPUNIT_ASSERT(hms.AddMinute     (     1) ==  0); CPPUNIT_ASSERT(hms == HMS( 0, 1, 0));
    hms.Set( 0, 0, 0); CPPUNIT_ASSERT(hms.AddMinute     (  1440) ==  1); CPPUNIT_ASSERT(hms == HMS( 0, 0, 0));
    hms.Set( 0,10, 3); CPPUNIT_ASSERT(hms.AddMinute     (     1) ==  0); CPPUNIT_ASSERT(hms == HMS( 0,11, 3));
    hms.Set( 9,10, 3); CPPUNIT_ASSERT(hms.AddMinute     (     2) ==  0); CPPUNIT_ASSERT(hms == HMS( 9,12, 3));
    hms.Set(12,59,59); CPPUNIT_ASSERT(hms.AddMinute     (    10) ==  0); CPPUNIT_ASSERT(hms == HMS(13, 9,59));
    hms.Set(19,59,59); CPPUNIT_ASSERT(hms.AddMinute     (   240) ==  0); CPPUNIT_ASSERT(hms == HMS(23,59,59));
    hms.Set(23,59,59); CPPUNIT_ASSERT(hms.AddMinute     (    18) ==  1); CPPUNIT_ASSERT(hms == HMS( 0,17,59));
    hms.Set(23,59,59); CPPUNIT_ASSERT(hms.AddMinute     (     1) ==  1); CPPUNIT_ASSERT(hms == HMS( 0, 0,59));

    hms.Set( 0, 0, 0); CPPUNIT_ASSERT(hms.SubtractMinute(     1) == -1); CPPUNIT_ASSERT(hms == HMS(23,59, 0));
    hms.Set( 0, 0, 0); CPPUNIT_ASSERT(hms.SubtractMinute(  1440) == -1); CPPUNIT_ASSERT(hms == HMS( 0, 0, 0));
    hms.Set( 0,10, 3); CPPUNIT_ASSERT(hms.SubtractMinute(     1) ==  0); CPPUNIT_ASSERT(hms == HMS( 0, 9, 3));
    hms.Set( 9,10, 3); CPPUNIT_ASSERT(hms.SubtractMinute(     2) ==  0); CPPUNIT_ASSERT(hms == HMS( 9, 8, 3));
    hms.Set(12, 0,59); CPPUNIT_ASSERT(hms.SubtractMinute(    10) ==  0); CPPUNIT_ASSERT(hms == HMS(11,50,59));
    hms.Set(19,59,59); CPPUNIT_ASSERT(hms.SubtractMinute(   240) ==  0); CPPUNIT_ASSERT(hms == HMS(15,59,59));
    hms.Set( 0, 9,59); CPPUNIT_ASSERT(hms.SubtractMinute(    18) == -1); CPPUNIT_ASSERT(hms == HMS(23,51,59));
    hms.Set(23,59,59); CPPUNIT_ASSERT(hms.SubtractMinute(     1) ==  0); CPPUNIT_ASSERT(hms == HMS(23,58,59));

    hms.Set( 0, 0, 0); CPPUNIT_ASSERT(hms.AddSecond     (     1) ==  0); CPPUNIT_ASSERT(hms == HMS( 0, 0, 1));
    hms.Set( 0, 0, 0); CPPUNIT_ASSERT(hms.AddSecond     ( 86400) ==  1); CPPUNIT_ASSERT(hms == HMS( 0, 0, 0));
    hms.Set( 0, 0, 0); CPPUNIT_ASSERT(hms.AddSecond     ( 86401) ==  1); CPPUNIT_ASSERT(hms == HMS( 0, 0, 1));
    hms.Set( 0,10, 3); CPPUNIT_ASSERT(hms.AddSecond     (     1) ==  0); CPPUNIT_ASSERT(hms == HMS( 0,10, 4));
    hms.Set( 9,10, 3); CPPUNIT_ASSERT(hms.AddSecond     (     2) ==  0); CPPUNIT_ASSERT(hms == HMS( 9,10, 5));
    hms.Set(12,59,59); CPPUNIT_ASSERT(hms.AddSecond     (    10) ==  0); CPPUNIT_ASSERT(hms == HMS(13, 0, 9));
    hms.Set(19,59,59); CPPUNIT_ASSERT(hms.AddSecond     ( 14400) ==  0); CPPUNIT_ASSERT(hms == HMS(23,59,59));
    hms.Set(23,59,59); CPPUNIT_ASSERT(hms.AddSecond     (    18) ==  1); CPPUNIT_ASSERT(hms == HMS( 0, 0,17));
    hms.Set(23,59,59); CPPUNIT_ASSERT(hms.AddSecond     (     1) ==  1); CPPUNIT_ASSERT(hms == HMS( 0, 0, 0));

    hms.Set( 0, 0, 0); CPPUNIT_ASSERT(hms.SubtractSecond(     1) == -1); CPPUNIT_ASSERT(hms == HMS(23,59,59));
    hms.Set( 0, 0, 0); CPPUNIT_ASSERT(hms.SubtractSecond( 86400) == -1); CPPUNIT_ASSERT(hms == HMS( 0, 0, 0));
    hms.Set( 0,10, 3); CPPUNIT_ASSERT(hms.SubtractSecond(     1) ==  0); CPPUNIT_ASSERT(hms == HMS( 0,10, 2));
    hms.Set( 9,10, 3); CPPUNIT_ASSERT(hms.SubtractSecond(     2) ==  0); CPPUNIT_ASSERT(hms == HMS( 9,10, 1));
    hms.Set(12, 0, 9); CPPUNIT_ASSERT(hms.SubtractSecond(    10) ==  0); CPPUNIT_ASSERT(hms == HMS(11,59,59));
    hms.Set(19,59,59); CPPUNIT_ASSERT(hms.SubtractSecond( 14400) ==  0); CPPUNIT_ASSERT(hms == HMS(15,59,59));
    hms.Set( 0, 0, 9); CPPUNIT_ASSERT(hms.SubtractSecond(    18) == -1); CPPUNIT_ASSERT(hms == HMS(23,59,51));
    hms.Set(23,59,59); CPPUNIT_ASSERT(hms.SubtractSecond(     1) ==  0); CPPUNIT_ASSERT(hms == HMS(23,59,58));


    //---------------------------------------------

    YMDHMS yh(2014,6,8,13,30,15);
    CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8));
    CPPUNIT_ASSERT(yh.GetHMS() == HMS(13,30,15));
    YMDHMS yh2(2014,6,8,13,30,16);
    YMDHMS yh3(2015,6,8,13,30,15);
    CPPUNIT_ASSERT(!(yh == yh2));
    CPPUNIT_ASSERT(!(yh == yh3));
    CPPUNIT_ASSERT(yh == yh);



    //---------------------------------------------
    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddMinute(     1); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0, 1, 0));
    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddMinute(  1440); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,9)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0, 0, 0));
    yh.SetYMD(2014,6,8); yh.SetHMS( 0,10, 3); yh.AddMinute(     1); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0,11, 3));
    yh.SetYMD(2014,6,8); yh.SetHMS( 9,10, 3); yh.AddMinute(     2); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 9,12, 3));
    yh.SetYMD(2014,6,8); yh.SetHMS(12,59,59); yh.AddMinute(    10); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS(13, 9,59));
    yh.SetYMD(2014,6,8); yh.SetHMS(19,59,59); yh.AddMinute(   240); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS(23,59,59));
    yh.SetYMD(2014,6,8); yh.SetHMS(23,59,59); yh.AddMinute(    18); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,9)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0,17,59));
    yh.SetYMD(2014,6,8); yh.SetHMS(23,59,59); yh.AddMinute(     1); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,9)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0, 0,59));

    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddSecond(     1); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0, 0, 1));
    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddSecond( 86400); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,9)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0, 0, 0));
    yh.SetYMD(2014,6,8); yh.SetHMS( 0, 0, 0); yh.AddSecond( 86401); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,9)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0, 0, 1));
    yh.SetYMD(2014,6,8); yh.SetHMS( 0,10, 3); yh.AddSecond(     1); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0,10, 4));
    yh.SetYMD(2014,6,8); yh.SetHMS( 9,10, 3); yh.AddSecond(     2); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 9,10, 5));
    yh.SetYMD(2014,6,8); yh.SetHMS(12,59,59); yh.AddSecond(    10); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS(13, 0, 9));
    yh.SetYMD(2014,6,8); yh.SetHMS(19,59,59); yh.AddSecond( 14400); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,8)); CPPUNIT_ASSERT(yh.GetHMS() == HMS(23,59,59));
    yh.SetYMD(2014,6,8); yh.SetHMS(23,59,59); yh.AddSecond(    18); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,9)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0, 0,17));
    yh.SetYMD(2014,6,8); yh.SetHMS(23,59,59); yh.AddSecond(     1); CPPUNIT_ASSERT(yh.GetYMD() == YMD(2014,6,9)); CPPUNIT_ASSERT(yh.GetHMS() == HMS( 0, 0, 0));


  }

  {
    YYYYMMDD ymd1;
    ymd1.Set(2014,6,29);
    CPPUNIT_ASSERT(ymd1.Year() == 2014);
    CPPUNIT_ASSERT(ymd1.Month() == 6);
    CPPUNIT_ASSERT(ymd1.Day() == 29);
  }


  {
    // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << endl << flush;
    YYYYMMDDHHMMSS ymdhms1(20150118,114200);
    const YYYYMMDD & ymd = ymdhms1.GetYYYYMMDD();
    const HHMMSS & hms = ymdhms1.GetHHMMSS();
    CPPUNIT_ASSERT(ymd.ToInt() == 20150118);
    CPPUNIT_ASSERT(hms.ToInt() == 114200);

    CPPUNIT_ASSERT(!(ymdhms1 < ymdhms1));
    CPPUNIT_ASSERT(ymdhms1 <= ymdhms1);
    CPPUNIT_ASSERT(!(ymdhms1 >  ymdhms1));
    CPPUNIT_ASSERT(ymdhms1 >= ymdhms1);
    CPPUNIT_ASSERT(ymdhms1 == ymdhms1);
    CPPUNIT_ASSERT(!(ymdhms1 != ymdhms1));

    YYYYMMDDHHMMSS ymdhms2(20150118,114201);
    CPPUNIT_ASSERT(ymdhms1 < ymdhms2);
    CPPUNIT_ASSERT(ymdhms1 <= ymdhms2);
    CPPUNIT_ASSERT(!(ymdhms1 >  ymdhms2));
    CPPUNIT_ASSERT(!(ymdhms1 >= ymdhms2));
    CPPUNIT_ASSERT(!(ymdhms1 == ymdhms2));
    CPPUNIT_ASSERT(ymdhms1 != ymdhms2);

    YYYYMMDDHHMMSS ymdhms3(20150119,114200);
    CPPUNIT_ASSERT(ymdhms1 < ymdhms3);
    CPPUNIT_ASSERT(ymdhms1 <= ymdhms3);
    CPPUNIT_ASSERT(!(ymdhms1 >  ymdhms3));
    CPPUNIT_ASSERT(!(ymdhms1 >= ymdhms3));
    CPPUNIT_ASSERT(!(ymdhms1 == ymdhms3));
    CPPUNIT_ASSERT(ymdhms1 != ymdhms3);

  }

  {
    YYYYMMDDHHMMSS ymdhms1(20150119,114200);
    YYYYMMDDHHMMSS ymdhms2(20150119,114201);
    CPPUNIT_ASSERT(ymdhms2 - ymdhms1 == 1);
    // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << ymdhms2 - ymdhms1 << endl << flush;
  }
  {
    YYYYMMDDHHMMSS ymdhms1(20150119,104200);
    YYYYMMDDHHMMSS ymdhms2(20150120,94200);
    CPPUNIT_ASSERT(ymdhms2 - ymdhms1 == 23*60*60);
    // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << ymdhms2 - ymdhms1 << endl << flush;
  }
  {
    YYYYMMDDHHMMSS ymdhms1(20150120,94200);
    YYYYMMDDHHMMSS ymdhms2(20150119,104200);
    CPPUNIT_ASSERT(ymdhms2 - ymdhms1 == -23*60*60);
    // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << ymdhms2 - ymdhms1 << endl << flush;
  } 


  {
    HHMMSS hms1(91800);
    HHMMSS hms2(101800);

    CPPUNIT_ASSERT(hms2 - hms1 ==  60*60);
    CPPUNIT_ASSERT(hms1 - hms2 == -60*60);
  }

  {
    HMS hms1(91800);
    CPPUNIT_ASSERT(hms1.Hour()   == 9);
    CPPUNIT_ASSERT(hms1.Minute() ==18);
    CPPUNIT_ASSERT(hms1.Second() == 0);
    HMS hms2(191850);
    CPPUNIT_ASSERT(hms2.Hour()   ==19);
    CPPUNIT_ASSERT(hms2.Minute() ==18);
    CPPUNIT_ASSERT(hms2.Second() ==50);
  }

  {
    YMDHMS ymdhms;
    ymdhms.Set(YYYYMMDD(20150407));
    ymdhms.Set(HHMMSS(140138));
    CPPUNIT_ASSERT(ymdhms.GetYMD() == YMD(2015, 4, 7));
    CPPUNIT_ASSERT(ymdhms.GetHMS() == HMS(14, 1,38));
  }

  {
    YYYYMMDDHHMMSS ymdhms("2015-10-17","04:30:00");
    YYYYMMDDHHMMSS ymdhmsOut = SDateTime::ChangeTimeZone(ymdhms, SDateTime::HKT, SDateTime::EST);
    CPPUNIT_ASSERT(ymdhmsOut == YYYYMMDDHHMMSS("2015-10-16","16:30:00"));
  }
  {
    YYYYMMDDHHMMSS ymdhms("2015-10-16","21:30:00");
    YYYYMMDDHHMMSS ymdhmsOut = SDateTime::ChangeTimeZone(ymdhms, SDateTime::HKT, SDateTime::EST);
    CPPUNIT_ASSERT(ymdhmsOut == YYYYMMDDHHMMSS("2015-10-16","09:30:00"));
  }
  {
    YYYYMMDDHHMMSS ymdhms("2015-10-16","11:30:00");
    YYYYMMDDHHMMSS ymdhmsOut = SDateTime::ChangeTimeZone(ymdhms, SDateTime::EST, SDateTime::HKT);
    CPPUNIT_ASSERT(ymdhmsOut == YYYYMMDDHHMMSS("2015-10-16","23:30:00"));
  }
  {
    YYYYMMDDHHMMSS ymdhms("2015-10-16","16:30:00");
    YYYYMMDDHHMMSS ymdhmsOut = SDateTime::ChangeTimeZone(ymdhms, SDateTime::EST, SDateTime::HKT);
    CPPUNIT_ASSERT(ymdhmsOut == YYYYMMDDHHMMSS("2015-10-17","04:30:00"));
  }

  {
    // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << SDateTime::fromUnixTimeToString(634695410, SDateTime::SECOND, SDateTime::GMT, SDateTime::GMT) << endl << flush;
    CPPUNIT_ASSERT(SDateTime::fromUnixTimeToString(634695410, SDateTime::SECOND, SDateTime::GMT, SDateTime::GMT) == "19900211_001650_000000");
    CPPUNIT_ASSERT(SDateTime::fromUnixTimeToString(634695410150, SDateTime::MILLISEC, SDateTime::GMT, SDateTime::GMT) == "19900211_001650_150000");
    CPPUNIT_ASSERT(SDateTime::fromUnixTimeToString(634695410150250, SDateTime::MICROSEC, SDateTime::GMT, SDateTime::GMT) == "19900211_001650_150250");
    CPPUNIT_ASSERT(SDateTime::fromUnixTimeToString(634695410150250350, SDateTime::NANOSEC, SDateTime::GMT, SDateTime::GMT) == "19900211_001650_150250");
    CPPUNIT_ASSERT(SDateTime::fromUnixTimeToString(634695410, SDateTime::SECOND, SDateTime::GMT, SDateTime::HKT) == "19900211_081650_000000");
    CPPUNIT_ASSERT(SDateTime::fromUnixTimeToString(10, SDateTime::DAY, SDateTime::GMT, SDateTime::GMT) == "19700111_000000_000000");
    CPPUNIT_ASSERT(SDateTime::fromStringToUnixTime("19700111_000000_000000", SDateTime::DAY) == 10);
    CPPUNIT_ASSERT(SDateTime::fromStringToUnixTime("19900211_001650_000000", SDateTime::SECOND) == 634695410);
    CPPUNIT_ASSERT(SDateTime::fromStringToUnixTime("19900211_001650_150250", SDateTime::MILLISEC) == 634695410150);
    CPPUNIT_ASSERT(SDateTime::fromStringToUnixTime("19900211_001650_150250", SDateTime::MICROSEC) == 634695410150250);
    CPPUNIT_ASSERT(SDateTime::fromStringToUnixTime("19900211_001650_150250", SDateTime::NANOSEC) == 634695410150250000);
  }

  {
    YYYYMMDDHHMMSS ymdhms1(634695410150250,SDateTime::MICROSEC,"");
    CPPUNIT_ASSERT(ymdhms1 == YYYYMMDDHHMMSS(1990,2,11,0,16,50));
    YYYYMMDDHHMMSS ymdhms2(634695410150,SDateTime::MILLISEC,"");
    CPPUNIT_ASSERT(ymdhms2 == YYYYMMDDHHMMSS(1990,2,11,0,16,50));
    YYYYMMDDHHMMSS ymdhms3(634695410,SDateTime::SECOND,"");
    CPPUNIT_ASSERT(ymdhms3 == YYYYMMDDHHMMSS(1990,2,11,0,16,50));
  }

  //	//Get Year / Month / Day
  //	CPPUNIT_ASSERT(SDateTime::GetYear(20110531) == 2011);
  //	CPPUNIT_ASSERT(SDateTime::GetYear(20110501) == 2011);
  //	CPPUNIT_ASSERT(SDateTime::GetYear(19701201) == 1970);
  //	CPPUNIT_ASSERT(SDateTime::GetYear(19991231) == 1999);
  //	CPPUNIT_ASSERT(SDateTime::GetYear(201105) == 2011);
  //	CPPUNIT_ASSERT(SDateTime::GetYear(202105) == 2021);
  //	CPPUNIT_ASSERT(SDateTime::GetYear(197012) == 1970);
  //	CPPUNIT_ASSERT(SDateTime::GetYear(199912) == 1999);
  //	CPPUNIT_ASSERT(SDateTime::GetMonth(20110531) == 5);
  //	CPPUNIT_ASSERT(SDateTime::GetMonth(20110501) == 5);
  //	CPPUNIT_ASSERT(SDateTime::GetMonth(20111201) == 12);
  //	CPPUNIT_ASSERT(SDateTime::GetMonth(20111231) == 12);
  //	CPPUNIT_ASSERT(SDateTime::GetMonth(201112) == 12);
  //	CPPUNIT_ASSERT(SDateTime::GetMonth(200101) == 1);
  //	CPPUNIT_ASSERT(SDateTime::GetMonth(190005) == 5);
  //	CPPUNIT_ASSERT(SDateTime::GetDay(20110531) == 31);
  //	CPPUNIT_ASSERT(SDateTime::GetDay(20110501) == 1);
  //	CPPUNIT_ASSERT(SDateTime::GetDay(20111201) == 1);
  //	CPPUNIT_ASSERT(SDateTime::GetDay(20111231) == 31);
  //
  //	int iY,iM,iD;
  //	SDateTime::GetYMD(20130103,iY,iM,iD); CPPUNIT_ASSERT(iY == 2013 && iM == 1 && iD == 3);
  //	SDateTime::GetYMD(19801231,iY,iM,iD); CPPUNIT_ASSERT(iY == 1980 && iM == 12 && iD == 31);
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130131) == 20130228);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130228) == 20130328);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130328) == 20130428);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130331) == 20130430);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130430) == 20130530);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000130) == 20000229);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000131) == 20000229);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000101) == 20000201);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000229) == 20000329);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000228) == 20000328);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130120,31) == 20130228);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130220,28) == 20130328);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130220,31) == 20130331);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130328,28) == 20130428);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130328,30) == 20130430);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20130328,31) == 20130430);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000120,31) == 20000229);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000220,28) == 20000328);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000220,31) == 20000331);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000328,28) == 20000428);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000328,30) == 20000430);
  //	CPPUNIT_ASSERT(SDateTime::NextMonthStrict(20000328,31) == 20000430);
  //
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::YYYY_YY(2010).compare("10") == 0);
  //	CPPUNIT_ASSERT(SDateTime::YYYY_YY(2009).compare("09") == 0);
  //	CPPUNIT_ASSERT(SDateTime::YYYY_YY(1980).compare("80") == 0);
  //	CPPUNIT_ASSERT(SDateTime::YYYY_YY(2000).compare("00") == 0);
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::YMDi("2011,3,4") == 20110304);
  //	CPPUNIT_ASSERT(SDateTime::YMDi("2013-01-23") == 20130123);
  //	try { SDateTime::YMDi("2013-01"); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //	CPPUNIT_ASSERT(SDateTime::YMDi(2011,3,4) == 20110304);
  //	CPPUNIT_ASSERT(SDateTime::YMDi(2013,01,23) == 20130123);
  //
  //	CPPUNIT_ASSERT(SDateTime::YMi("2011,3,4") == 201103);
  //	CPPUNIT_ASSERT(SDateTime::YMi("2013-01-23") == 201301);
  //
  //	CPPUNIT_ASSERT(SDateTime::YMi("2010-2") == 201002);
  //	CPPUNIT_ASSERT(SDateTime::YMi("2007-08") == 200708);
  //	try { SDateTime::YMi("2013"); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //	CPPUNIT_ASSERT(SDateTime::YMi(2011,3,4) == 201103);
  //	CPPUNIT_ASSERT(SDateTime::YMi(2013,01,23) == 201301);
  //
  //	CPPUNIT_ASSERT(SDateTime::YMi(2010,2) == 201002);
  //	CPPUNIT_ASSERT(SDateTime::YMi(2007,8) == 200708);
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::YMi(20061005) == 200610);
  //	CPPUNIT_ASSERT(SDateTime::YMi(200611) == 200611);
  //
  //
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::HMSi("02:12:58") == 21258);
  //	CPPUNIT_ASSERT(SDateTime::HMSi("19:45") == 194500);
  //	CPPUNIT_ASSERT(SDateTime::HMSi("03:56") == 35600);
  //
  //	try { SDateTime::HMSi("ew:45:22"); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //	CPPUNIT_ASSERT(SDateTime::HMSi(2,12,58) == 21258);
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::HMSs(94234).compare("09:42:34") == 0);
  //	CPPUNIT_ASSERT(SDateTime::HMSs(194234).compare("19:42:34") == 0);
  //	CPPUNIT_ASSERT(SDateTime::HMSs(190234).compare("19:02:34") == 0);
  //	try { SDateTime::HMSs(1945); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::HMi("02:13:58") == 213);
  //	CPPUNIT_ASSERT(SDateTime::HMi("13:21:58") == 1321);
  //	try { SDateTime::HMi("19"); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::HMi(2,13,58) == 213);
  //	CPPUNIT_ASSERT(SDateTime::HMi(13,21,58) == 1321);
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::HMs(194234).compare("19:42") == 0);
  //	try { SDateTime::HMs(19); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::YMDs(20130123).compare("2013-01-23") == 0);
  //	CPPUNIT_ASSERT(SDateTime::YMDs(19991003).compare("1999-10-03") == 0);
  //	try { SDateTime::YMDs(199709); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //	try { SDateTime::YMDs(1996); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //
  //
  //
  //	CPPUNIT_ASSERT(SDateTime::YMs(200211).compare("2002-11") == 0);
  //	CPPUNIT_ASSERT(SDateTime::YMs(20141123).compare("2014-11") == 0);
  //	try { SDateTime::YMs(1996); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); }
  //	catch (exception& e) {}
  //
  //	CPPUNIT_ASSERT(SDateTime::YMDs(2001,04,11).compare("2001-04-11") == 0);
  //	CPPUNIT_ASSERT(SDateTime::YMs(2009,04,11).compare("2009-04") == 0);
  //	CPPUNIT_ASSERT(SDateTime::YMs(2003,03).compare("2003-03") == 0);
  //
  //	//NextYMi
  //	CPPUNIT_ASSERT(SDateTime::NextYMi(200303,-4) == 200211);
  //	CPPUNIT_ASSERT(SDateTime::NextYMi(200303,-3) == 200212);
  //	CPPUNIT_ASSERT(SDateTime::NextYMi(200303,-2) == 200301);
  //	CPPUNIT_ASSERT(SDateTime::NextYMi(200303,-1) == 200302);
  //	CPPUNIT_ASSERT(SDateTime::NextYMi(200303,0) == 200303);
  //	CPPUNIT_ASSERT(SDateTime::NextYMi(200303,1) == 200304);
  //	CPPUNIT_ASSERT(SDateTime::NextYMi(200303,5) == 200308);
  //	CPPUNIT_ASSERT(SDateTime::NextYMi(200303,9) == 200312);
  //	CPPUNIT_ASSERT(SDateTime::NextYMi(200303,10) == 200401);
  //
  //
  //	//NextHMi
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(0003,-80) == 2243);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(0003,-4) == 2359);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(1615,-3) == 1612);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(2301,-2) == 2259);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(1801,-1) == 1800);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(2359,0) == 2359);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(2359,1) == 0);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(2359,5) == 4);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(2359,9) == 8);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(1345,10) == 1355);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(1345,70) == 1455);
  //	CPPUNIT_ASSERT(SDateTime::NextHMi(2345,70) == 55);
  //
  //	CPPUNIT_ASSERT(SDateTime::YYYYMM(20230625) == 202306);
  //	CPPUNIT_ASSERT(SDateTime::YYYYMM(20030625) == 200306);
  //	CPPUNIT_ASSERT(SDateTime::YYYYMM(19970625) == 199706);
  //
  //	//Month diff
  //	CPPUNIT_ASSERT(SDateTime::MthDiff(201010,201105) == 7);
  //	CPPUNIT_ASSERT(SDateTime::MthDiff(201105,201010) == -7);
  //	CPPUNIT_ASSERT(SDateTime::MthDiff(201105,201105) == 0);
  //	CPPUNIT_ASSERT(SDateTime::MthAbsDiff(201010,201105) == 7);
  //	CPPUNIT_ASSERT(SDateTime::MthAbsDiff(201105,201010) == 7);
  //	CPPUNIT_ASSERT(SDateTime::MthAbsDiff(201105,201105) == 0);

  return;
}


void TestSDateTime::setUp()
{
}

void TestSDateTime::tearDown()
{
}

CppUnit::Test *TestSDateTime::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSDateTime");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestSDateTime>(
        "TestSDateTime",
        &TestSDateTime::RunTest));

  return suiteOfTests;
}

