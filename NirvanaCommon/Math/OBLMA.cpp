#include "OBLMA.h"

OBLMA::OBLMA()
{
  this->_iTargetSize = 0;
  this->_bCheckDataAsPrice = true;

  ema10_M.Reset(0.1,true);
}


void OBLMA::Reset()
{
		dqHistM.clear();
		ema10_M.Reset(0.1,true);
		dq_ema10_M.clear();
}

bool OBLMA::Ready() const
{
  if (!dq_1bl_wma_42.empty()) return true; else return false;
}

void OBLMA::AddChild(double dH, double dL, double dC)
{
  double dM = (dH+dL+dC) / 3;

  dqHistM.push_back(dM);
  ema10_M.Add(dM);
  dq_ema10_M.push_back(ema10_M.Value());

  //--------------------------------------------------
  // CalOneBarLagMA
  //--------------------------------------------------
  if (dqHistM.size() < 3) return;

  dq_1bl_wma_431.push_back(
      (+ 4 * dqHistM[dqHistM.size()-1]
       + 3 * dqHistM[dqHistM.size()-2]
       + 1 * dqHistM[dqHistM.size()-3]) / 8
      );

  if (dq_ema10_M.size() < 3) return;

  dq_1bl_taylor.push_back(
      dq_1bl_wma_431.back()
      + 1.1 * (dq_ema10_M.back() - dq_ema10_M[dq_ema10_M.size()-2])
      + 0.15 * (dq_ema10_M.back() - 2*dq_ema10_M[dq_ema10_M.size()-2] + dq_ema10_M[dq_ema10_M.size()-3] )
      );

  if (dq_1bl_taylor.size() < 2) return;

  dq_1bl_wma_42.push_back(
      (+ 4 * dq_1bl_taylor[dq_1bl_taylor.size()-1]
       + 2 * dq_1bl_taylor[dq_1bl_taylor.size()-2]) / 6
      );
}

double OBLMA::Value()
{
  if (!dq_1bl_wma_42.empty())
    return dq_1bl_wma_42.back();
  else
    return 0;
}
