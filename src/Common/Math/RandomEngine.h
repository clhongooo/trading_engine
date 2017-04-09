#ifndef RANDOMENGINE_HPP
#define RANDOMENGINE_HPP

#include "PCH.h"
#include "Constants.h"
#include <ctime>

using namespace std;

class RandomEngine
{
	private:
		RandomEngine();
		//static RandomEngine* _instance;
		static bool _bInited;

	public:
		//~RandomEngine();
		int NextInt(int,int);
		double Rand01();
		bool CoinFlip();
		void RandomEngineInit();

		//real singleton that results in mem leakage
		/*
		static RandomEngine* Instance()
		{
			if (_instance == 0)
				_instance = new RandomEngine;
			return _instance;
		}
		*/

		//not real singleton
		static RandomEngine& Instance()
		{
			static RandomEngine _instance;
			return _instance;
		}

};
#endif
