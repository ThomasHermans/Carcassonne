#include "Utils/PrintUtils.h"

#include "Model/Board.h"

#include <sstream>

std::string
Utils::printBoard( Model::Board const & inBoard )
{
	std::stringstream stream;
	stream << inBoard.getNrOfTiles() << " tiles on the board." << std::endl;
	for ( int row = inBoard.getTopRow(); row <= inBoard.getBottomRow(); ++row )
	{
		for ( int col = inBoard.getLeftCol(); col <= inBoard.getRightCol(); ++col )
		{
			if ( inBoard.isTile( row, col ) )
			{
				boost::optional< Model::TileOnBoard > const tile = inBoard.getTile( row, col );
				stream << tile->getID() << tile->getRotation();
			}
			else
			{
				stream << std::string( 2, ' ' );
			}
			stream << ' ';
		}
		stream << std::endl;
	}
	return stream.str();
}