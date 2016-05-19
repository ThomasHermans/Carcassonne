#ifndef CARCASSONNE_UTILS_RANDOM_20160512
#define CARCASSONNE_UTILS_RANDOM_20160512

#include <cstdlib>
#include <vector>

namespace Utils
{
	std::size_t Random( std::size_t inSize );

	template< typename T >
	void RandomShuffle( std::vector< T > & ioVector )
	{
		std::random_shuffle( ioVector.begin(), ioVector.end(), Random );
	}
}

#endif