#include "Controller/Moderator.h"

#include "Controller/GUIPlayer.h"
#include "Controller/Player.h"
#include "Controller/RobotPlayer.h"

#include "Model/CreateTilesAndPieces.h"
#include "Model/Points.h"
#include "Model/Tile.h"

#include "Utils/Location.h"
#include "Utils/PrintUtils.h"

#include "View/GameWindow.h"
#include "View/StartScreen.h"

#include "ModelViewGlue.h"

#include <array>

namespace
{
	std::array< Model::Piece::PieceType, 2 > const kPieceTypes =
	{
		{
			Model::Piece::kFollower,
			Model::Piece::kLargeFollower
		}
	};

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

	std::vector< Controller::PlayerInfo >
	createPlayerInfos( std::vector< std::shared_ptr< Controller::Player > > const & inPlayers )
	{
		std::vector< Controller::PlayerInfo > playerInfos;
		for ( auto const & player : inPlayers )
		{
			playerInfos.emplace_back( createPlayerInfo( *player ) );
		}
		return playerInfos;
	}
}

Controller::Moderator::Moderator
(
	std::vector< View::PlayerInfo > const & inPlayers,
	std::set< Utils::Expansion::Type > const & inExpansions,
	std::shared_ptr< View::GameWindow > inGameWindow,
	QObject * inParent
)
:
	QObject( inParent ),
	mExpansionsInPlay( inExpansions ),
	mPlayers(),
	mCurrentPlayer(),
	mBoard(),
	mBag( Model::createTiles( inExpansions ) ),
	mNextTile( boost::none )
{
	std::cout << "Bag: " << Utils::printTiles( mBag ) << std::endl;
	std::map< Model::Piece::PieceType, std::size_t > meepleSupply = Model::createPieces( inExpansions );
	for ( View::PlayerInfo const & playerInfo : inPlayers )
	{
		std::shared_ptr< Player > player;
		if ( playerInfo.isAI )
		{
			player.reset( new RobotPlayer( playerInfo.name, modelFromView( playerInfo.color ), meepleSupply ) );
		}
		else
		{
			player.reset( new GUIPlayer( playerInfo.name, modelFromView( playerInfo.color ), meepleSupply ) );
		}
		player->setWindow( inGameWindow );
		player->setNumberOfPlayers( inPlayers.size() );
		mPlayers.emplace_back( player );

		inGameWindow->addPlayer( player->getName(), viewFromModel( player->getColor() ) );
		for ( auto pieceType : kPieceTypes )
		{
			inGameWindow->setPlayerSupply( player->getName(), viewFromModel( pieceType ), player->getSupply( pieceType ) );
		}

		Utils::PlayerID const id = player->getID();
		player->scoreChanged.connect( [ id, this ]( std::size_t inScore ){ emit playerScoreChanged( id, inScore ); } );
		player->meepleSupplyChanged.connect( [ id, this ]{ sendPlayerSupply( id ); } );
	}

	mBoard.getTilePlacedSignal().connect( boost::bind( &Moderator::sendTilePlaced, this, _1, _2 ) );
	mBoard.getPiecePlacedSignal().connect( boost::bind( &Moderator::sendPiecePlaced, this, _1, _2 ) );
	mBoard.getPieceRemovedSignal().connect( boost::bind( &Moderator::sendPieceRemoved, this, _1, _2 ) );
}

Model::Board const &
Controller::Moderator::getBoard() const
{
	return mBoard;
}

std::vector< std::shared_ptr< Controller::Player > >
Controller::Moderator::getGameWinners()
{
	std::vector< std::shared_ptr< Player > > winners;
	std::size_t maxScore = 0;
	for ( auto const & player : mPlayers )
	{
		if ( player->getScore() > maxScore )
		{
			winners.clear();
			maxScore = player->getScore();
		}
		if ( player->getScore() == maxScore )
		{
			winners.push_back( player );
		}
	}
	return winners;
}

std::string
Controller::Moderator::getPlayerName( Utils::PlayerID inPlayerID ) const
{
	auto playerIt = std::find_if
	(
		mPlayers.begin(), mPlayers.end(),
		[ inPlayerID ]( std::shared_ptr< Player > const & inPlayer ){ return inPlayer->getID() == inPlayerID; }
	);
	if ( playerIt != mPlayers.end() )
	{
		return (*playerIt)->getName();
	}
	else
	{
		return std::string();
	}
}

