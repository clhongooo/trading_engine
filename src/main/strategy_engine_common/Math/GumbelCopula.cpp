#include <GumbelCopula.h>

GumbelCopula::GumbelCopula() :
  m_theta(NAN),
  m_delta(0.01)
{
}

GumbelCopula::~GumbelCopula()
{
}

void GumbelCopula::SetParam(const double theta)
{
  m_theta = theta;
}

double GumbelCopula::C(const double u, const double v)
{
  if (u > 1 || u < 0 || v > 1 || v < 0 || m_theta == NAN) return NAN;

  return 
    exp
    (
     - pow
     (
      pow(-log(u),m_theta)
      +
      pow(-log(v),m_theta)
      ,
      1/m_theta
     )
    );
}

double GumbelCopula::Prob_U_smlr_u_V_eq_v(const double u, const double v)
{
  if (u > 1 || u < 0 || v > 1 || v < 0 || m_theta == NAN) return NAN;

  double dNumerator = 0;
  double dDenominator = 0;

  if (v < m_delta)
  {
    dNumerator =
      +C(u,v+1.5*m_delta)
      -C(u,v)
      -C(0,v+1.5*m_delta)
      +C(0,v);

    dDenominator =
      +C(1,v+1.5*m_delta)
      -C(1,v)
      -C(0,v+1.5*m_delta)
      +C(0,v);
  }
  else if (v > 1-m_delta)
  {
    dNumerator =
      +C(u,v)
      -C(u,v-1.5*m_delta)
      -C(0,v)
      +C(0,v-1.5*m_delta);

    dDenominator =
      +C(1,v)
      -C(1,v-1.5*m_delta)
      -C(0,v)
      +C(0,v-1.5*m_delta);
  }
  else
  {
    dNumerator =
      +C(u,v+m_delta)
      -C(u,v-m_delta)
      -C(0,v+m_delta)
      +C(0,v-m_delta);

    dDenominator =
      +C(1,v+m_delta)
      -C(1,v-m_delta)
      -C(0,v+m_delta)
      +C(0,v-m_delta);
  }

  return dNumerator / dDenominator;
}

