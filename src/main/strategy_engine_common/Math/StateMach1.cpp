#include "StateMach1.h"

const char * StateMach1::Name() { return "swv"; }
StateMach1::StateMach1()
{
	//20130207
	//score:	124:		20,50,100,60
	//score:	150:		10,20,50,20
	//score:	150:		7,15,50,20
	//score:	151:		5,10,20,10
	//score:	152:		7,15,100,60
	//score:	153:		10,20,100,40
	//score:	154:		7,15,200,40
	//score:	161:		7,15,100,40
	//score:	170:		5,15,100,40
	//score:	177:		5,10,100,40
	//score:	178:		3,10,100,40
	//score:	189:		3,5,50,40
	//score:	190:		2,5,100,40
	//score:	194:		3,5,100,40



	//20130208
	//score:	17.80:		3,5,100,40 (without vhsi)
	//score:	35.61:		20,50,100,60
	//score:	42.55:		7,15,100,40
	//score:	43.35:		3,5,100,40






	_sma1.Reset(3,true);
	_sma2.Reset(5,true);
	_sma3.Reset(100,true);
	_wr1.Reset(40,true);
	_dqVhsi.clear();
}

void StateMach1::Add(double dH, double dL, double dC, double dVHSI)
{
	_dH = dH;
	_dL = dL;
	_dC = dC;
	_sma1.Add(dC);
	_sma2.Add(dC);
	_sma3.Add(dC);
	_wr1.Add(dH,dL,dC);
	_dqVhsi.push_back(dVHSI);
}

int StateMach1::TotalStates() { return 64; }
int StateMach1::State()
{
	if (!Ready())
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	unsigned char ucBinState = 0b0000;
	if (_dC >= _sma1.Value()) ucBinState += 0x1; //1
	if (_dC >= _sma2.Value()) ucBinState += 0x2; //2
	if (_dC >= _sma3.Value()) ucBinState += 0x4; //4

	// 1: extreme values
	// 0: otherwise
	if (_wr1.Value() > -10 || _wr1.Value() < -90) ucBinState += 0x8; //8

	// 11: > 104
	// 10: > 23
	// 01: > 19
	// 00: otherwise
	if (_dqVhsi.back() > 104) ucBinState += 0x30; //16+32
	else if (_dqVhsi.back() > 23) ucBinState += 0x20; //32
	else if (_dqVhsi.back() > 19) ucBinState += 0x10; //16


	return ucBinState;	
}
string StateMach1::StateDesc(int iState)
{
	string sState = STool::PadLeft(STool::DecToBin(iState),'0',6);
	string sDesc("");

	if (sState.substr(0,2).compare("11") == 0) sDesc += "VHSI > 104, ";
	else if (sState.substr(0,2).compare("10") == 0) sDesc += "VHSI=23:104, ";
	else if (sState.substr(0,2).compare("01") == 0) sDesc += "VHSI=19:23, ";
	else sDesc += "VHSI=0:19, ";

	if (sState.at(2)=='1') sDesc += "WR extreme, ";
	else sDesc += "WR normal, ";

	if (sState.at(3)=='1') sDesc += "> SMA3, ";
	else sDesc += "< SMA3, ";
	if (sState.at(4)=='1') sDesc += "> SMA2, ";
	else sDesc += "< SMA2, ";
	if (sState.at(5)=='1') sDesc += "> SMA1.";
	else sDesc += "< SMA1.";

	return sDesc;

}

bool StateMach1::Ready() const
{
	bool bWU = true;
	if (bWU && !_sma1.Ready()) bWU = false;
	if (bWU && !_sma2.Ready()) bWU = false;
	if (bWU && !_sma3.Ready()) bWU = false;
	if (bWU && !_wr1.Ready()) bWU = false;
	if (bWU && _dqVhsi.empty()) bWU = false;
	return bWU;
}


