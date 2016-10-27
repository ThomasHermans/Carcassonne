#include "View/GameWindow.h"

#include "AllScoresWidget.h"
#include "DragMeepleLabel.h"
#include "DragTileLabel.h"
#include "LocationUtils.h"
#include "ModelViewGlue.h"
#include "MeepleUtils.h"
#include "QtGlue.h"
#include "TileUtils.h"
#include "UserInfoWidget.h"

#include "View/DragData.h"
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
	std::unique_ptr< QGraphicsPixmapItem > mItem;
	int mX;
	int mY;
	Color mColor;
	bool isRemoved;
	
	GuiPlacedPiece( QGraphicsPixmapItem * inItem, int inX, int inY, Color inColor )
	: mItem( inItem ), mX( inX ), mY( inY ), mColor( inColor ), isRemoved( false ) {}
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
	mLastPlacedTiles(),
	mPossibleLocations(),
	mTilesLeft(),
	mPickedTileLabel(),
	mMessageLabel(),
	mUserInfo(),
	mUserInfoMap(),
	mAllScoresWidget(),
	mShowRecentTileBorders( true ),
	mShowRemovedMeeple( true )
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
		this, SLOT( onDroppedPiece( Dragging::PieceData, int, int ) ) );
	connect( mBoardView, SIGNAL( clicked( int, int ) ), this, SIGNAL( clicked( int, int ) ) );
	connect( mBoardView, SIGNAL( droppedTile( int, int, std::string const &, View::Rotation ) ),
		this, SLOT( onDroppedTile( int, int, std::string const &, View::Rotation ) ) );

	mainLayout->addWidget( mBoardView, 1 );

	QVBoxLayout * sidebarLayout = new QVBoxLayout();
	sidebarLayout->setContentsMargins( 6, 6, 6, 6 );
	sidebarLayout->setSpacing( 0 );

	mTilesLeft = new QLabel(centralWidget);
	mTilesLeft->setText("X tiles left");
	sidebarLayout->addWidget(mTilesLeft);

	mPickedTileLabel = new DragTileLabel( centralWidget );
	sidebarLayout->addWidget( mPickedTileLabel );

	mMessageLabel = new QLabel( this );
	sidebarLayout->addWidget( mMessageLabel );

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

	QMenuBar * theMenuBar = menuBar();
	QMenu * viewMenu = theMenuBar->addMenu( "View" );
	QAction * showRecentTileBordersAction = viewMenu->addAction( "Show Recently Placed Tiles" );
	showRecentTileBordersAction->setCheckable( true );
	showRecentTileBordersAction->setChecked( mShowRecentTileBorders );
	connect( showRecentTileBordersAction, &QAction::triggered, [this](){ toggleRecentTileBorders(); } );
	QAction * showRemovedMeepleAction = viewMenu->addAction( "Show Removed Meeple" );
	showRemovedMeepleAction->setCheckable( true );
	showRemovedMeepleAction->setChecked( mShowRemovedMeeple );
	connect( showRemovedMeepleAction, &QAction::triggered, [this](){ toggleRemovedMeeple(); } );
}

View::GameWindow::~GameWindow()
{
}

void
View::GameWindow::addPlayer( std::string const & inName, Color inColor )
{
	UserInfoWidget * newUserInfo = new UserInfoWidget( inName, inColor, mUserInfo );
	mUserInfo->addWidget( newUserInfo );
	mUserInfo->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	mUserInfoMap[inName] = newUserInfo;

	mAllScoresWidget->addPlayer( inName, inColor );
}

void
View::GameWindow::setTile
(
	Utils::Location const & inLocation,
	std::string const & inId,
	Rotation inRotation,
	boost::optional< Color > const & inColor
)
{
	TileItem *item = new TileItem( inId, inRotation );
	item->moveBy( getX( inLocation ), getY( inLocation ) );
	mTiles.push_back( item );
	mBoardScene->addItem( item );
	mBoardView->placeTile( getX( inLocation ), getY( inLocation ), inId, inRotation );
	updateSceneRect();

	// Add the "last placed tile" marker around this tile.
	if ( inColor )
	{
		Color const color = *inColor;
		auto lastPlacedTile = std::find_if
		(
			mLastPlacedTiles.begin(), mLastPlacedTiles.end(),
			[ color ]( std::pair< Color, QGraphicsPathItem * > const & inPlacedTile )
			{ return inPlacedTile.first == color; }
		);
		if ( lastPlacedTile != mLastPlacedTiles.end() )
		{
			mBoardScene->removeItem( lastPlacedTile->second );
			mLastPlacedTiles.erase( lastPlacedTile );
		}
		QGraphicsPathItem * placedTile = new QGraphicsPathItem( getLastPlacedTilePath() );
		placedTile->setPen( Qt::NoPen );
		QColor theQColor = toQColor( color );
		theQColor.setAlphaF( .6f );
		placedTile->setBrush( theQColor );
		placedTile->moveBy( getX( inLocation ), getY( inLocation ) );
		placedTile->setVisible( mShowRecentTileBorders );
		mBoardScene->addItem( placedTile );
		mLastPlacedTiles.push_back( std::make_pair( color, placedTile ) );
	}
}

