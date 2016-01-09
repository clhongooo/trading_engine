#include "OParam.h"

const int OParam::VecLenA01 = 6;
const double OParam::OptimalVecA01a[6] = {18,75,60,76,0,1};
const double OParam::OptimalVecA01b[6] = {10,21,10,20,0,-1};
const double OParam::MatrixA01[6][3] =
{
	{3,100,15}, // Entry1
	{5,200,15}, // Entry2
	{3,100,15}, // Exit1
	{5,200,15}, // Exit2
	{0,0,1}, // Permitted Trade Dir
	{1,1,2} // Reverse
};

const int OParam::VecLenA06 = 8;
const double OParam::OptimalVecA06a[8] = {10,60,0.9,1.3,0.006,0.1,1,-1};
const double OParam::OptimalVecA06b[8] = {5,185,1.7,0.2,0.007,0.1,-1,1};
const double OParam::MatrixA06[8][3] =
{
	{10,20,5}, // Entry
	{60,80,10}, // Exit
	{0.5,1,0.2}, // Take profit fraction (of Entry Channel height)
	{0.7,1.5,0.2}, // Stop loss fraction (of Entry Channel height)
	{0.004,0.01,0.002}, // Max channel height
	{0.1,0.1,0.05}, // ATR alpha
	{1,1,2}, // Permitted Trade Dir
	{-1,-1,1}, // Reverse
};

const int OParam::VecLenR01 = 6;
const double OParam::OptimalVecR01[6] = {205,0.002,0.007,1,1,0};
//const double OParam::OptimalVecR01[6] = {180,0.0035,0.008,1,1,-1};
//const double OParam::OptimalVecR01[6] = {190,0.0005,0.009,-1,1,0};
//const double OParam::OptimalVecR01[6] = {205,0.002,0.008,1,3,0};
const double OParam::MatrixR01[6][3] =
{
	{15,250,10}, // Minutes before close
	{0.002,0.01,0.002}, // Threshold
	{0.002,0.01,0.002}, // Stop loss from POE
	{1,1,2}, // Reverse
	{1,5,1}, // Max no trial
	{0,0,1} // Permitted trade dir
};

const int OParam::VecLenR08 = 6;
const double OParam::OptimalVecR08[6] = {22,-1,-1000,5,0.9,0.2};
const double OParam::MatrixR08[6][3] =
{
	{22,28,6}, // Regression Lag
	{-3,-1,1}, // Slope Max
	{-1000,-1000,1}, // Slope Min
	{4,10,1}, // N Bar High
	{0.8,1.2,0.1}, // Take Profit
	{0.2,4.0,0.3} // Stop Loss
};


