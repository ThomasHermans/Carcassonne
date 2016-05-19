#include "Controller/RobotPlayer.h"

#include "Model/Board.h"
#include "Model/Tile.h"
#include "Model/TileOnBoard.h"

#include "Utils/Location.h"
#include "Utils/Random.h"

#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace
{
	int const kPlaceTileInterval = 1000;
	int const kPlacePieceInterval = 1000;

	std::size_t const kDefaultValue = 1;

	std::size_t const kValueStartCity = 10;
	std::size_t const kValuePerCityTile = 50;
	std::size_t const kValuePerSharedCityTile = 20;
	std::size_t const kValueContinueOwnCity = 2500;
	std::size_t const kValueContinueOtherCity = 0;

	std::size_t const kValueStartCloister = 5;
	std::size_t const kValuePerCloisterTile = 15;

	std::array< Model::Area::Area, 13 > const kAreas = 
	{
		{
			Model::Area::kTopLeft,
			Model::Area::kTop,
			Model::Area::kTopRight,
			Model::Area::kRightTop,
			Model::Area::kRight,
			Model::Area::kRightBottom,
			Model::Area::kBottomRight,
			Model::Area::kBottom,
			Model::Area::kBottomLeft,
			Model::Area::kLeftBottom,
			Model::Area::kLeft,
			Model::Area::kLeftTop,
			Model::Area::kCentral
		}
	};

	struct PossiblePlacement
	{
		Utils::Location location;
		Model::Rotation rotation;
		std::size_t value;
		boost::optional< Model::PlacedPiece > piece;

		PossiblePlacement( Utils::Location const & inLocation, Model::Rotation inRotation )
		:
			location( inLocation ),
			rotation( inRotation ),
			value( kDefaultValue ),
			piece( boost::none )
		{
		}
	};

	std::size_t
	getTotalValue( std::vector< PossiblePlacement > const & inPossibilities )
	{
		std::size_t totalValue = 0;
		for ( auto const & possibility : inPossibilities )
		{
			assert( possibility.value >= 0 );
			totalValue += possibility.value;
		}
		assert( totalValue > 0 );
		return totalValue;
	}

	PossiblePlacement
	getRandomPossibility( std::vector< PossiblePlacement > const & inPossibilities )
	{
		std::size_t const totalValue = getTotalValue( inPossibilities );
		std::size_t const chosenValue = Utils::Random( totalValue );
		std::size_t cumulativeValue = 0;
		for ( auto const & possibility : inPossibilities )
		{
			cumulativeValue += possibility.value;
			if ( chosenValue < cumulativeValue )
			{
				return possibility;
			}
		}
		return PossiblePlacement( Utils::Location( 0, 0 ), Model::Rotation::kCw0 );
	}

	PossiblePlacement
	getBestPossibility( std::vector< PossiblePlacement > const & inPossibilities )
	{
		std::size_t bestValue = 0;
		std::vector< PossiblePlacement > bestPossibilities;
		for ( auto const & possibility : inPossibilities )
		{
			if ( possibility.value > bestValue )
			{
				bestPossibilities.clear();
				bestValue = possibility.value;
			}
			if ( possibility.value == bestValue )
			{
				bestPossibilities.emplace_back( possibility );
			}
		}
		return bestPossibilities[ Utils::Random( bestPossibilities.size() ) ];
	}

	std::set< Model::Color::Color >
	getWinningColors( std::vector< Model::PlacedPiece > const & inPieces )
	{
		typedef std::map< Model::Color::Color, std::size_t > Counts;
		Counts counts;
		for ( Model::PlacedPiece const & piece : inPieces )
		{
			Model::Color::Color const color = piece.getPiece().getColor();
			Counts::iterator const it = counts.find( color );
			if ( it == counts.end() )
			{
				counts.insert( std::make_pair( color, piece.getPiece().getWeight() ) );
			}
			else
			{
				it->second += piece.getPiece().getWeight();
			}
		}
		std::set< Model::Color::Color > winningColors;
		std::size_t winningAmount = 0;
		for ( Counts::const_iterator it = counts.begin(); it != counts.end(); ++it )
		{
			if ( it->second > winningAmount )
			{
				winningColors.clear();
				winningAmount = it->second;
			}
			if ( it->second == winningAmount )
			{
				winningColors.insert( it->first );
			}
		}
		return winningColors;
	}

	std::set< Model::Color::Color >
	getCityOccupants( Model::Board const & inBoard, Utils::Location const & inLocation, Model::Area::Area inArea )
	{
		std::vector< Model::PlacedPiece > allPieces;
		for ( auto const & cityPiece : inBoard.getCompleteCity( Model::PlacedCity( inLocation.row, inLocation.col, inArea ) ) )
		{
			std::vector< Model::PlacedPiece > const pieces = inBoard.getPieces( cityPiece );
			allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
		}
		return getWinningColors( allPieces );
	}

	std::size_t
	getCitySize( Model::Board const & inBoard, Utils::Location const & inLocation, Model::Area::Area inArea )
	{
		std::set< Utils::Location > usedTiles;
		for ( auto const & cityPiece : inBoard.getCompleteCity( Model::PlacedCity( inLocation.row, inLocation.col, inArea ) ) )
		{
			usedTiles.insert( Utils::Location( cityPiece.row, cityPiece.col ) );
		}
		return usedTiles.size();
	}

	std::size_t
	getAmountOfOwnCloistersNearby( Model::Board const & inBoard, Utils::Location const & inLocation, Model::Color::Color inColor )
	{
		std::size_t amountOfCloisters = 0;
		for ( int row = inLocation.row - 1; row <= inLocation.row + 1; ++row )
		{
			for ( int col = inLocation.col - 1; col <= inLocation.col + 1; ++col )
			{
				boost::optional< Model::TileOnBoard > tile = inBoard.getTile( row, col );
				if ( tile )
				{
					for ( Model::Area::Area area : kAreas )
					{
						if ( tile->isCloister( area ) && tile->hasPiece( area ) )
						{
							std::set< Model::Color::Color > winningColors = getWinningColors( tile->getPieces( area ) );
							if ( winningColors.find( inColor ) != winningColors.end() )
							{
								++amountOfCloisters;
							}
						}
					}
				}
			}
		}
		return amountOfCloisters;
	}

	std::size_t
	getValueForTile
	(
		Model::Board inBoardCopy,
		Model::Tile const & inTile,
		PossiblePlacement const & inPlacement,
		Model::Color::Color inColor
	)
	{
		std::size_t value = kDefaultValue;
		inBoardCopy.placeValidTile( Model::TileOnBoard( inTile, inPlacement.rotation ), inPlacement.location );
		std::size_t const amountOfOwnCloistersNearby = getAmountOfOwnCloistersNearby( inBoardCopy, inPlacement.location, inColor );
		for ( Model::Area::Area area : kAreas )
		{
			if ( inBoardCopy.isCity( inPlacement.location, area ) )
			{
				if ( inBoardCopy.isOccupiedCity( inPlacement.location, area ) )
				{
					// If continuation of own city, add more
					std::set< Model::Color::Color > const winners = getCityOccupants( inBoardCopy, inPlacement.location, area );
					if ( winners.find( inColor ) != winners.end() )
					{
						if ( winners.size() == 1 )
						{
							value += getCitySize( inBoardCopy, inPlacement.location, area ) * kValuePerCityTile;
						}
						else
						{
							value += getCitySize( inBoardCopy, inPlacement.location, area ) * kValuePerSharedCityTile;
						}
					}
				}
			}
			value += amountOfOwnCloistersNearby * kValuePerCloisterTile;
		}
		// If finishing own city, add more
		// If continuation of other color's city, subtract some
		// If finishing of other color's city, subtract some
		// If merging own city with other city, add depending on gain
		return value;
	}

	std::vector< std::pair< std::size_t, boost::optional< Model::PlacedPiece > > >
	getValuesPerPiecePlacement
	(
		Model::Board inBoardCopy,
		Model::Tile const & inTile,
		PossiblePlacement const & inPlacement,
		Model::Color::Color inColor
	)
	{
		inBoardCopy.placeValidTile( Model::TileOnBoard( inTile, inPlacement.rotation ), inPlacement.location );
		Model::Piece const piece( Model::Piece::kFollower, inColor );
		std::vector< std::pair< std::size_t, boost::optional< Model::PlacedPiece > > > values;
		for ( Model::Area::Area area : kAreas )
		{
			Model::PlacedPiece const placedPiece( piece, area );
			if ( inBoardCopy.isValidPiecePlacement( inPlacement.location, placedPiece ) )
			{
				if ( inBoardCopy.isCity( inPlacement.location, area ) )
				{
					std::size_t value = kDefaultValue;
					std::size_t const citySize = getCitySize( inBoardCopy, inPlacement.location, area );
					value = kValueStartCity + citySize * kValuePerCityTile;
					values.emplace_back( std::make_pair( value, placedPiece ) );
				}
				else if ( inBoardCopy.isCloister( inPlacement.location, area ) )
				{
					std::size_t value = kDefaultValue;
					std::size_t const cloisterSize = inBoardCopy.getNrOfSurroundingTiles( inPlacement.location );
					value = kValueStartCloister + cloisterSize * kValuePerCloisterTile;
					values.emplace_back( std::make_pair( value, placedPiece ) );
				}
			}
		}
		return values;
	}

	std::pair< std::size_t, boost::optional< Model::PlacedPiece > >
	getBestValue( std::vector< std::pair< std::size_t, boost::optional< Model::PlacedPiece > > > const & inValues )
	{
		if ( inValues.empty() )
		{
			return std::make_pair( 0, boost::none );
		}
		assert( !inValues.empty() );
		std::pair< std::size_t, boost::optional< Model::PlacedPiece > > bestValue = inValues.front();
		for ( auto const & value : inValues )
		{
			if ( value.first > bestValue.first )
			{
				bestValue = value;
			}
		}
		return bestValue;
	}

	std::pair< std::size_t, boost::optional< Model::PlacedPiece > >
	determineValue
	(
		Model::Board const & inBoard,
		Model::Tile const & inTile,
		PossiblePlacement const & inPlacement,
		Model::Color::Color inColor
	)
	{
		std::size_t valueOfTilePlacement = getValueForTile( inBoard, inTile, inPlacement, inColor );
		auto const valuesPerPiecePlacement = getValuesPerPiecePlacement( inBoard, inTile, inPlacement, inColor );
		auto bestPlacement = getBestValue( valuesPerPiecePlacement );
		bestPlacement.first += valueOfTilePlacement;
		return bestPlacement;
	}
}

