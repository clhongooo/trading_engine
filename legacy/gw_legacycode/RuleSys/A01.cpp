#include "A01.h"

const char * A01::Name() { return "A01"; }
A01::A01(const double *dParamVector)
{
	_iEntry1 = (int) dParamVector[0];
	_iEntry2 = (int) dParamVector[1];
	_iExit1 = (int) dParamVector[2];
	_iExit2 = (int) dParamVector[3];
	_iPermittedTrdDir = (int) dParamVector[4];
	_iReverse = (int) dParamVector[5];

	Reset();
}

void A01::DailyInit()
{
  _iPos = 0;
}

void A01::ResetChild()
{
	_smaEntry1.Reset(_iEntry1,true);
	_smaEntry2.Reset(_iEntry2,true);
	_smaExit1.Reset(_iExit1,true);
	_smaExit2.Reset(_iExit2,true);
}

void A01::Add(const BarProvider &bA)
{
	// Update indicators
	_smaEntry1.Add(bA.Close());
	_smaEntry2.Add(bA.Close());
	_smaExit1.Add(bA.Close());
	_smaExit2.Add(bA.Close());

	if (!_smaEntry1.Ready() || !_smaEntry2.Ready() || !_smaExit1.Ready() || !_smaExit2.Ready()) return;

  // Generate Signals
  bool bGoLong     = false;
  bool bCloseLong  = false;
  bool bGoShort    = false;
  bool bCloseShort = false;

	// Trading logic
	if (_iPermittedTrdDir != -1 && (_iReverse * this->_iPos) == 0) if (_smaEntry1.Value() > _smaEntry2.Value()) bGoLong = true;
	if (_iPermittedTrdDir !=  1 && (_iReverse * this->_iPos) == 0) if (_smaEntry1.Value() < _smaEntry2.Value()) bGoShort = true;
	if (_iPermittedTrdDir != -1 && (_iReverse * this->_iPos)  > 0) if (_smaExit1.Value() < _smaExit2.Value()) bCloseLong = true;
	if (_iPermittedTrdDir !=  1 && (_iReverse * this->_iPos)  < 0) if (_smaExit1.Value() > _smaExit2.Value()) bCloseShort = true;

	// Execute orders
	if (bGoLong)     this->_iPos = 1 * _iReverse;
	if (bGoShort)    this->_iPos = -1 * _iReverse;
	if (bCloseLong)  this->_iPos = 0;
	if (bCloseShort) this->_iPos = 0;



}
