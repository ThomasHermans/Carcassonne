#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Model/Game.h"
#include "View/GameWindow.h"

#include <QObject>

namespace Dragging
{
	class PieceData;
}

class GameController : public QObject
{
	Q_OBJECT
public:
	explicit GameController( QObject * inParent = 0 );
	explicit GameController( std::string const & inTiles, QObject * inParent = 0 );
	explicit GameController( std::vector< Player > const & inPlayer, QObject * inParent = 0 );

private slots:
	// From model to view
	void onTilePlaced( unsigned inCol, unsigned inRow, std::string const & inId, TileOnBoard::Rotation inRot );
	void onTileUnplaced( unsigned inCol, unsigned inRow );
	void onTileRotated( unsigned inCol, unsigned inRow, std::string const & inId, TileOnBoard::Rotation inRot );
	void onNextTile( std::string const & inNextId );
	void onTilesLeft( unsigned inNr );

	void onPiecePlaced( unsigned inCol, unsigned inRow, Area::Area inArea, Player const & inCurrentPlayer );
	void onPieceReturned( unsigned inCol, unsigned inRow, Area::Area inArea, Player const & inCurrentPlayer );
	void onPlayerInfoChanged( Player const & inNewInfo );
	void onCurrentPlayerChanged( Player const & inCurrentPlayer);

	void onFinishedCloister( unsigned inCol, unsigned inRow );

	void onEndOfGame( unsigned inTilesLeft );

	// From view to model
	void onClicked( int inX, int inY );
	void onTryToPlacePiece( Dragging::PieceData const & inData, int inX, int inY );

private:
	void addPlayers();
	void makeConnections();
	void startGame();

private:
	Game *mGame;
	GameWindow *mWindow;
	
};

#endif // GAMECONTROLLER_H
