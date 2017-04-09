#ifndef LINEARSYSSOLVER_H_
#define LINEARSYSSOLVER_H_

#include "PCH.h"
#include "Constants.h"
#include <gsl/gsl_linalg.h>

using namespace std;

class LinearSysSolver {
  public:
    LinearSysSolver(unsigned int);
    virtual ~LinearSysSolver();
    void SpecifyMatrixA(const vector<double> &);
    void SpecifyVectorB(const vector<double> &);
    // void PrintMatrixA();
    // void PrintVectorB();
    vector<double> Solve();
  private:
    unsigned int _iNoOfUnknowns;
    vector<double> _vMatrixA;
    vector<double> _vVectorB;
};

#endif /* LINEARSYSSOLVER_H_ */
