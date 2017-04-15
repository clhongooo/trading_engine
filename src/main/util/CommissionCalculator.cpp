#include <CommissionCalculator.h>

CommissionCalculator::CommissionCalculator() {
}

CommissionCalculator::~CommissionCalculator() {
}


double CommissionCalculator::CalcCommission(const BROKER bkr, const MARKET mkt, const PRODUCT pdt, const double price, const double qty)
{
  if (abs(qty) < EPSILON) return 0;

  if (bkr == IB         && mkt == USSTK && pdt == SPOT) return max(min(0.005*abs(qty),0.005*abs(price*qty)),1.0);
    
  return -1;
}

bool CommissionCalculator::PaysMoreThanMinCommission(const BROKER bkr, const MARKET mkt, const PRODUCT pdt, const double price, const double qty)
{
  if (bkr == IB         && mkt == USSTK && pdt == SPOT) { if (0.005*abs(qty) > (double)1    ) return true; else return false; }

  return false;
}

long CommissionCalculator::GetTrdgUnitConsidergMinComm(const BROKER bkr, const MARKET mkt, const PRODUCT pdt)
{
  if (bkr == IB         && mkt == USSTK && pdt == SPOT) { return ceil((double)  1/(double)0.005); }

  return -1;
}

