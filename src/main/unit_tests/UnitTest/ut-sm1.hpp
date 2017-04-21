#include "UTest.h"
#include "Util/BarProvider.h"
#include "Util/HKFE.h"
#include "Util/SDateTime.h"
#include "Math/StateMach1.h"
#include <iostream>
#include <cmath>

int TestSM1()
{
	UTest ut;
	StateMach1 sm1;

	BarProvider bp("./UnitTest/data-old.csv", "DOHLCV", 2, 'F', 'N', 'P');
	BarProvider bp1("./UnitTest/0001.HK.csv", "DOHLCV", 2, 'B', 'H', 'P');
	BarProvider bp2628("./UnitTest/2628.HK.csv", "DOHLCV", 2, 'B', 'H', 'P');
	BarProvider bpv("./UnitTest/data-vhsi-old.csv", "DOHLCV", 2, 'F', 'N', 'P');


	YMD ymdTrdgDate(2000,1,2);
	YYYYMMDD ymdTermination(20130103);


	cout << "State:" << endl;

	bp1.MoveNext();
	bpv.MoveNext();

	while (ymdTrdgDate < ymdTermination)
	{
		while (bp1.GetYYYYMMDD() < ymdTrdgDate && bp1.HasNextBar()) bp1.MoveNext();
		while (bpv.GetYYYYMMDD() < ymdTrdgDate && bpv.HasNextBar()) bpv.MoveNext();

		if (bp1.GetYYYYMMDD() == bpv.GetYYYYMMDD())
			sm1.Add(bp1.High(), bp1.Low(), bp1.Close(), bpv.Close());

		if (sm1.Ready() && ymdTrdgDate > 20121201)
		{
			cout << sm1.State() << "," << bp1.GetYYYYMMDD().ToStrYYYYMM() << "," << bp1.Close() << "," << bpv.Close() << endl;
		}
		HKFE::NextTrdgDate(ymdTrdgDate,1);

	}


	cout << sm1.StateDesc(16) << endl;



	ut.PrintResult();


	return 0;
}
