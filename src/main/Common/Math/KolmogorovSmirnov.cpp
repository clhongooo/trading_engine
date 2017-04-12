#include <KolmogorovSmirnov.h>

KolmogorovSmirnov::KolmogorovSmirnov()
{
}

KolmogorovSmirnov::~KolmogorovSmirnov()
{
}


double KolmogorovSmirnov::TwoSmpKSStat(const vector<double> & vd_data1, const vector<double> & vd_data2, const int numOfIntvl)
{
  if (vd_data1.empty() || vd_data2.empty() || numOfIntvl <= 1) return NAN;

  vector<double> vd_data1_sorted;
  vector<double> vd_data2_sorted;
  vector<double> vd_all_data;

  vd_data1_sorted.reserve(vd_data1.size());
  vd_data2_sorted.reserve(vd_data2.size());
  vd_data1_sorted.insert(vd_data1_sorted.begin(),vd_data1.begin(),vd_data1.end());
  vd_data2_sorted.insert(vd_data2_sorted.begin(),vd_data2.begin(),vd_data2.end());
  sort(vd_data1_sorted.begin(), vd_data1_sorted.end());
  sort(vd_data2_sorted.begin(), vd_data2_sorted.end());

  double dSmallestX = min(vd_data1_sorted.front(),vd_data2_sorted.front());
  double dLargestX  = max(vd_data1_sorted.back(),vd_data2_sorted.back());

  double dSupremum = 0;
  for (unsigned int i = 1; i < numOfIntvl; ++i)
  {
    double dX = dSmallestX + ((dLargestX-dSmallestX)*((double)i/(double)numOfIntvl));

    vector<double>::iterator itF_1 = std::lower_bound(vd_data1_sorted.begin(),vd_data1_sorted.end(),dX);
    double dF_1 = ((double)std::distance(vd_data1_sorted.begin(),itF_1))/(double)vd_data1_sorted.size();

    vector<double>::iterator itF_2 = std::lower_bound(vd_data2_sorted.begin(),vd_data2_sorted.end(),dX);
    double dF_2 = ((double)std::distance(vd_data2_sorted.begin(),itF_2))/(double)vd_data2_sorted.size();

    double F_F = abs(dF_1-dF_2);
    if (F_F > dSupremum) dSupremum = F_F;
  }

  return dSupremum;
}

