#ifndef PPARAM_HPP
#define PPARAM_HPP

using namespace std;

class PParam
{
	public:
		// HSI Intraday
		static const int A01a_SIZE;
		static const int A01b_SIZE;
		static const int A06a_SIZE;
		static const int A06b_SIZE;
		static const int R01_SIZE;
		static const int R08_SIZE;

		// HSI Day
		static const int B3_SIZE;
		static const int B5_SIZE;
		static const int B10_SIZE;
		static const int FSMC_SIZE;
};

// HSI Intraday
const int PParam::A01a_SIZE = 0;
const int PParam::A01b_SIZE = 0;
const int PParam::A06a_SIZE = 0;
const int PParam::A06b_SIZE = 0;
const int PParam::R01_SIZE = 1;
const int PParam::R08_SIZE = 0;

// HSI Day
const int PParam::B3_SIZE = 0;
const int PParam::B5_SIZE = 0;
const int PParam::B10_SIZE = 0;
const int PParam::FSMC_SIZE = 1000000;
#endif
