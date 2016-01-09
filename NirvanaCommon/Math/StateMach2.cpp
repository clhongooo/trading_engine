#include "StateMach2.h"

const char * StateMach2::Name() { return "sp"; }
StateMach2::StateMach2() : _dH(-1), _dL(-1), _dC(-1), _dVHSI(-1)
{
	//ranking: OBLMA > ATR  > Period > Sine > VHSI > DistPASMA || > TrendCycle > SD (> WR) (> PASMA > SNR)
	//ATR/price ratio does not improve score, but is reasonble to include

	//Comparison base: Sine,Period,ATR,OBLMA,VHSI,DistPASMA (15,25,10,2,2,10)
	//score:	94.20:		(base case)

	//approx using HSI
	//score:	~81.71:		(ATR SMA 5 instead of 10)
	//score:	~83.52:		(ATR SMA 15 instead of 10)
	//score:	~84.19:		(base case)

	_atr.Reset((double)1/15,true);
	_sma_Period.Reset(25,true);
	_sma_ATR.Reset(10,true);
	_sma_OBLMA_1.Reset(2,true);
	_sma_VHSI.Reset(2,true);
	_sma_distPASMA.Reset(10,true);

}

void StateMach2::Add(double dH, double dL, double dC, double dVHSI)
{
	_dH = dH;
	_dL = dL;
	_dC = dC;
	_dVHSI = dVHSI;

	_sma_VHSI.Add(dVHSI);
	_atr.Add(dH,dL,dC);
	if (_atr.Ready()) _sma_ATR.Add(_atr.Value());

	_sp.Add(dH,dL,dC);
	if (_sp.Ready())
	{
		_sma_Period.Add(_sp.Period_HD_DD());
		_sma_OBLMA_1.Add(_sp.OBLMA());
		_sma_distPASMA.Add(abs(_sp.OBLMA()-_sp.PASMA()));
	}
}

int StateMach2::TotalStates() { return 64; }
int StateMach2::State()
{
	if (!Ready())
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	unsigned char ucBinState = 0b0000;
	if (_dVHSI >= _sma_VHSI.Value()) ucBinState += 0x1; //1
	if (_sp.Period_HD_DD() >= _sma_Period.Value()) ucBinState += 0x2; //10
	if (_atr.Value() >= _sma_ATR.Value()) ucBinState += 0x4; //100
	if (_sp.OBLMA() >= _sma_OBLMA_1.Value()) ucBinState += 0x8; //1000
	if (abs(_sp.OBLMA()-_sp.PASMA()) >= _sma_distPASMA.Value()) ucBinState += 0x10;
	if (_sp.SineWaveILead() >= _sp.SineWaveI()) ucBinState += 0x20; //100000
	//percentile of all indvl stk: 2.21%, 3.36%, 5.10%, 9.69%
	//if (_sma_ATR.Value()/_dC >= 0.0510) ucBinState += 0xC0; //0x80+0x40
	//if (_sma_ATR.Value()/_dC >= 0.0336) ucBinState += 0x80; //0x80
	//if (_sma_ATR.Value()/_dC >= 0.0221) ucBinState += 0x40; //0x40

	//deprecated
	//if (_sp.TrendCycleMode()) ucBinState += 0x40; //1000000
	//if (_ss.SD(0) >= _sma_SD.Value()) ucBinState += 0x1; //1
	//if (_sp.SNR() >= _sma_SNR.Value()) ucBinState += 0x1; //1
	//if (_sp.OBLMA() >= _sp.PASMA()) ucBinState += 0x10; //10000

	return ucBinState;	
}
string StateMach2::StateDesc(int iState)
{
	string sState = STool::PadLeft(STool::DecToBin(iState),'0',6+2); //64*4
	string sDesc("");
	return sDesc;//skip logic for now

	if (sState.at(0)=='1') sDesc += "To be done, ";
	else sDesc += "To be done, ";


	return sDesc;

}

bool StateMach2::Ready() const
{
	bool bWU = true;
	if (bWU && !_sp.Ready()) bWU = false;
	if (bWU && !_sma_Period.Ready()) bWU = false;
	if (bWU && !_sma_ATR.Ready()) bWU = false;
	if (bWU && !_sma_OBLMA_1.Ready()) bWU = false;
	if (bWU && !_sma_distPASMA.Ready()) bWU = false;
	return bWU;
}