void
View::GameWindow::clearTile( Utils::Location const & inLocation )
{
	QPointF const position( getX( inLocation ), getY( inLocation ) );
	std::vector< TileItem * >::iterator it = mTiles.end();
	while ( it != mTiles.begin() )
	{
		--it;
		if ( (*it)->scenePos() == position )
		{
			mBoardScene->removeItem( *it );
			mTiles.erase( it );
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
View::GameWindow::setPlayerSupply( std::string const & inName, Meeple::MeepleType inMeepleType, std::size_t inAmount )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	assert( it != mUserInfoMap.end() );
	it->second->setSupply( inMeepleType, inAmount );
}

void
View::GameWindow::setNextTile( std::string const & inId )
{
	mPickedTileLabel->setTile( inId );
}

void
View::GameWindow::placePiece( int inX, int inY, Meeple const & inMeeple )
{
	QGraphicsPixmapItem * meeple = new QGraphicsPixmapItem( getMeeplePixmap( inMeeple ) );
	meeple->moveBy( inX + Gui::kTileWidth / 2 - Gui::kMeepleWidth / 2, inY + Gui::kTileHeight / 2 - Gui::kMeepleHeight / 2 );
	mBoardScene->addItem( meeple );
	mMeeples.push_back( GuiPlacedPiece( meeple, inX, inY, inMeeple.getColor() ) );
}

void
View::GameWindow::removePiece( int inX, int inY, Meeple const & inMeeple )
{
	std::vector< GuiPlacedPiece >::iterator it = std::find_if
	(
		mMeeples.begin(), mMeeples.end(),
		boost::bind( &HasMembers, _1, inX, inY, inMeeple.getColor() )
	);
	assert( it != mMeeples.end() );
	if ( it != mMeeples.end() )
	{
		it->mItem.reset( new QGraphicsPixmapItem( getRemovedMeeplePixmap( inMeeple ) ) );
		it->mItem->moveBy( inX + Gui::kTileWidth / 2 - Gui::kMeepleWidth / 2, inY + Gui::kTileHeight / 2 - Gui::kMeepleHeight / 2 );
		it->isRemoved = true;
		it->mItem->setVisible( mShowRemovedMeeple );
		mBoardScene->addItem( it->mItem.get() );
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
View::GameWindow::showMessage( std::string const & inMessage )
{
	mMessageLabel->setText( QString::fromStdString( inMessage ) );
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
	noPiecePlaced();
}

void
View::GameWindow::onDroppedTile( int inX, int inY, std::string const & inTileId, View::Rotation inRotation )
{
	emit tileDropped( inX, inY, inTileId, inRotation );

	Utils::Location const location( Controller::rowFromY( inY ), Controller::colFromX( inX ) );
	tilePlaced( location, inTileId, inRotation );
}

void
View::GameWindow::onDroppedPiece( Dragging::PieceData const & inData, int inX, int inY )
{
	piecePlaced( inX, inY, inData.getMeeple() );
}

void
View::GameWindow::updateSceneRect()
{
	QRectF bounding = mBoardScene->itemsBoundingRect();
	int const widthAdjustment = 10 * Gui::kTileWidth;
	int const heightAdjustment = 10 * Gui::kTileHeight;
	bounding.adjust( -widthAdjustment, -heightAdjustment, widthAdjustment, heightAdjustment );
	mBoardScene->setSceneRect( bounding );
}

void
View::GameWindow::toggleRecentTileBorders()
{
	mShowRecentTileBorders = !mShowRecentTileBorders;
	for ( auto const & lastPlacedTile : mLastPlacedTiles )
	{
		lastPlacedTile.second->setVisible( mShowRecentTileBorders );
	}
}

void
View::GameWindow::toggleRemovedMeeple()
{
	mShowRemovedMeeple = !mShowRemovedMeeple;
	for ( GuiPlacedPiece const & placedPiece : mMeeples )
	{
		if ( placedPiece.isRemoved )
		{
			placedPiece.mItem->setVisible( mShowRemovedMeeple );
		}
	}
}
