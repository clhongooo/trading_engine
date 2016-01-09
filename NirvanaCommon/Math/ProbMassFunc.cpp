#include <ProbMassFunc.h>

ProbMassFunc::ProbMassFunc()
{
}

ProbMassFunc::~ProbMassFunc()
{
}


void ProbMassFunc::Reset(const long yyyymmddhhmmss)
{
  if (m_pmf.find(yyyymmddhhmmss) != m_pmf.end()) m_pmf.erase(yyyymmddhhmmss);
  if (m_pdf.find(yyyymmddhhmmss) != m_pdf.end()) m_pdf.erase(yyyymmddhhmmss);
  m_b_IsDensityReady.clear();
  m_PMF_Mean.clear();
  m_PMF_Variance.clear();
  return;
}

double ProbMassFunc::GetPr(const long yyyymmddhhmmss, const double point)
{
  if (m_pmf.find(yyyymmddhhmmss) == m_pmf.end() || m_pmf[yyyymmddhhmmss].find(point) == m_pmf[yyyymmddhhmmss].end())
  {
    return NAN;
  }
  else
  {
    return m_pmf[yyyymmddhhmmss][point];
  }
}

double ProbMassFunc::GetPr(const long yyyymmddhhmmss, const double range_start, const double range_end)
{
  if (m_pmf.find(yyyymmddhhmmss) == m_pmf.end()) return -1;
  if (!IsDensityReady(yyyymmddhhmmss) && !Normalize(yyyymmddhhmmss)) return -1;

  map<double,double> & mPDF = m_pdf.find(yyyymmddhhmmss)->second;

  double dNumericalIntegral = 0;
  double dPeriodStart = 0;

  if (range_start <= mPDF.begin()->first)
  {
    dPeriodStart = mPDF.begin()->first;
  }
  else
  {
    map<double,double>::iterator itmore = mPDF.lower_bound(range_start);
    --itmore;
    if (itmore->first > range_start) dPeriodStart = itmore->first;
    else                             dPeriodStart = range_start;
  }

  map<double,double>::iterator it_m_pdf2 = mPDF.begin();

  double dProbDen = 0;
  for (;;)
  {
    if (it_m_pdf2 == mPDF.end()) break;

    if (dPeriodStart <= it_m_pdf2->first)
    {
      if (range_end > it_m_pdf2->first)
      {
        dNumericalIntegral += (it_m_pdf2->first - dPeriodStart) * dProbDen;
      }
      else
      {
        dNumericalIntegral += (range_end - dPeriodStart) * dProbDen;
        break;
      }
    }

    if (dPeriodStart < it_m_pdf2->first) dPeriodStart = it_m_pdf2->first;
    dProbDen = it_m_pdf2->second;
    ++it_m_pdf2;
  }

  return dNumericalIntegral;

}

void ProbMassFunc::SetPr(const long yyyymmddhhmmss, const double point, const double prob)
{
  map<long,map<double,double> >::iterator it = m_pmf.find(yyyymmddhhmmss);

  if (it == m_pmf.end())
  {
    m_pmf[yyyymmddhhmmss] = map<double,double>();
    it = m_pmf.find(yyyymmddhhmmss);
  }

  it->second[point] = prob;
  SetIsDensityReady(yyyymmddhhmmss,false);
  return;
}

void ProbMassFunc::SetIsDensityReady(const long yyyymmddhhmmss, const bool bRdy)
{
  m_b_IsDensityReady[yyyymmddhhmmss] = bRdy;
  if (!bRdy)
  {
    m_PMF_Mean[yyyymmddhhmmss] = -1;
    m_PMF_Variance[yyyymmddhhmmss] = -1;
  }
  return;
}

bool ProbMassFunc::IsDensityReady(const long yyyymmddhhmmss)
{
  map<long,bool>::iterator it = m_b_IsDensityReady.find(yyyymmddhhmmss);
  if (it == m_b_IsDensityReady.end()) return false;

  return it->second;
}

