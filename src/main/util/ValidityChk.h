#ifndef VALIDITYCHK_HPP
#define VALIDITYCHK_HPP

#include "Constants.h"
using namespace std;

class ValidityChk
{
	public:
		//General Number
		static bool GeneralNumber(double d) {
			if (d != d) return false; else return true;
		}
		static bool GeneralNumber(float f) {
			if (f != f) return false; else return true;
		}
		static bool GeneralNumber(int i) {
			return true; //always valid
		}

		//Price
		static bool Price(double dP) {
			if (dP != dP || dP < EPSILON) return false;
			else return true;
		}
		static bool Price(float fP) {
			if (fP != fP || fP < EPSILON) return false;
			else return true;
		}
		static bool Price(int iP) {
			if (iP < 0) return false;
			else return true;
		}

		//DateTime
		static bool Year(int iY) {
			if (iY < 1900 || iY > 2100) return false; else return true;
		}
		static bool Month(int iM) {
			if (iM < 1 || iM > 12) return false; else return true;
		}
		static bool Day(int iD) {
			if (iD < 0 || iD > 31) return false; else return true;
		}
		static bool Hour(int iH) {
			if (iH < 0 || iH > 23) return false; else return true;
		}
		static bool Minute(int iM) {
			if (iM < 0 || iM > 59) return false; else return true;
		}
		static bool Second(int iS) {
			if (iS < 0 || iS >= 60) return false; else return true; //can be 59.999
		}

		//HKFE Derivatives
		static bool HKFEDeriType(char c) {
			if (c != 'F' &&
					c != 'C' &&
					c != 'P')
				return false; else return true;
		}
		static bool HKFEOptStrike(int iK) {
			if (iK <= 0 || iK > 40000) return false;
			if (iK % 100 != 0) return false;
			return true;
		}

};

#endif
