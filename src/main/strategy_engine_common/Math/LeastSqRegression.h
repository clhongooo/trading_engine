#ifndef LEASTSQREGRESSION_H_
#define LEASTSQREGRESSION_H_

#include "PCH.h"
#include "Constants.h"
#include "LinearSysSolver.h"

class LeastSqRegression {
  public:
    LeastSqRegression(unsigned int);
    virtual ~LeastSqRegression();

    void AddPoint(double, double);
    vector<double> Solve();

  private:
    double get_sum_x(double);
    double get_sum_xy(double);

    unsigned int _iHighestOrder;
    vector<double> _daX;
    vector<double> _daY;
    vector<double> _vSoln;

    bool _bSolved;
};

#endif /* LEASTSQREGRESSION_H_ */
