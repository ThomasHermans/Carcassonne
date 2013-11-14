#include "Controller/GameController.h"

#include "Model/Color.h"
#include "View/Typedefs.h"
#include "View/DragData.h"

#include <cassert>

namespace
{
	unsigned
	colFromX( int inX, unsigned inStartCol )
	{
		return ( inX / Gui::kTileWidth + inStartCol - ( inX < 0 ? 1 : 0 ) );
	}

	unsigned
	rowFromY( int inY, unsigned inStartRow )
	{
		return ( inY / Gui::kTileHeight + inStartRow - ( inY < 0 ? 1 : 0 ) );
	}

	int
	xFromCol( unsigned inCol, unsigned inStartCol )
	{
		return ( ((int)inCol - (int)inStartCol) * Gui::kTileWidth );
	}

	int
	yFromRow( unsigned inRow, unsigned inStartRow )
	{
		return ( ((int)inRow - (int)inStartRow) * Gui::kTileHeight );
	}

	int
	xFromArea( Area::Area inArea )
	{
		switch ( inArea )
		{
			case Area::LeftTop:
			case Area::Left:
			case Area::LeftBottom:
				return .15 * Gui::kTileWidth;
			case Area::TopLeft:
			case Area::BottomLeft:
				return .3 * Gui::kTileWidth;
			case Area::Top:
			case Area::Central:
			case Area::Bottom:
				return .5 * Gui::kTileWidth;
			case Area::TopRight:
			case Area::BottomRight:
				return .7 * Gui::kTileWidth;
			case Area::RightTop:
			case Area::Right:
			case Area::RightBottom:
				return .85 * Gui::kTileWidth;
		}
		assert( !"Invalid Area" );
		return .5 * Gui::kTileWidth;
	}

	int
	yFromArea( Area::Area inArea )
	{
		switch ( inArea )
		{
			case Area::TopLeft:
			case Area::Top:
			case Area::TopRight:
				return .15 * Gui::kTileHeight;
			case Area::LeftTop:
			case Area::RightTop:
				return .3 * Gui::kTileHeight;
			case Area::Left:
			case Area::Central:
			case Area::Right:
				return .5 * Gui::kTileHeight;
			case Area::LeftBottom:
			case Area::RightBottom:
				return .7 * Gui::kTileHeight;
			case Area::BottomLeft:
			case Area::Bottom:
			case Area::BottomRight:
				return .85 * Gui::kTileHeight;
		}
		assert( !"Invalid Area" );
		return .5 * Gui::kTileHeight;
	}

	int
	posXFromX( int inX )
	{
		int res = ( inX % Gui::kTileWidth );
		if ( res < 0 )
		{
			res += Gui::kTileWidth;
		}
		return res;
	}

	int
	posYFromY( int inY )
	{
		int res = ( inY % Gui::kTileHeight );
		if ( res < 0 )
		{
			res += Gui::kTileHeight;
		}
		return res;
	}

	Area::Area
	areaFromPos( int inX, int inY )
	{
		using namespace Gui;
		if ( inX < kFirstBorder )
		{
			if ( inY < kFirstBorder )
			{
				if ( inY < inX )
					return Area::TopLeft;
				else
					return Area::LeftTop;
			}
			else if ( kSecondBorder < inY )
			{
				if ( kTileHeight - inY < inX )
					return Area::BottomLeft;
				else
					return Area::LeftBottom;
			}
			else if ( kFirstBorder < inY && inY < kSecondBorder )
				return Area::Left;
		}
		else if ( kSecondBorder < inX )
		{
			if ( inY < kFirstBorder )
			{
				if ( inY < kTileWidth - inX )
					return Area::TopRight;
				else
					return Area::RightTop;
			}
			else if ( kSecondBorder < inY )
			{
				if ( kTileHeight - inY < kTileWidth - inX )
					return Area::BottomRight;
				else
					return Area::RightBottom;
			}
			else if ( kFirstBorder < inY && inY < kSecondBorder )
				return Area::Right;
		}
		else if ( kFirstBorder < inX && inX < kSecondBorder )
		{
			if ( inY < kFirstBorder )
				return Area::Top;
			else if ( kSecondBorder < inY )
				return Area::Bottom;
			if ( kFirstBorder < inY && inY < kSecondBorder )
				return Area::Central;
		}
		assert( !"Invalid Area" );
		return Area::Central;
	}

	Color::Color
	modelFromView( Dragging::Color inColor )
	{
		switch ( inColor )
		{
			case Dragging::kRed:
				return Color::kRed;
			case Dragging::kGreen:
				return Color::kGreen;
			case Dragging::kBlue:
				return Color::kBlue;
			case Dragging::kYellow:
				return Color::kYellow;
			case Dragging::kBlack:
				return Color::kBlack;
			case Dragging::kGray:
				return Color::kGray;
		}
		assert( !"Invalid Dragging Color" );
		return Color::kRed;
	}

