#ifndef OPTMASTER_HPP
#define OPTMASTER_HPP

#include "../corelib/ValidityChk.hpp"
#include <map>

using namespace std;

class OptMaster
{
	protected:

	public:
	OptMaster();
	bool LoadData(int iYMD, int iHM);
};

#endif
