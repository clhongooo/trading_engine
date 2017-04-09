#ifndef MATH_COUNTINGFUNCTION_H_
#define MATH_COUNTINGFUNCTION_H_

#include "PCH.h"
#include "Constants.h"

using namespace std;

class CountingFunction {
public:
	CountingFunction();
	virtual ~CountingFunction();
  void Add(const bool);
  size_t Size() const;
  long CountInclusive(const long,const long) const;
  long Count(const long,const long) const;
  long CountLastNElements(const long) const;
  long HowManyElemFromTailToAccum(const long) const;
  long HowManyElemFromTailToAccumOpp(const long) const;
  void Reset();
private:
  deque<long> m_NumList;
};

#endif /* MATH_COUNTINGFUNCTION_H_ */