	Dragging::Color
	viewFromModel( Color::Color inColor )
	{
		switch ( inColor )
		{
			case Color::kRed:
				return Dragging::kRed;
			case Color::kGreen:
				return Dragging::kGreen;
			case Color::kBlue:
				return Dragging::kBlue;
			case Color::kYellow:
				return Dragging::kYellow;
			case Color::kBlack:
				return Dragging::kBlack;
			case Color::kGray:
				return Dragging::kGray;
		}
		assert( !"Invalid Color Color" );
		return Dragging::kRed;
	}

	Piece::PieceType
	modelFromView( Dragging::Piece inPiece )
	{
		switch ( inPiece )
		{
			case Dragging::kFollower:
				return Piece::kFollower;
			case Dragging::kLargeFollower:
				return Piece::kLargeFollower;
			case Dragging::kBuilder:
				return Piece::kBuilder;
			case Dragging::kPig:
				return Piece::kPig;
		}
		assert( !"Invalid Dragging Piece" );
		return Piece::kFollower;
	}

	QColor
	toQColor( Color::Color inColor )
	{
		switch ( inColor )
		{
			case Color::kRed:
				return Qt::red;
			case Color::kGreen:
				return Qt::green;
			case Color::kBlue:
				return Qt::blue;
			case Color::kYellow:
				return Qt::yellow;
			case Color::kBlack:
				return Qt::black;
			case Color::kGray:
				return Qt::gray;
		}
		assert( !"Invalid Color Color" );
		return Qt::white;
	}
}

GameController::GameController( QObject *parent )
:
	QObject( parent ),
	mGame( new Game( this ) ),
	mWindow( new GameWindow() )
{
	addPlayers();
	makeConnections();
	startGame();
}

GameController::GameController( std::string const & inTiles, QObject * inParent )
:
	QObject( inParent ),
	mGame( new Game( inTiles, this ) ),
	mWindow( new GameWindow() )
{
	addPlayers();
	makeConnections();
	startGame();
}

GameController::GameController( std::vector< Player > const & inPlayers, QObject * inParent )
:
	QObject( inParent ),
	mGame( new Game( inPlayers, this ) ),
	mWindow( new GameWindow() )
{
	addPlayers();
	makeConnections();
	startGame();
}

void
GameController::onTilePlaced
(
	unsigned inCol,
	unsigned inRow,
	std::string const & inId,
	TileOnBoard::Rotation inRot
)
{
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	mWindow->setTile( x, y, inId, inRot * 30 );
	mWindow->fadeNextTile();
}

void
GameController::onTileUnplaced
(
	unsigned inCol,
	unsigned inRow
)
{
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	mWindow->clearTile( x, y );
}

void
GameController::onTileRotated
(
	unsigned inCol,
	unsigned inRow,
	std::string const & inId,
	TileOnBoard::Rotation inRot
)
{
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	mWindow->rotateTile( x, y, inId, inRot * 30 );
}

void
GameController::onNextTile( std::string const & inNextId )
{
	mWindow->setNextTile( inNextId );
}

void
GameController::onTilesLeft( unsigned inNr )
{
	mWindow->displayTilesLeft(inNr);
}

void
GameController::onPiecePlaced( unsigned inCol, unsigned inRow, Area::Area inArea, Player const & inCurrentPlayer )
{
	std::cout << inCurrentPlayer.getName() << " placed a piece." << std::endl;
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	x += xFromArea( inArea ) - .5 * Gui::kTileWidth;
	y += yFromArea( inArea ) - .5 * Gui::kTileHeight;
	mWindow->placePiece( x, y, toQColor( inCurrentPlayer.getColor() ) );
}

void
GameController::onPieceReturned( unsigned inCol, unsigned inRow, Area::Area inArea, Player const & inPlayer )
{
	std::cout << inPlayer.getName() << " got a piece back." << std::endl;
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	x += xFromArea( inArea ) - .5 * Gui::kTileWidth;
	y += yFromArea( inArea ) - .5 * Gui::kTileHeight;
	mWindow->returnPiece( x, y, toQColor( inPlayer.getColor() ) );
}

void
GameController::onPlayerInfoChanged( Player const & inNewInfo )
{
	mWindow->setFollowersLeft( inNewInfo.getName(), inNewInfo.getNumberOfFreePieces() );
	mWindow->setScore( inNewInfo.getName(), inNewInfo.getScore() );
}

void
GameController::onCurrentPlayerChanged( Player const & inCurrentPlayer )
{
	mWindow->setActivePlayer( inCurrentPlayer.getName() );
}

void
GameController::onFinishedCloister( unsigned inCol, unsigned inRow )
{
	std::cout << "Finished cloister on tile " << inCol << ", " << inRow << std::endl;
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	mWindow->finishCloister( x, y );
}

