#include "RandomEngine.hpp"

//RandomEngine* RandomEngine::_instance;
bool RandomEngine::_bInited = false;

//--------------------------------------------

RandomEngine::RandomEngine()
{
	RandomEngineInit();
}

//RandomEngine::~RandomEngine()
//{
// delete _instance;
//}


void RandomEngine::RandomEngineInit()
{
	if (!_bInited)
	{
		srand ( time(NULL) );
		_bInited = true;
	}
}


//iL inclusive
//iU exclusive
int RandomEngine::NextInt(int iL, int iU)
{
	RandomEngineInit();
	return ( rand() % (iU-iL-1) ) + iL;
}

bool RandomEngine::CoinFlip()
{
	return ( NextInt(1,10001) > 5000);
}

//generate a random rational number between 0 and 1
double RandomEngine::Rand01()
{
	return (double) rand() / RAND_MAX;
}
