#include "View/GameWindow.h"

#include "AllScoresWidget.h"
#include "DragMeepleLabel.h"
#include "UserInfoWidget.h"

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QPen>
#include <QStackedWidget>

#include <iostream>
#include <sstream>

namespace
{

QPixmap
getPixmapFromId( std::string inId )
{
	std::stringstream sstr;
	sstr << ":/tiles/" << inId << ".png";
	const QPixmap pm(QString::fromStdString(sstr.str()));
	return pm;
}

} // end of nameless namespace

GameWindow::GameWindow( QWidget *parent )
:
	QMainWindow( parent )
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

	mBoardView = new BoardView( mBoardScene, this );
	mBoardView->setObjectName( QString::fromUtf8("mBoardView") );
	mBoardView->setFrameStyle( QFrame::NoFrame );
	mBoardView->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

	connect( mBoardView, SIGNAL( enterPressed() ), this, SIGNAL( endCurrentTurn() ) );
	connect( mBoardView, SIGNAL( spacePressed() ), this, SIGNAL( endCurrentTurn() ) );
	connect( mBoardView, SIGNAL( dropped( DragData, int, int ) ),
		this, SIGNAL( tryToPlacePiece( DragData, int, int ) ) );

	boardAndSideBarLayout->addWidget( mBoardView, 1 );

	connect( mBoardView, SIGNAL(clicked(int,int)), this, SLOT(onClicked(int,int)) );

	mSideBarLayout = new QVBoxLayout();
	mSideBarLayout->setObjectName(QString::fromUtf8("mSideBarLayout"));
	mSideBarLayout->setContentsMargins( 6, 6, 6, 6 );
	mSideBarLayout->setSpacing( 0 );

	mTilesLeft = new QLabel(centralWidget);
	mTilesLeft->setObjectName(QString::fromUtf8("mTilesLeft"));
	mTilesLeft->setText("X tiles left");
	mSideBarLayout->addWidget(mTilesLeft);

	mPickedTileLabel = new QLabel(centralWidget);
	mPickedTileLabel->setObjectName(QString::fromUtf8("mPickedTileLabel"));
	mPickedTileLabel->setFixedSize( QSize(100, 100) );
	mPickedTileLabel->setText(".");
	mPickedTileLabel->setAlignment(Qt::AlignCenter);
	mSideBarLayout->addWidget(mPickedTileLabel);

	mUserInfo = new QStackedWidget( centralWidget );
	mSideBarLayout->addWidget( mUserInfo, 0 );

	mEndTurnButton = new QPushButton(centralWidget);
	mEndTurnButton->setObjectName(QString::fromUtf8("mEndTurnButton"));
	mEndTurnButton->setText("End Turn");
	connect( mEndTurnButton, SIGNAL( clicked() ), this, SIGNAL( endCurrentTurn() ) );
	mSideBarLayout->addWidget( mEndTurnButton );

	mSideBarLayout->addStretch();

	mAllScoresWidget = new AllScoresWidget( centralWidget );
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

GameWindow::~GameWindow()
{
}

void
GameWindow::addPlayer
(
	std::string const & inName,
	Dragging::Color inColor,
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
GameWindow::clearTile(int x, int y)
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
GameWindow::rotateTile(int x, int y, std::string inId, int inRotation)
{
	std::vector< TileItem * >::iterator it = mTiles.end();
	while (it != mTiles.begin())
	{
		--it;
		if ((*it)->scenePos() == QPointF(x, y))
		{
			QPixmap pixmap = getPixmapFromId( inId );
			QTransform rotation = QTransform();
			rotation.rotate( inRotation );
			pixmap = pixmap.transformed( rotation );
			(*it)->setPixmap( pixmap );
			break;
		}
	}
}

void
GameWindow::displayTilesLeft( unsigned int inNr )
{
	mTilesLeft->setText( QString::number( inNr ).append( " tiles left." ) );
}

void
GameWindow::setActivePlayer( std::string const & inName )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		mUserInfo->setCurrentWidget( it->second );
	}
}

