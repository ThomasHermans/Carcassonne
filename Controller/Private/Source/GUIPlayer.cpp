#include "Controller/GUIPlayer.h"

#include "Model/Board.h"
#include "Model/Tile.h"
#include "Model/TileOnBoard.h"

#include "View/GameWindow.h"

#include "Utils/Location.h"

#include "ModelViewGlue.h"

Controller::GUIPlayer::GUIPlayer
(
	std::string const & inName,
	Model::Color::Color inColor,
	std::map< Model::Piece::PieceType, std::size_t > const & inMeepleSupply
)
:
	Player( inName, inColor, inMeepleSupply ),
	mWindow(),
	mPlacePieceConnections()
{
}

Controller::GUIPlayer::~GUIPlayer()
{
}

void
Controller::GUIPlayer::setWindow( std::shared_ptr< View::GameWindow > inWindow )
{
	mWindow = inWindow;
}

void
Controller::GUIPlayer::placeTile
(
	Model::Board const & /*inBoard*/,
	std::size_t /*inTilesLeft*/,
	Model::Tile const & /*inTile*/
)
{
	assert( mWindow );

	typedef boost::signals2::signal< void ( Utils::Location const &, std::string const &, View::Rotation ) > windowSignal;
	windowSignal::extended_slot_type extendedSlot = [ this ]
	(
		boost::signals2::connection const & inConnection,
		Utils::Location const & inLocation,
		std::string const & inTileId,
		View::Rotation inRotation
	)
	{
		inConnection.disconnect();
		assert( mWindow );
		mWindow->showMessage( std::string() );
		onTilePlaced( inLocation, inTileId, inRotation );
	};
	mWindow->tilePlaced.connect_extended( extendedSlot );

	mWindow->showMessage( "Place your tile." );
}

void
Controller::GUIPlayer::placePiece
(
	Model::Board const & inBoard,
	std::size_t inTilesLeft,
	Utils::Location const & inTile
)
{
	assert( mWindow );

	typedef boost::signals2::signal< void ( int, int, View::Meeple const & ) > windowPiecePlacedSignalType;
	windowPiecePlacedSignalType::slot_type piecePlacedSlot = [ this, &inBoard, inTilesLeft, inTile ]
	(
		int inX,
		int inY,
		View::Meeple const & inMeeple
	)
	{
		disconnectAllPlacePieceConnections();
		assert( mWindow );
		mWindow->showMessage( std::string() );
		if ( inTile == locationFromXY( inX, inY ) )
		{
			Model::Area::Area const area = areaFromPos( inX, inY );
			piecePlaced( Model::PlacedPiece( Model::Piece( modelFromView( inMeeple.getType() ), getColor() ), area ) );
		}
		else
		{
			placePiece( inBoard, inTilesLeft, inTile );
		}
	};
	mPlacePieceConnections.emplace_back( mWindow->piecePlaced.connect( piecePlacedSlot ) );

	typedef boost::signals2::signal< void () > windowNoPiecePlacedSignalType;
	windowNoPiecePlacedSignalType::slot_type noPiecePlacedSlot = [ this ]()
	{
		disconnectAllPlacePieceConnections();
		assert( mWindow );
		mWindow->showMessage( std::string() );
		piecePlaced( boost::none );
	};
	mPlacePieceConnections.emplace_back( mWindow->noPiecePlaced.connect( noPiecePlacedSlot ) );

	mWindow->showMessage( "Place your piece." );
}

void
Controller::GUIPlayer::onTilePlaced( Utils::Location const & inLocation, std::string const & /*inTileId*/, View::Rotation inRotation )
{
	Model::Rotation const rotation = modelFromView( inRotation );
	tilePlaced( TilePlacement( inLocation, rotation ) );
}

void
Controller::GUIPlayer::disconnectAllPlacePieceConnections()
{
	while ( !mPlacePieceConnections.empty() )
	{
		mPlacePieceConnections.begin()->disconnect();
		mPlacePieceConnections.erase( mPlacePieceConnections.begin() );
	}
}
