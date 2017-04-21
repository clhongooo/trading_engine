#include "UTest.h"
#include "Math/SgnlPrc.h"
#include "Util/BarProvider.h"
#include <iostream>
#include <string>
#include <cmath>

int TestSP()
{
	UTest ut;
	SgnlPrc sp;




	/*
		 double p = 25;
		 int i = 2;
		 while (i < 1000)
		 {
	//double n = ((i+7)%100)+32;
	double n = i%1000;
	p *= 1.0001;
	double d = 100 * sin(2 * M_PI /p *n) + 200;
	sp.Add(d+0.1,d-0.1,d);
	if (sp.Ready())
	//cout << "d=\t" << d << "\tp=" << p << "\tGS=" << sp.Period_Guess() << "\tHD=" << sp.Period_HD() << "\tDD=" << sp.Period_DD() << endl;
	cout << (d-200)/100 << "\t" << sp.SineWaveI() << "\t" << sp.SineWaveILead() << endl;
	//cout << "d=\t" << d << "\tSNR=" << sp.SNR() << endl;
	i++;
	}
	 */



	/**/
	BarProvider bp("./UnitTest/0001.HK.csv", "DOHLCV", 2, 'B', 'H', 'P');

	while (bp.HasNextBar())
	{
		bp.MoveNext();
		sp.Add(bp.High(), bp.Low(), bp.Close());

		if (sp.Ready())
			//cout << bp.YMDi() << "\tp=" << sp.Period_HD_DD() << "\tSNR=" << sp.SNR() << endl;
			//cout << "p=\t" << sp.Period_HD_DD() << "\tOBLMA=\t" << sp.OBLMA() << "\tSWI=\t" << sp.SineWaveI() << "\tSWIL=\t" << sp.SineWaveILead() << endl;
			//cout << bp.YMDi() << "\tp=\t" << bp.Close << "\tOBLMA=\t" << sp.OBLMA() << endl;
			cout << bp.GetYYYYMMDD().ToStrYYYYMMDD() << "," << sp.TrendCycleMode() << endl;
	}
	/**/







	//ut.Assert(,__FILE__,__FUNCTION__,__LINE__);



	ut.PrintResult();


	return 0;
}
