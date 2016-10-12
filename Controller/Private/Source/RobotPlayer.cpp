#include "Controller/RobotPlayer.h"

#include "Model/Area.h"
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
			value( 0. ),
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
		Model::Board const & inBoard,
		Controller::Player const & inPlayer,
		Utils::Location const & inLocation,
		Model::Area::Area inArea
	)
	{
		std::vector< Model::PlacedProject > const completeProject = getCompleteProject( inBoard, inLocation, inArea );
		std::vector< Model::PlacedPiece > allPieces;
		for ( Model::PlacedProject const & project : completeProject )
		{
			std::vector< Model::PlacedPiece > const pieces = inBoard.getPieces( project );
			allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
		}
		std::set< Model::Color::Color > const winningColors = getWinningColors( allPieces );
		return winningColors.find( inPlayer.getColor() ) != winningColors.end();
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
	getEstimationForMeeple( std::size_t inSupply, std::size_t inTilesLeft )
	{
		return std::log( inSupply + 1 ) * std::log( inTilesLeft + 1 );
	}

	double
	getEstimation
	(
		Model::Board const & inBoard,
		std::size_t inTilesLeft,
		Controller::Player const & inPlayer
	)
	{
		double estimation = inPlayer.getScore();
		// for every project on the board
		//  add probability(finished) * score estimation(finished)
		//	add probability(!finished) * score estimation(!finished)
		for ( auto const & locatedPiece : getPieces( inBoard, inPlayer.getColor() ) )
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
		std::size_t const supply = inPlayer.getSupply( Model::Piece::kFollower ) + inPlayer.getSupply( Model::Piece::kLargeFollower );
		estimation += getEstimationForMeeple( supply, inTilesLeft );
		return estimation;
	}

	std::pair< double, boost::optional< Model::PlacedPiece > >
	determineValuePointBased
	(
		Model::Board inBoard,
		std::size_t inTilesLeft,
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
		estimations.emplace_back( getEstimation( inBoard, inTilesLeft, inPlayer ), boost::none );

		for ( Model::Piece::PieceType pieceType : kPieces )
		{
			if ( !inPlayer.hasPiece( pieceType ) )
			{
				continue;
			}
			Model::Piece const piece( pieceType, inPlayer.getColor() );

			for ( Model::ContiguousProject project : getAllProjects( tilePlacement ) )
			{
				Model::Board boardCopy = inBoard;
				Model::Area::Area const area = project.front();
				Model::PlacedPiece const placedPiece( piece, area );
				if ( boardCopy.isValidPiecePlacement( inPlacement.location, placedPiece ) )
				{
					boardCopy.placeValidPiece( placedPiece, inPlacement.location );
					estimations.emplace_back( getEstimation( boardCopy, inTilesLeft, inPlayer ), placedPiece );
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
	mTilesLeft(),
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
	std::size_t inTilesLeft,
	Model::Tile const & inTileToPlace
)
{
	// Save current board and picked tile to make the decision.
	mCurrentBoard = inCurrentBoard;
	mTilesLeft = inTilesLeft;
	mTileToPlace = inTileToPlace;

	// Pretend to think.
	mPlaceTileTimer->start();
}

void
Controller::RobotPlayer::placePiece
(
	Model::Board const & /*inCurrentBoard*/,
	std::size_t /*inTilesLeft*/,
	Utils::Location const & /*inLocation*/
)
{
	// Pretend to think.
	mPlacePieceTimer->start();
}

void
Controller::RobotPlayer::sendTilePlaced()
{
	// Decide where to place the tile.
	decideTileAndPiecePlacement();
	
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
	assert( mTilesLeft );
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
				std::tie( placement.value, placement.piece ) = determineValuePointBased( *mCurrentBoard, *mTilesLeft, *mTileToPlace, placement, *this );
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
