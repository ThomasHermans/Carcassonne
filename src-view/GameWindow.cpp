#include "src-view/GameWindow.h"

#include "src-view/GuiConstants.h"

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QPen>

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

GameWindow::GameWindow(QWidget *parent) :
	QMainWindow(parent)
{
	this->resize(800, 500);
	mCentralWidget = new QWidget(this);
	mCentralWidget->setObjectName(QString::fromUtf8("mCentralWidget"));

	mBoardAndSideBarLayout = new QHBoxLayout();
	mBoardAndSideBarLayout->setContentsMargins( 0, 0, 0, 0 );
	mBoardAndSideBarLayout->setSpacing( 0 );
	mBoardAndSideBarLayout->setObjectName(QString::fromUtf8("mBoardAndSideBarLayout"));

	mBoardScene = new QGraphicsScene( mCentralWidget );
	mBoardScene->setObjectName( QString::fromUtf8("mBoardScene") );

	mBoardView = new BoardView( mBoardScene, this );
	mBoardView->setObjectName( QString::fromUtf8("mBoardView") );
	mBoardView->setFrameStyle( QFrame::NoFrame );

//    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    sizePolicy.setHorizontalStretch(0);
//    sizePolicy.setVerticalStretch(0);
//    sizePolicy.setHeightForWidth(mBoardScrollArea->sizePolicy().hasHeightForWidth());
//    mBoardScrollArea->setSizePolicy(sizePolicy);
//    mBoardScrollArea->setMinimumSize(QSize(400, 400));
//    mBoardScrollArea->setBaseSize(QSize(100, 100));
//    mBoardScrollArea->setWidgetResizable(true);

//    mBoardWidget = new BoardWidget();
//    mBoardWidget->setObjectName(QString::fromUtf8("mBoardWidget"));
//    mBoardWidget->setGeometry(QRect(0, 0, 400, 400));
//    mBoardScrollArea->setWidget(mBoardWidget);

//    connect(mBoardWidget, SIGNAL(clicked(uint, uint)), this, SLOT(onClicked(uint, uint)));

	mBoardAndSideBarLayout->addWidget( mBoardView );

	connect( mBoardView, SIGNAL(clicked(int,int)), this, SLOT(onClicked(int,int)) );

	mSideBarLayout = new QVBoxLayout();
	mSideBarLayout->setObjectName(QString::fromUtf8("mSideBarLayout"));
	mSideBarLayout->setContentsMargins( 6, 6, 6, 6 );
	mSideBarLayout->setSpacing( 0 );

	mTilesLeft = new QLabel(mCentralWidget);
	mTilesLeft->setObjectName(QString::fromUtf8("mTilesLeft"));
	mTilesLeft->setText("X tiles left");

	mSideBarLayout->addWidget(mTilesLeft);

	mPickedTileLabel = new QLabel(mCentralWidget);
	mPickedTileLabel->setObjectName(QString::fromUtf8("mPickedTileLabel"));
	mPickedTileLabel->setFixedSize( QSize(100, 100) );
	mPickedTileLabel->setText(".");
	mPickedTileLabel->setAlignment(Qt::AlignCenter);

	mSideBarLayout->addWidget(mPickedTileLabel);

	mActiveUserNameLabel = new QLabel(mCentralWidget);
	mActiveUserNameLabel->setObjectName(QString::fromUtf8("mActiveUserNameLabel"));
	mActiveUserNameLabel->setText("Active user name");
	mActiveUserNameLabel->setAlignment(Qt::AlignLeft);

	mSideBarLayout->addWidget(mActiveUserNameLabel);

	mActiveUserScoreLabel = new QLabel(mCentralWidget);
	mActiveUserScoreLabel->setObjectName(QString::fromUtf8("mActiveUserScoreLabel"));
	mActiveUserScoreLabel->setText("Active user score");
	mActiveUserScoreLabel->setAlignment(Qt::AlignLeft);

	mSideBarLayout->addWidget(mActiveUserScoreLabel);

	mActiveUserMeepleLeftLabel = new QLabel(mCentralWidget);
	mActiveUserMeepleLeftLabel->setObjectName(QString::fromUtf8("mActiveUserMeepleLeftLabel"));
	mActiveUserMeepleLeftLabel->setText("Active user meeple left");
	mActiveUserMeepleLeftLabel->setAlignment(Qt::AlignLeft);

	mSideBarLayout->addWidget(mActiveUserMeepleLeftLabel);

	mEndTurnButton = new QPushButton(mCentralWidget);
	mEndTurnButton->setObjectName(QString::fromUtf8("mEndTurnButton"));
	mEndTurnButton->setText("End Turn");
	
	connect( mEndTurnButton, SIGNAL( clicked() ), this, SIGNAL( endCurrentTurn() ) );
	mSideBarLayout->addWidget(mEndTurnButton);

	mTryToPlacePieceButton = new QPushButton(mCentralWidget);
	mTryToPlacePieceButton->setObjectName(QString::fromUtf8("mTryToPlacePieceButton"));
	mTryToPlacePieceButton->setText("Try to place a Piece");

	connect( mTryToPlacePieceButton, SIGNAL( clicked() ), this, SIGNAL( tryToPlacePiece() ) );
	mSideBarLayout->addWidget( mTryToPlacePieceButton );

	mSideBarLayout->addStretch();

	mBoardAndSideBarLayout->addLayout(mSideBarLayout);

	mCentralWidget->setLayout(mBoardAndSideBarLayout);
	setCentralWidget(mCentralWidget);

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
GameWindow::displayTilesLeft(unsigned int inNr)
{
	mTilesLeft->setText(QString::number(inNr).append(" tiles left."));
}

void
GameWindow::setActivePlayer(std::string const & inName)
{
	mActiveUserNameLabel->setText(QString::fromUtf8(inName.c_str()));
	mActiveUserScoreLabel->setText("Active user score");
	mActiveUserMeepleLeftLabel->setText("Active user meeple left");
}

void
GameWindow::setActivePlayer(std::string const & inName, int inScore, int inPiecesLeft)
{
	mActiveUserNameLabel->setText(QString::fromUtf8(inName.c_str()));
	mActiveUserScoreLabel->setText(QString::number(inScore));
	setMeepleLeft( inPiecesLeft );
}

void
GameWindow::setMeepleLeft( int inMeepleLeft )
{
	mActiveUserMeepleLeftLabel->setText( QString::number( inMeepleLeft ).append( " meeple left." ) );
}

void
GameWindow::finishCloister(int inX, int inY)
{
	QGraphicsEllipseItem* circle = new QGraphicsEllipseItem( inX, inY, 100, 100 );
	circle->setPen( QPen( QBrush( Qt::blue ), 2 ) );
	mBoardScene->addItem( circle );
}

void
GameWindow::finishCity(int inLeft, int inRight, int inTop, int inBottom)
{
	QGraphicsEllipseItem* city = new QGraphicsEllipseItem( inLeft, inTop, inRight - inLeft, inBottom - inTop);
	city->setPen( QPen( QBrush( Qt::red ), 2 ) );
	mBoardScene->addItem( city );
}

void
GameWindow::finishRoad(int inLeft, int inRight, int inTop, int inBottom)
{
	QGraphicsEllipseItem* city = new QGraphicsEllipseItem( inLeft, inTop, inRight - inLeft, inBottom - inTop);
	city->setPen( QPen( QBrush( Qt::gray ), 2 ) );
	mBoardScene->addItem( city );
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
	meeple->moveBy( inX + GuiConstants::tileWidth / 2 - 12, inY + GuiConstants::tileHeight / 2 + 17 );
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
	bounding.adjust( -GuiConstants::tileWidth, -GuiConstants::tileHeight, GuiConstants::tileWidth, GuiConstants::tileHeight );
	mBoardScene->setSceneRect( bounding );
}
