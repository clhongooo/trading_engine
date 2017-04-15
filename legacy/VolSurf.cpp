#include "VolSurf.h"

VolSurf::VolSurf() : m_Undly_Price(-1)
{}

VolSurf::~VolSurf()
{
  Reset();
}

void VolSurf::Reset()
{
  boost::unique_lock<boost::shared_mutex> lock(m_VolSurfMutex);

  {
    map<unsigned long, map<double,double> >::iterator it;
    for (it = m_VolSurfInStrikes.begin(); it != m_VolSurfInStrikes.end(); ++it)
    {
      it->second.clear();  
    }
    m_VolSurfInStrikes.clear();
  }

  // {
  //   map<unsigned long, vector<double> >::iterator it;
  //   for (it = m_FittedVolSmileInMoneyness.begin(); it != m_FittedVolSmileInMoneyness.end(); ++it)
  //   {
  //     it->second.clear();  
  //   }
  //   m_FittedVolSmileInMoneyness.clear();
  // }

  {
    while (!m_GSL_Interp_Accel.empty())
    {
      map<unsigned long, gsl_interp_accel* >::iterator it_interp = m_GSL_Interp_Accel.begin();
      gsl_interp_accel_free (it_interp->second);
      m_GSL_Interp_Accel.erase(it_interp);
    }
    while (!m_GSL_Spline.empty())
    {
      map<unsigned long, gsl_spline* >::iterator it_spline = m_GSL_Spline.begin();
      gsl_spline_free (it_spline->second);
      m_GSL_Spline.erase(it_spline);
    }

  }

  m_Undly_Price = -1;

  return;
}

void VolSurf::UpdateUndly(const unsigned long ymd,const double undly)
{
  boost::unique_lock<boost::shared_mutex> lock(m_VolSurfMutex);
  m_Undly_Price = undly;
  return;
}

double VolSurf::GetUndly(const unsigned long ymd)
{
  boost::shared_lock<boost::shared_mutex> lock(m_VolSurfMutex);
  return m_Undly_Price;
}

