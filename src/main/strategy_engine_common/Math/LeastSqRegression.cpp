#include "LeastSqRegression.h"

LeastSqRegression::LeastSqRegression(unsigned int iHighestOrder) :
  _iHighestOrder(iHighestOrder),
  _bSolved(false)
{
}

LeastSqRegression::~LeastSqRegression()
{
}

void LeastSqRegression::AddPoint(double x, double y)
{
  _daX.push_back(x);
  _daY.push_back(y);
}

vector<double> LeastSqRegression::Solve()
{
  if (_daX.size() < _iHighestOrder+1 ||
      _daY.size() < _iHighestOrder+1
     ) return vector<double>();

  if (_bSolved) return _vSoln;

  vector<double> vMatrixA;
  vector<double> vVectorB;

  for (unsigned int j = 0; j < _iHighestOrder+1; ++j)
  {
    for (unsigned int i = 0; i < _iHighestOrder+1; ++i)
    {
      int k = (2*_iHighestOrder)-j-i;

      if (k > 0)
        vMatrixA.push_back(get_sum_x(k));
      else
        vMatrixA.push_back(_daX.size());
    }
  }

  for (unsigned int j = 0; j < _iHighestOrder+1; ++j)
  {
    vVectorB.push_back(get_sum_xy(_iHighestOrder-j));
  }

  LinearSysSolver lss(_iHighestOrder+1);
  lss.SpecifyMatrixA(vMatrixA);
  lss.SpecifyVectorB(vVectorB);

  // lss.PrintMatrixA();
  // lss.PrintVectorB();

  _vSoln = lss.Solve();

  _bSolved = true;
  return _vSoln;
}

double LeastSqRegression::get_sum_x(double dXPower)
{
  double dRtn = 0;
  for (unsigned int i = 0; i < _daX.size(); i++)
    dRtn += pow(_daX[i],dXPower);
  return dRtn;
}
double LeastSqRegression::get_sum_xy(double dXPower)
{
  double dRtn = 0;
  for (unsigned int i = 0; i < _daY.size(); i++)
    dRtn += (pow(_daX[i], dXPower) * _daY[i]);
  return dRtn;
}
