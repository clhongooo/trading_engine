#ifndef MATH_NUMBERPOSITION_H_
#define MATH_NUMBERPOSITION_H_

#include "PCH.h"

class NumberPosition {
  public:
    NumberPosition();
    virtual ~NumberPosition();
    void Reset();
    bool IsPriceApart(const double,const double) const;
    void AddNum(const double);
    void RemoveNum(const double);
    size_t Size();

  private:
    set<double> m_setNum;
};

#endif /* MATH_NUMBERPOSITION_H_ */