void VolSurf::UpdateUndlIVByStrike(const unsigned long ymd, const double strike, const double undly, const double iv)
{
  boost::unique_lock<boost::shared_mutex> lock(m_VolSurfMutex);

  map<unsigned long, map<double,double> >::iterator iter = m_VolSurfInStrikes.find(ymd);

  if (iter == m_VolSurfInStrikes.end())
    m_VolSurfInStrikes[ymd] = map<double,double>();

  //--------------------------------------------------
  // Update values
  //--------------------------------------------------
  m_VolSurfInStrikes[ymd][strike] = iv;
  m_Undly_Price = undly;

  return;
}
void VolSurf::RemoveIVByStrike(const unsigned long ymd, const double strike)
{
  boost::unique_lock<boost::shared_mutex> lock(m_VolSurfMutex);
  map<unsigned long, map<double,double> >::iterator iter = m_VolSurfInStrikes.find(ymd);

  if (iter != m_VolSurfInStrikes.end()) 
    m_VolSurfInStrikes[ymd].erase(strike);

  return;
}
//--------------------------------------------------
// Returns -1 if that IV doesn't exist
//--------------------------------------------------
double VolSurf::GetIVByStrike(const unsigned long ymd, const double strike)
{
  //--------------------------------------------------
  // New approach: either match exactly or do interpolation (linear)
  //--------------------------------------------------
  boost::shared_lock<boost::shared_mutex> lock(m_VolSurfMutex);

  map<unsigned long, map<double,double> >::iterator iter = m_VolSurfInStrikes.find(ymd);

  if (iter == m_VolSurfInStrikes.end()) return -1;

  map<double,double>::iterator iter2;
  iter2 = iter->second.find(strike);

  if (iter2 != iter->second.end())
  {
    //--------------------------------------------------
    // Exact match
    //--------------------------------------------------
    return iter2->second;
  }
  else
  {
    map<double,double>::iterator itmore = iter->second.lower_bound(strike);
    map<double,double>::iterator itless = iter->second.lower_bound(strike);
    itless--;

    //--------------------------------------------------
    // Outside upper and lower bounds
    //--------------------------------------------------
    if (itmore == iter->second.end())
    {
      // map<double,double>::iterator it1 = iter->second.end();
      // it1--;
      // map<double,double>::iterator it2 = it1;
      // it1--;
      //
      // return ((it2->second - it1->second) / (it2->first - it1->first)) * (strike - it2->first) + it2->second;

      map<double,double>::iterator it1 = iter->second.end();
      it1--;
      return it1->second;

    }
    else if (itmore == iter->second.begin())
    {
      // map<double,double>::iterator it1 = iter->second.begin();
      // map<double,double>::iterator it2 = iter->second.begin();
      // it2++;
      //
      // return ((it2->second - it1->second) / (it2->first - it1->first)) * (strike - it2->first) + it2->second;

      map<double,double>::iterator it1 = iter->second.begin();
      return it1->second;
    }
    else if (abs(itmore->first - strike) < strike * 0.002)
    {
      return itmore->second;
    }
    else if (abs(itless->first - strike) < strike * 0.002)
    {
      return itless->second;
    }
    else
    {
      //--------------------------------------------------
      // Within upper and lower bounds
      //--------------------------------------------------
      // Linear interpolation
      // map<double,double>::iterator itless = itmore--;
      // return (itmore->second * abs(strike - itless->first) + itless->second * abs(strike - itmore->first)) / abs(itmore->first - itless->first);

      //--------------------------------------------------
      // GSL Cubic Spline Interpolation
      //--------------------------------------------------
      boost::shared_lock<boost::shared_mutex> lock(m_VolSurfMutex);
      map<unsigned long, gsl_spline*>::iterator it_spline = m_GSL_Spline.find(ymd);
      if (it_spline == m_GSL_Spline.end()) return -1;
      map<unsigned long, gsl_interp_accel*>::iterator it_interp = m_GSL_Interp_Accel.find(ymd);
      if (it_interp == m_GSL_Interp_Accel.end()) return -1;

      // save original handler, install new handler
      gsl_error_handler_t * old_handler = gsl_set_error_handler_off();
      double dIV = gsl_spline_eval(it_spline->second, strike, it_interp->second);
      // restore original handler
      gsl_set_error_handler (old_handler); 
      if (STool::IsNaN(dIV)) return -1;
      else                   return dIV;
    }
  }
}

double VolSurf::GetIVByMoneyness(const unsigned long ymd, const double moneyness)
{

  double dEquivStrike = m_Undly_Price / moneyness;
  return GetIVByStrike(ymd,dEquivStrike);

}

