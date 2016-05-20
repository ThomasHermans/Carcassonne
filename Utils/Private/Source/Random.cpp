#include "Utils/Random.h"

#include <random>

namespace
{
	std::mt19937
	GetMersenneTwister()
	{
		std::random_device randomDevice;
		return std::mt19937( randomDevice() );
	}
}

std::size_t
Utils::Random( std::size_t inSize )
{
	static std::mt19937 sMersenneTwister = GetMersenneTwister();
	if ( inSize == 0 )
	{
		return 0;
	}
	std::uniform_int_distribution< std::size_t > distribution( 0, inSize - 1 );
	return distribution( sMersenneTwister );
}

double
Utils::Random( double inMaximum )
{
	static std::mt19937 sMersenneTwister = GetMersenneTwister();
	std::uniform_real_distribution< double > distribution( 0, inMaximum );
	return distribution( sMersenneTwister );
}