#include "Controller/RobotPlayer.h"

#include "Model/Board.h"
#include "Model/Points.h"
#include "Model/Tile.h"
#include "Model/TileOnBoard.h"

#include "Utils/Location.h"
#include "Utils/Random.h"

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace
{
	int const kPlaceTileInterval = 1000;
	int const kPlacePieceInterval = 1000;

	double const kDefaultValue = 0.;

	double const kValueStartCity = 10.;
	double const kValuePerCityTile = 50.;
	double const kValuePerSharedCityTile = 15.;
	double const kValueContinueOwnCity = 25.;
	double const kValueFinishOwnCity = 25.;

	double const kValueStartRoad = 6.;
	double const kValuePerRoadTile = 30.;
	double const kValuePerSharedRoadTile = 9.;
	double const kValueContinueOwnRoad = 15.;
	double const kValueFinishOwnRoad = 15.;

	double const kValueStartCloister = 5.;
	double const kValuePerCloisterTile = 15.;

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
	std::array< Model::Piece::PieceType, 2 > const kPieces =
	{
		{
			Model::Piece::kFollower,
			Model::Piece::kLargeFollower
		}
	};

	struct PossiblePlacement
	{
		Utils::Location location;
		Model::Rotation rotation;
		double value;
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

	void
	printPossibility( PossiblePlacement const & inPossibility )
	{
		std::cout << "  " << inPossibility.location << ", " << inPossibility.rotation << ", " << inPossibility.value;
		if ( inPossibility.piece )
		{
			std::cout << ", " << inPossibility.piece->getArea();
		}
		std::cout << std::endl;
	}

	void
	printPossibilities( std::vector< PossiblePlacement > const & inPossibilities )
	{
		std::cout << inPossibilities.size() << " possibilities." << std::endl;
		for ( PossiblePlacement const & possibility : inPossibilities )
		{
			printPossibility( possibility );
		}
	}

	void
	makePossibilitiesPositive( std::vector< PossiblePlacement > & ioPossiblePlacements )
	{
		for ( auto & possibility : ioPossiblePlacements )
		{
			if ( possibility.value < 0. )
			{
				possibility.value = exp( possibility.value );
			}
			else
			{
				possibility.value += 1.;
			}
		}
	}

	double
	getTotalValue( std::vector< PossiblePlacement > const & inPossibilities )
	{
		double totalValue = 0.;
		for ( auto const & possibility : inPossibilities )
		{
			assert( possibility.value >= 0. );
			totalValue += possibility.value;
		}
		assert( totalValue > 0. );
		return totalValue;
	}

	PossiblePlacement
	getRandomPossibility( std::vector< PossiblePlacement > const & inPossibilities )
	{
		double const totalValue = getTotalValue( inPossibilities );
		double const chosenValue = Utils::Random( totalValue );
		double cumulativeValue = 0;
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
		double bestValue = 0;
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

	std::set< Model::Color::Color >
	getRoadOccupants( Model::Board const & inBoard, Utils::Location const & inLocation, Model::Area::Area inArea )
	{
		std::vector< Model::PlacedPiece > allPieces;
		for ( auto const & roadPiece : inBoard.getCompleteRoad( Model::PlacedRoad( inLocation.row, inLocation.col, inArea ) ) )
		{
			std::vector< Model::PlacedPiece > const pieces = inBoard.getPieces( roadPiece );
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
	getRoadSize( Model::Board const & inBoard, Utils::Location const & inLocation, Model::Area::Area inArea )
	{
		std::set< Utils::Location > usedTiles;
		for ( auto const & cityPiece : inBoard.getCompleteRoad( Model::PlacedRoad( inLocation.row, inLocation.col, inArea ) ) )
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

	void
	addForContinuingCities
	(
		Model::Board const & inBoard,
		Model::TileOnBoard const & inTile,
		Utils::Location const & inLocation,
		Model::Color::Color inColor,
		double & ioValue
	)
	{
		for ( auto const & city : inTile.getContiguousCities() )
		{
			Model::Area::Area const cityArea = city.front();
			if ( inBoard.isOccupiedCity( inLocation, cityArea ) )
			{
				// If continuation of own city, add more
				std::set< Model::Color::Color > const winners = getCityOccupants( inBoard, inLocation, cityArea );
				if ( winners.find( inColor ) != winners.end() )
				{
					if ( winners.size() == 1 )
					{
						ioValue += getCitySize( inBoard, inLocation, cityArea ) * kValuePerCityTile + kValueContinueOwnCity;
					}
					else
					{
						ioValue += getCitySize( inBoard, inLocation, cityArea ) * kValuePerSharedCityTile;
					}
				}
			}
		}
	}

	void
	addForFinishingCities
	(
		Model::Board const & inBoard,
		Model::TileOnBoard const & inTile,
		Utils::Location const & inLocation,
		Model::Color::Color inColor,
		double & ioValue
	)
	{
		for ( auto const & city : inTile.getContiguousCities() )
		{
			Model::Area::Area const cityArea = city.front();
			if ( inBoard.isFinishedCity( inLocation, cityArea ) )
			{
				if ( inBoard.isOccupiedCity( inLocation, cityArea ) )
				{
					// If continuation of own city, add more
					std::set< Model::Color::Color > const winners = getCityOccupants( inBoard, inLocation, cityArea );
					if ( winners.find( inColor ) != winners.end() )
					{
						if ( winners.size() == 1 )
						{
							ioValue += getCitySize( inBoard, inLocation, cityArea ) * kValuePerCityTile + kValueFinishOwnCity;
						}
						else
						{
							ioValue += getCitySize( inBoard, inLocation, cityArea ) * kValuePerSharedCityTile;
						}
					}
				}
			}
		}
	}

	void
	addForContinuingRoads
	(
		Model::Board const & inBoard,
		Model::TileOnBoard const & inTile,
		Utils::Location const & inLocation,
		Model::Color::Color inColor,
		double & ioValue
	)
	{
		for ( auto const & road : inTile.getContiguousRoads() )
		{
			Model::Area::Area const roadArea = road.front();
			if ( inBoard.isOccupiedRoad( inLocation, roadArea ) )
			{
				// If continuation of own road, add more
				std::set< Model::Color::Color > const winners = getRoadOccupants( inBoard, inLocation, roadArea );
				if ( winners.find( inColor ) != winners.end() )
				{
					if ( winners.size() == 1 )
					{
						ioValue += getRoadSize( inBoard, inLocation, roadArea ) * kValuePerRoadTile + kValueContinueOwnRoad;
					}
					else
					{
						ioValue += getRoadSize( inBoard, inLocation, roadArea ) * kValuePerSharedRoadTile;
					}
				}
			}
		}
	}

	void
	addForFinishingRoads
	(
		Model::Board const & inBoard,
		Model::TileOnBoard const & inTile,
		Utils::Location const & inLocation,
		Model::Color::Color inColor,
		double & ioValue
	)
	{
		for ( auto const & road : inTile.getContiguousRoads() )
		{
			Model::Area::Area const roadArea = road.front();
			if ( inBoard.isFinishedRoad( inLocation, roadArea ) )
			{
				if ( inBoard.isOccupiedRoad( inLocation, roadArea ) )
				{
					// If continuation of own road, add more
					std::set< Model::Color::Color > const winners = getRoadOccupants( inBoard, inLocation, roadArea );
					if ( winners.find( inColor ) != winners.end() )
					{
						if ( winners.size() == 1 )
						{
							ioValue += getRoadSize( inBoard, inLocation, roadArea ) * kValuePerRoadTile + kValueFinishOwnRoad;
						}
						else
						{
							ioValue += getRoadSize( inBoard, inLocation, roadArea ) * kValuePerSharedRoadTile;
						}
					}
				}
			}
		}
	}

	double
	getValueForTile
	(
		Model::Board inBoardCopy,
		Model::Tile const & inTile,
		PossiblePlacement const & inPlacement,
		Controller::Player const & inPlayer
	)
	{
		double value = kDefaultValue;
		Model::TileOnBoard const proposedTile( inTile, inPlacement.rotation );
		inBoardCopy.placeValidTile( proposedTile, inPlacement.location );
		std::size_t const amountOfOwnCloistersNearby = getAmountOfOwnCloistersNearby( inBoardCopy, inPlacement.location, inPlayer.getColor() );
		value += amountOfOwnCloistersNearby * kValuePerCloisterTile;
		addForContinuingCities( inBoardCopy, proposedTile, inPlacement.location, inPlayer.getColor(), value );
		addForFinishingCities( inBoardCopy, proposedTile, inPlacement.location, inPlayer.getColor(), value );
		// If continuation of other color's city, subtract some
		// If finishing of other color's city, subtract some
		// If merging own city with other city, add depending on gain
		addForContinuingRoads( inBoardCopy, proposedTile, inPlacement.location, inPlayer.getColor(), value );
		addForFinishingRoads( inBoardCopy, proposedTile, inPlacement.location, inPlayer.getColor(), value );
		return value;
	}

	std::vector< std::pair< double, boost::optional< Model::PlacedPiece > > >
	getValuesPerPiecePlacement
	(
		Model::Board inBoardCopy,
		Model::Tile const & inTile,
		PossiblePlacement const & inPlacement,
		Model::Piece::PieceType inPieceType,
		Controller::Player const & inPlayer
	)
	{
		if ( !inPlayer.hasPiece( inPieceType ) )
		{
			return {};
		}
		inBoardCopy.placeValidTile( Model::TileOnBoard( inTile, inPlacement.rotation ), inPlacement.location );
		Model::Piece const piece( inPieceType, inPlayer.getColor() );
		std::vector< std::pair< double, boost::optional< Model::PlacedPiece > > > values;
		for ( Model::Area::Area area : kAreas )
		{
			Model::PlacedPiece const placedPiece( piece, area );
			if ( inBoardCopy.isValidPiecePlacement( inPlacement.location, placedPiece ) )
			{
				if ( inBoardCopy.isCity( inPlacement.location, area ) )
				{
					std::size_t const citySize = getCitySize( inBoardCopy, inPlacement.location, area );
					double value = kValueStartCity + kValuePerCityTile * citySize;
					values.emplace_back( std::make_pair( value, placedPiece ) );
				}
				else if ( inBoardCopy.isCloister( inPlacement.location, area ) )
				{
					std::size_t const cloisterSize = inBoardCopy.getNrOfSurroundingTiles( inPlacement.location );
					double value = kValueStartCloister + kValuePerCloisterTile * cloisterSize;
					values.emplace_back( std::make_pair( value, placedPiece ) );
				}
				else if ( inBoardCopy.isRoad( inPlacement.location, area ) )
				{
					std::size_t const roadSize = getRoadSize( inBoardCopy, inPlacement.location, area );
					double value = kValueStartRoad + kValuePerRoadTile * roadSize;
					values.emplace_back( std::make_pair( value, placedPiece ) );
				}
			}
		}
		return values;
	}

	std::vector< std::pair< double, boost::optional< Model::PlacedPiece > > >
	getValuesPerPiecePlacement
	(
		Model::Board const & inBoard,
		Model::Tile const & inTile,
		PossiblePlacement const & inPlacement,
		Controller::Player const & inPlayer
	)
	{
		std::vector< std::pair< double, boost::optional< Model::PlacedPiece > > > values;
		for ( Model::Piece::PieceType pieceType : kPieces )
		{
			auto const pieceValues = getValuesPerPiecePlacement( inBoard, inTile, inPlacement, pieceType, inPlayer );
			values.insert( values.end(), pieceValues.begin(), pieceValues.end() );
		}
		return values;
	}

	std::pair< double, boost::optional< Model::PlacedPiece > >
	getBestPlacement( std::vector< std::pair< double, boost::optional< Model::PlacedPiece > > > const & inValues )
	{
		if ( inValues.empty() )
		{
			return std::make_pair( 0., boost::none );
		}
		std::pair< double, boost::optional< Model::PlacedPiece > > bestValue = inValues.front();
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
		Controller::Player const & inPlayer
	)
	{
		double const valueOfTilePlacement = getValueForTile( inBoard, inTile, inPlacement, inPlayer );
		auto const valuesPerPiecePlacement = getValuesPerPiecePlacement( inBoard, inTile, inPlacement, inPlayer );
		auto bestPlacement = getBestPlacement( valuesPerPiecePlacement );
		bestPlacement.first += valueOfTilePlacement;
		return bestPlacement;
	}

	double
	getProbabilityOfFinishing
	(
		Model::Board const & /*inBoard*/,
		Controller::Player const & /*inPlayer*/,
		Utils::Location const & /*inLocation*/,
		Model::Area::Area /*inArea*/
	)
	{
		return 0.5;
	}

	bool
	isWinningColor
	(
		Model::Board const & /*inBoard*/,
		Controller::Player const & /*inPlayer*/,
		Utils::Location const & /*inLocation*/,
		Model::Area::Area /*inArea*/
	)
	{
		return true;
	}

	double
	getEstimationFinished
	(
		Model::Board const & inBoard,
		Controller::Player const & inPlayer,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	)
	{
		if ( isWinningColor( inBoard, inPlayer, inLocation, inArea ) )
		{
			return getPointsAssumingFinished( inBoard, inLocation, inArea );
		}
		return 0.;
	}

	double
	getEstimationCurrent
	(
		Model::Board const & inBoard,
		Controller::Player const & inPlayer,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	)
	{
		if ( isWinningColor( inBoard, inPlayer, inLocation, inArea ) )
		{
			return getPoints( inBoard, inLocation, inArea );
		}
		return 0.;
	}

	double
	getEstimation
	(
		Model::Board const & inBoard,
		Controller::Player const & inPlayer
	)
	{
		double estimation = inPlayer.getScore();
		// for every project on the board
		//  add probability(finished) * score estimation(finished)
		//	add probability(!finished) * score estimation(!finished)
		for ( auto const & locatedPiece : inBoard.getPieces( inPlayer.getColor() ) )
		{
			double const probabilityOfFinishing = getProbabilityOfFinishing
			(
				inBoard, inPlayer, locatedPiece.first, locatedPiece.second.getArea()
			);
			double const estimationWhenFinished = getEstimationFinished
			(
				inBoard, inPlayer, locatedPiece.first, locatedPiece.second.getArea()
			);
			double const estimationCurrent = getEstimationCurrent
			(
				inBoard, inPlayer, locatedPiece.first, locatedPiece.second.getArea()
			);
			estimation += probabilityOfFinishing * estimationWhenFinished;
			estimation += ( 1. - probabilityOfFinishing ) * estimationCurrent;
		}
		// for every piece not on the board
		//  add estimated value
		return estimation;
	}

	std::pair< double, boost::optional< Model::PlacedPiece > >
	determineValuePointBased
	(
		Model::Board inBoard,
		Model::Tile const & inTile,
		PossiblePlacement const & inPlacement,
		Controller::Player const & inPlayer
	)
	{
		Model::TileOnBoard const tilePlacement( inTile, inPlacement.rotation );
		inBoard.placeValidTile( tilePlacement, inPlacement.location );
		
		// For every possible piece placement (+no piece placement), calculate
		// point estimation.
		std::vector< std::pair< double, boost::optional< Model::PlacedPiece > > > estimations;
		estimations.emplace_back( getEstimation( inBoard, inPlayer ), boost::none );

		for ( Model::Piece::PieceType pieceType : kPieces )
		{
			if ( !inPlayer.hasPiece( pieceType ) )
			{
				continue;
			}
			Model::Piece const piece( pieceType, inPlayer.getColor() );

			for ( Model::Area::Area area : kAreas )
			{
				Model::Board boardCopy = inBoard;
				Model::PlacedPiece const placedPiece( piece, area );
				if ( boardCopy.isValidPiecePlacement( inPlacement.location, placedPiece ) )
				{
					boardCopy.placeValidPiece( placedPiece, inPlacement.location );
					estimations.emplace_back( getEstimation( boardCopy, inPlayer ), placedPiece );
				}
			}
		}
		// Return most promising piece placement.
		return getBestPlacement( estimations );
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

	assert( mTilePlacement );

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
				std::tie( placement.value, placement.piece ) = determineValue( *mCurrentBoard, *mTileToPlace, placement, *this );
				possibilities.emplace_back( placement );
			}
		}
	}

	printPossibilities( possibilities );

	assert( !possibilities.empty() );
	makePossibilitiesPositive( possibilities );
	printPossibilities( possibilities );
	PossiblePlacement const chosenPlacement = getBestPossibility( possibilities );
	// PossiblePlacement const chosenPlacement = getRandomPossibility( possibilities );

	std::cout << "Chosen placement:" << std::endl;
	printPossibility( chosenPlacement );

	mTilePlacement = TilePlacement( chosenPlacement.location, chosenPlacement.rotation );
	mPiecePlacement = chosenPlacement.piece;

	assert( mTilePlacement );
}
