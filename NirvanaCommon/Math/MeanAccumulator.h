#ifndef MATH_MEANACCUMULATOR_H_
#define MATH_MEANACCUMULATOR_H_

#include "PCH.h"
#include "Constants.h"

class MeanAccumulator {
  public:
    MeanAccumulator();
    virtual ~MeanAccumulator();
    void Add(const double);
    void Reset();
    bool Ready() const;
    double GetMean() const;
  private:
    double m_Total;
    long m_Count;
};

#endif /* MATH_MEANACCUMULATOR_H_ */
