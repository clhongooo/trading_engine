#ifndef BAR_HPP
#define BAR_HPP

#include "STool.hpp"

using namespace std;

// Lightweight OHLCV
// Won't have any performance gain passing by reference
struct Bar
{
	double Open;
	double High;
	double Low;
	double Close;
	int Volume;

	Bar(const double o, const double h, const double l, const double c, const int v)
		: Open(o), High(h), Low(l), Close(c), Volume(v) {}
	Bar(const double o, const double h, const double l, const double c)
		: Open(o), High(h), Low(l), Close(c), Volume(0) {}
};

// Lightweight DTOHLCV
// Won't have any performance gain passing by reference
// Not inheriting from Bar to make interface simple
struct FBar
{
	int YMDi;
	int HMSi;
	double Open;
	double High;
	double Low;
	double Close;
	int Volume;

	FBar(const int d, const int t, const double o, const double h, const double l, const double c, const int v)
		: YMDi(d), HMSi(t), Open(o), High(h), Low(l), Close(c), Volume(v) {}

};

#endif
