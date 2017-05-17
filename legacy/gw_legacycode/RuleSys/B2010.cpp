#include "B2010.h"

const char * B2010::Name() { return "B2010"; }
B2010::B2010(const double *dParamVector)
{
	_dInitSize[0] = dParamVector[0];
	_dInitSize[1] = dParamVector[1];
	_dInitSize[2] = dParamVector[2];

	Reset();
}

void B2010::ResetChild()
{
	_sma3.Reset(3,true);
	_sma5.Reset(5,true);
	_sma10.Reset(10,true);
	_atr.Reset(0.1,true);
}

void B2010::Add(const BarProvider &bA, bool bAtDayOpen)
{
	double dPx = 0;
	if (bAtDayOpen) dPx = bA.Open(); else dPx = bA.Close();

	// Update indicators
	if (!bAtDayOpen)
	{
		_sma3.Add(bA.Close());
		_sma5.Add(bA.Close());
		_sma10.Add(bA.Close());
		_atr.Add(bA.High(),bA.Low(),bA.Close());
	}

	if (!_sma10.Ready() || !_atr.Ready()) return;



	// Calculate Position size
	//Monthly weights
	int iSysB3MthW = 0;
	int iSysB5MthW = 0;
	int iSysB10MthW = 0; 

	//System B3: Monthly weight
	//According to probability of crossing
	switch (bA.GetYYYYMMDD().Month())
	{
		case 1 : if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 3; else iSysB3MthW = 2; break;
		case 2 : if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 1; else iSysB3MthW = 2; break;
		case 3 : if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 2; else iSysB3MthW = 2; break;
		case 4 : if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 2; else iSysB3MthW = 2; break;
		case 5 : if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 2; else iSysB3MthW = 3; break;
		case 6 : if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 1; else iSysB3MthW = 2; break;
		case 7 : if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 3; else iSysB3MthW = 3; break;
		case 8 : if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 1; else iSysB3MthW = 2; break;
		case 9 : if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 3; else iSysB3MthW = 2; break;
		case 10: if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 3; else iSysB3MthW = 1; break;
		case 11: if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 3; else iSysB3MthW = 2; break;
		case 12: if ((bA.GetYYYYMMDD().Day()) < 15) iSysB3MthW = 2; else iSysB3MthW = 2; break;
		default: iSysB3MthW = 0; break;
	}

	//System B5: Monthly weight
	//According to probability of crossing
	switch (bA.GetYYYYMMDD().Month())
	{
		case 1 : if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
		case 2 : if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
		case 3 : if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
		case 4 : if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 3; else iSysB5MthW = 2; break;
		case 5 : if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 2; else iSysB5MthW = 3; break;
		case 6 : if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
		case 7 : if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 2; else iSysB5MthW = 3; break;
		case 8 : if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
		case 9 : if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
		case 10: if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 3; else iSysB5MthW = 2; break;
		case 11: if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 2; else iSysB5MthW = 2; break;
		case 12: if (bA.GetYYYYMMDD().Day() < 15) iSysB5MthW = 1; else iSysB5MthW = 2; break;
		default: iSysB5MthW = 0; break;
	}

	//System B10: Monthly weight
	//According to probability of crossing
	switch (bA.GetYYYYMMDD().Month())
	{
		case 1:  if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 2; else iSysB10MthW = 1; break;
		case 2:  if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 1; else iSysB10MthW = 2; break;
		case 3:  if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
		case 4:  if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
		case 5:  if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 2; else iSysB10MthW = 1; break;
		case 6:  if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
		case 7:  if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
		case 8:  if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 1; else iSysB10MthW = 2; break;
		case 9:  if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
		case 10: if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 3; else iSysB10MthW = 2; break;
		case 11: if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 2; else iSysB10MthW = 2; break;
		case 12: if (bA.GetYYYYMMDD().Day() < 15) iSysB10MthW = 1; else iSysB10MthW = 2; break;
		default: iSysB10MthW = 0; break;
	}

	//Volatility-adjusted position size
	double dAlpha = _atr.Value() / dPx;
	double dVolAdjB3 = pow(1 / dAlpha, 1.3) / 250;
	double dVolAdjB5 = pow(1 / dAlpha, 1) / 250;
	double dVolAdjB10 = pow(1 / dAlpha, 1) / 250;

	//System B3, B5, B10: Position size
	double dPosSize3 = _dInitSize[0] * (double)iSysB3MthW * dVolAdjB3 / dPx;
	int iPosSize3 = (int) STool::Round(dPosSize3, 0);
	double dPosSize5 = _dInitSize[1] * (double)iSysB5MthW * dVolAdjB5 / dPx;
	int iPosSize5 = (int) STool::Round(dPosSize5, 0);
	double dPosSize10 = _dInitSize[2] * (double)iSysB10MthW * dVolAdjB10 / dPx;
	int iPosSize10 = (int) STool::Round(dPosSize10, 0);



	// Generate Signals
	// Trading logic
	// Execute orders
	this->_iPos = 0;

	if (dPx > _sma3.Value()) this->_iPos += iPosSize3;
	else if (dPx < _sma3.Value()) this->_iPos -= iPosSize3;
	if (dPx > _sma5.Value()) this->_iPos += iPosSize5;
	else if (dPx < _sma5.Value()) this->_iPos -= iPosSize5;
	if (dPx > _sma10.Value()) this->_iPos += iPosSize10;
	else if (dPx < _sma10.Value()) this->_iPos -= iPosSize10;

}
