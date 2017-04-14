#include "LinearSysSolver.h"

LinearSysSolver::LinearSysSolver(unsigned int iNoOfUnknowns) :
  _iNoOfUnknowns(iNoOfUnknowns)
{
}

LinearSysSolver::~LinearSysSolver()
{
}



void LinearSysSolver::SpecifyMatrixA(const vector<double> & vMatrixA)
{
  if (vMatrixA.size() != _iNoOfUnknowns * _iNoOfUnknowns) return;
  _vMatrixA = vMatrixA;
}

void LinearSysSolver::SpecifyVectorB(const vector<double> & vVectorB)
{
  if (vVectorB.size() != _iNoOfUnknowns) return;
  _vVectorB = vVectorB;
}

// void LinearSysSolver::PrintMatrixA()
// {
//   if (_vMatrixA.empty()) return;
//   cout << "MatrixA:" << endl;
//   vector<double>::iterator it;
//   for (it = _vMatrixA.begin(); it != _vMatrixA.end(); ++it) cout << *it << " ";
//   cout << endl << flush;
// }
//
// void LinearSysSolver::PrintVectorB()
// {
//   if (_vVectorB.empty()) return;
//   cout << "VectorB:" << endl;
//   vector<double>::iterator it;
//   for (it = _vVectorB.begin(); it != _vVectorB.end(); ++it) cout << *it << " ";
//   cout << endl << flush;
// }

vector<double> LinearSysSolver::Solve()
{
  double *a_data = &_vMatrixA[0];
  double *b_data = &_vVectorB[0];

  gsl_set_error_handler_off();
  gsl_matrix_view m = gsl_matrix_view_array (a_data, _iNoOfUnknowns, _iNoOfUnknowns);
  gsl_vector_view b = gsl_vector_view_array (b_data, _iNoOfUnknowns);
  gsl_vector *x = gsl_vector_alloc (_iNoOfUnknowns);
  int s;
  gsl_permutation * p = gsl_permutation_alloc (_iNoOfUnknowns);

  int status = 0; 
  status = gsl_linalg_LU_decomp (&m.matrix, p, &s);
  if (status)
  {
    gsl_permutation_free (p);
    gsl_vector_free (x);

    return vector<double>();
  }

  status = gsl_linalg_LU_solve (&m.matrix, p, &b.vector, x);
  if (status)
  {
    gsl_permutation_free (p);
    gsl_vector_free (x);

    return vector<double>();
  }

  vector<double> rtnVec(x->data, x->data+_iNoOfUnknowns);

  gsl_permutation_free (p);
  gsl_vector_free (x);

  return rtnVec;
}