void VolSurf::FitVolSmile(unsigned long ymd)
{
  boost::unique_lock<boost::shared_mutex> lock(m_VolSurfMutex);

  //--------------------------------------------------
  // Exit if no / not enough data
  //--------------------------------------------------
  map<unsigned long, map<double,double> >::iterator it_volsurfinstrikes = m_VolSurfInStrikes.find(ymd);
  if (it_volsurfinstrikes == m_VolSurfInStrikes.end()) return;
  long lNoOfDataPt = m_VolSurfInStrikes[ymd].size();
  if (lNoOfDataPt < 5) return;

  //--------------------------------------------------
  // Re-init
  //--------------------------------------------------
  map<unsigned long, gsl_interp_accel* >::iterator it_interp = m_GSL_Interp_Accel.find(ymd);
  if (it_interp != m_GSL_Interp_Accel.end())
  {
    gsl_interp_accel_free(it_interp->second);
    m_GSL_Interp_Accel.erase(it_interp);
  }

  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  m_GSL_Interp_Accel[ymd] = acc;
  it_interp = m_GSL_Interp_Accel.find(ymd);

  map<unsigned long, gsl_spline* >::iterator it_spline = m_GSL_Spline.find(ymd);
  if (it_spline != m_GSL_Spline.end())
  {
    gsl_spline_free(it_spline->second);
    m_GSL_Spline.erase(it_spline);
  }

  gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline,lNoOfDataPt);
  m_GSL_Spline[ymd] = spline;
  it_spline = m_GSL_Spline.find(ymd);

  //--------------------------------------------------
  // TODO: Better implementation?
  // Copy data to array for GSL
  //--------------------------------------------------
  double* x = (double*)malloc(sizeof(double)*lNoOfDataPt);
  double* y = (double*)malloc(sizeof(double)*lNoOfDataPt);
  int i = 0;

  for (map<double,double>::iterator it = it_volsurfinstrikes->second.begin(); it != it_volsurfinstrikes->second.end(); ++it, ++i)
  {
    x[i] = it->first;
    y[i] = it->second;
  }
  gsl_spline_init(it_spline->second, x, y, lNoOfDataPt);

  free(x);
  free(y);
}




//--------------------------------------------------
// Legacy code
//--------------------------------------------------

