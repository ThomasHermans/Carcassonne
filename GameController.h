#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "src-model/Game.h"
#include "src-view/GameWindow.h"

#include <QObject>

class DragData;

class GameController : public QObject
{
	Q_OBJECT
public:
	explicit GameController(QObject *parent = 0);

private slots:
	// From model to view
	void onTilePlaced(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
	void onTileUnplaced(unsigned int inCol, unsigned int inRow);
	void onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot);
	void onNextTile(std::string inNextId);
	void onTilesLeft(unsigned int inNr);

	void onPiecePlaced( unsigned int inCol, unsigned int inRow, Player const & inCurrentPlayer );
	void onPieceReturned( unsigned int inCol, unsigned int inRow, Player const & inCurrentPlayer );
	void onPlayerInfoChanged( Player const & inNewInfo );
	void onCurrentPlayerChanged(Player const & inCurrentPlayer);

	void onFinishedCloister(unsigned int inCol, unsigned int inRow);
	void onFinishedCity(std::vector< std::pair< unsigned int, unsigned int > > inTiles);
	void onFinishedRoad(std::vector< std::pair< unsigned int, unsigned int > > inTiles);

	void onEndOfGame(unsigned int inTilesLeft);

	// From view to model
	void onClicked( int inX, int inY );
	void onTryToPlacePiece( DragData const & inData, int inX, int inY );

private:
	Game *mGame;
	GameWindow *mWindow;
	
};

#endif // GAMECONTROLLER_H
