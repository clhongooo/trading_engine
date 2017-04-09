#ifndef OPARAM_H
#define OPARAM_H

using namespace std;

class OParam
{
	public:
		// Param Matrix [LowerBound, UpperBound, Step]
		static const int VecLenA01;
		static const double MatrixA01[][3];
		static const double OptimalVecA01a[];
		static const double OptimalVecA01b[];
		static const int VecLenA06;
		static const double MatrixA06[][3];
		static const double OptimalVecA06a[];
		static const double OptimalVecA06b[];
		static const int VecLenR01;
		static const double MatrixR01[][3];
		static const double OptimalVecR01[];
		static const int VecLenR08;
		static const double MatrixR08[][3];
		static const double OptimalVecR08[];
};

#endif
