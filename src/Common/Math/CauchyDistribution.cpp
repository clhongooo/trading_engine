#include <CauchyDistribution.h>

CauchyDistribution::CauchyDistribution() :
  m_location(NAN),
  m_scale(NAN)
{
}

CauchyDistribution::~CauchyDistribution()
{
}


void CauchyDistribution::SetParam(const double location, const double scale)
{
  m_location = location;
  m_scale    = scale;
}


double CauchyDistribution::F(const double x)
{
  if (std::isnan(m_location) || std::isnan(m_scale)) return NAN;

  return ((1/M_PI) * atan((x-m_location)/m_scale)) + 0.5;
}

double CauchyDistribution::F_inv(const double x)
{
  if (x > 1 || x < 0 || std::isnan(m_location) || std::isnan(m_scale)) return NAN;

  return (tan(x-0.5) * M_PI * m_scale) + m_location;

}