// void VolSurf::FitVolSmile(unsigned long ymd, const double dStrikeLowerBound, const double dStrikeUpperBound)
// {
//   boost::unique_lock<boost::shared_mutex> lock(m_VolSurfMutex);
//
//   //--------------------------------------------------
//   // Check if there are enough data points
//   //--------------------------------------------------
//   if (m_VolSurfInStrikes.find(ymd) == m_VolSurfInStrikes.end() || m_VolSurfInStrikes[ymd].size() < (unsigned)7)
//   {
//     m_FittedVolSmileInMoneyness.clear();
//     return;
//   }
//
//   //--------------------------------------------------
//   int iNoOfHigherStrikes = 0;
//   int iNoOfLowerStrikes  = 0;
//   LeastSqRegression lsrInMoneyness_deg2(2);
//   LeastSqRegression lsrInMoneyness_deg3(3);
//   LeastSqRegression lsrInMoneyness_deg4(4);
//   LeastSqRegression lsrInMoneynessPinLowerEnd(2);
//   LeastSqRegression lsrInMoneynessPinUpperEnd(2);
//
//   map<double,double>::iterator it;
//   for (it = m_VolSurfInStrikes[ymd].begin(); it != m_VolSurfInStrikes[ymd].end(); ++it)
//   {
//     lsrInMoneyness_deg2.AddPoint(m_Undly_Price / it->first, it->second);
//     lsrInMoneyness_deg3.AddPoint(m_Undly_Price / it->first, it->second);
//     lsrInMoneyness_deg4.AddPoint(m_Undly_Price / it->first, it->second);
//
//     if (it->first < m_Undly_Price)
//     {
//       lsrInMoneynessPinUpperEnd.AddPoint(m_Undly_Price / it->first, it->second);
//       iNoOfLowerStrikes++;
//     }
//     else
//     {
//       lsrInMoneynessPinLowerEnd.AddPoint(m_Undly_Price / it->first, it->second);
//       iNoOfHigherStrikes++;
//     }
//   }
//
//   --------------------------------------------------
//   Pin the end points
//   --------------------------------------------------
//   if (iNoOfHigherStrikes > 4)
//   {
//     vector<double> vCoefInMoneynessPinUpperEnd = lsrInMoneynessPinUpperEnd.Solve();
//     reverse(vCoefInMoneynessPinUpperEnd.begin(),vCoefInMoneynessPinUpperEnd.end());
//
//     double dMoneynessUpperBound = m_Undly_Price / dStrikeLowerBound; // meaning low strike
//     if (!vCoefInMoneynessPinUpperEnd.empty())
//     {
//       lsrInMoneyness_deg2.AddPoint(dMoneynessUpperBound, vCoefInMoneynessPinUpperEnd[2] * dMoneynessUpperBound * dMoneynessUpperBound + vCoefInMoneynessPinUpperEnd[1] * dMoneynessUpperBound + vCoefInMoneynessPinUpperEnd[0]);
//       lsrInMoneyness_deg3.AddPoint(dMoneynessUpperBound, vCoefInMoneynessPinUpperEnd[2] * dMoneynessUpperBound * dMoneynessUpperBound + vCoefInMoneynessPinUpperEnd[1] * dMoneynessUpperBound + vCoefInMoneynessPinUpperEnd[0]);
//       lsrInMoneyness_deg4.AddPoint(dMoneynessUpperBound, vCoefInMoneynessPinUpperEnd[2] * dMoneynessUpperBound * dMoneynessUpperBound + vCoefInMoneynessPinUpperEnd[1] * dMoneynessUpperBound + vCoefInMoneynessPinUpperEnd[0]);
//     }
//   }
//
//   if (iNoOfLowerStrikes > 4)
//   {
//     vector<double> vCoefInMoneynessPinLowerEnd = lsrInMoneynessPinLowerEnd.Solve();
//     reverse(vCoefInMoneynessPinLowerEnd.begin(),vCoefInMoneynessPinLowerEnd.end());
//
//     double dMoneynessLowerBound = m_Undly_Price / dStrikeUpperBound; // meaning high strike
//     if (!vCoefInMoneynessPinLowerEnd.empty())
//     {
//       lsrInMoneyness_deg2.AddPoint(dMoneynessLowerBound, vCoefInMoneynessPinLowerEnd[2] * dMoneynessLowerBound * dMoneynessLowerBound + vCoefInMoneynessPinLowerEnd[1] * dMoneynessLowerBound + vCoefInMoneynessPinLowerEnd[0]);
//       lsrInMoneyness_deg3.AddPoint(dMoneynessLowerBound, vCoefInMoneynessPinLowerEnd[2] * dMoneynessLowerBound * dMoneynessLowerBound + vCoefInMoneynessPinLowerEnd[1] * dMoneynessLowerBound + vCoefInMoneynessPinLowerEnd[0]);
//       lsrInMoneyness_deg4.AddPoint(dMoneynessLowerBound, vCoefInMoneynessPinLowerEnd[2] * dMoneynessLowerBound * dMoneynessLowerBound + vCoefInMoneynessPinLowerEnd[1] * dMoneynessLowerBound + vCoefInMoneynessPinLowerEnd[0]);
//     }
//   }
//
//   //--------------------------------------------------
//   vector<double> vCoefInMoneyness_deg2  = lsrInMoneyness_deg2.Solve();
//   vector<double> vCoefInMoneyness_deg3  = lsrInMoneyness_deg3.Solve();
//   vector<double> vCoefInMoneyness_deg4  = lsrInMoneyness_deg4.Solve();
//   if (vCoefInMoneyness_deg2.empty() ||
//       vCoefInMoneyness_deg3.empty() ||
//       vCoefInMoneyness_deg4.empty())
//   {
//     m_FittedVolSmileInMoneyness.clear();
//     return;
//   }
//
//   reverse(vCoefInMoneyness_deg2.begin(),vCoefInMoneyness_deg2.end());
//   reverse(vCoefInMoneyness_deg3.begin(),vCoefInMoneyness_deg3.end());
//   reverse(vCoefInMoneyness_deg4.begin(),vCoefInMoneyness_deg4.end());
//
//   //--------------------------------------------------
//   // Compare results and choose the best one
//   //--------------------------------------------------
//   double dSumOfErrors_deg2 = 0;
//   double dSumOfErrors_deg3 = 0;
//   double dSumOfErrors_deg4 = 0;
//
//   for (it = m_VolSurfInStrikes[ymd].begin(); it != m_VolSurfInStrikes[ymd].end(); ++it)
//   {
//     double dMoneyness = (m_Undly_Price / it->first);
//
//     dSumOfErrors_deg2 +=
//       pow(( vCoefInMoneyness_deg2[2] * pow(dMoneyness,2) +
//             vCoefInMoneyness_deg2[1] * pow(dMoneyness,1) +
//             vCoefInMoneyness_deg2[0]) - it->second, 2);
//
//     dSumOfErrors_deg3 +=
//       pow(( vCoefInMoneyness_deg3[3] * pow(dMoneyness,3) +
//             vCoefInMoneyness_deg3[2] * pow(dMoneyness,2) +
//             vCoefInMoneyness_deg3[1] * pow(dMoneyness,1) +
//             vCoefInMoneyness_deg3[0]) - it->second, 2);
//
//     dSumOfErrors_deg4 +=
//       pow(( vCoefInMoneyness_deg4[4] * pow(dMoneyness,4) +
//             vCoefInMoneyness_deg4[3] * pow(dMoneyness,3) +
//             vCoefInMoneyness_deg4[2] * pow(dMoneyness,2) +
//             vCoefInMoneyness_deg4[1] * pow(dMoneyness,1) +
//             vCoefInMoneyness_deg4[0]) - it->second, 2);
//
//   }
//
//   double dSmallestSumOfErrors = dSumOfErrors_deg2;
//   if (dSumOfErrors_deg3 < dSmallestSumOfErrors) dSmallestSumOfErrors = dSumOfErrors_deg3;
//   if (dSumOfErrors_deg4 < dSmallestSumOfErrors) dSmallestSumOfErrors = dSumOfErrors_deg4;
//
//   m_FittedVolSmileInMoneyness.clear();
//   if      (dSumOfErrors_deg2 == dSmallestSumOfErrors) m_FittedVolSmileInMoneyness[ymd] = vCoefInMoneyness_deg2;
//   else if (dSumOfErrors_deg3 == dSmallestSumOfErrors) m_FittedVolSmileInMoneyness[ymd] = vCoefInMoneyness_deg3;
//   else if (dSumOfErrors_deg4 == dSmallestSumOfErrors) m_FittedVolSmileInMoneyness[ymd] = vCoefInMoneyness_deg4;
//
//   return;
// }

