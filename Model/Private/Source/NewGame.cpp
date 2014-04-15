#include "Model/NewGame.h"

#include "CreateBaseGameTiles.h"

#include <boost/foreach.hpp>

#include <cassert>
#include <map>
#include <utility>

namespace
{
	std::size_t const kBrigandPointsPerTileUnfinished = 1;
	std::size_t const kBrigandPointsPerTileFinished = 1;
	std::size_t const kFarmerPointsPerCity = 3;
	std::size_t const kKnightPointsPerTileFinished = 2;
	std::size_t const kKnightsPointsPerShieldFinished = 2;
	std::size_t const kKnightPointsPerTileUnfinished = 1;
	std::size_t const kKnightsPointsPerShieldUnfinished = 1;
	std::size_t const kMonkPointsPerTile = 1;
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
				counts.insert( std::make_pair( color, 1 ) );
			}
			else
			{
				++it->second;
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
}

Model::NewGame::NewGame( std::vector< Player > const & inPlayers )
:
	mBoard(),
	mBag( createBaseGameTiles() ),
	mNextTile(),
	mPlayers( inPlayers ),
	mCurrentPlayer( 0 ),
	mPiecesPlacedThisTurn( 0 ),
	mCurrentPlacedTile()
{
	initialize();
}

Model::NewGame::NewGame( std::vector< Player > const & inPlayers, std::string const & inTiles )
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
Model::NewGame::placeStartTileOnBoard()
{
	if ( mNextTile )
	{
		Rotation const rotation = kCw0; // TODO: get a random Rotation each time
		TileOnBoard const startTile( *mNextTile, rotation );
		bool const placed = mBoard.placeStartTile( startTile );
		if ( placed )
		{
			tilePlaced( 0, 0, startTile.getID(), startTile.getRotation() );
			pickNextTile();
		}
	}
}

void
Model::NewGame::tryToPlaceTile
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
				tileRemoved( earlierTile->row, earlierTile->col );
				if ( earlierTile->tile.hasPieces() )
				{
					mPiecesPlacedThisTurn = 0;
					returnPieces( earlierTile->tile.removeAllPieces(), earlierTile->row, earlierTile->col );
				}
			}
			mCurrentPlacedTile = PlacedTile( placedTile, inRow, inCol );
			tilePlaced( inRow, inCol, placedTile.getID(), placedTile.getRotation() );
		}
	}
}

void
Model::NewGame::rotateTile( int inRow, int inCol )
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
			tileRemoved( mCurrentPlacedTile->row, mCurrentPlacedTile->col );
			mCurrentPlacedTile = PlacedTile( rotatedTile, inRow, inCol );
			tilePlaced( mCurrentPlacedTile->row, mCurrentPlacedTile->col, mCurrentPlacedTile->tile.getID(), mCurrentPlacedTile->tile.getRotation() );
		}
	}
}

void
Model::NewGame::tryToPlacePiece
(
	NewPlacedProject const & inPlace,
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
		if ( player.getColor() == inColor && player.hasFreePieces() && mPiecesPlacedThisTurn == 0 )
		{
			// You can only place a piece in an unoccupied area
			if ( !isOccupiedOnCurrentTile( inPlace.area ) )
			{
				PlacedPiece const placedPiece( player.getPieceToPlace(), inPlace.area );
				if ( mCurrentPlacedTile->tile.placePiece( placedPiece ) )
				{
					++mPiecesPlacedThisTurn;
					piecePlaced( mCurrentPlacedTile->row, mCurrentPlacedTile->col, placedPiece, player );
				}
				else
				{
					player.returnPiece( placedPiece.getPiece() );
				}
			}
		}
	}
}

void
Model::NewGame::endTurn()
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
Model::NewGame::awardEndPoints()
{
	awardEndCloisterPoints();
	awardEndCityPoints();
	awardEndRoadPoints();
	awardEndFieldPoints();
}

void
Model::NewGame::initialize()
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
	mBoard.finishedCity.connect( boost::bind( &Model::NewGame::onFinishedCity, this, _1 ) );
	mBoard.finishedRoad.connect( boost::bind( &Model::NewGame::onFinishedRoad, this, _1 ) );
	mBoard.finishedCloister.connect( boost::bind( &Model::NewGame::onFinishedCloister, this, _1, _2 ) );
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
Model::NewGame::pickNextTile()
{
	std::vector< Tile >::iterator it = mBag.end();
	bool foundPossible = false;
	while ( !foundPossible && it != mBag.begin() )
	{
		--it;
		Tile const possibleNextTile = *it;
		if ( mBoard.isPossibleTile( possibleNextTile ) )
		{
			mNextTile = possibleNextTile;
			nextTile( mNextTile->getID() );
			mBag.erase( it );
			foundPossible = true;
		}
	}
	tilesLeft( mBag.size() );
	if ( !foundPossible )
	{
		mNextTile = boost::none;
		endOfGame( mBag.size() );
	}
}

