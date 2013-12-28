#include "View/GameWindow.h"

#include "AllScoresWidget.h"
#include "DragMeepleLabel.h"
#include "DragTileLabel.h"
#include "MeepleUtils.h"
#include "QtGlue.h"
#include "UserInfoWidget.h"

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QPen>
#include <QStackedWidget>

#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>
#include <sstream>

struct GuiPlacedPiece
{
	QGraphicsPixmapItem* mItem;
	int mX;
	int mY;
	View::Color mColor;
	
	GuiPlacedPiece( QGraphicsPixmapItem* inItem, int inX, int inY, View::Color inColor );
};

namespace
{
	bool
	HasMembers( GuiPlacedPiece const & inPiece, int inX, int inY, View::Color inColor )
	{
		return ( inPiece.mX == inX && inPiece.mY == inY && inPiece.mColor == inColor );
	}
}

GuiPlacedPiece::GuiPlacedPiece( QGraphicsPixmapItem * inItem, int inX, int inY, View::Color inColor )
:
	mItem( inItem ),
	mX( inX ),
	mY( inY ),
	mColor( inColor )
{}

View::GameWindow::GameWindow( QWidget *parent )
:
	QMainWindow( parent ),
	mBoardScene(),
	mBoardView(),
	mTiles(),
	mMeeples(),
	mSideBarLayout(),
	mTilesLeft(),
	mPickedTileLabel(),
	mUserInfo(),
	mUserInfoMap(),
	mEndTurnButton(),
	mAllScoresWidget()
{
	this->resize( 800, 500 );
	QWidget * centralWidget = new QWidget( this );
	centralWidget->setObjectName( QString::fromUtf8( "centralWidget" ) );

	QHBoxLayout * boardAndSideBarLayout = new QHBoxLayout();
	boardAndSideBarLayout->setContentsMargins( 0, 0, 0, 0 );
	boardAndSideBarLayout->setSpacing( 0 );
	boardAndSideBarLayout->setObjectName(QString::fromUtf8("boardAndSideBarLayout"));

	mBoardScene = new QGraphicsScene( centralWidget );
	mBoardScene->setObjectName( QString::fromUtf8("mBoardScene") );

	mBoardView = new View::BoardView( mBoardScene, this );
	mBoardView->setObjectName( QString::fromUtf8("mBoardView") );
	mBoardView->setFrameStyle( QFrame::NoFrame );
	mBoardView->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

	connect( mBoardView, SIGNAL( enterPressed() ), this, SLOT( onEndCurrentTurn() ) );
	connect( mBoardView, SIGNAL( spacePressed() ), this, SLOT( onEndCurrentTurn() ) );
	connect( mBoardView, SIGNAL( droppedPiece( Dragging::PieceData, int, int ) ),
		this, SIGNAL( tryToPlacePiece( Dragging::PieceData, int, int ) ) );

	boardAndSideBarLayout->addWidget( mBoardView, 1 );

	connect( mBoardView, SIGNAL( clicked( int, int ) ), this, SLOT( onClicked( int, int ) ) );
	connect( mBoardView, SIGNAL( droppedTile( int, int, std::string const &, View::Rotation ) ),
		this, SLOT( onDroppedTile( int, int, std::string const &, View::Rotation ) ) );

	mSideBarLayout = new QVBoxLayout();
	mSideBarLayout->setObjectName(QString::fromUtf8("mSideBarLayout"));
	mSideBarLayout->setContentsMargins( 6, 6, 6, 6 );
	mSideBarLayout->setSpacing( 0 );

	mTilesLeft = new QLabel(centralWidget);
	mTilesLeft->setObjectName(QString::fromUtf8("mTilesLeft"));
	mTilesLeft->setText("X tiles left");
	mSideBarLayout->addWidget(mTilesLeft);

	mPickedTileLabel = new View::DragTileLabel( centralWidget );
	mPickedTileLabel->setObjectName( QString::fromUtf8( "mPickedTileLabel" ) );
	mSideBarLayout->addWidget( mPickedTileLabel );

	mUserInfo = new QStackedWidget( centralWidget );
	mSideBarLayout->addWidget( mUserInfo, 0 );

	mEndTurnButton = new QPushButton(centralWidget);
	mEndTurnButton->setObjectName(QString::fromUtf8("mEndTurnButton"));
	mEndTurnButton->setText("End Turn");
	connect( mEndTurnButton, SIGNAL( clicked() ), this, SLOT( onEndCurrentTurn() ) );
	mSideBarLayout->addWidget( mEndTurnButton );

	mSideBarLayout->addStretch();

	mAllScoresWidget = new View::AllScoresWidget( centralWidget );
	mAllScoresWidget->setObjectName( QString::fromUtf8( "mAllScoresWidget" ) );
	mSideBarLayout->addWidget( mAllScoresWidget );

	boardAndSideBarLayout->addLayout(mSideBarLayout);

	centralWidget->setLayout(boardAndSideBarLayout);
	setCentralWidget(centralWidget);

//    mMenuBar = new QMenuBar(this);
//    mMenuBar->setObjectName(QString::fromUtf8("mMenuBar"));
//    mMenuBar->setGeometry(QRect(0, 0, 800, 21));
//    setMenuBar(mMenuBar);
//    mMainToolBar = new QToolBar(this);
//    mMainToolBar->setObjectName(QString::fromUtf8("mMainToolBar"));
//    addToolBar(Qt::TopToolBarArea, mMainToolBar);
//    mStatusBar = new QStatusBar(this);
//    mStatusBar->setObjectName(QString::fromUtf8("mStatusBar"));
//    setStatusBar(mStatusBar);
}