Model::Color::Color
Controller::Moderator::getPlayerColor( Utils::PlayerID inPlayerID ) const
{
	auto playerIt = std::find_if
	(
		mPlayers.begin(), mPlayers.end(),
		[ inPlayerID ]( std::shared_ptr< Player > const & inPlayer ){ return inPlayer->getID() == inPlayerID; }
	);
	if ( playerIt != mPlayers.end() )
	{
		return (*playerIt)->getColor();
	}
	else
	{
		assert( !"Could not find specified player" );
		return Model::Color::kRed;
	}
}

void
Controller::Moderator::playGame()
{
	assert( !mBag.empty() );
	Model::Tile const tile = mBag.back();
	mBag.pop_back();
	emit tilesLeftChanged( mBag.size() );

	Model::TileOnBoard const startTile( tile, Model::kCw0 );
	bool const placed = mBoard.placeStartTile( startTile );
	assert( placed );

	prepareNewTurn();
}

void
Controller::Moderator::pickNextTile()
{
	mNextTile = boost::none;
	std::vector< Model::Tile >::iterator it = mBag.end();
	while ( it != mBag.begin() )
	{
		--it;
		if ( mBoard.isPossibleTile( *it ) )
		{
			mNextTile = *it;
			emit nextTileChanged( mNextTile->getID() );
			mBag.erase( it );
			emit tilesLeftChanged( mBag.size() );
			return;
		}
	}
}

void
Controller::Moderator::switchToNextPlayer()
{
	if ( mCurrentPlayer )
	{
		auto it = std::find( mPlayers.begin(), mPlayers.end(), mCurrentPlayer );
		assert( it != mPlayers.end() );
		++it;
		if ( it == mPlayers.end() )
			it = mPlayers.begin();
		mCurrentPlayer = *it;
	}
	else
	{
		assert( !mPlayers.empty() );
		mCurrentPlayer = *mPlayers.begin();
	}
	emit currentPlayerChanged( mCurrentPlayer->getName() );
}

void
Controller::Moderator::prepareNewTurn()
{
	switchToNextPlayer();
	pickNextTile();

	if ( mNextTile )
	{
		requestTilePlacement();
	}
	else
	{
		endGame();
	}
}

void
Controller::Moderator::requestTilePlacement()
{
	// Ask current player for tile placement, until we receive a valid one.
	typedef boost::signals2::signal< void ( TilePlacement ) > tileSignal;
	tileSignal::extended_slot_type extendedSlot = boost::bind( &Moderator::onTilePlaced, this, _1, _2 );
	mCurrentPlayer->tilePlaced.connect_extended( extendedSlot );

	GameState currentGameState{ mBoard, mExpansionsInPlay, mBag.size(), createPlayerInfos( mPlayers ) };
	mCurrentPlayer->placeTile( currentGameState, *mNextTile );
}

void
Controller::Moderator::onTilePlaced
(
	boost::signals2::connection const & inConnection,
	TilePlacement const & inTilePlacement
)
{
	inConnection.disconnect();
	if ( isValidPlacement( *mNextTile, inTilePlacement ) )
	{
		Model::TileOnBoard const tileToPlace( *mNextTile, inTilePlacement.rotation );
		mBoard.placeValidTile( tileToPlace, inTilePlacement.location );

		requestPiecePlacement( inTilePlacement.location );
	}
	else
	{
		// TODO: give player a warning.
		requestTilePlacement();
	}
}

void
Controller::Moderator::requestPiecePlacement( Utils::Location const & inLocation )
{
	// Ask current player for piece placement, until we receive a valid one.
	typedef boost::signals2::signal< void ( boost::optional< Model::PlacedPiece > ) > pieceSignal;
	pieceSignal::extended_slot_type pieceSlot = boost::bind( &Moderator::onPiecePlaced, this, _1, inLocation, _2 );
	mCurrentPlayer->piecePlaced.connect_extended( pieceSlot );

	GameState currentGameState{ mBoard, mExpansionsInPlay, mBag.size(), createPlayerInfos( mPlayers ) };
	mCurrentPlayer->placePiece( currentGameState, inLocation );
}

