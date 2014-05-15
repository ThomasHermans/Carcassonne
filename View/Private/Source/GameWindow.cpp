#include "View/GameWindow.h"

#include "AllScoresWidget.h"
#include "DragMeepleLabel.h"
#include "DragTileLabel.h"
#include "LocationUtils.h"
#include "MeepleUtils.h"
#include "QtGlue.h"
#include "UserInfoWidget.h"

#include "View/Meeple.h"

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>
#include <QGraphicsPolygonItem>
#include <QHBoxLayout>
#include <QPainterPath>
#include <QPen>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>

namespace
{
	int const kOffset = 3;
	int const kRadius = 7;
	QColor const kPossibleLocationsColor( 140, 70, 0 );

	QPainterPath
	createPossibleLocationsPath( Utils::Locations const & inLocations )
	{
		QPainterPath path;
		BOOST_FOREACH( Utils::Location const & location, inLocations )
		{
			int const x = View::getX( location );
			int const y = View::getY( location );
			QRectF const rect( x + kOffset, y + kOffset, Gui::kTileWidth - 2 * kOffset, Gui::kTileHeight - 2 * kOffset );
			path.addRoundedRect( rect, kRadius, kRadius );
		}
		return path;
	}
}

struct View::GuiPlacedPiece
{
	QGraphicsPixmapItem* mItem;
	int mX;
	int mY;
	View::Color mColor;
	
	GuiPlacedPiece( QGraphicsPixmapItem* inItem, int inX, int inY, View::Color inColor )
	: mItem( inItem ), mX( inX ), mY( inY ), mColor( inColor ) {}
};

namespace
{
	bool
	HasMembers( View::GuiPlacedPiece const & inPiece, int inX, int inY, View::Color inColor )
	{
		return ( inPiece.mX == inX && inPiece.mY == inY && inPiece.mColor == inColor );
	}
}

View::GameWindow::GameWindow( QWidget *parent )
:
	QMainWindow( parent ),
	mBoardScene(),
	mBoardView(),
	mTiles(),
	mMeeples(),
	mPossibleLocations(),
	mTilesLeft(),
	mPickedTileLabel(),
	mUserInfo(),
	mUserInfoMap(),
	mAllScoresWidget()
{
	this->resize( 800, 500 );
	QWidget * centralWidget = new QWidget( this );

	QHBoxLayout * mainLayout = new QHBoxLayout();
	mainLayout->setContentsMargins( 0, 0, 0, 0 );
	mainLayout->setSpacing( 0 );

	mBoardScene = new QGraphicsScene( centralWidget );

	mBoardView = new BoardView( mBoardScene, this );
	mBoardView->setFrameStyle( QFrame::NoFrame );
	mBoardView->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	mBoardView->setRenderHint( QPainter::Antialiasing );

	connect( mBoardView, SIGNAL( enterPressed() ), this, SLOT( onEndCurrentTurn() ) );
	connect( mBoardView, SIGNAL( spacePressed() ), this, SLOT( onEndCurrentTurn() ) );
	connect( mBoardView, SIGNAL( droppedPiece( Dragging::PieceData, int, int ) ),
		this, SIGNAL( tryToPlacePiece( Dragging::PieceData, int, int ) ) );

	mainLayout->addWidget( mBoardView, 1 );

	connect( mBoardView, SIGNAL( clicked( int, int ) ), this, SIGNAL( clicked( int, int ) ) );
	connect( mBoardView, SIGNAL( droppedTile( int, int, std::string const &, View::Rotation ) ),
		this, SLOT( onDroppedTile( int, int, std::string const &, View::Rotation ) ) );

	QVBoxLayout * sidebarLayout = new QVBoxLayout();
	sidebarLayout->setContentsMargins( 6, 6, 6, 6 );
	sidebarLayout->setSpacing( 0 );

	mTilesLeft = new QLabel(centralWidget);
	mTilesLeft->setText("X tiles left");
	sidebarLayout->addWidget(mTilesLeft);

	mPickedTileLabel = new DragTileLabel( centralWidget );
	sidebarLayout->addWidget( mPickedTileLabel );

	mUserInfo = new QStackedWidget( centralWidget );
	sidebarLayout->addWidget( mUserInfo, 0 );

	QPushButton * endTurnButton = new QPushButton( centralWidget );
	endTurnButton->setText("End Turn");
	connect( endTurnButton, SIGNAL( clicked() ), this, SLOT( onEndCurrentTurn() ) );
	sidebarLayout->addWidget( endTurnButton );

	QLabel * emptyLabel = new QLabel( centralWidget );
	sidebarLayout->addWidget( emptyLabel, 5 );

	mAllScoresWidget = new AllScoresWidget( centralWidget );
	sidebarLayout->addWidget( mAllScoresWidget );

	mainLayout->addLayout( sidebarLayout );

	centralWidget->setLayout( mainLayout );
	setCentralWidget( centralWidget );

//    mMenuBar = new QMenuBar(this);
//    mMenuBar->setGeometry(QRect(0, 0, 800, 21));
//    setMenuBar(mMenuBar);
//    mMainToolBar = new QToolBar(this);
//    addToolBar(Qt::TopToolBarArea, mMainToolBar);
//    mStatusBar = new QStatusBar(this);
//    setStatusBar(mStatusBar);
}

