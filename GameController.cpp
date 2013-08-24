#include "GameController.h"

#include "src-model/Color.h"
#include "src-view/DragData.h"
#include "src-view/Typedefs.h"

namespace
{
	unsigned int
	colFromX(int inX, unsigned int inStartCol)
	{
		return (inX / Gui::kTileWidth + inStartCol - (inX < 0 ? 1 : 0));
	}

	unsigned int
	rowFromY(int inY, unsigned int inStartRow)
	{
		return (inY / Gui::kTileHeight + inStartRow - (inY < 0 ? 1 : 0));
	}

	int
	xFromCol(unsigned int inCol, unsigned int inStartCol)
	{
		return ( ((int)inCol - (int)inStartCol) * Gui::kTileWidth );
	}

	int
	yFromRow(unsigned int inRow, unsigned int inStartRow)
	{
		return ( ((int)inRow - (int)inStartRow) * Gui::kTileHeight );
	}

	int
	xFromArea( Area::Area inArea )
	{
		using namespace Gui;
		switch ( inArea )
		{
			case Area::LeftTop:
			case Area::Left:
			case Area::LeftBottom:
				return .15 * kTileWidth;
			case Area::TopLeft:
			case Area::BottomLeft:
				return .3 * kTileWidth;
			case Area::Top:
			case Area::Central:
			case Area::Bottom:
				return .5 * kTileWidth;
			case Area::TopRight:
			case Area::BottomRight:
				return .7 * kTileWidth;
			case Area::RightTop:
			case Area::Right:
			case Area::RightBottom:
				return .85 * kTileWidth;
			default:
				return .5 * kTileWidth;
		}
	}

	int
	yFromArea( Area::Area inArea )
	{
		using namespace Gui;
		switch ( inArea )
		{
			case Area::TopLeft:
			case Area::Top:
			case Area::TopRight:
				return .15 * kTileHeight;
			case Area::LeftTop:
			case Area::RightTop:
				return .3 * kTileHeight;
			case Area::Left:
			case Area::Central:
			case Area::Right:
				return .5 * kTileHeight;
			case Area::LeftBottom:
			case Area::RightBottom:
				return .7 * kTileHeight;
			case Area::BottomLeft:
			case Area::Bottom:
			case Area::BottomRight:
				return .85 * kTileHeight;
			default:
				return .5 * kTileHeight;
		}
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
		return Area::Invalid;
	}

	Color::Color
	modelFromView( Dragging::Color inColor )
	{
		switch ( inColor )
		{
			case Dragging::kRed:
				return Color::Red;
			case Dragging::kGreen:
				return Color::Green;
			case Dragging::kBlue:
				return Color::Blue;
			case Dragging::kYellow:
				return Color::Yellow;
			case Dragging::kBlack:
				return Color::Black;
			case Dragging::kGray:
				return Color::Gray;
			default:
				return Color::Red;
		}
	}

	Dragging::Color
	viewFromModel( Color::Color inColor )
	{
		switch ( inColor )
		{
			case Color::Red:
				return Dragging::kRed;
			case Color::Green:
				return Dragging::kGreen;
			case Color::Blue:
				return Dragging::kBlue;
			case Color::Yellow:
				return Dragging::kYellow;
			case Color::Black:
				return Dragging::kBlack;
			case Color::Gray:
				return Dragging::kGray;
			default:
				return Dragging::kRed;
		}
	}

	Piece::PieceType
	modelFromView( Dragging::Piece inPiece )
	{
		switch ( inPiece )
		{
			case Dragging::kFollower:
				return Piece::Follower;
			case Dragging::kLargeFollower:
				return Piece::LargeFollower;
			case Dragging::kBuilder:
				return Piece::Builder;
			case Dragging::kPig:
				return Piece::Pig;
			default:
				return Piece::Follower;
		}
	}

	QColor
	toQColor( Color::Color inColor )
	{
		switch ( inColor )
		{
			case Color::Red:
				return Qt::red;
			case Color::Green:
				return Qt::green;
			case Color::Blue:
				return Qt::blue;
			case Color::Yellow:
				return Qt::yellow;
			case Color::Black:
				return Qt::black;
			case Color::Gray:
				return Qt::gray;
			default:
				return Qt::white;
		}
	}
}

