#include "Random.h"
#include <random>

using namespace std;

int clsRANDOM::GetRandom( const int iMin, const int iSize )
{
	mt19937 mt{ std::random_device{}() };

	int iMax = iSize - 1;
	if( iMax < iMin ){
		iMax = iMin;
	}

	uniform_int_distribution<int> dist( iMin, iMax );

	return dist( mt );
}

