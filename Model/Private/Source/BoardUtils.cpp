#include "Model/BoardUtils.h"

#include "Model/Board.h"
#include "Model/PlacedPiece.h"
#include "Model/PlacedProject.h"

#include "Utils/Location.h"

std::vector< Model::PlacedProject >
Model::getCompleteProject
(
	Board const & inBoard,
	Utils::Location const & inLocation,
	Area::Area inArea
)
{
	if ( inBoard.isRoad( inLocation, inArea ) )
	{
		return inBoard.getCompleteRoad( inLocation, inArea );
	}
	else if ( inBoard.isCity( inLocation, inArea ) )
	{
		return inBoard.getCompleteCity( inLocation, inArea );
	}
	else if ( inBoard.isField( inLocation, inArea ) )
	{
		return inBoard.getCompleteField( inLocation, inArea );
	}
	else
	{
		return { { inLocation, inArea } };
	}
}

std::vector< std::pair< Utils::Location, Model::PlacedPiece > >
Model::getPieces( Board const & inBoard, Color::Color inColor )
{
	std::vector< std::pair< Utils::Location, Model::PlacedPiece > > allPieces;
	for ( int row = inBoard.getTopRow(); row <= inBoard.getBottomRow(); ++row )
	{
		for ( int col = inBoard.getLeftCol(); col <= inBoard.getRightCol(); ++col )
		{
			if ( inBoard.isTile( row, col ) )
			{
				for ( Area::Area area : getAllAreas() )
				{
					Utils::Location const location( row, col );
					std::vector< PlacedPiece > const pieces = inBoard.getPieces( { location, area } );
					for ( PlacedPiece const & piece : pieces )
					{
						if ( piece.getColor() == inColor )
						{
							allPieces.emplace_back( location, piece );
						}
					}
				}
			}
		}
	}
	return allPieces;
}