View::GameWindow::~GameWindow()
{
}

void
View::GameWindow::addPlayer
(
	std::string const & inName,
	View::Color inColor,
	unsigned inNumberOfFollowers
)
{
	UserInfoWidget * newUserInfo = new UserInfoWidget( inName, inColor, inNumberOfFollowers, mUserInfo );
	mUserInfo->addWidget( newUserInfo );
	mUserInfo->setFixedHeight( newUserInfo->sizeHint().height() );
	mUserInfoMap[inName] = newUserInfo;

	mAllScoresWidget->addPlayer( inName );
}

void
View::GameWindow::setTile( int inX, int inY, std::string const & inId, Rotation inRotation )
{
	TileItem *item = new TileItem( inId, inRotation );
	item->moveBy( inX, inY );
	mTiles.push_back( item );
	mBoardScene->addItem( item );
	mBoardView->placeTile( inX, inY, inId, inRotation );
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
View::GameWindow::setScore( std::string const & inName, unsigned inScore )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		it->second->setScore( inScore );
	}
	mAllScoresWidget->setScore( inName, inScore );
}

void
View::GameWindow::setFollowersLeft( std::string const & inName, unsigned inNumberOfFollowers )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		it->second->setNumberOfFollowers( inNumberOfFollowers );
	}
}

void
View::GameWindow::setNextTile( std::string const & inId )
{
	mPickedTileLabel->setTile( inId );
}

void
View::GameWindow::placePiece( int inX, int inY, View::Color inColor )
{
	QGraphicsPixmapItem * meeple = new QGraphicsPixmapItem( getMeeplePixmap( inColor ) );
	meeple->moveBy( inX + Gui::kTileWidth / 2 - Gui::kMeepleWidth / 2, inY + Gui::kTileHeight / 2 - Gui::kMeepleHeight / 2 );
	mBoardScene->addItem( meeple );
	mMeeples.push_back( GuiPlacedPiece( meeple, inX, inY, inColor ) );
}

void
View::GameWindow::returnPiece( int inX, int inY, View::Color inColor )
{
	std::vector< GuiPlacedPiece >::iterator it = std::find_if
	(
		mMeeples.begin(), mMeeples.end(),
		boost::bind( &HasMembers, _1, inX, inY, inColor )
	);
	if ( it != mMeeples.end() )
	{
		mBoardScene->removeItem( it->mItem );
		mMeeples.erase( it );
	}
	mBoardView->update();
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
View::GameWindow::onClicked( int inX, int inY )
{
	emit clicked( inX, inY, mPickedTileLabel->getCurrentTile(), mPickedTileLabel->getCurrentRotation() );
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