Controller::RobotPlayer::RobotPlayer
(
	std::string const & inName,
	Model::Color::Color inColor,
	std::map< Model::Piece::PieceType, std::size_t > const & inMeepleSupply
)
:
	Player( inName, inColor, inMeepleSupply ),
	mPlaceTileTimer( new QTimer( this ) ),
	mPlacePieceTimer( new QTimer( this ) ),
	mCurrentBoard(),
	mTileToPlace(),
	mTilePlacement(),
	mPiecePlacement()
{
	mPlaceTileTimer->setSingleShot( true );
	mPlaceTileTimer->setInterval( kPlaceTileInterval );
	connect( mPlaceTileTimer, &QTimer::timeout, [ this ]{ sendTilePlaced(); } );
	mPlacePieceTimer->setSingleShot( true );
	mPlacePieceTimer->setInterval( kPlacePieceInterval );
	connect( mPlacePieceTimer, &QTimer::timeout, [ this ]{ sendPiecePlaced(); } );
}

Controller::RobotPlayer::~RobotPlayer()
{
}

void
Controller::RobotPlayer::placeTile
(
	Model::Board const & inCurrentBoard,
	Model::Tile const & inTileToPlace
)
{
	// Save current board and picked tile to make the decision.
	mCurrentBoard = inCurrentBoard;
	mTileToPlace = inTileToPlace;

	// Decide where to place the tile.
	decideTileAndPiecePlacement();

	// Pretend to think.
	mPlaceTileTimer->start();
}