void
Controller::Moderator::onPiecePlaced
(
	boost::signals2::connection const & inConnection,
	Utils::Location const & inLocation,
	boost::optional< Model::PlacedPiece > const & inPiecePlacement
)
{
	inConnection.disconnect();
	if ( inPiecePlacement )
	{
		if ( mCurrentPlayer->hasPiece( inPiecePlacement->getType() ) && isValidPlacement( inLocation, *inPiecePlacement ) )
		{
			mCurrentPlayer->takePiece( inPiecePlacement->getType() );
			mBoard.placeValidPiece( *inPiecePlacement, inLocation );
		}
		else
		{
			// TODO: give player a warning.
			// TODO: should player get a second chance?
		}
	}
	endTurn( inLocation );
}

void
Controller::Moderator::endTurn( Utils::Location const & inLocation )
{
	finishProjects( inLocation );

	prepareNewTurn();
}

void
Controller::Moderator::endGame()
{
	awardEndPoints();
}

std::shared_ptr< Controller::Player >
Controller::Moderator::getPlayer( Model::Color::Color inColor )
{
	for ( std::shared_ptr< Player > player : mPlayers )
	{
		if ( player->getColor() == inColor )
		{
			return player;
		}
	}
	assert( !"Color belongs to no player" );
	return std::shared_ptr< Player >();
}

std::shared_ptr< Controller::Player >
Controller::Moderator::getPlayer( Utils::PlayerID inPlayerID )
{
	for ( std::shared_ptr< Player > player : mPlayers )
	{
		if ( player->getID() == inPlayerID )
		{
			return player;
		}
	}
	assert( !"PlayerID belongs to no player" );
	return std::shared_ptr< Player >();
}

bool
Controller::Moderator::isValidPlacement
(
	Model::Tile const & inTile,
	TilePlacement const & inPlacement
) const
{
	Model::TileOnBoard const tile( inTile, inPlacement.rotation );
	return mBoard.isValidTilePlacement( tile, inPlacement.location );
}

bool
Controller::Moderator::isValidPlacement
(
	Utils::Location const & inLocation,
	Model::PlacedPiece const & inPiece
) const
{
	return mBoard.isValidPiecePlacement( inLocation, inPiece );
}

void
Controller::Moderator::finishProjects( Utils::Location const & inLocation )
{
	// Check all roads on tile
	finishRoads( inLocation );
	// Check all cities on tile
	finishCities( inLocation );
	// Check all cloisters on tile and on neighboring tiles
	finishCloisters( inLocation );
}

void
Controller::Moderator::finishRoads( Utils::Location const & inLocation )
{
	boost::optional< Model::TileOnBoard > const tileOnBoard = mBoard.getTile( inLocation );
	assert( tileOnBoard );
	for ( auto const & road : tileOnBoard->getContiguousRoads() )
	{
		Model::PlacedRoad const roadOnTile( inLocation.row, inLocation.col, road.front() );
		if ( mBoard.isFinishedRoad( roadOnTile ) )
		{
			// Go over all the tiles from this road, returning all pieces and remembering them to
			// determine the winner.
			std::vector< Model::PlacedPiece > allPieces;
			for ( auto const & roadPiece : mBoard.getCompleteRoad( roadOnTile ) )
			{
				// Remove and return all pieces from this PlacedRoad.
				std::vector< Model::PlacedPiece > const pieces = mBoard.removePieces( roadPiece );
				returnPieces( pieces );
				// Add pieces to all pieces.
				allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
			}
			if ( !allPieces.empty() )
			{
				std::set< Model::Color::Color > const winningColors = getWinningColors( allPieces );
				std::size_t const points = getPointsForRoad( mBoard, inLocation, roadOnTile.area );
				awardPoints( winningColors, points );
			}
		}
	}
}