GameController::GameController(QObject *parent) :
	QObject(parent),
	mGame(new Game()),
	mWindow(new GameWindow())
{
	for ( std::vector< Player >::const_iterator it = mGame->getPlayers().begin();
		it != mGame->getPlayers().end();
		++it )
	{
		mWindow->addPlayer( it->getName(), viewFromModel( it->getColor() ), it->getNumberOfFreePieces() );
	}
	connect( mGame, SIGNAL( tilePlaced(unsigned int, unsigned int, std::string, TileOnBoard::Rotation) ),
			this, SLOT( onTilePlaced(unsigned int, unsigned int, std::string, TileOnBoard::Rotation) ) );
	connect( mGame, SIGNAL( tileUnplaced(unsigned int, unsigned int) ),
			this, SLOT( onTileUnplaced(unsigned int, unsigned int) ) );
	connect( mGame, SIGNAL( tileRotated(uint,uint,std::string,TileOnBoard::Rotation) ),
			this, SLOT( onTileRotated(uint,uint,std::string,TileOnBoard::Rotation) ) );
	connect( mGame, SIGNAL( nextTile(std::string) ), this, SLOT( onNextTile(std::string) ) );
	connect( mGame, SIGNAL( tilesLeft(uint) ), this, SLOT( onTilesLeft(uint) ) );

	connect( mGame, SIGNAL( piecePlaced(uint, uint, Area::Area, Player) ), this, SLOT( onPiecePlaced(uint, uint, Area::Area, Player ) ) );
	connect( mGame, SIGNAL( pieceReturned(uint, uint, Player) ), this, SLOT( onPieceReturned(uint, uint, Player ) ) );
	connect( mGame, SIGNAL( playerInfoChanged(Player) ), this, SLOT( onPlayerInfoChanged(Player) ) );
	connect( mGame, SIGNAL( currentPlayerChanged(Player) ), this, SLOT( onCurrentPlayerChanged(Player) ) );

	connect( mGame, SIGNAL( finishedCloister(uint, uint) ), this, SLOT( onFinishedCloister(uint, uint) ) );
	connect( mGame, SIGNAL( finishedCity(std::vector<std::pair<uint,uint>>)), this, SLOT( onFinishedCity(std::vector<std::pair<uint,uint>>) ) );
	connect( mGame, SIGNAL( finishedRoad(std::vector<std::pair<uint,uint>>)), this, SLOT( onFinishedRoad(std::vector<std::pair<uint,uint>>) ) );

	connect( mWindow, SIGNAL( clicked(int,int) ), this, SLOT( onClicked(int,int) ) );
	connect( mWindow, SIGNAL( tryToPlacePiece( DragData, int, int ) ), this, SLOT( onTryToPlacePiece( DragData, int, int ) ) );
	connect( mWindow, SIGNAL( endCurrentTurn() ), mGame, SLOT( onEndCurrentTurn() ) );

	connect( mGame, SIGNAL( endOfGame(uint) ), this, SLOT( onEndOfGame(uint) ) );

	onCurrentPlayerChanged( mGame->getCurrentPlayer() );
	mWindow->show();
	if (mGame->getNextTile())
	{
		mGame->placeStartTileOnBoard();
	}
}

void
GameController::onTilePlaced(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	mWindow->setTile(x, y, inId, inRot * 30);
	mWindow->fadeNextTile();
}

void
GameController::onTileUnplaced(unsigned int inCol, unsigned int inRow)
{
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	mWindow->clearTile(x, y);
}

void
GameController::onTileRotated(unsigned int inCol, unsigned int inRow, std::string inId, TileOnBoard::Rotation inRot)
{
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	mWindow->rotateTile(x, y, inId, inRot * 30);
}

void
GameController::onNextTile( std::string inNextId )
{
	mWindow->setNextTile(inNextId);
}

void
GameController::onTilesLeft(unsigned int inNr)
{
	mWindow->displayTilesLeft(inNr);
}