double ProbMassFunc::GetMean(const long yyyymmddhhmmss)
{
  if (!IsDensityReady(yyyymmddhhmmss)) return -1;

  map<long,double>::iterator it = m_PMF_Mean.find(yyyymmddhhmmss);
  if (it == m_PMF_Mean.end())
  {
    m_PMF_Mean[yyyymmddhhmmss] = -1;
    it = m_PMF_Mean.find(yyyymmddhhmmss);
  }
  if (it->second > 0) return it->second;

  //--------------------------------------------------
  // Calculate mean
  //--------------------------------------------------
  double dExpectedVal = 0;
  map<long,map<double,double> >::iterator it_m_pmf = m_pmf.find(yyyymmddhhmmss);
  for (map<double,double>::iterator it = it_m_pmf->second.begin(); it != it_m_pmf->second.end(); ++it)
  {
    dExpectedVal += it->second * it->first;
  }

  //--------------------------------------------------
  // Save for later use
  //--------------------------------------------------
  m_PMF_Mean[yyyymmddhhmmss] = dExpectedVal;
  return dExpectedVal;
}

double ProbMassFunc::GetVariance(const long yyyymmddhhmmss)
{
  if (!IsDensityReady(yyyymmddhhmmss)) return -1;

  map<long,double>::iterator it = m_PMF_Variance.find(yyyymmddhhmmss);
  if (it == m_PMF_Variance.end())
  {
    m_PMF_Variance[yyyymmddhhmmss] = -1;
    it = m_PMF_Variance.find(yyyymmddhhmmss);
  }
  if (it->second > 0) return it->second;

  //--------------------------------------------------
  // Calculate variance
  //--------------------------------------------------
  double dVariance = 0;
  map<long,map<double,double> >::iterator it_m_pmf = m_pmf.find(yyyymmddhhmmss);
  double dExpectedVal = GetMean(yyyymmddhhmmss);
  for (map<double,double>::iterator it = it_m_pmf->second.begin(); it != it_m_pmf->second.end(); ++it)
  {
    dVariance += it->second * pow(it->first - dExpectedVal, 2);
  }

  //--------------------------------------------------
  // Save for later use
  //--------------------------------------------------
  m_PMF_Variance[yyyymmddhhmmss] = dVariance;
  return dVariance;
}
double ProbMassFunc::GetStdev(const long yyyymmddhhmmss)
{
  if (!IsDensityReady(yyyymmddhhmmss)) return -1;
  
  double dVar = GetVariance(yyyymmddhhmmss);
  if (dVar > 0) return sqrt(dVar);
  else          return -1;
}

bool ProbMassFunc::DeriveDensity(const long yyyymmddhhmmss)
{
  map<long,map<double,double> >::iterator it_m_pmf = m_pmf.find(yyyymmddhhmmss);
  if (it_m_pmf == m_pmf.end()) return false;
  if (it_m_pmf->second.size() <= 1) return false;


  map<long,map<double,double> >::iterator it_m_pdf = m_pdf.find(yyyymmddhhmmss);
  if (it_m_pdf == m_pdf.end())
  {
    m_pdf[yyyymmddhhmmss] = map<double,double>();
    it_m_pdf = m_pdf.find(yyyymmddhhmmss);
  }
  it_m_pdf->second.clear();


  //--------------------------------------------------
  // For easy read
  //--------------------------------------------------
  map<double,double> & mPMF  = it_m_pmf->second;
  map<double,double> & mPDF  = it_m_pdf->second;
  //--------------------------------------------------

  //--------------------------------------------------
  // First pass
  //--------------------------------------------------
  vector<double> vPMFX;
  vector<double> vPMFProbMass;
  vPMFX.reserve(mPMF.size());
  vPMFProbMass.reserve(mPMF.size());

  for (map<double,double>::iterator it_m_pmf2 = mPMF.begin(); it_m_pmf2 != mPMF.end(); ++it_m_pmf2)
  {
    vPMFX.push_back(it_m_pmf2->first);
    vPMFProbMass.push_back(it_m_pmf2->second);
  }
  //--------------------------------------------------


  //--------------------------------------------------
  // Calculate densities
  //--------------------------------------------------
  mPDF[vPMFX.front()] = vPMFProbMass.front() / ((vPMFX[1]-vPMFX[0])/2);
  mPDF[vPMFX.back()] = 0;

  for (unsigned int i = 1; i < vPMFX.size()-1; ++i)
  {
    mPDF[(vPMFX[i-1]+vPMFX[i])/2] = vPMFProbMass[i] / ((vPMFX[i+1]-vPMFX[i-1])/2);
  }

  mPDF[(vPMFX.back()+vPMFX[vPMFX.size()-2])/2] = vPMFProbMass.back() / ((vPMFX.back()-vPMFX[vPMFX.size()-2])/2);
  //--------------------------------------------------


  for (map<double,double>::iterator it_m_pdf2 = mPDF.begin(); it_m_pdf2 != mPDF.end(); ++it_m_pdf2)
  {
  }

  //--------------------------------------------------
  SetIsDensityReady(yyyymmddhhmmss,true);
  return true;
}