void
Controller::Moderator::finishCities( Utils::Location const & inLocation )
{
	boost::optional< Model::TileOnBoard > & tileOnBoard = mBoard.getTile( inLocation );
	assert( tileOnBoard );
	for ( auto const & city : tileOnBoard->getContiguousCities() )
	{
		Model::PlacedCity const cityOnTile( inLocation.row, inLocation.col, city.front() );
		if ( mBoard.isFinishedCity( cityOnTile ) )
		{
			// Go over all the tiles from this city, returning all pieces and remembering them to
			// determine the winner.
			std::vector< Model::PlacedPiece > allPieces;
			for ( auto const & cityPiece : mBoard.getCompleteCity( cityOnTile ) )
			{
				// Remove and return all pieces from this PlacedCity.
				std::vector< Model::PlacedPiece > const pieces = mBoard.removePieces( cityPiece );
				returnPieces( pieces );
				// Add pieces to all pieces.
				allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
			}
			if ( !allPieces.empty() )
			{
				std::set< Model::Color::Color > const winningColors = getWinningColors( allPieces );
				std::size_t const points = getPointsForCity( mBoard, inLocation, cityOnTile.area );
				awardPoints( winningColors, points );
			}
		}
	}
}

void
Controller::Moderator::finishCloisters( Utils::Location const & inLocation )
{
	for ( int row = inLocation.row - 1; row <= inLocation.row + 1; ++row )
	{
		for ( int col = inLocation.col - 1; col <= inLocation.col + 1; ++col )
		{
			if ( mBoard.isFinishedCloister( row, col ) )
			{
				Model::PlacedProject const cloister( row, col, Model::Area::kCentral );
				std::vector< Model::PlacedPiece > const pieces = mBoard.removePieces( cloister );
				returnPieces( pieces );
				std::set< Model::Color::Color > const winningColors = getWinningColors( pieces );
				awardPoints( winningColors, getPointsForCloister( mBoard, {row,col}, Model::Area::kCentral ) );
			}
		}
	}
}

void
Controller::Moderator::awardEndPoints()
{
	awardEndRoadPoints();
	awardEndCityPoints();
	awardEndCloisterPoints();
	awardEndFarmPoints();
}

