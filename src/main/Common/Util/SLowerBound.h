#ifndef SLOWERBOUND_HPP
#define SLOWERBOUND_HPP

#include "PCH.h"
#include "Constants.h"
using namespace std;

template<class Iter, class T>
class SLowerBound
{
	public:
		//returns true if exact match is found
		//returns false if exact match is not found
		//'it' points to the entry
		static bool ExactMatch(Iter begin, Iter end, T val, Iter& it)
		{
			it = std::lower_bound(begin, end, val);

			if (it != end && *it == val) return true;
			else return false;
		}
		//returns true if val is not larger than the whole set.
		//returns false if val is larger than the whole set.
		//'it' points to the entry
		static bool JustLarger(Iter begin, Iter end, T val, Iter& it)
		{
			it = std::lower_bound(begin, end, val);

			if (it != end) return true;
			else return false;
		}
};

#endif
