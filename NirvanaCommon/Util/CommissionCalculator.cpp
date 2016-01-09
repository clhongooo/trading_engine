#include <CommissionCalculator.h>

CommissionCalculator::CommissionCalculator() {
}

CommissionCalculator::~CommissionCalculator() {
}


double CommissionCalculator::CalcCommission(const BROKER bkr, const MARKET mkt, const PRODUCT pdt, const double price, const double qty)
{
  if (bkr == IB         && mkt == USSTK && pdt == SPOT) return max((double)1    ,min(0.005*qty,0.005*price*qty));

  return -1;
}

bool CommissionCalculator::PaysMoreThanMinCommission(const BROKER bkr, const MARKET mkt, const PRODUCT pdt, const double price, const double qty)
{
  if (bkr == IB         && mkt == USSTK && pdt == SPOT) { if (0.005*qty > (double)1    ) return true; else return false; }

  return false;
}

long CommissionCalculator::GetTrdgUnitConsidergMinComm(const BROKER bkr, const MARKET mkt, const PRODUCT pdt)
{
  if (bkr == IB         && mkt == USSTK && pdt == SPOT) { return ceil((double)  1/(double)0.005); }

  return -1;
}