bool ProbMassFunc::Normalize(const long yyyymmddhhmmss)
{
  map<long,map<double,double> >::iterator it = m_pmf.find(yyyymmddhhmmss);
  if (it == m_pmf.end()) return false;

  //--------------------------------------------------
  // First pass
  //--------------------------------------------------
  double dTotalProbMass = 0;
  for (map<double,double>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
  {
    dTotalProbMass += it2->second;
  }

  //--------------------------------------------------
  // Second pass
  //--------------------------------------------------
  if (abs(dTotalProbMass - 1) > 0.01)
  {
    for (map<double,double>::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++)
    {
      it2->second = it2->second / dTotalProbMass;
    }
  }

  return DeriveDensity(yyyymmddhhmmss);
}

bool ProbMassFunc::CheckValidity(const long yyyymmddhhmmss)
{
  map<long,map<double,double> >::iterator it_m_pmf = m_pmf.find(yyyymmddhhmmss);
  if (it_m_pmf == m_pmf.end()) return false;

  double dCumProb = 0;
  for (map<double,double>::iterator it_m_pmf2 = it_m_pmf->second.begin(); it_m_pmf2 != it_m_pmf->second.end(); ++it_m_pmf2)
  {
    dCumProb += it_m_pmf2->second;
  }

  if (abs(dCumProb - 1) < 0.01 && dCumProb <= 1) return true;
  else                                          return false;
}

void ProbMassFunc::MergeWith(const long yyyymmddhhmmss_this, const ProbMassFunc & pmfa, const long yyyymmddhhmmss_a, const double ratio_of_pmfa_thispmfa)
{
  ImportButNotNmlz(yyyymmddhhmmss_this, pmfa, yyyymmddhhmmss_a, ratio_of_pmfa_thispmfa);

  Normalize(yyyymmddhhmmss_this);
  return;
}

void ProbMassFunc::ImportButNotNmlz(const long yyyymmddhhmmss_this, const ProbMassFunc & pmfa, const long yyyymmddhhmmss_a, const double weight_of_pmfa)
{
  ImportButNotNmlz(yyyymmddhhmmss_this, pmfa, yyyymmddhhmmss_a, weight_of_pmfa, 1);
}

void ProbMassFunc::ImportButNotNmlz(const long yyyymmddhhmmss_this, const ProbMassFunc & pmfa, const long yyyymmddhhmmss_a, const double weight_of_pmfa, const double x_multiplier)
{
  map<long,map<double,double> >::iterator itthis = this->m_pmf.find(yyyymmddhhmmss_this);

  SetIsDensityReady(yyyymmddhhmmss_this,false);

  if (itthis == this->m_pmf.end())
  {
    this->m_pmf[yyyymmddhhmmss_this] = map<double,double>();
    itthis = this->m_pmf.find(yyyymmddhhmmss_this);
  }

  map<long,map<double,double> >::const_iterator ita = pmfa.m_pmf.find(yyyymmddhhmmss_a);
  if (ita == pmfa.m_pmf.end()) return;

  for(map<double,double>::const_iterator ita2 = ita->second.begin(); ita2 != ita->second.end(); ++ita2)
  {
    map<double,double>::iterator itthis2 = itthis->second.find(ita2->first);
    if (itthis2 == itthis->second.end())
    {
      itthis->second[ita2->first * x_multiplier] = ita2->second * weight_of_pmfa;
    }
    else
    {
      itthis->second[ita2->first * x_multiplier] += ita2->second * weight_of_pmfa;
    }
  }
  return;
}

void ProbMassFunc::ImportAndNmlz(const long yyyymmddhhmmss_this, const ProbMassFunc & pmfa, const long yyyymmddhhmmss_a, const double weight_of_pmfa)
{
  ImportAndNmlz(yyyymmddhhmmss_this, pmfa, yyyymmddhhmmss_a, weight_of_pmfa, 1);
}

void ProbMassFunc::ImportAndNmlz(const long yyyymmddhhmmss_this, const ProbMassFunc & pmfa, const long yyyymmddhhmmss_a, const double weight_of_pmfa, const double x_multiplier)
{
  ImportButNotNmlz(yyyymmddhhmmss_this, pmfa, yyyymmddhhmmss_a, weight_of_pmfa, x_multiplier);
  Normalize(yyyymmddhhmmss_this);
}

void ProbMassFunc::AdjustToStdevNmlz(const long yyyymmddhhmmss, const double target_sd)
{
  //--------------------------------------------------
  double dMean = GetMean(yyyymmddhhmmss);
  double dVar  = GetVariance(yyyymmddhhmmss);

  //--------------------------------------------------
  map<long,map<double,double> >::iterator it_m_pmf = m_pmf.find(yyyymmddhhmmss);
  if (it_m_pmf == m_pmf.end()) return;

  //--------------------------------------------------
  map<double,double> m_Tmp;
  for (map<double,double>::iterator it = it_m_pmf->second.begin(); it != it_m_pmf->second.end(); ++it)
  {
    m_Tmp[((it->first - dMean) / sqrt(dVar) * target_sd) + dMean] = it->second;
  }

  //--------------------------------------------------
  SetIsDensityReady(yyyymmddhhmmss,false);
  //--------------------------------------------------
  it_m_pmf->second.clear();
  it_m_pmf->second = m_Tmp;

  //--------------------------------------------------
  Normalize(yyyymmddhhmmss);
  return;
}

string ProbMassFunc::ToString(const long yyyymmddhhmmss)
{
  ostringstream oo;
  oo << "Is Density Ready: " << (IsDensityReady(yyyymmddhhmmss) ? "true" : "false") << "\t";

  oo << "PMF\t";
  map<long,map<double,double> >::iterator it_m_pmf = m_pmf.find(yyyymmddhhmmss);
  if (it_m_pmf != m_pmf.end())
  {
    for (map<double,double>::iterator it = it_m_pmf->second.begin(); it != it_m_pmf->second.end(); ++it)
    {
      oo << fixed << setprecision(10) << it->first << ":" << it->second << "\t";
    }
  }
  oo << "\t";

  oo << "PDF\t";
  map<long,map<double,double> >::iterator it_m_pdf = m_pdf.find(yyyymmddhhmmss);
  if (it_m_pdf != m_pdf.end())
  {
    for (map<double,double>::iterator it = it_m_pdf->second.begin(); it != it_m_pdf->second.end(); ++it)
    {
      oo << fixed << setprecision(10) << it->first << ":" << it->second << "\t";
    }
  }

  return oo.str();
}

