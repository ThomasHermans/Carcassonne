#include "Model/Game.h"

#include "Model/CreateTilesAndPieces.h"

#include <boost/foreach.hpp>

#include <cassert>
#include <map>
#include <utility>

namespace
{
	std::size_t const kBrigandPPTileUnfinished = 1;
	std::size_t const kBrigandPPTileFinished = 1;
	std::size_t const kBrigandPPTileFinishedWithInn = 2;
	std::size_t const kFarmerPPCity = 3;
	std::size_t const kKnightPPTileFinished = 2;
	std::size_t const kKnightPPTileFinishedWithCathedral = 3;
	std::size_t const kKnightPPShieldFinished = 2;
	std::size_t const kKnightPPShieldFinishedWithCathedral = 3;
	std::size_t const kKnightPPTileUnfinished = 1;
	std::size_t const kKnightPPShieldUnfinished = 1;
	std::size_t const kMonkPPTile = 1;
	std::size_t const kMonkPointsFinished = 9;

	std::set< Model::Color::Color >
	getWinningColors( std::vector< Model::PlacedPiece > const & inPieces )
	{
		typedef std::map< Model::Color::Color, std::size_t > Counts;
		Counts counts;
		BOOST_FOREACH( Model::PlacedPiece const & piece, inPieces )
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

	std::size_t
	calculatePointsUnfinishedRoad( std::size_t inNrOfTiles, bool inHasInn )
	{
		if ( inHasInn )
		{
			return 0;
		}
		else
		{
			return inNrOfTiles * kBrigandPPTileUnfinished;
		}
	}

	std::size_t
	calculatePointsFinishedRoad( std::size_t inNrOfTiles, bool inHasInn )
	{
		if ( inHasInn )
		{
			return inNrOfTiles * kBrigandPPTileFinishedWithInn;
		}
		else
		{
			return inNrOfTiles * kBrigandPPTileFinished;
		}		
	}

	std::size_t
	calculatePointsUnfinishedCity( std::size_t inNrOfTiles, std::size_t inNrOfShields, bool inHasCathedral )
	{
		if ( inHasCathedral )
		{
			return 0;
		}
		else
		{
			return inNrOfTiles * kKnightPPTileUnfinished + inNrOfShields * kKnightPPShieldUnfinished;
		}
	}

	std::size_t
	calculatePointsFinishedCity( std::size_t inNrOfTiles, std::size_t inNrOfShields, bool inHasCathedral )
	{
		if ( inHasCathedral )
		{
			return inNrOfTiles * kKnightPPTileFinishedWithCathedral + inNrOfShields * kKnightPPShieldFinishedWithCathedral;
		}
		else
		{
			return inNrOfTiles * kKnightPPTileFinished + inNrOfShields * kKnightPPShieldFinished;
		}
	}
}

Model::Game::Game
(
	std::vector< Player > const & inPlayers,
	std::set< Utils::Expansion::Type > const & inExpansions
)
:
	mBoard(),
	mBag( createTiles( inExpansions ) ),
	mNextTile(),
	mPlayers( inPlayers ),
	mCurrentPlayer( 0 ),
	mPiecesPlacedThisTurn( 0 ),
	mCurrentPlacedTile()
{
	initialize();
}

Model::Game::Game( std::vector< Player > const & inPlayers, std::string const & inTiles )
:
	mBoard(),
	mBag( createTiles( inTiles ) ),
	mNextTile(),
	mPlayers( inPlayers ),
	mCurrentPlayer( 0 ),
	mPiecesPlacedThisTurn( 0 ),
	mCurrentPlacedTile()
{
	initialize();
}

void
Model::Game::placeStartTileOnBoard()
{
	if ( mNextTile )
	{
		Rotation const rotation = kCw0; // TODO: get a random Rotation each time
		TileOnBoard const startTile( *mNextTile, rotation );
		bool const placed = mBoard.placeStartTile( startTile );
		if ( placed )
		{
			tilePlaced( Utils::Location( 0, 0 ), startTile.getID(), startTile.getRotation() );
			pickNextTile();
		}
	}
}

void
Model::Game::tryToPlaceTile
(
	int inRow,
	int inCol,
	std::string const & inTileID,
	Rotation inRotation
)
{
	if ( mNextTile && mNextTile->getID() == inTileID && isEmptySpot( inRow, inCol ) )
	{
		// Remember earlier placed tile
		boost::optional< PlacedTile > earlierTile = mCurrentPlacedTile;
		// Try the new chosen position
		Rotation rotation = inRotation;
		TileOnBoard placedTile( *mNextTile, rotation );
		bool found = false;
		for ( std::size_t i = 0; i < 4; ++i )
		{
			placedTile = TileOnBoard( *mNextTile, rotation );
			if ( mBoard.isValidTilePlacement( placedTile, inRow, inCol ) )
			{
				found = true;
				break;
			}
			rotation = rotateCW( rotation );
		}
		if ( found )
		{
			if ( earlierTile )
			{
				tileRemoved( Utils::Location( earlierTile->row, earlierTile->col ) );
				if ( earlierTile->tile.hasPieces() )
				{
					mPiecesPlacedThisTurn = 0;
					returnPieces( earlierTile->tile.removeAllPieces(), earlierTile->row, earlierTile->col );
				}
			}
			mCurrentPlacedTile = PlacedTile( placedTile, inRow, inCol );
			tilePlaced( Utils::Location( inRow, inCol ), placedTile.getID(), placedTile.getRotation() );
		}
	}
}

void
Model::Game::rotateTile( int inRow, int inCol )
{
	if ( isCurrentSpot( inRow, inCol ) )
	{
		Tile const tile = mCurrentPlacedTile->tile.getTile();
		Rotation rotation = mCurrentPlacedTile->tile.getRotation();
		TileOnBoard rotatedTile( tile, rotation );
		bool found = false;
		for ( std::size_t i = 0; i < 4; ++i )
		{
			rotation = rotateCW( rotation );
			rotatedTile = TileOnBoard( tile, rotation );
			if ( mBoard.isValidTilePlacement( rotatedTile, inRow, inCol ) )
			{
				found = true;
				break;
			}
		}
		if ( found )
		{
			if ( mCurrentPlacedTile->tile.hasPieces() )
			{
				mPiecesPlacedThisTurn = 0;
				returnPieces( mCurrentPlacedTile->tile.removeAllPieces(), mCurrentPlacedTile->row, mCurrentPlacedTile->col );
			}
			tileRemoved( Utils::Location( mCurrentPlacedTile->row, mCurrentPlacedTile->col ) );
			mCurrentPlacedTile = PlacedTile( rotatedTile, inRow, inCol );
			Utils::Location const location( mCurrentPlacedTile->row, mCurrentPlacedTile->col );
			tilePlaced( location, mCurrentPlacedTile->tile.getID(), mCurrentPlacedTile->tile.getRotation() );
		}
	}
}

void
Model::Game::tryToPlacePiece
(
	PlacedProject const & inPlace,
	Piece::PieceType inType,
	Color::Color inColor
)
{
	// A piece can only be placed on the current spot
	if ( isCurrentSpot( inPlace.row, inPlace.col ) )
	{
		// Only the active player can place a piece, if he has free pieces
		// and if he hasn't already placed a piece
		Player & player = mPlayers[ mCurrentPlayer ];
		if ( player.getColor() == inColor && player.hasPieceToPlace( inType ) && mPiecesPlacedThisTurn == 0 )
		{
			// You can only place a piece in an unoccupied area
			if ( !isOccupiedOnCurrentTile( inPlace.area ) )
			{
				boost::optional< Piece > const piece = player.getPieceToPlace( inType );
				if ( piece )
				{
					PlacedPiece const placedPiece( *piece, inPlace.area );
					if ( mCurrentPlacedTile->tile.placePiece( placedPiece ) )
					{
						++mPiecesPlacedThisTurn;
						Utils::Location const location( mCurrentPlacedTile->row, mCurrentPlacedTile->col );
						piecePlaced( location, placedPiece, player );
					}
					else
					{
						player.returnPiece( placedPiece.getPiece() );
					}
				}
			}
		}
	}
}

void
Model::Game::endTurn()
{
	if ( mCurrentPlacedTile )
	{
		if ( mBoard.placeValidTile( mCurrentPlacedTile->tile, mCurrentPlacedTile->row, mCurrentPlacedTile->col ) )
		{
			mCurrentPlayer = ( mCurrentPlayer + 1 ) % mPlayers.size();
			mPiecesPlacedThisTurn = 0;
			currentPlayerChanged( boost::ref( mPlayers[mCurrentPlayer] ) );
			mCurrentPlacedTile.reset();
			pickNextTile();
		}
	}
}

void
Model::Game::awardEndPoints()
{
	awardEndCloisterPoints();
	awardEndCityPoints();
	awardEndRoadPoints();
	awardEndFieldPoints();
}

void
Model::Game::initialize()
{
	// Print out bag
	BOOST_FOREACH( Tile const & tile, mBag )
	{
		std::cout << tile.getID();
	}
	std::cout << std::endl;
	// Initialize first tile
	if ( !mBag.empty() )
	{
		mNextTile = mBag.back();
		mBag.pop_back();
	}
	// Game signals
	mBoard.finishedCity.connect( boost::bind( &Model::Game::onFinishedCity, this, _1 ) );
	mBoard.finishedRoad.connect( boost::bind( &Model::Game::onFinishedRoad, this, _1 ) );
	mBoard.finishedCloister.connect( boost::bind( &Model::Game::onFinishedCloister, this, _1, _2 ) );
	// Player signals
	BOOST_FOREACH( Player & player, mPlayers )
	{
		boost::function< void () > forwardSignal = boost::bind( boost::ref( playerInfoChanged ), boost::ref( player ) );
		// player.GetNrOfFreePiecesChangedSignal().connect( forwardSignal );
		// player.GetScoreChangedSignal().connect( forwardSignal );
		player.GetInfoChangedSignal().connect( forwardSignal );
	}
	// Inform controller about first player
	currentPlayerChanged( boost::ref( mPlayers.front() ) );
}

void
Model::Game::pickNextTile()
{
	std::vector< Tile >::iterator it = mBag.end();
	mNextTile = boost::none;
	while ( !mNextTile && it != mBag.begin() )
	{
		--it;
		Tile const possibleNextTile = *it;
		if ( mBoard.isPossibleTile( possibleNextTile ) )
		{
			mNextTile = possibleNextTile;
			nextTile( mNextTile->getID() );
			mBag.erase( it );
		}
	}
	tilesLeft( mBag.size() );
	if ( mNextTile )
	{
		possibleLocationsChanged( mBoard.getPossibleLocations( *mNextTile ) );
	}
	else
	{
		mNextTile = boost::none;
		endOfGame( mBag.size() );
	}
}

bool
Model::Game::isEmptySpot( int inRow, int inCol ) const
{
	return ( !isCurrentSpot( inRow, inCol ) && !mBoard.isTile( inRow, inCol ) );
}

bool
Model::Game::isCurrentSpot( int inRow, int inCol ) const
{
	return ( mCurrentPlacedTile && mCurrentPlacedTile->row == inRow && mCurrentPlacedTile->col == inCol );
}

void
Model::Game::returnPieces( std::vector< PlacedPiece > const & inPieces, int inRow, int inCol )
{
	BOOST_FOREACH( PlacedPiece const & piece, inPieces )
	{
		Player & player = getPlayer( piece.getPiece().getColor() );
		player.returnPiece( piece.getPiece() );
		pieceRemoved( Utils::Location( inRow, inCol ), piece, player );
	}
}

Model::Player &
Model::Game::getPlayer( Color::Color inColor )
{
	BOOST_FOREACH( Player & player, mPlayers )
	{
		if ( player.getColor() == inColor )
		{
			return player;
		}
	}
	assert( !"getPlayer found no player with the given color." );
}

bool
Model::Game::isOccupiedOnCurrentTile( Area::Area inArea ) const
{
	if ( !mCurrentPlacedTile )
	{
		return true;
	}
	// Cloister
	if ( mCurrentPlacedTile->tile.isCloister( inArea ) )
	{
		return mCurrentPlacedTile->tile.hasPiece( inArea );
	}
	// Road
	if ( mCurrentPlacedTile->tile.isRoad( inArea ) )
	{
		ContiguousRoad const contRoad = mCurrentPlacedTile->tile.getContiguousRoad( inArea );
		std::vector< PlacedRoad > roadsToCheck;
		BOOST_FOREACH( Area::Area area, contRoad )
		{
			if ( mCurrentPlacedTile->tile.hasPiece( area ) )
			{
				return true;
			}
			PlacedRoad const thisRoad( mCurrentPlacedTile->row, mCurrentPlacedTile->col, area );
			roadsToCheck.push_back( getNeighbor( thisRoad ) );
		}
		BOOST_FOREACH( PlacedRoad road, roadsToCheck )
		{
			if ( mBoard.isOccupiedRoad( road ) )
			{
				return true;
			}
		}
		return false;
	}
	// City
	if ( mCurrentPlacedTile->tile.isCity( inArea ) )
	{
		ContiguousCity const contCity = mCurrentPlacedTile->tile.getContiguousCity( inArea );
		std::vector< PlacedCity > citiesToCheck;
		BOOST_FOREACH( Area::Area area, contCity )
		{
			if ( mCurrentPlacedTile->tile.hasPiece( area ) )
			{
				return true;
			}
			PlacedCity const thisCity( mCurrentPlacedTile->row, mCurrentPlacedTile->col, area );
			citiesToCheck.push_back( getNeighbor( thisCity ) );
		}
		BOOST_FOREACH( PlacedCity city, citiesToCheck )
		{
			if ( mBoard.isOccupiedCity( city ) )
			{
				return true;
			}
		}
		return false;
	}
	// Field
	if ( mCurrentPlacedTile->tile.isField( inArea ) )
	{
		ContiguousField const contField = mCurrentPlacedTile->tile.getContiguousField( inArea );
		std::vector< PlacedField > fieldsToCheck;
		BOOST_FOREACH( Area::Area area, contField )
		{
			if ( mCurrentPlacedTile->tile.hasPiece( area ) )
			{
				return true;
			}
			PlacedField const thisField( mCurrentPlacedTile->row, mCurrentPlacedTile->col, area );
			fieldsToCheck.push_back( getNeighbor( thisField ) );
		}
		BOOST_FOREACH( PlacedField field, fieldsToCheck )
		{
			if ( mBoard.isOccupiedField( field ) )
			{
				return true;
			}
		}
		return false;
	}
	// Something else is always occupied
	return true;
}

void
Model::Game::awardEndCloisterPoints()
{
	for ( int row = mBoard.getTopRow(); row <= mBoard.getBottomRow(); ++row )
	{
		for ( int col = mBoard.getLeftCol(); col <= mBoard.getRightCol(); ++col )
		{
			if ( mBoard.isTile( row, col ) )
			{
				TileOnBoard const tile = *mBoard.getTile( row, col );
				std::vector< PlacedPiece > const placedPieces = tile.getPlacedPieces();
				BOOST_FOREACH( PlacedPiece const piece, placedPieces )
				{
					if ( tile.isCloister( piece.getArea() ) )
					{
						// Remove and return all pieces from the cloister
						PlacedProject const project( row, col, piece.getArea() );
						std::vector< PlacedPiece > const pieces = mBoard.removePieces( project );
						returnPieces( pieces, row, col );
						// Calculate winner(s) and points of the cloister
						std::set< Color::Color > const winningColors = getWinningColors( pieces );
						std::size_t const points = mBoard.getNrOfSurroundingTiles( row, col ) * kMonkPPTile;
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Model::Game::awardEndCityPoints()
{
	for ( int row = mBoard.getTopRow(); row <= mBoard.getBottomRow(); ++row )
	{
		for ( int col = mBoard.getLeftCol(); col <= mBoard.getRightCol(); ++col )
		{
			if ( mBoard.isTile( row, col ) )
			{
				TileOnBoard const tile = *mBoard.getTile( row, col );
				std::vector< PlacedPiece > const placedPieces = tile.getPlacedPieces();
				BOOST_FOREACH( PlacedPiece const piece, placedPieces )
				{
					if ( tile.isCity( piece.getArea() ) )
					{
						PlacedCity const cityPart( row, col, piece.getArea() );
						std::vector< PlacedCity > const completeCity = mBoard.getCompleteCity( cityPart );
						// Go over the complete city to get all the information
						std::vector< PlacedPiece > allPieces;
						std::set< std::pair< int, int > > usedTiles;
						std::set< PlacedCity > allShields;
						bool hasCathedral = false;
						BOOST_FOREACH( PlacedCity const & city, completeCity )
						{
							// Add the tile to usedTiles
							usedTiles.insert( std::make_pair( city.row, city.col ) );
							// Check if a cathedral is present
							hasCathedral = hasCathedral || mBoard.getTile( city.row, city.col )->getCenter() == Tile::kCenterCathedral;
							// Add the shields on the tile
							std::vector< Area::Area > const shields = mBoard.getTile( city.row, city.col )->getShields();
							if ( std::find( shields.begin(), shields.end(), city.area ) != shields.end() )
							{
								allShields.insert( city );
							}
							// Remove and return all the pieces from this city
							std::vector< PlacedPiece > const pieces = mBoard.removePieces( city );
							returnPieces( pieces, city.row, city.col );
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner(s) and points of the city
						std::set< Color::Color > const winningColors = getWinningColors( allPieces );
						std::size_t const points = calculatePointsUnfinishedCity( usedTiles.size(), allShields.size(), hasCathedral );
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Model::Game::awardEndRoadPoints()
{
	for ( int row = mBoard.getTopRow(); row <= mBoard.getBottomRow(); ++row )
	{
		for ( int col = mBoard.getLeftCol(); col <= mBoard.getRightCol(); ++col )
		{
			if ( mBoard.isTile( row, col ) )
			{
				TileOnBoard const tile = *mBoard.getTile( row, col );
				std::vector< PlacedPiece > const placedPieces = tile.getPlacedPieces();
				BOOST_FOREACH( PlacedPiece const piece, placedPieces )
				{
					if ( tile.isRoad( piece.getArea() ) )
					{
						PlacedRoad const roadPart( row, col, piece.getArea() );
						std::vector< PlacedRoad > const completeRoad = mBoard.getCompleteRoad( roadPart );
						// Go over the complete road to get all the information
						std::vector< PlacedPiece > allPieces;
						std::set< std::pair< int, int > > usedTiles;
						bool hasInn = false;
						BOOST_FOREACH( PlacedRoad const & road, completeRoad )
						{
							// Add the tile to usedTiles
							usedTiles.insert( std::make_pair( road.row, road.col ) );
							// Check if an inn is present
							hasInn = hasInn || mBoard.hasInn( road );
							// Remove and return all the pieces from this road
							std::vector< PlacedPiece > const pieces = mBoard.removePieces( road );
							returnPieces( pieces, road.row, road.col );
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner(s) and points of the road
						std::set< Color::Color > const winningColors = getWinningColors( allPieces );
						std::size_t const points = calculatePointsUnfinishedRoad( usedTiles.size(), hasInn );
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Model::Game::awardEndFieldPoints()
{
	for ( int row = mBoard.getTopRow(); row <= mBoard.getBottomRow(); ++row )
	{
		for ( int col = mBoard.getLeftCol(); col <= mBoard.getRightCol(); ++col )
		{
			if ( mBoard.isTile( row, col ) )
			{
				TileOnBoard const tile = *mBoard.getTile( row, col );
				std::vector< PlacedPiece > const placedPieces = tile.getPlacedPieces();
				BOOST_FOREACH( PlacedPiece const piece, placedPieces )
				{
					if ( tile.isField( piece.getArea() ) )
					{
						PlacedField const fieldPart( row, col, piece.getArea() );
						std::vector< PlacedField > const completeField = mBoard.getCompleteField( fieldPart );
						// Go over the complete field to get all the information
						std::vector< PlacedPiece > allPieces;
						std::set< PlacedCity > finishedCities;
						BOOST_FOREACH( PlacedField const & field, completeField )
						{
							std::vector< ContiguousCity > const cities = mBoard.getTile( field.row, field.col )->getCitiesPerField( field.area );
							// Add the identifier city part of every finished city to finishedCities
							BOOST_FOREACH( ContiguousCity const & city, cities )
							{
								PlacedCity const cityPart( field.row, field.col, city.front() );
								if ( mBoard.isFinishedCity( cityPart ) )
								{
									PlacedCity const id = mBoard.getIdentifierCity( cityPart );
									finishedCities.insert( id );
								}
							}
							// Remove and return all the pieces from this field
							std::vector< PlacedPiece > const pieces = mBoard.removePieces( field );
							returnPieces( pieces, field.row, field.col );
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner(s) and points of the field
						std::set< Color::Color > const winningColors = getWinningColors( allPieces );
						std::size_t const points = finishedCities.size() * kFarmerPPCity;
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Model::Game::awardPoints( std::set< Color::Color > const & inWinningColors, std::size_t inPoints )
{
	BOOST_FOREACH( Color::Color color, inWinningColors )
	{
		getPlayer( color ).awardPoints( inPoints );
	}
}

void
Model::Game::onFinishedCity( std::vector< PlacedCity > const & inCity )
{
	std::vector< PlacedPiece > allPieces;
	std::set< std::pair< int, int > > usedTiles;
	std::set< PlacedCity > allShields;
	bool hasCathedral = false;
	BOOST_FOREACH( PlacedCity const & city, inCity )
	{
		// Add to used tiles
		usedTiles.insert( std::make_pair( city.row, city.col ) );
		// Check if a cathedral is present
		hasCathedral = hasCathedral || mBoard.getTile( city.row, city.col )->getCenter() == Tile::kCenterCathedral;
		// Add shield
		std::vector< Area::Area > const shields = mBoard.getTile( city.row, city.col )->getShields();
		if ( std::find( shields.begin(), shields.end(), city.area ) != shields.end() )
		{
			allShields.insert( city );
		}
		// Remove and return all pieces from this PlacedCity
		std::vector< PlacedPiece > const pieces = mBoard.removePieces( city );
		returnPieces( pieces, city.row, city.col );
		// Add to allPieces
		allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
	}
	std::set< Color::Color > const winningColors = getWinningColors( allPieces );
	std::size_t const points = calculatePointsFinishedCity( usedTiles.size(), allShields.size(), hasCathedral );
	awardPoints( winningColors, points );
}

void
Model::Game::onFinishedRoad( std::vector< PlacedRoad > const & inRoad )
{
	std::vector< PlacedPiece > allPieces;
	std::set< std::pair< int, int > > usedTiles;
	bool hasInn = false;
	BOOST_FOREACH( PlacedRoad const & road, inRoad )
	{
		// Add to used tiles
		usedTiles.insert( std::make_pair( road.row, road.col ) );
		// Check if an inn is present
		hasInn = hasInn || mBoard.hasInn( road );
		// Remove and return all pieces from this PlacedRoad
		std::vector< PlacedPiece > const pieces = mBoard.removePieces( road );
		returnPieces( pieces, road.row, road.col );
		// Add to allPieces
		allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
	}
	std::set< Color::Color > const winningColors = getWinningColors( allPieces );
	std::size_t const points = calculatePointsFinishedRoad( usedTiles.size(), hasInn );
	awardPoints( winningColors, points );
}

void
Model::Game::onFinishedCloister( int inRow, int inCol )
{
	// Remove and return all pieces
	std::vector< PlacedPiece > const pieces = mBoard.removePieces( PlacedProject( inRow, inCol, Area::kCentral ) );
	returnPieces( pieces, inRow, inCol );
	std::set< Color::Color > const winningColors = getWinningColors( pieces );
	awardPoints( winningColors, kMonkPointsFinished );
}