View::GameWindow::~GameWindow()
{
}

void
View::GameWindow::addPlayer
(
	std::string const & inName,
	View::Color inColor
)
{
	UserInfoWidget * newUserInfo = new UserInfoWidget( inName, inColor, mUserInfo );
	mUserInfo->addWidget( newUserInfo );
	mUserInfo->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	mUserInfoMap[inName] = newUserInfo;

	mAllScoresWidget->addPlayer( inName );
}

void
View::GameWindow::setTile
(
	Utils::Location const & inLocation,
	std::string const & inId,
	Rotation inRotation
)
{
	TileItem *item = new TileItem( inId, inRotation );
	item->moveBy( getX( inLocation ), getY( inLocation ) );
	mTiles.push_back( item );
	mBoardScene->addItem( item );
	mBoardView->placeTile( getX( inLocation ), getY( inLocation ), inId, inRotation );
	updateSceneRect();
}

void
View::GameWindow::clearTile(int x, int y)
{
	std::vector< TileItem * >::iterator it = mTiles.end();
	while (it != mTiles.begin())
	{
		--it;
		if ((*it)->scenePos() == QPointF(x, y))
		{
			mBoardScene->removeItem( *it );
			mTiles.erase(it);
			break;
		}
	}
}

void
View::GameWindow::rotateTile( int inX, int inY, std::string const & inId, Rotation inRotation )
{
	for ( std::vector< TileItem * >::reverse_iterator it = mTiles.rbegin();
		it != mTiles.rend();
		++it )
	{
		if ( (*it)->scenePos() == QPointF( inX, inY ) )
		{
			(*it)->setTile( inId, inRotation );
			break;
		}
	}
}

void
View::GameWindow::displayTilesLeft( unsigned inNr )
{
	mTilesLeft->setText( QString::number( inNr ).append( " tiles left." ) );
}

void
View::GameWindow::setActivePlayer( std::string const & inName )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		mUserInfo->setCurrentWidget( it->second );
	}
}

void
View::GameWindow::setScore( std::string const & inName, std::size_t inScore )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		it->second->setScore( inScore );
	}
	mAllScoresWidget->setScore( inName, inScore );
}

void
View::GameWindow::setFollowersLeft( std::string const & inName, std::size_t inNumberOfFollowers )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		it->second->setNumberOfFollowers( inNumberOfFollowers );
	}
}

void
View::GameWindow::enableLargeFollowers( std::string const & inName )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		it->second->enableLargeFollowers();
	}	
}

void
View::GameWindow::setLargeFollowersLeft( std::string const & inName, std::size_t inNumberOfLargeFollowers )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		it->second->setNumberOfLargeFollowers( inNumberOfLargeFollowers );
	}
}

void
View::GameWindow::setNextTile( std::string const & inId )
{
	mPickedTileLabel->setTile( inId );
}

void
View::GameWindow::placePiece( int inX, int inY, View::Meeple const & inMeeple )
{
	QGraphicsPixmapItem * meeple = new QGraphicsPixmapItem( getMeeplePixmap( inMeeple ) );
	meeple->moveBy( inX + Gui::kTileWidth / 2 - Gui::kMeepleWidth / 2, inY + Gui::kTileHeight / 2 - Gui::kMeepleHeight / 2 );
	mBoardScene->addItem( meeple );
	mMeeples.push_back( GuiPlacedPiece( meeple, inX, inY, inMeeple.getColor() ) );
}

void
View::GameWindow::returnPiece( int inX, int inY, View::Meeple const & inMeeple )
{
	std::vector< GuiPlacedPiece >::iterator it = std::find_if
	(
		mMeeples.begin(), mMeeples.end(),
		boost::bind( &HasMembers, _1, inX, inY, inMeeple.getColor() )
	);
	if ( it != mMeeples.end() )
	{
		mBoardScene->removeItem( it->mItem );
		mMeeples.erase( it );
	}
	mBoardView->update();
	update();
}

void
View::GameWindow::setPossibleLocations( Utils::Locations const & inLocations )
{
	if ( mPossibleLocations )
	{
		mBoardScene->removeItem( mPossibleLocations.get() );
	}
	QPainterPath const path = createPossibleLocationsPath( inLocations );
	mPossibleLocations.reset( new QGraphicsPathItem( path ) );
	mPossibleLocations->setPen( Qt::NoPen );
	mPossibleLocations->setBrush( kPossibleLocationsColor );
	mBoardScene->addItem( mPossibleLocations.get() );
}

void
View::GameWindow::fadeNextTile()
{
	mPickedTileLabel->fadeTile();
}

void
View::GameWindow::onEndCurrentTurn()
{
	mBoardView->clearCurrentTile();
	emit endCurrentTurn();
}

void
View::GameWindow::onDroppedTile( int inX, int inY, std::string const & inTileId, View::Rotation inRotation )
{
	emit tileDropped( inX, inY, inTileId, inRotation );
}

void
View::GameWindow::updateSceneRect()
{
	QRectF bounding = mBoardScene->itemsBoundingRect();
	bounding.adjust( -Gui::kTileWidth, -Gui::kTileHeight, Gui::kTileWidth, Gui::kTileHeight );
	mBoardScene->setSceneRect( bounding );
}
