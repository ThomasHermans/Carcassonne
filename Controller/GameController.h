#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Model/Game.h"
#include "View/GameWindow.h"

#include <QObject>

namespace Dragging
{
	class PieceData;
}

namespace Controller
{
	class GameController : public QObject
	{
		Q_OBJECT
	public:
		explicit GameController( QObject * inParent = 0 );
		explicit GameController( std::string const & inTiles, QObject * inParent = 0 );
		explicit GameController( std::vector< Model::Player > const & inPlayer, QObject * inParent = 0 );

	private slots:
		// From model to view
		void onTilePlaced( unsigned inCol, unsigned inRow, std::string const & inId, Model::Rotation inRotation );
		void onTileUnplaced( unsigned inCol, unsigned inRow );
		void onTileRotated( unsigned inCol, unsigned inRow, std::string const & inId, Model::Rotation inRotation );
		void onNextTile( std::string const & inNextId );
		void onTilesLeft( unsigned inNr );

		void onPiecePlaced( unsigned inCol, unsigned inRow, Model::Area::Area inArea, Model::Player const & inCurrentPlayer );
		void onPieceReturned( unsigned inCol, unsigned inRow, Model::Area::Area inArea, Model::Player const & inCurrentPlayer );
		void onPlayerInfoChanged( Model::Player const & inNewInfo );
		void onCurrentPlayerChanged( Model::Player const & inCurrentPlayer);

		void onEndOfGame( unsigned inTilesLeft );

		// From view to model
		void onClicked( int inX, int inY );
		void onTileDropped( int inX, int inY, std::string const & inTileId, View::Rotation inRotation );
		void onTryToPlacePiece( Dragging::PieceData const & inData, int inX, int inY );

	private:
		void addPlayers();
		void makeConnections();
		void startGame();

	private:
		Model::Game *mGame;
		View::GameWindow *mWindow;
	};
}

#endif // GAMECONTROLLER_H
