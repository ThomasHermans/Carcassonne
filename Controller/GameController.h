#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Model/Game.h"
#include "Model/Player.h"

#include "View/GameWindow.h"
#include "View/Typedefs.h"

#include "Utils/Typedefs.h"

#include <boost/scoped_ptr.hpp>

#include <QObject>

#include <set>
#include <string>
#include <vector>

namespace Dragging
{
	class PieceData;
}

namespace View
{
	struct PlayerInfo;
}

namespace Controller
{
	class GameController : public QObject
	{
		Q_OBJECT
	public:
		explicit GameController( std::string const & inTiles, QObject * inParent = 0 );
		explicit GameController
		(
			std::set< View::Expansion::Type > const & inExpansions,
			std::vector< View::PlayerInfo > const & inPlayer,
			QObject * inParent = 0
		);

	private slots:
		// From model to view
		void onTilePlaced( int inRow, int inCol, std::string const & inId, Model::Rotation inRotation );
		void onTileRemoved( int inRow, int inCol );
		void onNextTile( std::string const & inNextId );
		void onTilesLeft( std::size_t inNr );

		void onPiecePlaced( int inRow, int inCol, Model::PlacedPiece const & inPiece );
		void onPieceRemoved( int inRow, int inCol, Model::PlacedPiece const & inPiece );
		void onPlayerInfoChanged( Model::Player const & inNewInfo );
		void onCurrentPlayerChanged( Model::Player const & inCurrentPlayer );
		void onPossibleLocationsChanged( Utils::Locations const & inLocations );

		void onEndOfGame( std::size_t inTilesLeft );

		// From view to model
		void onClicked( int inX, int inY );
		void onTileDropped( int inX, int inY, std::string const & inTileId, View::Rotation inRotation );
		void onTryToPlacePiece( Dragging::PieceData const & inData, int inX, int inY );
		void onEndCurrentTurn();

	private:
		void addPlayersToWindow();
		void makeConnections();
		void startGame();

	private:
		std::vector< Model::Player > mPlayers;

		Model::Game mGame;
		boost::scoped_ptr< View::GameWindow > mWindow;
	};
}

#endif // GAMECONTROLLER_H
