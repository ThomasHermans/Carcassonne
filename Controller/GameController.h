#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Model/Game.h"
#include "Model/ModelPlayer.h"

#include "View/GameWindow.h"
#include "View/Typedefs.h"

#include "Utils/Location.h"
#include "Utils/Typedefs.h"

#ifndef Q_MOC_RUN
#include <boost/scoped_ptr.hpp>
#endif

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
			std::set< Utils::Expansion::Type > const & inExpansions,
			std::vector< View::PlayerInfo > const & inPlayer,
			QObject * inParent = 0
		);

	private slots:
		// From model to view
		void onTilePlaced( Utils::Location const & inLocation, std::string const & inId, Model::Rotation inRotation );
		void onTileRemoved( Utils::Location const & inLocation );
		void onNextTile( std::string const & inNextId );
		void onTilesLeft( std::size_t inNr );

		void onPiecePlaced( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece );
		void onPieceRemoved( Utils::Location const & inLocation, Model::PlacedPiece const & inPiece );
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