bool
Model::NewGame::isEmptySpot( int inRow, int inCol ) const
{
	return ( !isCurrentSpot( inRow, inCol ) && !mBoard.isTile( inRow, inCol ) );
}

bool
Model::NewGame::isCurrentSpot( int inRow, int inCol ) const
{
	return ( mCurrentPlacedTile && mCurrentPlacedTile->row == inRow && mCurrentPlacedTile->col == inCol );
}

void
Model::NewGame::returnPieces( std::vector< PlacedPiece > const & inPieces, int inRow, int inCol )
{
	BOOST_FOREACH( PlacedPiece const & piece, inPieces )
	{
		Player & player = getPlayer( piece.getPiece().getColor() );
		player.returnPiece( piece.getPiece() );
		pieceRemoved( inRow, inCol, piece, player );
	}
}

Model::Player &
Model::NewGame::getPlayer( Color::Color inColor )
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
Model::NewGame::isOccupiedOnCurrentTile( Area::Area inArea ) const
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
		std::vector< NewPlacedRoad > roadsToCheck;
		BOOST_FOREACH( Area::Area area, contRoad )
		{
			if ( mCurrentPlacedTile->tile.hasPiece( area ) )
			{
				return true;
			}
			NewPlacedRoad const thisRoad( mCurrentPlacedTile->row, mCurrentPlacedTile->col, area );
			roadsToCheck.push_back( getNeighbor( thisRoad ) );
		}
		BOOST_FOREACH( NewPlacedRoad road, roadsToCheck )
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
		std::vector< NewPlacedCity > citiesToCheck;
		BOOST_FOREACH( Area::Area area, contCity )
		{
			if ( mCurrentPlacedTile->tile.hasPiece( area ) )
			{
				return true;
			}
			NewPlacedCity const thisCity( mCurrentPlacedTile->row, mCurrentPlacedTile->col, area );
			citiesToCheck.push_back( getNeighbor( thisCity ) );
		}
		BOOST_FOREACH( NewPlacedCity city, citiesToCheck )
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
		std::vector< NewPlacedField > fieldsToCheck;
		BOOST_FOREACH( Area::Area area, contField )
		{
			if ( mCurrentPlacedTile->tile.hasPiece( area ) )
			{
				return true;
			}
			NewPlacedField const thisField( mCurrentPlacedTile->row, mCurrentPlacedTile->col, area );
			fieldsToCheck.push_back( getNeighbor( thisField ) );
		}
		BOOST_FOREACH( NewPlacedField field, fieldsToCheck )
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
Model::NewGame::awardEndCloisterPoints()
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
						NewPlacedProject const project( row, col, piece.getArea() );
						std::vector< PlacedPiece > const pieces = mBoard.removePieces( project );
						returnPieces( pieces, row, col );
						// Calculate winner(s) and points of the cloister
						std::set< Color::Color > const winningColors = getWinningColors( pieces );
						std::size_t const points = mBoard.getNrOfSurroundingTiles( row, col ) * kMonkPointsPerTile;
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Model::NewGame::awardEndCityPoints()
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
						NewPlacedCity const cityPart( row, col, piece.getArea() );
						std::vector< NewPlacedCity > const completeCity = mBoard.getCompleteCity( cityPart );
						// Go over the complete city to get all the information
						std::vector< PlacedPiece > allPieces;
						std::set< std::pair< int, int > > usedTiles;
						std::set< NewPlacedCity > allShields;
						BOOST_FOREACH( NewPlacedCity const & city, completeCity )
						{
							// Add the tile to usedTiles
							usedTiles.insert( std::make_pair( city.row, city.col ) );
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
						std::size_t const points = usedTiles.size() * kKnightPointsPerTileUnfinished + allShields.size() * kKnightsPointsPerShieldUnfinished;
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Model::NewGame::awardEndRoadPoints()
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
						NewPlacedRoad const roadPart( row, col, piece.getArea() );
						std::vector< NewPlacedRoad > const completeRoad = mBoard.getCompleteRoad( roadPart );
						// Go over the complete road to get all the information
						std::vector< PlacedPiece > allPieces;
						std::set< std::pair< int, int > > usedTiles;
						BOOST_FOREACH( NewPlacedRoad const & road, completeRoad )
						{
							// Add the tile to usedTiles
							usedTiles.insert( std::make_pair( road.row, road.col ) );
							// Remove and return all the pieces from this road
							std::vector< PlacedPiece > const pieces = mBoard.removePieces( road );
							returnPieces( pieces, road.row, road.col );
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner(s) and points of the road
						std::set< Color::Color > const winningColors = getWinningColors( allPieces );
						std::size_t const points = usedTiles.size() * kBrigandPointsPerTileUnfinished;
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Model::NewGame::awardEndFieldPoints()
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
						NewPlacedField const fieldPart( row, col, piece.getArea() );
						std::vector< NewPlacedField > const completeField = mBoard.getCompleteField( fieldPart );
						// Go over the complete field to get all the information
						std::vector< PlacedPiece > allPieces;
						std::set< NewPlacedCity > finishedCities;
						BOOST_FOREACH( NewPlacedField const & field, completeField )
						{
							std::vector< ContiguousCity > const cities = mBoard.getTile( field.row, field.col )->getCitiesPerField( field.area );
							// Add the identifier city part of every finished city to finishedCities
							BOOST_FOREACH( ContiguousCity const & city, cities )
							{
								NewPlacedCity const cityPart( field.row, field.col, city.front() );
								if ( mBoard.isFinishedCity( cityPart ) )
								{
									NewPlacedCity const id = mBoard.getIdentifierCity( cityPart );
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
						std::size_t const points = finishedCities.size() * kFarmerPointsPerCity;
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Model::NewGame::awardPoints( std::set< Color::Color > const & inWinningColors, std::size_t inPoints )
{
	BOOST_FOREACH( Color::Color color, inWinningColors )
	{
		getPlayer( color ).awardPoints( inPoints );
	}
}

void
Model::NewGame::onFinishedCity( std::vector< NewPlacedCity > const & inCity )
{
	std::vector< PlacedPiece > allPieces;
	std::set< std::pair< int, int > > usedTiles;
	std::set< NewPlacedCity > allShields;
	BOOST_FOREACH( NewPlacedCity const & city, inCity )
	{
		// Add to used tiles
		usedTiles.insert( std::make_pair( city.row, city.col ) );
		// Add shield
		std::vector< Area::Area > const shields = mBoard.getTile( city.row, city.col )->getShields();
		if ( std::find( shields.begin(), shields.end(), city.area ) != shields.end() )
		{
			allShields.insert( city );
		}
		// Remove and return all pieces from this NewPlacedCity
		std::vector< PlacedPiece > const pieces = mBoard.removePieces( city );
		returnPieces( pieces, city.row, city.col );
		// Add to allPieces
		allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
	}
	std::set< Color::Color > const winningColors = getWinningColors( allPieces );
	std::size_t const points = usedTiles.size() * kKnightPointsPerTileFinished + allShields.size() * kKnightsPointsPerShieldFinished;
	awardPoints( winningColors, points );
}

void
Model::NewGame::onFinishedRoad( std::vector< NewPlacedRoad > const & inRoad )
{
	std::vector< PlacedPiece > allPieces;
	std::set< std::pair< int, int > > usedTiles;
	BOOST_FOREACH( NewPlacedRoad const & road, inRoad )
	{
		// Add to used tiles
		usedTiles.insert( std::make_pair( road.row, road.col ) );
		// Remove and return all pieces from this NewPlacedRoad
		std::vector< PlacedPiece > const pieces = mBoard.removePieces( road );
		returnPieces( pieces, road.row, road.col );
		// Add to allPieces
		allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
	}
	std::set< Color::Color > const winningColors = getWinningColors( allPieces );
	std::size_t const points = usedTiles.size() * kBrigandPointsPerTileFinished;
	awardPoints( winningColors, points );
}

void
Model::NewGame::onFinishedCloister( int inRow, int inCol )
{
	// Remove and return all pieces
	std::vector< PlacedPiece > const pieces = mBoard.removePieces( NewPlacedProject( inRow, inCol, Area::kCentral ) );
	returnPieces( pieces, inRow, inCol );
	std::set< Color::Color > const winningColors = getWinningColors( pieces );
	awardPoints( winningColors, kMonkPointsFinished );
}