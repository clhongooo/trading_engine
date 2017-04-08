#ifndef STRATEGIES_STRATEGYCONSTANTS_H_
#define STRATEGIES_STRATEGYCONSTANTS_H_

#include "PCH.h"
#include "Constants.h"

enum StrategyID {
  STY_NIR=1,
  STY_B1_HKF,
  STY_B2_US1,
  STY_B2_US2,
  STY_B2_HK,
  STY_B2_US3,
  STY_B3_US,
  STY_NIR1,
  STY_CMA,
  STY_S11A,
  STY_A1,
  STY_A6,
  STY_R1,
  STY_R3,
  STY_R7,
  STY_R8,
  STY_R9,
  STY_TEST,
  STY_LAST=99};

string GetStrategyName(const StrategyID);
void GetStrategyNameCStr(const StrategyID, char [HEALTHKEYLEN+1]);

#endif