void
GameController::onEndOfGame( unsigned inTilesLeft )
{
	mGame->calculateEndPoints();
	std::cout << "Game has ended." << std::endl;
	if ( inTilesLeft >= 1 )
	{
		std::cout << "Number of unplayed tiles: " << inTilesLeft << std::endl;
	}
}

void
GameController::onClicked( int inX, int inY )
{
	unsigned col = colFromX( inX, mGame->getStartCol() );
	unsigned row = rowFromY( inY, mGame->getStartRow() );
	std::cout << "GameController sees a click at x, y: " << inX << ", " << inY << ", which is col, row: " << col << ", " << row << std::endl;
	if ( col < mGame->getNrOfCols() && row < mGame->getNrOfRows() )
	{
		mGame->clickTile(col, row);
	}
}

void
GameController::onTryToPlacePiece( DragData const & inData, int inX, int inY )
{
	unsigned col = colFromX( inX, mGame->getStartCol() );
	unsigned row = rowFromY( inY, mGame->getStartRow() );
	std::cout << "GameController sees drop at x, y: " << inX << ", " << inY << ", which is col, row: " << col << ", " << row << std::endl;
	std::cout << "Which is at " << posXFromX( inX ) << ", " << posYFromY( inY ) << " at that tile." << std::endl;
	std::cout << "Which is at Area::" << areaFromPos( posXFromX( inX ), posYFromY( inY ) ) << std::endl;
	std::cout << "Drop contains " << inData.getColor() << " " << inData.getPiece() << std::endl;
	if ( col < mGame->getNrOfCols() && row < mGame->getNrOfRows() )
	{
		// Decipher data
		Color::Color color = modelFromView( inData.getColor() );
		Piece::PieceType type = modelFromView( inData.getPiece() );
		Area::Area area = areaFromPos( posXFromX( inX ), posYFromY( inY ) );
		// Send to mGame
		mGame->tryToPlacePiece( color, type, col, row, area );
	}
}

void
GameController::addPlayers()
{
	for ( std::vector< Player >::const_iterator it = mGame->getPlayers().begin();
		it != mGame->getPlayers().end();
		++it )
	{
		mWindow->addPlayer( it->getName(), viewFromModel( it->getColor() ), it->getNumberOfFreePieces() );
	}
}

void
GameController::makeConnections()
{
	connect( mGame, SIGNAL( tilePlaced(unsigned int, unsigned int, std::string, TileOnBoard::Rotation) ),
			this, SLOT( onTilePlaced(unsigned int, unsigned int, std::string, TileOnBoard::Rotation) ) );
	connect( mGame, SIGNAL( tileUnplaced(unsigned int, unsigned int) ),
			this, SLOT( onTileUnplaced(unsigned int, unsigned int) ) );
	connect( mGame, SIGNAL( tileRotated(uint,uint,std::string,TileOnBoard::Rotation) ),
			this, SLOT( onTileRotated(uint,uint,std::string,TileOnBoard::Rotation) ) );
	connect( mGame, SIGNAL( nextTile(std::string) ), this, SLOT( onNextTile(std::string) ) );
	connect( mGame, SIGNAL( tilesLeft(uint) ), this, SLOT( onTilesLeft(uint) ) );

	connect( mGame, SIGNAL( piecePlaced(uint, uint, Area::Area, Player) ), this, SLOT( onPiecePlaced(uint, uint, Area::Area, Player ) ) );
	connect( mGame, SIGNAL( pieceReturned(uint, uint, Area::Area, Player) ), this, SLOT( onPieceReturned(uint, uint, Area::Area, Player ) ) );
	connect( mGame, SIGNAL( playerInfoChanged(Player) ), this, SLOT( onPlayerInfoChanged(Player) ) );
	connect( mGame, SIGNAL( currentPlayerChanged(Player) ), this, SLOT( onCurrentPlayerChanged(Player) ) );

	connect( mGame, SIGNAL( finishedCloister(uint, uint) ), this, SLOT( onFinishedCloister(uint, uint) ) );

	connect( mWindow, SIGNAL( clicked(int,int) ), this, SLOT( onClicked(int,int) ) );
	connect( mWindow, SIGNAL( tryToPlacePiece( DragData, int, int ) ), this, SLOT( onTryToPlacePiece( DragData, int, int ) ) );
	connect( mWindow, SIGNAL( endCurrentTurn() ), mGame, SLOT( onEndCurrentTurn() ) );

	connect( mGame, SIGNAL( endOfGame(uint) ), this, SLOT( onEndOfGame(uint) ) );
}

void
GameController::startGame()
{
	onCurrentPlayerChanged( mGame->getCurrentPlayer() );
	mWindow->show();
	if ( mGame->getNextTile() )
	{
		mGame->placeStartTileOnBoard();
	}
}