void
Controller::RobotPlayer::placePiece
(
	Model::Board const & /*inCurrentBoard*/,
	Utils::Location const & /*inLocation*/
)
{
	// Pretend to think.
	mPlacePieceTimer->start();
}

void
Controller::RobotPlayer::sendTilePlaced()
{
	assert( mTilePlacement );
	tilePlaced( *mTilePlacement );
	mTilePlacement = boost::none;
}

void
Controller::RobotPlayer::sendPiecePlaced()
{
	piecePlaced( mPiecePlacement );
	mPiecePlacement = boost::none;
}

void
Controller::RobotPlayer::decideTileAndPiecePlacement()
{
	assert( mCurrentBoard );
	assert( mTileToPlace );

	Utils::Locations const possibleLocations = mCurrentBoard->getPossibleLocations( *mTileToPlace );
	std::vector< PossiblePlacement > possibilities;
	for ( auto const & location : possibleLocations )
	{
		Model::Rotation rotation = Model::kCw0;
		for ( std::size_t i = 0; i < 4; ++i, rotation = rotateCW( rotation ) )
		{
			if ( mCurrentBoard->isValidTilePlacement( Model::TileOnBoard( *mTileToPlace, rotation ), location ) )
			{
				PossiblePlacement placement( location, rotation );
				std::tie( placement.value, placement.piece ) = determineValue( *mCurrentBoard, *mTileToPlace, placement, getColor() );
				possibilities.emplace_back( placement );
			}
		}
	}

	assert( !possibilities.empty() );
	// PossiblePlacement const chosenPlacement = getBestPossibility( possibilities );
	PossiblePlacement const chosenPlacement = getRandomPossibility( possibilities );

	mTilePlacement = TilePlacement( chosenPlacement.location, chosenPlacement.rotation );
	mPiecePlacement = chosenPlacement.piece;

	assert( mTilePlacement );
}
