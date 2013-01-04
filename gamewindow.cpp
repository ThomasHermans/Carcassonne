#include "gamewindow.h"

#include <sstream>

#include "boardwidget.h"

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mGame = Game();
    this->resize(800, 500);
    mCentralWidget = new QWidget(this);
    mCentralWidget->setObjectName(QString::fromUtf8("mCentralWidget"));

    mBoardAndSideBarLayout = new QHBoxLayout();
    mBoardAndSideBarLayout->setSpacing(6);
    mBoardAndSideBarLayout->setObjectName(QString::fromUtf8("mBoardAndSideBarLayout"));

    mBoardScrollArea = new QScrollArea(mCentralWidget);
    mBoardScrollArea->setObjectName(QString::fromUtf8("mBoardScrollArea"));

    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(mBoardScrollArea->sizePolicy().hasHeightForWidth());
    mBoardScrollArea->setSizePolicy(sizePolicy);
    mBoardScrollArea->setMinimumSize(QSize(400, 400));
    mBoardScrollArea->setBaseSize(QSize(100, 100));
    mBoardScrollArea->setWidgetResizable(true);

    scrollAreaWidgetContents = new BoardWidget(this);
    scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
    scrollAreaWidgetContents->setGeometry(QRect(0, 0, 400, 400));
    mBoardScrollArea->setWidget(scrollAreaWidgetContents);

    mBoardAndSideBarLayout->addWidget(mBoardScrollArea);

    mSideBarLayout = new QVBoxLayout();
    mSideBarLayout->setSpacing(6);
    mSideBarLayout->setObjectName(QString::fromUtf8("mSideBarLayout"));

    mPickedTileLabel = new QLabel(mCentralWidget);
    mPickedTileLabel->setObjectName(QString::fromUtf8("mPickedTileLabel"));
    mPickedTileLabel->setMinimumSize(QSize(100, 100));
    std::string id = mGame.getNextTile()->getID();
    std::stringstream name;
    name << ":/tiles/" << id << ".png";
    QPixmap pm = QPixmap(QString::fromStdString(name.str()));
    mPickedTileLabel->setPixmap(pm);
    mPickedTileLabel->setAlignment(Qt::AlignCenter);

    mSideBarLayout->addWidget(mPickedTileLabel);

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

Board
GameWindow::getBoard() const
{
    return mGame.getBoard();
}