void
GameWindow::setScore( std::string const & inName, unsigned inScore )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		it->second->setScore( inScore );
	}
	mAllScoresWidget->setScore( inName, inScore );
}

void
GameWindow::setFollowersLeft( std::string const & inName, unsigned inNumberOfFollowers )
{
	std::map< std::string, UserInfoWidget * >::iterator it = mUserInfoMap.find( inName );
	if ( it != mUserInfoMap.end() )
	{
		it->second->setNumberOfFollowers( inNumberOfFollowers );
	}
}

void
GameWindow::finishCloister(int inX, int inY)
{
	QGraphicsEllipseItem* circle = new QGraphicsEllipseItem( inX, inY, 100, 100 );
	circle->setPen( QPen( QBrush( Qt::blue ), 2 ) );
	mBoardScene->addItem( circle );
}

void
GameWindow::setTile(int inX, int inY, std::string inId, int inRotation)
{
	QPixmap pixmap = getPixmapFromId( inId );
	QTransform rotation = QTransform();
	rotation.rotate( inRotation );
	pixmap = pixmap.transformed( rotation );
	TileItem *item = new TileItem( pixmap );
	item->moveBy(inX, inY);
	mTiles.push_back( item );
	mBoardScene->addItem( item );
	updateSceneRect();
}

void
GameWindow::setNextTile(std::string inId)
{
	QPixmap pixmap = getPixmapFromId( inId );
	mPickedTileLabel->setPixmap( pixmap );
}

void
GameWindow::fadeNextTile()
{
	if ( mPickedTileLabel->pixmap() == 0)
		return;
	QPixmap pixmap( *mPickedTileLabel->pixmap() );
	QPainter painter( &pixmap );
	QPen pen(Qt::black, 2, Qt::SolidLine);
	painter.setPen(pen);
	painter.drawLine( 0, pixmap.height(), pixmap.width(), 0 );
	mPickedTileLabel->setPixmap( pixmap );
}

void
GameWindow::onClicked(int x, int y)
{
	std::cout << "GameWindow sees a click" << std::endl;
	emit clicked(x, y);
}

void
GameWindow::placePiece( int inX, int inY, QColor inColor )
{
	QVector< QPointF > points;
	points << QPointF( 0, 0 ) << QPointF( -5, -25 ) << QPointF( 10, -35 ) << QPointF( 25, -25 ) << QPointF( 20, 0 ) << QPointF( 0, 0 );
	QPolygonF polygon( points );
	QGraphicsPolygonItem* meeple = new QGraphicsPolygonItem( polygon );
	meeple->moveBy( inX + Gui::kTileWidth / 2 - 12, inY + Gui::kTileHeight / 2 + 17 );
	meeple->setPen( QPen( QBrush( inColor ), 2 ) );
	mBoardScene->addItem( meeple );
	mMeeples.push_back( GuiPlacedPiece( meeple, inX, inY, inColor ) );
}

void
GameWindow::returnPiece( int inX, int inY, QColor inColor )
{
	// QList< QGraphicsItem * > items = mBoardScene->items( QPointF( inX, inY ), Qt::IntersectsItemShape, Qt::AscendingOrder );
	for ( std::vector< GuiPlacedPiece >::iterator it = mMeeples.begin(); it != mMeeples.end(); ++it )
	{
		if ( it->mX == inX && it->mY == inY && it->mColor == inColor )
		{
			mBoardScene->removeItem( it->mItem );
			mMeeples.erase( it );
			break;
		}
	}
}

void
GameWindow::updateSceneRect()
{
	QRectF bounding = mBoardScene->itemsBoundingRect();
	bounding.adjust( -Gui::kTileWidth, -Gui::kTileHeight, Gui::kTileWidth, Gui::kTileHeight );
	mBoardScene->setSceneRect( bounding );
}
