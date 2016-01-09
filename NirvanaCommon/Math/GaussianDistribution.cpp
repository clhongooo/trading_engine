#include <GaussianDistribution.h>

GaussianDistribution::GaussianDistribution() :
  m_mean(NAN),
  m_sd(NAN)
{
}

GaussianDistribution::~GaussianDistribution()
{
}

void GaussianDistribution::SetParam(const double mn, const double sd)
{
  m_mean = mn;
  m_sd   = sd;
  dist.reset(new boost::math::normal(m_mean, m_sd));
}


double GaussianDistribution::F(const double x)
{
  if (!dist) return NAN;

  return cdf(*dist,x);
}

double GaussianDistribution::F_inv(const double x)
{
  if (x > 1 || x < 0 || !dist) return NAN;

  return quantile(*dist,x);
}

