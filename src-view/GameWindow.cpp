#include "src-view/GameWindow.h"

#include "src-view/GuiConstants.h"

#include <QBrush>
#include <QGraphicsEllipseItem>
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

    mSubmitTileButton = new QPushButton(mCentralWidget);
    mSubmitTileButton->setObjectName(QString::fromUtf8("mSubmitTileButton"));
    mSubmitTileButton->setText("Submit Tile");

    connect( mSubmitTileButton, SIGNAL( clicked() ), this, SIGNAL( submitCurrentTile() ) );
    mSideBarLayout->addWidget(mSubmitTileButton);

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
GameWindow::updateSceneRect()
{
    QRectF bounding = mBoardScene->itemsBoundingRect();
    bounding.adjust( -GuiConstants::tileWidth, -GuiConstants::tileHeight, GuiConstants::tileWidth, GuiConstants::tileHeight );
    mBoardScene->setSceneRect( bounding );
}