void
GameController::onPiecePlaced( unsigned int inCol, unsigned int inRow, Area::Area inArea, Player const & inCurrentPlayer )
{
	std::cout << inCurrentPlayer.getName() << " placed a piece." << std::endl;
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	x += xFromArea( inArea ) - .5 * Gui::kTileWidth;
	y += yFromArea( inArea ) - .5 * Gui::kTileHeight;
	mWindow->placePiece( x, y, toQColor( inCurrentPlayer.getColor() ) );
}

void
GameController::onPieceReturned( unsigned int inCol, unsigned int inRow, Player const & inPlayer )
{
	std::cout << inPlayer.getName() << " got a piece back." << std::endl;
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
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
GameController::onFinishedCloister(unsigned int inCol, unsigned int inRow)
{
	std::cout << "Finished cloister on tile " << inCol << ", " << inRow << std::endl;
	int x = xFromCol( inCol, mGame->getStartCol() );
	int y = yFromRow( inRow, mGame->getStartRow() );
	mWindow->finishCloister( x, y );
}

void
GameController::onFinishedCity(std::vector< std::pair< unsigned int, unsigned int > > inTiles)
{
	if ( inTiles.empty() )
	{
		return;
	}
	std::cout << "Finished city on tiles ";
	unsigned int leftCol = inTiles[0].first;
	unsigned int rightCol = inTiles[0].first;
	unsigned int topRow = inTiles[0].second;
	unsigned int bottomRow = inTiles[0].second;
	for ( unsigned int i = 0; i < inTiles.size(); ++i )
	{
		std::cout << inTiles[i].first << ", " << inTiles[i].second << "; ";
		leftCol = std::min( leftCol, inTiles[i].first );
		rightCol = std::max( rightCol, inTiles[i].first );
		topRow = std::min( topRow, inTiles[i].second );
		bottomRow = std::max( bottomRow, inTiles[i].second );
	}
	std::cout << std::endl;
	++rightCol;
	++bottomRow;
	int left = xFromCol( leftCol, mGame->getStartCol() );
	int right = xFromCol( rightCol, mGame->getStartCol() );
	int top = yFromRow( topRow, mGame->getStartRow() );
	int bottom = yFromRow( bottomRow, mGame->getStartRow() );
	mWindow->finishCity( left, right, top, bottom );
}

void
GameController::onFinishedRoad(std::vector< std::pair< unsigned int, unsigned int > > inTiles)
{
	if ( inTiles.empty() )
	{
		return;
	}
	std::cout << "Finished road on tiles ";
	unsigned int leftCol = inTiles[0].first;
	unsigned int rightCol = inTiles[0].first;
	unsigned int topRow = inTiles[0].second;
	unsigned int bottomRow = inTiles[0].second;
	for ( unsigned int i = 0; i < inTiles.size(); ++i )
	{
		std::cout << inTiles[i].first << ", " << inTiles[i].second << "; ";
		leftCol = std::min( leftCol, inTiles[i].first );
		rightCol = std::max( rightCol, inTiles[i].first );
		topRow = std::min( topRow, inTiles[i].second );
		bottomRow = std::max( bottomRow, inTiles[i].second );
	}
	std::cout << std::endl;
	++rightCol;
	++bottomRow;
	int left = xFromCol( leftCol, mGame->getStartCol() );
	int right = xFromCol( rightCol, mGame->getStartCol() );
	int top = yFromRow( topRow, mGame->getStartRow() );
	int bottom = yFromRow( bottomRow, mGame->getStartRow() );
	mWindow->finishRoad( left, right, top, bottom );
}

void
GameController::onEndOfGame(unsigned int inTilesLeft)
{
	std::cout << "Game has ended." << std::endl;
	if ( inTilesLeft >= 1 )
	{
		std::cout << "Number of unplayed tiles: " << inTilesLeft << std::endl;
	}
}

void
GameController::onClicked( int inX, int inY )
{
	unsigned int col = colFromX( inX, mGame->getStartCol() );
	unsigned int row = rowFromY( inY, mGame->getStartRow() );
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