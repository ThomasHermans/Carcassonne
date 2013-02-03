#include "GameWindow.h"

#include "GuiConstants.h"

#include <sstream>

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->resize(800, 500);
    mCentralWidget = new QWidget(this);
    mCentralWidget->setObjectName(QString::fromUtf8("mCentralWidget"));

    mBoardAndSideBarLayout = new QHBoxLayout();
    mBoardAndSideBarLayout->setSpacing(6);
    mBoardAndSideBarLayout->setObjectName(QString::fromUtf8("mBoardAndSideBarLayout"));

    mBoardScene = new QGraphicsScene( mCentralWidget );
    mBoardScene->setObjectName( QString::fromUtf8("mBoardScene") );

    mBoardView = new BoardView( mBoardScene, this );
    mBoardView->setObjectName( QString::fromUtf8("mBoardView") );

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
    mSideBarLayout->setSpacing(6);

    mTilesLeft = new QLabel(mCentralWidget);
    mTilesLeft->setObjectName(QString::fromUtf8("mTilesLeft"));
    mTilesLeft->setText("X tiles left");

    mSideBarLayout->addWidget(mTilesLeft);

    mPickedTileLabel = new QLabel(mCentralWidget);
    mPickedTileLabel->setObjectName(QString::fromUtf8("mPickedTileLabel"));
    mPickedTileLabel->setFixedSize(QSize(100, 100));
    mPickedTileLabel->setText(".");
    mPickedTileLabel->setAlignment(Qt::AlignCenter);

    mSideBarLayout->addWidget(mPickedTileLabel);

    mSideBarLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

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
GameWindow::displayTilesLeft(unsigned int inNr)
{
    mTilesLeft->setText(QString::number(inNr).append(" tiles left."));
}

void
GameWindow::setTile(int inX, int inY, std::string inId, int inRotation)
{
    TileItem *item = new TileItem( inId, inRotation );
    item->moveBy(inX, inY);
    mTiles.push_back( item );
    mBoardScene->addItem( item );
    updateSceneRect(inX, inY);
}

void
GameWindow::setNextTile(std::string inId)
{
    std::stringstream sstr;
    sstr << ":/tiles/" << inId << ".png";
    const QPixmap pm(QString::fromStdString(sstr.str()));
    mPickedTileLabel->setPixmap(pm);
}

void
GameWindow::onClicked(int x, int y)
{
    std::cout << "GameWindow sees a click" << std::endl;
    emit clicked(x, y);
}

void
GameWindow::updateSceneRect(int inX, int inY)
{
    QRectF currentRect = mBoardScene->sceneRect();
    if ((inX <= currentRect.x()) || (inX >= currentRect.x() + currentRect.width() - GuiConstants::tileWidth))
    {
        mBoardScene->setSceneRect(inX - GuiConstants::tileWidth,
                                  currentRect.y(),
                                  currentRect.width() + currentRect.x() - inX + 2 * GuiConstants::tileWidth,
                                  currentRect.height());
    }
    currentRect = mBoardScene->sceneRect();
    if ((inY <= currentRect.y()) || (inY >= currentRect.y() + currentRect.height() - GuiConstants::tileHeight))
    {
        mBoardScene->setSceneRect(currentRect.x(),
                                  currentRect.y() - GuiConstants::tileHeight,
                                  currentRect.width(),
                                  currentRect.height() + inY - currentRect.y() + 2 * GuiConstants::tileHeight);
    }
}