// vector<double> VolSurf::GetFittedVolSmileInMoneyness(unsigned long ymd)
// {
//   boost::shared_lock<boost::shared_mutex> lock(m_VolSurfMutex);
//   if (m_FittedVolSmileInMoneyness.find(ymd) == m_FittedVolSmileInMoneyness.end()) return vector<double>();
//
//   return m_FittedVolSmileInMoneyness[ymd];
// }

// short VolSurf::GetOrderOfFittedPolynomial(const unsigned long ymd)
// {
//   map<unsigned long, vector<double> >::iterator it = m_FittedVolSmileInMoneyness.find(ymd);
//   if (it == m_FittedVolSmileInMoneyness.end()) return -1;
//
//   if (it->second.empty()) return -1;
//
//   return it->second.size()-1;
// }

// double VolSurf::GetIVByMoneyness(const unsigned long ymd, const double moneyness)
// {
//   //--------------------------------------------------
//   // Old approach of fitting polynomial
//   //--------------------------------------------------
//   boost::shared_lock<boost::shared_mutex> lock(m_VolSurfMutex);
//   if (m_FittedVolSmileInMoneyness.empty()) return -1;
//
//   map<unsigned long,vector<double> >::iterator it = m_FittedVolSmileInMoneyness.find(ymd);
//   if (it == m_FittedVolSmileInMoneyness.end()) return -1;
//
//   double dIV = 0;
//   for (unsigned int i = 0; i < (it->second).size(); ++i)
//   {
//     dIV += (it->second)[i] * pow(moneyness,(it->second).size()-1-i);
//   }
//   return dIV;
// }