void
Controller::Moderator::awardEndRoadPoints()
{
	for ( int row = mBoard.getTopRow(); row <= mBoard.getBottomRow(); ++row )
	{
		for ( int col = mBoard.getLeftCol(); col <= mBoard.getRightCol(); ++col )
		{
			if ( mBoard.isTile( row, col ) )
			{
				Model::TileOnBoard const tile = *mBoard.getTile( row, col );
				for ( Model::PlacedPiece const piece : tile.getPlacedPieces() )
				{
					if ( tile.isRoad( piece.getArea() ) )
					{
						Model::PlacedRoad const roadPart( row, col, piece.getArea() );
						std::vector< Model::PlacedRoad > const completeRoad = mBoard.getCompleteRoad( roadPart );
						// Get all information from this road.
						std::vector< Model::PlacedPiece > allPieces;
						for ( Model::PlacedRoad const & road : completeRoad )
						{
							std::vector< Model::PlacedPiece > const pieces = mBoard.removePieces( road );
							returnPieces( pieces );
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner(s) and points of this road.
						std::set< Model::Color::Color > const winningColors = getWinningColors( allPieces );
						std::size_t const points = getPointsForRoad( mBoard, Utils::Location( row, col ), piece.getArea() );
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Controller::Moderator::awardEndCityPoints()
{
	for ( int row = mBoard.getTopRow(); row <= mBoard.getBottomRow(); ++row )
	{
		for ( int col = mBoard.getLeftCol(); col <= mBoard.getRightCol(); ++col )
		{
			if ( mBoard.isTile( row, col ) )
			{
				Model::TileOnBoard const tile = *mBoard.getTile( row, col );
				for ( Model::PlacedPiece const piece : tile.getPlacedPieces() )
				{
					if ( tile.isCity( piece.getArea() ) )
					{
						Model::PlacedCity const cityPart( row, col, piece.getArea() );
						std::vector< Model::PlacedCity > const completeCity = mBoard.getCompleteCity( cityPart );
						// Get all information from this city.
						std::vector< Model::PlacedPiece > allPieces;
						for ( Model::PlacedCity const & city : completeCity )
						{
							// Remove and return all pieces from this PlacedCity.
							std::vector< Model::PlacedPiece > const pieces = mBoard.removePieces( city );
							returnPieces( pieces );
							// Add pieces to all pieces.
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner(s) and points of this city.
						std::set< Model::Color::Color > const winningColors = getWinningColors( allPieces );
						std::size_t const points = getPointsForCity( mBoard, Utils::Location( row, col ), piece.getArea() );
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Controller::Moderator::awardEndCloisterPoints()
{
	for ( int row = mBoard.getTopRow(); row <= mBoard.getBottomRow(); ++row )
	{
		for ( int col = mBoard.getLeftCol(); col <= mBoard.getRightCol(); ++col )
		{
			if ( mBoard.isTile( row, col ) )
			{
				Model::TileOnBoard const tile = *mBoard.getTile( row, col );
				std::vector< Model::PlacedPiece > const placedPieces = tile.getPlacedPieces();
				for ( Model::PlacedPiece const piece : placedPieces )
				{
					if ( tile.isCloister( piece.getArea() ) )
					{
						// Remove and return all pieces from the cloister
						Model::PlacedProject const project( row, col, piece.getArea() );
						std::vector< Model::PlacedPiece > const pieces = mBoard.removePieces( project );
						returnPieces( pieces );
						// Calculate winner(s) and points of the cloister
						std::set< Model::Color::Color > const winningColors = getWinningColors( pieces );
						std::size_t const points = getPointsForCloister( mBoard, Utils::Location( row, col ), piece.getArea() );
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Controller::Moderator::awardEndFarmPoints()
{
	for ( int row = mBoard.getTopRow(); row <= mBoard.getBottomRow(); ++row )
	{
		for ( int col = mBoard.getLeftCol(); col <= mBoard.getRightCol(); ++col )
		{
			if ( mBoard.isTile( row, col ) )
			{
				Model::TileOnBoard const tile = *mBoard.getTile( row, col );
				std::vector< Model::PlacedPiece > const placedPieces = tile.getPlacedPieces();
				for ( Model::PlacedPiece const piece : placedPieces )
				{
					if ( tile.isField( piece.getArea() ) )
					{
						Model::PlacedField const fieldPart( row, col, piece.getArea() );
						std::vector< Model::PlacedPiece > allPieces;
						std::vector< Model::PlacedField > const completeField = mBoard.getCompleteField( fieldPart );
						for ( Model::PlacedField const & field : completeField )
						{
							// Remove and return all the pieces from this field
							std::vector< Model::PlacedPiece > const pieces = mBoard.removePieces( field );
							returnPieces( pieces );
							allPieces.insert( allPieces.end(), pieces.begin(), pieces.end() );
						}
						// Calculate winner(s) and points of the field
						std::set< Model::Color::Color > const winningColors = getWinningColors( allPieces );
						std::size_t const points = getPointsForField( mBoard, Utils::Location( row, col ), piece.getArea() );
						// Award the points to the winners
						awardPoints( winningColors, points );
					}
				}
			}
		}
	}
}

void
Controller::Moderator::returnPieces( std::vector< Model::PlacedPiece > const & inPieces )
{
	for ( Model::PlacedPiece const & piece : inPieces )
	{
		std::shared_ptr< Player > player = getPlayer( piece.getColor() );
		player->returnPiece( piece.getPiece() );
	}
}

void
Controller::Moderator::awardPoints
(
	std::set< Model::Color::Color > const & inWinningColors,
	std::size_t inPoints
)
{
	for ( Model::Color::Color color : inWinningColors )
	{
		getPlayer( color )->awardPoints( inPoints );
	}
}

void
Controller::Moderator::sendTilePlaced
(
	Utils::Location const & inLocation,
	Model::TileOnBoard const & inTile
)
{
	if ( mCurrentPlayer )
	{
		emit tilePlaced( inLocation, inTile, mCurrentPlayer->getID() );
	}
	else
	{
		emit tilePlaced( inLocation, inTile, boost::none );
	}
}

void
Controller::Moderator::sendPiecePlaced
(
	Utils::Location const & inLocation,
	Model::PlacedPiece const & inPiece
)
{
	emit piecePlaced( inLocation, inPiece );
}


void
Controller::Moderator::sendPieceRemoved
(
	Utils::Location const & inLocation,
	Model::PlacedPiece const & inPiece
)
{
	emit pieceRemoved( inLocation, inPiece );
}

void
Controller::Moderator::sendPlayerSupply( Utils::PlayerID inPlayerID )
{
	for ( auto type : kPieceTypes )
	{
		emit playerSupplyChanged( inPlayerID, type, getPlayer( inPlayerID )->getSupply( type ) );
	}
}